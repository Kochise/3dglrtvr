#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <gl\gl.h>

#include "GLWindow.hpp"
#include "Log.hpp"
#include "FileProcs.hpp"

char szClassName[] = "OpenGL Window";

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);

OPENGLWINDOW::OPENGLWINDOW(char *configfilename, HINSTANCE hThisInstance, int nFunsterStil) {
	WNDCLASSEX wincl;
  PIXELFORMATDESCRIPTOR pfd;
  DWORD	 dwExStyle;				//Window extended style
  DWORD	 dwStyle;				//Window style
  DEVMODE dmScreenSettings;								// Device Mode
  int iFormat;
  bool Fullscreen = true;
  int Bbp = 32;

  // Used for fonts, don't allocate anything if we don't need it.
  FontBaseList = 0;

  FileDecoder *fd = new FileDecoder(configfilename);
  if (!fd->ReadString("<gfx>")) fd->LogFileError("Missing start tag <gfx>");
  SizeX = fd->ReadTagInt("sizex");
  SizeY = fd->ReadTagInt("sizey");
  Bbp = fd->ReadTagInt("bbp");
  Fullscreen = fd->ReadTagInt("fullscreen");
  if (!fd->ReadString("</gfx>")) fd->LogFileError("Missing end tag </gfx>");
  delete fd;

  // Now open the window
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof (WNDCLASSEX);
	wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
	if (!RegisterClassEx (&wincl)) return;

	if (Fullscreen) {
		Log("Fullscreen...");
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= SizeX;//fd->ReadTagInt("sizex");				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= SizeY;//fd->ReadTagInt("sizey");				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= Bbp;//fd->ReadTagInt("bbp");					  // Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL) {
			Log("... Fail, can't enter full screen mode");
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			//Window extended style
			dwStyle	 = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW;							//Windows style
			Fullscreen = false;
		}	else {
			dwExStyle= WS_EX_APPWINDOW;				//Window extended style
			dwStyle	 = WS_POPUP | WS_VISIBLE;					//Windows style
			ShowCursor(false);						//Hide the mouse pointer
		}
	}

	hwnd = CreateWindowEx(
		dwExStyle,                   /* Extended possibilites for variation */
		szClassName,         /* Classname */
		"OpenGL WIndow",       /* Title Text */
		dwStyle, /* default window */
		CW_USEDEFAULT,       /* Windows decides the position */
		CW_USEDEFAULT,       /* where the window ends up on the screen */
		SizeX,		/* The programs width */
		SizeY,	/* and height in pixels */
		Fullscreen ? NULL : HWND_DESKTOP,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hThisInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
	);

	ShowWindow (hwnd, nFunsterStil);
	
	// Initialize OpenGL

	Log("Initialize OpenGL...");
  // Screen HDC
  hDC = GetDC(hwnd);
  // Pixel Format
  ZeroMemory(&pfd, sizeof(pfd));
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cDepthBits = 16;
  pfd.iLayerType = PFD_MAIN_PLANE;
  iFormat = ChoosePixelFormat(hDC,&pfd);
  SetPixelFormat(hDC, iFormat, &pfd);
  // create and enable the render context (RC)
  hRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, hRC);

  // Setup coords and projection
  glViewport(0, 0, SizeX, SizeY);       // Reset The Current Viewport
  glMatrixMode(GL_PROJECTION);          // Select The Projection Matrix
  glLoadIdentity();                     // Reset The Projection Matrix
  // Calculate The Aspect Ratio Of The Window
  perspectiveGL(45.0f,(GLfloat)SizeX/(GLfloat)SizeY,0.1f,DEPTH_PLANE);
	
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Perspective Calculations

  glMatrixMode(GL_MODELVIEW);           // Select The Modelview Matrix
  glLoadIdentity();                     // Reset The Modelview Matrix

  glShadeModel(GL_SMOOTH);                // Enables Smooth Shading

  // glAlphaFunc(GL_GREATER, 0.5f);       // Type of alphafunction to use
  // glEnable(GL_ALPHA_TEST);             // Enable Alpha testing

  glClearDepth(1.0f);                  // Depth Buffer Setup
  glEnable(GL_DEPTH_TEST);             // Enables Depth Testing
  glDepthFunc(GL_LEQUAL);              // The Type Of Depth Test To Do

  glEnable(GL_BLEND);                   // Enable Blending (Alpha channel)
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Type of blend function

  glFrontFace(GL_CW);                   // Define frontface
  glCullFace(GL_BACK);                  // What is backside
  glEnable(GL_CULL_FACE);               // Hide backside.

  glPolygonMode(GL_FRONT, GL_FILL);     // Draw mode
  // glPolygonMode(GL_BACK, GL_FILL);     // Back Face Is Filled In
  // glPolygonMode(GL_FRONT, GL_LINE);    // Front Face Is Drawn With Lines

  GLfloat fogcolor[4] = {0.0f,0.0f,0.0f,0.0f};
  glClearColor(fogcolor[0],fogcolor[1],fogcolor[2],fogcolor[3]);
  glFogi(GL_FOG_MODE, GL_LINEAR);        // GL_EXP, GL_EXP2 or GL_LINEAR
  glFogfv(GL_FOG_COLOR, fogcolor);
  glFogf(GL_FOG_DENSITY, 1.0f);
  glHint(GL_FOG_HINT, GL_FASTEST);     // GL_DONT_CARE, GL_NICEST or GL_FASTEST
  glFogf(GL_FOG_START, DEPTH_PLANE * 0.9);
  glFogf(GL_FOG_END, DEPTH_PLANE);
  glEnable(GL_FOG);

  Frustum = new FRUSTUM();

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // Black Background
};

OPENGLWINDOW::~OPENGLWINDOW() {
  if (FontBaseList!=0) DeleteFont();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hRC);
  ReleaseDC(hwnd,hDC );
  if (Frustum!=0) delete Frustum;
}

void OPENGLWINDOW::Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear Screen And Depth Buffer
}

void OPENGLWINDOW::Swap() {
  SwapBuffers(hDC);
}

/****** Fonts *******/

void OPENGLWINDOW::BuildFont() {
  HFONT font;                   // Windows Font ID
  HFONT	oldfont;                // Used For Good House Keeping

  FontBaseList = glGenLists(96);        // Storage For 96 Characters
  font = CreateFont(
    -12,                        // Height Of Font
    0,                          // Width Of Font
    0,                          // Angle Of Escapement
    0,                          // Orientation Angle
    FW_BOLD,                    // Font Weight
    FALSE,                      // Italic
    FALSE,                      // Underline
    FALSE,                      // Strikeout
    ANSI_CHARSET,               // Character Set Identifier
    OUT_TT_PRECIS,              // Output Precision
    CLIP_DEFAULT_PRECIS,        // Clipping Precision
    ANTIALIASED_QUALITY,        // Output Quality
    FF_DONTCARE|DEFAULT_PITCH,  // Family And Pitch
    "Courier New");             // Font Name

  oldfont = (HFONT)SelectObject(hDC, font); // Selects The Font We Want
  wglUseFontBitmaps(hDC, 32, 96, FontBaseList);     // Builds 96 Characters Starting At Character 32

  SelectObject(hDC, oldfont);               // Selects The Font We Want
  DeleteObject(font);                       // Delete The Font
}

void OPENGLWINDOW::DeleteFont() {
 	if (FontBaseList!=0) glDeleteLists(FontBaseList, 96);      // Delete All 96 Characters
}

void OPENGLWINDOW::printAt(int x, int y, const char *fmt, ...) {
  char buf[256];
  va_list arg;
  if (fmt == NULL)
    return;
  va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
  va_end(arg);

  if (FontBaseList==0) BuildFont();

  glPushMatrix();
  glPushAttrib(GL_LIST_BIT);

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_FOG);
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);

  glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
  glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f,SizeX, SizeY, 0.0f,-1.0f,1.0f);
  //glColor3f(1.0,1.0,1.0);
  glRasterPos2f(x,y);
  glListBase(FontBaseList - 32);                    // Sets The Base Character to 32
  glCallLists(strlen(buf), GL_UNSIGNED_BYTE, buf);  // Draws The Display List Text
  glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

  glPopAttrib();                                    // Pops The Display List Bits
  glPopMatrix();
}

/*** End of Fonts ***/

// Replaces gluPerspective. Sets the frustum to perspective mode.
// fovY		- Field of vision in degrees in the y direction
// aspect	- Aspect ratio of the viewport
// zNear	- The near clipping distance
// zFar		- The far clipping distance
void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar ) {
  const GLdouble pi = 3.1415926535897932384626433832795;
  GLdouble fW, fH;
  fH = tan( fovY / 180 * pi ) * zNear / 2;
  fW = fH * aspect;
  glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

/***********/

/*
  Frustum is based on the code from:
  http://www.markmorley.com/opengl/frustumculling.html

  This is a very powerfull way to do some cliping...
*/

FRUSTUM::FRUSTUM() {
};

FRUSTUM::~FRUSTUM() {
};

void FRUSTUM::ExtractFrustum() {
	float   proj[16];
	float   modl[16];
	float   clip[16];
	float   t;

	/* Get the current PROJECTION matrix from OpenGL */
	glGetFloatv( GL_PROJECTION_MATRIX, proj );

	/* Get the current MODELVIEW matrix from OpenGL */
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );

	/* Combine the two matrices (multiply projection by modelview) */
	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

	/* Extract the numbers for the RIGHT plane */
	frustumdata[0][0] = clip[ 3] - clip[ 0];
	frustumdata[0][1] = clip[ 7] - clip[ 4];
	frustumdata[0][2] = clip[11] - clip[ 8];
	frustumdata[0][3] = clip[15] - clip[12];

	/* Normalize the result */
	t = sqrt( frustumdata[0][0] * frustumdata[0][0] + frustumdata[0][1] * frustumdata[0][1] + frustumdata[0][2] * frustumdata[0][2] );
	frustumdata[0][0] /= t;
	frustumdata[0][1] /= t;
	frustumdata[0][2] /= t;
	frustumdata[0][3] /= t;

	/* Extract the numbers for the LEFT plane */
	frustumdata[1][0] = clip[ 3] + clip[ 0];
	frustumdata[1][1] = clip[ 7] + clip[ 4];
	frustumdata[1][2] = clip[11] + clip[ 8];
	frustumdata[1][3] = clip[15] + clip[12];

	/* Normalize the result */
	t = sqrt( frustumdata[1][0] * frustumdata[1][0] + frustumdata[1][1] * frustumdata[1][1] + frustumdata[1][2] * frustumdata[1][2] );
	frustumdata[1][0] /= t;
	frustumdata[1][1] /= t;
	frustumdata[1][2] /= t;
	frustumdata[1][3] /= t;

	/* Extract the BOTTOM plane */
	frustumdata[2][0] = clip[ 3] + clip[ 1];
	frustumdata[2][1] = clip[ 7] + clip[ 5];
	frustumdata[2][2] = clip[11] + clip[ 9];
	frustumdata[2][3] = clip[15] + clip[13];

	/* Normalize the result */
	t = sqrt( frustumdata[2][0] * frustumdata[2][0] + frustumdata[2][1] * frustumdata[2][1] + frustumdata[2][2] * frustumdata[2][2] );
	frustumdata[2][0] /= t;
	frustumdata[2][1] /= t;
	frustumdata[2][2] /= t;
	frustumdata[2][3] /= t;

	/* Extract the TOP plane */
	frustumdata[3][0] = clip[ 3] - clip[ 1];
	frustumdata[3][1] = clip[ 7] - clip[ 5];
	frustumdata[3][2] = clip[11] - clip[ 9];
	frustumdata[3][3] = clip[15] - clip[13];

	/* Normalize the result */
	t = sqrt( frustumdata[3][0] * frustumdata[3][0] + frustumdata[3][1] * frustumdata[3][1] + frustumdata[3][2] * frustumdata[3][2] );
	frustumdata[3][0] /= t;
	frustumdata[3][1] /= t;
	frustumdata[3][2] /= t;
	frustumdata[3][3] /= t;

	/* Extract the FAR plane */
	frustumdata[4][0] = clip[ 3] - clip[ 2];
	frustumdata[4][1] = clip[ 7] - clip[ 6];
	frustumdata[4][2] = clip[11] - clip[10];
	frustumdata[4][3] = clip[15] - clip[14];

	/* Normalize the result */
	t = sqrt( frustumdata[4][0] * frustumdata[4][0] + frustumdata[4][1] * frustumdata[4][1] + frustumdata[4][2] * frustumdata[4][2] );
	frustumdata[4][0] /= t;
	frustumdata[4][1] /= t;
	frustumdata[4][2] /= t;
	frustumdata[4][3] /= t;

	/* Extract the NEAR plane */
	frustumdata[5][0] = clip[ 3] + clip[ 2];
	frustumdata[5][1] = clip[ 7] + clip[ 6];
	frustumdata[5][2] = clip[11] + clip[10];
	frustumdata[5][3] = clip[15] + clip[14];

	/* Normalize the result */
	t = sqrt( frustumdata[5][0] * frustumdata[5][0] + frustumdata[5][1] * frustumdata[5][1] + frustumdata[5][2] * frustumdata[5][2] );
	frustumdata[5][0] /= t;
	frustumdata[5][1] /= t;
	frustumdata[5][2] /= t;
	frustumdata[5][3] /= t;
};

bool FRUSTUM::PointInFrustum(float x, float y, float z) {
	int p;
	for( p = 0; p < 6; p++ )
		if( frustumdata[p][0] * x + frustumdata[p][1] * y + frustumdata[p][2] * z + frustumdata[p][3] <= 0 )
			return false;
	return true;
};

bool FRUSTUM::SphereInFrustum(float x, float y, float z, float radius) {
	int p;
	for( p = 0; p < 6; p++ )
		if( frustumdata[p][0] * x + frustumdata[p][1] * y + frustumdata[p][2] * z + frustumdata[p][3] <= -radius )
			return false;
	return true;
};

bool FRUSTUM::CubeInFrustum(float x, float y, float z, float size) {
	int p;
	for( p = 0; p < 6; p++ ) {
		if( frustumdata[p][0] * (x - size) + frustumdata[p][1] * (y - size) + frustumdata[p][2] * (z - size) + frustumdata[p][3] > 0 )
			continue;
		if( frustumdata[p][0] * (x + size) + frustumdata[p][1] * (y - size) + frustumdata[p][2] * (z - size) + frustumdata[p][3] > 0 )
			continue;
		if( frustumdata[p][0] * (x - size) + frustumdata[p][1] * (y + size) + frustumdata[p][2] * (z - size) + frustumdata[p][3] > 0 )
			continue;
		if( frustumdata[p][0] * (x + size) + frustumdata[p][1] * (y + size) + frustumdata[p][2] * (z - size) + frustumdata[p][3] > 0 )
			continue;
		if( frustumdata[p][0] * (x - size) + frustumdata[p][1] * (y - size) + frustumdata[p][2] * (z + size) + frustumdata[p][3] > 0 )
			continue;
		if( frustumdata[p][0] * (x + size) + frustumdata[p][1] * (y - size) + frustumdata[p][2] * (z + size) + frustumdata[p][3] > 0 )
			continue;
		if( frustumdata[p][0] * (x - size) + frustumdata[p][1] * (y + size) + frustumdata[p][2] * (z + size) + frustumdata[p][3] > 0 )
			continue;
		if( frustumdata[p][0] * (x + size) + frustumdata[p][1] * (y + size) + frustumdata[p][2] * (z + size) + frustumdata[p][3] > 0 )
			continue;
		return false;
	}
	return true;
};

/****************/


LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc (hwnd, message, wParam, lParam);
	}
	return 0;
};
