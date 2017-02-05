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

#include "define.h"

/**
 *  Initialise notre model
 */
void InitModel ( void )
{
	int i;
	// La taille de notre 'monde'
	// si on utilise la FFT, il faut que le monde soit carre
	// et la taille doit etre multiple de 2
	calculs.length[0] = 80;
	calculs.length[1] = 80;

	calculs.dt = 0.4f;
	calculs.visc = 0.001;
	calculs.aS = 0.995;
	calculs.kS = 0.0;

#ifdef USE_FFT
	// initialisation de la fft
	calculs.plan1 = rfftw2d_create_plan ( calculs.length[0], calculs.length[1], FFTW_REAL_TO_COMPLEX, FFTW_IN_PLACE );
	calculs.plan2 = rfftw2d_create_plan ( calculs.length[0], calculs.length[1], FFTW_COMPLEX_TO_REAL, FFTW_IN_PLACE );
	// La taille est augmente de 1 octets a cause de la FFT
	int size = calculs.length[0] * ( calculs.length[1] + 2 );
#else
	int size = calculs.length[0] * calculs.length[1];
#endif

	for ( i = 0; i < NDIM; i++ ) {
		calculs.u0[i] = new double [ size ];
		calculs.u1[i] = new double [ size ];
		calculs.f[i] = new double [ size ];
		calculs.vort_conf[i] = new double [ size ];
	}
	calculs.s0 = new double [ size ];
	calculs.s1 = new double [ size ];
	calculs.t0 = new double [ size ];
	calculs.t1 = new double [ size ];
	calculs.source = new double [ size ];
	calculs.chaleur = new double [ size ];
	calculs.vort = new double [ size ];

	ResetSimulation();

	AddLine ( "Initialisation done.\r\n" );
}

/**
 *  Libère la memoire
 */
void FreeModel ( void )
{
	// c'est plus propre...
	int i;
	for ( i = 0; i < NDIM; i++ ) {
		delete[] calculs.u0[i];
		delete[] calculs.u1[i];
		delete[] calculs.f[i];
		delete[] calculs.vort_conf[i];
	}
	delete[] calculs.s0;
	delete[] calculs.s1;
	delete[] calculs.t0;
	delete[] calculs.t1;
	delete[] calculs.source;
	delete[] calculs.chaleur;
	delete[] calculs.vort;
#ifdef USE_FFT
	rfftwnd_destroy_plan ( calculs.plan1 );
    rfftwnd_destroy_plan ( calculs.plan2 );
#endif

}

/**
 *  Remet la simulation à zero
 */
void ResetSimulation ( void )
{
	int i, k, size;
#ifdef USE_FFT
	size = calculs.length[0] * ( 2 + calculs.length[1] );
#else
	size = calculs.length[0] * calculs.length[1];
#endif
	for ( i = 0; i < size; i++ ) {
		for ( k = 0; k < NDIM; k++ ) {
			calculs.u0[k][i] = 0.0f;
			calculs.u1[k][i] = 0.0f;
			calculs.f[k][i] = 0.0f;
			calculs.vort_conf[k][i] = 0.0f;
		}
		calculs.s0[i]  = 0.0f;
		calculs.s1[i]  = 0.0f;
		calculs.t0[i]  = 0.0f;
		calculs.t1[i]  = 0.0f;
		calculs.source[i] = 0.0f;
		calculs.chaleur[i] = 0.0f;
	}
}

inline void AddForce ( double *V1, double *V2 )
{
	for ( int i = 0; i < ( calculs.length[0] * calculs.length[1] ); i++ ) {
		V1[i] += V2[i] * calculs.dt;
		V2[i] *= 0.8; // ou V[2] = 0.0 (ou autre chose) ???
	}
}

void Transport ( double *V1, double *V2, double *U[NDIM] )
{
	int i, j, i0, j0, i1, j1;
	double x, y, s, t;

	for ( i = 0; i < calculs.length[0]; i++) {

		for ( j = 0; j < calculs.length[1]; j++ ) {

			// On calcul les coordonnes du points qui apres deplacement
			// arrive au point courant

			x = i - calculs.length[0] * U[0][ i + j * calculs.length[0] ] * calculs.dt;
			i0 = floor ( x );
			s = x - i0;
			i0 = ( ( i0 % calculs.length[0] ) + calculs.length[0] ) % calculs.length[0];
			i1 = ( i0 + 1 ) % calculs.length[0];

			y = j - calculs.length[1] * U[1][ i + j * calculs.length[0] ] * calculs.dt;
			j0 = floor ( y );
			t = y - j0;
			j0 = ( ( j0 % calculs.length[1] ) + calculs.length[1] ) % calculs.length[1];
			j1 = ( j0 + 1 ) % calculs.length[1];

			// on interpole les valeurs des 4 voxels les plus proches
			// et on affecte ca a notre voxel de depart

			/*
			V1[ i + calculs.length[0] * j ] = ( 1 - s ) * ( 1 - t ) * V2[ i0 + calculs.length[0] * j0 ] +
				                              ( 1 - s ) * t * V2[ i0 + calculs.length[0] * j1 ] +
											  s * ( 1 - t ) * V2[ i1 + calculs.length[0] * j0 ] +
											  s * t * V2[ i1 + calculs.length[0] * j1 ];
			*/

			// un peu plus optimise...
			V1[ i + calculs.length[0] * j ] = ( 1 - s ) * ( ( 1 - t ) * V2[ i0 + calculs.length[0] * j0 ] + t * V2[ i0 + calculs.length[0] * j1 ] ) +
											  s * ( ( 1 - t ) * V2[ i1 + calculs.length[0] * j0 ] + t * V2[ i1 + calculs.length[0] * j1 ] );

		}

	}

}

// pour la diffusion, on travaille dans le domaine de Fourier.
// Dans ce monde obscure, la viscosite est un filtre passe bas
inline void DiffuseFFT ( double *V1, double coef )
{
	int i, j;
	double x, y, dist2, filtre;

	for ( j = 0; j <= calculs.length[0]; j+=2 ) {

		x = 0.5 * j;
		
		for ( i = 0; i < calculs.length[1]; i++ ) {

			y = ( i <= calculs.length[1]/2 ) ? i : ( i - calculs.length[1] );
			dist2 = x * x + y * y;
			if ( dist2 == 0 ) {
				continue;
			}
			filtre = exp ( - dist2 * calculs.dt * coef );

			// partie reelle
			V1[ j + ( calculs.length[0] + 2 ) * i ] *= filtre;
			// partie imaginaire
			V1[ j + 1 + ( calculs.length[0] + 2 ) * i ] *= filtre;

		}

	}
}

// On projete les vitesses sur les vecteurs tangents
// aux cercles centres sur l'origine
inline void ProjectFFT ( double *V1[NDIM] )
{
	int i, j;
	double x, y, dist2;

	for ( j = 0; j <= calculs.length[0]; j+=2 ) {

		x = 0.5 * j;

		for ( i = 0; i < calculs.length[1]; i++ ) {

			y = ( i <= calculs.length[1] / 2 ) ? i : ( i - calculs.length[1] );
			dist2 = x * x + y * y;
			if ( dist2 == 0 ) {
				continue;
			}
			// partie reelle en X et Y
			double RVx = V1[0][j+(calculs.length[0]+2)*i];
			double RVy = V1[1][j+(calculs.length[0]+2)*i];
			// partie imaginaire en X et Y
			double IVx = V1[0][j+1+(calculs.length[0]+2)*i];
			double IVy = V1[1][j+1+(calculs.length[0]+2)*i];

			double ddotur = ( RVx * x + RVy * y )/ dist2;
			double ddotui = ( IVx * x + IVy * y ) / dist2;

			// partie reelle
			V1[0][ARR2(i,j)] = RVx - ddotur * x;
			// partie imaginaire
			V1[0][ARR2(i,j+1)] = IVx - ddotui * x;

			// partie reelle
			V1[1][ARR2(i,j)] = RVy - ddotur * y;
			// partie imaginaire
			V1[1][ARR2(i,j+1)] = IVy - ddotui * y;

		}

	}
}

inline void Dissipate ( double *V, double coef )
{
	for ( int i = 0; i < ( calculs.length[0] * calculs.length[1] ); i++ ) {
		V[i] *= coef;
	}
}

inline void VorticityConfinement ( double *V[NDIM] )
{

	int i, j;

	for ( i = 0; i < calculs.length[1]; i++ ) {

		int im1 = ( (i-1)%calculs.length[1] + calculs.length[1] ) % calculs.length[1];
		int ip1 = ( (i+1)%calculs.length[1] + calculs.length[1] ) % calculs.length[1];

		for ( j = 0; j < calculs.length[0]; j++ ) {

			int jm1 = ( (j-1)%calculs.length[0] + calculs.length[0] ) % calculs.length[0];
			int jp1 = ( (j+1)%calculs.length[0] + calculs.length[0] ) % calculs.length[0];
			calculs.vort[ARR(i,j)] = ( ( V[1][ARR(ip1,j)] - V[1][ARR(im1,j)] ) -
				                       ( V[0][ARR(i,jp1)] - V[0][ARR(i,jm1)] ) ) / 2;

		}

	}

	double vort_c[NDIM];
	double norm;

	for ( i = 0; i < calculs.length[1]; i++ ) {
		int im1 = ( (i-1)%calculs.length[1] + calculs.length[1] ) % calculs.length[1];
		int ip1 = ( (i+1)%calculs.length[1] + calculs.length[1] ) % calculs.length[1];
		for ( j = 0; j < calculs.length[0]; j++ ) {
			int jm1 = ( (j-1)%calculs.length[0] + calculs.length[0] ) % calculs.length[0];
			int jp1 = ( (j+1)%calculs.length[0] + calculs.length[0] ) % calculs.length[0];
			vort_c[0] = ( fabs ( calculs.vort[ARR(ip1,j)] ) - fabs ( calculs.vort[ARR(im1,j)] ) ) / 2;
			vort_c[1] = ( fabs ( calculs.vort[ARR(i,jp1)] ) - fabs ( calculs.vort[ARR(i,jm1)] ) ) / 2;
			norm = sqrt ( vort_c[0] * vort_c[0] + vort_c[1] * vort_c[1] );
			if ( norm != 0.0 ) {
				calculs.vort_conf[0][ARR(i,j)] = vort_c[0] / norm;
				calculs.vort_conf[1][ARR(i,j)] = vort_c[1] / norm;
			}
		}
	}

	for ( i = 0; i < calculs.length[1]; i++ ) {
		for ( j = 0; j < calculs.length[0]; j++ ) {
			double temp = calculs.vort_conf[0][ARR(i,j)];
			calculs.vort_conf[0][ARR(i,j)] = 0.1 * calculs.vort[ARR(i,j)] * calculs.vort_conf[1][ARR(i,j)];
			calculs.vort_conf[1][ARR(i,j)] = -0.1 * calculs.vort[ARR(i,j)] * temp;
		}
	}


}

void SolveVelocity ( void )
{
	int i, j, k;

	for ( i = 0; i < NDIM; i++ ) {
		AddForce ( calculs.u0[i], calculs.f[i] );
	}

	if ( globals.bUseConfinement ) {
		for ( i = 0; i < NDIM; i++ ) {
			AddForce ( calculs.u0[i], calculs.vort_conf[i] );
		}
	}

	for ( i = 0; i < NDIM; i++ ) {
		Transport ( calculs.u1[i], calculs.u0[i], calculs.u0 );
	}

	// On se place dans le domaine de Fourier
	for ( j = 0; j < calculs.length[0]; j++ ) {
		for ( i = 0; i < calculs.length[1]; i++ ) {
			for ( k = 0; k < NDIM; k++ ) {
				calculs.u0[k][j+i*(2+calculs.length[0])] = calculs.u1[k][j+i*calculs.length[0]];
			}
		}
	}

	for ( i = 0; i < NDIM; i++ ) {
		rfftwnd_one_real_to_complex ( calculs.plan1, (fftw_real *)calculs.u0[i], NULL );
	}

	for ( i = 0; i < NDIM; i++ ) {
		DiffuseFFT ( calculs.u0[i], calculs.visc );
	}

	ProjectFFT ( calculs.u0 );

	// On revient dans le domaine 'normal'
	for ( i = 0; i < NDIM; i++ ) {
		rfftwnd_one_complex_to_real ( calculs.plan2, (fftw_complex *)calculs.u0[i], NULL );
	}

	// On normalise le resultat
	//( la FFT multiplie tout par 'calculs.length[0] * calculs.length[1]' )
	for ( j = 0; j < calculs.length[0]; j++ ) {
		for ( i = 0; i < calculs.length[1]; i++ ) {
			for ( k = 0; k < NDIM; k++ ) {
				calculs.u1[k][j+i*calculs.length[0]] = calculs.u0[k][j+i*(2+calculs.length[0])] / ( calculs.length[0] * calculs.length[1] );
			}
		}
	}

	if ( globals.bUseConfinement ) {
		VorticityConfinement ( calculs.u1 );
	}

}

void ApplyGaussianFilter ( double *V0, double *V1 )
{
	int i,j;

	// On se place dans le domaine de Fourier
	for ( j = 0; j < calculs.length[0]; j++ ) {
		for ( i = 0; i < calculs.length[1]; i++ ) {
			V0[ARR2(i,j)] = V1[ARR(i,j)];
		}
	}

	rfftwnd_one_real_to_complex ( calculs.plan1, (fftw_real *)V0, NULL );

	double *masque = new double [ calculs.length[0] * ( calculs.length[1] + 2 ) ];

	double x, y, dist2;

	for ( j = 0; j <= calculs.length[0]; j+=2 ) {

		x = 0.5 * j;

		for ( i = 0; i < calculs.length[1]; i++ ) {

			y = ( i <= calculs.length[1] / 2 ) ? i : ( i - calculs.length[1] );

			dist2 = x * x + y * y;
			if ( dist2 == 0 ) {
				continue;
			}

			masque[ARR2(i,j)] = exp ( - dist2 * 0.2 );

		}

	}

	delete[] masque;

	rfftwnd_one_complex_to_real ( calculs.plan2, (fftw_complex *)V0, NULL );

	// On normalise le resultat
	//( la FFT multiplie tout par 'calculs.length[0] * calculs.length[1]' )
	for ( j = 0; j < calculs.length[0]; j++ ) {
		for ( i = 0; i < calculs.length[1]; i++ ) {
			V1[ARR(i,j)] = V0[ARR2(i,j)] / ( calculs.length[0] * calculs.length[1] );
		}
	}
}

void ApplyBilinearFilter ( double *V0, double *V1 )
{
	int i,j;

	memcpy ( V0, V1, calculs.length[0] * calculs.length[1] * sizeof ( double ) );

	for ( i = 0; i < calculs.length[1]; i++ ) {
		int im1 = ( (i-1)%calculs.length[1] + calculs.length[1] ) % calculs.length[1];
		int ip1 = ( (i+1)%calculs.length[1] + calculs.length[1] ) % calculs.length[1];
		for ( j = 0; j < calculs.length[0]; j++ ) {
			int jm1 = ( (j-1)%calculs.length[0] + calculs.length[0] ) % calculs.length[0];
			int jp1 = ( (j+1)%calculs.length[0] + calculs.length[0] ) % calculs.length[0];
			V0[ARR(i,j)] = V1[ARR(i,j)] + 1/4 * V1[ARR(ip1,j)] + 1/4 * V1[ARR(im1,j)] + 1/4 * V1[ARR(i,jp1)] + 1/4 * V1[ARR(i,jm1)];
			//V0[ARR(i,j)] /= 2.0;
		}
	}

}

inline void SolveScalar ( void )
{
	AddForce ( calculs.s0, calculs.source );
	Transport ( calculs.s1, calculs.s0, calculs.u1 );
	Dissipate ( calculs.s1, calculs.aS );
	if ( globals.bFiltre ) {
		ApplyBilinearFilter ( calculs.s0, calculs.s1 );
		//ApplyGaussianFilter ( calculs.s0, calculs.s1 );
	} else {
		memcpy ( calculs.s0, calculs.s1, calculs.length[0] * calculs.length[1] * sizeof ( double ) );
	}
}

inline SolveTemperature ( void )
{
	AddForce ( calculs.t0, calculs.chaleur );
	Transport ( calculs.t1, calculs.t0, calculs.u1 );
	Dissipate ( calculs.t1, 0.8 );
}

void calcul ( void )
{
	int k;

	for ( k = 0; k < NDIM; k++ ) {
		memcpy ( calculs.u0[k], calculs.u1[k], calculs.length[0] * calculs.length[1] * sizeof ( double ) );
	}
	memcpy ( calculs.s0, calculs.s1, calculs.length[0] * calculs.length[1] * sizeof ( double ) );
	memcpy ( calculs.t0, calculs.t1, calculs.length[0] * calculs.length[1] * sizeof ( double ) );

	// Calcul de la nouvelle vitesse et la
	// nouvelle quantite de matiere
	SolveVelocity();
	SolveScalar();
	SolveTemperature();
}

/**
 *  Affiche le champs de vitesse
 */
inline void RenderVelocity ( float w, float h )
{
	int i, j;

	glDisable ( GL_TEXTURE_2D );
	glBegin ( GL_LINES );
	glColor3f ( 1.0f, 0.0f, 0.0f );

	for ( j = 0; j < calculs.length[0]; j++ ) {
		for ( i = 0; i < calculs.length[1]; i++ ) {
			glVertex2f ( j * w, i * h );
			glVertex2f ( j * w + 1000*(float)calculs.u1[0][j+i*calculs.length[0]],
						 i * h + 1000*(float)calculs.u1[1][j+i*calculs.length[0]] );
		}
	}

	glEnd();
}

/**
 *  Affiche un champ de densité à l'écran
 */
inline void RenderScalar ( double *V, float w, float h )
{
	int i, j;

	if ( globals.bUseTexture ) {
		glEnable ( GL_TEXTURE_2D );
	} else {
		glDisable ( GL_TEXTURE_2D );
		glEnable ( GL_BLEND );
	}

	if ( globals.bLine ) {
		glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
	} else {
		glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
	}

	for ( i = 0; i < calculs.length[1] - 1; i++ ) {

		glBegin ( GL_TRIANGLE_STRIP );

		if ( globals.bUseTexture ) {
			glTexCoord2f ( ( 10*(float)V[ i * calculs.length[0] ] ) / globals.width,
						   ( i * h + 10*(float)V[ i * calculs.length[0] ] ) / globals.height );
			glColor3f ( 1.0f, 1.0f, 1.0f );
		} else {
			glColor4f ( (float)V[ i * calculs.length[0] ], 
						(float)V[ i * calculs.length[0] ], 
						(float)V[ i * calculs.length[0] ], 0.2f );
		}

		glVertex2f ( 0, i * h );

		for ( j = 0; j < calculs.length[0] - 1; j++ ) {

			if ( globals.bUseTexture ) {
				glTexCoord2f ( ( j * w + 10*(float)V[ j + i * calculs.length[0] ] ) / globals.width,
							   ( ( i + 1 ) * h + 10*(float)V[ j + i * calculs.length[0] ]) / globals.height );
				glColor3f ( 1.0f, 1.0f, 1.0f );
			} else {
				glColor4f ( (float)V[ j + ( i + 1 ) * calculs.length[0] ], 
							(float)V[ j + ( i + 1 ) * calculs.length[0] ], 
							(float)V[ j + ( i + 1 ) * calculs.length[0] ], 0.2f );
			}

			glVertex2f ( j * w, ( i + 1 ) * h );

			if ( globals.bUseTexture ) {
				glTexCoord2f ( ( ( j + 1 ) * w + 10*(float)V[ j + i * calculs.length[0] ] ) / globals.width,
							   ( i * h + 10*(float)V[ j + i * calculs.length[0] ]) / globals.height );
				glColor3f ( 1.0f, 1.0f, 1.0f );
			} else {
				glColor4f ( (float)V[ j + 1 + i * calculs.length[0] ], 
							(float)V[ j + 1 + i * calculs.length[0] ], 
							(float)V[ j + 1 + i * calculs.length[0] ], 0.2f );
			}

			glVertex2f ( ( j + 1 ) * w, i * h );

		}

		if ( globals.bUseTexture ) {
			glTexCoord2f ( ( j * w + 10*(float)calculs.s1 [ j + i * calculs.length[0] ] ) / globals.width,
						   ( ( i + 1 ) * h + 10*(float)calculs.s1 [ j + i * calculs.length[0] ]) / globals.height );
			glColor3f ( 1.0f, 1.0f, 1.0f );
		} else {
			glColor4f ( (float)V[ j + ( i + 1 ) * calculs.length[0] ], 
						(float)V[ j + ( i + 1 ) * calculs.length[0] ], 
						(float)V[ j + ( i + 1 ) * calculs.length[0] ], 0.2f );
		}

		glVertex2f ( j * w, ( i + 1 ) * h );

		glEnd();


	}

	glDisable ( GL_BLEND );

}

/**
 *  Affiche la scene
 */
void RenderScene ( void )
{
	float w = (float)globals.width / (float)( calculs.length[0] - 1 );
	float h = (float)globals.height / (float)( calculs.length[1] - 1 );

	RenderScalar ( calculs.s0, w, h );
	// RenderScalar ( calculs.t1, w, h );
	// RenderScalar ( calculs.vort, w, h );
	if ( globals.bVelocity ) {
		RenderVelocity ( w, h );
	}
}

/**
 *  Applique une force sur chaque voxel considérant le poids
 *  de la fumée à cet endroit et sa température.
 */
void MakeForces ( void )
{
	for ( int i = 0; i < calculs.length[1]; i++ ) {
		for ( int j = 0; j < calculs.length[0]; j++ ) {
			calculs.f[1][ARR(i,j)] = 0.007 * calculs.t1[ARR(i,j)];
			calculs.f[1][ARR(i,j)] += - 0.0001 * calculs.s1[ARR(i,j)]; // * ( calculs.length[0] - j );
			if ( calculs.s1[ARR(i,j)] > 1e-5 ) {
				if ( rand() / (float)RAND_MAX > 0.5 ) {
					calculs.f[0][ARR(i,j)] = ( rand() - RAND_MAX/2 ) / ( 100.0 * RAND_MAX );
				}
			}
		}
	}
}

/**
 *  Cree un source aléatoire
 */
void MakeSources ( void )
{
	const int i = calculs.length[1] / 4;
	const int j = 0;

	calculs.source[ARR(i-1,j)] = rand() % 4;
	calculs.source[ARR(i,j)]   = rand() % 4;
	calculs.source[ARR(i+1,j)] = rand() % 4;
		
	calculs.chaleur[ARR(i-1,j)] = rand() % 5;
	calculs.chaleur[ARR(i,j)]   = rand() % 5;
	calculs.chaleur[ARR(i+1,j)] = rand() % 5;

	calculs.f[0][ARR(i-1,j)] = (double)rand() / ( 5.0 * RAND_MAX );
	calculs.f[1][ARR(i,j)] = -(double)rand() / ( 15 * RAND_MAX );
	calculs.f[0][ARR(i+1,j)] = (double)rand() / ( 5.0 * RAND_MAX );
	calculs.f[1][ARR(i-1,j)] = -(double)rand() / ( 15 * RAND_MAX );
	calculs.f[0][ARR(i,j)] = (double)rand() / ( 5.0 * RAND_MAX );
	calculs.f[1][ARR(i+1,j)] = -(double)rand() / ( 15 * RAND_MAX );
}

/**
 *  Crée une source de matière/chaleur
 *  en bas a droite de l'ecran
 */
void MakeSources2 ( void )
{
	const int i = 0;
	const int j = calculs.length[0] * 3 / 4;

	if ( ( rand() / (float)RAND_MAX ) < 0.3 ) {
		calculs.source[ARR(i-2,j)] = 2.0f * rand()/(float)RAND_MAX;
		calculs.source[ARR(i-1,j)] = 2.0f * rand()/(float)RAND_MAX;
		calculs.source[ARR(i,j)]   = 2.0f * rand()/(float)RAND_MAX;
		calculs.source[ARR(i+1,j)] = 2.0f * rand()/(float)RAND_MAX;
		calculs.source[ARR(i+2,j)] = 2.0f * rand()/(float)RAND_MAX;
		calculs.chaleur[ARR(i-2,j)] = rand()%3;
		calculs.chaleur[ARR(i-1,j)] = rand()%3;
		calculs.chaleur[ARR(i,j)]   = rand()%3;
		calculs.chaleur[ARR(i+1,j)] = rand()%3;
		calculs.chaleur[ARR(i+2,j)] = rand()%3;
		calculs.f[0][ARR(i-2,j)] = rand() / ( 100.0 * RAND_MAX );
		calculs.f[0][ARR(i-1,j)] = rand() / ( 100.0 * RAND_MAX );
		calculs.f[0][ARR(i,j)]   = rand() / ( 100.0 * RAND_MAX );
		calculs.f[0][ARR(i+1,j)] = rand() / ( 100.0 * RAND_MAX );
		calculs.f[0][ARR(i+2,j)] = rand() / ( 100.0 * RAND_MAX );
		calculs.f[1][ARR(i-2,j)] = rand() / ( 100.0 * RAND_MAX );
		calculs.f[1][ARR(i-1,j)] = rand() / ( 100.0 * RAND_MAX );
		calculs.f[1][ARR(i,j)]   = rand() / ( 100.0 * RAND_MAX );
		calculs.f[1][ARR(i+1,j)] = rand() / ( 100.0 * RAND_MAX );
		calculs.f[1][ARR(i+2,j)] = rand() / ( 100.0 * RAND_MAX );
	}
}

void CreateFields ( void )
{
	int randx = rand() % calculs.length[0];
	int randy = rand() % calculs.length[1];
	calculs.source[ARR(randx, randy)] = 20.0;
	calculs.chaleur[ARR(randx, randy)] = 5.0;
	calculs.f[0][ARR(randx, randy)] = 3 * ( (double)rand() / (double)RAND_MAX - 0.5 );
	calculs.f[1][ARR(randx, randy)] = 3 * ( (double)rand() / (double)RAND_MAX - 0.5 );
}
