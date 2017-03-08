#include "Common.H"
#include "Log.H"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_ACTIVATE:
		{
		}

		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
			break;
		}

		case WM_KEYDOWN:
		{
			if (wParam==VK_ESCAPE)  PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

bool RegisterWindowClass(APPLICATION *Application)
{
	WNDCLASSEX WindowClass;

	ZeroMemory (&WindowClass, sizeof (WNDCLASSEX));
	WindowClass.cbSize = sizeof (WNDCLASSEX);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WindowClass.lpfnWndProc = (WNDPROC)(WindowProc);
	WindowClass.hInstance = Application->hInstance;
	WindowClass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);	
	WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowClass.lpszClassName = Application->ClassName;
	
	if (RegisterClassEx (&WindowClass) == 0)
	{
		Log.Write("Red","Error: Registering Windows Class");
		return FALSE;
	}
	return TRUE;
}

bool GreateWindow(WINDOW *Window, SCREEN* Screen)
{
	DWORD WindowStyle = WS_POPUPWINDOW | WS_VISIBLE;
	DWORD WindowExtendedStyle = WS_EX_TOPMOST;

	PIXELFORMATDESCRIPTOR Pfd =
	{
		sizeof (PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		Screen->BytesPerPixel,
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

	GLuint PixelFormat;

	Window->hWnd = CreateWindowEx (WindowExtendedStyle,
									Window->Application->ClassName,
									Window->Title,
									WindowStyle,
									0, 0,
									Screen->Width,
									Screen->Height,
									HWND_DESKTOP,
									0,
									Window->Application->hInstance,
									NULL);
	
	if (Window->hWnd == 0) return false;

	::ShowCursor(false);
	
	Window->hDC = GetDC(Window->hWnd)	;
	if (Window->hDC == 0)
	{
		DestroyWindow (Window->hWnd);
		Window->hWnd = 0;
		return false;
	}

	PixelFormat = ChoosePixelFormat (Window->hDC, &Pfd);	
	if (PixelFormat==0)
	{
		ReleaseDC(Window->hWnd, Window->hDC);
		Window->hDC = 0;
		DestroyWindow (Window->hWnd);
		Window->hWnd = 0;
		return false;
	}

	if (SetPixelFormat (Window->hDC, PixelFormat, &Pfd) == FALSE)
	{
		ReleaseDC (Window->hWnd, Window->hDC);
		Window->hDC = 0;
		DestroyWindow (Window->hWnd);
		Window->hWnd = 0;
		return false;
	}

	Window->hRC = wglCreateContext (Window->hDC);
	if (Window->hRC == 0)
	{
		ReleaseDC (Window->hWnd, Window->hDC);
		Window->hDC = 0;
		DestroyWindow (Window->hWnd);
		Window->hWnd = 0;
		return false;
	}

	if (wglMakeCurrent (Window->hDC, Window->hRC) == false)
	{
		wglDeleteContext (Window->hRC);
		Window->hRC = 0;
		ReleaseDC (Window->hWnd, Window->hDC);
		Window->hDC = 0;
		DestroyWindow (Window->hWnd);
		Window->hWnd = 0;
		return false;
	}

	ShowWindow (Window->hWnd, SW_NORMAL);
	ZeroMemory (Window->Keys, sizeof (KEYS));
	
	return true;
}

bool DestroyWindow(WINDOW* Window)
{
	if (Window->hWnd != 0)
	{	
		if (Window->hDC != 0)
		{
			wglMakeCurrent (Window->hDC, 0);
			if (Window->hRC != 0)
			{
				wglDeleteContext (Window->hRC);
				Window->hRC = 0;
			}
			ReleaseDC (Window->hWnd, Window->hDC);
			Window->hDC = 0;
		}
		DestroyWindow (Window->hWnd);
		Window->hWnd = 0;
	}

	::ShowCursor(true);

	return TRUE;
}
