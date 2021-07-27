/*
 * Copyright (C) 1999-2005  Terence M. Welsh
 *
 * This file is part of rsWin32Saver.
 *
 * rsWin32Saver is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * rsWin32Saver is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include <rsWin32Saver.h>


//#define WINVER 0x0500


// These variables are externed in rsWin32Saver.h so you can use them in your saver
HINSTANCE mainInstance = 0;
HWND mainWindow = 0;
int childPreview = 0;
int windowedSaver = 0;
int reallyClose = 0;
int checkingPassword = 0;
int isSuspended = 0;
int doingPreview = 0;
int pfd_swap_exchange = 0;
int pfd_swap_copy = 0;
unsigned int dFrameRateLimit = 0;
int kStatistics = 0;

POINT mousePoint;
int closing = 0;
int wakeThreshold = 4;  // must move 4 pixels to wake up
typedef BOOL (WINAPI* VERIFYPASSWORDPROC) (HWND hwnd);
typedef VOID (WINAPI* PASSWORDCHANGEPROC) (LPCSTR regKeyName, HWND hwnd, DWORD dwd, LPVOID lpv);

static int startScreenSaver(HWND parent);
static int openConfigBox(HWND parent);
static int startSaverPreview(LPCTSTR str);



//----------------------------------------------------------------------------
// Password functions (only needed on Win95 and Win98)
// I think this code still has trouble actually drawing the password checking box
// Maybe I should install Win95 and test it.  Ha!


BOOL checkPassword(HWND hwnd){
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize=sizeof(osvi);
	GetVersionEx(&osvi);
	if(osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS){  // this is Win95 or Win98
		HINSTANCE passwordcpl = LoadLibrary("PASSWORD.CPL");
		if(passwordcpl == NULL)
			return TRUE;
		VERIFYPASSWORDPROC verifyPwdProc;
		verifyPwdProc = (VERIFYPASSWORDPROC)GetProcAddress(passwordcpl, "VerifyScreenSavePwd");
		if(verifyPwdProc==NULL){
			FreeLibrary(passwordcpl);
			return TRUE;
		}
		BOOL junk;
		SystemParametersInfo(SPI_SCREENSAVERRUNNING, TRUE, &junk, 0);  // disable ctrl-alt-delete
		checkingPassword = TRUE;
		BOOL verified = verifyPwdProc(hwnd);
		checkingPassword = FALSE;
		SystemParametersInfo(SPI_SCREENSAVERRUNNING, FALSE, &junk, 0);
		FreeLibrary(passwordcpl);
		return verified;
	}

	return TRUE;
}


void changePassword(HWND hwnd){
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	GetVersionEx(&osvi);
	if(osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS){  // this is Win95 or Win98
		HINSTANCE mprdll = LoadLibrary("MPR.DLL");
		if(mprdll == NULL)
			return;
		PASSWORDCHANGEPROC PwdChangePassword = (PASSWORDCHANGEPROC)GetProcAddress(mprdll, "PwdChangePasswordA");
		if(PwdChangePassword == NULL){
			FreeLibrary(mprdll);
			return;
		}
		PwdChangePassword("SCRSAVE", hwnd, 0, 0);
		FreeLibrary(mprdll);
	}
}


//----------------------------------------------------------------------------


LRESULT DefScreenSaverProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
	static int firstMouseMove = 0;  // Used for 3dfx driver fix

	if(!childPreview && !closing){
		switch(msg){
		case WM_CLOSE:
			// un-suspend saver if it has been suspended
			isSuspended = 0;
			// We need to explicitly tell this program to close.
			// NT will sometimes send a WM_CLOSE on its own.
			if(reallyClose == 0)
				return 0;
			// Check password on Win95 or Win98 (NT takes care of this on its own)
			if(checkPassword(hwnd) == FALSE){
				GetCursorPos(&mousePoint);  // reset mouse position if not quitting
				return 0;
			}
			break;
		default:{
			POINT movePoint, checkPoint;
			if(checkingPassword)
				break;
			switch(msg){
			case WM_SHOWWINDOW:
				if(wparam)
					SetCursor(NULL);
				break;
			case WM_SETCURSOR:
				if(!windowedSaver)
					SetCursor(NULL);
				return TRUE;
			case WM_MOUSEMOVE:
				// Capture first mouse movement and do nothing.
				// This prevents 3dfx drivers from killing saver because
				// 3dfx drivers send extraneous WM_MOUSEMOVE message.
				if(firstMouseMove == 0)
					firstMouseMove = 1;
				else
					reallyClose = 1;
				// check for real movements
				GetCursorPos(&checkPoint);
				movePoint.x = mousePoint.x - checkPoint.x;
				if(movePoint.x < 0)
					movePoint.x *= -1;
				movePoint.y = mousePoint.y - checkPoint.y;
				if(movePoint.y < 0)
					movePoint.y *= -1;
				if((movePoint.x + movePoint.y) > wakeThreshold && !windowedSaver)
					PostMessage(hwnd, WM_CLOSE, 0, 0);
				else
					mousePoint = checkPoint;
				break;
			case WM_ACTIVATEAPP:
				if(wparam)
					break;
			case WM_KEYDOWN:
				if(wparam == 's' || wparam == 'S'){
					kStatistics = !kStatistics;
					break;
				}
				// else pass through
			case WM_SYSKEYDOWN:
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
				if(windowedSaver == 0){
					reallyClose = 1;
					PostMessage(hwnd, WM_CLOSE, 0, 0);
				}
				break;
            }
         }
      }
   }

   return DefWindowProc(hwnd, msg, wparam, lparam);
}


LRESULT WINAPI RealScreenSaverProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
	switch(msg){
	case WM_CREATE:
		// get the mouse position at startup
		GetCursorPos(&mousePoint);
		if(!childPreview)
			SetCursor(NULL);
		break;
	case WM_SYSCOMMAND:
		if(!childPreview){
			switch(wparam){
			// ignore these messages
			case SC_NEXTWINDOW:       // alt-tabs
			case SC_PREVWINDOW:       // shift-alt-tabs
			case SC_SCREENSAVE:       // other screensavers
				return FALSE;
			case SC_MONITORPOWER:     // turn off saver if monitor turns off
				// 1 = put monitor in low power mode
				// 2 = turn monitor off
				if((lparam == 1 || lparam == 2) && !windowedSaver)
					isSuspended = 1;
				break;
			}
		}
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		if(closing)
			return DefWindowProc(hwnd, msg, wparam, lparam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return ScreenSaverProc(hwnd, msg, wparam, lparam);
}


//-----------------------------------------------------------------------

// Choose the best pixel format possible, giving preference to harware
// accelerated modes.
void SetBestPixelFormat(HDC hdc){
	int moreFormats, score = 0, nPixelFormat = 1, bestPixelFormat = 0, temp;
	PIXELFORMATDESCRIPTOR pfd;

	// Try to find the best pixel format
	moreFormats = DescribePixelFormat(hdc, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	while(moreFormats){
		// Absolutely must have these 4 attributes
		if((pfd.dwFlags & PFD_SUPPORT_OPENGL)
			&& (pfd.dwFlags & PFD_DRAW_TO_WINDOW)
			&& (pfd.dwFlags & PFD_DOUBLEBUFFER)
			&& (pfd.iPixelType == PFD_TYPE_RGBA)){
			// If this pixel format is good, see if it's the best...
			temp = 0;
			// color depth and z depth?
			temp += pfd.cColorBits + 2 * pfd.cDepthBits;
			if(pfd.cColorBits > 16)
				temp += (16 - pfd.cColorBits) / 2;
			// hardware accelerated?
			if(pfd.dwFlags & PFD_GENERIC_FORMAT){
				if(pfd.dwFlags & PFD_GENERIC_ACCELERATED)
					temp += 1000;
			}
			else
				temp += 2000;
			// Compare score
			if(temp > score){
				score = temp;
				bestPixelFormat = nPixelFormat;
			}
		}
		// Try the next pixel format
		nPixelFormat++;
		moreFormats = DescribePixelFormat(hdc, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	}

	// Set the pixel format for the device context
	if(bestPixelFormat){
		DescribePixelFormat(hdc, bestPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
		SetPixelFormat(hdc, bestPixelFormat, &pfd);
		if(pfd.dwFlags & PFD_SWAP_EXCHANGE)
			pfd_swap_exchange = 1;
		else
			pfd_swap_exchange = 0;
		if(pfd.dwFlags & PFD_SWAP_COPY)
			pfd_swap_copy = 1;
		else
			pfd_swap_copy = 0;
	}
	else{  // Just in case a best pixel format wasn't found
		PIXELFORMATDESCRIPTOR defaultPfd = {
			sizeof(PIXELFORMATDESCRIPTOR),          // Size of this structure
			1,                                      // Version of this structure    
			PFD_DRAW_TO_WINDOW |                    // Draw to Window (not to bitmap)
			PFD_SUPPORT_OPENGL |					// Support OpenGL calls in window
			PFD_DOUBLEBUFFER,                       // Double buffered
			PFD_TYPE_RGBA,                          // RGBA Color mode
			24,                                     // Want 24bit color 
			0,0,0,0,0,0,                            // Not used to select mode
			0,0,                                    // Not used to select mode
			0,0,0,0,0,                              // Not used to select mode
			24,                                     // Size of depth buffer
			0,                                      // Not used to select mode
			0,                                      // Not used to select mode
			PFD_MAIN_PLANE,                         // Draw in main plane
			0,                                      // Not used to select mode
			0,0,0 };                                // Not used to select mode
		bestPixelFormat = ChoosePixelFormat(hdc, &defaultPfd);
		// ChoosePixelFormat is poorly documented and I don't trust it.
		// That's the main reason for this whole function.
		SetPixelFormat(hdc, bestPixelFormat, &defaultPfd);
		if(defaultPfd.dwFlags & PFD_SWAP_EXCHANGE)
			pfd_swap_exchange = 1;
		else
			pfd_swap_exchange = 0;
		if(defaultPfd.dwFlags & PFD_SWAP_COPY)
			pfd_swap_copy = 1;
		else
			pfd_swap_copy = 0;
	}
}


//-----------------------------------------------------------------


static int startScreenSaver(HWND parent){
	LPCTSTR windowClass = TEXT("WindowsScreenSaverClass");
	LPCTSTR windowTitle;
	WNDCLASS wclass;
	MSG msg;
	UINT style;
	UINT exStyle;
	int left, top, width, height;

	wclass.style          = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	wclass.lpfnWndProc    = RealScreenSaverProc;
	wclass.cbClsExtra     = 0;
	wclass.cbWndExtra     = 0;
	wclass.hInstance      = mainInstance;
	wclass.hIcon          = LoadIcon(mainInstance, MAKEINTATOM(ID_APP));
	wclass.hCursor        = NULL;
	wclass.hbrBackground  = HBRUSH(GetStockObject(BLACK_BRUSH));
	wclass.lpszMenuName   = NULL;
	wclass.lpszClassName  = windowClass;

	if(parent){
		RECT parentRect;
		GetClientRect(parent, &parentRect);
		left = 0;
		top = 0;
		width = parentRect.right;
		height = parentRect.bottom;
		style = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN;
		exStyle = 0;
		childPreview = 1;
		windowTitle = TEXT("Screensaver Preview");  // must differ from full screen window name
	}
	else{
		left = GetSystemMetrics(SM_XVIRTUALSCREEN);
		top = GetSystemMetrics(SM_YVIRTUALSCREEN);
		width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		style = WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		exStyle = WS_EX_TOPMOST;

		// if there is another normal screen saver instance, switch to it
		windowTitle = TEXT("Screensaver");  // must differ from preview window name
		HWND hOther = FindWindow(windowClass, windowTitle);

		if(hOther && IsWindow(hOther)){
			SetForegroundWindow(hOther);
			return 0;
		}
	}

	if(RegisterClass(&wclass))
		mainWindow = CreateWindowEx(exStyle, windowClass, windowTitle, style,
			left, top, width, height, parent, (HMENU)NULL, mainInstance, (LPVOID)NULL);

	if(mainWindow){
		if(!childPreview)
			SetForegroundWindow(mainWindow);

		// variables for limiting frame rate
		float desiredTimeStep = 0.0f;
		float timeRemaining = 0.0f;
		rsTimer timer;
		if(dFrameRateLimit)
			desiredTimeStep = 1.0f / float(dFrameRateLimit);
		
		while(1){
			// Message loop
			while(!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)){
				if(isSuspended)  // don't waste cycles if saver is suspended
					Sleep(1);
				if(dFrameRateLimit){  // frame rate is limited
					timeRemaining -= timer.tick();
					// don't allow underflow
					if(timeRemaining < -1000.0f)
						timeRemaining = 0.0f;
					if(timeRemaining > 0.0f){
						// wait some more
						if(timeRemaining > 0.001f)
							Sleep(1);
					}
					else{
						IdleProc();  // do idle processing (i.e. draw frames)
						timeRemaining += desiredTimeStep;
					}
				}
				else{  // frame rate is unbound (draw as fast as possible)
					IdleProc();  // do idle processing (i.e. draw frames)
					Sleep(0);
				}
			}
			// drop down to here once a message is found in the queue
			if(GetMessage(&msg, NULL, 0, 0)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				return msg.wParam;
		}
	}

	return msg.wParam;
}


// convert text to unsigned int
static UINT atoui(LPCTSTR str){
	UINT returnval = 0;
	while((*str >= TEXT('0')) && (*str <= TEXT('9')))
		returnval = ((returnval * 10) + (*str++ - TEXT('0')));
	return returnval;
}


static int startSaverPreview(LPCTSTR str){
	doingPreview = 1;

	// get parent handle from string
	HWND hParent = (HWND)(atoui(str));

	// only preview on a valid parent window (NOT full screen)
	return((hParent && IsWindow(hParent)) ? startScreenSaver(hParent) : -1);
}


static int startWindowedSaver(){
	LPCTSTR windowClass = TEXT("WindowsScreenSaverClass");
	LPCTSTR windowTitle;
	WNDCLASS wclass;
	MSG msg;
	UINT style;

	windowedSaver = 1;

	wclass.style          = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	//wclass.style          = 0;
	wclass.lpfnWndProc    = RealScreenSaverProc;
	wclass.cbClsExtra     = 0;
	wclass.cbWndExtra     = 0;
	wclass.hInstance      = mainInstance;
	wclass.hIcon          = LoadIcon(mainInstance, MAKEINTATOM(ID_APP));
	//wclass.hCursor        = NULL;
	wclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wclass.hbrBackground  = HBRUSH(GetStockObject(BLACK_BRUSH));
	wclass.lpszMenuName   = NULL;
	wclass.lpszClassName  = windowClass;

	style = WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE;

	// if there is another normal screen saver instance, switch to it
	windowTitle = TEXT("Windowed Screensaver");  // must differ from preview window name

	if(RegisterClass(&wclass))
		mainWindow = CreateWindow(windowClass, windowTitle, style,
			100, 100, 512, 480, NULL, (HMENU)NULL, mainInstance, (LPVOID)NULL);

	if(mainWindow){
		while(1){
			// Message loop
			while(!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)){
				// do idle processing (i.e. draw frames)
				IdleProc();
				Sleep(0);
			}
			// drop down to here once a message is found in the queue
			if(GetMessage(&msg, NULL, 0, 0)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				return msg.wParam;
		}
	}

	return msg.wParam;
}


static int openConfigBox(HWND parent){
	return DialogBox(mainInstance, MAKEINTRESOURCE(DLG_SCRNSAVECONFIGURE),
		parent, (DLGPROC)ScreenSaverConfigureDialog);
}


//-----------------------------------------------------------------------
// main function

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prevInst, LPSTR cmdLine, int cmdShow){
	LPCTSTR commandLine = cmdLine;
	HWND parent = 0;

	mainInstance = inst;

	while(1){
		switch(*commandLine){
		case TEXT(' '):  // skip over whitespace
		case TEXT('-'):
		case TEXT('/'):
			commandLine++;
			break;
		case TEXT('a'):  // change password on Win95
		case TEXT('A'):
			changePassword(parent);
			return -1;
		case TEXT('c'):  // open "settings" dialog box
		case TEXT('C'):
			return openConfigBox(GetForegroundWindow());
		case TEXT('\0'):
			return openConfigBox(NULL);
		case TEXT('p'):  // run the preview
		case TEXT('P'):
			do
				commandLine++;
			while(*commandLine == TEXT(' '));  // skip over whitespace (I'm not sure why)
			return startSaverPreview(commandLine);
		case TEXT('s'):  // run the saver
		case TEXT('S'):
			return startScreenSaver(NULL);
		case TEXT('w'):  // run windowed saver, for debugging purposes
		case TEXT('W'):
			return startWindowedSaver();
		default:
			return -1;
		}
	}

	return -1;
}


//----------------------------------------------------------------------------

// Functions for using frame rate limiter

void initFrameRateLimitSlider(HWND hdlg, int sliderID, int textID){
	SendDlgItemMessage(hdlg, sliderID, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(0), DWORD(120))));
	SendDlgItemMessage(hdlg, sliderID, TBM_SETPOS, 1, LPARAM(dFrameRateLimit));
	SendDlgItemMessage(hdlg, sliderID, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, sliderID, TBM_SETPAGESIZE, 0, LPARAM(10));
	char cval[16];
	sprintf(cval, "%d", dFrameRateLimit);
	SendDlgItemMessage(hdlg, textID, WM_SETTEXT, 0, LPARAM(cval));
}

void updateFrameRateLimitSlider(HWND hdlg, int sliderID, int textID){
	int ival = SendDlgItemMessage(hdlg, sliderID, TBM_GETPOS, 0, 0);
	char cval[16];
	sprintf(cval, "%d", ival);
	SendDlgItemMessage(hdlg, textID, WM_SETTEXT, 0, LPARAM(cval));
}

void readFrameRateLimitFromRegistry(){
	LONG result;
	HKEY skey;
	DWORD valtype, valsize;
	int val;

	// default value = no limit
	dFrameRateLimit = 0;

	result = RegOpenKeyEx(HKEY_CURRENT_USER, registryPath, 0, KEY_READ, &skey);
	if(result != ERROR_SUCCESS)
		return;
	valsize=sizeof(val);
	result = RegQueryValueEx(skey, "FrameRateLimit", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dFrameRateLimit = val;

	RegCloseKey(skey);
}

