// **************************************************************************
// HCSG Demo
// (c) Bernie Freidin, 1999-2000
// **************************************************************************

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#ifdef __MAC__
#include <Windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <gl/glut.h>
#include "vector.h"
#include "hcsg.h"
#include "hcsg_modeler.h"
#include "camera.h"
#include "collision.h"

#define WAIT_FOR_CLOCK 1

static int     MAIN__STOP  = 0;
static hull_t *MAIN__WORLD = NULL;
static GLuint  MAIN__texid[12];
static int     MAIN__texcount = 3;
static int COLLISIONS = 1;

#ifndef __MAC__
#define BYTESWAP(x)
#else
static void BYTESWAP(short &x)
{
	x = ((x>>8)&0x00FF) | ((x<<8)&0xFF00);
}
#endif

static void MAIN__LoadTGA(char *fpath)
{
	FILE *tga = fopen(fpath, "rb");
	if(!tga)
	{
		fprintf(stderr, "\n\ncan't find required TGA (%s)\n", fpath);
		fprintf(stderr, "PRESS RETURN TO EXIT\n");
		char wait[200];
		gets(wait);
		exit(-1);
	}
	// ****************************
	//   2  bytes offset = N
	//   10 bytes of crap
	//   2  bytes = WIDTH
	//   2  bytes = HEIGHT
	//   2  bytes = BPP (24)
	//   N  bytes of crap..
	//   PIXEL DATA FOLLOWS (24bpp)
	// ****************************
	
	short offset, width, height;
	fread(&offset, sizeof(short), 1, tga); BYTESWAP(offset);
	fseek(tga, 10, SEEK_CUR);
	fread(&width,  sizeof(short), 1, tga); BYTESWAP(width);
	fread(&height, sizeof(short), 1, tga); BYTESWAP(height);
	fseek(tga, offset+2, SEEK_CUR);
	
	char *rgb = (char*)CSG_AllocMem(3, width*height);
	fread(rgb, 3, width*height, tga);
	fclose(tga);
	
	// Swap red and blue values
	
	for(int i = 0; i < width*height; i++)
	{
		char tmp = rgb[i*3];
		rgb[i*3] = rgb[i*3+2];
		rgb[i*3+2] = tmp;
	}
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8,
	                  width, height, GL_RGB,
	                  GL_UNSIGNED_BYTE, rgb);
	CSG_FreeMem(rgb);
}

static int POLYCOUNT = 0;

static vec3 MAIN__GetPolyNormal(poly_t *poly)
{
	surf_t *sp = CSG_GetSurfPool();
	vec3 n = sp[poly->surf_id & CSG_SURF_MASK].norm;
	hull_t *h2 = (hull_t*)poly->parent;
	if(!(h2->depth & 1)) n *= -1.0;
	if(poly->surf_id & CSG_SIDE_MASK) n *= -1.0;
	return n;
}

static void MAIN__RenderScene(hull_t *hull)
{
	vert_t *vp = CSG_GetVertPool();
	surf_t *sp = CSG_GetSurfPool();
	
	glColor3d(1.0, 1.0, 1.0);
	
	for(poly_t *p = hull->face; p; p = p->next)
	{
		GLdouble s_plane[] = {0.0, 0.0, 0.0, 0.0};
		GLdouble t_plane[] = {0.0, 0.0, 0.0, 0.0};
		
		vec3 n = MAIN__GetPolyNormal(p);
		
		glNormal3d(n.x, n.y, n.z);
		
		switch(sp[p->surf_id & CSG_SURF_MASK].mask)
		{
			case 1+2: s_plane[0] = t_plane[1] = 1.0; break;
			case 2+4: s_plane[1] = t_plane[2] = 1.0; break;
			case 4+1: s_plane[2] = t_plane[0] = 1.0; break;
			default: break;
		}
		glTexGendv(GL_S, GL_OBJECT_PLANE, s_plane);
		glTexGendv(GL_T, GL_OBJECT_PLANE, t_plane);
		
		for(poly_t *f = p->down; f; f = f->next)
		{
			glBindTexture(GL_TEXTURE_2D,
MAIN__texid[sp[p->surf_id & CSG_SURF_MASK].data % MAIN__texcount]);
			glBegin(GL_POLYGON);
			
			for(int n = 0; n < f->vertcount; n++)
			{
				glVertex3dv((GLdouble*)&vp[f->vref[n]]);
			}
			glEnd();
			
			if(POLYCOUNT > -1) POLYCOUNT++;
		}
	}
	for(hull_t *h = hull->down; h; h = h->next)
	{
		MAIN__RenderScene(h);
	}
}

static void MAIN__TrackFPS(void)
{
	static double sum = 0.f;
	static double num = 0.f;
	static int clk_count = 1;
	static clock_t clk1;
	static clock_t clk2;
	
	clk1 = clk2;
	clk2 = clock();
	sum += (double)(clk2-clk1);
	num += 1.f;
	
	if(MAIN__STOP)
	{
		double fps = (double)CLOCKS_PER_SEC * num / sum;
		char msg[200];
		//FIXME: why doesn't this work??
		sprintf(msg, "MEMORY IN USE= %i bytes", CSG_GetMemInUse(NULL));
		CSG_Messg(msg);
		sprintf(msg, "POLYGON COUNT= %i", -POLYCOUNT);
		CSG_Messg(msg);
		sprintf(msg, "AVERAGE FPS= %lf\n", fps);
		CSG_Messg(msg);
		exit(0);
	}
}

static void MAIN__InitSimpleTexture(int width, int height)
{
	long *buffer = (long*)calloc(sizeof(long), width * height);
	if(!buffer) exit(-1); // error
	
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			long pix = 0;
			
			pix += ((x ^ y) &  1) ? 0x00000088 : 0;
			pix += ((x ^ y) &  2) ? 0x00006600 : 0;
			pix += ((x ^ y) &  4) ? 0x00440000 : 0;
			pix += ((x ^ y) &  8) ? 0x00000044 : 0;
			pix += ((x ^ y) & 16) ? 0x00006600 : 0;
			pix += ((x ^ y) & 32) ? 0x00880000 : 0;
			
			buffer[x + y*width] = pix;
		}
	}
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    
	free(buffer);
}

static void GLUT__Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	MAIN__RenderScene(MAIN__WORLD);
	MAIN__TrackFPS();
	if(POLYCOUNT > -1) POLYCOUNT = -POLYCOUNT;
	
	glFlush();
    glutSwapBuffers();
}

static void GLUT__Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

static void execScript(char *script)
{
	fprintf(stdout, "Reading geometry script...\n");
	CSG_RunScript(script);
	fprintf(stdout, "Processing solid geometry");
	for(int i = 0;; i++)
	{
		if(_CSG_AddHull()) break;
		fprintf(stdout, ".");
		fflush(stdout);
	}
	fprintf(stdout, "\n");
	InitializeCamera("Scripts/cameraprefs.txt");
}

static void GLUT__Keyboard(unsigned char key, int x, int y)
{
	if(key == 27) MAIN__STOP = 1;
	if(0 && key == 'o')
	{
		char path[200], *badch;
		fprintf(stdout, "Enter script pathname: ");
		fgets(path, 199, stdin);
		if(badch = strchr(path, '\n')) badch[0] = '\0';
		CSG_RunScript(path);
	}
	if(key == ' ')
	{
		_CSG_AddHull();
	}
}

static void GLUT__Mouse(int button, int state, int x, int y)
{
}

static void GLUT__Idle(void)
{
#if WAIT_FOR_CLOCK
#ifdef WIN32
	Sleep(7); // milliseconds
#endif
#ifdef __MAC__
	long dval;
	Delay(1, &dval); // 60th's of a second
#endif
#endif
	UpdateCamera(2, COLLISIONS);
	glutPostRedisplay();
}

#ifndef __MAC__
static char texturenames[][200] = {
	"Textures/brick1.tga",
	"Textures/brick2.tga",
	"Textures/marble.tga"
};
#else
static char texturenames[][200] = {
	":Textures:brick1.tga",
	":Textures:brick2.tga",
	":Textures:marble.tga"
};
#endif

static void _mainMenuCallback(int item)
{
	if(item == 1) exit(0);
}

static void _collisionsCallback(int item)
{
	if(item == 1) COLLISIONS = 1;
	if(item == 2) COLLISIONS = 0;
}

static void _loadSceneCallback(int item)
{
	char script[200];

	switch(item)
	{
	case 1: strcpy(script, "Scripts/archtest.txt"); break;
	case 2: strcpy(script, "Scripts/spiraltest1.txt"); break;
	case 3: strcpy(script, "Scripts/spiraltest2.txt"); break;
	default: return;
	}
	for(hull_t *hnext, *h = MAIN__WORLD->down; h; h = hnext)
	{
		hnext = h->next;
		CSG_Free(h);
	}
	execScript(script);
}

static void InitializeMenus(void)
{
	int collisionsMenu = glutCreateMenu(_collisionsCallback);
	glutAddMenuEntry("On", 1);
	glutAddMenuEntry("Off", 2);
	int loadSceneMenu = glutCreateMenu(_loadSceneCallback);
	glutAddMenuEntry("archtest", 1);
	glutAddMenuEntry("spiraltest1", 2);
	glutAddMenuEntry("spiraltest2", 3);
	int mainMenu = glutCreateMenu(_mainMenuCallback);
	glutAddSubMenu("Collisions", collisionsMenu);
	glutAddSubMenu("Load Scene...", loadSceneMenu);
	glutAddMenuEntry("Exit", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void main(int argc, char *argv[])
{
	int i;
	
	// ***************
	// Initialize GLUT
	// ***************
	
	fprintf(stdout, "Flythrough (HSBM) Demo\n");
	fprintf(stdout, "(c) Bernie Freidin, 1999-2000\n\n");
	fprintf(stdout, "Keys:\n");
	fprintf(stdout, "  Numeric keypad to move (NumLock ON)\n");
	fprintf(stdout, "  Arrow keys UP/DOWN to hover\n");
	fprintf(stdout, "  A/Z to control pitch");
	fprintf(stdout, "  Right-click for menu (LOAD SCENE and COLLISIONS)\n");
	fprintf(stdout, "  ESCAPE to quit\n\n");
	fprintf(stdout, "Tips:\n");
	fprintf(stdout, "  Collisions are unstable; it might crash..\n");
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(40, 40);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(GLUT__Display);
	glutReshapeFunc(GLUT__Reshape);
	glutKeyboardFunc(GLUT__Keyboard);
	glutMouseFunc(GLUT__Mouse);
	glutIdleFunc(GLUT__Idle);
	
	const unsigned char *ext = glGetString(GL_EXTENSIONS);
	
	InitializeMenus();
	
	// ***********************
	// Initialize OpenGL state
	// ***********************
	
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	GLdouble plane1[] = {1.f, 1.f, 0.f, 0.f};
	GLdouble plane2[] = {0.f, 1.f, 1.f, 0.f};
	glTexGendv(GL_S, GL_OBJECT_PLANE, plane1);
	glTexGendv(GL_T, GL_OBJECT_PLANE, plane2);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
//	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	GLfloat fogcolor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glFogfv(GL_FOG_COLOR, fogcolor);
	glFogf(GL_FOG_DENSITY, 0.12f);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_START, 1.0f);
	glFogf(GL_FOG_END, 40.0f); // doesn't seem to affect?
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	
//	MAIN__InitSimpleTexture(64, 64);
	
	fprintf(stdout, "Loading textures...\n");
	
	glGenTextures(MAIN__texcount, MAIN__texid);
	
	for(i = 0; i < MAIN__texcount; i++)
	{
		glBindTexture(GL_TEXTURE_2D, MAIN__texid[i]);
#if 0 // good for software rendering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#else // good for hardware rendering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#endif
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		fprintf(stdout, "\t%s\n", texturenames[i]);
		MAIN__LoadTGA(texturenames[i]);
	}
	InitializeCamera("Scripts/cameraprefs.txt");
	{
		float ambient[]   = {0.5f, 0.5f, 0.5f, 1.0f};
		float diffuse[]   = {0.4f, 0.4f, 0.4f, 1.0f};
		float specular[]  = {1.0f, 1.0f, 1.0f, 1.0f};
		float shininess[] = {40.0f};
		float light_position[] = {10.0f, 10.0f, 10.0f, 1.0f};
		
		float color[] = {1.0f, 1.0f, 1.0f, 1.0f};
		
		glShadeModel(GL_SMOOTH);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,  emission);
		
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		glLightfv(GL_LIGHT0, GL_SHININESS, shininess);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_NORMALIZE);
	}
/*
	static void _currentObjectCallback(int value)
{
	if(value == 1)
	{
		GRS_MODE = 0;
		ZOOM_LIST[0].x = 0.0;
		ZOOM_LIST[0].y = 0.0;
		ZOOM_LIST[0].z = 0.95;
		ZOOM_COUNT = 1;
		ZOOM_STATE = 0;
		Z = ZOOM_LIST[0];
		glutPostRedisplay();
	}
}
*/
	fprintf(stdout, "Initializing HCSG engine...\n");
	CSG_Initialize();
	MAIN__WORLD = CSG_World();
	CSG_InitializeModeler(MAIN__WORLD);
	CDR_Initialize(MAIN__WORLD);

	execScript("Scripts/CSG_default_script.txt");
	
	for(i = 0; i < 20*0; i++)
	{
		double x1 = (double)(rand() & 255)*.015;
		double y1 = (double)(rand() & 255)*.015;
		double z1 = (double)(rand() & 255)*.015;
		double x2 = (double)(rand() & 255)*.0075 + x1 + .0125;
		double y2 = (double)(rand() & 255)*.0075 + y1 + .0125;
		double z2 = (double)(rand() & 255)*.0075 + z1 + .0125;
		
		hull_t *cube = CSG_SolidCube(x1, y1, z1, x2, y2, z2);
		CSG_BooleanOp(MAIN__WORLD, cube, 1, CSG_ADD, 1);
	}
	glutMainLoop();
}
