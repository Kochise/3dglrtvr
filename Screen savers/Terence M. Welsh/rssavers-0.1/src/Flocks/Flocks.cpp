/*
 * Copyright (C) 1999-2005  Terence M. Welsh
 *
 * This file is part of Flocks.
 *
 * Flocks is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 *
 * Flocks is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


// Flocks screensaver

#include <windows.h>
#include <stdio.h>
#include <rsWin32Saver/rsWin32Saver.h>
#include <rsText/rsText.h>
#include <math.h>
#include <time.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <regstr.h>
#include <commctrl.h>
#include <Rgbhsl/Rgbhsl.h>
#include <resource.h>


#define R2D 57.2957795131f


class bug;


// Globals
LPCTSTR registryPath = ("Software\\Really Slick\\Flocks");
HDC hdc;
HGLRC hglrc;
int readyToDraw = 0;
float frameTime = 0.0f;
float aspectRatio;
int wide;
int high;
int deep;
bug *lBugs;
bug *fBugs;
float colorFade;
// text output
rsText* textwriter;
// Parameters edited in the dialog box
int dLeaders;
int dFollowers;
int dGeometry;
int dSize;
int dComplexity;
int dSpeed;
int dStretch;
int dColorfadespeed;
BOOL dChromatek;
BOOL dConnections;


// useful random functions
int rsRandi(int x){return(rand() * x / 32767);}
float rsRandf(float x){return(float(rand()) * x / 32767.0f);}


class bug{
public:
	int type;  // 0 = leader   1 = follower
	float h, s, l;
	float r, g, b;
	float halfr, halfg, halfb;
	float x, y, z;
	float xSpeed, ySpeed, zSpeed, maxSpeed;
	float accel;
	int right, up, forward;
	int leader;
	float craziness;  // How prone to switching direction is this leader
	float nextChange;  // Time until this leader's next direction change

	bug();
	virtual ~bug();
	void initLeader();
	void initFollower();
	void update(bug *bugs);
};

bug::bug(){
}

bug::~bug(){
}

void bug::initLeader(){
	type = 0;
	h = rsRandf(1.0);
	s = 1.0f;
	l = 1.0f;
	x = rsRandf(float(wide * 2)) - float(wide);
	y = rsRandf(float(high * 2)) - float(high);
	z = rsRandf(float(wide * 2)) + float(wide * 2);
	right = up = forward = 1;
	xSpeed = ySpeed = zSpeed = 0.0f;
	maxSpeed = 8.0f * float(dSpeed);
	accel = 13.0f * float(dSpeed);
	craziness = rsRandf(4.0f) + 0.05f;
	nextChange = 1.0f;
}

void bug::initFollower(){
	type = 1;
	h = rsRandf(1.0);
	s = 1.0f;
	l = 1.0f;
	x = rsRandf(float(wide * 2)) - float(wide);
	y = rsRandf(float(high * 2)) - float(high);
	z = rsRandf(float(wide * 5)) + float(wide * 2);
	right = up = forward = 0;
	xSpeed = ySpeed = zSpeed = 0.0f;
	maxSpeed = (rsRandf(6.0f) + 4.0f) * float(dSpeed);
	accel = (rsRandf(4.0f) + 9.0f) * float(dSpeed);
	leader = 0;
}

void bug::update(bug *bugs){
	int i;
	float scale[4];

	if(!type){  // leader
		nextChange -= frameTime;
		if(nextChange <= 0.0f){
			if(rsRandi(2))
				right ++;
			if(rsRandi(2))
				up ++;
			if(rsRandi(2))
				forward ++;
			if(right >= 2)
				right = 0;
			if(up >= 2)
				up = 0;
			if(forward >= 2)
				forward = 0;
			nextChange = rsRandf(craziness);
		}
		if(right)
			xSpeed += accel * frameTime;
		else
			xSpeed -= accel * frameTime;
		if(up)
			ySpeed += accel * frameTime;
		else
			ySpeed -= accel * frameTime;
		if(forward)
			zSpeed -= accel * frameTime;
		else
			zSpeed += accel * frameTime;
		if(x < float(-wide))
			right = 1;
		if(x > float(wide))
			right = 0;
		if(y < float(-high))
			up = 1;
		if(y > float(high))
			up = 0;
		if(z < float(-deep))
			forward = 0;
		if(z > float(deep))
			forward = 1;
		// Even leaders change color from Chromatek 3D
		if(dChromatek){
			h = 0.666667f * ((float(wide) - z) / float(wide + wide));
			if(h > 0.666667f)
				h = 0.666667f;
			if(h < 0.0f)
				h = 0.0f;
		}
	}
	else{  // follower
		if(!rsRandi(10)){
			float oldDistance = 10000000.0f, newDistance;
			for(i=0; i<dLeaders; i++){
				newDistance = ((bugs[i].x - x) * (bugs[i].x - x)
							+ (bugs[i].y - y) * (bugs[i].y - y)
							+ (bugs[i].z - z) * (bugs[i].z - z));
				if(newDistance < oldDistance){
					oldDistance = newDistance;
					leader = i;
				}
			}
		}
		if((bugs[leader].x - x) > 0.0f)
			xSpeed += accel * frameTime;
		else
			xSpeed -= accel * frameTime;
		if((bugs[leader].y - y) > 0.0f)
			ySpeed += accel * frameTime;
		else
			ySpeed -= accel * frameTime;
		if((bugs[leader].z - z) > 0.0f)
			zSpeed += accel * frameTime;
		else
			zSpeed -= accel * frameTime;
		if(dChromatek){
			h = 0.666667f * ((float(wide) - z) / float(wide + wide));
			if(h > 0.666667f)
				h = 0.666667f;
			if(h < 0.0f)
				h = 0.0f;
		}
		else{
			if(fabs(h - bugs[leader].h) < (colorFade * frameTime))
				h = bugs[leader].h;
			else{
				if(fabs(h - bugs[leader].h) < 0.5f){
					if(h > bugs[leader].h)
						h -= colorFade * frameTime;
					else
						h += colorFade * frameTime;
				}
				else{
					if(h > bugs[leader].h)
						h += colorFade * frameTime;
					else
						h -= colorFade * frameTime;
					if(h > 1.0f)
						h -= 1.0f;
					if(h < 0.0f)
						h += 1.0f;
				}
			}
		}
	}

	if(xSpeed > maxSpeed)
		xSpeed = maxSpeed;
	if(xSpeed < -maxSpeed)
		xSpeed = -maxSpeed;
	if(ySpeed > maxSpeed)
		ySpeed = maxSpeed;
	if(ySpeed < -maxSpeed)
		ySpeed = -maxSpeed;
	if(zSpeed > maxSpeed)
		zSpeed = maxSpeed;
	if(zSpeed < -maxSpeed)
		zSpeed = -maxSpeed;
	
	x += xSpeed * frameTime;
	y += ySpeed * frameTime;
	z += zSpeed * frameTime;
	if(dStretch){
		scale[0] = xSpeed * 0.04f;
		scale[1] = ySpeed * 0.04f;
		scale[2] = zSpeed * 0.04f;
		scale[3] = scale[0] * scale[0] + scale[1] * scale[1] + scale[2] * scale[2];
		if(scale[3] > 0.0f){
			scale[3] = float(sqrt(scale[3]));
			scale[0] /= scale[3];
			scale[1] /= scale[3];
			scale[2] /= scale[3];
		}
	}
	hsl2rgb(h, s, l, r, g, b);
	halfr = r * 0.5f;
	halfg = g * 0.5f;
	halfb = b * 0.5f;
	glColor3f(r, g, b);
	if(dGeometry){  // Draw blobs
		glPushMatrix();
			glTranslatef(x, y, z);
			if(dStretch){
				scale[3] *= float(dStretch) * 0.05f;
				if(scale[3] < 1.0f)
					scale[3] = 1.0f;
				glRotatef(float(atan2(-scale[0], -scale[2])) * R2D, 0.0f, 1.0f, 0.0f);
				glRotatef(float(asin(scale[1])) * R2D, 1.0f, 0.0f, 0.0f);
				glScalef(1.0f, 1.0f, scale[3]);
			}
			glCallList(1);
		glPopMatrix();
	}
	else{  // Draw dots
		if(dStretch){
			glLineWidth(float(dSize) * float(700 - z) * 0.0002f);
			scale[0] *= float(dStretch);
			scale[1] *= float(dStretch);
			scale[2] *= float(dStretch);
			glBegin(GL_LINES);
				glVertex3f(x - scale[0], y - scale[1], z - scale[2]);
				glVertex3f(x + scale[0], y + scale[1], z + scale[2]);
			glEnd();
		}
		else{
			glPointSize(float(dSize) * float(700 - z) * 0.001f);
			glBegin(GL_POINTS);
				glVertex3f(x, y, z);
			glEnd();
		}
	}

	if(dConnections && type){  // draw connections
		glLineWidth(1.0f);
		glBegin(GL_LINES);
			glColor3f(halfr, halfg, halfb);
			glVertex3f(x, y, z);
			glColor3f(bugs[leader].halfr, bugs[leader].halfg, bugs[leader].halfb);
			glVertex3f(bugs[leader].x, bugs[leader].y, bugs[leader].z);
		glEnd();
	}
}


void draw(){
	int i;

	static int first = 1;
	if(first){
		textwriter = new rsText;
		first = 0;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, aspectRatio, 0.1, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -float(wide * 2));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update and draw leaders
	for(i=0; i<dLeaders; i++)
		lBugs[i].update(lBugs);
	// Update and draw followers
	for(i=0; i<dFollowers; i++)
		fBugs[i].update(lBugs);

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


void initSaver(HWND hwnd){
	int i;
	RECT rect;

	srand((unsigned)time(NULL));

	// Window initialization
	hdc = GetDC(hwnd);
	SetBestPixelFormat(hdc);
	hglrc = wglCreateContext(hdc);
	GetClientRect(hwnd, &rect);
	wglMakeCurrent(hdc, hglrc);
	aspectRatio = float(rect.right) / float(rect.bottom);
	glViewport(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);

	// calculate boundaries
	if((rect.right - rect.left) > (rect.bottom - rect.top)){
		high = deep = 160;
		wide = high * (rect.right - rect.left) / (rect.bottom - rect.top);
	}
	else{
		wide = deep = 160;
		high = wide * (rect.bottom - rect.top) / (rect.right - rect.left);
	}

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	if(dGeometry){  // Setup lights and build blobs
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		float ambient[4] = {0.25f, 0.25f, 0.25f, 0.0f};
		float diffuse[4] = {1.0f, 1.0f, 1.0f, 0.0f};
		float specular[4] = {1.0f, 1.0f, 1.0f, 0.0f};
		float position[4] = {500.0f, 500.0f, 500.0f, 0.0f};
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glEnable(GL_COLOR_MATERIAL);
		glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		glColor3f(0.7f, 0.7f, 0.7f);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

		glNewList(1, GL_COMPILE);
			GLUquadricObj *qobj = gluNewQuadric();
			gluSphere(qobj, float(dSize) * 0.5f, dComplexity + 2, dComplexity + 1);
			gluDeleteQuadric(qobj);
		glEndList();
	}
	else{
		if(dStretch == 0){
			// make GL_POINTS round instead of square
			glEnable(GL_POINT_SMOOTH);
			glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		}
	}

	lBugs = new bug[dLeaders];
	fBugs = new bug[dFollowers];
	for(i=0; i<dLeaders; i++)
		lBugs[i].initLeader();
	for(i=0; i<dFollowers; i++)
		fBugs[i].initFollower();

	colorFade = float(dColorfadespeed) * 0.01f;
}

void cleanUp(HWND hwnd){
	// Free memory
	delete[] lBugs;
	delete[] fBugs;

	// Kill device context
	ReleaseDC(hwnd, hdc);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
}


void setDefaults(){
	dLeaders = 4;
	dFollowers = 1000;
	dGeometry = 1;
	dSize = 5;
	dComplexity = 1;
	dSpeed = 15;
	dStretch = 20;
	dColorfadespeed = 15;
	dChromatek = FALSE;
	dConnections = FALSE;
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

	result = RegQueryValueEx(skey, "Leaders", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dLeaders = val;
	result = RegQueryValueEx(skey, "Followers", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dFollowers = val;
	result = RegQueryValueEx(skey, "Geometry", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dGeometry = val;
	result = RegQueryValueEx(skey, "Size", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dSize = val;
	result = RegQueryValueEx(skey, "Complexity", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dComplexity = val;
	result = RegQueryValueEx(skey, "Speed", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dSpeed = val;
	result = RegQueryValueEx(skey, "Stretch", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dStretch = val;
	result = RegQueryValueEx(skey, "Colorfadespeed", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dColorfadespeed = val;
	result = RegQueryValueEx(skey, "Chromatek", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dChromatek = val;
	result = RegQueryValueEx(skey, "Connections", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dConnections = val;
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

	val = dLeaders;
	RegSetValueEx(skey, "Leaders", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dFollowers;
	RegSetValueEx(skey, "Followers", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dGeometry;
	RegSetValueEx(skey, "Geometry", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dSize;
	RegSetValueEx(skey, "Size", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dComplexity;
	RegSetValueEx(skey, "Complexity", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dSpeed;
	RegSetValueEx(skey, "Speed", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dStretch;
	RegSetValueEx(skey, "Stretch", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dColorfadespeed;
	RegSetValueEx(skey, "Colorfadespeed", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dChromatek;
	RegSetValueEx(skey, "Chromatek", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dConnections;
	RegSetValueEx(skey, "Connections", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dFrameRateLimit;
	RegSetValueEx(skey, "FrameRateLimit", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));

	RegCloseKey(skey);
}


BOOL CALLBACK aboutProc(HWND hdlg, UINT msg, WPARAM wpm, LPARAM lpm){
	switch(msg){
	case WM_CTLCOLORSTATIC:
		if((HWND(lpm) == GetDlgItem(hdlg, WEBPAGE)) || (HWND(lpm) == GetDlgItem(hdlg, CHROMATEKWEBPAGE))){
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
			ShellExecute(NULL, "open", "http://www.reallyslick.com", NULL, NULL, SW_SHOWNORMAL);
			break;
		case CHROMATEKWEBPAGE:
			ShellExecute(NULL, "open", "http://www.chromatek.com", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	return FALSE;
}


void initControls(HWND hdlg){
	char cval[16];

	SendDlgItemMessage(hdlg, LEADERS, UDM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(100), DWORD(1))));
	SendDlgItemMessage(hdlg, LEADERS, UDM_SETPOS, 0, LPARAM(dLeaders));

	SendDlgItemMessage(hdlg, FOLLOWERS, UDM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(10000), DWORD(0))));
	SendDlgItemMessage(hdlg, FOLLOWERS, UDM_SETPOS, 0, LPARAM(dFollowers));

	SendDlgItemMessage(hdlg, GEOMETRY, CB_DELETESTRING, WPARAM(1), 0);
	SendDlgItemMessage(hdlg, GEOMETRY, CB_DELETESTRING, WPARAM(0), 0);
	SendDlgItemMessage(hdlg, GEOMETRY, CB_ADDSTRING, 0, LPARAM("Dots"));
	SendDlgItemMessage(hdlg, GEOMETRY, CB_ADDSTRING, 0, LPARAM("Blobs"));
	SendDlgItemMessage(hdlg, GEOMETRY, CB_SETCURSEL, WPARAM(dGeometry), 0);

	SendDlgItemMessage(hdlg, SIZE, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(100))));
	SendDlgItemMessage(hdlg, SIZE, TBM_SETPOS, 1, LPARAM(dSize));
	SendDlgItemMessage(hdlg, SIZE, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, SIZE, TBM_SETPAGESIZE, 0, LPARAM(10));
	sprintf(cval, "%d", dSize);
	SendDlgItemMessage(hdlg, SIZETEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, COMPLEXITY, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(10))));
	SendDlgItemMessage(hdlg, COMPLEXITY, TBM_SETPOS, 1, LPARAM(dComplexity));
	SendDlgItemMessage(hdlg, COMPLEXITY, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, COMPLEXITY, TBM_SETPAGESIZE, 0, LPARAM(2));
	sprintf(cval, "%d", dComplexity);
	SendDlgItemMessage(hdlg, COMPLEXITYTEXT, WM_SETTEXT, 0, LPARAM(cval));
	if(dGeometry)
		EnableWindow(GetDlgItem(hdlg, COMPLEXITY), TRUE);
	else
		EnableWindow(GetDlgItem(hdlg, COMPLEXITY), FALSE);
	sprintf(cval, "%d", dComplexity);

	SendDlgItemMessage(hdlg, SPEED, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(100))));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETPOS, 1, LPARAM(dSpeed));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETPAGESIZE, 0, LPARAM(10));
	sprintf(cval, "%d", dSpeed);
	SendDlgItemMessage(hdlg, SPEEDTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, STRETCH, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(0), DWORD(100))));
	SendDlgItemMessage(hdlg, STRETCH, TBM_SETPOS, 1, LPARAM(dStretch));
	SendDlgItemMessage(hdlg, STRETCH, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, STRETCH, TBM_SETPAGESIZE, 0, LPARAM(10));
	sprintf(cval, "%d", dStretch);
	SendDlgItemMessage(hdlg, STRETCHTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, COLORFADE, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(0), DWORD(100))));
	SendDlgItemMessage(hdlg, COLORFADE, TBM_SETPOS, 1, LPARAM(dColorfadespeed));
	SendDlgItemMessage(hdlg, COLORFADE, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, COLORFADE, TBM_SETPAGESIZE, 0, LPARAM(10));
	sprintf(cval, "%d", dColorfadespeed);
	SendDlgItemMessage(hdlg, COLORFADETEXT, WM_SETTEXT, 0, LPARAM(cval));

	CheckDlgButton(hdlg, CHROMATEK, dChromatek);

	CheckDlgButton(hdlg, CONNECTIONS, dConnections);

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
            dLeaders = SendDlgItemMessage(hdlg, LEADERS, UDM_GETPOS, 0, 0);
			dFollowers = SendDlgItemMessage(hdlg, FOLLOWERS, UDM_GETPOS, 0, 0);
			dGeometry = SendDlgItemMessage(hdlg, GEOMETRY, CB_GETCURSEL, 0, 0);
			dSize = SendDlgItemMessage(hdlg, SIZE, TBM_GETPOS, 0, 0);
			dComplexity = SendDlgItemMessage(hdlg, COMPLEXITY, TBM_GETPOS, 0, 0);
			dSpeed = SendDlgItemMessage(hdlg, SPEED, TBM_GETPOS, 0, 0);
			dStretch = SendDlgItemMessage(hdlg, STRETCH, TBM_GETPOS, 0, 0);
			dColorfadespeed = SendDlgItemMessage(hdlg, COLORFADE, TBM_GETPOS, 0, 0);
			dChromatek = (IsDlgButtonChecked(hdlg, CHROMATEK) == BST_CHECKED);
			dConnections = (IsDlgButtonChecked(hdlg, CONNECTIONS) == BST_CHECKED);
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
			break;
		case GEOMETRY:
			if(SendDlgItemMessage(hdlg, GEOMETRY, CB_GETCURSEL, 0, 0) == 1)
				EnableWindow(GetDlgItem(hdlg, COMPLEXITY), TRUE);
			else
				EnableWindow(GetDlgItem(hdlg, COMPLEXITY), FALSE);
		}
        return TRUE;
	case WM_HSCROLL:
		if(HWND(lpm) == GetDlgItem(hdlg, SIZE)){
			ival = SendDlgItemMessage(hdlg, SIZE, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, SIZETEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, COMPLEXITY)){
			ival = SendDlgItemMessage(hdlg, COMPLEXITY, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, COMPLEXITYTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, SPEED)){
			ival = SendDlgItemMessage(hdlg, SPEED, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, SPEEDTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, STRETCH)){
			ival = SendDlgItemMessage(hdlg, STRETCH, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, STRETCHTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, COLORFADE)){
			ival = SendDlgItemMessage(hdlg, COLORFADE, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, COLORFADETEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, FRAMERATELIMIT))
			updateFrameRateLimitSlider(hdlg, FRAMERATELIMIT, FRAMERATELIMITTEXT);
		return TRUE;
    }
    return FALSE;
}


LONG ScreenSaverProc(HWND hwnd, UINT msg, WPARAM wpm, LPARAM lpm){
	static unsigned long threadID;

	switch(msg){
	case WM_CREATE:
		readRegistry();
		initSaver(hwnd);
		readyToDraw = 1;
		break;
	case WM_DESTROY:
		readyToDraw = 0;
		cleanUp(hwnd);
		break;
	}
	return DefScreenSaverProc(hwnd, msg, wpm, lpm);
}