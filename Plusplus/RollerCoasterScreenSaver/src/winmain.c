/*
 *	RollerCoaster2000
 *	Copyright (C) 2003 Plusplus (plusplus@free.fr)
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#define COBJMACROS

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <scrnsave.h>
#include <string.h>
#include <time.h>
#include <shlobj.h>

#include "roller.h"
#include "resource.h"

UINT_PTR timer = 0;
HDC hDC=NULL;
HGLRC hRC=NULL;
HWND hWnd=NULL;
HWND hWndSaver = NULL;
HWND hWndChild = NULL;
HINSTANCE hInstance;
HWND prevWnd;
int width = 1024;
int height = 768;
int bpp = 32;
int randomtrack = 0;
char *trackfile = "";
char *tracksdir = "";
int debug = 0;

int preview = 0; /* flag indicating whether we are in preview mode */
int changeDisplay = 1; /* flag indicating whether display resolution must be changed */

char *GetMSG(UINT msg)
{
	switch(msg)
	{
		case WM_NULL:
			return "WM_NULL";
		case WM_APP:
			return "WM_APP";
		case WM_ACTIVATE:
			return "WM_ACTIVATE";
		case WM_ACTIVATEAPP:
			return "WM_ACTIVATEAPP";
		case WM_ASKCBFORMATNAME:
			return "WM_ASKCBFORMATNAME";
		case WM_CANCELJOURNAL:
			return "WM_CANCELJOURNAL";
		case WM_CANCELMODE:
			return "WM_CANCELMODE";
		case WM_CAPTURECHANGED:
			return "WM_CAPTURECHANGED";
		case WM_CHANGECBCHAIN:
			return "WM_CHANGECBCHAIN";
		case WM_CHAR:
			return "WM_CHAR";
		case WM_CHARTOITEM:
			return "WM_CHARTOITEM";
		case WM_CHILDACTIVATE:
			return "WM_CHILDACTIVATE";
		case WM_CHOOSEFONT_GETLOGFONT:
			return "WM_CHOOSEFONT_GETLOGFONT";
		case WM_CHOOSEFONT_SETLOGFONT:
			return "WM_CHOOSEFONT_SETLOGFONT";
		case WM_CHOOSEFONT_SETFLAGS:
			return "WM_CHOOSEFONT_SETFLAGS";
		case WM_CLEAR:
			return "WM_CLEAR";
		case WM_CLOSE:
			return "WM_CLOSE";
		case WM_COMMAND:
			return "WM_COMMAND";
		case WM_COMPACTING:
			return "WM_COMPACTING";
		case WM_COMPAREITEM:
			return "WM_COMPAREITEM";
		case WM_CONTEXTMENU:
			return "WM_CONTEXTMENU";
		case WM_COPY:
			return "WM_COPY";
		case WM_COPYDATA:
			return "WM_COPYDATA";
		case WM_CREATE:
			return "WM_CREATE";
		case WM_CTLCOLORBTN:
			return "WM_CTLCOLORBTN";
		case WM_CTLCOLORDLG:
			return "WM_CTLCOLORDLG";
		case WM_CTLCOLOREDIT:
			return "WM_CTLCOLOREDIT";
		case WM_CTLCOLORLISTBOX:
			return "WM_CTLCOLORLISTBOX";
		case WM_CTLCOLORMSGBOX:
			return "WM_CTLCOLORMSGBOX";
		case WM_CTLCOLORSCROLLBAR:
			return "WM_CTLCOLORSCROLLBAR";
		case WM_CTLCOLORSTATIC:
			return "WM_CTLCOLORSTATIC";
		case WM_CUT:
			return "WM_CUT";
		case WM_DEADCHAR:
			return "WM_DEADCHAR";
		case WM_DELETEITEM:
			return "WM_DELETEITEM";
		case WM_DESTROY:
			return "WM_DESTROY";
		case WM_DESTROYCLIPBOARD:
			return "WM_DESTROYCLIPBOARD";
		case WM_DEVICECHANGE:
			return "WM_DEVICECHANGE";
		case WM_DEVMODECHANGE:
			return "WM_DEVMODECHANGE";
		case WM_DISPLAYCHANGE:
			return "WM_DISPLAYCHANGE";
		case WM_DRAWCLIPBOARD:
			return "WM_DRAWCLIPBOARD";
		case WM_DRAWITEM:
			return "WM_DRAWITEM";
		case WM_DROPFILES:
			return "WM_DROPFILES";
		case WM_ENABLE:
			return "WM_ENABLE";
		case WM_ENDSESSION:
			return "WM_ENDSESSION";
		case WM_ENTERIDLE:
			return "WM_ENTERIDLE";
		case WM_ENTERMENULOOP:
			return "WM_ENTERMENULOOP";
		case WM_ENTERSIZEMOVE:
			return "WM_ENTERSIZEMOVE";
		case WM_ERASEBKGND:
			return "WM_ERASEBKGND";
		case WM_EXITMENULOOP:
			return "WM_EXITMENULOOP";
		case WM_EXITSIZEMOVE:
			return "WM_EXITSIZEMOVE";
		case WM_FONTCHANGE:
			return "WM_FONTCHANGE";
		case WM_GETDLGCODE:
			return "WM_GETDLGCODE";
		case WM_GETFONT:
			return "WM_GETFONT";
		case WM_GETHOTKEY:
			return "WM_GETHOTKEY";
		case WM_GETICON:
			return "WM_GETICON";
		case WM_GETMINMAXINFO:
			return "WM_GETMINMAXINFO";
		case WM_GETTEXT:
			return "WM_GETTEXT";
		case WM_GETTEXTLENGTH:
			return "WM_GETTEXTLENGTH";
		case WM_HELP:
			return "WM_HELP";
		case WM_HOTKEY:
			return "WM_HOTKEY";
		case WM_HSCROLL:
			return "WM_HSCROLL";
		case WM_HSCROLLCLIPBOARD:
			return "WM_HSCROLLCLIPBOARD";
		case WM_ICONERASEBKGND:
			return "WM_ICONERASEBKGND";
		case WM_IME_CHAR:
			return "WM_IME_CHAR";
		case WM_IME_COMPOSITION:
			return "WM_IME_COMPOSITION";
		case WM_IME_COMPOSITIONFULL:
			return "WM_IME_COMPOSITIONFULL";
		case WM_IME_CONTROL:
			return "WM_IME_CONTROL";
		case WM_IME_ENDCOMPOSITION:
			return "WM_IME_ENDCOMPOSITION";
		case WM_IME_KEYDOWN:
			return "WM_IME_KEYDOWN";
		case WM_IME_KEYUP:
			return "WM_IME_KEYUP";
		case WM_IME_NOTIFY:
			return "WM_IME_NOTIFY";
		case WM_IME_SELECT:
			return "WM_IME_SELECT";
		case WM_IME_SETCONTEXT:
			return "WM_IME_SETCONTEXT";
		case WM_IME_STARTCOMPOSITION:
			return "WM_IME_STARTCOMPOSITION";
		case WM_INITDIALOG:
			return "WM_INITDIALOG";
		case WM_INITMENU:
			return "WM_INITMENU";
		case WM_INITMENUPOPUP:
			return "WM_INITMENUPOPUP";
		case WM_INPUTLANGCHANGE:
			return "WM_INPUTLANGCHANGE";
		case WM_INPUTLANGCHANGEREQUEST:
			return "WM_INPUTLANGCHANGEREQUEST";
		case WM_KEYDOWN:
			return "WM_KEYDOWN";
		case WM_KEYUP:
			return "WM_KEYUP";
		case WM_KILLFOCUS:
			return "WM_KILLFOCUS";
		case WM_LBUTTONDBLCLK:
			return "WM_LBUTTONDBLCLK";
		case WM_LBUTTONDOWN:
			return "WM_LBUTTONDOWN";
		case WM_LBUTTONUP:
			return "WM_LBUTTONUP";
		case WM_MBUTTONDBLCLK:
			return "WM_MBUTTONDBLCLK";
		case WM_MBUTTONDOWN:
			return "WM_MBUTTONDOWN";
		case WM_MBUTTONUP:
			return "WM_MBUTTONUP";
		case WM_MDIACTIVATE:
			return "WM_MDIACTIVATE";
		case WM_MDICASCADE:
			return "WM_MDICASCADE";
		case WM_MDICREATE:
			return "WM_MDICREATE";
		case WM_MDIDESTROY:
			return "WM_MDIDESTROY";
		case WM_MDIGETACTIVE:
			return "WM_MDIGETACTIVE";
		case WM_MDIICONARRANGE:
			return "WM_MDIICONARRANGE";
		case WM_MDIMAXIMIZE:
			return "WM_MDIMAXIMIZE";
		case WM_MDINEXT:
			return "WM_MDINEXT";
		case WM_MDIREFRESHMENU:
			return "WM_MDIREFRESHMENU";
		case WM_MDIRESTORE:
			return "WM_MDIRESTORE";
		case WM_MDISETMENU:
			return "WM_MDISETMENU";
		case WM_MDITILE:
			return "WM_MDITILE";
		case WM_MEASUREITEM:
			return "WM_MEASUREITEM";
		case WM_MENUCHAR:
			return "WM_MENUCHAR";
		case WM_MENUSELECT:
			return "WM_MENUSELECT";
		case WM_MOUSEACTIVATE:
			return "WM_MOUSEACTIVATE";
		case WM_MOUSEMOVE:
			return "WM_MOUSEMOVE";
/*		case WM_MOUSEHOVER:            ** those are defined in different headers between lcc and vc **
			return "WM_MOUSEHOVER";
		case WM_MOUSELEAVE:
			return "WM_MOUSELEAVE";
		case WM_MOUSEWHEEL:
			return "WM_MOUSEWHEEL";
*/		case WM_MOVE:
			return "WM_MOVE";
		case WM_MOVING:
			return "WM_MOVING";
		case WM_NCACTIVATE:
			return "WM_NCACTIVATE";
		case WM_NCCALCSIZE:
			return "WM_NCCALCSIZE";
		case WM_NCCREATE:
			return "WM_NCCREATE";
		case WM_NCDESTROY:
			return "WM_NCDESTROY";
		case WM_NCHITTEST:
			return "WM_NCHITTEST";
		case WM_NCLBUTTONDBLCLK:
			return "WM_NCLBUTTONDBLCLK";
		case WM_NCLBUTTONDOWN:
			return "WM_NCLBUTTONDOWN";
		case WM_NCLBUTTONUP:
			return "WM_NCLBUTTONUP";
		case WM_NCMBUTTONDBLCLK:
			return "WM_NCMBUTTONDBLCLK";
		case WM_NCMBUTTONDOWN:
			return "WM_NCMBUTTONDOWN";
		case WM_NCMBUTTONUP:
			return "WM_NCMBUTTONUP";
		case WM_NCMOUSEMOVE:
			return "WM_NCMOUSEMOVE";
		case WM_NCPAINT:
			return "WM_NCPAINT";
		case WM_NCRBUTTONDBLCLK:
			return "WM_NCRBUTTONDBLCLK";
		case WM_NCRBUTTONDOWN:
			return "WM_NCRBUTTONDOWN";
		case WM_NCRBUTTONUP:
			return "WM_NCRBUTTONUP";
		case WM_NEXTDLGCTL:
			return "WM_NEXTDLGCTL";
		case WM_NOTIFY:
			return "WM_NOTIFY";
		case WM_NOTIFYFORMAT:
			return "WM_NOTIFYFORMAT";
		case WM_PAINT:
			return "WM_PAINT";
		case WM_PAINTCLIPBOARD:
			return "WM_PAINTCLIPBOARD";
		case WM_PAINTICON:
			return "WM_PAINTICON";
		case WM_PALETTECHANGED:
			return "WM_PALETTECHANGED";
		case WM_PALETTEISCHANGING:
			return "WM_PALETTEISCHANGING";
		case WM_PARENTNOTIFY:
			return "WM_PARENTNOTIFY";
		case WM_PASTE:
			return "WM_PASTE";
/*		case WM_PENWINFIRST:
			return "WM_PENWINFIRST";
*/		case WM_PENWINLAST:
			return "WM_PENWINLAST";
		case WM_POWER:
			return "WM_POWER";
		case WM_POWERBROADCAST:
			return "WM_POWERBROADCAST";
		case WM_PRINT:
			return "WM_PRINT";
		case WM_PRINTCLIENT:
			return "WM_PRINTCLIENT";
		case WM_PSD_ENVSTAMPRECT:
			return "WM_PSD_ENVSTAMPRECT";
		case WM_PSD_GREEKTEXTRECT:
			return "WM_PSD_GREEKTEXTRECT";
		case WM_PSD_MARGINRECT:
			return "WM_PSD_MARGINRECT";
		case WM_PSD_MINMARGINRECT:
			return "WM_PSD_MINMARGINRECT";
		case WM_PSD_PAGESETUPDLG:
			return "WM_PSD_PAGESETUPDLG";
		case WM_PSD_YAFULLPAGERECT:
			return "WM_PSD_YAFULLPAGERECT";
		case WM_QUERYDRAGICON:
			return "WM_QUERYDRAGICON";
		case WM_QUERYENDSESSION:
			return "WM_QUERYENDSESSION";
		case WM_QUERYNEWPALETTE:
			return "WM_QUERYNEWPALETTE";
		case WM_QUERYOPEN:
			return "WM_QUERYOPEN";
		case WM_QUEUESYNC:
			return "WM_QUEUESYNC";
		case WM_QUIT:
			return "WM_QUIT";
		case WM_RBUTTONDBLCLK:
			return "WM_RBUTTONDBLCLK";
		case WM_RBUTTONDOWN:
			return "WM_RBUTTONDOWN";
		case WM_RBUTTONUP:
			return "WM_RBUTTONUP";
		case WM_RENDERALLFORMATS:
			return "WM_RENDERALLFORMATS";
		case WM_RENDERFORMAT:
			return "WM_RENDERFORMAT";
		case WM_SETCURSOR:
			return "WM_SETCURSOR";
		case WM_SETFOCUS:
			return "WM_SETFOCUS";
		case WM_SETFONT:
			return "WM_SETFONT";
		case WM_SETHOTKEY:
			return "WM_SETHOTKEY";
		case WM_SETICON:
			return "WM_SETICON";
		case WM_SETREDRAW:
			return "WM_SETREDRAW";
		case WM_SETTEXT:
			return "WM_SETTEXT";
		case WM_SETTINGCHANGE:
			return "WM_SETTINGCHANGE";
		case WM_SHOWWINDOW:
			return "WM_SHOWWINDOW";
		case WM_SIZE:
			return "WM_SIZE";
		case WM_SIZECLIPBOARD:
			return "WM_SIZECLIPBOARD";
		case WM_SIZING:
			return "WM_SIZING";
		case WM_SPOOLERSTATUS:
			return "WM_SPOOLERSTATUS";
		case WM_STYLECHANGED:
			return "WM_STYLECHANGED";
		case WM_STYLECHANGING:
			return "WM_STYLECHANGING";
		case WM_SYSCHAR:
			return "WM_SYSCHAR";
		case WM_SYSCOLORCHANGE:
			return "WM_SYSCOLORCHANGE";
		case WM_SYSCOMMAND:
			return "WM_SYSCOMMAND";
		case WM_SYSDEADCHAR:
			return "WM_SYSDEADCHAR";
		case WM_SYSKEYDOWN:
			return "WM_SYSKEYDOWN";
		case WM_SYSKEYUP:
			return "WM_SYSKEYUP";
		case WM_TCARD:
			return "WM_TCARD";
		case WM_TIMECHANGE:
			return "WM_TIMECHANGE";
		case WM_TIMER:
			return "WM_TIMER";
		case WM_UNDO:
			return "WM_UNDO";
		case WM_USERCHANGED:
			return "WM_USERCHANGED";
		case WM_VKEYTOITEM:
			return "WM_VKEYTOITEM";
		case WM_VSCROLL:
			return "WM_VSCROLL";
		case WM_VSCROLLCLIPBOARD:
			return "WM_VSCROLLCLIPBOARD";
		case WM_WINDOWPOSCHANGED:
			return "WM_WINDOWPOSCHANGED";
		case WM_WINDOWPOSCHANGING:
			return "WM_WINDOWPOSCHANGING";
		case WM_KEYLAST:
			return "WM_KEYLAST";
		case WM_NEXTMENU:
			return "WM_NEXTMENU";
		case 136:
			return "WM_CTLCOLORDLG";
	}
	return "";
}


static LRESULT CALLBACK ScreenSaverChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
			// make current in case user wishes to delete lists (for example)
			// wglMakeCurrent(hDC, hRC);
			wglMakeCurrent(NULL,NULL);
			PostQuitMessage(0);
			break;

		case WM_ERASEBKGND:
			return TRUE;

		case WM_SIZE:
			width  = LOWORD(lParam);
			height = HIWORD(lParam);
			if( wglMakeCurrent(hDC, hRC) )
			{
				ReSizeGLScene(width,height);
			}
			break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			// Call users OpenGL drawing code (they are responsible
			// for buffer swap, for flexibility)

			// Make rendering context current
			if( wglMakeCurrent(hDC, hRC) )
			{
				DrawRoller();
				SwapBuffers(hDC);
			}
			EndPaint(hWnd, &ps);
		}
		break;

		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
		case WM_KEYDOWN:
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_MOUSEMOVE:
		case WM_NCACTIVATE:
		case WM_RBUTTONDOWN:
		case WM_SETCURSOR:
		case WM_SYSCOMMAND:
		case WM_SYSKEYDOWN:
		{
			LRESULT result;
			result = DefScreenSaverProc(hWnd, message, wParam, lParam);
			DefScreenSaverProc(hWndSaver, message, wParam, lParam);
			return result;
		}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void KillGLWindow(void)
{
	if(changeDisplay)
	{
		ChangeDisplaySettings(NULL,0);
	}
	ShowCursor(TRUE);
	if(hRC)
	{
		wglMakeCurrent(hDC,NULL);
		wglDeleteContext(hRC);
		hRC=NULL;
	}
	if(hDC && !ReleaseDC(hWndSaver,hDC))
	{
		hDC=NULL;
	}
//	if(hWnd && !DestroyWindow(hWnd))
//	{
//		hWnd=NULL;
//	}
	if(!UnregisterClass("OpenGL",hInstance))
	{
		hInstance=NULL;
	}
}


int CreateGLWindow(char* title, int width, int height, int bits)
{
	int PixelFormat;
	WNDCLASS wc;
	DEVMODE dmScreenSettings;
	PIXELFORMATDESCRIPTOR pfd;

	memset(&wc,0,sizeof(WNDCLASS));
	hInstance			= GetModuleHandle(NULL);
	wc.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;	// Own DC For Window
	wc.lpfnWndProc		= (WNDPROC)ScreenSaverChildProc;		// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= NULL;									// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL,IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name
	RegisterClass(&wc);

	hWndChild = CreateWindow(
		"OpenGL",
		title,
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0, width, height,
		hWndSaver,
		NULL,
		hInstance,
		NULL
	);

	hDC=GetDC(hWndChild);

	memset(&pfd,0,sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	if(changeDisplay) pfd.cColorBits = pfd.cDepthBits = bits;
	pfd.iLayerType = PFD_MAIN_PLANE;

	PixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, PixelFormat, &pfd);

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	if(changeDisplay)
	{
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = bits;
		dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		if(ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card.","ERROR",MB_OK|MB_ICONSTOP);
			return FALSE;
		}
	}
	return TRUE;
}


void defaultInit()
{
	width = 800;
	height = 600;
	bpp = 32;
	trackfile = "";
	randomtrack = 0;
	tracksdir = "";
	debug = 0;
	preview = 0;
	changeDisplay = 1;
}


int WriteRegistry()
{
	HKEY key;
	DWORD disposition;

	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\Plusplus\\RollerCoaster2000ScreenSaver",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&key,&disposition))
	{
		return 0;
	}
	RegSetValueEx(key,"TrackFile",0,REG_SZ,(BYTE*)trackfile,strlen(trackfile)+1);
	RegSetValueEx(key,"RandomTrack",0,REG_DWORD,(BYTE*)&randomtrack,sizeof(randomtrack));
	if(randomtrack)
	{
		RegSetValueEx(key,"TracksDirectory",0,REG_SZ,(BYTE*)tracksdir,strlen(tracksdir)+1);
	}
	RegCloseKey(key);
	return 1;
}


int ReadRegistry()
{
	HKEY key;
	DWORD valueType, valueSize;
	static char buf[1024];
	static char dirbuf[1024];

	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Plusplus\\RollerCoaster2000ScreenSaver",0,KEY_ALL_ACCESS,&key))
	{
		return 0;
	}
	valueSize = sizeof(buf);
	if(ERROR_SUCCESS != RegQueryValueEx(key,"TrackFile",0,&valueType,(LPBYTE)buf,&valueSize))
	{
		RegCloseKey(key);
		return 0;
	}
	else
	{
		trackfile = buf;
	}
	valueSize = sizeof(randomtrack);
	if(ERROR_SUCCESS != RegQueryValueEx(key,"RandomTrack",0,&valueType,(LPBYTE)&randomtrack,&valueSize))
	{
		RegCloseKey(key);
		return 0;
	}
	valueSize = sizeof(dirbuf);
	if(ERROR_SUCCESS != RegQueryValueEx(key,"TracksDirectory",0,&valueType,(LPBYTE)dirbuf,&valueSize))
	{
		tracksdir = "";
	}
	else
	{
		tracksdir = dirbuf;
	}
	RegCloseKey(key);
	return 1;
}


BOOL CALLBACK RegisterDialogClasses(HANDLE hInst)
{
	return TRUE;
}


BOOL CALLBACK ScreenSaverConfigureDialog(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char filename_buf[1024];
	static char tracksdir_buf[1024];

	switch (msg)
	{
	case WM_INITDIALOG:
		// Read the current configuration from the registry
		if(!ReadRegistry())
			defaultInit();
		SendMessage(GetDlgItem(hwndDlg,IDD_EDIT),WM_SETTEXT,(WPARAM)0,(LPARAM)trackfile);
		SendMessage(GetDlgItem(hwndDlg,IDD_EDIT_DIR),WM_SETTEXT,(WPARAM)0,(LPARAM)tracksdir);
		if(randomtrack)
		{
			CheckDlgButton(hwndDlg, IDD_RANDOMTRACKS, BST_CHECKED);
			EnableWindow(GetDlgItem(hwndDlg,IDD_EDIT),FALSE);
			EnableWindow(GetDlgItem(hwndDlg,IDD_BROWSE),FALSE);
		}
		else
		{
			CheckDlgButton(hwndDlg, IDD_RANDOMTRACKS, BST_UNCHECKED);
			EnableWindow(GetDlgItem(hwndDlg,IDD_EDIT_DIR),FALSE);
			EnableWindow(GetDlgItem(hwndDlg,IDD_BROWSE_DIR),FALSE);

		}
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDOK:
				SendMessage(GetDlgItem(hwndDlg,IDD_EDIT),WM_GETTEXT,(WPARAM)sizeof(filename_buf),(LPARAM)filename_buf);
				trackfile = filename_buf;
				SendMessage(GetDlgItem(hwndDlg,IDD_EDIT_DIR),WM_GETTEXT,(WPARAM)sizeof(tracksdir_buf),(LPARAM)tracksdir_buf);
				tracksdir = tracksdir_buf;
				if(BST_CHECKED == IsDlgButtonChecked(hwndDlg,IDD_RANDOMTRACKS))
				{
					randomtrack = 1;
				}
				else
				{
					randomtrack = 0;
				}
				WriteRegistry();
				EndDialog(hwndDlg,1);
				return TRUE;
			case IDCANCEL:
				EndDialog(hwndDlg,0);
				return TRUE;
			case IDD_RANDOMTRACKS:
				if(HIWORD(wParam) == BN_CLICKED)
				{
					int checked = (BST_CHECKED == IsDlgButtonChecked(hwndDlg,IDD_RANDOMTRACKS));
					CheckDlgButton(hwndDlg,IDD_RANDOMTRACKS, checked ? BST_UNCHECKED : BST_CHECKED);
					EnableWindow(GetDlgItem(hwndDlg,IDD_EDIT),checked);
					EnableWindow(GetDlgItem(hwndDlg,IDD_BROWSE),checked);
					EnableWindow(GetDlgItem(hwndDlg,IDD_EDIT_DIR),!checked);
					EnableWindow(GetDlgItem(hwndDlg,IDD_BROWSE_DIR),!checked);
					return TRUE;
				}
				return FALSE;
			case IDD_BROWSE:
				if(HIWORD(wParam) == BN_CLICKED)
				{
					OPENFILENAME ofn;
					static char buf[1024]="";
					FILE* file;
					/* the explorer open dialog displays the current file only if it can be opened */
					file = fopen(trackfile,"r");
					if(file != NULL)
					{
						strncpy(buf,trackfile,sizeof(buf));
						fclose(file);
					}
					else
					{
						strncpy(buf,"",sizeof(buf));
					}
					memset(&ofn,0,sizeof(OPENFILENAME));
					ofn.lStructSize = sizeof(OPENFILENAME);
					ofn.hwndOwner = hwndDlg;
					ofn.hInstance = hInstance;
					ofn.lpstrFilter = "All\0*\0Rollercoaster track file (*.trk)\0*.trk\0";
					ofn.nFilterIndex = 1;
					ofn.lpstrFile = buf;
					ofn.nMaxFile = sizeof(buf);
					ofn.lpstrTitle = "Select a rollercoaster track file";
					ofn.Flags = OFN_HIDEREADONLY;
					if(GetOpenFileName(&ofn))
					{
						SendMessage(GetDlgItem(hwndDlg,IDD_EDIT),WM_SETTEXT,(WPARAM)0,(LPARAM)buf);
						trackfile = buf;
					}
					return TRUE;
				}
				return FALSE;
			case IDD_BROWSE_DIR:
				if(HIWORD(wParam) == BN_CLICKED)
				{
					BROWSEINFO bi;
					LPITEMIDLIST lpidlist = NULL;
					LPMALLOC pmalloc = NULL;
					static char buf[1024]="";
					
					strncpy(buf,tracksdir,sizeof(buf));
					memset(&bi,0,sizeof(LPBROWSEINFO));
					bi.hwndOwner = hwndDlg;
					bi.pidlRoot = NULL;
					bi.pszDisplayName = buf;
					bi.lpszTitle = "Select the directory containing rollercoaster track files";
					bi.ulFlags = BIF_RETURNONLYFSDIRS;
					bi.lpfn = NULL;
					if(lpidlist = SHBrowseForFolder(&bi))
					{
						SHGetMalloc(&pmalloc);
						SHGetPathFromIDList(lpidlist, buf);
						SendMessage(GetDlgItem(hwndDlg,IDD_EDIT_DIR),WM_SETTEXT,(WPARAM)0,(LPARAM)buf);
						tracksdir = buf;
						IMalloc_Free(pmalloc,lpidlist);
						IMalloc_Release(pmalloc);
					}
					return TRUE;
				}
				return FALSE;

		}
		break;
	case WM_CLOSE:
		EndDialog(hwndDlg,0);
		return TRUE;
	}
	return FALSE;
}


void ChooseRandomTrack()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE SearchHandle;
	int NbTracks = 0;
	int Selected;
	static char buf[1024];

	SetCurrentDirectory(tracksdir);
	SearchHandle = FindFirstFile("*.trk", &FindFileData);
	if(SearchHandle != INVALID_HANDLE_VALUE)
	{
		NbTracks = 1;
		while( FindNextFile(SearchHandle, &FindFileData) )
		{
			NbTracks++;
		}
		FindClose(SearchHandle);
		srand(time(NULL));
		Selected = rand() % NbTracks;
		SearchHandle = FindFirstFile("*.trk", &FindFileData);
		while(Selected)
		{
			FindNextFile(SearchHandle, &FindFileData);
			Selected--;
		}
		strncpy(buf, FindFileData.cFileName, sizeof(buf));
		trackfile = buf;
		FindClose(SearchHandle);
	}
}


LRESULT CALLBACK ScreenSaverProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	HDC hdc;
	MSG m;
	POINT cursorPos;

	switch (message)
	{
		case WM_CREATE:
			hWndSaver = hWnd;

			if(!ReadRegistry())
				defaultInit();
			if(randomtrack)
				ChooseRandomTrack();
			// get screen size
			GetClientRect(hWnd,&rect);
			hdc = GetDC(hWnd);
			ReleaseDC(hWnd,hdc);
			changeDisplay = 0;
			if( rect.right != GetSystemMetrics(SM_CXSCREEN) || rect.bottom != GetSystemMetrics(SM_CYSCREEN) )
			{
				changeDisplay = 0;
				width  = rect.right;
				height = rect.bottom;
			}
			else
			if(width == -1 || height == -1 || changeDisplay == 0)
			{
				changeDisplay = 0;
				width  = rect.right;
				height = rect.bottom;
			}
			ParseRollerFile(trackfile);

			GetCursorPos(&cursorPos);

			CreateGLWindow("RollerCoaster 2000",width,height,bpp);
			InitializeRoller(width,height,0,60.0f,0.005f,0.05f);
			// Request 1ms timer resolution
			timeBeginPeriod(1);
			// Set timer interval for refresh to 60Hz
			timer = SetTimer(hWnd, 1, 1000/60, NULL);

			SetCursorPos(cursorPos.x, cursorPos.y);
			break;

		case WM_DESTROY:
			KillGLWindow();
			PostQuitMessage(0);
			// Finished with timer
			KillTimer(hWnd, timer);
			timeEndPeriod(1);
			break;

		case WM_TIMER:
			InvalidateRect(hWndChild, NULL, FALSE);
			// yield a little
			Sleep(1);
			for( ; PeekMessage(&m, hWnd, WM_TIMER, WM_TIMER, PM_REMOVE) ; );
			break;

		default:
			return DefScreenSaverProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

