#include "define.h"

globals_t globals;
calculs_t calculs;
render_t render;

void InitOpenGL ( void )
{
	ilInit();
	iluInit();
	ilutRenderer ( ILUT_OPENGL );

	glShadeModel ( GL_SMOOTH );
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
	glClearDepth ( 1.0f );

	glDisable ( GL_CULL_FACE );
	glDisable ( GL_DEPTH_TEST );
	glEnable ( GL_BLEND );
	glBlendFunc ( GL_SRC_ALPHA, GL_ONE );

	/*
	glh_init_extensions ( "GL_ARB_texture_compression " \
						  "GL_EXT_texture_compression_s3tc " \
						  "GL_EXT_texture_filter_anisotropic " );
	*/
}

GLvoid ReSizeGLScene ( void )
{
	if ( globals.height == 0 ) {
		globals.height = 1;
	}

	glViewport ( 0, 0, globals.width, globals.height );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity();

	gluPerspective ( 45.0f, (float)globals.width/(float)globals.height, 0.1f, 10000.0f );

	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity();
}

void HandleCommand ( const LPARAM lParam, const WPARAM wParam )
{
	char buffer[512];
	switch ( LOWORD ( wParam ) ) {
	case IDM_SOURCE:
		globals.bSource = 1 - globals.bSource;
		if ( globals.bSource ) {
			AddLine ( "Sources are on.\r\n" );
		} else {
			AddLine ( "Sources are off.\r\n" );
		}
		break;
	case IDM_START:
		if ( !globals.bSimulate ) {
			globals.bSimulate = TRUE;
			AddLine ( "Starting simulation.\r\n" );
			UpdateMyMenus();
		}
		break;
	case IDM_SIMUL_FPS:
		if ( globals.bSimulate ) {
			sprintf ( buffer, "Seconds/frame : %.1f\r\n", 1.0/globals.fps );
			AddLine ( buffer );
		}
		break;
	case IDM_SIMUL_VIT:
		globals.bVelocity = 1 - globals.bVelocity;
		if ( globals.bVelocity ) {
			AddLine ( "Displaying velocity field\r\n" );
		} else {
			AddLine ( "Hiding velocity field\r\n" );
		}
		UpdateMyMenus();
		break;
	case IDM_DRAWBOX:
		globals.bDrawBox = 1 - globals.bDrawBox;
		break;
	case IDM_LINE:
		globals.bLine = 1 - globals.bLine;
		if ( globals.bLine ) {
			AddLine ( "Wireframe on\r\n" );
		} else {
			AddLine ( "Wireframe off\r\n" );
		}
		UpdateMyMenus();
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
		if ( globals.bSimulate ) {
			globals.bMovie = 1 - globals.bMovie;
			if ( globals.bMovie ) {
				AddLine ( "Recording movie...\r\n" );
				StartRecording();
			} else {
				StopRecording();
				AddLine ( "Finishing recording !\r\n" );
			}
		}
		break;
	case IDM_SCREENSHOT:
		TakeScreenshot();
		break;
	case IDM_EXPORT_DEVRT:
		ExportAsDevRT();
		break;
	case IDM_STOP:
		if ( globals.bSimulate ) {
			if ( globals.bMovie ) {
				StopRecording();
				AddLine ( "Finishing recording !\r\n" );
				globals.bMovie = FALSE;
			}
			globals.bSimulate = FALSE;
			AddLine ( "Simulation stopped.\r\n" );
			UpdateMyMenus();
		}
		break;
	case IDM_FILE_EXIT:
		SendMessage ( globals.hwnd, WM_CLOSE, 0, 0 );
		break;
	}
}

LRESULT CALLBACK MainWndProc ( HWND hWnd, UINT uMsg, UINT wParam, LONG lParam )
{
	switch ( uMsg ) {
	case WM_COMMAND:
		HandleCommand ( lParam, wParam );
		break;
	case WM_KEYDOWN:
		render.keys[wParam] = TRUE;
		break;
	case WM_KEYUP:
		render.keys[wParam] = FALSE;
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
	memset ( &render, 0, sizeof ( render_t ) );

	globals.width  = 640;
	globals.height = 480;
	globals.hInstance = hInst;

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

	GetWindowRect ( globals.hDlg, &rect );
	rect.right = GetSystemMetrics ( SM_CXSCREEN ) - ( rect.right - rect.left );
	rect.bottom = GetSystemMetrics ( SM_CYSCREEN ) - ( rect.bottom - rect.top ) - 30;
	SetWindowPos ( globals.hDlg, HWND_TOP, rect.right, rect.bottom, 0, 0, SWP_NOSIZE );
	
	ShowWindow ( globals.hwnd, nCmdShow );
    UpdateWindow ( globals.hwnd );

	srand( GetTickCount() );
	AVIFileInit();
	TimerInit();
	AddLine ( "Init OpenGL...\r\n" );
	InitOpenGL();
	ReSizeGLScene ();
	//globals.bUseCg = CgInit();
	if ( globals.bUseCg ) {
		AddLine ( "-> using cg\r\n" );
	}
	AddLine ( "Init model...\r\n" );
	char buffer[512];
	sprintf ( buffer, "Memory used : %.1f Mo\r\n", InitModel() );
	AddLine ( buffer );
	AddLine ( "Initialisation done !\r\n" );
	globals.bDrawBox = TRUE;

	UpdateMyMenus();

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
			HandleKeyboard();
			glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			if ( globals.bSimulate ) {
				if ( globals.bSource ) {
					//MakeSources();
					MakeBallSource();
				}
				calcul();
			}
			RenderScene();
			if ( globals.bMovie ) {
				globals.movie->TakeShot();
			}
			globals.fps = GetFPS ( );
			SwapBuffers ( hDC );
		}
	}

	FreeModel();
	if ( globals.bUseCg ) {
		CgShutdown();
	}

	//glh_shutdown_extensions();

    return msg.wParam;
}
