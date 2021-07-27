/**
 *  Marching Cubes Demo, by glUser3f [gluser3f@gdnmail.net]
 *  Feel free to use this code in your own projects.
 *  If you do something cool with it, please email me so I can see it!
 *  
 *  Credits:
 *  Theory by Paul Bourke        [http://astronomy.swin.edu.au/~pbourke/modelling/polygonise/]
 *  OpenGL basecode by NeHe      [nehe.gamedev.net]
 *  Lookup Tables by Paul Bourke [http://astronomy.swin.edu.au/~pbourke/modelling/polygonise/]
 *  The rest is done by me, glUser3f ;)
 *
 */

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <cstdio>
#include <cmath>

#include "glfw.h"
#include "Grid.h"

float xrot = 30.0f;
float yrot = 30.0f;
float zoom = 2.2f;

bool gWire;
bool gWPressed;

bool gLight = true;
bool gLPressed;

bool gPaused;
bool gPPressed;

struct Cube {
	float size;
	float center[3];
};

Cube gCubes[3] = {
	{ 0.2f , { 0.0f, 0.0f, 0.0f } },
	{ 0.25f, { 0.0f, 0.0f, 0.0f } },
	{ 0.3f,  { 0.0f, 0.0f, 0.0f } }
};

float AmbLight[4] = { 0.3f, 0.3f, 0.5f, 0.3f };
float DifLight[4] = { 0.6f, 0.6f, 0.6f, 0.6f };
float LightPos[4] = { 0.0f, 0.0f, 12.0f, 0.0f };

#define POW4(X) ((X)*(X)*(X)*(X))

float cube(float x, float y, float z) {
	float val = 0.0f;
	for (int i = 0; i < 3; i++) {
		val += POW4(gCubes[i].size) / (POW4(x - gCubes[i].center[0]) + POW4(y - gCubes[i].center[1]) + POW4(z - gCubes[i].center[2]));
	}
	return val;
}

Grid gGrid(30, 1.0f, cube);

BOOL glfwInit(HINSTANCE hInstance, HWND hWnd) {
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, DifLight);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_LIGHTING);
	return TRUE;
}

GLvoid glfwRender() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -zoom);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	gGrid.draw();
	glFlush();
}

GLvoid glfwDeInit() {

}

GLvoid glfwUpdate(bool keys[256]) {
	gGrid.calculateGrids();
	gGrid.go();
	static int i = 0;

	gCubes[0].center[0] = sin(i/140.0f) * 0.2f;
	gCubes[0].center[1] = sin(i/120.0f) * 0.4f;
	gCubes[0].center[2] = sin(i/100.0f) * 0.3f;
	
	gCubes[1].center[0] = sin(i/40.0f)  * 0.7f;
	gCubes[1].center[1] = sin(i/80.0f)  * 0.5f;
	gCubes[1].center[2] = sin(i/120.0f) * 0.3f;

	gCubes[2].center[0] = sin(i/80.0f) * 0.6f;
	gCubes[2].center[1] = sin(i/60.0f) * 0.1f;
	gCubes[2].center[2] = sin(i/30.0f) * 0.5f;
	
	if (!gPaused) {
		i++;
	}

	xrot += 0.4f;
	yrot += 0.3f;

	if (keys[VK_ESCAPE]) {
		Terminate();
	}
	if (keys[VK_PRIOR]) {
		zoom -= 0.1f;
		if (zoom < 2.2f) {
			zoom = 2.2f;
		}
	}
	if (keys[VK_NEXT]) {
		zoom += 0.1f;
		if (zoom > 30.0f) {
			zoom = 30.0f;
		}
	}
	if (keys['W'] && !gWPressed) {
		gWire = !gWire;
		gWPressed = true;
		if (gWire) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	if (!keys['W']) {
		gWPressed = false;
	}
	if (keys['L'] && !gLPressed) {
		gLight = !gLight;
		if (gLight) {
			glEnable(GL_LIGHTING);
		} else {
			glDisable(GL_LIGHTING);
		}
		gLPressed = true;
	}
	if (!keys['L']) {
		gLPressed = false;
	}
	if (keys['P'] && !gPPressed) {
		gPaused = !gPaused;
		gPPressed = true;
	}
	if (!keys['P']) {
		gPPressed = false;
	}
}

GLvoid glfwActivate() {

}

GLvoid glfwDeActivate() {

}
