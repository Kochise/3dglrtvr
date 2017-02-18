#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class CScopeDepthTest
{
public:
	 CScopeDepthTest() { glEnable(GL_DEPTH_TEST); }
	~CScopeDepthTest() { glDisable(GL_DEPTH_TEST);}
};

class CScopeLighting
{
public:
	 CScopeLighting() { glEnable(GL_LIGHTING); glShadeModel(GL_SMOOTH);}
	~CScopeLighting() { glDisable(GL_LIGHTING);}
};

class CScopeLight0
{
public:
	CScopeLight0()
	{
		glEnable(GL_LIGHT0);

		// Lights properties
		static GLfloat LightAmbient[]  = {0.7f, 0.7f, 0.7f, 1.0f};
		static GLfloat LightDiffuse[]  = {0.8f, 0.8f, 0.8f, 1.0f};
		static GLfloat LightSpecular[] = {0.2f, 0.2f, 0.2f, 1.0f};
		//static GLfloat LightPosition[] = {1.0f, -1.0f, 1.0f, 0.0f};

		glLightfv( GL_LIGHT0, GL_AMBIENT,  LightAmbient);
		glLightfv( GL_LIGHT0, GL_DIFFUSE,  LightDiffuse);
		glLightfv( GL_LIGHT0, GL_SPECULAR, LightSpecular);
		//glLightfv( GL_LIGHT0, GL_POSITION, LightPosition);
	}
	~CScopeLight0()
	{
		glDisable(GL_LIGHT0);
	}
};

class CScopeBlueMaterial
{
public:
	CScopeBlueMaterial()
	{
		static GLfloat MaterialDiffuse[]   = {0.227,0.431,0.647,1.0};
		static GLfloat MaterialSpecular[]  = {1,1,1,1.0};//{0.227,0.431,0.647,1.0};
		static GLfloat MaterialShininess[] = {50};

		glMaterialfv(GL_FRONT, GL_DIFFUSE  , MaterialDiffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR , MaterialSpecular);
		glMaterialfv(GL_FRONT, GL_SHININESS, MaterialShininess);
	}
	~CScopeBlueMaterial()
	{

	}
};


