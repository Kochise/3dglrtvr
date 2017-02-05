#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <glh/gl/glext.h>
#include <glh/glh_extensions.h>
#include <cg/cgGL.h>
#include "resource.h"
#include <rfftw.h>
#include <il/il.h>
#include <il/ilu.h>
#include <il/ilut.h>
#include <vfw.h>
#include "shadow.h"
#include "utils.h"
#include "calcul.h"
#include "movie.h"

#define NDIM 3

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#pragma message ( "3 dimensions" )
#define APPNAME "3D Smoke Demo"

#define floor(x) ( (x)>=0.0 ? ((int)(x)) : (-((int)(1-(x)))) )
#define ARR(i,j,k) ( i + calculs.size * ( j + calculs.size * (k) ) )
#define ARR2(i,j,k) ( i + ( calculs.size + 2 ) * ( j + calculs.size * (k) ) )

inline int idist ( int ci, int cj, int ck, int i, int j, int k ) 
{
	return (int) sqrt ( ( ci - i )*( ci - i ) + ( cj - j )*( cj - j ) + ( ck - k )*( ck - k ) );
}

class AVIMovie {
private:
	PAVIFILE file;
	PAVISTREAM stream;
	PAVISTREAM streamcmp;
	AVISTREAMINFO asi;
	BITMAPINFOHEADER bmpInfo;
	int width;
	int height;
	int frameSize;
	int curFrame;

public:
	inline AVIMovie() {};
	inline ~AVIMovie() {};
	int CreateMovie ( const char *filename, const int rate );
	void TakeShot();
	void FinishMovie();
};

typedef struct {
	HWND hwnd;
	HINSTANCE hInstance;
	HWND hEdit;
	int bSimulate;
	int bVelocity;
	int bUseTexture;
	int bLine;
	int bMovie;
	int bDrawBox;
	int bSource;
	int bUseCg;
	HWND hDlg;
	int width;
	int height;
	float fps;
	LARGE_INTEGER ticksPerSecond;
	LARGE_INTEGER startTime;
	AVIMovie *movie;
	cgContext *cgcontext;
	cgProgramIter *cgprogiter;
	cgBindIter *ModelViewProjIter;
	cgBindIter *LightPosIter;
	cgBindIter *EyePosIter;
	cgBindIter *KdIter;
	cgBindIter *KsIter;
	cgBindIter *ShininessIter;
	cgBindIter *PositionIter;
	cgBindIter *NormalIter;
	float Kd;
	float Ks;
	float Shininess;
} globals_t;

typedef struct {
	// taille du modele en voxel
	int size;
	// le pas de temps
	double dt;
	// viscosite du fluide
	double visc;
	// constante de diffusion de la substance
	double kS;
	// taux de dissipation de la substance
	double aS;
	// les tableaux de vitesses ( nouveau et ancien )
	double *u0[NDIM];
	double *u1[NDIM];
	// le tableau des forces ( x et y )
	double *f[NDIM];
	// les tableaux des densites ( nouveau et ancien )
	double *s0, *s1;
	// les tableaux des temperatures ( nouveau et ancien )
	double *t0, *t1;
	// le tableau des sources de matieres
	double *source;
	// le tableau des sources de temperatures
	double *chaleur;
	// le tableau contenant les forces de confinements
	double *vort_conf[NDIM];
	// pour le calcul des forces de confinements
	double *vort[NDIM];
	// les plans pour la fft
	rfftwnd_plan plan1, plan2;
} calculs_t;

typedef struct {
	float pos[3];
	float light[3];
	float eyeDist;
	float eyeAngle, eyeHeight;
	float lightFrustumMatrix[16];
	float lightFieldOfView;
	char keys[256];
} render_t;

extern globals_t globals;
extern calculs_t calculs;
extern render_t render;

#endif // _DEFINE_H_
