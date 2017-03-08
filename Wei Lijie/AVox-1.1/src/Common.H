#ifndef __Common
#define __Common

#include "Includes.H"

/*typedef struct 
{
	bool KeyDown[256];
} KEYS;

typedef struct 
{
	HINSTANCE hInstance;
	const char* ClassName;
} APPLICATION;
*/

typedef struct
{
	int Width;
	int Height;
	int BytesPerPixel;
} SCREEN;

/*typedef struct 
{
	APPLICATION* Application;
	char* Title;
	
	KEYS* Keys;
	
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;

} WINDOW;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 

void TerminateApplication (WINDOW* Window);
bool RegisterWindowClass(APPLICATION *Application);
bool GreateWindow(WINDOW *Window, SCREEN* Screen);
bool DestroyWindow(WINDOW* Window);*/

#endif