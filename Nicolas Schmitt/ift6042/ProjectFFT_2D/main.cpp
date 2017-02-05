/*
 *   Copyright (c) 2002 Nicolas Schmitt
 *
 *   This file is part of (s)Smoke.
 *
 *   (s)Smoke is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   (s)Smoke is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details (gpl.txt).
 *
 *   You should have received a copy of the GNU General Public License
 *   along with (s)Smoke; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "define.h"

globals_t globals;
calculs_t calculs;

void RenderGround ( void )
{
	glDisable ( GL_TEXTURE_2D );
	glDisable ( GL_BLEND );
	glColor3ub ( 198, 145, 53 );
	glBegin ( GL_POLYGON );
		glVertex3f (   0.0, 100.0, 50.0 );
		glVertex3f (   0.0, 100.0, -1000.0 );
		glVertex3f ( 1000.0, 100.0, -1000.0 );
		glVertex3f ( 1000.0, 100.0, 50.0 );
	glEnd();
}

void InitOpenGL ( void )
{
	ilInit();
	iluInit();
	ilutRenderer ( ILUT_OPENGL );
	globals.texture1 = ilutGLLoadImage ( TEXT_NAME1 );

	glBlendFunc ( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
	glDisable ( GL_CULL_FACE );
	glShadeModel ( GL_SMOOTH );
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
	glDisable ( GL_DEPTH_TEST );
}

GLvoid ReSizeGLScene ( void )
{
	if ( globals.height == 0 ) {
		globals.height = 1;
	}

	glViewport ( 0, 0, globals.width, globals.height );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity();

	//gluOrtho2D ( 0.0, (double)globals.width, 0.0, (double)globals.height );
	gluPerspective ( 45, (double)globals.width / (double)globals.height, 0.1f, 10000.0f );

	glDisable ( GL_CULL_FACE );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef ( -globals.width / 2.0f, -globals.height / 2.0f, -515.0f );
}

BOOL APIENTRY AboutProc ( HWND hDlg, UINT uMsg, UINT wParam, LONG lParam )
{
	if ( uMsg == WM_COMMAND ) {
		if ( LOWORD ( wParam ) == IDOK ) {
			EndDialog ( hDlg, TRUE );
		}
	}
	return FALSE;
}

void HandleCommand ( LPARAM lParam, WPARAM wParam )
{
	char buffer[512];
	switch ( LOWORD ( wParam ) ) {
	case IDM_SOURCE:
		globals.bSources = 1 - globals.bSources;
		if ( globals.bSources ) {
			AddLine ( "Sources are on\r\n" );
		} else {
			AddLine ( "Sources are off\r\n" );
		}
		break;
	case IDM_START:
		if ( !globals.bSimulate ) {
			globals.bSimulate = TRUE;
			AddLine ( "Starting simulation.\r\n" );
			UpdateMyMenus();
		}
		break;
	case IDM_TEMPERATURE:
		globals.bTemperature = 1 - globals.bTemperature;
		if ( globals.bTemperature ) {
			AddLine ( "Gravity and t° are on\r\n" );
		} else {
			AddLine ( "Gravity and t° are off\r\n" );
		}
		break;
	case IDM_SIMUL_FPS:
		if ( globals.bSimulate ) {
			wsprintf ( buffer, "FPS : %d\r\n", globals.fps );
			AddLine ( buffer );
		}
		break;
	case IDM_EXPORT_DEVRT:
		ExportAsRTFile ( "smoke.ray" );
		break;
	case IDM_SIMUL_VIT:
		globals.bVelocity = 1 - globals.bVelocity;
		if ( globals.bVelocity ) {
			AddLine ( "Displaying velocity field\r\n" );
		} else {
			AddLine ( "Hiding velocity field\r\n" );
		}
		break;
	case IDM_SIMUL_VORTCONF:
		globals.bUseConfinement = 1 - globals.bUseConfinement;
		if ( globals.bUseConfinement ) {
			AddLine ( "Velocity confinement enabled\r\n" );
		} else {
			AddLine ( "Velocity confinement disabled\r\n" );
		}
		break;
	case IDM_SIMUL_FILTRE:
		globals.bFiltre = 1 - globals.bFiltre;
		if ( globals.bFiltre ) {
			AddLine ( "Using filter\r\n" );
		} else {
			AddLine ( "Not using filter\r\n" );
		}
		break;
	case IDM_SIMUL_TEXT:
		globals.bUseTexture = 1 - globals.bUseTexture;
		if ( globals.bUseTexture ) {
			AddLine ( "Render using texture\r\n" );
		} else {
			AddLine ( "Render without texture\r\n" );
		}
		break;
	case IDM_TEXT1:
		glBindTexture ( GL_TEXTURE_2D, globals.texture1 );
		break;
	case IDM_LINE:
		globals.bLine = 1 - globals.bLine;
		if ( globals.bLine ) {
			AddLine ( "Wireframe on\r\n" );
		} else {
			AddLine ( "Wireframe off\r\n" );
		}
		break;
	case IDM_SIMUL_CLEAR:
		ResetSimulation();
		AddLine ( "Simulation reseted !\r\n" );
		break;
	case IDM_SIMUL_SAVE:
		SaveGrids ( "simulation.sm" );
		break;
	case IDM_SIMUL_LOAD:
		LoadGrids ( "simulation.sm" );
		break;
	case IDM_MOVIE:
		if ( !globals.bMovie ) {
			if ( StartRecording() ) {
				globals.bMovie = TRUE;
				AddLine ( "Recording movie...\r\n" );
			}
		} else {
			StopRecording();
			AddLine ( "Finishing recording !\r\n" );
			globals.bMovie = FALSE;
		}
		break;
	case IDM_SCREENSHOT:
		TakeScreenshot();
		break;
	case IDM_STOP:
		if ( globals.bSimulate ) {
			globals.bSimulate = FALSE;
			if ( globals.bCapture ) {
				globals.bCapture = FALSE;
			}
			AddLine ( "Simulation stopped.\r\n" );
			UpdateMyMenus();
		}
		break;
	case IDM_ABOUT:
		DialogBox ( globals.hInstance, MAKEINTRESOURCE ( IDD_DIALOG2 ), globals.hwnd, AboutProc );
		break;
	case IDM_FILE_EXIT:
		SendMessage ( globals.hwnd, WM_CLOSE, 0, 0 );
		break;
	}
	UpdateMyMenus();
}

LRESULT CALLBACK MainWndProc ( HWND hWnd, UINT uMsg, UINT wParam, LONG lParam )
{
	switch ( uMsg ) {
	case WM_COMMAND:
		HandleCommand ( lParam, wParam );
		break;
	case WM_LBUTTONDOWN:
		//if ( globals.bSimulate ) {
			globals.bCapture = TRUE;
			GetCursorPos ( &globals.oldp );
			ScreenToClient ( globals.hwnd, &globals.oldp );
		//}
		break;
	case WM_LBUTTONUP:
		if ( globals.bCapture ) {
			globals.bCapture = FALSE;
		}
		break;
	case WM_MOUSEMOVE:
		if ( globals.bCapture ) {
			POINT p;
			GetCursorPos ( &p );
			ScreenToClient ( globals.hwnd, &p );

			// deplacement
			double dx = p.x - globals.oldp.x;
			double dy = globals.oldp.y - p.y;
			globals.oldp.x = p.x;
			globals.oldp.y = p.y;
			p.y = globals.height - p.y;
			// le voxel ou se trouve la souris
			int i = (int)floor ( ( (double)p.x / (double)globals.width ) * (double)calculs.length[0] );
			int j = (int)floor ( ( (double)p.y / (double)globals.height ) * (double)calculs.length[1] );
			// on verifie bien qu'on est dans la limite
			if ( i < 0 ) i = 0;
			if ( i >= calculs.length[0] ) i = calculs.length[0] - 1;
			if ( j < 0 ) j = 0;
			if ( j >= calculs.length[1] ) j = calculs.length[1] - 1;

			calculs.source[ i + j * calculs.length[0] ] = 4.0f;
			calculs.chaleur[ i + j * calculs.length[0] ] = 5.0f;
			double len = sqrt ( dx * dx + dy * dy );
			if ( len != 0.0 ) {
				dx *= 0.1 / len;
				dy *= 0.1 / len;
			}
			calculs.f[0][ i + j * calculs.length[0] ] += dx; // peut etre qu'il faut
			calculs.f[1][ i + j * calculs.length[0] ] += dy; // mettre un facteur (10?)
		}
		break;
	case WM_SIZE:
		globals.width  = LOWORD ( lParam );
		globals.height = HIWORD ( lParam );
		ReSizeGLScene();
		if ( !globals.bSimulate ) {
			glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		}
		break;
 	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		SendMessage ( hWnd, WM_COMMAND, IDM_STOP, 0 );
		DestroyWindow ( globals.hEdit );
		DestroyWindow ( hWnd );
		break;
	default:
		return DefWindowProc ( hWnd, uMsg, wParam, lParam );
	}
	return 0;
}

BOOL APIENTRY DlgProc ( HWND hDlg, UINT uMsg, UINT wParam, LONG lParam )
{
    switch ( uMsg ) {
	case WM_INITDIALOG:
		globals.hEdit = GetDlgItem ( hDlg, IDC_EDIT );
		break;
	}
	return FALSE;
}

int WINAPI WinMain ( HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow )
{
	MSG msg;
    WNDCLASS wc;
	RECT rect;
	HACCEL hAccel;

	memset ( &globals, 0, sizeof ( globals_t ) );
	memset ( &calculs, 0, sizeof ( calculs_t ) );

	globals.width  = 640;
	globals.height = 480;
	globals.hInstance = hInst;
	globals.bUseConfinement = TRUE;
	globals.bTemperature = TRUE;

#ifndef _DEBUG
	//SetPriorityClass ( GetCurrentProcess(), HIGH_PRIORITY_CLASS );
#endif

	// Creating class
	if ( !hPrev ) {
		wc.lpszClassName = APPNAME;
        wc.hInstance     = hInst;
        wc.lpfnWndProc   = MainWndProc;
        wc.hCursor       = LoadCursor ( NULL, IDC_ARROW );
        wc.hIcon         = LoadIcon ( hInst, MAKEINTRESOURCE(IDI_ICON1) );
        wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
        wc.hbrBackground = NULL;
        wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;

		RegisterClass ( &wc );
	}

	globals.hwnd = CreateWindow ( APPNAME,
								  APPNAME,
								  WS_CAPTION	  |
								  WS_SYSMENU	  |
								  WS_MINIMIZEBOX  |
								  WS_MAXIMIZEBOX  |
								  WS_THICKFRAME   |
								  WS_CLIPCHILDREN |
								  WS_OVERLAPPED,
								  0, 0,
								  640, 480, NULL, NULL,
								  hInst, NULL );

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		32, 0,	0,
		PFD_MAIN_PLANE,
		0, 0, 0, 0
	};

	HDC hDC = GetDC ( globals.hwnd );
	int PixelFormat = ChoosePixelFormat ( hDC, &pfd );
	SetPixelFormat ( hDC, PixelFormat, &pfd );
	HGLRC hRC = wglCreateContext ( hDC );
	wglMakeCurrent ( hDC, hRC );

	globals.hDlg = CreateDialog ( globals.hInstance, MAKEINTRESOURCE ( IDD_DIALOG1 ),
								  globals.hwnd, DlgProc );

	UpdateMyMenus();

	GetWindowRect ( globals.hDlg, &rect );
	rect.right = GetSystemMetrics ( SM_CXSCREEN ) - ( rect.right - rect.left );
	rect.bottom = GetSystemMetrics ( SM_CYSCREEN ) - ( rect.bottom - rect.top ) - 30;
	SetWindowPos ( globals.hDlg, HWND_TOP, rect.right, rect.bottom, 0, 0, SWP_NOSIZE );
	
	ShowWindow ( globals.hwnd, nCmdShow );
    UpdateWindow ( globals.hwnd );

	TimerInit();
	AVIFileInit();
	InitOpenGL();
	ReSizeGLScene ();
	InitModel();

	hAccel = LoadAccelerators ( hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1) );

	while ( 1 ) {
		if ( PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			if ( msg.message == WM_QUIT ) {
				break;
			}
			if ( !TranslateAccelerator ( globals.hwnd, hAccel, &msg ) ) {
				TranslateMessage ( &msg );
				DispatchMessage ( &msg );
			}
		} else {
			glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			//RenderGround();
			if ( globals.bSimulate ) {
				globals.fps = GetFPS ( );
				if ( globals.bSources ) {
					//MakeSources();
					MakeSources2();
				}
				if ( globals.bTemperature ) {
					MakeForces();
				}
				calcul();
				RenderScene();
				if ( globals.bMovie ) {
					globals.movie->TakeShot();
				}
			}
			SwapBuffers ( hDC );
		}
	}

	FreeModel();

    return msg.wParam;
}
