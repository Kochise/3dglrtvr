////////////////////////////////////////////////////////////////////////////////
//
//	WinMain.cpp - Main Window Initialization
//
//	Project:	3D Jello
//
//	Author:		Asen L. Kovatchev (aceh@bulinfo.net)
//	Created:	Nov 19, 1999
//
////////////////////////////////////////////////////////////////////////////////



#include "StdMain.h"
#include "DXApp.h"



////////////////////////////////////////////////////////////////////////////////
//	Constant Declarations

const char * cszAppTitle	= "3D Jello (aceh@bulinfo.net)";
const char * cszClassName	= "3D_Jello_DirectX_Demo";


////////////////////////////////////////////////////////////////////////////////
//	Global Variables

CDXApp *	g_pDXApp;
BOOL		g_bInitialized = FALSE;
BOOL		g_bActive = TRUE;
float		g_fClock = 0.0;


////////////////////////////////////////////////////////////////////////////////
//	Main Window Callback Procedure

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch ( uMsg )
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		// Release DirectX wrapper
		delete g_pDXApp;
		break;

	case WM_PAINT:
		break;

	case WM_KEYDOWN:
		g_pDXApp->m_bKey[wParam] = 1;
		break;

	case WM_KEYUP:
		if (wParam == 27)
		{
			PostQuitMessage(0);
		}

		if (wParam == VK_F1)
		{
			g_pDXApp->m_bShowHelp = !g_pDXApp->m_bShowHelp;
		}

		g_pDXApp->m_bKey[wParam] = 0;
		break;

	case WM_SIZE:
        if(g_bInitialized && LOWORD(lParam) > 0 && HIWORD(lParam) > 0)
        {
            HRESULT hr;

            if(FAILED(hr = g_pDXApp->m_pd3dx->Resize(LOWORD(lParam),HIWORD(lParam))))
            {
                g_bInitialized = FALSE;
                PostQuitMessage(0);
            }

            D3DXMatrixPerspectiveFov(&g_pDXApp->m_matProjection, D3DXToRadian(60.0f), 3.0f / 4.0f, 0.1f, 1000.0f);
            g_pDXApp->m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION, g_pDXApp->m_matProjection);
        }
        break;
	}

    return DefWindowProc(hwnd,uMsg,wParam,lParam);
}



////////////////////////////////////////////////////////////////////////////////
//	Program Main Entry Point

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT nCmdShow)
{
	WNDCLASS	wndClass;
	HWND		hWnd;
    MSG			msg;
	HRESULT		hr;

	// Register the window class
	wndClass.style			= CS_HREDRAW | CS_VREDRAW;
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_MAIN));
	wndClass.hInstance		= hInst;
	wndClass.hbrBackground	= NULL;
	wndClass.lpfnWndProc	= MainWndProc;
	wndClass.lpszMenuName	= NULL;
	wndClass.lpszClassName	= cszClassName;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	RegisterClass( &wndClass );

	// Create our main window
	hWnd = CreateWindowEx( WS_EX_TOPMOST,
						   cszClassName,
						   cszAppTitle,
						   WS_OVERLAPPEDWINDOW,	// for windowed mode
						   //WS_POPUP,			// for full screen
						   CW_USEDEFAULT,
						   CW_USEDEFAULT,
						   640,
						   480, 0L, 0L, hInst, 0L );

	// Initialize DirectX wrapper
	g_pDXApp = new CDXApp;
	hr = g_pDXApp->InitD3D( hWnd );

	if (FAILED( hr ))
	{
		char str[1000];
		D3DXGetErrorString( hr, sizeof(str), str );
		MessageBox( hWnd, str, "Error", MB_ICONERROR | MB_APPLMODAL | MB_OK);
		PostQuitMessage(0);
	}
	else
	{
		g_pDXApp->InitSceneObjects();
		g_bInitialized = TRUE;
	}

	// Show our main window
	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	// Implement Standard Windows Message Pump
	BOOL bGotMsg;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
	while( WM_QUIT != msg.message  )
	{
		bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );

		if( bGotMsg )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if (g_bActive && g_bInitialized)
			{
				g_fClock = g_fClock + 0.01f;
				g_pDXApp->FrameMove( g_fClock );
				g_pDXApp->RenderScene();
				g_pDXApp->ShowFrame();
        Sleep(10);
			}
			else
			{
				WaitMessage();
			}
		}
	}

	// Return back
	return msg.wParam;
}
