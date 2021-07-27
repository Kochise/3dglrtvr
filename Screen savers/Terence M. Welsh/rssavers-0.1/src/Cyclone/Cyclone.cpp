/*
 * Copyright (C) 1999-2005  Terence M. Welsh
 *
 * This file is part of Cyclone.
 *
 * Cyclone is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 *
 * Cyclone is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


// Cyclone screen saver


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


class cyclone;
class particle;


#define PIx2 6.28318530718f
#define PI 3.14159265359f
#define wide 200
#define high 200

// Global variables
LPCTSTR registryPath = ("Software\\Really Slick\\Cyclone");
HGLRC hglrc;
HDC hdc;
int readyToDraw = 0;
// Parameters edited in the dialog box
int dCyclones;
int dParticles;
int dSize;
int dComplexity;
int dSpeed;
BOOL dStretch;
BOOL dShowCurves;
// Other globals
float aspectRatio;
cyclone **cyclones;
particle **particles;
float fact[13];
float frameTime = 0.0f;
// text output
rsText* textwriter;


// useful random functions
inline int rsRandi(int x){
	return rand() % x;
}
inline float rsRandf(float x){
	return x * (float(rand()) / float(RAND_MAX));
}


// useful vector functions
float length(float *xyz){
	return(float(sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2])));
}
float normalize(float *xyz){
	float length = float(sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]));
	if(length == 0.0)
		return(0.0);
	xyz[0] /= length;
	xyz[1] /= length;
	xyz[2] /= length;
	return(length);
}
float dot(float *xyz1, float *xyz2){
	return(xyz1[0] * xyz2[0] + xyz1[1] * xyz2[1] + xyz1[2] * xyz2[2]);
}
void cross(float *xyz1, float *xyz2, float *xyzOut){
	xyzOut[0] = xyz1[1] * xyz2[2] - xyz2[1] * xyz1[2];
	xyzOut[1] = xyz1[2] * xyz2[0] - xyz2[2] * xyz1[0];
	xyzOut[2] = xyz1[0] * xyz2[1] - xyz2[0] * xyz1[1];
}


// useful factorial function
int factorial(int x){
    int returnval = 1;

    if(x == 0)
        return(1);
    else{
        do{
            returnval *= x;
            x -= 1;
        }
        while(x!=0);
    }
    return(returnval);
}


class cyclone{
public:
	float **targetxyz;
	float **xyz;
	float **oldxyz;
	float *targetWidth;
	float *width;
	float *oldWidth;
	float targethsl[3];
	float hsl[3];
	float oldhsl[3];
	float **xyzChange;
	float **widthChange;
	float hslChange[2];

	cyclone();
	virtual ~cyclone(){};
	void update();
};

cyclone::cyclone(){
	int i;

	// Initialize position stuff
	targetxyz = new float*[dComplexity+3];
	xyz = new float*[dComplexity+3];
	oldxyz = new float*[dComplexity+3];
	for(i=0; i<int(dComplexity)+3; i++){
		targetxyz[i] = new float[3];
		xyz[i] = new float[3];
		oldxyz[i] = new float[3];
	}
	xyz[dComplexity+2][0] = rsRandf(float(wide*2)) - float(wide);
	xyz[dComplexity+2][1] = float(high);
	xyz[dComplexity+2][2] = rsRandf(float(wide*2)) - float(wide);
	xyz[dComplexity+1][0] = xyz[dComplexity+2][0];
	xyz[dComplexity+1][1] = rsRandf(float(high / 3)) + float(high / 4);
	xyz[dComplexity+1][2] = xyz[dComplexity+2][2];
	for(i=dComplexity; i>1; i--){
		xyz[i][0] = xyz[i+1][0] + rsRandf(float(wide)) - float(wide / 2);
		xyz[i][1] = rsRandf(float(high * 2)) - float(high);
		xyz[i][2] = xyz[i+1][2] + rsRandf(float(wide)) - float(wide / 2);
	}
	xyz[1][0] = xyz[2][0] + rsRandf(float(wide / 2)) - float(wide / 4);
	xyz[1][1] = -rsRandf(float(high / 2)) - float(high / 4);
	xyz[1][2] = xyz[2][2] + rsRandf(float(wide / 2)) - float(wide / 4);
	xyz[0][0] = xyz[1][0] + rsRandf(float(wide / 8)) - float(wide / 16);
	xyz[0][1] = float(-high);
	xyz[0][2] = xyz[1][2] + rsRandf(float(wide / 8)) - float(wide / 16);
	// Initialize width stuff
	targetWidth = new float[dComplexity+3];
	width = new float[dComplexity+3];
	oldWidth = new float[dComplexity+3];
	width[dComplexity+2] = rsRandf(175.0f) + 75.0f;
	width[dComplexity+1] = rsRandf(60.0f) + 15.0f;
	for(i=dComplexity; i>1; i--)
		width[i] = rsRandf(25.0f) + 15.0f;
	width[1] = rsRandf(25.0f) + 5.0f;
	width[0] = rsRandf(15.0f) + 5.0f;
	// Initialize transition stuff
	xyzChange = new float*[dComplexity + 3];
	widthChange = new float*[dComplexity + 3];
	for(i=0; i<(dComplexity+3); i++){
		xyzChange[i] = new float[2];	// 0 = step   1 = total steps
		widthChange[i] = new float[2];
		xyzChange[i][0] = 0.0f;
		xyzChange[i][1] = 0.0f;
		widthChange[i][0] = 0.0f;
		widthChange[i][1] = 0.0f;
	}
	// Initialize color stuff
	hsl[0] = oldhsl[0] = rsRandf(1.0f);
	hsl[1] = oldhsl[1] = rsRandf(1.0f);
	hsl[2] = oldhsl[2] = 0.0f;  // start out dark
	targethsl[0] = rsRandf(1.0f);
	targethsl[1] = rsRandf(1.0f);
	targethsl[2] = 1.0f;
	hslChange[0] = 0.0f;
	hslChange[1] = 10.0f;
}

void cyclone::update(){
	int i;
	int temp;
	float between;
	float diff;
	int direction;
	float point[3];
	float step;
	float blend;

	// update cyclone's path
	temp = dComplexity + 2;
	if(xyzChange[temp][0] >= xyzChange[temp][1]){
		oldxyz[temp][0] = xyz[temp][0];
		oldxyz[temp][1] = xyz[temp][1];
		oldxyz[temp][2] = xyz[temp][2];
		targetxyz[temp][0] = rsRandf(float(wide*2)) - float(wide);
		targetxyz[temp][1] = float(high);
		targetxyz[temp][2] = rsRandf(float(wide*2)) - float(wide);
		xyzChange[temp][0] = 0.0f;
		xyzChange[temp][1] = rsRandf(150.0f / float(dSpeed)) + 75.0f / float(dSpeed);
	}
	temp = dComplexity + 1;
	if(xyzChange[temp][0] >= xyzChange[temp][1]){
		oldxyz[temp][0] = xyz[temp][0];
		oldxyz[temp][1] = xyz[temp][1];
		oldxyz[temp][2] = xyz[temp][2];
		targetxyz[temp][0] = xyz[temp+1][0];
		targetxyz[temp][1] = rsRandf(float(high / 3)) + float(high / 4);
		targetxyz[temp][2] = xyz[temp+1][2];
		xyzChange[temp][0] = 0.0f;
		xyzChange[temp][1] = rsRandf(100.0f / float(dSpeed)) + 75.0f / float(dSpeed);
	}
	for(i=dComplexity; i>1; i--){
		if(xyzChange[i][0] >= xyzChange[i][1]){
			oldxyz[i][0] = xyz[i][0];
			oldxyz[i][1] = xyz[i][1];
			oldxyz[i][2] = xyz[i][2];
			targetxyz[i][0] = targetxyz[i+1][0] + (targetxyz[i+1][0] - targetxyz[i+2][0]) / 2.0f + rsRandf(float(wide / 2)) - float(wide / 4);
			targetxyz[i][1] = (targetxyz[i+1][1] + targetxyz[i-1][1]) / 2.0f + rsRandf(float(high / 8)) - float(high / 16);
			targetxyz[i][2] = targetxyz[i+1][2] + (targetxyz[i+1][2] - targetxyz[i+2][2]) / 2.0f + rsRandf(float(wide / 2)) - float(wide / 4);
			if(targetxyz[i][1] > high)
				targetxyz[i][1] = high;
			if(targetxyz[i][1] < -high)
				targetxyz[i][1] = -high;
			xyzChange[i][0] = 0.0f;
			xyzChange[i][1] = rsRandf(75.0f / float(dSpeed)) + 50.0f / float(dSpeed);
		}
	}
	if(xyzChange[1][0] >= xyzChange[1][1]){
		oldxyz[1][0] = xyz[1][0];
		oldxyz[1][1] = xyz[1][1];
		oldxyz[1][2] = xyz[1][2];
		targetxyz[1][0] = targetxyz[2][0] + rsRandf(float(wide / 2)) - float(wide / 4);
		targetxyz[1][1] = -rsRandf(float(high / 2)) - float(high / 4);
		targetxyz[1][2] = targetxyz[2][2] + rsRandf(float(wide / 2)) - float(wide / 4);
		xyzChange[1][0] = 0.0f;
		xyzChange[1][1] = rsRandf(50.0f / float(dSpeed)) + 30.0f / float(dSpeed);
	}
	if(xyzChange[0][0] >= xyzChange[0][1]){
		oldxyz[0][0] = xyz[0][0];
		oldxyz[0][1] = xyz[0][1];
		oldxyz[0][2] = xyz[0][2];
		targetxyz[0][0] = xyz[1][0] + rsRandf(float(wide / 8)) - float(wide / 16);
		targetxyz[0][1] = float(-high);
		targetxyz[0][2] = xyz[1][2] + rsRandf(float(wide / 8)) - float(wide / 16);
		xyzChange[0][0] = 0.0f;
		xyzChange[0][1] = rsRandf(100.0f / float(dSpeed)) + 75.0f / float(dSpeed);
	}
	for(i=0; i<(dComplexity+3); i++){
		between = xyzChange[i][0] / xyzChange[i][1] * PIx2;
		between = (1.0f - float(cos(between))) / 2.0f; 
		xyz[i][0] = ((targetxyz[i][0] - oldxyz[i][0]) * between) + oldxyz[i][0];
		xyz[i][1] = ((targetxyz[i][1] - oldxyz[i][1]) * between) + oldxyz[i][1];
		xyz[i][2] = ((targetxyz[i][2] - oldxyz[i][2]) * between) + oldxyz[i][2];
		xyzChange[i][0] += frameTime;
	}

	// Update cyclone's widths
	temp = dComplexity + 2;
	if(widthChange[temp][0] >= widthChange[temp][1]){
		oldWidth[temp] = width[temp];
		targetWidth[temp] = rsRandf(225.0f) + 75.0f;
		widthChange[temp][0] = 0.0f;
		widthChange[temp][1] = rsRandf(50.0f / float(dSpeed)) + 50.0f / float(dSpeed);
	}
	temp = dComplexity + 1;
	if(widthChange[temp][0] >= widthChange[temp][1]){
		oldWidth[temp] = width[temp];
		targetWidth[temp] = rsRandf(100.0f) + 15.0f;
		widthChange[temp][0] = 0.0f;
		widthChange[temp][1] = rsRandf(50.0f / float(dSpeed)) + 50.0f / float(dSpeed);
	}
	for(i=dComplexity; i>1; i--){
		if(widthChange[i][0] >= widthChange[i][1]){
			oldWidth[i] = width[i];
			targetWidth[i] = rsRandf(50.0f) + 15.0f;
			widthChange[i][0] = 0.0f;
			widthChange[i][1] = rsRandf(50.0f / float(dSpeed)) + 40.0f / float(dSpeed);
		}
	}
	if(widthChange[1][0] >= widthChange[1][1]){
		oldWidth[1] = width[1];
		targetWidth[1] = rsRandf(40.0f) + 5.0f;
		widthChange[1][0] = 0.0f;
		widthChange[1][1] = rsRandf(50.0f / float(dSpeed)) + 30.0f / float(dSpeed);
	}
	if(widthChange[0][0] >= widthChange[0][1]){
		oldWidth[0] = width[0];
		targetWidth[0] = rsRandf(30.0f) + 5.0f;
		widthChange[0][0] = 0.0f;
		widthChange[0][1] = rsRandf(50.0f / float(dSpeed)) + 20.0f / float(dSpeed);
	}
	for(i=0; i<(dComplexity+3); i++){
		between = widthChange[i][0] / widthChange[i][1];
		width[i] = ((targetWidth[i] - oldWidth[i]) * between) + oldWidth[i];
		widthChange[i][0] += frameTime;
	}

	// Update cyclones color
	if(hslChange[0] >= hslChange[1]){
		oldhsl[0] = hsl[0];
		oldhsl[1] = hsl[1];
		oldhsl[2] = hsl[2];
		targethsl[0] = rsRandf(1.0f);
		targethsl[1] = rsRandf(1.0f);
		targethsl[2] = rsRandf(1.0f) + 0.5f;
		if(targethsl[2] > 1.0f)
			targethsl[2] = 1.0f;
		hslChange[0] = 0.0f;
		hslChange[1] = rsRandf(30.0f) + 2.0f;
	}
	between = hslChange[0] / hslChange[1];
	diff = targethsl[0] - oldhsl[0];
	direction = 0;
	if((targethsl[0] > oldhsl[0] && diff > 0.5f) || (targethsl[0] < oldhsl[0] && diff < -0.5f))
		if(diff > 0.5f)
			direction = 1;
	hslTween(oldhsl[0], oldhsl[1], oldhsl[2],
			targethsl[0], targethsl[1], targethsl[2], between, direction,
			hsl[0], hsl[1], hsl[2]);
	hslChange[0] += frameTime;

	if(dShowCurves){
		glDisable(GL_LIGHTING);
		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		for(step=0.0; step<1.0; step+=0.02f){
			point[0] = point[1] = point[2] = 0.0f;
			for(i=0; i<(dComplexity+3); i++){
				blend = fact[dComplexity+2] / (fact[i]
					* fact[dComplexity+2-i]) * powf(step, float(i))
					* powf((1.0f - step), float(dComplexity+2-i));
				point[0] += xyz[i][0] * blend;
				point[1] += xyz[i][1] * blend;
				point[2] += xyz[i][2] * blend;
			}
			glVertex3fv(point);
		}
		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		for(i=0; i<(dComplexity+3); i++)
			glVertex3fv(&xyz[i][0]);
		glEnd();
		glEnable(GL_LIGHTING);
	}
}


class particle{
public:
	float r, g, b;
	float xyz[3], lastxyz[3];
	float width;
	float step;
	float spinAngle;
	cyclone *cy;

	particle(cyclone *);
	virtual ~particle(){};
	void init();
	void update();
};

particle::particle(cyclone *c){
	cy = c;
	init();
}

void particle::init(){
	width = rsRandf(0.8f) + 0.2f;
	step = 0.0f;
	spinAngle = rsRandf(360);
	hsl2rgb(cy->hsl[0], cy->hsl[1], cy->hsl[2], r, g, b);
}

void particle::update(){
	int i;
	float scale, temp;
	float newStep;
	float newSpinAngle;
	float cyWidth;
	float between;
	float dir[3];
	float crossVec[3];
	float tiltAngle;
	float up[3] = {0.0f, 1.0f, 0.0f};
	float blend;

	lastxyz[0] = xyz[0];
	lastxyz[1] = xyz[1];
	lastxyz[2] = xyz[2];
	if(step > 1.0f)
		init();
	xyz[0] = xyz[1] = xyz[2] = 0.0f;
	for(i=0; i<(dComplexity+3); i++){
		blend = fact[dComplexity+2] / (fact[i]
			* fact[dComplexity+2-i]) * powf(step, float(i))
			* powf((1.0f - step), float(dComplexity+2-i));
		xyz[0] += cy->xyz[i][0] * blend;
		xyz[1] += cy->xyz[i][1] * blend;
		xyz[2] += cy->xyz[i][2] * blend;
	}
	dir[0] = dir[1] = dir[2] = 0.0f;
	for(i=0; i<(dComplexity+3); i++){
		blend = fact[dComplexity+2] / (fact[i]
			* fact[dComplexity+2-i]) * powf(step - 0.01f, float(i))
			* powf((1.0f - (step - 0.01f)), float(dComplexity+2-i));
		dir[0] += cy->xyz[i][0] * blend;
		dir[1] += cy->xyz[i][1] * blend;
		dir[2] += cy->xyz[i][2] * blend;
	}
	dir[0] = xyz[0] - dir[0];
	dir[1] = xyz[1] - dir[1];
	dir[2] = xyz[2] - dir[2];
	normalize(dir);
	cross(dir, up, crossVec);
	tiltAngle = -acosf(dot(dir, up)) * 180.0f / PI;
	i = int(step * (float(dComplexity) + 2.0f));
	if(i >= (dComplexity + 2))
		i = dComplexity + 1;
	between = (step - (float(i) / float(dComplexity + 2))) * float(dComplexity + 2);
	cyWidth = cy->width[i] * (1.0f - between) + cy->width[i+1] * (between);
	newStep = (0.2f * frameTime * float(dSpeed)) / (width * width * cyWidth);
	step += newStep;
	newSpinAngle = (1500.0f * frameTime * float(dSpeed)) / (width * cyWidth);
	spinAngle += newSpinAngle;
	if(dStretch){
		scale = width * cyWidth * newSpinAngle * 0.02f;
		temp = cyWidth * 2.0f / float(dSize);
		if(scale > temp)
			scale = temp;
		if(scale < 3.0f)
			scale = 3.0f;
	}
	glColor3f(r, g, b);
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(xyz[0], xyz[1], xyz[2]);
		glRotatef(tiltAngle, crossVec[0], crossVec[1], crossVec[2]);
		glRotatef(spinAngle, 0, 1, 0);
		glTranslatef(width * cyWidth, 0, 0);
		if(dStretch)
			glScalef(1.0f, 1.0f, scale);
		glCallList(1);
	glPopMatrix();
}


void draw(){
	int i, j;

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(i=0; i<dCyclones; i++){
		cyclones[i]->update();
		for(j=(i * dParticles); j<((i+1) * dParticles); j++)
			particles[j]->update();
	}

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

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	aspectRatio = float(rect.right) / float(rect.bottom);
	gluPerspective(80.0, aspectRatio, 50, 3000);
	if(!rsRandi(500)){  // Easter egg view
		glRotatef(90, 1, 0, 0);
		glTranslatef(0.0f, -(wide * 2), 0.0f);
	}
	else  // Normal view
		glTranslatef(0.0f, 0.0f, -(wide * 2));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glNewList(1, GL_COMPILE);
		GLUquadricObj *qobj = gluNewQuadric();
		gluSphere(qobj, float(dSize) / 4.0f, 3, 2);
		gluDeleteQuadric(qobj);
	glEndList();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float ambient[4] = {0.25f, 0.25f, 0.25f, 0.0f};
	float diffuse[4] = {1.0f, 1.0f, 1.0f, 0.0f};
	float specular[4] = {1.0f, 1.0f, 1.0f, 0.0f};
	float position[4] = {float(wide * 2), -float(high), float(wide * 2), 0.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	glColor3f(0.7f, 0.7f, 0.7f);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Initialize cyclones and their particles
	for(i=0; i<13; i++)
		fact[i] = float(factorial(i));
	cyclones = new cyclone*[dCyclones];
	particles = new particle*[dParticles * dCyclones];
	for(i=0; i<dCyclones; i++){
		cyclones[i] = new cyclone;
		for(j=i*dParticles; j<((i+1)*dParticles); j++)
			particles[j] = new particle(cyclones[i]);
	}

	// Initialize text
	textwriter = new rsText;
}


void cleanUp(HWND hwnd){
	// Free memory
	delete[] particles;
	delete[] cyclones;

	// Kill device context
	ReleaseDC(hwnd, hdc);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
}


void setDefaults(){
	dCyclones = 1;
	dParticles = 400;
	dSize = 7;
	dComplexity = 3;
	dSpeed = 10;
	dStretch = TRUE;
	dShowCurves = FALSE;
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

	result = RegQueryValueEx(skey, "Cyclones", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dCyclones = val;
	result = RegQueryValueEx(skey, "Particles", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dParticles = val;
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
	result = RegQueryValueEx(skey, "ShowCurves", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dShowCurves = val;
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

	val = dCyclones;
	RegSetValueEx(skey, "Cyclones", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dParticles;
	RegSetValueEx(skey, "Particles", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dSize;
	RegSetValueEx(skey, "Size", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dComplexity;
	RegSetValueEx(skey, "Complexity", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dSpeed;
	RegSetValueEx(skey, "Speed", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dStretch;
	RegSetValueEx(skey, "Stretch", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dShowCurves;
	RegSetValueEx(skey, "ShowCurves", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
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
			ShellExecute(NULL, "open", "http://www.reallyslick.com", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	return FALSE;
}


void initControls(HWND hdlg){
	char cval[16];

	SendDlgItemMessage(hdlg, CYCLONES, UDM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(10), DWORD(1))));
	SendDlgItemMessage(hdlg, CYCLONES, UDM_SETPOS, 0, LPARAM(dCyclones));

	SendDlgItemMessage(hdlg, PARTICLES, UDM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(10000), DWORD(1))));
	SendDlgItemMessage(hdlg, PARTICLES, UDM_SETPOS, 0, LPARAM(dParticles));

	SendDlgItemMessage(hdlg, PARTICLESIZE, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(100))));
	SendDlgItemMessage(hdlg, PARTICLESIZE, TBM_SETPOS, 1, LPARAM(dSize));
	SendDlgItemMessage(hdlg, PARTICLESIZE, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, PARTICLESIZE, TBM_SETPAGESIZE, 0, LPARAM(5));
	sprintf(cval, "%d", dSize);
	SendDlgItemMessage(hdlg, SIZETEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, COMPLEXITY, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(10))));
	SendDlgItemMessage(hdlg, COMPLEXITY, TBM_SETPOS, 1, LPARAM(dComplexity));
	SendDlgItemMessage(hdlg, COMPLEXITY, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, COMPLEXITY, TBM_SETPAGESIZE, 0, LPARAM(2));
	sprintf(cval, "%d", dComplexity);
	SendDlgItemMessage(hdlg, COMPLEXITYTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, SPEED, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(100))));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETPOS, 1, LPARAM(dSpeed));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETPAGESIZE, 0, LPARAM(10));
	sprintf(cval, "%d", dSpeed);
	SendDlgItemMessage(hdlg, SPEEDTEXT, WM_SETTEXT, 0, LPARAM(cval));

	CheckDlgButton(hdlg, STRETCH, dStretch);

	CheckDlgButton(hdlg, SHOWCURVES, dShowCurves);

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
            dCyclones = SendDlgItemMessage(hdlg, CYCLONES, UDM_GETPOS, 0, 0);
			dParticles = SendDlgItemMessage(hdlg, PARTICLES, UDM_GETPOS, 0, 0);
			dSize = SendDlgItemMessage(hdlg, PARTICLESIZE, TBM_GETPOS, 0, 0);
			dComplexity = SendDlgItemMessage(hdlg, COMPLEXITY, TBM_GETPOS, 0, 0);
			dSpeed = SendDlgItemMessage(hdlg, SPEED, TBM_GETPOS, 0, 0);
			dStretch = (IsDlgButtonChecked(hdlg, STRETCH) == BST_CHECKED);
			dShowCurves = (IsDlgButtonChecked(hdlg, SHOWCURVES) == BST_CHECKED);
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
		if(HWND(lpm) == GetDlgItem(hdlg, PARTICLESIZE)){
			ival = SendDlgItemMessage(hdlg, PARTICLESIZE, TBM_GETPOS, 0, 0);
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
