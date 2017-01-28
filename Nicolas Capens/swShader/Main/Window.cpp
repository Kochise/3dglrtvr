#include "Window.hpp"

#include "Error.hpp"

#include <string.h>

namespace swShader
{
	Window::Window(HINSTANCE instance, const char *name, int width, int height, int style)
	{
		this->instance = instance;
		this->name = strdup(name);
		this->width = width;
		this->height = height;
		this->style = style;

		open();
	}

	Window::~Window()
	{
		delete[] name;
		name = 0;

		close();
	}

	void Window::toggleFullscreen()
	{
		close();

		if(style & FULLSCREEN)
		{
			style &= ~FULLSCREEN;
		}
		else
		{
			style |= FULLSCREEN;
		}

		open();
	}

	LRESULT CALLBACK Window::messageHandler(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
	{
		Window *window = (Window*)GetWindowLong(handle, GWL_USERDATA);
		
		switch(message)
		{
			case WM_KEYDOWN:
			{
				window->eventKeyDown(wparam);
				break;
			}
			case WM_KEYUP:
			{	
				window->eventKeyUp(wparam);
				break;
			}
			case WM_HOTKEY:
			{
				int hotKey = (int)wparam;
				UINT modifiers = (UINT)LOWORD(lparam);
				UINT virtualKey = (UINT)HIWORD(lparam);

				if(modifiers == MOD_ALT && virtualKey == VK_RETURN)
				{
					window->destroy();
					window->toggleFullscreen();
					window->init();
				}
			}
			case WM_PAINT:
			{
				PAINTSTRUCT paint;
				BeginPaint(handle, &paint);

				window->eventDraw();

				EndPaint(handle, &paint);
				ValidateRect(handle, 0);
				break;
			}
			case WM_ERASEBKGND:
			{
				// Take no action
				return 1;
			}
			case WM_DESTROY:
			{
				window->terminate();
				break;
			}
			default:
			{
				return(DefWindowProc(handle, message, wparam, lparam));
			}
		}

		return 0;
	}

	void Window::mainLoop()
	{
		MSG message;

  		do
		{
			if(PeekMessage(&message, handle, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			else
			{
				eventMain();
			}
		}
		while(!quit);
	}

	void Window::eventMain()
	{
		eventDraw();
	}

	void Window::eventDraw()
	{
	}

	void Window::eventKeyUp(int key)
	{
	}

	void Window::eventKeyDown(int key)
	{
	}

	void Window::eventDestroy()
	{
	}

	void Window::close()
	{
		DestroyWindow(handle);
		handle = 0;
	}

	void Window::open()
	{
		extendedClass.cbSize = sizeof(WNDCLASSEX);
		extendedClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		extendedClass.lpfnWndProc = messageHandler;
		extendedClass.cbClsExtra = 0;
		extendedClass.cbWndExtra = 0;
		extendedClass.hInstance = instance;
		extendedClass.hIcon = LoadIcon(0, IDI_WINLOGO);
		extendedClass.hCursor = LoadCursor(0, IDC_ARROW);
		extendedClass.hbrBackground = 0;
		extendedClass.lpszMenuName = 0;
		extendedClass.lpszClassName = name;
		extendedClass.hIconSm = LoadIcon(0, IDI_WINLOGO);

		RegisterClassEx(&extendedClass);

		unsigned int flags = 0;
		
		if(!(style & INVISIBLE))
		{
			flags |= WS_VISIBLE;
		}

		if(style & FULLSCREEN)
		{
			flags |= WS_POPUP;
		}
		else
		{
			if(style & FIXED)
			{
				flags |= WS_OVERLAPPED;
			}
			else
			{
				flags |= WS_OVERLAPPEDWINDOW;
			}
		}

		RECT rectangle;
		rectangle.left = 0;
		rectangle.right = width - 1;
		rectangle.top = 0;
		rectangle.bottom = height - 1;
		AdjustWindowRect(&rectangle, flags, false);
		const int windowWidth = rectangle.right - rectangle.left + 1;
		const int windowHeight = rectangle.bottom - rectangle.top + 1;

		handle = CreateWindowEx(0, name, name, flags, 10, 10, windowWidth, windowHeight, 0, 0, instance, 0);

		if(!handle)
		{
			throw Error("Could not create window");
		}

		SetWindowLong(handle, GWL_USERDATA, (LONG)this);

		RegisterHotKey(handle, 0, MOD_ALT, VK_RETURN);   // Alt-enter for fullscreen

		quit = false;
	}

	void Window::showCursor()
	{
		while(ShowCursor(true) < 0);
	}

	void Window::hideCursor()
	{
		while(ShowCursor(false) >= 0);
	}

	void Window::terminate()
	{
		eventDestroy();
		quit = true;
	}
}