// Intro3D.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Intro3D.h"
#include "InputManager.h"
#include "DemoEngine.h"
#include <sstream>
#include <GdiPlus.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
HWND g_hWnd;									// Window Handle
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

DemoEngine* demoEngine;
InputManager inputManager;

LARGE_INTEGER startTime;
LARGE_INTEGER lastTime;
LARGE_INTEGER timerFreq;

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	MSG msg;

	// Register the window class
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";
	RegisterClassEx(&wc);

	// Get the window size for a given client area
	RECT wr = {0, 0, 800, 480};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window
	g_hWnd = CreateWindowEx(
		NULL,
		L"WindowClass",
		L"Introduction to 3D Graphics Programming - 100161995",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	// Show and update the window
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// Initialize GDI+
	ULONG gdiToken;
	Gdiplus::GdiplusStartupInput input;
	if (Gdiplus::GdiplusStartup(&gdiToken, &input, NULL) != Gdiplus::Ok)
	{
		return FALSE;
	}
	
	// Get the total number of logical processors in the system
	// Use this as the number of worker threads to run in the app engine.
	SYSTEM_INFO sysinfo; 
	GetSystemInfo( &sysinfo ); 
	UINT numWorkerThreads = sysinfo.dwNumberOfProcessors;

	// Create the demo engine
	demoEngine = new DemoEngine(g_hWnd, inputManager, numWorkerThreads);
	if (demoEngine->LoadFailed())
	{
		delete demoEngine;
		return FALSE;
	}
	
	// Initialize timing values
	QueryPerformanceFrequency(&timerFreq);
	QueryPerformanceCounter(&startTime);
	lastTime = startTime;

	// Main Message Loop
	for(;;)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Update timing values
			LARGE_INTEGER currentTime;
			QueryPerformanceCounter(&currentTime);
			LONGLONG totalTicks = currentTime.QuadPart - startTime.QuadPart;
			LONGLONG elapsedTicks = currentTime.QuadPart - lastTime.QuadPart;
			lastTime = currentTime;

			double totalSeconds = ((double)totalTicks) / ((double)timerFreq.QuadPart);
			double elapsedSeconds = ((double)elapsedTicks) / ((double)timerFreq.QuadPart);
			
			demoEngine->Process(totalSeconds, elapsedSeconds);
			inputManager.Update();

			if (inputManager.IsButtonDown(XINPUT_GAMEPAD_BACK) || inputManager.IsKeyDown(0x1B))
				PostQuitMessage(0);
		}
	}

	delete demoEngine;

	Gdiplus::GdiplusShutdown(gdiToken);

	return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		// Paint Handling for AppEngine
	case WM_ERASEBKGND:
		return TRUE;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		demoEngine->Paint(hdc);
		EndPaint(hWnd, &ps);
		break;

		// Resize Handling for AppEngine
	case WM_SIZE:
		demoEngine->NotifyResized();
		return DefWindowProc(hWnd, message, wParam, lParam);

		// Keyboard Handling for inputManager
	case WM_KEYDOWN:
	case WM_KEYUP:
		inputManager.ParseMessage(message, wParam, lParam);
		return TRUE;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}