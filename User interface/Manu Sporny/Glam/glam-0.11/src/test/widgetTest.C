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
#include <iostream>
#include <GL/glut.h>
#include <glam/glam.h>
#include <WidgetTestUI.h>

using namespace std;

// OpenGL picking items
#define SELECT_BUFFER_SIZE 1024

enum GlutEventType
{
   GLUT_EVENT_MOUSE_BUTTON,
   GLUT_EVENT_MOUSE_MOVE,
   GLUT_EVENT_MOUSE_DRAG
};

GlutEventType gGlutEventType;
GLuint gDragName;
GLuint gPreviousName;
GmEvent::MouseEventType gLastMouseEvent;

// frame timing stuff
static GLint T0 = 0;
int framesDrawn;

// view stuff
float gScaleFactor = 100.0f;
float gRotation = 0.0f;
float gTransX = 0.0f;
float gTransY = 0.0f;
float gARatio = 1.0f;

/* Global variables */
static int gRenderMode = GL_RENDER;
static bool gPickMode = true;

/* Picking selection buffer */
GLuint gSelectBuffer[SELECT_BUFFER_SIZE];

/* UI Class */
WidgetTestUI gUI;

/* Event distribution */

void distributeMouseEvent(GLuint name, GmTypes::EventType eType)
{
   GmEventDistributor* ed = GmEventDistributor::getInstance();

   GmEvent* mouseEvent = new GmEvent();
   mouseEvent->setMouseEvent(eType, name,
                             gLastMouseEvent.button, gLastMouseEvent.state,
                             gLastMouseEvent.x, gLastMouseEvent.y);
   ed->distributeEvent(mouseEvent);
   delete mouseEvent;
}

void distributeMouseButtonEvent(GLuint name)
{
   GmTypes::EventType eType;

   switch(gLastMouseEvent.state)
   {
      case GLUT_UP:
         eType = GmTypes::E_MOUSE_RELEASE;
         break;
      case GLUT_DOWN:
         eType = GmTypes::E_MOUSE_PRESS;
         break;
   }

   distributeMouseEvent(name, eType);
}

void distributeMouseMoveEvent(GLuint name)
{
   if(gPreviousName != name)
   {
      // mouse entered new name
      distributeMouseEvent(name, GmTypes::E_MOUSE_ENTERED);

      // mouse exited prev name
      distributeMouseEvent(gPreviousName, GmTypes::E_MOUSE_EXITED);
   }
   else
   {
      // mouse moved within name
      distributeMouseEvent(name, GmTypes::E_MOUSE_MOVEMENT);
   }
}

void distributeMouseDragEvent(GLuint name)
{
   // mouse move event must be passed to cover mouse enter and exit
   distributeMouseMoveEvent(name);

   // pass mouse drag event
   distributeMouseEvent(gDragName, GmTypes::E_MOUSE_DRAG);
}

/* End of event distribution */

/* The picking process implementation */

/**
 *  In order to select components in the UI, OpenGL's picking paradigm
 * is employed. When mouse input is received by glut, picking mode
 * becomes active and the pixel location of the mouse is used as the
 * picking location.
 *
 * The display is then rendered so that the components are rendered
 * and thus made available for picking. After rendering is complete,
 * picking mode is deactived, and the component that was drawn on the
 * top level of the picking area is packaged with a mouse event and
 * sent to the UI's mouse event handler.
 */

void getClosestPick(GLint hits)
{
   if(!gPickMode)
      return;
   
   GLuint currentName = 0;

   // point at the selection buffer
   GLuint* ptr = (GLuint*)gSelectBuffer;
   
   // initialize minZ to maximum depth
   GLuint minZ = 0xffffffff;

   for(int i = 0; i < hits; i++)
   {
      // get the number of names
      GLuint numNames = *ptr;

      // move to the minimum z value
      ptr++;

      // next is minimum z value of the hit
      if(*ptr < minZ)
      {
         // update minZ
         minZ = *ptr;

         // get the current name
         currentName = (numNames != 0) ? *(ptr + 2) : 0;
      }

      ptr += numNames + 2;
   }

   switch(gGlutEventType)
   {
      case GLUT_EVENT_MOUSE_BUTTON:
         distributeMouseButtonEvent(currentName);
         break;
         
      case GLUT_EVENT_MOUSE_MOVE:
         distributeMouseMoveEvent(currentName);
         break;
         
      case GLUT_EVENT_MOUSE_DRAG:
         distributeMouseDragEvent(currentName);
         break;
   }

   // update the name
   gPreviousName = currentName;

   // update the drag name
   if(gGlutEventType != GLUT_EVENT_MOUSE_DRAG &&
       gLastMouseEvent.state == GLUT_UP)
      gDragName = currentName;
}

void enablePicking()
{
   GLint viewport[4];

   // prepare the selection buffer
	glSelectBuffer(SELECT_BUFFER_SIZE, gSelectBuffer);

   // enter selection render mode
	glRenderMode(GL_SELECT);
   gUI.setRenderOptions(GmTypes::RO_2D_SELECT);

   // prepare the name stack
	glInitNames();

   // set up perspective
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glGetIntegerv(GL_VIEWPORT, viewport);

   // Picking sensitivity is set to 1 pixel, all objects at x,y are selected.
   gluPickMatrix(gLastMouseEvent.x, viewport[3] - gLastMouseEvent.y, 1, 1,
                 viewport);

	glMatrixMode(GL_PROJECTION);
   glOrtho(-gScaleFactor*gARatio, gScaleFactor*gARatio,
           -gScaleFactor, gScaleFactor,
           0.0, 1.0);
}

void disablePicking()
{
   // restore original projection
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();

   // return to normal rendering mode and get # of hits
   GLint hits = glRenderMode(GL_RENDER);
   gUI.setRenderOptions(GmTypes::RO_2D);
   gRenderMode = GL_RENDER;

   // get the closest picks
   getClosestPick(hits);
}

/* End of the picking process implementation */

static void renderDraggedObjects()
{
   GmEventDistributor* ed = GmEventDistributor::getInstance();
   GmIcon* icon = ed->getDraggedIcon();

   if(icon != NULL)
   {         
      glPushMatrix();
      glTranslatef(gLastMouseEvent.GLx, gLastMouseEvent.GLy, 0.0);
      icon->setRelativeGeometry(1.0, 1.0, 1.0, 1.0);
      icon->layoutInGLArea(-20.0, -20.0, 20.0, 20.0, -1.0);
      icon->render(GmTypes::RO_2D);
      glPopMatrix();
   }
}

static void glDisplay()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-gScaleFactor * gARatio, gScaleFactor * gARatio,
           -gScaleFactor, gScaleFactor,
           0.0, 1.0);

   if(gRenderMode == GL_SELECT)
      enablePicking();

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   renderDraggedObjects();
   
   glRotatef(gRotation, 0.0, 0.0, -1.0);
   glTranslatef(gTransX, gTransY, 0.0);

   // render the UI
   gUI.renderUI();
   // render any objects that are being dragged currently

   // a cross to indicate the center of the world
   glPushAttrib(GL_CURRENT_BIT);
   
   glColor4f(0.0, 1.0, 0.0, 0.7);
   glLineWidth(1.0);
   glBegin(GL_LINES);
   glVertex2f(-2.0, 0.0);
   glVertex2f(2.0, 0.0);
   glVertex2f(0.0, 3.0);
   glVertex2f(0.0, -3.0);
   glVertex2f(0.0, 3.0);
   glVertex2f(-1.0, 2.5);
   glVertex2f(0.0, 3.0);
   glVertex2f(1.0, 2.5);
   glEnd();

   glPopAttrib();

   // flush to screen
   glFlush();

   // Frame timing calculations
   framesDrawn++;
   {
      GLint t = glutGet(GLUT_ELAPSED_TIME);
      if(t - T0 >= 200)
      {
         char fpsText[256];
         GLfloat seconds = (t - T0) / 1000.0;
         GLfloat fps = framesDrawn / seconds;
         sprintf(fpsText, "Frame Rate: %i FPS", int(fps));

         // make the bar meter show this stuff here
         gUI.updateBarMeter(fps);
         gUI.updateLabel(fpsText);

         T0 = t;
         framesDrawn = 0;         
      }
   }
   
   if(gRenderMode == GL_SELECT)
      disablePicking();
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
           0.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

static void glKey(unsigned char key, int x, int y)
{
   //override any other handling by quitting when ESC is pushed
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
      case '-':
         gScaleFactor += 1.0f;
         break;
      case 'f':
      {
         cout << endl << "FOCUS LIST" << endl;

         GmEventDistributor* ed = GmEventDistributor::getInstance();
         vector<GmInteractiveWidget*>* widgets = ed->getFocusList();

         vector<GmInteractiveWidget*>::iterator i;
         for(i = widgets->begin(); i != widgets->end(); i++)
            cout << (*i)->getObjectId() << endl;

         break;
      }
      case 'j':
         gRotation -= 1.0f;
         gRotation = (float)fmod(gRotation, 360.0f);
         break;
      case 'l':
         gRotation += 1.0f;
         gRotation = (float)fmod(gRotation, 360.0f);
         break;
      case '1':
         gUI.setRenderOptions(GmTypes::RO_2D);
         break;
      case '2':
         gUI.setRenderOptions(GmTypes::RO_2D_SELECT);
         break;
      case '3':
         gPickMode = !gPickMode;
         break;
      // leave these 2 alone; glam handles them for now
      case ' ':
      case '\t':
         break;
      default:
         cout << "unknown key: " << key << endl;
   }

   // create a keyboard event and pass it to the widget in focus
   GmEventDistributor* ed = GmEventDistributor::getInstance();
   GmInteractiveWidget* focus = ed->getFocus();

   if(focus != NULL)
   {
      //cout << "focus before: " << focus->getObjectId() << endl;

      GmEvent* keyEvent = new GmEvent();
      keyEvent->setKeyEvent(GmTypes::E_KEY_PRESS,
                            focus->getObjectId(), key, x, y);
      ed->distributeEvent(keyEvent);
      delete keyEvent;

      //cout << "focus after: " << ed->getFocus()->getObjectId() << endl;
   }
   
   glutPostRedisplay();
}

static void glSpecialKey(int key, int x, int y)
{
   switch(key)
   {
      case GLUT_KEY_LEFT:
         gTransX -= 1.0f;
         break;
      case GLUT_KEY_RIGHT:
         gTransX += 1.0f;
         break;
      case GLUT_KEY_UP:
         gTransY += 1.0f;
         break;
      case GLUT_KEY_DOWN:
         gTransY -= 1.0f;
         break;
   }
}

static void onMouseButton(int button, int state, int x, int y)
{
   gLastMouseEvent.button = button;
   gLastMouseEvent.state = state;
   gLastMouseEvent.x = x;
   gLastMouseEvent.y = y;
   gGlutEventType = GLUT_EVENT_MOUSE_BUTTON;

   gRenderMode = GL_SELECT;

   glDisplay();
}

static void onMouseMove(int x, int y)
{
   gLastMouseEvent.x = x;
   gLastMouseEvent.y = y;   
   gGlutEventType = GLUT_EVENT_MOUSE_MOVE;

   gRenderMode = GL_SELECT;

   glDisplay();
}

static void onMouseDrag(int x, int y)
{
   if(!gPickMode)
   {
      if(gLastMouseEvent.button == GLUT_LEFT_BUTTON)
      {
         gTransX += (float)(x - gLastMouseEvent.x) / (100.0 / gScaleFactor);
         gTransY -= (float)(y - gLastMouseEvent.y) / (100.0 / gScaleFactor);
      }
      else
         gScaleFactor += (y - gLastMouseEvent.y);
   }

   gLastMouseEvent.x = x;
   gLastMouseEvent.y = y;
   gGlutEventType = GLUT_EVENT_MOUSE_DRAG;
  
   gRenderMode = GL_SELECT;

   glDisplay();
}

void initDisplay()
{
   // Setup the OpenGL display
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glClearColor(0.0, 0.0, 0.0, 1.0);

   glEnable(GL_TEXTURE_2D);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

   // initialize the UI (creates and sets up the widgets)
   gUI.initUI();
}

int main(int argc, char** argv)
{
   GmOptions* options = GmOptions::getInstance();
   
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH);
   glutInitWindowSize(400, 400);
   glutCreateWindow(argv[0]);

   // Load all the options from the configuration file
   if (!options->loadConfigurationFile(GLAM_DATA_DIR PATH_SEPARATOR \
                                       "uiWidgetTest-ui.cfg"))
   {
      cout << "Config file not loaded, exiting...\n";
      exit(0);
   }

   // Initialize the display
   initDisplay();
   
   if(argc > 1)
   {
      if(!strcmp("-h", argv[1]) || !strcmp("--help", argv[1]))
      {
         gUI.print_usage();
         exit(0);
      }
   }

   glutReshapeFunc(glReshape);
   glutDisplayFunc(glDisplay);
   glutIdleFunc(glIdle);
   glutKeyboardFunc(glKey);
   glutSpecialFunc(glSpecialKey);
   glutMouseFunc(&onMouseButton);
   glutMotionFunc(&onMouseDrag);
   glutPassiveMotionFunc(&onMouseMove);
   glutMainLoop();
   
   return 0;
}
