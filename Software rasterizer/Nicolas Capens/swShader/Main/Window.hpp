#ifndef BaseWin32Window_hpp
#define BaseWin32Window_hpp

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

namespace swShader
{
	class Window
	{
	public:
		enum
		{
			INVISIBLE	= 1 << 0,
			FULLSCREEN	= 1 << 1,
			FIXED		= 1 << 2,
		};

		Window(HINSTANCE instance, const char *name = "", int width = 320, int height = 240, int style = 0);

		virtual ~Window();

		virtual void mainLoop();

	protected:
		virtual void init() = 0;
		virtual void destroy() = 0;

		virtual void eventMain();
		virtual void eventDraw();
		virtual void eventKeyUp(int key);
		virtual void eventKeyDown(int key);
		virtual void eventDestroy();

		virtual void toggleFullscreen();

		void showCursor();
		void hideCursor();

		void terminate();

		HWND handle;

		char *name;
		int width;
		int height;
		int style;

	private:
		virtual void open();
		virtual void close();

		static LRESULT CALLBACK messageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		HINSTANCE instance;
		WNDCLASSEX extendedClass;

		bool quit;
	};
}

#endif   // BaseWin32Window_hpp