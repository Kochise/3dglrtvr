#include "define.h"

float InitModel ( void )
{
	int i;
	int memory = sizeof ( calculs_t ) + sizeof ( globals_t ) + sizeof ( render_t );

	// La taille de notre 'monde'
	// si on utilise la FFT, il faut que le monde soit carre
	// et la taille doit etre multiple de 2 ( peut etre pas en fait )
	calculs.size = 80;

	render.eyeDist = - 2.0f * calculs.size;

	render.pos[0] = 0.0f;
	render.pos[1] = 0.0f;
	render.pos[2] = -2.0f * calculs.size;

	render.light[0] = -60.0f;
	render.light[1] = 90.0f;
	render.light[2] = 0.0f;

	calculs.dt = 0.4f;
	calculs.visc = 0.001;
	calculs.aS = 0.90;
	calculs.kS = 0.0;

	// initialisation de la fft
	calculs.plan1 = rfftw3d_create_plan ( calculs.size, calculs.size, calculs.size, FFTW_REAL_TO_COMPLEX, FFTW_IN_PLACE );
	calculs.plan2 = rfftw3d_create_plan ( calculs.size, calculs.size, calculs.size, FFTW_COMPLEX_TO_REAL, FFTW_IN_PLACE );
	// La taille est augmente de 1 octets a cause de la FFT
	int size = calculs.size * calculs.size * ( calculs.size + 2 );

	for ( i = 0; i < NDIM; i++ ) {
		calculs.u0[i] = new double [ size ];
		calculs.u1[i] = new double [ size ];
		calculs.f[i] = new double [ size ];
		calculs.vort_conf[i] = new double [ size ];
		calculs.vort[i] = new double [ size ];
		memory += 5 * size * sizeof ( double );
	}
	calculs.s0 = new double [ size ];
	calculs.s1 = new double [ size ];
	calculs.t0 = new double [ size ];
	calculs.t1 = new double [ size ];
	calculs.source = new double [ size ];
	calculs.chaleur = new double [ size ];
	memory += 6 * size * sizeof ( double );

	ResetSimulation();

	return ( memory / ( 1024.0f * 1024.0f ) );

}

void FreeModel ( void )
{
	// c'est plus propre...
	int i;
	for ( i = 0; i < NDIM; i++ ) {
		delete[] calculs.u0[i];
		delete[] calculs.u1[i];
		delete[] calculs.f[i];
		delete[] calculs.vort_conf[i];
		delete[] calculs.vort[i];
	}
	delete[] calculs.s0;
	delete[] calculs.s1;
	delete[] calculs.t0;
	delete[] calculs.t1;
	delete[] calculs.source;
	delete[] calculs.chaleur;
	rfftwnd_destroy_plan ( calculs.plan1 );
    rfftwnd_destroy_plan ( calculs.plan2 );
}

void ResetSimulation ( void )
{
	int k;
	const int size = calculs.size * calculs.size * ( 2 + calculs.size ) * sizeof ( double );

	for ( k = 0; k < NDIM; k++ ) {
		memset ( calculs.u0[k], 0, size );
		memset ( calculs.u1[k], 0, size );
		memset ( calculs.f[k], 0, size );
		memset ( calculs.vort_conf[k], 0, size );
		memset ( calculs.vort[k], 0, size );
	}
	memset ( calculs.s0, 0, size );
	memset ( calculs.s1, 0, size );
	memset ( calculs.t0, 0, size );
	memset ( calculs.t1, 0, size );
	memset ( calculs.source, 0, size );
	memset ( calculs.chaleur, 0, size );
}

inline void AddForce ( double *V1, double *V2 )
{
	for ( int i = 0; i < ( calculs.size * calculs.size * calculs.size ); i++ ) {
		V1[i] += V2[i] * calculs.dt;
		//V2[i] *= 0.8;
		V2[i] = 0.0;
	}
}

void Transport ( double *V1, double *V2, double *U[NDIM] )
{
	int i, j, k, i0, j0, k0, i1, j1, k1;
	double x, y, z, s, t, r;

	for ( i = 0; i < calculs.size; i++) {

		for ( j = 0; j < calculs.size; j++ ) {

			for ( k = 0; k < calculs.size; k++ ) {

				// On calcul les coordonnes du points qui apres deplacement
				// arrive au point courant

				x = i - calculs.size * U[0][ARR(i,j,k)] * calculs.dt;
				i0 = floor ( x );
				s = x - i0;
				i0 = ( ( i0 % calculs.size ) + calculs.size ) % calculs.size;
				i1 = ( i0 + 1 ) % calculs.size;

				y = j - calculs.size * U[1][ARR(i,j,k)] * calculs.dt;
				j0 = floor ( y );
				t = y - j0;
				j0 = ( ( j0 % calculs.size ) + calculs.size ) % calculs.size;
				j1 = ( j0 + 1 ) % calculs.size;

				z = k - calculs.size * U[2][ARR(i,j,k)] * calculs.dt;
				k0 = floor ( z );
				r = z - k0;
				k0 = ( ( k0 % calculs.size ) + calculs.size ) % calculs.size;
				k1 = ( k0 + 1 ) % calculs.size;

				// on interpole les valeurs des 4 voxels les plus proches
				// et on affecte ca a notre voxel de depart
				V1[ARR(i,j,k)] = ( 1 - s ) * ( 1 - t ) * ( 1 - r ) * V2[ARR(i0,j0,k0)] + 
					             ( 1 - s ) * ( 1 - t ) * r * V2[ARR(i0,j0,k1)] +
								 ( 1 - s ) * t * ( 1 - r ) * V2[ARR(i0,j1,k0)] + 
								 s * ( 1 - t ) * ( 1 - r ) * V2[ARR(i1,j0,k0)] + 
								 ( 1 - s ) * t * r * V2[ARR(i0,j1,k1)] + 
								 s * t * ( 1 - r ) * V2[ARR(i1,j1,k0)] + 
								 s * ( 1 - t ) * r * V2[ARR(i1,j0,k1)] + 
								 s * t * r * V2[ARR(i1,j1,k1)];

			}

		}

	}

}

// On projete les vitesses sur les vecteurs tangents
// aux cercles centres sur l'origine
// et on applique un filtre passe bas pour la viscosite
void DiffuseAndProjectFFT ( double *V1[NDIM], double coef )
{
	int i, j, k;
	double x, y, z, dist2, filtre;

	for ( i = 0; i <= calculs.size; i += 2 ) {

		x = 0.5 * i;

		for ( j = 0; j < calculs.size; j++ ) {

			y = ( j <= calculs.size / 2 ) ? j : ( j - calculs.size );

			for ( k = 0; k < calculs.size; k++ ) {

				z = ( k <= calculs.size / 2 ) ? k : ( k - calculs.size );

				dist2 = x*x + y*y + z*z;
				if ( dist2 == 0.0 ) {
					continue;
				}

				filtre = exp ( - dist2 * calculs.dt * coef );
				// partie reelle en X, Y et Z
				const double RVx = filtre * V1[0][ARR2(i,j,k)];
				const double RVy = filtre * V1[1][ARR2(i,j,k)];
				const double RVz = filtre * V1[2][ARR2(i,j,k)];
				// partie imaginaire en X, Y et Z
				const double IVx = filtre * V1[0][ARR2(i+1,j,k)];
				const double IVy = filtre * V1[1][ARR2(i+1,j,k)];
				const double IVz = filtre * V1[2][ARR2(i+1,j,k)];

				const double ddotur = ( RVx * x + RVy * y + RVz * z ) / dist2;
				const double ddotui = ( IVx * x + IVy * y + IVz * z ) / dist2;

				// partie reelle
				V1[0][ARR2(i,j,k)] = RVx - ddotur * x;
				// partie imaginaire
				V1[0][ARR2(i+1,j,k)] = IVx - ddotui * x;

				// partie reelle
				V1[1][ARR2(i,j,k)] = RVy - ddotur * y;
				// partie imaginaire
				V1[1][ARR2(i+1,j,k)] = IVy - ddotui * y;

				// partie reelle
				V1[2][ARR2(i,j,k)] = RVz - ddotur * z;
				// partie imaginaire
				V1[2][ARR2(i+1,j,k)] = IVz - ddotui * z;

			}

		}

	}

}

inline void Dissipate ( double *V, const double coef )
{
	for ( int i = 0; i < ( calculs.size * calculs.size * calculs.size ); i++ ) {
		V[i] *= coef;
	}
}

void VorticityConfinement ( double *V[NDIM] )
{

	int i, j, k;

	for ( i = 0; i < calculs.size; i++ ) {

		int im1 = ( (i-1)%calculs.size + calculs.size ) % calculs.size;
		int ip1 = ( (i+1)%calculs.size + calculs.size ) % calculs.size;

		for ( j = 0; j < calculs.size; j++ ) {

			int jm1 = ( (j-1)%calculs.size + calculs.size ) % calculs.size;
			int jp1 = ( (j+1)%calculs.size + calculs.size ) % calculs.size;

			for ( k = 0; k < calculs.size; k++ ) {

				int km1 = ( (k-1)%calculs.size + calculs.size ) % calculs.size;
				int kp1 = ( (k+1)%calculs.size + calculs.size ) % calculs.size;

				calculs.vort[0][ARR(i,j,k)] = ( ( V[2][ARR(i,jp1,k)] - V[2][ARR(i,jm1,k)] ) -
											    ( V[1][ARR(i,j,kp1)] - V[1][ARR(i,j,km1)] ) ) / 2;

				calculs.vort[1][ARR(i,j,k)] = ( ( V[0][ARR(i,j,kp1)] - V[0][ARR(i,j,km1)] ) -
											    ( V[2][ARR(ip1,j,k)] - V[2][ARR(im1,j,k)] ) ) / 2;

				calculs.vort[2][ARR(i,j,k)] = ( ( V[1][ARR(ip1,j,k)] - V[1][ARR(im1,j,k)] ) -
											    ( V[0][ARR(i,jp1,k)] - V[0][ARR(i,jm1,k)] ) ) / 2;

			}

		}

	}

	double vort_c[NDIM];
	double norm, norm2;

	for ( i = 0; i < calculs.size; i++ ) {
		int im1 = ( (i-1)%calculs.size + calculs.size ) % calculs.size;
		int ip1 = ( (i+1)%calculs.size + calculs.size ) % calculs.size;
		for ( j = 0; j < calculs.size; j++ ) {
			int jm1 = ( (j-1)%calculs.size + calculs.size ) % calculs.size;
			int jp1 = ( (j+1)%calculs.size + calculs.size ) % calculs.size;
			for ( k = 0; k < calculs.size; k++ ) {
				int km1 = ( (k-1)%calculs.size + calculs.size ) % calculs.size;
				int kp1 = ( (k+1)%calculs.size + calculs.size ) % calculs.size;
				norm  = fabs ( calculs.vort[0][ARR(ip1,j,k)] ) +
					    fabs ( calculs.vort[1][ARR(ip1,j,k)] ) +
					    fabs ( calculs.vort[2][ARR(ip1,j,k)] );
				norm2 = fabs ( calculs.vort[0][ARR(im1,j,k)] ) +
					    fabs ( calculs.vort[1][ARR(im1,j,k)] ) +
					    fabs ( calculs.vort[2][ARR(im1,j,k)] );
				vort_c[0] = ( norm - norm2 ) / 2;

				norm  = fabs ( calculs.vort[0][ARR(i,jp1,k)] ) +
					    fabs ( calculs.vort[1][ARR(i,jp1,k)] ) +
					    fabs ( calculs.vort[2][ARR(i,jp1,k)] );
				norm2 = fabs ( calculs.vort[0][ARR(i,jm1,k)] ) +
					    fabs ( calculs.vort[1][ARR(i,jm1,k)] ) +
					    fabs ( calculs.vort[2][ARR(i,jm1,k)] );
				vort_c[1] = ( norm - norm2 ) / 2;

				norm  = fabs ( calculs.vort[0][ARR(i,j,kp1)] ) +
					    fabs ( calculs.vort[1][ARR(i,j,kp1)] ) +
					    fabs ( calculs.vort[2][ARR(i,j,kp1)] );
				norm2 = fabs ( calculs.vort[0][ARR(i,j,km1)] ) +
					    fabs ( calculs.vort[1][ARR(i,j,km1)] ) +
					    fabs ( calculs.vort[2][ARR(i,j,km1)] );
				vort_c[2] = ( norm - norm2 ) / 2;

				norm = sqrt ( vort_c[0] * vort_c[0] + vort_c[1] * vort_c[1] + vort_c[2] * vort_c[2] );
				if ( norm != 0.0 ) {
					calculs.vort_conf[0][ARR(i,j,k)] = vort_c[0] / norm;
					calculs.vort_conf[1][ARR(i,j,k)] = vort_c[1] / norm;
					calculs.vort_conf[2][ARR(i,j,k)] = vort_c[2] / norm;
				}
			}
		}
	}

	for ( i = 0; i < calculs.size; i++ ) {
		for ( j = 0; j < calculs.size; j++ ) {
			for ( k = 0; k < calculs.size; k++ ) {
				double temp0 = calculs.vort_conf[0][ARR(i,j,k)];
				double temp1 = calculs.vort_conf[1][ARR(i,j,k)];
				double temp2 = calculs.vort_conf[2][ARR(i,j,k)];
				calculs.vort_conf[0][ARR(i,j,k)] = 0.1 * ( calculs.vort_conf[1][ARR(i,j,k)] * calculs.vort[2][ARR(i,j,k)] - calculs.vort_conf[2][ARR(i,j,k)] * calculs.vort[1][ARR(i,j,k)] );
				calculs.vort_conf[1][ARR(i,j,k)] = 0.1 * ( calculs.vort_conf[2][ARR(i,j,k)] * calculs.vort[0][ARR(i,j,k)] - calculs.vort_conf[0][ARR(i,j,k)] * calculs.vort[2][ARR(i,j,k)] );
				calculs.vort_conf[2][ARR(i,j,k)] = 0.1 * ( calculs.vort_conf[0][ARR(i,j,k)] * calculs.vort[1][ARR(i,j,k)] - calculs.vort_conf[1][ARR(i,j,k)] * calculs.vort[0][ARR(i,j,k)] );
			}
		}
	}

}

void SolveVelocity ( void )
{
	int i, j, k, l;

	for ( i = 0; i < NDIM; i++ ) {
		AddForce ( calculs.u0[i], calculs.f[i] );
	}

	for ( i = 0; i < NDIM; i++ ) {
		Transport ( calculs.u1[i], calculs.u0[i], calculs.u0 );
	}

	// On se place dans le domaine de Fourier
	for ( i = 0; i < calculs.size; i++ ) {
		for ( j = 0; j < calculs.size; j++ ) {
			for ( k = 0; k < calculs.size; k++ ) {
				for ( l = 0; l < NDIM; l++ ) {
					calculs.u0[l][ARR2(i,j,k)] = calculs.u1[l][ARR(i,j,k)];
				}
			}
		}
	}

	for ( i = 0; i < NDIM; i++ ) {
		rfftwnd_one_real_to_complex ( calculs.plan1, (fftw_real *)calculs.u0[i], NULL );
	}

	DiffuseAndProjectFFT ( calculs.u0, calculs.visc );

	// On revient dans le domaine 'normal'
	for ( i = 0; i < NDIM; i++ ) {
		rfftwnd_one_complex_to_real ( calculs.plan2, (fftw_complex *)calculs.u0[i], NULL );
	}

	// On normalise le resultat
	//( la FFT multiplie tout par 'length[0] * length[1] * length[2]' )
	for ( i = 0; i < calculs.size; i++ ) {
		for ( j = 0; j < calculs.size; j++ ) {
			for ( k = 0; k < calculs.size; k++ ) {
				for ( l = 0; l < NDIM; l++ ) {
					calculs.u1[l][ARR(i,j,k)] = calculs.u0[l][ARR2(i,j,k)] / ( calculs.size * calculs.size * calculs.size );
				}
			}
		}
	}

	VorticityConfinement ( calculs.u1 );

}

inline void SolveScalar ( void )
{
	AddForce ( calculs.s0, calculs.source );
	Transport ( calculs.s1, calculs.s0, calculs.u1 );
	Dissipate ( calculs.s1, calculs.aS );
}

inline void SolveTemperature ( void )
{
	AddForce ( calculs.t0, calculs.chaleur );
	Transport ( calculs.t1, calculs.t0, calculs.u1 );
	Dissipate ( calculs.t1, 0.9 );
}

void calcul ( void )
{
	// Swap grids
	int size = calculs.size * calculs.size * calculs.size * sizeof ( double );
	for ( int k = 0; k < NDIM; k++ ) {
		memcpy ( calculs.u0[k], calculs.u1[k], size );
	}
	memcpy ( calculs.s0, calculs.s1, size );
	memcpy ( calculs.t0, calculs.t1, size );

	// Calcul de la nouvelle vitesse et
	// de la nouvelle quantite de matiere
	SolveVelocity();
	SolveScalar();
	SolveTemperature();
}

void RenderDensity ( void )
{

	if ( globals.bLine ) {
		glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
	} else {
		glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
	}

	for ( int k = 0; k < calculs.size; k++ ) {

		for ( int i = 0; i < calculs.size - 1; i++ ) {

			glBegin ( GL_TRIANGLE_STRIP );

			glColor4f ( (float)calculs.s1[ARR(i,0,k)], 
						(float)calculs.s1[ARR(i,0,k)], 
						(float)calculs.s1[ARR(i,0,k)], 0.5f );

			glVertex3f ( (float)i, 0.0f, (float)k );

			for ( int j = 0; j < calculs.size - 1; j++ ) {

				glColor4f ( (float)calculs.s1[ARR(i+1,j,k)], 
							(float)calculs.s1[ARR(i+1,j,k)], 
							(float)calculs.s1[ARR(i+1,j,k)], 0.5f );

				glVertex3f ( (float)( i + 1 ), (float)j, (float)k );

				glColor4f ( (float)calculs.s1[ARR(i,j+1,k)], 
							(float)calculs.s1[ARR(i,j+1,k)], 
							(float)calculs.s1[ARR(i,j+1,k)], 0.5f );

				glVertex3f ( (float)i, (float)( j + 1 ), (float)k );

			}

			glColor4f ( (float)calculs.s1[ARR(i+1,j,k)], 
						(float)calculs.s1[ARR(i+1,j,k)], 
						(float)calculs.s1[ARR(i+1,j,k)], 0.5f );

			glVertex3f ( (float)i+1, (float)j, (float)k );

			glEnd();

		}
	}

	if ( globals.bDrawBox ) {
		DrawBox();
	}

}

void RenderVelocity ( void )
{
	int i, j, k;

	glDisable ( GL_TEXTURE_2D );

	glBegin ( GL_LINES );
	glColor3f ( 1.0f, 0.0f, 0.0f );

	for ( k = 0; k < calculs.size; k++ ) {
		for ( i = 0; i < calculs.size; i++ ) {
			for ( j = 0; j < calculs.size; j++ ) {
				glVertex3f ( (float)i, (float)j, (float)k );
				glVertex3f ( i + 1000*(float)calculs.u1[0][ARR(i,j,k)],
							 j + 1000*(float)calculs.u1[1][ARR(i,j,k)],
							 k + 1000*(float)calculs.u1[2][ARR(i,j,k)] );
			}
		}
	}

	glEnd();

}

void RenderScene ( void )
{
	glDisable ( GL_TEXTURE_2D );

	glLoadIdentity();

	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective ( 45.0f, (float)globals.width/(float)globals.height, 0.1f, 10000.0f );

	glMatrixMode ( GL_MODELVIEW );
	gluLookAt ( render.eyeDist * (float) sin ( render.eyeAngle ) * (float) cos ( render.eyeHeight ) + (float)( calculs.size - 1 ) / 2.0f,
				render.eyeDist * (float) sin ( render.eyeHeight ) + (float)( calculs.size - 1 ) / 2.0f,
				render.eyeDist * (float) cos ( render.eyeAngle ) * (float) cos ( render.eyeHeight ) + (float)( calculs.size - 1 ) / 2.0f,
				(float)( calculs.size - 1 ) / 2.0f,
				(float)( calculs.size - 1 ) / 2.0f,
				(float)( calculs.size - 1 ) / 2.0f,
				0.0f, 1.0f, 0.0f );

	RenderDensity();
	if ( globals.bVelocity ) {
		RenderVelocity();
	}
	//RenderGround();
}

void HandleKeyboard ( void )
{

	if ( render.keys[ VK_UP ] ) {
		render.eyeHeight += 0.2f;
	}
	if ( render.keys[ VK_DOWN ] ) {
		render.eyeHeight -= 0.2f;
	}
	if ( render.keys[ VK_LEFT ] ) {
		render.eyeAngle += 0.2f;
	}
	if ( render.keys[ VK_RIGHT ] ) {
		render.eyeAngle -= 0.2f;
	}
	if ( render.keys[ VK_PRIOR ] ) {
		render.eyeDist += 1.0f;
	}
	if ( render.keys[ VK_NEXT ] ) {
		render.eyeDist -= 1.0f;
	}

	BuildPerspectiveMatrix ( render.lightFrustumMatrix, render.lightFieldOfView, 1.0f, 0.0f, 0.0f );
  
}

void DrawBox ( void )
{
	glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
	glColor3f ( 1.0f, 1.0f, 1.0f );

	glLoadName ( 0 );
	glBegin ( GL_POLYGON );
	glVertex3f ( 0.0f, 0.0f, 0.0f );
	glVertex3f ( 0.0f, (float)( calculs.size - 1 ), 0.0f );
	glVertex3f ( (float)( calculs.size - 1 ), (float)( calculs.size - 1 ), 0.0f );
	glVertex3f ( (float)( calculs.size - 1 ), 0.0f, 0.0f );
	glEnd();

	glLoadName ( 1 );
	glBegin ( GL_POLYGON );
	glVertex3f ( 0.0f, 0.0f, (float)( calculs.size - 1 ) );
	glVertex3f ( 0.0f, (float)( calculs.size - 1 ), (float)( calculs.size - 1 ) );
	glVertex3f ( (float)( calculs.size - 1 ), (float)( calculs.size - 1 ), (float)( calculs.size - 1 ) );
	glVertex3f ( (float)( calculs.size - 1 ), 0.0f, (float)( calculs.size - 1 ) );
	glEnd();

	glLoadName ( 2 );
	glBegin ( GL_POLYGON );
	glVertex3f ( 0.0f, 0.0f, 0.0f );
	glVertex3f ( 0.0f, (float)( calculs.size - 1 ), 0.0f );
	glVertex3f ( 0.0f, (float)( calculs.size- 1 ), (float)( calculs.size - 1 ) );
	glVertex3f ( 0.0f, 0.0f, (float)( calculs.size - 1 ) );
	glEnd();

	glLoadName ( 3 );
	glBegin ( GL_POLYGON );
	glVertex3f ( (float)( calculs.size - 1 ), 0.0f, 0.0f );
	glVertex3f ( (float)( calculs.size - 1 ), (float)( calculs.size - 1 ), 0.0f );
	glVertex3f ( (float)( calculs.size - 1 ), (float)( calculs.size - 1 ), (float)( calculs.size - 1 ) );
	glVertex3f ( (float)( calculs.size - 1 ), 0.0f, (float)( calculs.size - 1 ) );
	glEnd();

	glLoadName ( 4 );
	glBegin ( GL_POLYGON );
	glVertex3f ( 0.0f, 0.0f, 0.0f );
	glVertex3f ( 0.0f, 0.0f, (float)( calculs.size - 1 ) );
	glVertex3f ( (float)( calculs.size - 1 ), 0.0f, (float)( calculs.size - 1 ) );
	glVertex3f ( (float)( calculs.size - 1 ), 0.0f, 0.0f );
	glEnd();

	glLoadName ( 5 );
	glBegin ( GL_POLYGON );
	glVertex3f ( 0.0f, (float)( calculs.size - 1 ), 0.0f );
	glVertex3f ( 0.0f, (float)( calculs.size - 1 ), (float)( calculs.size - 1 ) );
	glVertex3f ( (float)( calculs.size - 1 ), (float)( calculs.size - 1 ), (float)( calculs.size - 1 ) );
	glVertex3f ( (float)( calculs.size - 1 ), (float)( calculs.size - 1 ), 0.0f );
	glEnd();

}

void MakeSources ( void )
{
	static int cnt = 0;
	int i = calculs.size / 2;
	int j = 0;
	int k = 0; //calculs.size / 2;
	cnt++;

	if ( cnt == 25 ) {
		cnt = 0;

		calculs.source[ARR(i, j, k)] = 20.0f;
		
		calculs.f[0][ARR(i,j,k)] = (double)rand() / ( 4.0 * RAND_MAX );
		calculs.f[1][ARR(i,j,k)] = 0.8;
		calculs.f[2][ARR(i,j,k)] = (double)rand() / ( 4.0 * RAND_MAX );

		calculs.f[0][ARR(i,j+1,k)] = (double)rand() / ( 4.0 * RAND_MAX );
		calculs.f[1][ARR(i,j+1,k)] = 0.6;
		calculs.f[2][ARR(i,j+1,k)] = (double)rand() / ( 4.0 * RAND_MAX );
	}

}

void MakeBallSource ( void )
{
	const int ci = 8;
	const int cj = 6;
	const int ck = 15;
	const int r  = 3;

	for ( int i = 0; i < calculs.size; i++ ) {
		for ( int j = 0; j < calculs.size; j++ ) {
			for ( int k = 0; k < calculs.size; k++ ) {
				if ( idist ( ci, cj, ck, i, j, k ) <= r ) {
					calculs.source[ARR(i,j,k)]  = 1.0;
					calculs.chaleur[ARR(i,j,k)] = 0.8;
					calculs.f[0][ARR(i,j,k)] = 3; //( 3.0 * rand() ) / ( 1.0 * RAND_MAX );
				}
				calculs.f[1][ARR(i,j,k)] += -0.1 * calculs.s1[ARR(i,j,k)] + 0.5 * calculs.t1[ARR(i,j,k)];

				calculs.f[0][ARR(i,j,k)] += ( rand() - RAND_MAX/2 ) / ( 6.0 * RAND_MAX );
				calculs.f[1][ARR(i,j,k)] += ( rand() - RAND_MAX/2 ) / ( 6.0 * RAND_MAX );
				calculs.f[2][ARR(i,j,k)] += ( rand() - RAND_MAX/2 ) / ( 6.0 * RAND_MAX );

			}
		}
	}
}

