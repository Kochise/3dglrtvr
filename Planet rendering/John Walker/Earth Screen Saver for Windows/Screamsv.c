/*

					 SCREAM SAVER

				    by John Walker
				http://www.fourmilab.ch/

			This program is in the public domain.

	This program is a Windows (32-bit) application which
	serves as a test jig for the development of screen
	savers.  By allowing you to run an experimental screen
	saver inside a window, you can use conventional debuggers,
	Bounds Checker, etc. on it without the problems caused
	by the normal preemption of the screen performed by
	the code in scrnsave.lib.

*/

//#define SCREAM_SAVER					// Uncomment this line for SCREAM SAVER build
/*#define RIGHT_BUTTON_PASSTHROUGH*/	/* Define this if you'd like to pass through the
										   clicks of the right mouse button to the screen
										   saver rather than unconditionally terminating
										   it.  This allows testing screen savers which
										   take some special action (for example, copying
										   the current image to the clipboard) when the right
										   mouse button is clicked. */

#include <windows.h>
#include <scrnsave.h>

#include "screamsv.h"

#ifdef SCREAM_SAVER

BOOL fChildPreview = FALSE;

static char appName[] = "SCREAMSV";

static HWND testWindow;					// Window in which test is being run
static HWND controlWindow;				// Control window
static BOOL testActive = FALSE;			// Is a test underway ?
static BOOL fullScreen = FALSE;			// Is test full screen ?

/* Forward procedures */

static int nCwRegisterClasses(void);

/*

	Main Program

*/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{

    MSG msg;       			// MSG structure to store your messages

    hMainInstance = hInstance;

	if (nCwRegisterClasses() == -1) {
        MessageBox(NULL, "SCREAM SAVER: Cannot register classes.", NULL, MB_ICONEXCLAMATION);
        return 2;
	}

	RegisterDialogClasses(hMainInstance);

    /* Create application's Main window */

    controlWindow = hMainWindow = CreateWindow(
		"WindowsScreenSaverClass", // Window class name
		"Scream Saver[tm] by John Walker (http://www.fourmilab.ch/)", // Window's title
        WS_CAPTION      |       // Title and Min/Max
        WS_SYSMENU      |       // Add system menu box
        WS_MAXIMIZEBOX	|		// Add maximise box
        WS_MINIMIZEBOX  |       // Add minimise box
        WS_THICKFRAME   |       // Thick frame: allow resize
        WS_CLIPCHILDREN |       // don't draw in child windows areas
        WS_OVERLAPPED,          // this is a conventional overlapped window
        CW_USEDEFAULT, 0,       // Use default X, Y
        640 + (GetSystemMetrics(SM_CXFRAME) * 2), // Initial window width
        480 + (GetSystemMetrics(SM_CYFRAME) * 2) +
        	   GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU), // Initial height
        NULL,                   // Parent window's handle
        NULL,					// Default to Class Menu
        hMainInstance,          // Instance of window
        NULL);                  // Create struct for WM_CREATE

    if (hMainWindow == NULL) {
        MessageBox(NULL, "SCREAM SAVER: Cannot create main window.", NULL, MB_ICONEXCLAMATION);
        return 2;
    }

    ShowWindow(hMainWindow, nCmdShow);     // Display main window

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;		// You can stop screaming now
}

/*

    Main Window Procedure

*/

static CREATESTRUCT ourCreation;

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    HDC hDC;                			// handle for the display device
    int nRc = 0;            			// return code
	RECT cr;

    switch (Message) {
        case WM_COMMAND:
         switch (LOWORD(wParam)) {

			case SCREAMSV_IDM_F_EXIT:
				PostMessage(hWnd, WM_CLOSE, 0, 0L);
				break;

			case SCREAMSV_IDM_F_COPY:
				{
					HBITMAP hBitmap;
					hDC = GetDC(hWnd);
					GetClientRect(hWnd, &cr);
					hBitmap = CreateCompatibleBitmap(hDC, cr.right, cr.bottom);
					if (hBitmap) {
						HBITMAP obmap;
						HDC hdcMem;

						hdcMem = CreateCompatibleDC(hDC);
						obmap = SelectObject(hdcMem, hBitmap);
						BitBlt(hdcMem, 0, 0, cr.right, cr.bottom, hDC, 0, 0, SRCCOPY);
						SelectObject(hdcMem, obmap);
						DeleteDC(hdcMem);
						OpenClipboard(hWnd);
						EmptyClipboard();
						SetClipboardData(CF_BITMAP, hBitmap);
						CloseClipboard();
					}
					ReleaseDC(hWnd, hDC);
				}
				break;

			case SCREAMSV_IDM_T_WINDOW:
				fChildPreview = FALSE;
windTest:		testWindow = hWnd;
				fullScreen = FALSE;
				testActive = TRUE;
				hMainWindow = hWnd;
				ScreenSaverProc(hWnd, WM_CREATE, 0, (LPARAM) &ourCreation);
				{
					hDC = GetDC(hWnd);
					ScreenSaverProc(hWnd, WM_ERASEBKGND, (WPARAM) hDC, 0);
					ReleaseDC(hWnd, hDC);
				}
				break;

			case SCREAMSV_IDM_T_PREVIEW:
				fChildPreview = TRUE;
				goto windTest;

			case SCREAMSV_IDM_T_FULL:
				fChildPreview = FALSE;
				GetWindowRect(GetDesktopWindow(), &cr);
				testWindow = CreateWindowEx(
					WS_EX_TOPMOST,
					"FullTest",				// Window class name
					"Test Window",			// Window's title
					WS_POPUP,				// Style
					0, 0,					// X, Y position
					cr.right, cr.bottom,	// Size
					NULL,                   // Parent window's handle
					NULL,                   // No Menu
					hMainInstance,          // Instance of window
					NULL);                  // Create struct for WM_CREATE
				SetMenu(testWindow, NULL);
				ShowWindow(testWindow, SW_SHOW);
				fullScreen = TRUE;
				break;

			case SCREAMSV_IDM_T_END:
endTest:
				ScreenSaverProc(hWnd, WM_DESTROY, 0, 0);
				testWindow = hWnd;
				testActive = FALSE;
				if (!fullScreen) {
					hDC = GetDC(hWnd);
					GetClientRect(hWnd, &cr);
					FillRect(hDC, &cr, (HBRUSH) GetStockObject(BLACK_BRUSH));
					ReleaseDC(hWnd, hDC);
				}
				testWindow = hMainWindow = controlWindow;
				break;

			case SCREAMSV_IDM_T_SETTINGS:
				DialogBox(hMainInstance, MAKEINTRESOURCE(DLG_SCRNSAVECONFIGURE),
					hWnd, ScreenSaverConfigureDialog);
				break;

			default:
				return DefWindowProc(hWnd, Message, wParam, lParam);
            }
            break;

        case WM_CREATE:
			memcpy(&ourCreation, (LPCREATESTRUCT) lParam, sizeof(CREATESTRUCT));
            break;
            
        case WM_CLOSE:
            DestroyWindow(hWnd);
			break;        

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_LBUTTONDOWN:			// Left mouse button down
        case WM_MBUTTONDOWN:
#ifndef RIGHT_BUTTON_PASSTHROUGH
        case WM_RBUTTONDOWN:
#endif
		case WM_KEYDOWN:
			if (testActive) {
				goto endTest;
			}
			break;

        case WM_INITMENU:
			{
				HMENU menu = (HMENU) wParam;

#define ME(x) ((x) ? (MF_ENABLED) : (MF_DISABLED | MF_GRAYED))
				EnableMenuItem(menu, SCREAMSV_IDM_T_SETTINGS, ME(!testActive));
				EnableMenuItem(menu, SCREAMSV_IDM_T_WINDOW, ME(!testActive));
				EnableMenuItem(menu, SCREAMSV_IDM_T_PREVIEW, ME(!testActive));
				EnableMenuItem(menu, SCREAMSV_IDM_T_FULL, ME(!testActive));
				EnableMenuItem(menu, SCREAMSV_IDM_T_END, ME(testActive));
#undef ME
			}
            break;

        default:
            return testActive ?
				ScreenSaverProc(hWnd, Message, wParam, lParam) :
				DefWindowProc(hWnd, Message, wParam, lParam);
    }
    return 0L;
}

/*	DefScreenSaverProc  --  This is the procedure called by the screen
							saver to perform default processing for any
							message it doesn't itself process in its
							own ScreenSaverProc.  This is thus the heart
							of the emulation (more or less) of SCRNSAVE.LIB.
							Note that we get first dibs on arriving messages
							in our own WndProc above and thus can
							override the operation of the screen saver
							to, for example, switch it off when a menu
							item is picked.  */

LRESULT WINAPI DefScreenSaverProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hDC;                			// handle for the display device
    PAINTSTRUCT ps;         			// holds PAINT information
	RECT cr;							// Rectangle to obtain window size
	HBRUSH br, obr;						// Brushes to erase screen

    switch (msg) {

        case WM_PAINT:
            memset(&ps, 0, sizeof(PAINTSTRUCT));
            InvalidateRgn(hWnd, NULL, TRUE);
            hDC = BeginPaint(hWnd, &ps);
		    GetClientRect(hWnd, &cr);
			br = CreateSolidBrush(GetBkColor(hDC));
			obr = SelectObject(hDC, br);
		    FillRect(hDC, &cr, br);
			SelectObject(hDC, obr);
			DeleteObject(br);
            EndPaint(hWnd, &ps);
            return 0L;

		case WM_DESTROY:
			return 0L;

        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0L;
}

//	GUZZWndProc  --  Window procedure for full screen window

LRESULT CALLBACK GUZZWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message) {

        case WM_CREATE:
			testWindow = hMainWindow = hWnd;
			ScreenSaverProc(hWnd, Message, wParam, lParam);
            break;
            
        case WM_CLOSE:
            DestroyWindow(hWnd);
			break;        

        case WM_DESTROY:
            ScreenSaverProc(hWnd, Message, wParam, lParam);
			testWindow = hMainWindow = controlWindow;
            break;

        case WM_LBUTTONDOWN:			// Left mouse button down
        case WM_MBUTTONDOWN:
#ifndef RIGHT_BUTTON_PASSTHROUGH
        case WM_RBUTTONDOWN:
#endif
		case WM_KEYDOWN:
            DestroyWindow(hWnd);
			break;

        default:
			ScreenSaverProc(hWnd, Message, wParam, lParam);
    }
    return DefWindowProc(hWnd, Message, wParam, lParam);
}

/*
    nCwRegisterClasses Function

    The following function  registers  all  the  classes  of  all  the
    windows associated with this application.  The function returns an
    error code if unsuccessful, otherwise it returns 0.
*/

static int nCwRegisterClasses(void)
{
    WNDCLASS wndclass;          /* struct to define a window class */
    memset(&wndclass, 0, sizeof(WNDCLASS));

    /* load WNDCLASS with window's characteristics */
    wndclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wndclass.lpfnWndProc = WndProc;
    /* Extra storage for Class and Window objects */
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hMainInstance;
    wndclass.hIcon = LoadIcon(hMainInstance, MAKEINTATOM(ID_APP));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    /* Create brush for erasing background */
    wndclass.hbrBackground = GetStockObject(BLACK_BRUSH);
    wndclass.lpszMenuName = appName;	// Menu name
    wndclass.lpszClassName = "WindowsScreenSaverClass"; // Class Name
    if (!RegisterClass(&wndclass)) {
        return -1;
	}

	//	Register class for full-screen test window

    wndclass.lpfnWndProc = GUZZWndProc;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "FullTest";
	if (!RegisterClass(&wndclass)) {
        return -1;
	}

    return 0;
}
#endif

