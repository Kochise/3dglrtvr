/**
 *  Marching Cubes Demo, by glUser3f [gluser3f@gdnmail.net]
 *  Feel free to use this code in your own projects.
 *  If you do something cool with it, please email me so I can see it!
 *  
 *  Credits:
 *  Theory by Paul Bourke        [http://astronomy.swin.edu.au/~pbourke/modelling/polygonise/]
 *  OpenGL basecode by NeHe      [nehe.gamedev.net]
 *  Lookup Tables by Paul Bourke [http://astronomy.swin.edu.au/~pbourke/modelling/polygonise/]
 *  The rest is done by me, glUser3f ;)
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "glfw.h"

HDC       hDC  = NULL;
HGLRC     hRC  = NULL;
HWND      hWnd = NULL;
HINSTANCE hInstance;

BOOL glfwActive     = TRUE;
BOOL glfwFullScreen = TRUE;

const int glfwWidth    = 640;
const int glfwHeight   = 480;
const int glfwBPP      = 32;
const char glfwTitle[] = "Marching Cubes Demo .:Programmed By glUser3f:.";

bool glfwKeys[256];

LRESULT	CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

void glfwResizeScene(GLsizei width, GLsizei height) {
	if (height == 0) {
		height = 1;
	}
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void glfwKillWnd() {
	if (glfwFullScreen)	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}
	glfwDeInit();
	if (hRC) {
		if (!wglMakeCurrent(NULL, NULL)) {
			MessageBox(NULL, "Error: Function wglMakeCurrent.", "SHUTDOWN ERROR", MB_OK | MB_ICONERROR);
		}
		if (!wglDeleteContext(hRC)) {
			MessageBox(NULL, "Error: Function wglDeleteContext.", "SHUTDOWN ERROR", MB_OK | MB_ICONERROR);
		}
		hRC = NULL;
	}
	if (hDC && !ReleaseDC(hWnd, hDC)) {
		MessageBox(NULL, "Error: Function ReleaseDC.", "SHUTDOWN ERROR", MB_OK | MB_ICONERROR);
		hDC = NULL;
	}
	if (hWnd && !DestroyWindow(hWnd)) {
		MessageBox(NULL, "Error: Function DestroyWindow.", "SHUTDOWN ERROR", MB_OK | MB_ICONERROR);
		hWnd = NULL;
	}
	if (!UnregisterClass("OpenGL", hInstance)) {
		MessageBox(NULL, "Error: Function UnregisterClass.", "SHUTDOWN ERROR", MB_OK | MB_ICONERROR);
		hInstance = NULL;
	}
}

BOOL glfwCreateWnd(const char* title, int width, int height, int bits, BOOL fullScreen) {
	GLuint   pixelFormat;
	WNDCLASS wc;
	DWORD    exStyle;
	DWORD    style;
	RECT     windowRect;
	windowRect.left = (long)0;
	windowRect.right = (long)width;
	windowRect.top = (long)0;
	windowRect.bottom = (long)height;

	glfwFullScreen = fullScreen;

	hInstance			= GetModuleHandle(NULL);
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC)WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "OpenGL";

	if (!RegisterClass(&wc)) {
		MessageBox(NULL, "Error: Function RegisterClass.", "STARTUP ERROR", MB_OK|MB_ICONERROR);
		return FALSE;
	}
	
	if (glfwFullScreen) {
		DEVMODE screenSettings;
		memset(&screenSettings, 0, sizeof(DEVMODE));
		screenSettings.dmSize = sizeof(DEVMODE);
		screenSettings.dmPelsWidth	= width;
		screenSettings.dmPelsHeight	= height;
		screenSettings.dmBitsPerPel	= bits;
		screenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			if (MessageBox(NULL, "FullScreen Mode Is Not Supported.\nUse Windowed Mode?", "FullScreen/Windowed?", MB_YESNO|MB_ICONEXCLAMATION)==IDYES)	{
				glfwFullScreen=FALSE;
			} else {
				MessageBox(NULL,"Program Will Now Close.","STARTUP ERROR",MB_OK|MB_ICONERROR);
				return FALSE;
			}
		}
	}

	if (glfwFullScreen) {
		exStyle = WS_EX_APPWINDOW;
		style   = WS_POPUP;
		ShowCursor(FALSE);
	} else {
		exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		style   = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&windowRect, style, FALSE, exStyle);

	if (!(hWnd=CreateWindowEx(	exStyle, "OpenGL", title, style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
								0, 0,
								windowRect.right  - windowRect.left,
								windowRect.bottom - windowRect.top,
								NULL, NULL,
								hInstance,
								NULL)))	{
		glfwKillWnd();
		MessageBox(NULL, "Error: Function CreateWindowEx.", "STARTUP ERROR", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	static	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		bits,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	
	if (!(hDC = GetDC(hWnd)))	{
		glfwKillWnd();
		MessageBox(NULL, "Error: Function GetDC.", "STARTUP ERROR", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	if (!(pixelFormat = ChoosePixelFormat(hDC, &pfd)))	{
		glfwKillWnd();
		MessageBox(NULL, "Error: Function ChoosePixelFormat.", "STARTUP ERROR", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	if(!SetPixelFormat(hDC, pixelFormat, &pfd)) {
		glfwKillWnd();
		MessageBox(NULL, "Error: Function SetPixelFormat.", "STARTUP ERROR", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	if (!(hRC = wglCreateContext(hDC))) {
		glfwKillWnd();
		MessageBox(NULL, "Error: Function wglCreateContext.", "STARTUP ERROR", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	if(!wglMakeCurrent(hDC, hRC)) {
		glfwKillWnd();
		MessageBox(NULL, "Error: Function wglMakeCurrent.", "STARTUP ERROR", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	glfwResizeScene(width, height);

	if (!glfwInit(hInstance, hWnd)) {
		glfwKillWnd();
		MessageBox(NULL, "Error: Function glfwInit.", "STARTUP ERROR", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_ACTIVATE:
			glfwActive = !HIWORD(wParam);
			if (glfwActive) {
				glfwActivate();
			} else {
				glfwDeActivate();
			}
			return 0;
		
		case WM_SYSCOMMAND:
			switch (wParam) {
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
			break;
		
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		case WM_SIZE:
			glfwResizeScene(LOWORD(lParam), HIWORD(lParam));
			return 0;

		case WM_KEYDOWN:
			glfwKeys[wParam] = true;
			break;

		case WM_KEYUP:
			glfwKeys[wParam] = false;
			break;
	}

	return DefWindowProc(wnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int pCmdShow) {
	MSG  msg;
	BOOL done = FALSE;

	//if (MessageBox(NULL, "Would You Like To Run In FullScreen Mode?", "FullScreen/Windowed?",MB_YESNO | MB_ICONQUESTION) == IDNO) {
		glfwFullScreen = FALSE;
	//}

		MessageBox(NULL, "Welcome To The Marching Cubes Algorithm Demo.\n Programmed By glUser3f.\n\nControls:\nPg Up/Down: Zoom In / Out\nW:  Wireframe / Polygons\nL:    Light On / Off\nP:    Pause\n\nEnjoy!", "Welcome!", MB_OK);

	if (!glfwCreateWnd(glfwTitle, glfwWidth, glfwHeight, glfwBPP, glfwFullScreen)) {
		return 0;
	}

	while(!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				done = TRUE;
			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} else {
			if (glfwActive) {
				glfwRender();
				glfwUpdate(glfwKeys);
				SwapBuffers(hDC);
			}

		}
	}
	glfwKillWnd();
	return (int)(msg.wParam);
}

void Terminate() {
	PostMessage(hWnd, WM_QUIT, 0, 0);
}