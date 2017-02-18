/* clipping3D.c */

/*
	Tutorial Exercise for COMP4201/COMP7203

	Purpose: Implement 3D clipping with Cohen-Sutherland and Liang-Barsky
*/

#include "Simulation.h"
#include "Lights.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifndef _WIN32
#include <stdlib.h>
#endif

#include <stdio.h>

#include "trackball.h"



#define KEY_ESC 27     /* escape    */
#define KEY_BS  8      /* backspace */
#define KEY_DEL 127    /* delete    */

#define MAX_LINES 100

#define X_MIN -1.0
#define X_MAX  1.0
#define Y_MIN -1.0
#define Y_MAX  1.0
#define Z_MIN -1.0
#define Z_MAX 1.0


/* Global variables */

float fSimulationSpeed = 1.0;
CSimulation Simulation;


int dragging = 0;
Point2fT mousePt;

Trackball trackball(640.0f,480.0f);


Matrix4fT transform = { 1.0f,  0.0f,  0.0f,  0.0f,	
                        0.0f,  1.0f,  0.0f,  0.0f,
                        0.0f,  0.0f,  1.0f,  0.0f,
                        0.0f,  0.0f,  0.0f,  1.0f };

Matrix3fT lastRot = { 1.0f,  0.0f,  0.0f,
                      0.0f,  1.0f,  0.0f,
                      0.0f,  0.0f,  1.0f };

Matrix3fT thisRot = { 1.0f,  0.0f,  0.0f,
                      0.0f,  1.0f,  0.0f,
                      0.0f,  0.0f,  1.0f };


/* end global variables*/



GLvoid keyboard( GLubyte key, GLint x, GLint y)
{
	switch (key)
	{
	case '1':
		Simulation.InitPart1();
		break;

	case '2':
		Simulation.InitPart2();
		break;

	case '3':
		Simulation.InitPart3();
		break;

	case '4':
		Simulation.InitPart4();
		break;

	case '5':
		Simulation.InitPart5();
		break;

	case '6':
		Simulation.InitPart6();
		break;


	case 'w':
	case 'W':
		fSimulationSpeed *= 1.1;
		break;

	case 's':
	case 'S':
		fSimulationSpeed *= 0.9;
		break;
	
	case 'a':
	case 'A':
		Simulation.Advance(-0.005);
		break;

	case 'd':
	case 'D':
		Simulation.Advance(0.005);
		break;


	case 'r':
	case 'R':
		Simulation.Reverse();
		break;

	case 'p':
	case 'P':
		if(fSimulationSpeed > 0) 
			fSimulationSpeed = 0.0;
		else
			fSimulationSpeed = 1.0;
		break;

	case 'q':
	case 'Q':	
		Simulation.PrintState();
		break;

	/* exit */
  	case KEY_ESC:   
		exit(0);
	    break;

	case KEY_DEL:
	case KEY_BS:
        break;

	/*help*/
	case 'H':
	case 'h':
		Simulation.PrintHelp();
		break;
	}
	glutPostRedisplay();
}


GLvoid checkError( const char* const label )
{
	GLenum error;

	error = glGetError();
	while ( GL_NO_ERROR != error )
	{
		fprintf( stderr,"%s: %s\n", label, gluErrorString(error) );
		error = glGetError();
	}
}

void drawBox ( void )
{
	glBegin(GL_QUADS);
		glVertex3f(X_MIN,Y_MIN,Z_MIN);		// back
		glVertex3f(X_MIN,Y_MAX,Z_MIN);
		glVertex3f(X_MAX,Y_MAX,Z_MIN);
		glVertex3f(X_MAX,Y_MIN,Z_MIN);

		glVertex3f(X_MIN,Y_MIN,Z_MAX);		// front
		glVertex3f(X_MAX,Y_MIN,Z_MAX);
		glVertex3f(X_MAX,Y_MAX,Z_MAX);
		glVertex3f(X_MIN,Y_MAX,Z_MAX);

    	glVertex3f(X_MAX,Y_MIN,Z_MIN);		// left
		glVertex3f(X_MAX,Y_MAX,Z_MIN);
		glVertex3f(X_MAX,Y_MAX,Z_MAX);
		glVertex3f(X_MAX,Y_MIN,Z_MAX);

	   	glVertex3f(X_MIN,Y_MIN,Z_MIN);		// right
		glVertex3f(X_MIN,Y_MIN,Z_MAX);
		glVertex3f(X_MIN,Y_MAX,Z_MAX);
		glVertex3f(X_MIN,Y_MAX,Z_MIN);

	   	glVertex3f(X_MAX,Y_MAX,Z_MAX);		// top
		glVertex3f(X_MAX,Y_MAX,Z_MIN);
		glVertex3f(X_MIN,Y_MAX,Z_MIN);
		glVertex3f(X_MIN,Y_MAX,Z_MAX);

   	    glVertex3f(X_MIN,Y_MIN,Z_MAX);		// bottom
		glVertex3f(X_MIN,Y_MIN,Z_MIN);
		glVertex3f(X_MAX,Y_MIN,Z_MIN);
		glVertex3f(X_MAX,Y_MIN,Z_MAX);
	glEnd();
}

void drawClipBox( void )
{
    glColor4f(1.0,1.0,1.0,1.0);

	glLineWidth(2.0);   
    glPolygonMode(GL_FRONT,GL_LINE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	drawBox();

	glLineWidth(1.0);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1,0x000f);
	glPolygonMode(GL_BACK,GL_LINE);
	glCullFace(GL_FRONT);
	drawBox();

	glCullFace(GL_BACK);
	glDisable(GL_LINE_STIPPLE);
	glPolygonMode(GL_FRONT,GL_FILL);
    glPolygonMode(GL_BACK ,GL_FILL);

}


GLvoid display( GLvoid )
{	
	glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();   // Reset the ModelView transformation 

	glTranslatef(0.0f,0.0f,-4.0f*Z_MAX);	

	glPushMatrix();													
    glMultMatrixf(transform.M);	

	CScopeDepthTest depthTest;
	Simulation.UpdateAndRender(fSimulationSpeed); 
	drawClipBox();

    glPopMatrix();
   
#ifdef _DEBUG
	checkError( "display" );
#endif

	glutSwapBuffers();
}

GLvoid mouse(GLint button, GLint state, GLint x, GLint y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			if  (state == GLUT_DOWN)
			{
				dragging = 1;						
				mousePt.s.X = (GLfloat) x;
				mousePt.s.Y = (GLfloat) y;				

				lastRot = thisRot;
				trackball.click(&mousePt);		
            }
			else if (state == GLUT_UP)
			{
				dragging = 0;
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			break;

		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_UP)
			{
				Matrix3fSetIdentity(&lastRot);
				Matrix3fSetIdentity(&thisRot);
				Matrix4fSetRotationFromMatrix3f(&transform, &thisRot);
				glutPostRedisplay();
			}
			break;
	}
}

GLvoid motion(GLint x, GLint y)
{
	if (dragging)
	{
	    mousePt.s.X = (GLfloat) x;
		mousePt.s.Y = (GLfloat) y;

		Quat4fT thisQuat;

        trackball.drag(&mousePt, &thisQuat);						// Update End Vector And Get Rotation As Quaternion
        Matrix3fSetRotationFromQuat4f(&thisRot, &thisQuat);		    // Convert Quaternion Into Matrix3fT
        Matrix3fMulMatrix3f(&thisRot, &lastRot);				    // Accumulate Last Rotation Into This One
        Matrix4fSetRotationFromMatrix3f(&transform, &thisRot);	    // Set Our Final Transform's Rotation From This One

		glutPostRedisplay();
	}
}

GLvoid idle()
{
	glutPostRedisplay();
}

void init(int width, int height)
{
	glClearColor( 0.0, 0.0, 0.0, 0.0 );  

	glViewport(0,0,width,height);

    glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 0.1f, 20.0f);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	trackball.setBounds(width,height);
}


int main(int argc, char* argv[])
{
	GLint width;
    GLint height;

	srand(GetTickCount());
    
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
	width = glutGet( GLUT_SCREEN_WIDTH );
	height = glutGet( GLUT_SCREEN_HEIGHT );
	glutInitWindowPosition( width/4, height/4);
	glutInitWindowSize( width/2, height/2 );
	glutCreateWindow( argv[0] );

	glutDisplayFunc( display );
	glutReshapeFunc( init );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouse );
    glutMotionFunc( motion );
	glutIdleFunc( idle ); 


	Simulation.InitPart1();
	Simulation.Start();
	init(width/2,height/2);

	Simulation.PrintHelp();

	glutMainLoop();

	return 0;
}
