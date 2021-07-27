/*
 * Copyright (C) 2000-2005  Terence M. Welsh
 *
 * This file is part of Euphoria.
 *
 * Euphoria is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 *
 * Euphoria is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


// Euphoria screensaver


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
#include <rsMath/rsMath.h>
#include <Rgbhsl/Rgbhsl.h>
#include <resource.h>
#include <Euphoria/texture.h>

#define NUMCONSTS 9
#define PIx2 6.28318530718f


class wisp;


// Global variables
LPCTSTR registryPath = ("Software\\Really Slick\\Euphoria");
HGLRC hglrc;
HDC hdc;
int readyToDraw = 0;
wisp *backwisps;
wisp *wisps;
unsigned int tex;
//unsigned char* feedbackmap;
float aspectRatio;
int viewport[4];
float frameTime = 0.0f;
// text output
rsText* textwriter;
// Parameters edited in the dialog box
int dWisps;
int dBackground;
int dDensity;
int dVisibility;
int dSpeed;
int dFeedback;
int dFeedbackspeed;
int dFeedbacksize;
int dWireframe;
int dTexture;


// feedback texture object
unsigned int feedbacktex;
int feedbacktexsize;
// feedback variables
static float fr[4] = {0.0f, 0.0f, 0.0f, 0.0f};
static float fv[4];
static float f[4];
// feedback limiters
static float lr[3] = {0.0f, 0.0f, 0.0f};
static float lv[3];
static float l[3];



class wisp{
public:
	float ***vertices;
	float c[NUMCONSTS];     // constants
	float cr[NUMCONSTS];    // constants' radial position
	float cv[NUMCONSTS];    // constants' change velocities
	float hsl[3];
	float rgb[3];
	float hueSpeed;
	float saturationSpeed;

	wisp();
	~wisp();
	void update();
	void draw();
	void drawAsBackground();
};

wisp::wisp(){
	int i, j;
	float recHalfDens = 1.0f / (float(dDensity) * 0.5f);

	vertices = new float**[dDensity+1];
	for(i=0; i<=dDensity; i++){
		vertices[i] = new float*[dDensity+1];
		for(j=0; j<=dDensity; j++){
			vertices[i][j] = new float[7];
			vertices[i][j][3] = float(i) * recHalfDens - 1.0f;  // x position on grid
			vertices[i][j][4] = float(j) * recHalfDens - 1.0f;  // y position on grid
			// distance squared from the center
			vertices[i][j][5] = vertices[i][j][3] * vertices[i][j][3]
				+ vertices[i][j][4] * vertices[i][j][4];
			vertices[i][j][6] = 0.0f;  // intensity
		}
	}

	// initialize constants
	for(i=0; i<NUMCONSTS; i++){
		c[i] = rsRandf(2.0f) - 1.0f;
		cr[i] = rsRandf(PIx2);
		cv[i] = rsRandf(float(dSpeed) * 0.03f) + (float(dSpeed) * 0.001f);
	}

	// pick color
	hsl[0] = rsRandf(1.0f);
	hsl[1] = 0.1f + rsRandf(0.9f);
	hsl[2] = 1.0f;
	hueSpeed = rsRandf(0.1f) - 0.05f;
	saturationSpeed = rsRandf(0.04f) + 0.001f;
}

wisp::~wisp(){
	int i, j;

	for(i=0; i<=dDensity; i++){
		for(j=0; j<=dDensity; j++){
			delete[] vertices[i][j];
		}
		delete[] vertices[i];
	}
	delete[] vertices;
}


void wisp::update(){
	int i, j;
	rsVec up, right, crossvec;
	// visibility constants
	static float viscon1 = float(dVisibility) * 0.01f;
	static float viscon2 = 1.0f / viscon1;

	// update constants
	for(i=0; i<NUMCONSTS; i++){
		cr[i] += cv[i] * frameTime;
		if(cr[i] > PIx2)
			cr[i] -= PIx2;
		c[i] = cosf(cr[i]);
	}

	// update vertex positions
	for(i=0; i<=dDensity; i++){
		for(j=0; j<=dDensity; j++){
			vertices[i][j][0] = vertices[i][j][3] * vertices[i][j][3] * vertices[i][j][4] * c[0]
				+ vertices[i][j][5] * c[1] + 0.5f * c[2];
			vertices[i][j][1] = vertices[i][j][4] * vertices[i][j][4] * vertices[i][j][5] * c[3]
				+ vertices[i][j][3] * c[4] + 0.5f * c[5];
			vertices[i][j][2] = vertices[i][j][5] * vertices[i][j][5] * vertices[i][j][3] * c[6]
				+ vertices[i][j][4] * c[7] + c[8];
		}
	}

	// update vertex normals for most of mesh
	for(i=1; i<dDensity; i++){
		for(j=1; j<dDensity; j++){
			up.set(vertices[i][j+1][0] - vertices[i][j-1][0],
				vertices[i][j+1][1] - vertices[i][j-1][1],
				vertices[i][j+1][2] - vertices[i][j-1][2]);
			right.set(vertices[i+1][j][0] - vertices[i-1][j][0],
				vertices[i+1][j][1] - vertices[i-1][j][1],
				vertices[i+1][j][2] - vertices[i-1][j][2]);
			up.normalize();
			right.normalize();
			crossvec.cross(right, up);
			// Use depth component of normal to compute intensity
			// This way only edges of wisp are bright
			if(crossvec[2] < 0.0f)
				crossvec[2] *= -1.0f;
			vertices[i][j][6] = viscon2 * (viscon1 - crossvec[2]);
			if(vertices[i][j][6] > 1.0f)
				vertices[i][j][6] = 1.0f;
			if(vertices[i][j][6] < 0.0f)
				vertices[i][j][6] = 0.0f;
		}
	}

	// update color
	hsl[0] += hueSpeed * frameTime;
	if(hsl[0] < 0.0f)
		hsl[0] += 1.0f;
	if(hsl[0] > 1.0f)
		hsl[0] -= 1.0f;
	hsl[1] += saturationSpeed * frameTime;
	if(hsl[1] <= 0.1f){
		hsl[1] = 0.1f;
		saturationSpeed = -saturationSpeed;
	}
	if(hsl[1] >= 1.0f){
		hsl[1] = 1.0f;
		saturationSpeed = -saturationSpeed;
	}
	hsl2rgb(hsl[0], hsl[1], hsl[2], rgb[0], rgb[1], rgb[2]);
}

void wisp::draw(){
	int i, j;

	glPushMatrix();

	if(dWireframe){
		for(i=1; i<dDensity; i++){
			glBegin(GL_LINE_STRIP);
			for(j=0; j<=dDensity; j++){
				glColor3f(rgb[0] + vertices[i][j][6] - 1.0f, rgb[1] + vertices[i][j][6] - 1.0f, rgb[2] + vertices[i][j][6] - 1.0f);
				glTexCoord2d(vertices[i][j][3] - vertices[i][j][0], vertices[i][j][4] - vertices[i][j][1]);
				glVertex3fv(vertices[i][j]);
			}
			glEnd();
		}
		for(j=1; j<dDensity; j++){
			glBegin(GL_LINE_STRIP);
			for(i=0; i<=dDensity; i++){
				glColor3f(rgb[0] + vertices[i][j][6] - 1.0f, rgb[1] + vertices[i][j][6] - 1.0f, rgb[2] + vertices[i][j][6] - 1.0f);
				glTexCoord2d(vertices[i][j][3] - vertices[i][j][0], vertices[i][j][4] - vertices[i][j][1]);
				glVertex3fv(vertices[i][j]);
			}
			glEnd();
		}
	}
	else{
		for(i=0; i<dDensity; i++){
			glBegin(GL_TRIANGLE_STRIP);
				for(j=0; j<=dDensity; j++){
					glColor3f(rgb[0] + vertices[i+1][j][6] - 1.0f, rgb[1] + vertices[i+1][j][6] - 1.0f, rgb[2] + vertices[i+1][j][6] - 1.0f);
					glTexCoord2d(vertices[i+1][j][3] - vertices[i+1][j][0], vertices[i+1][j][4] - vertices[i+1][j][1]);
					glVertex3fv(vertices[i+1][j]);
					glColor3f(rgb[0] + vertices[i][j][6] - 1.0f, rgb[1] + vertices[i][j][6] - 1.0f, rgb[2] + vertices[i][j][6] - 1.0f);
					glTexCoord2d(vertices[i][j][3] - vertices[i][j][0], vertices[i][j][4] - vertices[i][j][1]);
					glVertex3fv(vertices[i][j]);
				}
			glEnd();
		}
	}

	glPopMatrix();
}


void wisp::drawAsBackground(){
	int i, j;
	
	glPushMatrix();
	glTranslatef(c[0] * 0.2f, c[1] * 0.2f, 1.6f);

	if(dWireframe){
		for(i=1; i<dDensity; i++){
			glBegin(GL_LINE_STRIP);
			for(j=0; j<=dDensity; j++){
				glColor3f(rgb[0] + vertices[i][j][6] - 1.0f, rgb[1] + vertices[i][j][6] - 1.0f, rgb[2] + vertices[i][j][6] - 1.0f);
				glTexCoord2d(vertices[i][j][3] - vertices[i][j][0], vertices[i][j][4] - vertices[i][j][1]);
				glVertex3f(vertices[i][j][3], vertices[i][j][4], vertices[i][j][6]);
			}
			glEnd();
		}
		for(j=1; j<dDensity; j++){
			glBegin(GL_LINE_STRIP);
			for(i=0; i<=dDensity; i++){
				glColor3f(rgb[0] + vertices[i][j][6] - 1.0f, rgb[1] + vertices[i][j][6] - 1.0f, rgb[2] + vertices[i][j][6] - 1.0f);
				glTexCoord2d(vertices[i][j][3] - vertices[i][j][0], vertices[i][j][4] - vertices[i][j][1]);
				glVertex3f(vertices[i][j][3], vertices[i][j][4], vertices[i][j][6]);
			}
			glEnd();
		}
	}
	else{
		for(i=0; i<dDensity; i++){
			glBegin(GL_TRIANGLE_STRIP);
				for(j=0; j<=dDensity; j++){
					glColor3f(rgb[0] + vertices[i+1][j][6] - 1.0f, rgb[1] + vertices[i+1][j][6] - 1.0f, rgb[2] + vertices[i+1][j][6] - 1.0f);
					glTexCoord2d(vertices[i+1][j][3] - vertices[i+1][j][0], vertices[i+1][j][4] - vertices[i+1][j][1]);
					glVertex3f(vertices[i+1][j][3], vertices[i+1][j][4], vertices[i+1][j][6]);
					glColor3f(rgb[0] + vertices[i][j][6] - 1.0f, rgb[1] + vertices[i][j][6] - 1.0f, rgb[2] + vertices[i][j][6] - 1.0f);
					glTexCoord2d(vertices[i][j][3] - vertices[i][j][0], vertices[i][j][4] - vertices[i][j][1]);
					glVertex3f(vertices[i][j][3], vertices[i][j][4], vertices[i][j][6]);
				}
			glEnd();
		}
	}

	glPopMatrix();
}


void draw(){
	int i;

	// Update wisps
	for(i=0; i<dWisps; i++)
		wisps[i].update();
	for(i=0; i<dBackground; i++)
		backwisps[i].update();

	// Render feedback and copy to texture if necessary
	if(dFeedback){
		static float feedbackIntensity = float(dFeedback) / 101.0f;

		// update feedback variables
		for(i=0; i<4; i++){
			fr[i] += frameTime * fv[i];
			if(fr[i] > PIx2)
				fr[i] -= PIx2;
		}
		f[0] = 30.0f * cosf(fr[0]);
		f[1] = 0.2f * cosf(fr[1]);
		f[2] = 0.2f * cosf(fr[2]);
		f[3] = 0.8f * cosf(fr[3]);
		for(i=0; i<3; i++){
			lr[i] += frameTime * lv[i];
			if(lr[i] > PIx2)
				lr[i] -= PIx2;
			l[i] = cosf(lr[i]);
			l[i] = l[i] * l[i];
		}

		// Create drawing area for feedback texture
		glViewport(0, 0, feedbacktexsize, feedbacktexsize);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(30.0, aspectRatio, 0.01f, 20.0f);
		glMatrixMode(GL_MODELVIEW);

		// Draw
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(feedbackIntensity, feedbackIntensity, feedbackIntensity);
		glBindTexture(GL_TEXTURE_2D, feedbacktex);
		glPushMatrix();
		glTranslatef(f[1] * l[1], f[2] * l[1], f[3] * l[2]);
		glRotatef(f[0] * l[0], 0, 0, 1);
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(-0.5f, -0.5f);
			glVertex3f(-aspectRatio*2.0f, -2.0f, 1.25f);
			glTexCoord2f(1.5f, -0.5f);
			glVertex3f(aspectRatio*2.0f, -2.0f, 1.25f);
			glTexCoord2f(-0.5f, 1.5f);
			glVertex3f(-aspectRatio*2.0f, 2.0f, 1.25f);
			glTexCoord2f(1.5f, 1.5f);
			glVertex3f(aspectRatio*2.0f, 2.0f, 1.25f);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, tex);
		for(i=0; i<dBackground; i++)
			backwisps[i].drawAsBackground();
		for(i=0; i<dWisps; i++)
			wisps[i].draw();

		// readback feedback texture
		glReadBuffer(GL_BACK);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, feedbacktexsize);
		glBindTexture(GL_TEXTURE_2D, feedbacktex);
		//glReadPixels(0, 0, feedbacktexsize, feedbacktexsize, GL_RGB, GL_UNSIGNED_BYTE, feedbackmap);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, feedbacktexsize, feedbacktexsize, GL_RGB, GL_UNSIGNED_BYTE, feedbackmap);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, feedbacktexsize, feedbacktexsize);

		// create regular drawing area
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(20.0, aspectRatio, 0.01f, 20.0f);
		glMatrixMode(GL_MODELVIEW);

		// Draw again
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(feedbackIntensity, feedbackIntensity, feedbackIntensity);
		glPushMatrix();
		glTranslatef(f[1] * l[1], f[2] * l[1], f[3] * l[2]);
		glRotatef(f[0] * l[0], 0, 0, 1);
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(-0.5f, -0.5f);
			glVertex3f(-aspectRatio*2.0f, -2.0f, 1.25f);
			glTexCoord2f(1.5f, -0.5f);
			glVertex3f(aspectRatio*2.0f, -2.0f, 1.25f);
			glTexCoord2f(-0.5f, 1.5f);
			glVertex3f(-aspectRatio*2.0f, 2.0f, 1.25f);
			glTexCoord2f(1.5f, 1.5f);
			glVertex3f(aspectRatio*2.0f, 2.0f, 1.25f);
		glEnd();
		glPopMatrix();
	}
	// Just clear the screen if feedback is not in use
	else
		glClear(GL_COLOR_BUFFER_BIT);

	// draw regular top layer
	glBindTexture(GL_TEXTURE_2D, tex);
	for(i=0; i<dBackground; i++)
		backwisps[i].drawAsBackground();
	for(i=0; i<dWisps; i++)
		wisps[i].draw();

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
	RECT rect;

	srand((unsigned)time(NULL));

	// Window initialization
	hdc = GetDC(hwnd);
	SetBestPixelFormat(hdc);
	hglrc = wglCreateContext(hdc);
	GetClientRect(hwnd, &rect);
	wglMakeCurrent(hdc, hglrc);
	viewport[0] = rect.left;
	viewport[1] = rect.top;
	viewport[2] = rect.right - rect.left;
	viewport[3] = rect.bottom - rect.top;
	glViewport(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
	aspectRatio = float(rect.right) / float(rect.bottom);

	// setup regular drawing area just in case feedback isn't used
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(20.0, float(rect.right) / float(rect.bottom), 0.01, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glLineWidth(2.0f);
	// Commented out because smooth lines and textures don't mix on my TNT.
	// It's like it rendering in software mode
	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	if(dTexture){
		int whichtex = dTexture;
		if(whichtex == 4)  // random texture
			whichtex = rsRandi(3) + 1;
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		// Initialize texture
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		switch(whichtex){
		case 1:
			gluBuild2DMipmaps(GL_TEXTURE_2D, 1, TEXSIZE, TEXSIZE, GL_LUMINANCE, GL_UNSIGNED_BYTE, plasmamap);
			break;
		case 2:
			gluBuild2DMipmaps(GL_TEXTURE_2D, 1, TEXSIZE, TEXSIZE, GL_LUMINANCE, GL_UNSIGNED_BYTE, stringymap);
			break;
		case 3:
			gluBuild2DMipmaps(GL_TEXTURE_2D, 1, TEXSIZE, TEXSIZE, GL_LUMINANCE, GL_UNSIGNED_BYTE, linesmap);
		}
	}

	if(dFeedback){
		feedbacktexsize = int(pow(2, dFeedbacksize));
		while(feedbacktexsize > viewport[2] || feedbacktexsize > viewport[3]){
			dFeedbacksize -= 1;
			feedbacktexsize = int(pow(2, dFeedbacksize));
		}

		// feedback texture setup
		glEnable(GL_TEXTURE_2D);
		//feedbackmap = new unsigned char[feedbacktexsize*feedbacktexsize*3];
		glGenTextures(1, &feedbacktex);
		glBindTexture(GL_TEXTURE_2D, feedbacktex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, feedbacktexsize, feedbacktexsize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		// feedback velocity variable setup
		fv[0] = float(dFeedbackspeed) * (rsRandf(0.025f) + 0.025f);
		fv[1] = float(dFeedbackspeed) * (rsRandf(0.05f) + 0.05f);
		fv[2] = float(dFeedbackspeed) * (rsRandf(0.05f) + 0.05f);
		fv[3] = float(dFeedbackspeed) * (rsRandf(0.1f) + 0.1f);
		lv[0] = float(dFeedbackspeed) * (rsRandf(0.0025f) + 0.0025f);
		lv[1] = float(dFeedbackspeed) * (rsRandf(0.0025f) + 0.0025f);
		lv[2] = float(dFeedbackspeed) * (rsRandf(0.0025f) + 0.0025f);
	}

	// Initialize wisps
	wisps = new wisp[dWisps];
	backwisps = new wisp[dBackground];

	// Initialize text
	textwriter = new rsText;
}

void cleanUp(HWND hwnd){
	// Free memory
	delete[] wisps;

	// Kill device context
	ReleaseDC(hwnd, hdc);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
}


void setDefaults(int which){
	switch(which){
	case DEFAULTS1:  // Regular
		dWisps = 5;
		dBackground = 0;
		dDensity = 35;
		dVisibility = 35;
		dSpeed = 15;
		dFeedback = 0;
		dFeedbackspeed = 1;
		dFeedbacksize = 10;
		dWireframe = 0;
		dTexture = 2;
		break;
	case DEFAULTS2:  // Grid
		dWisps = 4;
		dBackground = 1;
		dDensity = 30;
		dVisibility = 70;
		dSpeed = 15;
		dFeedback = 0;
		dFeedbackspeed = 1;
		dFeedbacksize = 10;
		dWireframe = 1;
		dTexture = 0;
		break;
	case DEFAULTS3:  // Cubism
		dWisps = 15;
		dBackground = 0;
		dDensity = 4;
		dVisibility = 15;
		dSpeed = 10;
		dFeedback = 0;
		dFeedbackspeed = 1;
		dFeedbacksize = 10;
		dWireframe = 0;
		dTexture = 0;
		break;
	case DEFAULTS4:  // Bad math
		dWisps = 2;
		dBackground = 2;
		dDensity = 20;
		dVisibility = 40;
		dSpeed = 30;
		dFeedback = 40;
		dFeedbackspeed = 5;
		dFeedbacksize = 10;
		dWireframe = 1;
		dTexture = 2;
		break;
	case DEFAULTS5:  // M-Theory
		dWisps = 3;
		dBackground = 0;
		dDensity = 35;
		dVisibility = 15;
		dSpeed = 20;
		dFeedback = 40;
		dFeedbackspeed = 20;
		dFeedbacksize = 10;
		dWireframe = 0;
		dTexture = 0;
		break;
	case DEFAULTS6:  // ultra high frequency tunneling electron microscope
		dWisps = 0;
		dBackground = 3;
		dDensity = 35;
		dVisibility = 5;
		dSpeed = 50;
		dFeedback = 0;
		dFeedbackspeed = 1;
		dFeedbacksize = 10;
		dWireframe = 0;
		dTexture = 0;
		break;
	case DEFAULTS7:  // Nowhere
		dWisps = 0;
		dBackground = 3;
		dDensity = 30;
		dVisibility = 40;
		dSpeed = 20;
		dFeedback = 80;
		dFeedbackspeed = 10;
		dFeedbacksize = 10;
		dWireframe = 1;
		dTexture = 3;
		break;
	case DEFAULTS8:  // Echo
		dWisps = 3;
		dBackground = 0;
		dDensity = 35;
		dVisibility = 30;
		dSpeed = 20;
		dFeedback = 85;
		dFeedbackspeed = 30;
		dFeedbacksize = 10;
		dWireframe = 0;
		dTexture = 1;
		break;
	case DEFAULTS9:  // Kaleidoscope
		dWisps = 3;
		dBackground = 0;
		dDensity = 35;
		dVisibility = 40;
		dSpeed = 15;
		dFeedback = 90;
		dFeedbackspeed = 3;
		dFeedbacksize = 10;
		dWireframe = 0;
		dTexture = 0;
		break;
	}
}


// Initialize all user-defined stuff
void readRegistry(){
	LONG result;
	HKEY skey;
	DWORD valtype, valsize, val;

	setDefaults(DEFAULTS1);

	result = RegOpenKeyEx(HKEY_CURRENT_USER, registryPath, 0, KEY_READ, &skey);
	if(result != ERROR_SUCCESS)
		return;

	valsize=sizeof(val);

	result = RegQueryValueEx(skey, "Wisps", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dWisps = val;
	result = RegQueryValueEx(skey, "Background", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dBackground = val;
	result = RegQueryValueEx(skey, "Density", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dDensity = val;
	result = RegQueryValueEx(skey, "Visibility", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dVisibility = val;
	result = RegQueryValueEx(skey, "Speed", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dSpeed = val;
	result = RegQueryValueEx(skey, "Feedback", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dFeedback = val;
	result = RegQueryValueEx(skey, "Feedbackspeed", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dFeedbackspeed = val;
	result = RegQueryValueEx(skey, "Feedbacksize", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dFeedbacksize = val;
	result = RegQueryValueEx(skey, "Texture", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dTexture = val;
	result = RegQueryValueEx(skey, "Wireframe", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dWireframe = val;
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

	val = dWisps;
	RegSetValueEx(skey, "Wisps", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dBackground;
	RegSetValueEx(skey, "Background", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dDensity;
	RegSetValueEx(skey, "Density", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dVisibility;
	RegSetValueEx(skey, "Visibility", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dSpeed;
	RegSetValueEx(skey, "Speed", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dFeedback;
	RegSetValueEx(skey, "Feedback", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dFeedbackspeed;
	RegSetValueEx(skey, "Feedbackspeed", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dFeedbacksize;
	RegSetValueEx(skey, "Feedbacksize", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dTexture;
	RegSetValueEx(skey, "Texture", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dWireframe;
	RegSetValueEx(skey, "Wireframe", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dFrameRateLimit;
	RegSetValueEx(skey, "FrameRateLimit", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));

	RegCloseKey(skey);
}


BOOL CALLBACK aboutProc(HWND hdlg, UINT msg, WPARAM wpm, LPARAM lpm){
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
			ShellExecute(NULL, "open", "http://www.reallyslick.com", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	return FALSE;
}


void initControls(HWND hdlg){
	char cval[16];

	SendDlgItemMessage(hdlg, WISPS, UDM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(100), DWORD(0))));
	SendDlgItemMessage(hdlg, WISPS, UDM_SETPOS, 0, LPARAM(dWisps));

	SendDlgItemMessage(hdlg, BACKGROUND, UDM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(100), DWORD(0))));
	SendDlgItemMessage(hdlg, BACKGROUND, UDM_SETPOS, 0, LPARAM(dBackground));

	SendDlgItemMessage(hdlg, DENSITY, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(2), DWORD(100))));
	SendDlgItemMessage(hdlg, DENSITY, TBM_SETPOS, 1, LPARAM(dDensity));
	SendDlgItemMessage(hdlg, DENSITY, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, DENSITY, TBM_SETPAGESIZE, 0, LPARAM(5));
	sprintf(cval, "%d", dDensity);
	SendDlgItemMessage(hdlg, DENSITYTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, VISIBILITY, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(100))));
	SendDlgItemMessage(hdlg, VISIBILITY, TBM_SETPOS, 1, LPARAM(dVisibility));
	SendDlgItemMessage(hdlg, VISIBILITY, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, VISIBILITY, TBM_SETPAGESIZE, 0, LPARAM(5));
	sprintf(cval, "%d", dVisibility);
	SendDlgItemMessage(hdlg, VISIBILITYTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, SPEED, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(100))));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETPOS, 1, LPARAM(dSpeed));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETPAGESIZE, 0, LPARAM(5));
	sprintf(cval, "%d", dSpeed);
	SendDlgItemMessage(hdlg, SPEEDTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, FEEDBACK, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(0), DWORD(100))));
	SendDlgItemMessage(hdlg, FEEDBACK, TBM_SETPOS, 1, LPARAM(dFeedback));
	SendDlgItemMessage(hdlg, FEEDBACK, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, FEEDBACK, TBM_SETPAGESIZE, 0, LPARAM(5));
	sprintf(cval, "%d", dFeedback);
	SendDlgItemMessage(hdlg, FEEDBACKTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, FEEDBACKSPEED, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(100))));
	SendDlgItemMessage(hdlg, FEEDBACKSPEED, TBM_SETPOS, 1, LPARAM(dFeedbackspeed));
	SendDlgItemMessage(hdlg, FEEDBACKSPEED, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, FEEDBACKSPEED, TBM_SETPAGESIZE, 0, LPARAM(5));
	sprintf(cval, "%d", dFeedbackspeed);
	SendDlgItemMessage(hdlg, FEEDBACKSPEEDTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, FEEDBACKSIZE, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(10))));
	SendDlgItemMessage(hdlg, FEEDBACKSIZE, TBM_SETPOS, 1, LPARAM(dFeedbacksize));
	SendDlgItemMessage(hdlg, FEEDBACKSIZE, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, FEEDBACKSIZE, TBM_SETPAGESIZE, 0, LPARAM(1));
	sprintf(cval, "%d", int(pow(2, dFeedbacksize)));
	SendDlgItemMessage(hdlg, FEEDBACKSIZETEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(4), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(3), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(2), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(1), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(0), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("none"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("Plasma"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("Stringy"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("Linear"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("random"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_SETCURSEL, WPARAM(dTexture), 0);

	CheckDlgButton(hdlg, WIREFRAME, dWireframe);

	initFrameRateLimitSlider(hdlg, FRAMERATELIMIT, FRAMERATELIMITTEXT);
}


BOOL ScreenSaverConfigureDialog(HWND hdlg, UINT msg, WPARAM wpm, LPARAM lpm){
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
			dWisps = SendDlgItemMessage(hdlg, WISPS, UDM_GETPOS, 0, 0);
			if(dWisps < 0)
				dWisps = 0;
			if(dWisps > 100)
				dWisps = 100;
			dBackground = SendDlgItemMessage(hdlg, BACKGROUND, UDM_GETPOS, 0, 0);
			if(dBackground < 0)
				dBackground = 0;
			if(dBackground > 100)
				dBackground = 100;
            dDensity = SendDlgItemMessage(hdlg, DENSITY, TBM_GETPOS, 0, 0);
			dVisibility = SendDlgItemMessage(hdlg, VISIBILITY, TBM_GETPOS, 0, 0);
			dSpeed = SendDlgItemMessage(hdlg, SPEED, TBM_GETPOS, 0, 0);
			dFeedback = SendDlgItemMessage(hdlg, FEEDBACK, TBM_GETPOS, 0, 0);
			dFeedbackspeed = SendDlgItemMessage(hdlg, FEEDBACKSPEED, TBM_GETPOS, 0, 0);
			dFeedbacksize = SendDlgItemMessage(hdlg, FEEDBACKSIZE, TBM_GETPOS, 0, 0);
			dTexture = SendDlgItemMessage(hdlg, TEXTURE, CB_GETCURSEL, 0, 0);
			dWireframe = (IsDlgButtonChecked(hdlg, WIREFRAME) == BST_CHECKED);
			dFrameRateLimit = SendDlgItemMessage(hdlg, FRAMERATELIMIT, TBM_GETPOS, 0, 0);
			writeRegistry();
            // Fall through
        case IDCANCEL:
            EndDialog(hdlg, LOWORD(wpm));
            break;
		case DEFAULTS1:
		case DEFAULTS2:
		case DEFAULTS3:
		case DEFAULTS4:
		case DEFAULTS5:
		case DEFAULTS6:
		case DEFAULTS7:
		case DEFAULTS8:
		case DEFAULTS9:
			setDefaults(wpm);
			initControls(hdlg);
			break;
		case ABOUT:
			DialogBox(mainInstance, MAKEINTRESOURCE(DLG_ABOUT), hdlg, DLGPROC(aboutProc));
		}
        return TRUE;
	case WM_HSCROLL:
		if(HWND(lpm) == GetDlgItem(hdlg, DENSITY)){
			ival = SendDlgItemMessage(hdlg, DENSITY, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, DENSITYTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, VISIBILITY)){
			ival = SendDlgItemMessage(hdlg, VISIBILITY, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, VISIBILITYTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, SPEED)){
			ival = SendDlgItemMessage(hdlg, SPEED, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, SPEEDTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, FEEDBACK)){
			ival = SendDlgItemMessage(hdlg, FEEDBACK, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, FEEDBACKTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, FEEDBACKSPEED)){
			ival = SendDlgItemMessage(hdlg, FEEDBACKSPEED, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, FEEDBACKSPEEDTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, FEEDBACKSIZE)){
			ival = SendDlgItemMessage(hdlg, FEEDBACKSIZE, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", int(pow(2, ival)));
			SendDlgItemMessage(hdlg, FEEDBACKSIZETEXT, WM_SETTEXT, 0, LPARAM(cval));
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
		// Don't bother with feedback when doing the preview.
		// The window isn't big enough to make it worthwhile.
		if(doingPreview)
			dFeedback = 0;
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
