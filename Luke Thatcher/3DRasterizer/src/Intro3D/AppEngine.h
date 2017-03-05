
#pragma once

#include "Rasterizer.h"
#include "Model.h"
#include "InputManager.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "DepthBuffer.h"

enum AppEngineWorkType
{
	None,
	TransformVerts,
	DrawPrimitives,
	Exit
};

class AppEngine;
struct AppEngineThreadStartData
{
	int threadIndex;
	AppEngine* owner;
};

__declspec(align(16)) class AppEngine abstract
{
public:
	DEFINE_NEW_DELETE_ALIGNED

	AppEngine(HWND hWnd, UINT numWorkerThreads);
	virtual ~AppEngine(void);

	void Process(double totalTime, double elapsedTime);
	void Paint(HDC hdc);
	void NotifyResized(void);
	void DrawModel(bool useMultithreading, const vector<Vertex, AlignAlloc<Vertex, 16>>& verts, const vector<Face, AlignAlloc<Face, 16>>& faces, bool usePerVertexNormals, UINT numVector4Slots, VertexShaderCallback vertexShader, PixelShaderCallback pixelShader);
	
protected:
	virtual void Update(double totalTime, double elapsedTime) abstract;
	virtual void Draw(double totalTime, double elapsedTime) abstract;
	virtual void DrawHUD(Gdiplus::Graphics& graphics) abstract;

	Rasterizer* rasterizer;
	RenderTarget* backBufferTarget;
	DepthBuffer* backBufferDepth;

private:
	HWND hWnd;
	UINT numWorkerThreads;
	HANDLE* threadArray;
	UINT* runningWorkerThreadCount;
	HANDLE mainThreadWaitHandle;
	HANDLE workerWaitSemaphore;
	AppEngineThreadStartData* threadStartDataArray;

	volatile AppEngineWorkType currentWorkType;

	void CreateBackBuffer(void);
	void DestroyBackBuffer(void);

	static DWORD WINAPI WorkerThreadEntry(LPVOID lpParameter);
	void WorkerThreadProc(int threadIndex);
	void DispatchAndSync(AppEngineWorkType workType);
};