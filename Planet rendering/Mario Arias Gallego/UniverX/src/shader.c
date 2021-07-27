/* Práctica Final Informática Gráfica 2006
   Simulador Sistema Solar
   
   Mario Arias Gallego <mariari@lab.fi.uva.es>
   Álvaro Zubizarreta López <alvazub@lab.fi.uva.es>
*/


#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include "supergl.h"

#include <stdlib.h>
#include <stdio.h>

#include "shader.h"
#include "texturas.h"

// Variables
int cg_enable = 1;

CGcontext	cgContext;

struct vshader vshaders[NUM_VSHADERS];
struct pshader pshaders[NUM_PSHADERS];

int InicializaShaders()
{
	int i;

	for(i=0;i<NUM_VSHADERS;i++) {
		vshaders[i].activo=0;
	}

	for(i=0;i<NUM_PSHADERS;i++) {
		vshaders[i].activo=0;
	}

	printf("---- Cargando Shaders ----\n");

	// Create A New Context For Our Cg Program(s)
	cgContext = cgCreateContext();

	// Validate Our Context Generation Was Successful
	if (cgContext == 0)
	{
		printf("Failed To Create Cg Context\n");
		cg_enable = 0;
		return(1);
	}

#if 1
	// Cargar Vertex shader para las estrellas
	if( loadVShader( VSHADER_STARS , VSHADER_STARS_NAME , VSHADER_STARS_FUNNAME ) ) {
		printf("Error cargando shader %s\n", VSHADER_STARS_NAME);
		return(1);
	} 

	loadStarVShaderVars();
#endif

	// Cargar Vertex shader para Normal bumpmaps
#if 1
	if( loadVShader( VSHADER_NORMAL , VSHADER_NORMAL_NAME  , VSHADER_NORMAL_FUNNAME ) ) {
		printf("Error cargando shader %s\n", VSHADER_NORMAL_NAME);
		return(1);
	}
	loadNormalVShaderVars();
#endif

	// Cargar Pixel shader para Normal bumpmaps
#if 1
	if( loadPShader( PSHADER_NORMAL, PSHADER_NORMAL_NAME  , PSHADER_NORMAL_FUNNAME ) ) {
		printf("Error cargando shader %s\n", PSHADER_NORMAL_NAME);
		return(1);
	}
	loadNormalPShaderVars();
#endif

	return 0;
}

void ParaShaders(void)
{
	// Destroy Our Cg Context And All Programs Contained Within It
	cgDestroyContext(cgContext);
}



// VERTEX SHADER

int loadVShader(int num, char *vshaderfile, char *functname)
{
	printf("Cargando Vertex Shader %s\n", vshaderfile);

	// Get The Latest GL Vertex Profile
	vshaders[num].cgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);

	// Validate Our Profile Determination Was Successful
	if ( vshaders[num].cgVertexProfile == CG_PROFILE_UNKNOWN)
	{
		printf("Invalid profile type\n");
		return(1);
	}

	cgGLSetOptimalOptions( vshaders[num].cgVertexProfile );

	// Load the shader 
	vshaders[num].cgProgram = cgCreateProgramFromFile(cgContext, CG_SOURCE, vshaderfile, vshaders[num].cgVertexProfile , functname, 0);

	// Validate Success
	if (vshaders[num].cgProgram == 0)
	{
		// We Need To Determine What Went Wrong
		CGerror Error = cgGetError();

		// Show A Message Box Explaining What Went Wrong
		printf("Error loading vertex program %s \n",cgGetErrorString(Error));
		return(1);			// We Cannot Continue
	}

	// Load The Program
	cgGLLoadProgram( vshaders[num].cgProgram );

	vshaders[num].activo = 1;
	return 0;
}


/* Vertex Shader para Estrellas */

void loadStarVShaderVars() {
	vshaders[VSHADER_STARS].cgParameter[CG_PARAM_POSITION] = cgGetNamedParameter( vshaders[VSHADER_STARS].cgProgram, "IN.position");
	vshaders[VSHADER_STARS].cgParameter[CG_PARAM_COLOR] = cgGetNamedParameter( vshaders[VSHADER_STARS].cgProgram, "IN.color");
	vshaders[VSHADER_STARS].cgParameter[CG_PARAM_MODELVIEWPROJ] = cgGetNamedParameter( vshaders[VSHADER_STARS].cgProgram, "ModelViewProj");
	vshaders[VSHADER_STARS].cgParameter[CG_PARAM_WAVE] = cgGetNamedParameter( vshaders[VSHADER_STARS].cgProgram, "IN.wave");
}

void ActivateStarVShader()
{
	if (cg_enable && vshaders[VSHADER_STARS].activo )
	{
		// Set The Modelview Matrix Of Our Shader To Our OpenGL Modelview Matrix
		cgGLSetStateMatrixParameter( vshaders[VSHADER_STARS].cgParameter[CG_PARAM_MODELVIEWPROJ], 
			CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

		// Enable Our Vertex Shader Profile
		cgGLEnableProfile( vshaders[VSHADER_STARS].cgVertexProfile );

		// Bind Our Vertex Program To The Current State
		cgGLBindProgram( vshaders[VSHADER_STARS].cgProgram );
	}

}

void DeactivateStarVShader() 
{
	if (cg_enable && vshaders[VSHADER_STARS].activo )
		cgGLDisableProfile( vshaders[VSHADER_STARS].cgVertexProfile );		// Disable Our Vertex Profile
}

void StarVShaderSetColor( float a, float b, float c, float d) 
{
	// Set The Drawing Color To Light Green (Can Be Changed By Shader, Etc...)
	if (cg_enable && vshaders[VSHADER_STARS].activo)
		cgGLSetParameter4f(vshaders[VSHADER_STARS].cgParameter[CG_PARAM_COLOR], a, b, c, d);
}

void StarVShaderSetWave( float param)
{
	// Set The Wave Parameter Of Our Shader To The Incremented Wave Value From Our Main Program
	if (cg_enable && vshaders[VSHADER_STARS].activo)
		cgGLSetParameter3f( vshaders[VSHADER_STARS].cgParameter[CG_PARAM_WAVE] , param, 1.0f, 1.0f);
}



/* Vertex Shader para Normal Bumpmap */

void loadNormalVShaderVars() 
{
	vshaders[VSHADER_NORMAL].cgParameter[NVPAR_POSITION] = cgGetNamedParameter( vshaders[VSHADER_NORMAL].cgProgram, "IN.position");
	vshaders[VSHADER_NORMAL].cgParameter[NVPAR_COLOR] = cgGetNamedParameter( vshaders[VSHADER_NORMAL].cgProgram, "IN.color");
	vshaders[VSHADER_NORMAL].cgParameter[NVPAR_MODELVIEWPROJ] = cgGetNamedParameter( vshaders[VSHADER_NORMAL].cgProgram, "ModelViewProj");
	//vshaders[VSHADER_NORMAL].cgParameter[NVPAR_MODELVIEW] = cgGetNamedParameter( vshaders[VSHADER_NORMAL].cgProgram, "ModelView");
	//vshaders[VSHADER_NORMAL].cgParameter[NVPAR_TEXCOORDS] = cgGetNamedParameter( vshaders[VSHADER_NORMAL].cgProgram, "texCoords");
	vshaders[VSHADER_NORMAL].cgParameter[NVPAR_VTANGENT] = cgGetNamedParameter( vshaders[VSHADER_NORMAL].cgProgram, "IN.vTangent");
	vshaders[VSHADER_NORMAL].cgParameter[NVPAR_VBINORMAL] = cgGetNamedParameter( vshaders[VSHADER_NORMAL].cgProgram, "IN.vBinormal");
	vshaders[VSHADER_NORMAL].cgParameter[NVPAR_VNORMAL] = cgGetNamedParameter( vshaders[VSHADER_NORMAL].cgProgram, "IN.vNormal");
	vshaders[VSHADER_NORMAL].cgParameter[NVPAR_LIGHTPOS] = cgGetNamedParameter( vshaders[VSHADER_NORMAL].cgProgram, "vLightPosition");
}

void ActivateNormalVShader()
{

	if (cg_enable && vshaders[VSHADER_NORMAL].activo)
	{
		// Set The Modelview Matrix Of Our Shader To Our OpenGL Modelview Matrix
		cgGLSetStateMatrixParameter( vshaders[VSHADER_NORMAL].cgParameter[NVPAR_MODELVIEWPROJ], 
			CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

		// Enable Our Vertex Shader Profile
		cgGLEnableProfile( vshaders[VSHADER_NORMAL].cgVertexProfile );

		// Bind Our Vertex Program To The Current State
		cgGLBindProgram( vshaders[VSHADER_NORMAL].cgProgram );
	}
}

void DeactivateNormalVShader() 
{
	if (cg_enable && vshaders[VSHADER_NORMAL].activo)
		cgGLDisableProfile( vshaders[VSHADER_NORMAL].cgVertexProfile );		// Disable Our Vertex Profile
}

void NormalVShaderSetBinormal( float a, float b, float c, float d) 
{
	// Set The Drawing Color To Light Green (Can Be Changed By Shader, Etc...)
	if (cg_enable && vshaders[VSHADER_NORMAL].activo)
		cgGLSetParameter4f(vshaders[VSHADER_NORMAL].cgParameter[NVPAR_VBINORMAL], a, b, c, d);
}

void NormalVShaderSetTangent( float a, float b, float c, float d) 
{
	// Set The Drawing Color To Light Green (Can Be Changed By Shader, Etc...)
	if (cg_enable && vshaders[VSHADER_NORMAL].activo)
		cgGLSetParameter4f(vshaders[VSHADER_NORMAL].cgParameter[NVPAR_VTANGENT], a, b, c, d);
}

void NormalVShaderSetLightPos( float a, float b, float c) 
{
	// Set The Drawing Color To Light Green (Can Be Changed By Shader, Etc...)
	if (cg_enable && vshaders[VSHADER_NORMAL].activo)
		cgGLSetParameter3f(vshaders[VSHADER_NORMAL].cgParameter[NVPAR_LIGHTPOS], a, b, c);
}


// PIXEL SHADERs

int loadPShader(int num, char *pshaderfile, char *functname)
{
	printf("Cargando Pixel Shader %s\n", pshaderfile);

	// Get The Latest GL Pixel Profile
	pshaders[num].cgProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);

	// Validate Our Profile Determination Was Successful
	if ( pshaders[num].cgProfile == CG_PROFILE_UNKNOWN)
	{
		printf("Invalid profile type\n");
		return(1);
	}

	cgGLSetOptimalOptions( pshaders[num].cgProfile );

	// Load the shader 
	pshaders[num].cgProgram = cgCreateProgramFromFile(cgContext, CG_SOURCE, pshaderfile, pshaders[num].cgProfile , functname, 0);

	// Validate Success
	if (pshaders[num].cgProgram == 0)
	{
		// We Need To Determine What Went Wrong
		CGerror Error = cgGetError();

		// Show A Message Box Explaining What Went Wrong
		printf("Error cargando Pixel Shader: %s \n",cgGetErrorString(Error));
		return(1);			// We Cannot Continue
	}

	// Load The Program
	cgGLLoadProgram( pshaders[num].cgProgram );

	pshaders[num].activo = 1;

	return 0;
}


/* Pixel Shader Normal */

void loadNormalPShaderVars() {
	pshaders[PSHADER_NORMAL].cgParameter[NPPAR_COLOR] = cgGetNamedParameter( pshaders[PSHADER_NORMAL].cgProgram, "IN.color");
	pshaders[PSHADER_NORMAL].cgParameter[NPPAR_TEXCOORDS] = cgGetNamedParameter( pshaders[PSHADER_NORMAL].cgProgram, "IN.texCoords");
	//pshaders[PSHADER_NORMAL].cgParameter[NPPAR_NORMALCOORDS] = cgGetNamedParameter( pshaders[PSHADER_NORMAL].cgProgram, "IN.normalCoords");
	pshaders[PSHADER_NORMAL].cgParameter[NPPAR_LIGHTVECTOR] = cgGetNamedParameter( pshaders[PSHADER_NORMAL].cgProgram, "IN.vLightVector");
	pshaders[PSHADER_NORMAL].cgParameter[NPPAR_BASETEXTURE] = cgGetNamedParameter( pshaders[PSHADER_NORMAL].cgProgram, "baseTexture");
	pshaders[PSHADER_NORMAL].cgParameter[NPPAR_NORMALTEXTURE] = cgGetNamedParameter( pshaders[PSHADER_NORMAL].cgProgram, "normalTexture");
	pshaders[PSHADER_NORMAL].cgParameter[NPPAR_LIGHTDIFFUSECOLOR] = cgGetNamedParameter( pshaders[PSHADER_NORMAL].cgProgram, "fLightDiffuseColor");
}


void ActivateNormalPShader(int textura, int texturanormal, float a, float b, float c)
{
	if (cg_enable && vshaders[ PSHADER_NORMAL ].activo)
	{
		// Enable Our Pixel Shader Profile
		cgGLEnableProfile( pshaders[PSHADER_NORMAL].cgProfile );

		// Bind Our Pixel Program To The Current State
		cgGLBindProgram( pshaders[PSHADER_NORMAL].cgProgram );

		cgGLSetTextureParameter( pshaders[PSHADER_NORMAL].cgParameter[NPPAR_BASETEXTURE], textura);
		cgGLEnableTextureParameter( pshaders[PSHADER_NORMAL].cgParameter[NPPAR_BASETEXTURE] );

		cgGLSetTextureParameter( pshaders[PSHADER_NORMAL].cgParameter[NPPAR_NORMALTEXTURE], texturanormal );
		cgGLEnableTextureParameter( pshaders[PSHADER_NORMAL].cgParameter[NPPAR_NORMALTEXTURE] );

		cgGLSetParameter3f( vshaders[PSHADER_NORMAL].cgParameter[NPPAR_LIGHTDIFFUSECOLOR] , 1,1,1);
	}

}


void DeactivateNormalPShader() 
{
	if (cg_enable && vshaders[ PSHADER_NORMAL ].activo){
		cgGLDisableProfile( pshaders[PSHADER_NORMAL].cgProfile );

	}
}


// Set color
void NormalPShaderSetColor(float a, float b, float c) 
{
	// Set The Drawing Color To Light Green (Can Be Changed By Shader, Etc...)
	if (cg_enable && vshaders[ PSHADER_NORMAL ].activo)
		cgGLSetParameter4f( pshaders[PSHADER_NORMAL].cgParameter[CG_PARAM_COLOR], a, b, c, 1.0);
}

#if 0
void NormalPShaderSetBaseTexture( float a, float b, float c)
{
	// Set The Wave Parameter Of Our Shader To The Incremented Wave Value From Our Main Program
	if (cg_enable && vshaders[ PSHADER_NORMAL ].activo)
		cgGLSetParameter3f( pshaders[PSHADER_NORMAL].cgParameter[CG_PARAM_WAVE] , a, b, c);
}


void NormalPShaderSetTexCoord(float a, float b)
{
	// Set The Wave Parameter Of Our Shader To The Incremented Wave Value From Our Main Program
	if (cg_enable && vshaders[ PSHADER_NORMAL ].activo)
		cgGLSetParameter2f( pshaders[PSHADER_NORMAL].cgParameter[NPPAR_TEXCOORDS] , a, b);
}

void NormalPShaderSetLightDist(float param)
{
	// Set The Wave Parameter Of Our Shader To The Incremented Wave Value From Our Main Program
	if (cg_enable && vshaders[ PSHADER_NORMAL ].activo)
		cgGLSetParameter3f( pshaders[PSHADER_NORMAL].cgParameter[CG_PARAM_WAVE] , param, 1.0f, 1.0f);
}
#endif
