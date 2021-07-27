/*
 * Copyright (c) 2002-2004, xRhino, Inc.
 *
 * This library is free software; you can redistribute and/or modify
 * it under the terms of the dual license distributed with this source
 * package (a combined Trolltech QPL/GPL-compatible license)
 *
 * Under this dual license scheme, a separate license is available for use of 
 * this code with a proprietary (closed) system. For more information on the 
 * license, please see the LICENSE file distributed with this source package.
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <istream>
#include <ostream>
#include <iostream>
#include <GL/glut.h>
#include <glmath/glmath.h>
#include <glam/glam.h>
#include <GaeControlPoint.h>
#include <GaeAnimEditor.h>

using namespace std;

#define SELECT_BUFFER_SIZE 1024

// The configuration stuff
GmOptions* gOptions = GmOptions::getInstance();

// control points and widgets
vector<GaeControlPoint*> gCp;
vector<GmDrawable*> gWidgets;

// frame timing stuff
static GLint T0 = 0;
int framesDrawn;
GaeAnimationMode gAnimMode = AM_PREVIEW;

// view stuff
float gScaleFactor = 70.0f;
float gRotation = 0.0f;
float gTransX = -40.0f;
float gTransY = -10.0f;
float gARatio = 1.0f;
GmTypes::RenderOptionType renderOptions = GmTypes::RO_2D;

/* Global variables */
static int gRenderMode = GL_RENDER;

/* Picking selection buffer */
GLuint gSelectBuffer[SELECT_BUFFER_SIZE];

/* The last mouse event that occurred */
GmEvent::MouseEventType gLastMouseEvent;

void gae_print_usage()
{
   cout << "glamAnimEditor v0.1" << endl;
   cout << "usage: glamAnimEditor" << endl;
   cout << endl;
}

void gaeRenderGrid(GmTypes::RenderOptionType renderOptions)
{
   glColor3f(0.6, 0.6, 0.6);
   glBegin(GL_POINTS);
   for(float x = -500.0; x < 501.0; x += 10.0)
      for(float y = -500.0; y < 501.0; y += 10.0)
         glVertex2f(x, y);
   glEnd();

   // a cross to indicate the center of the world
   glColor4f(0.0, 1.0, 0.0, 0.7);
   glBegin(GL_LINES);
   glVertex2f(-500.0, 0.0);
   glVertex2f(500.0, 0.0);
   glVertex2f(0.0, -500.0);
   glVertex2f(0.0, 500.0);
   glEnd();
}

void gaeRenderWidgets(GmTypes::RenderOptionType renderOptions)
{
   for(vector<GmDrawable*>::iterator w = gWidgets.begin();
       w != gWidgets.end(); w++)
   {
      (*w)->render(renderOptions);
   }
}

void gaeInitWidgets()
{
   // Setup the OpenGL display
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glClearColor(0.0, 0.0, 0.0, 1.0);
   
   // By default, draw the control points
   SET_RENDER_OPTION(renderOptions, RO_CONTROL_POINTS);
   
   glEnable(GL_BLEND);
   glEnable(GL_LINE_SMOOTH);
   glEnable(GL_POLYGON_SMOOTH);
   glEnable(GL_TEXTURE_2D);
   
   glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
   glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void gaeRemoveWidgets(GaeAnimationMode am)
{
   switch(am)
   {
      case AM_PREVIEW:
         gaeRemovePreviewWidgets();
         break;
      default:
         cout << "Remove widgets failed, unknown animation mode." << endl;
         break;
   };
}

void gaePickingEnable()
{
   GLint viewport[4];

	glSelectBuffer(SELECT_BUFFER_SIZE, gSelectBuffer);
	glGetIntegerv(GL_VIEWPORT, viewport);
   gRenderMode = GL_SELECT;
	glRenderMode(GL_SELECT);
   CLEAR_RENDER_OPTION(renderOptions, GmTypes::RO_2D);
   SET_RENDER_OPTION(renderOptions, GmTypes::RO_2D_SELECT);

	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix(gLastMouseEvent.x, viewport[3]-gLastMouseEvent.y, 5, 5,
                 viewport);

	glMatrixMode(GL_PROJECTION);
   glOrtho(-gScaleFactor*gARatio, gScaleFactor*gARatio,
           -gScaleFactor, gScaleFactor,
           -1.0, 1.0);
}

void gaePickingProcess(GLint hits)
{
   GLuint* ptr;
   GLuint numberOfNames;
   GLuint validHits = 0;

   ptr = (GLuint *)gSelectBuffer;

   for(int i = 0; i < hits; i++)
   {
      numberOfNames = *ptr;
      
      for(unsigned int j = 0; j < numberOfNames; j++)
      {
         GmEventDistributor* ed = GmEventDistributor::getInstance();
         GmEvent* mouseEvent = new GmEvent();
         GmTypes::EventType eType;
         gLastMouseEvent.basicData.rxId = *(ptr + 3);
         
         switch(gLastMouseEvent.state)
         {
            case GLUT_UP:
               eType = GmTypes::E_MOUSE_RELEASE;
               break;
            case GLUT_DOWN:
               eType = GmTypes::E_MOUSE_PRESS;
               validHits++;
               break;
            default:
               eType = GmTypes::E_MOUSE_RELEASE;
         }
         mouseEvent->setMouseEvent(eType, gLastMouseEvent.basicData.rxId,
                                   gLastMouseEvent.button,
                                   gLastMouseEvent.state,
                                   gLastMouseEvent.x, gLastMouseEvent.y);
         
         ed->distributeEvent(mouseEvent);
         
         delete mouseEvent;
         ptr++;
      }
      
      // go to the next name
      ptr += 3;
	}

   if(validHits < 1)
      gLastMouseEvent.basicData.rxId = 0;
}

void gaePickingDisable()
{
   GLint hits;
   
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	hits = glRenderMode(GL_RENDER);
	if(hits != 0)
		gaePickingProcess(hits);
   CLEAR_RENDER_OPTION(renderOptions, GmTypes::RO_2D_SELECT);
   SET_RENDER_OPTION(renderOptions, GmTypes::RO_2D);
	gRenderMode = GL_RENDER;
}

static void glDisplay()
{
   glClear(GL_COLOR_BUFFER_BIT);
   
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-gScaleFactor*gARatio, gScaleFactor*gARatio,
           -gScaleFactor, gScaleFactor,
           -1.0, 1.0);
   
   if(gRenderMode == GL_SELECT)
      gaePickingEnable();

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   glTranslatef(gTransX, gTransY, 0.0);
   glTranslatef(-gTransX, -gTransY, 0.0);
   glRotatef(gRotation, 0.0, 0.0, -1.0);
   glTranslatef(gTransX, gTransY, 0.0);

   gaeRenderGrid(renderOptions);
   gaeRenderWidgets(renderOptions);
   
   switch(gAnimMode)
   {
      case AM_PREVIEW:
         gaeRenderPreview(renderOptions);
         break;
      default:
         cout << "Unknown anim mode, render failed." << endl;
   }
   
   glFlush();

   // Frame timing calculations
   framesDrawn++;
   {
      GLint t = glutGet(GLUT_ELAPSED_TIME);
      if(t - T0 >= 1000)
      {
         char fpsText[256];
         GLfloat seconds = (t - T0) / 1000.0;
         GLfloat fps = framesDrawn / seconds;
         sprintf(fpsText, "Frame Rate: %i FPS", int(fps));
         T0 = t;
         framesDrawn = 0;
      }
   }
   
	if(gRenderMode == GL_SELECT)
   {
		gaePickingDisable();
   }
	else
   {
      glFlush();
		glutSwapBuffers();
   }
}

static void glIdle()
{
   glutPostRedisplay();
}

static void glReshape(int width, int height)
{
   gARatio = float(width)/float(height);
   glViewport(0, 0, (GLsizei)width, (GLsizei)height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-gScaleFactor*gARatio, gScaleFactor*gARatio,
           -gScaleFactor, gScaleFactor,
           -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

static void glKey(unsigned char key, int x, int y)
{
   switch(key)
   {
      case 27:  /* Escape */
         exit(0);
         break;
      case '=':
         if(gScaleFactor < 1.0f)
            gScaleFactor = 1.0f;
         else
            gScaleFactor -= 1.0f;
         break;
      case '+':
         if(gScaleFactor < 1.0f)
            gScaleFactor = 1.0f;
         else
            gScaleFactor -= 10.0f;
         break;
      case '-':
         gScaleFactor += 1.0f;
         break;
      case '_':
         gScaleFactor += 10.0f;
         break;
      case 'a':
         gTransX += 1.0f;
         break;
      case 'd':
         gTransX -= 1.0f;
         break;
      case 'w':
         gTransY -= 1.0f;
         break;
      case 's':
         gTransY += 1.0f;
         break;
      case 'j':
         gRotation -= 1.0f;
         gRotation = (float)fmod(gRotation, 360.0f);
         break;
      case 'l':
         gRotation += 1.0f;
         gRotation = (float)fmod(gRotation, 360.0f);
         break;
      case 'c':
         TOGGLE_RENDER_OPTION(renderOptions, RO_CONTROL_POINTS);
         break;
      case '1':
         gaeRemoveWidgets(gAnimMode);
         gAnimMode = AM_PREVIEW;
         gaeInitPreviewWidgets();
         break;
      default:
         cout << "unknown key: " << key << endl;
   }

   glutPostRedisplay();
}

static void glMouseButton(int button, int state, int x, int y)
{
   gLastMouseEvent.button = button;
   gLastMouseEvent.state = state;
   gLastMouseEvent.x = x;
   gLastMouseEvent.y = y;
   
   gRenderMode = GL_SELECT;
}

static void glMouseMotion(int x, int y)
{
   GmEventDistributor* ed = GmEventDistributor::getInstance();
   GmEvent* mouseEvent = new GmEvent();
   GLint viewport[4];
   GLdouble mvmatrix[16], projmatrix[16];
   GLint realy;
   GLdouble wx, wy, wz;
   
   glGetIntegerv(GL_VIEWPORT, viewport);
   glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
   glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
   realy = viewport[3] - (GLint)y - 1;
   
   gluUnProject((GLdouble)x, (GLdouble)realy, 0.0,
                mvmatrix, projmatrix, viewport, &wx, &wy, &wz);

   gLastMouseEvent.x = wx;
   gLastMouseEvent.y = wy;
   
   mouseEvent->setMouseEvent(GmTypes::E_MOUSE_DRAG,
                             gLastMouseEvent.basicData.rxId,
                             gLastMouseEvent.button,
                             gLastMouseEvent.state,
                             gLastMouseEvent.x, gLastMouseEvent.y);
   
   ed->distributeEvent(mouseEvent);
   
   delete mouseEvent;
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
   glutInitWindowSize(800, 800);
   glutCreateWindow(argv[0]);

   //Initialize widgets
   gaeInitWidgets();
   gaeInitPreviewWidgets();

   if(argc > 1)
   {
      if(!strcmp("-h", argv[1]) || !strcmp("--help", argv[1]))
      {
         gae_print_usage();
         exit(0);
      }
   }
   
   glutReshapeFunc(glReshape);
   glutDisplayFunc(glDisplay);
   glutIdleFunc(glIdle);
   glutKeyboardFunc(glKey);
   glutMouseFunc(glMouseButton);
   glutMotionFunc(glMouseMotion);
   glutMainLoop();
   
   return 0;
}
