// TODO
//	otf, ctf für datensätze
//	shading mit gradient als multitextur wie bei bump mapping / normal map
//	otf und ctf als 1d opengl texture
//	alpha multiplikator proportional zum slice abstand


#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "Volume.h"
#include "VolumeRenderer.h"

using namespace std;


Volume *vol;
VolumeRenderer *ren;

int nslices = 24;

int frames = 0;
int t0 = 0, te;
double fps = 0;

bool rotateVolume = false;
bool maxIntProj = false;
int oldx, oldy;

int width = 512;
int height = 512;


void renderBitmapString(float x, float y, char *str)
{
	char *c;

	glRasterPos2f(x, y);

	for (c=str; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
}

void displayInfo()
{
	char str[80];
	int w, d, h;
	double sx, sy, sz;

	vol->getDimensions(w, h, d);
	vol->getScale(sx, sy, sz);

	// switch to orthographic projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	renderBitmapString(10, height-25, vol->getDescription());

	sprintf(str, "%d x %d x %d", w, h, d);
	renderBitmapString(10, height-40, str);

	sprintf(str, "%.2f : %.2f : %.2f", 1/sx, 1/sy, 1/sz);
	renderBitmapString(10, height-55, str);

	sprintf(str, "num slices: %d", nslices);
	renderBitmapString(10, 30, str);

	sprintf(str, "frames per second: %.2f", fps);
	renderBitmapString(10, 15, str);

	if (maxIntProj)
		sprintf(str, "mip");
	else
		sprintf(str, "direct");

	renderBitmapString(width-60, 15, str);

	// reset to prespective projection
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void display()
{
	if (maxIntProj)
	{
		// max intensity projection
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquationEXT(GL_MAX_EXT);
	}
	else
	{
		// x-ray
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);	// reset blend equation
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	displayInfo();

	glPopMatrix();

	glPushMatrix();

	ren->render(nslices);

	glPopMatrix();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	width = w;
	height = h;

	if (h == 0)
		h = 1;

	float ratio = (float)w/h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glViewport(0, 0, w, h);

	gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		exit(0);
	}
	else if (key == '-')
	{
		nslices--;

		if (nslices <= 0)
			nslices = 1;
	}
	else if (key == '+')
	{
		nslices++;
	}
	else if (key == 'r')
	{
		if (maxIntProj)
			maxIntProj = false;
		else
			maxIntProj = true;
	}
}

void update()
{
	// redraw
	glutPostRedisplay();

	frames++;

	te = glutGet(GLUT_ELAPSED_TIME);
	
	// every second approximately
	if (te-t0 >= 1000)
	{
		fps = 1000.0*frames/(te-t0);

		frames = 0;
		t0 = te;
	}		
}

void mouse(int button, int state, int x, int y)
{
	rotateVolume = false;

	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			rotateVolume = true;
			oldx = x;
			oldy = y;
			
		}

		if (button == GLUT_RIGHT_BUTTON)
		{
			
		}
	}
}

void motion(int x, int y)
{
	if (rotateVolume)
	{
		ren->rotate(oldy-y, oldx-x);
		oldx = x;
		oldy = y;
	}
}

void cleanup()
{
	delete vol;
	delete ren;
}

int main(int argc, char *argv[])
{
	cout << "Mass Carver :: A Volume Renderer based on 3D Texture Slicing" << endl;
	cout << "Version 1.0 by Torsten Stoeter, Jan 2009" << endl << endl;
	
	if (argc != 2)
	{
		cout << "Usage: ./carve <dataset.vol>" << endl << endl;
		exit(0);
	}

	atexit(cleanup);

	// init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	
	glutCreateWindow("Mass Carver");

	if (glewInit() != GLEW_OK)
	{
		cout << "couldn't initialize glew." << endl;
		exit(0);
	}

	if (!glewIsSupported("GL_EXT_blend_minmax GL_EXT_texture3D"))
	{
		cout << "opengl extensions gl_ext_texture3d or gl_ext_blend_minmax not supported." << endl;
		exit(0);
	}

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(update);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	try
	{
		cout << "loading dataset... " << flush;
		vol = new Volume(argv[1]);
		cout << "done." << endl;

		cout << "creating 3d texture... " << flush;
		ren = new VolumeRenderer(vol);
		cout << "done." << endl;
	}
	catch (int e)
	{
		switch (e)
		{
			case 10:
				cout << "couldn't load volume metafile " << argv[1] << endl;
				exit(10);
			break;

			case 20:
				cout << "couldn't load raw volume dataset" << endl;
				exit(20);
			break;
		}
	}

	glClearColor(0, 0, 0, 0);

	glutMainLoop();

	return 0;
}

