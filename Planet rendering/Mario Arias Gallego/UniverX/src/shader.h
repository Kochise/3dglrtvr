/* Práctica Final Informática Gráfica 2006
   Simulador Sistema Solar
   
   Mario Arias Gallego <mariari@lab.fi.uva.es>
   Álvaro Zubizarreta López <alvazub@lab.fi.uva.es>
*/

#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include "supergl.h"

#define NUM_VSHADERS 5
#define NUM_PSHADERS 5

// Parameters
#define MAX_CG_PARAMETERS 	 10

// Parametros Estrellas
#define CG_PARAM_POSITION 	 0
#define CG_PARAM_COLOR 		 1
#define CG_PARAM_MODELVIEWPROJ	 2
#define CG_PARAM_WAVE 		 3

#define CG_PARAM_TEXCOORD	 2
#define CG_PARAM_NORMALCOORD	 3
#define CG_PARAM_LIGHTDIST	 4
#define CG_PARAM_BASETEXTURE 	 5
#define CG_PARAM_BUMPMAP	 6

// Normal Vertex Parameter
#define NVPAR_POSITION 		0
#define NVPAR_COLOR		1
#define NVPAR_MODELVIEW		2
#define NVPAR_MODELVIEWPROJ	3
#define NVPAR_TEXCOORDS		4
#define NVPAR_VTANGENT		5
#define NVPAR_VBINORMAL		6
#define NVPAR_VNORMAL		7
#define NVPAR_LIGHTPOS		8

/*
#define NVPAR_TEXCOORD		0
#define NVPAR_VTANGENT		1
#define NVPAR_VBINORMAL		2
#define NVPAR_VNORMAL		3
#define NVPAR_MODELVIEW		4
#define NVPAR_LIGHTPOS		5
*/

// Normal Pixel Parameter
#define NPPAR_COLOR		0
#define NPPAR_TEXCOORDS		1
#define NPPAR_NORMALCOORDS	2
#define NPPAR_LIGHTVECTOR	3
#define NPPAR_BASETEXTURE	4
#define NPPAR_NORMALTEXTURE	5
#define NPPAR_LIGHTDIFFUSECOLOR	6


// Vertex Shaders
#define VSHADER_STARS 0
#define VSHADER_NORMAL 1

//#define VSHADER_STARS_NAME "./shader/vnormal1.cg"
#define VSHADER_STARS_NAME "./shader/vstar.cg"
#define VSHADER_NORMAL_NAME   "./shader/vnormal.cg"

#define VSHADER_STARS_FUNNAME "main"
#define VSHADER_NORMAL_FUNNAME "main"


// Pixel Shaders
#define PSHADER_NORMAL   1

#define PSHADER_NORMAL_NAME   "./shader/pnormal.cg"
#define PSHADER_NORMAL_FUNNAME "main"


struct vshader {
	CGprogram cgProgram;
	CGprofile cgVertexProfile;
	CGparameter cgParameter[MAX_CG_PARAMETERS];
	int activo;
};

struct pshader {
	CGprogram cgProgram;
	CGprofile cgProfile;
	CGparameter cgParameter[MAX_CG_PARAMETERS];
	int activo;
};
// Prototypes



int InicializaShaders();
void ParaShaders(void);


// VERTEX SHADERs
int loadVShader(int num, char *vshaderfile, char *functname);

void loadStarVShaderVars();
void ActivateStarVShader();
void DeactivateStarVShader();
void StarVShaderSetColor( float a, float b, float c, float d);
void StarVShaderSetWave( float param);

void loadNormalVShaderVars();
void ActivateNormalVShader();
void DeactivateNormalVShader();
void NormalVShaderSetBinormal( float a, float b, float c, float d);
void NormalVShaderSetTangent( float a, float b, float c, float d);
void NormalVShaderSetLightPos( float a, float b, float c);

// PIXEL SHADERs
int loadPShader(int num, char *pshaderfile, char *functname);

void loadNormalPShaderVars();
void ActivateNormalPShader(int textura, int texturanormal, float a, float b, float c);
void DeactivateNormalPShader();
void NormalPShaderSetColor(float a, float b, float c);
void NormalPShaderSetBaseTexture( float a, float b, float c);
void NormalPShaderSetTexCoord(float a, float b);
void NormalPShaderSetLightDist(float param);
