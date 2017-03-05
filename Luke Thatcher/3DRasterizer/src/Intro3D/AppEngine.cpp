
#include "stdafx.h"
#include "AppEngine.h"
#include "Matrix4x4.h"
#include "MD2Loader.h"
#include <assert.h>
#include <GdiPlus.h>

using namespace Gdiplus;

AppEngine::AppEngine(HWND hWnd, UINT numWorkerThreads)
{
	this->hWnd = hWnd;
	this->rasterizer = new Rasterizer((DemoEngine*)this);
	this->backBufferDepth = NULL;
	this->backBufferTarget = NULL;
	this->numWorkerThreads = numWorkerThreads;
	this->currentWorkType = None;

	// Create the rasterizer for this window
	CreateBackBuffer();

	// Create the wait semaphore to synchronize worker threads
	this->workerWaitSemaphore = CreateSemaphore(NULL, 0, numWorkerThreads, NULL);

	// Create the main thread wait handle, used to signal the main thread when all worker threads are complete
	this->mainThreadWaitHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

	// Allocate a single 32-bit aligned UINT to hold the count of running worker threads
	this->runningWorkerThreadCount = (UINT*)_aligned_malloc(sizeof(UINT), 32);

	// Initialize array of start data for each thread
	threadStartDataArray = new AppEngineThreadStartData[numWorkerThreads];
	for (UINT i = 0; i < numWorkerThreads; i++)
	{
		threadStartDataArray[i].owner = this;
		threadStartDataArray[i].threadIndex = i;
	}

	// Spawn the worker threads
	threadArray = new HANDLE[numWorkerThreads];
	for (UINT i = 0; i < numWorkerThreads; i++)
	{
		threadArray[i] = CreateThread(NULL, 0, &WorkerThreadEntry, &threadStartDataArray[i], 0, NULL);
	}
}
AppEngine::~AppEngine(void)
{
	if (threadArray)
	{
		// Tell the worker threads to terminate
		// Wait until they have all stopped
		DispatchAndSync(Exit);

		// Destroy the worker threads
		for (UINT i = 0; i < numWorkerThreads; i++)
		{
			CloseHandle(threadArray[i]);
		}
		delete [] threadArray;
		threadArray = NULL;

		// Free the aligned thread counter
		_aligned_free(runningWorkerThreadCount);

		// Destroy the wait semaphore
		CloseHandle(workerWaitSemaphore);

		// Delete the start data array
		delete [] threadStartDataArray;
		threadStartDataArray = NULL;
	}

	// Cleanup the back buffer
	DestroyBackBuffer();

	if (rasterizer)
	{
		delete rasterizer;
		rasterizer = NULL;
	}
}

// Static entry point for worker threads
// Calls into the AppEngine instance passed in lpParameter
DWORD WINAPI AppEngine::WorkerThreadEntry(LPVOID lpParameter)
{
	// Retrieve the thread start data struct
	AppEngineThreadStartData* pStartData = (AppEngineThreadStartData*)lpParameter;
	pStartData->owner->WorkerThreadProc(pStartData->threadIndex);
	return 0;
}
void AppEngine::WorkerThreadProc(int threadIndex)
{
	bool isRunning = true;
	while(isRunning)
	{
		// Wait for a new work item
		WaitForSingleObject(this->workerWaitSemaphore, INFINITE);

		switch(this->currentWorkType)
		{
		case Exit:
			// If the thread has been told to exit,
			// set isRunning to false, to signal the end of the work loop
			isRunning = false;
			break;
		case TransformVerts:
			// This thread has been asked to transform vertices using a vertex shader
			// Call into the rasterizer to dispatch the work item
			rasterizer->RunTransformVertsWorkItem();
			break;
		case DrawPrimitives:
			// This thread has been asked to draw primitives to the screen using a pixel shader
			// Call into the rasterizer to dispatch the work item
			rasterizer->RunDrawPrimitivesWorkItem();
			break;
		}

		// Decrement the running thread count, and if the result is 0, signal
		// the main thread that all running worker threads have completed
		if (InterlockedDecrement(this->runningWorkerThreadCount) == 0)
		{
			SetEvent(this->mainThreadWaitHandle);
		}
	}
}

void AppEngine::DispatchAndSync(AppEngineWorkType workType)
{
	if (this->numWorkerThreads == 0)
		return;

	// Set the current work type for the threads
	this->currentWorkType = workType;

	// Set the running worker thread count to numWorkerThreads
	*this->runningWorkerThreadCount = this->numWorkerThreads;

	// Release the worker threads to do work
	LONG prevCount = 0;
	ReleaseSemaphore(this->workerWaitSemaphore, numWorkerThreads, &prevCount);

	// Wait for all threads to signal that they are done
	WaitForSingleObject(this->mainThreadWaitHandle, INFINITE);

	// Reset the main wait handle
	ResetEvent(this->mainThreadWaitHandle);

	// Set the current work type back to None
	this->currentWorkType = None;
}

void AppEngine::CreateBackBuffer(void)
{
	// Get window's client bounds
	RECT clientRect;
	if (!GetClientRect(hWnd, &clientRect))
		return;

	// Calculate width and height
	LONG width = clientRect.right - clientRect.left;
	LONG height = clientRect.bottom - clientRect.top;

	// If a back buffer already exists
	if (backBufferTarget)
	{
		// Cancel if the back buffer's dimensions match the window's
		if (backBufferTarget->GetWidth() == width && backBufferTarget->GetHeight() == height)
		{
			return;
		}
	}

	// Otherwiser, destroy any existing back buffer
	DestroyBackBuffer();

	// Initialize the new rasterizer with the correct window dimensions
	if (width > 0 && height > 0)
	{
		backBufferTarget = new RenderTarget(width, height);
		backBufferDepth = new DepthBuffer(width, height);
	}

	RenderTarget t = *backBufferTarget;
}
void AppEngine::DestroyBackBuffer(void)
{
	if (backBufferTarget)
	{
		delete backBufferTarget;
		backBufferTarget = NULL;
	}
	if (backBufferDepth)
	{
		delete backBufferDepth;
		backBufferDepth = NULL;
	}
}

void AppEngine::NotifyResized(void)
{
	// When the window is resized,
	// recreate the back buffer to fit.
	CreateBackBuffer();

	// Ensure that Windows is told that the contents of the window now needs painting
	InvalidateRect(hWnd, NULL, TRUE);
}

void AppEngine::Paint(HDC hdc)
{
	if (!backBufferTarget)
		return;

	// Get the rasterizer's bitmap handle
	HBITMAP bmp = this->backBufferTarget->GetHBitmap();
	
	// Create a compatible device context to read from the bitmap
	HDC hdcMem = CreateCompatibleDC(hdc);
	
	// Set the rasterizer's bitmap into the created device context
	HGDIOBJ oldObj = SelectObject(hdcMem, bmp);

	// Create a graphics object to render the HUD
	Graphics memGraphics(hdcMem);
	DrawHUD(memGraphics);
	
	// Draw the bitmap to the window
	BitBlt(hdc, 0, 0, this->backBufferTarget->GetWidth(), this->backBufferTarget->GetHeight(), hdcMem, 0, 0, SRCCOPY);

	// Replace the old object and cleanup
	SelectObject(hdcMem, oldObj);
	DeleteDC(hdcMem);
}

void AppEngine::Process(double totalTime, double elapsedTime)
{
	// Cancel if the back buffer is not initialized
	if (!backBufferTarget)
		return;

	// Update the scene
	Update(totalTime, elapsedTime);

	// Draw the scene to the rasterizer
	Draw(totalTime, elapsedTime);

	// Ensure that Windows is told that the contents of the window now needs painting
	InvalidateRect(hWnd, NULL, FALSE);
}

void AppEngine::DrawModel(bool useMultithreading, const vector<Vertex, AlignAlloc<Vertex, 16>>& verts, const vector<Face, AlignAlloc<Face, 16>>& faces, bool usePerVertexNormals, UINT numVector4Slots, VertexShaderCallback vertexShader, PixelShaderCallback pixelShader)
{
	// Begin the transformation work item
	rasterizer->BeginTransformVertsWorkItem(verts, faces, usePerVertexNormals, numVector4Slots, vertexShader);
	
	if (useMultithreading)
	{
		// Dispatch a TransformVert work item, and wait for all threads to finish
		DispatchAndSync(TransformVerts);
	}
	else
	{
		// Run the work item on the current thread
		rasterizer->RunTransformVertsWorkItem();
	}

	// Begin the draw primitives work item
	rasterizer->BeginDrawPrimitivesWorkItem(pixelShader);

	if (useMultithreading)
	{
		// Dispatch a DrawPrimitives work item, and wait for all threads to finish
		DispatchAndSync(DrawPrimitives);
	}
	else
	{
		// Run the work item on the current thread
		rasterizer->RunDrawPrimitivesWorkItem();
	}
}