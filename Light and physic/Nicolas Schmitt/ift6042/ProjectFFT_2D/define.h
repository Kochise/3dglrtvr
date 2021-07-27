#ifndef _DEFINE_H_
/*
 *   Copyright (c) 2002 Nicolas Schmitt
 *
 *   This file is part of (s)Smoke.
 *
 *   (s)Smoke is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   (s)Smoke is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details (gpl.txt).
 *
 *   You should have received a copy of the GNU General Public License
 *   along with (s)Smoke; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#define _DEFINE_H_

#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <rfftw.h>
#include <vfw.h>
#include <il/il.h>
#include <il/ilu.h>
#include <il/ilut.h>
#include "resource.h"
#include "utils.h"
#include "calcul.h"
#include "movie.h"

#define NDIM 2
#define USE_FFT
#define TEXT_NAME1 "texture.jpg"

#pragma message ( "2 dimensions" )
#define APPNAME "2D Smoke Demo"
#define ARR(i,j) ((i)*calculs.length[0]+(j))
#define ARR2(i,j) ((i)*(calculs.length[0]+2)+j)

#define floor(x) ( (x)>=0.0 ? ((int)(x)) : (-((int)(1-(x)))) )

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
	int CreateMovie ( char *filename, int rate );
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
	int bUseConfinement;
	int bMovie;
	int bSources;
	int bFiltre;
	int bTemperature;
	HWND hDlg;
	int width;
	int height;
	int fps;
	LARGE_INTEGER ticksPerSecond;
	LARGE_INTEGER startTime;
	int bCapture;
	POINT oldp;
	UINT texture1;
	AVIMovie *movie;
} globals_t;

typedef struct {
	// taille du modele en voxel
	int length[NDIM];
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
	double *vort;
#ifdef USE_FFT
	// les plans pour la fft
	rfftwnd_plan plan1, plan2;
#endif
} calculs_t;

extern globals_t globals;
extern calculs_t calculs;

#endif // _DEFINE_H_
