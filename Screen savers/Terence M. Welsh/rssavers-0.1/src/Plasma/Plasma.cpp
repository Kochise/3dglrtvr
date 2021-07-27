/*
 * Copyright (C) 1999-2005  Terence M. Welsh
 *
 * This file is part of Plasma.
 *
 * Plasma is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 *
 * Plasma is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


// Plasma screen saver


#include <windows.h>
#include <stdio.h>
#include <rsWin32Saver/rsWin32Saver.h>
#include <rsText/rsText.h>
#include <process.h>
#include <math.h>
#include <time.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <regstr.h>
#include <commctrl.h>
#include <resource.h>


LPCTSTR registryPath = ("Software\\Really Slick\\Plasma");
#define PIx2 6.28318530718f
#define NUMCONSTS 18
#define MAXTEXSIZE 1024

// Globals
HGLRC hglrc;
HDC hdc;
int readyToDraw = 0;
float frameTime = 0.0f;
float aspectRatio;
float wide;
float high;
float c[NUMCONSTS];  // constant
float ct[NUMCONSTS];  // temporary value of constant
float cv[NUMCONSTS];  // velocity of constant
float ***position;
float ***plasma;
unsigned int tex;
int texsize = 256;
int plasmasize = 64;
float *plasmamap;
// text output
rsText* textwriter;
// Parameters edited in the dialog box
int dZoom;
int dFocus;
int dSpeed;
int dResolution;


// Useful random number macros
// Don't forget to initialize with srand()
inline int rsRandi(int x){
	return rand() % x;
}
inline float rsRandf(float x){
	return x * (float(rand()) / float(RAND_MAX));
}


// Find absolute value and truncate to 1.0
float fabstrunc(float f){
	if(f >= 0.0f)
		return(f <= 1.0f ? f : 1.0f);
	else
		return(f >= -1.0f ? -f : 1.0f);
}



void draw(){
	int i, j;
	float rgb[3];
	float temp;
	static float focus = float(dFocus) / 50.0f + 0.3f;
	static float maxdiff = 0.004f * float(dSpeed);
	static int index;

	//Update constants
	for(i=0; i<NUMCONSTS; i++){
		ct[i] += cv[i];
		if(ct[i] > PIx2)
			ct[i] -= PIx2;
		c[i] = sinf(ct[i]) * focus;
	}

	// Update colors
	for(i=0; i<plasmasize; i++){
		for(j=0; j<int(float(plasmasize) / aspectRatio); j++){
			// Calculate vertex colors
			rgb[0] = plasma[i][j][0];
			rgb[1] = plasma[i][j][1];
			rgb[2] = plasma[i][j][2];
			plasma[i][j][0] = 0.7f
							* (c[0] * position[i][j][0] + c[1] * position[i][j][1]
							+ c[2] * (position[i][j][0] * position[i][j][0] + 1.0f)
							+ c[3] * position[i][j][0] * position[i][j][1]
							+ c[4] * rgb[1] + c[5] * rgb[2]);
			plasma[i][j][1] = 0.7f
							* (c[6] * position[i][j][0] + c[7] * position[i][j][1]
							+ c[8] * position[i][j][0] * position[i][j][0]
							+ c[9] * (position[i][j][1] * position[i][j][1] - 1.0f)
							+ c[10] * rgb[0] + c[11] * rgb[2]);
			plasma[i][j][2] = 0.7f
							* (c[12] * position[i][j][0] + c[13] * position[i][j][1]
							+ c[14] * (1.0f - position[i][j][0] * position[i][j][1])
							+ c[15] * position[i][j][1] * position[i][j][1]
							+ c[16] * rgb[0] + c[17] * rgb[1]);

			// Don't let the colors change too much
			temp = plasma[i][j][0] - rgb[0];
			if(temp > maxdiff)
				plasma[i][j][0] = rgb[0] + maxdiff;
			if(temp < -maxdiff)
				plasma[i][j][0] = rgb[0] - maxdiff;
			temp = plasma[i][j][1] - rgb[1];
			if(temp > maxdiff)
				plasma[i][j][1] = rgb[1] + maxdiff;
			if(temp < -maxdiff)
				plasma[i][j][1] = rgb[1] - maxdiff;
			temp = plasma[i][j][2] - rgb[2];
			if(temp > maxdiff)
				plasma[i][j][2] = rgb[2] + maxdiff;
			if(temp < -maxdiff)
				plasma[i][j][2] = rgb[2] - maxdiff;

			// Put colors into texture
			index = (i * texsize + j) * 3;
			plasmamap[index] = fabstrunc(plasma[i][j][0]);
			plasmamap[index+1] = fabstrunc(plasma[i][j][1]);
			plasmamap[index+2] = fabstrunc(plasma[i][j][2]);
		}
	}

	// Update texture
	glPixelStorei(GL_UNPACK_ROW_LENGTH, texsize);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, int(float(plasmasize) / aspectRatio), plasmasize,
		GL_RGB, GL_FLOAT, plasmamap);

	// Draw it
	// The "- 1" cuts off right and top edges to get rid of blending to black
	static float texright = float(plasmasize - 1) / float(texsize);
	static float textop = texright / aspectRatio;
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		glTexCoord2f(0.0f, texright);
		glVertex2f(1.0f, 0.0f);
		glTexCoord2f(textop, 0.0f);
		glVertex2f(0.0f, 1.0f);
		glTexCoord2f(textop, texright);
		glVertex2f(1.0f, 1.0f);
	glEnd();

	// print text
	static float totalTime = 0.0f;
	totalTime += frameTime;
	static std::string str;
	static int frames = 0;
	++frames;
	if(frames == 20){
		str = "FPS = " + to_string(20.0f / totalTime);
		totalTime = 0.0f;
		frames = 0;
	}
	if(kStatistics){
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0f, 50.0f * aspectRatio, 0.0f, 50.0f, -1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(1.0f, 48.0f, 0.0f);

		glColor3f(1.0f, 0.6f, 0.0f);
		textwriter->draw(str);

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}

    wglSwapLayerBuffers(hdc, WGL_SWAP_MAIN_PLANE);
}


void IdleProc(){
	// update time
	static rsTimer timer;
	frameTime = timer.tick();

	if(readyToDraw && !isSuspended && !checkingPassword)
		draw();
}


void doSaver(HWND hwnd){
	int i, j;
	RECT rect;

	srand((unsigned)time(NULL));

	// Window initialization
	hdc = GetDC(hwnd);
	SetBestPixelFormat(hdc);
	hglrc = wglCreateContext(hdc);
	GetClientRect(hwnd, &rect);
	wglMakeCurrent(hdc, hglrc);
	glViewport(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
	aspectRatio = float(rect.right) / float(rect.bottom);
	if(aspectRatio >= 1.0f){
		wide = 30.0f / float(dZoom);
		high = wide / aspectRatio;
	}
	else{
		high = 30.0f / float(dZoom);
		wide = high * aspectRatio;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 1.0f, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set resolution of plasma
	if(aspectRatio >= 1.0f)
		plasmasize = (dResolution * MAXTEXSIZE) / 100;
	else
		plasmasize = int(float(dResolution * MAXTEXSIZE) * aspectRatio * 0.01f);
	// Set resolution of texture
	texsize = 16;
	if(aspectRatio >= 1.0f)
		while(plasmasize > texsize)
			texsize *= 2;
	else
		while(int(float(plasmasize) / aspectRatio) > texsize)
			texsize *= 2;

	// Initialize memory and positions
	plasmamap = new float[texsize*texsize*3];
	for(i=0; i<texsize*texsize*3; i++)
		plasmamap[i] = 0.0f;
	plasma = new float**[plasmasize];
	position = new float**[plasmasize];
	for(i=0; i<plasmasize; i++){
		plasma[i] = new float*[int(float(plasmasize) / aspectRatio)];
		position[i] = new float*[int(float(plasmasize) / aspectRatio)];
		for(j=0; j<int(float(plasmasize) / aspectRatio); j++){
			plasma[i][j] = new float[3];
			position[i][j] = new float[2];
			plasma[i][j][0] = 0.0f;
			plasma[i][j][1] = 0.0f;
			plasma[i][j][2] = 0.0f;
			position[i][j][0] = float(i * wide) / float(plasmasize - 1) - (wide * 0.5f);
			position[i][j][1] = float(j * high) / (float(plasmasize) / aspectRatio - 1.0f) - (high * 0.5f);
		}
	}
	// Initialize constants
	for(i=0; i<NUMCONSTS; i++){
		ct[i] = rsRandf(PIx2);
		cv[i] = rsRandf(0.005f * float(dSpeed)) + 0.0001f;
	}

	// Make texture
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texsize, texsize, 0,
		GL_RGB, GL_FLOAT, plasmamap);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glPixelStorei(GL_UNPACK_ROW_LENGTH, texsize);

	// Initialize text
	textwriter = new rsText;
}


void cleanUp(HWND hwnd){
	// Kill device context
	ReleaseDC(hwnd, hdc);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
}


void setDefaults(){
	dZoom = 10;
	dFocus = 30;
	dSpeed = 20;
	dResolution = 25;
	dFrameRateLimit = 30;
}


// Initialize all user-defined stuff
void readRegistry(){
	LONG result;
	HKEY skey;
	DWORD valtype, valsize, val;

	setDefaults();

	result = RegOpenKeyEx(HKEY_CURRENT_USER, registryPath, 0, KEY_READ, &skey);
	if(result != ERROR_SUCCESS)
		return;

	valsize=sizeof(val);

	result = RegQueryValueEx(skey, "Zoom", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dZoom = val;
	result = RegQueryValueEx(skey, "Focus", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dFocus = val;
	result = RegQueryValueEx(skey, "Speed", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dSpeed = val;
	result = RegQueryValueEx(skey, "Resolution", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dResolution = val;
	result = RegQueryValueEx(skey, "FrameRateLimit", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dFrameRateLimit = val;

	RegCloseKey(skey);
}


// Save all user-defined stuff
void writeRegistry(){
    LONG result;
	HKEY skey;
	DWORD val, disp;

	result = RegCreateKeyEx(HKEY_CURRENT_USER, registryPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &skey, &disp);
	if(result != ERROR_SUCCESS)
		return;

	val = dZoom;
	RegSetValueEx(skey, "Zoom", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dFocus;
	RegSetValueEx(skey, "Focus", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dSpeed;
	RegSetValueEx(skey, "Speed", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dResolution;
	RegSetValueEx(skey, "Resolution", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dFrameRateLimit;
	RegSetValueEx(skey, "FrameRateLimit", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));

	RegCloseKey(skey);
}


BOOL aboutProc(HWND hdlg, UINT msg, WPARAM wpm, LPARAM lpm){
	switch(msg){
	case WM_CTLCOLORSTATIC:
		if(HWND(lpm) == GetDlgItem(hdlg, WEBPAGE)){
			SetTextColor(HDC(wpm), RGB(0,0,255));
			SetBkColor(HDC(wpm), COLORREF(GetSysColor(COLOR_3DFACE)));
			return(int(GetSysColorBrush(COLOR_3DFACE)));
		}
		break;
    case WM_COMMAND:
		switch(LOWORD(wpm)){
		case IDOK:
		case IDCANCEL:
			EndDialog(hdlg, LOWORD(wpm));
			break;
		case WEBPAGE:
			ShellExecute(NULL, "open", "http://www.reallyslick.com/", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	return FALSE;
}


void initControls(HWND hdlg){
	char cval[16];

	SendDlgItemMessage(hdlg, ZOOM, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(100))));
	SendDlgItemMessage(hdlg, ZOOM, TBM_SETPOS, 1, LPARAM(dZoom));
	SendDlgItemMessage(hdlg, ZOOM, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, ZOOM, TBM_SETPAGESIZE, 0, LPARAM(5));
	sprintf(cval, "%d", dZoom);
	SendDlgItemMessage(hdlg, ZOOMTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, FOCUS, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(100))));
	SendDlgItemMessage(hdlg, FOCUS, TBM_SETPOS, 1, LPARAM(dFocus));
	SendDlgItemMessage(hdlg, FOCUS, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, FOCUS, TBM_SETPAGESIZE, 0, LPARAM(5));
	sprintf(cval, "%d", dFocus);
	SendDlgItemMessage(hdlg, FOCUSTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, SPEED, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(100))));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETPOS, 1, LPARAM(dSpeed));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETPAGESIZE, 0, LPARAM(5));
	sprintf(cval, "%d", dSpeed);
	SendDlgItemMessage(hdlg, SPEEDTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, RESOLUTION, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(100))));
	SendDlgItemMessage(hdlg, RESOLUTION, TBM_SETPOS, 1, LPARAM(dResolution));
	SendDlgItemMessage(hdlg, RESOLUTION, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, RESOLUTION, TBM_SETPAGESIZE, 0, LPARAM(5));
	sprintf(cval, "%d", dResolution);
	SendDlgItemMessage(hdlg, RESOLUTIONTEXT, WM_SETTEXT, 0, LPARAM(cval));

	initFrameRateLimitSlider(hdlg, FRAMERATELIMIT, FRAMERATELIMITTEXT);
}


BOOL ScreenSaverConfigureDialog(HWND hdlg, UINT msg,
										 WPARAM wpm, LPARAM lpm){
	int ival;
	char cval[16];

    switch(msg){
    case WM_INITDIALOG:
        InitCommonControls();
        readRegistry();
		initControls(hdlg);
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wpm)){
        case IDOK:
			dZoom = SendDlgItemMessage(hdlg, ZOOM, TBM_GETPOS, 0, 0);
			dFocus = SendDlgItemMessage(hdlg, FOCUS, TBM_GETPOS, 0, 0);
			dSpeed = SendDlgItemMessage(hdlg, SPEED, TBM_GETPOS, 0, 0);
			dResolution = SendDlgItemMessage(hdlg, RESOLUTION, TBM_GETPOS, 0, 0);
			dFrameRateLimit = SendDlgItemMessage(hdlg, FRAMERATELIMIT, TBM_GETPOS, 0, 0);
			writeRegistry();
            // Fall through
        case IDCANCEL:
            EndDialog(hdlg, LOWORD(wpm));
            break;
		case DEFAULTS:
			setDefaults();
			initControls(hdlg);
			break;
		case ABOUT:
			DialogBox(mainInstance, MAKEINTRESOURCE(DLG_ABOUT), hdlg, DLGPROC(aboutProc));
		}
        return TRUE;
	case WM_HSCROLL:
		if(HWND(lpm) == GetDlgItem(hdlg, ZOOM)){
			ival = SendDlgItemMessage(hdlg, ZOOM, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, ZOOMTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, FOCUS)){
			ival = SendDlgItemMessage(hdlg, FOCUS, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, FOCUSTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, SPEED)){
			ival = SendDlgItemMessage(hdlg, SPEED, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, SPEEDTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, RESOLUTION)){
			ival = SendDlgItemMessage(hdlg, RESOLUTION, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, RESOLUTIONTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, FRAMERATELIMIT))
			updateFrameRateLimitSlider(hdlg, FRAMERATELIMIT, FRAMERATELIMITTEXT);
		return TRUE;
    }
    return FALSE;
}


LONG ScreenSaverProc(HWND hwnd, UINT msg, WPARAM wpm, LPARAM lpm){
	switch(msg){
	case WM_CREATE:
		readRegistry();
		doSaver(hwnd);
		readyToDraw = 1;
		break;
	case WM_DESTROY:
		readyToDraw = 0;
		cleanUp(hwnd);
		break;
	}
	return DefScreenSaverProc(hwnd, msg, wpm, lpm);
}