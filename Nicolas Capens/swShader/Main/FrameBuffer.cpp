#include "FrameBuffer.hpp"

#include "Error.hpp"

#include <windows.h>
#include <windowsx.h>
#include <float.h>

namespace swShader
{
	FrameBuffer::FrameBuffer()
	{
		directDraw = 0;
		frontBuffer = 0;
		backBuffer = 0;

		locked = false;

		colorBuffer = 0;
		width = 0;
		height = 0;
		bitDepth = 0;
		pitch = 0;
		stride = 0;
	}

	FrameBuffer::~FrameBuffer()
	{
		releaseAll();
	}

	void FrameBuffer::initFullscreen(HWND handle, int width, int height)
	{
		releaseAll();

		DirectDrawCreateEx(0, (void**)&directDraw, IID_IDirectDraw7, 0);
		directDraw->SetCooperativeLevel(handle, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
		directDraw->SetDisplayMode(width, height, 32, 0, 0);

		DDSURFACEDESC2 surfaceDescription = {0};
		surfaceDescription.dwSize	= sizeof(surfaceDescription);
		surfaceDescription.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		surfaceDescription.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE;
		surfaceDescription.dwBackBufferCount = 1;
		directDraw->CreateSurface(&surfaceDescription, &frontBuffer, 0);

		DDSCAPS2 surfaceCapabilties = {0};
		surfaceCapabilties.dwCaps = DDSCAPS_BACKBUFFER;
		frontBuffer->GetAttachedSurface(&surfaceCapabilties, &backBuffer);
		backBuffer->AddRef();

		depthBuffer = new byte[4 * width * height];

		this->windowHandle = handle;
		windowed = false;
		updateBounds();
	}

	void FrameBuffer::initWindowed(HWND handle, int width, int height)
	{
		releaseAll();

		DirectDrawCreateEx(0, (void**)&directDraw, IID_IDirectDraw7, 0);
		directDraw->SetCooperativeLevel(handle, DDSCL_NORMAL);
	
		RECT rcWork;
		RECT rc;
		DWORD dwStyle;

		dwStyle = GetWindowStyle(handle);
		dwStyle	&= ~WS_POPUP;
		dwStyle	|= WS_OVERLAPPED | WS_CAPTION |	WS_THICKFRAME |	WS_MINIMIZEBOX;
		SetWindowLong(handle, GWL_STYLE, dwStyle);

		SetRect(&rc, 0, 0, width, height);

		AdjustWindowRectEx(&rc, GetWindowStyle(handle), GetMenu(handle) != 0, GetWindowExStyle(handle));
		SetWindowPos(handle, 0, 0, 0, rc.right-rc.left, rc.bottom-rc.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		SetWindowPos(handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
		GetWindowRect(handle, &rc);
		if(rc.left < rcWork.left) rc.left = rcWork.left;
		if(rc.top < rcWork.top) rc.top = rcWork.top;
		SetWindowPos(handle, 0, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

		IDirectDrawClipper *clipper;
		
		DDSURFACEDESC2 ddsd = {0};
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps	= DDSCAPS_PRIMARYSURFACE;

		directDraw->CreateSurface(&ddsd, &frontBuffer, 0);

		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;	   
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
		ddsd.dwWidth = width;
		ddsd.dwHeight = height;

		directDraw->CreateSurface(&ddsd, &backBuffer, 0);

		directDraw->CreateClipper(0, &clipper, 0);
		clipper->SetHWnd(0, handle);
		frontBuffer->SetClipper(clipper);
		clipper->Release();

		depthBuffer = new byte[4 * width * height];

		windowHandle = handle;
		windowed = true;
		updateBounds();
	}

	void FrameBuffer::flip()
	{
		if(!frontBuffer || !backBuffer)
		{
			throw "Could not flip framebuffer";
		}

		unlock();

		HRESULT	result;

		while(true)
		{
			if(windowed)
			{
				result = frontBuffer->Blt(&bounds, backBuffer, 0, DDBLT_WAIT, 0);
			}
			else
			{
				result = frontBuffer->Flip(0, 0);
			}

			if(result == DDERR_SURFACELOST)
			{
				frontBuffer->Restore();
				backBuffer->Restore();
			}

			if(result != DDERR_WASSTILLDRAWING)
			{
				return;
			}
		}
	}

	void FrameBuffer::clear(Color<byte> color)
	{
		if(backBuffer == 0) throw Error("Could not clear frame buffer");

		for(int i = 0; i < width * height; i++)
		{
			((float*)depthBuffer)[i] = FLT_MAX;
		}

		unlock();

		// Erase the background
		DDBLTFX	ddbltfx = {0};
		ddbltfx.dwSize = sizeof(ddbltfx);
		ddbltfx.dwFillColor	= color;

		backBuffer->Blt(0, 0, 0, DDBLT_COLORFILL, &ddbltfx);
	}

	void FrameBuffer::updateBounds()
	{
		if(windowed)
		{
			GetClientRect(windowHandle, &bounds);
			ClientToScreen(windowHandle, (POINT*)&bounds);
			ClientToScreen(windowHandle, (POINT*)&bounds + 1);
		}
		else
		{
			SetRect(&bounds, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		}
	}

	void FrameBuffer::lock()
	{
		if(locked) return;

		DDSURFACEDESC2 DDSD;
		DDSD.dwSize = sizeof(DDSD);

		backBuffer->Lock(0, &DDSD, DDLOCK_WAIT, 0);

		colorBuffer = DDSD.lpSurface;
		width = DDSD.dwWidth;
		height = DDSD.dwHeight;
		bitDepth = DDSD.ddpfPixelFormat.dwRGBBitCount;
		pitch = DDSD.lPitch;
		stride = pitch / (bitDepth / 8);

		locked = true;
	}

	void FrameBuffer::unlock()
	{
		if(!locked) return;
		if(!backBuffer) return;

		backBuffer->Unlock(0);

		colorBuffer = 0;
		width = 0;
		height = 0;
		bitDepth = 0;
		pitch = 0;
		stride = 0;

		locked = false;
	}

	void *FrameBuffer::getColorBuffer()
	{
		if(!locked) lock();

		return colorBuffer;
	}

	void *FrameBuffer::getDepthBuffer()
	{
		return depthBuffer;
	}

	int FrameBuffer::getWidth()
	{
		if(!locked) lock();

		return width;
	}

	int FrameBuffer::getHeight()
	{
		if(!locked) lock();

		return height;
	}

	int FrameBuffer::getStride()
	{
		if(!locked) lock();

		return stride;
	}

	int FrameBuffer::getBitDepth()
	{
		if(!locked) lock();

		return bitDepth;
	}

	void FrameBuffer::releaseAll()
	{
		unlock();

		if(backBuffer)
		{
			backBuffer->Release();
			backBuffer = 0;
		}

		if(frontBuffer)
		{
			frontBuffer->Release();
			frontBuffer = 0;
		}

		if(directDraw)
		{
			directDraw->SetCooperativeLevel(0, DDSCL_NORMAL);
			directDraw->Release();
			directDraw = 0;
		}
	}
}