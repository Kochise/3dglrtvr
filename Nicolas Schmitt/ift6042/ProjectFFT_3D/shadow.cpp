#include "define.h"

void RenderGround ( void )
{
	DrawBox();

	glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
	
	glLoadIdentity();

	glTranslatef ( render.pos[0], render.pos[1], render.pos[2] );
	
	glTranslatef ( -(float)( calculs.size - 1 ) / 2,
				   -(float)( calculs.size - 1 ) / 2,
				   -(float)( calculs.size - 1 ) / 2 );

	if ( globals.bUseCg ) {
		CgEnableProgram();
		float modelview[16], viewmodel[16];
		glGetFloatv ( GL_MODELVIEW_MATRIX, modelview );
		float lightPos[3], eyePos[3];
		InverseMatrix ( modelview, viewmodel );
		MatxVect ( viewmodel, render.pos, eyePos );
		MatxVect ( viewmodel, render.light, lightPos );
		CgLoadUniformParameters ( lightPos, eyePos );
	}
	
	glColor3ub ( 219, 186, 24 );

	const int step = 5;
	int i, j;
	for ( i = -200; i < 200 ; i += step ) {
		glBegin ( GL_QUAD_STRIP );
			glVertex3f ( (float)i, 0.0f, -200.0f );
			for ( j = -200; j < 200; j += step ) {
				glVertex3f ( (float)i, 0.0f, (float)(j+step) );
				glVertex3f ( (float)(i+step), 0.0f, (float)j );
			}
			glVertex3f ( (float)(i+step), 0.0f, (float)j );
		glEnd();
	}

	if ( globals.bUseCg ) {
		CgDisableProgram();
	}

	glTranslatef ( render.light[0], render.light[1], render.light[2] );

	GLUquadricObj *q = gluNewQuadric();
	gluSphere ( q, 1.0, 50, 50 );
	gluDeleteQuadric ( q );

}

int CgInit ( void )
{
	cgError error;

	globals.Kd = 1.0f;
	globals.Ks = 1.0f;
	globals.Shininess = 50.0f;

	globals.cgcontext = cgCreateContext();

	error = cgAddProgramFromFile ( globals.cgcontext, "cgshadow.cg", cgVertexProfile, NULL );
	if ( error != cgNoError ) {
		return FALSE;
	}

	globals.cgprogiter = cgProgramByName ( globals.cgcontext, "main" );

	error = cgGLLoadProgram ( globals.cgprogiter, 1 );

	if ( error != cgNoError ) {
		return FALSE;
	}

	globals.ModelViewProjIter = cgGetBindByName ( globals.cgprogiter, "ModelViewProj" );

	// Light position
	globals.LightPosIter = cgGetBindByName ( globals.cgprogiter, "LightPos" );

	// Eye position
	globals.EyePosIter = cgGetBindByName ( globals.cgprogiter, "EyePos" );

	// Diffuse coefficient
	globals.KdIter = cgGetBindByName ( globals.cgprogiter, "Kd" );

	// Specular coefficient
	globals.KsIter = cgGetBindByName ( globals.cgprogiter, "Ks" );

	// Specular exponent
	globals.ShininessIter = cgGetBindByName ( globals.cgprogiter, "Shininess" );

	// Bind the program varying parameters (varying per vertex)

	// Position
	globals.PositionIter = cgGetBindByName ( globals.cgprogiter, "IN.Position" );

	// Normal
	globals.NormalIter = cgGetBindByName ( globals.cgprogiter, "IN.Normal" );

	// Enable client states
	cgGLEnableClientState ( globals.cgprogiter, globals.PositionIter );
	cgGLEnableClientState ( globals.cgprogiter, globals.NormalIter );

	return TRUE;
}

void CgEnableProgram ( void )
{
	cgGLBindProgram ( globals.cgprogiter );
	cgGLEnableProgramType ( cgVertexProfile );
}

void CgLoadUniformParameters ( const float lightPos[3], const float eyePos[3] )
{
	cgGLBindUniformStateMatrix ( globals.cgprogiter, globals.ModelViewProjIter, cgGLModelViewProjectionMatrix, cgGLMatrixIdentity );
	
	cgGLBindUniform4f ( globals.cgprogiter, globals.KdIter, globals.Kd, 0.0f, 0.0f, 0.0f );
	cgGLBindUniform4f ( globals.cgprogiter, globals.KsIter, globals.Ks, 0.0f, 0.0f, 0.0f );
	cgGLBindUniform4f ( globals.cgprogiter, globals.ShininessIter, globals.Shininess, 0.0f, 0.0f, 0.0f );

	cgGLBindUniform4f ( globals.cgprogiter, globals.LightPosIter, lightPos[0], lightPos[1], lightPos[2], 1.0 );
	cgGLBindUniform4f ( globals.cgprogiter, globals.EyePosIter, eyePos[0], eyePos[1], eyePos[2], 1.0 );
}

void CgLoadVaryingParameters ( const float mesh[3], const float normal[3] )
{
	// Position
	cgGLBindVaryingPointer ( globals.cgprogiter, globals.PositionIter, 3, GL_FLOAT, 0, (void *)mesh );
	// Normal
	cgGLBindVaryingPointer ( globals.cgprogiter, globals.NormalIter,   3, GL_FLOAT, 0, (void *)normal );
}

void CgDisableProgram ( void )
{
	cgGLDisableProgramType(cgVertexProfile);
}

void CgShutdown ( void )
{
	cgGLDisableClientState ( globals.cgprogiter, globals.PositionIter );
	cgGLDisableClientState ( globals.cgprogiter, globals.NormalIter );

	cgFreeBindIter ( globals.LightPosIter );
	cgFreeBindIter ( globals.EyePosIter );
	cgFreeBindIter ( globals.KdIter );
	cgFreeBindIter ( globals.KsIter );
	cgFreeBindIter ( globals.ShininessIter );
	cgFreeBindIter ( globals.PositionIter );
	cgFreeBindIter ( globals.NormalIter );
	cgFreeBindIter ( globals.ModelViewProjIter );

	cgFreeProgramIter ( globals.cgprogiter );

	cgFreeContext ( globals.cgcontext );

	cgCleanup();
}
