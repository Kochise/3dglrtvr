
/*
 * This program uses the fluid dynamics solver presented
 * in Jos Stam, "A Simple Fluid Solver Based on the FFT",
 * (http://www.dgp.utoronto.ca/people/stam/reality/Research/pub.html)
 * to produce a impression of swirly smoke in real-time.
 * 
 * To compile this program, you need "The Fastest Forurier 
 * Transform in the West", i.e., the fftw and rfftw libraries 
 * and header files. They can be downloaded from 
 * http://www.fftw.org.
 *
 * Author: Gustav Taxen, gustavt@nada.kth.se
 * Portions of the code by Jos Stam, from the paper
 * "A Simple Fluid Solver Based on the FFT", available at
 * http://www.dgp.utoronto.ca/people/stam/reality/Research/pub.html
 *
 * NOTE: Stam's FFT method is protected by a patent owned by
 * Alias (previously Alias/Wavefront) - use the method at your
 * own risk!
 *
 * USAGE:
 *
 * Drag with the mouse to add smoke to the fluid. This will
 * also move a "rotor" that disturbs the velocity field to
 * the mouse location.
 *
 * 'w' toggles wireframe rendering.
 * 'v' toggles rendering of velocity vectors. 
 * 'T' and 't' increases and decreases the simulation time step, 
 * respectively.
 *
 * NOTE: The parameters I've chosen for the simulation are
 * targeted at a Pentium III 850MHz laptop with a ATI Rage 
 * Mobility M4 graphics card. You may have to change them to
 * suit your machine (see below).
 *
 */

#ifdef WIN32
#include <windows.h>
#endif

#include <math.h>

#include <rfftw.h>
#include <GL/glut.h>

/*
 * Size of the simulation grid, i.e. the number of grid
 * points are (DIM x DIM).
 *
 */
#define DIM 50
#define ARRAYSIZE (DIM * 2*(DIM/2+1))

/* 
 * Simulation time step: you may need to change this to suit
 * your particular machine speed.
 *
 */
double dt = 0.4;


int    winWidth, winHeight;
int    drawVelocities = 0;
int    wireframe = 0;
int    lmx, lmy;

double t = 0;

/* 
 * See Jos Stam, "A Simple Fluid Solver Based on the FFT",
 * http://www.dgp.utoronto.ca/people/stam/reality/Research/pub.html
 * for more details on the following code.
 *
 */

fftw_real u[DIM * 2*(DIM/2+1)], v[DIM * 2*(DIM/2+1)], u0[DIM * 2*(DIM/2+1)], v0[DIM * 2*(DIM/2+1)];
fftw_real rho[DIM * DIM], rho0[DIM * DIM];  /* Smoke density */
fftw_real u_u0[DIM * DIM], u_v0[DIM * DIM];  /* User-induced forces */

static rfftwnd_plan plan_rc, plan_cr;

void init_FFT(int n) {
	plan_rc = rfftw2d_create_plan(n, n, FFTW_REAL_TO_COMPLEX, FFTW_IN_PLACE);
	plan_cr = rfftw2d_create_plan(n, n, FFTW_COMPLEX_TO_REAL, FFTW_IN_PLACE);
}

#define FFT(s,u)\
	if(s==1) rfftwnd_one_real_to_complex(plan_rc,(fftw_real *)u,(fftw_complex*)u);\
	else rfftwnd_one_complex_to_real(plan_cr,(fftw_complex *)u,(fftw_real *)u)

#define floor(x) ((x)>=0.0?((int)(x)):(-((int)(1-(x)))))

void stable_solve ( int n, fftw_real * u, fftw_real * v, fftw_real * u0, fftw_real * v0, fftw_real visc, fftw_real dt ) {
	fftw_real x, y, x0, y0, f, r, U[2], V[2], s, t;
	int i, j, i0, j0, i1, j1;

	for ( i=0 ; i<n*n ; i++ ) {
		u[i] += dt*u0[i]; 
		u0[i] = u[i];
		
		v[i] += dt*v0[i]; 
		v0[i] = v[i];
  }    

	for ( x=0.5f/n,i=0 ; i<n ; i++,x+=1.0f/n ) {
		for ( y=0.5f/n,j=0 ; j<n ; j++,y+=1.0f/n ) {
			x0 = n*(x-dt*u0[i+n*j])-0.5f; 
			y0 = n*(y-dt*v0[i+n*j])-0.5f;
			i0 = floor(x0);
			s = x0-i0;
			i0 = (n+(i0%n))%n;
			i1 = (i0+1)%n;
			j0 = floor(y0);
			t = y0-j0;
			j0 = (n+(j0%n))%n;
			j1 = (j0+1)%n;
			u[i+n*j] = (1-s)*((1-t)*u0[i0+n*j0]+t*u0[i0+n*j1])+                        
				s *((1-t)*u0[i1+n*j0]+t*u0[i1+n*j1]);
			v[i+n*j] = (1-s)*((1-t)*v0[i0+n*j0]+t*v0[i0+n*j1])+
				s *((1-t)*v0[i1+n*j0]+t*v0[i1+n*j1]);
		}    
	} 
	
	for ( i=0 ; i<n ; i++ )
		for ( j=0 ; j<n ; j++ ) { 
			u0[i+(n+2)*j] = u[i+n*j]; 
		  v0[i+(n+2)*j] = v[i+n*j];
		}

	FFT(1,u0);
	FFT(1,v0);

	for ( i=0 ; i<=n ; i+=2 ) {
		x = 0.5f*i;
		for ( j=0 ; j<n ; j++ ) {
			y = j<=n/2 ? (fftw_real)j : (fftw_real)j-n;
			r = x*x+y*y;
			if ( r==0.0f ) continue;
			f = (fftw_real)exp(-r*dt*visc);
			U[0] = u0[i  +(n+2)*j]; V[0] = v0[i  +(n+2)*j];
			U[1] = u0[i+1+(n+2)*j]; V[1] = v0[i+1+(n+2)*j];

			u0[i  +(n+2)*j] = f*( (1-x*x/r)*U[0]     -x*y/r *V[0] );
			u0[i+1+(n+2)*j] = f*( (1-x*x/r)*U[1]     -x*y/r *V[1] );
			v0[i+  (n+2)*j] = f*(   -y*x/r *U[0] + (1-y*y/r)*V[0] );
			v0[i+1+(n+2)*j] = f*(   -y*x/r *U[1] + (1-y*y/r)*V[1] );
		}    
	}

	FFT(-1,u0); 
	FFT(-1,v0);

	f = 1.0/(n*n);
 	for ( i=0 ; i<n ; i++ )
		for ( j=0 ; j<n ; j++ ) {
			u[i+n*j] = f*u0[i+(n+2)*j]; 
			v[i+n*j] = f*v0[i+(n+2)*j]; 
		}
} 


/*
 * This function diffuses matter that has been placed
 * in the velocity field. It's almost identical to the
 * velocity diffusion step in the function above. The
 * input matter densities are in rho0 and the result
 * is written into rho.
 *
 */
void diffuse_matter(int n, fftw_real *u, fftw_real *v, fftw_real *rho, fftw_real *rho0, fftw_real dt) {
	fftw_real x, y, x0, y0, s, t;
	int i, j, i0, j0, i1, j1;

	for ( x=0.5f/n,i=0 ; i<n ; i++,x+=1.0f/n ) {
		for ( y=0.5f/n,j=0 ; j<n ; j++,y+=1.0f/n ) {
			x0 = n*(x-dt*u[i+n*j])-0.5f; 
			y0 = n*(y-dt*v[i+n*j])-0.5f;
			i0 = floor(x0);
			s = x0-i0;
			i0 = (n+(i0%n))%n;
			i1 = (i0+1)%n;
			j0 = floor(y0);
			t = y0-j0;
			j0 = (n+(j0%n))%n;
			j1 = (j0+1)%n;
			rho[i+n*j] = (1-s)*((1-t)*rho0[i0+n*j0]+t*rho0[i0+n*j1])+                        
				s *((1-t)*rho0[i1+n*j0]+t*rho0[i1+n*j1]);
		}    
	} 
}

/*
 * Draw the fluid using triangle strips.
 *
 */
void drawField(void) {
	int        i, j, idx;
	fftw_real  wn = (fftw_real)winWidth / (fftw_real)(DIM + 1);   /* Grid element width */
	fftw_real  hn = (fftw_real)winHeight / (fftw_real)(DIM + 1);  /* Grid element height */
	double     px, py;

	if (wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	for (j = 0; j < DIM - 1; j++) {
		glBegin(GL_TRIANGLE_STRIP);

		i = 0;
		px = wn + (fftw_real)i * wn;
		py = hn + (fftw_real)j * hn;
		idx = (j * DIM) + i;
		glColor3f(rho[idx], rho[idx], rho[idx]);
		glVertex2f(px, py);
						
		for (i = 0; i < DIM - 1; i++) {
			px = wn + (fftw_real)i * wn;
			py = hn + (fftw_real)(j + 1) * hn;
			idx = ((j + 1) * DIM) + i;
			glColor3f(rho[idx], rho[idx], rho[idx]);
			glVertex2f(px, py);

			px = wn + (fftw_real)(i + 1) * wn;
			py = hn + (fftw_real)j * hn;
			idx = (j * DIM) + (i + 1);
			glColor3f(rho[idx], rho[idx], rho[idx]);
			glVertex2f(px, py);
		}

		px = wn + (fftw_real)(DIM - 1) * wn;
		py = hn + (fftw_real)(j + 1) * hn;
		idx = ((j + 1) * DIM) + (DIM - 1);
		glColor3f(rho[idx], rho[idx], rho[idx]);
		glVertex2f(px, py);

		glEnd();
	}

	if (drawVelocities) {
		glBegin(GL_LINES);
		for (i = 0; i < DIM; i++) {
			for (j = 0; j < DIM; j++) {
				idx = (j * DIM) + i;
				glColor3f(1, 0, 0);
				glVertex2f(wn + (fftw_real)i * wn, hn + (fftw_real)j * hn);
				glVertex2f((wn + (fftw_real)i * wn) + 1000 * u[idx], (hn + (fftw_real)j * hn) + 1000 * v[idx]);
			}
		}
		glEnd();
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawField();

	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h) {
 	glViewport(0.0f, 0.0f, (GLfloat)w, (GLfloat)h);
  glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
	winWidth = w;
	winHeight = h;
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'v':
			drawVelocities = 1 - drawVelocities;
			break;
		case 'w':
			wireframe = 1 - wireframe;
			break;
		case 't':
			dt -= 0.001;
			printf("dt = %f\n", dt);
			break;
		case 'T':
			dt += 0.001;
			printf("dt = %f\n", dt);
			break;
	}
}


void init(void) {
	int i;

	init_FFT(DIM);

	for (i = 0; i < DIM * DIM; i++) {
		u[i] = v[i] = u0[i] = v0[i] = rho[i] = rho0[i] = u_u0[i] = u_v0[i] = 0.0f;
	}
}


/*
 * When the user drags with the mouse, add a force
 * that corresponds to the direction of the mouse
 * cursor movement. Also inject som new matter into
 * the field at the mouse location.
 *
 */
void drag(int mx, int my) {
	int     xi;
	int     yi;
	double  dx, dy, len;
	int     X, Y;

	/* Compute the array index that corresponds to the
	 * cursor location */

	xi = (int)floor((double)(DIM + 1) * ((double)mx / (double)winWidth));
	yi = (int)floor((double)(DIM + 1) * ((double)(winHeight - my) / (double)winHeight));

	X = xi;
	Y = yi;

	if (X > (DIM - 1)) {
		X = DIM - 1;
	}
	if (Y > (DIM - 1)) {
		Y = DIM - 1;
	}
	if (X < 0) {
		X = 0;
	}
	if (Y < 0) {
		Y = 0;
	}

	/* Add force at the cursor location */

	my = winHeight - my;
	dx = mx - lmx;
	dy = my - lmy;
	len = sqrt(dx * dx + dy * dy);
  if (len != 0.0) { 
    dx *= 0.1 / len;
    dy *= 0.1 / len;
  }
	u_u0[Y * DIM + X] += dx;
	u_v0[Y * DIM + X] += dy;

	/* Increase matter density at the cursor location */

	rho[Y * DIM + X] = 10.0f;

	lmx = mx;
	lmy = my;
}


/*
 * Copy user-induced forces to the force vectors
 * that is sent to the solver. Also dampen forces and
 * matter density.
 *
 */
void setForces(void) {
	int i;
	for (i = 0; i < DIM * DIM; i++) {
		rho0[i] = 0.995 * rho[i];

		u_u0[i] *= 0.85;
		u_v0[i] *= 0.85;

		u0[i] = u_u0[i];
		v0[i] = u_v0[i];
	}
}

/*
 * Update the simulation when we're not drawing.
 *
 */
void idle(void) {
	setForces();
	stable_solve(DIM, u, v, u0, v0, 0.001, dt);
	diffuse_matter(DIM, u, v, rho, rho0, dt);

	t += dt;
	
	glutPostRedisplay();
}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Real-time wispy smoke");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(drag);
	
	init();
	
	glutMainLoop();
	return 0;
}

