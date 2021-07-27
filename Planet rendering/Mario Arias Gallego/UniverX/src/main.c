#include "supergl.h"
#include <stdlib.h>
#include <string.h>
#include <sys/time.h> 

#include "constantes.h"
#include "geometria.h"
#include "orbit.h"
#include "texturas.h"
#include "stars.h"
#include "texto.h"
#include "shader.h"

double time_old=0.0;
double atm=1.0,year=1.0;

// Par‡metros
int orbitando = 1;
int rotando = 1;
int plano = 0;
int atmosfera = 1;
int fondo = 1;
int ejes = 0;
int full_screen = 0;
int etiquetas = 1;
int seguir_planeta = -1; // No seguir a ninguno
int mostrarfps=1;
int animacion=1;
int shaders=0;

int fps=0;

//Mensajes
char fps_str[10]="";
char fecha_str[20]="";
char tmp_str[100]="";

char mensaje[100]="";
long msg_timestamp =0;

// Posición de la c‡mara
float camx = 0.0f;
float camy = 0.0f;
float camz = 0.0f;
float zoom = 16.0;
float nextzoom=16.0;
float minzoom[] = { 0.4,  0.09 , 0.125, 0.12, 0.09 , 0.965, 0.77, 0.32, 0.31, 0.08, 1.0 };
float gotozoom[] = { 0.63, 0.105 , 0.125, 0.125, 0.09 , 1.38, 1.705, 0.55, 0.52, 0.08, 1.0 };
float rotx = -25.0;
float roty = 0.0f;

float luzx = 0.0;
float luzy = 0.0;
float luzz = 0.0;

// Posición del raton
int lastx=0;
int lasty=0;
unsigned char Buttons[3] = {0};
int modifier=0;
int mainmenu=0;

// Escala
int escala_planetas=1000.0;
int escala_estrella=60.0;
double escala_tiempo=0.001;


/* Posicion y valor de las luces */
// Luz del Sol
GLfloat luz_specular0[] = { C0S, C0S, C0S, 1.0 };
GLfloat luz_difuso0[] = { C0D, C0D, C0D, 1.0 };
GLfloat luz_ambiente0[] = { C0A, C0A, C0A, 1.0 };
GLfloat luz_pos0[] = { 0 , 0, 0, 1.0 }; 

GLfloat luz_specular3[] = { C3S, C3S, C3S, 1.0 };
GLfloat luz_difuso3[] = { C3D, C3D, C3D, 1.0 };
GLfloat luz_ambiente3[] = { C3A, C3A, C3A, 1.0 };
GLfloat luz_pos3[] = { 0 , 0, 0, 1.0 }; 


// Luz blanca direccional desde la camara
GLfloat luz_specular1[] = { C1S, C1S, C1S, 1.0 };
GLfloat luz_difuso1[] = { C1D, C1D, C1D, 1.0 };
GLfloat luz_ambiente1[] = { C1A, C1A, C1A, 1.0 };
GLfloat luz_pos1[] = { 0, 0, DIST , 1.0 };

GLfloat luz_specular2[] = { C2S, C2S, C2S, 1.0 };
GLfloat luz_difuso2[] = { C2D, C2D, C2D, 1.0 };
GLfloat luz_ambiente2[] = { C2A, C2A, C2A, 1.0 };
GLfloat luz_pos2[] = { 0, 0, DIST, 1.0 };

// Materiales
// Planetas
GLfloat specular_planetas[] = { MATPLANETSPE, MATPLANETSPE, MATPLANETSPE, 1.0};
GLfloat diffuse_planetas[] = { MATPLANETDIF, MATPLANETDIF, MATPLANETDIF, 1.0};
GLfloat ambient_planetas[] = { MATPLANETAMB, MATPLANETAMB, MATPLANETAMB, 1.0};

// Sol
GLfloat specular_sol[] = { MATSOLSPE, MATSOLSPE, MATSOLSPE, 1.0};
GLfloat diffuse_sol[] = { MATSOLDIF, MATSOLDIF, MATSOLDIF, 1.0};
GLfloat ambient_sol[] = { MATSOLAMB, MATSOLAMB, MATSOLAMB, 1.0};


// Inicializar sistema
void init(void) 
{
	// Shader
	//if( InicializaShaders()) exit(1);
	InicializaShaders();
	
	// Cargar información de planetas
	cargarPlanetas();
	setIncT(1.024);

	// Cargar texturas
	CargarTexturas();

	// Configurar características
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel (GL_SMOOTH);
	glClearColor (0.0, 0.0, 0.0, 0.0);

	// Transparencia
	glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Antialiasing para lineas
	//glEnable(GL_LINE_SMOOTH);
	//glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

	// Luces
	glLightfv(GL_LIGHT0,GL_DIFFUSE,luz_difuso0);
	glLightfv(GL_LIGHT0,GL_SPECULAR,luz_specular0);
	glLightfv(GL_LIGHT0,GL_AMBIENT,luz_ambiente0);
	glLightfv(GL_LIGHT0, GL_POSITION, luz_pos0); 

	glLightfv(GL_LIGHT3,GL_DIFFUSE,luz_difuso3);
	glLightfv(GL_LIGHT3,GL_SPECULAR,luz_specular3);
	glLightfv(GL_LIGHT3,GL_AMBIENT,luz_ambiente3);
	glLightfv(GL_LIGHT3, GL_POSITION, luz_pos3);

	glLightfv(GL_LIGHT1,GL_DIFFUSE,luz_difuso1);
	glLightfv(GL_LIGHT1,GL_SPECULAR,luz_specular1);
	glLightfv(GL_LIGHT1,GL_AMBIENT,luz_ambiente1);
	glLightfv(GL_LIGHT1, GL_POSITION, luz_pos1); 

	glLightfv(GL_LIGHT2,GL_DIFFUSE,luz_difuso2);
	glLightfv(GL_LIGHT2,GL_SPECULAR,luz_specular2);
	glLightfv(GL_LIGHT2,GL_AMBIENT,luz_ambiente2);
	glLightfv(GL_LIGHT2, GL_POSITION, luz_pos2); 

	// Activar luces
	glEnable(GL_LIGHT0); 
	glEnable(GL_LIGHT1); 
	glEnable(GL_LIGHTING); 
	
	// Wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


// Colocar la c‡mara
void camara()
{
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt ( 0,0, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void multi( float *x, float *y, float *z) {
        float mat[16];

        glGetFloatv(GL_MODELVIEW_MATRIX,mat);

        *x = mat[0] * (*x) + mat[4]* (*y) + mat[8]*(*z) + mat[12];
        *y = mat[1] * (*x) + mat[5]* (*y) + mat[9]*(*z) + mat[13];
        *z = mat[2] * (*x) + mat[6]* (*y) + mat[10]*(*z)+ mat[14];
}



// Mostrar la escena!
void display(void)
{
	int i; float textdist; 

	fps++;

	// Borrar buffers de color y profundidad
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Colocar la c‡mara
	camara();

	// Colocar luces desde la c‡mara
	glLightfv(GL_LIGHT1, GL_POSITION, luz_pos1); 
	glLightfv(GL_LIGHT2, GL_POSITION, luz_pos2); 
	NormalVShaderSetLightPos( luz_pos1[0], luz_pos1[1], luz_pos1[2]);

	glPushMatrix();

	// Rotar la camara 
	glRotatef(-rotx,1,0,0);
	glRotatef(roty,0,1,0);
	glTranslatef( -camx, -camz, -camy );

	// Colocar luces desde el Sol
	glLightfv(GL_LIGHT0, GL_POSITION, luz_pos0); 
	glLightfv(GL_LIGHT3, GL_POSITION, luz_pos3); 



	// Dibujar plano base
	glDisable(GL_LIGHTING); 
	if(plano) {
		glColor3f (1.0, 1.0, 1.0);
		dibuja_cuadrado();
	}
	
	// Dibujar ejes para situarse
	//dibujaEjes( 1.0 );


	// Dibujar fondo
	if( fondo ) { 
		glBindTexture(GL_TEXTURE_2D, 0);   // Quitar texturas
		if(shaders) ActivateStarVShader();
		drawStars();
		if(shaders) DeactivateStarVShader();
	}
	glEnable(GL_LIGHTING); 


	// Establecer el material del sol
	glEnable(GL_LIGHT2); 
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular_sol);
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse_sol);
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient_sol);
	glBindTexture(GL_TEXTURE_2D, textura[20]);

	glPushMatrix();
	glScalef( TRIP(getStarRadius()*escala_estrella) );

	muestraEsfera();
	glPopMatrix();
	glDisable(GL_LIGHT2); 


	// Dibujar eje de rotación del Sol 
	if(ejes) {
		glDisable(GL_LIGHTING); 
		dibuja_eje_rotacion(LONG_EJE * getStarRadius()*escala_estrella );
		glEnable(GL_LIGHTING); 
	}

	// Establecer el material de los planetas
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular_planetas);
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse_planetas);
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient_planetas);

	// dibujar planetas 
	for (i=0; i<getNumPlanetas();i++) {
		glPushMatrix();

		glTranslatef (getX(i), getZ(i), getY(i)); // Traslacion
		
		if(!shaders) glRotatef (24, 0.0, 0.0, 1.0);		  // Inclinacion
		if(!shaders) glRotatef (getAng(i), 0.0, 1.0, 0.0);		  // angulo de rotacion. 
		
		glScalef( TRIP(getPlanetRadius(i)*escala_planetas) );	  // Tama–o del planeta

		if(getPlanetRadius(i)> MIN_PLANETRAD_LIGHT3) {
			glEnable(GL_LIGHT3);
		} 

		glBindTexture(GL_TEXTURE_2D, textura[ i ]);

#if 1
		glPushMatrix();
		glLoadIdentity();
		luzx = luzy = luzz = 0.0;
		//glRotatef (-getAng(i), 0.0, 1.0, 0.0);		  // angulo de rotacion. 
		//glRotatef (-24, 0.0, 0.0, 1.0);		  // angulo de rotacion. 
		glTranslatef (-getX(i), -getZ(i), -getY(i)); // Traslacion
		//muestraEsfera();
		
		multi(&luzx, &luzy, &luzz);
		luzx = luzx / (getPlanetRadius(i)*escala_planetas);
		luzy = luzy / (getPlanetRadius(i)*escala_planetas);
		luzz = luzz / (getPlanetRadius(i)*escala_planetas);
		//if(i==seguir_planeta) printf("Luz en %f: %f, %f, %f,\n",day, luzx, luzy, luzz);
                NormalVShaderSetLightPos( luzx, luzy, luzz);
		glPopMatrix();
#endif

		if(shaders && (i<4)) ActivateNormalPShader(textura[i], textura[i+10], 2, 2, 2);
		if(shaders && (i<4)) ActivateNormalVShader();

		muestraEsfera();

		if(shaders && (i<4)) DeactivateNormalVShader();
		if(shaders && (i<4)) DeactivateNormalPShader();
		
		if(ejes) {
			glDisable(GL_LIGHTING); 
			dibuja_eje_rotacion( 1.5) ;
			glEnable(GL_LIGHTING); 
		}


		if( (i==2) && atmosfera ) // Tierra: Atmosfera
		{
			glRotatef ( atm, 0.0, 1.0, 0.0);
			glScalef ( TRIP( 1.01 ) );
			glBindTexture(GL_TEXTURE_2D, textura[ 21 ]);
			
			muestraEsfera();
		}
		if(i==5) // Saturno: Anillos
		{
			glBindTexture(GL_TEXTURE_2D, textura[ 22 ]);
			dibujarAnillos(N_ANILLOS, ANILLOS_RADIO_INT, ANILLOS_RADIO_EXT );
		}

		if(getPlanetRadius(i)> MIN_PLANETRAD_LIGHT3) {
			glDisable(GL_LIGHT3);
		}

		glPopMatrix();
	}

	glPopMatrix();

	// Mostrar etiquetas de cada planeta
	if(etiquetas) {
		for (i=0; i<getNumPlanetas();i++) {
			glPushMatrix();

			// Distancia de la etiqueta respecto al centro del planeta.
			textdist = getPlanetRadius(i)*escala_planetas*1.1+0.5*zoom/MAX_ZOOM;
			glTranslatef(textdist, 0, 0 );

			// Rotación de la c‡mara
			glRotatef(-rotx, 1, 0, 0);
			glRotatef( roty, 0, 1, 0);

			// Seguir planeta;
			glTranslatef( -camx, -camz, -camy);

			// Traslacion
			glTranslatef (getX(i),getZ(i),getY(i));		

			// Dibujar texto
			glBindTexture(GL_TEXTURE_2D, textura[i]);
			texto( getNombre(i), 0,0,0  );

			glPopMatrix();
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);   // Quitar textura

	// Mostrar FPS
	if(mostrarfps) {
	   texto_encima( fps_str, TEXT_POS(-30), TEXT_POS(27), 0);
	}

	// Mostrar fecha
	texto_encima( fecha_str, TEXT_POS(20), TEXT_POS(-27), 0);

	// Mostrar último mensaje:
	if(time(NULL)-msg_timestamp<CADUCA_MENSAJE) {
	   texto_encima( mensaje, TEXT_POS(-30), TEXT_POS(-27), 0);
	}

	glutSwapBuffers();
}


void Orbitar () {
	int i;
	static int lastime;
	float camfactor;
	//double t;


	/*
	struct timeval tv;
	struct timezone tz;
	struct tm *tm;
	*/
	if(time(NULL)-lastime>2) {
		sprintf(fps_str,"FPS: %d",fps/2);
		lastime = time(NULL);
		fps=0;
	}
  
	for (i=0; i<getNumPlanetas(); i++) {
	 	if(orbitando)
			mover(i);
		
	 	if(rotando) {
			incAng(i);
			if(atm>360) atm-=360;
	 	}
			atm+=0.07;
	}	 
	sprintf( fecha_str, "Fecha: %.0f", getTiempo()/22000+2007.0 );

	if(animacion) {
		camfactor = getIncT()/60;
		if(camfactor<0.135) camfactor=0.135;
		
		// No animar porque va muy rapido
		if(camfactor>=1.0) {
				camx = getX(seguir_planeta);
				camy = getY(seguir_planeta);
				camz = getZ(seguir_planeta);
		} else {
				// animar
				camx -= (camx - getX(seguir_planeta))*camfactor;
				camy -= (camy - getY(seguir_planeta))*camfactor;
				camz -= (camz - getZ(seguir_planeta))*camfactor;
		}
		
		// Animar zoom
		zoom -= (zoom - nextzoom)*0.135;
	} else {
		camx = getX(seguir_planeta);
		camy = getY(seguir_planeta);
		camz = getZ(seguir_planeta);
	}

	glutPostRedisplay();
}


void redimensionar(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 0.05, 1000.0);

	camara();
}


void camaraPredeterminada(int camara) {
	switch(camara) {
		case 0:
			MENSAJE("F1) Vista global 1");
			seguir_planeta=-1;
			nextzoom = 2;
			rotx = 0;
			roty = 0;
			break;
		case 1:
			MENSAJE("F2) Vista global 2");
			rotx = -27.5;
			roty = -222;
			nextzoom = 2.69;
			seguir_planeta = 0;
			break;
		case 2:
			MENSAJE("F3) Vista de la Tierra");
			rotx = -14.0;
			roty = -171.5;
			nextzoom = 0.145;
			seguir_planeta = 2;
			break;
		case 3:
			MENSAJE("F4) Vista de Saturno");
			rotx = -16.0;
			roty = 12.5;
			nextzoom = 1.705;
			seguir_planeta = 5;
			break;
		case 4:
			MENSAJE("F5) Vista desde el Sol");
			rotx = -3.5;
			roty = 92.0;
			nextzoom = 0.06;
			seguir_planeta = -1;
			break;
	}
}


void teclado(unsigned char key, int key_x, int key_y)
{
	char *captura;

	switch (key) {
	case 'p': 
		if (escala_planetas > 1) {
			escala_planetas-=100;
			MENSAJE( "p) Tama–o Planetas: Aumentados %d00 veces",escala_planetas);	   
		}
		if (escala_planetas <= 1) {
			escala_planetas=1;
			MENSAJE( "p) Tama–o Planetas: ESCALA REAL");
		}
		glutPostRedisplay();
		break;

	case 'P': 
		escala_planetas+=100;
		MENSAJE("p) Tama–o Planetas: Aumentados %d00 veces",escala_planetas);	   
		glutPostRedisplay();
		break;
	case 's': 
		if (escala_estrella >1) {
			escala_estrella-=10;
			MENSAJE("s) Tama–o Sol: Aumentado %d00 veces",escala_estrella);	   
		}
		if (escala_estrella <= 1) {
			escala_estrella=1;
			MENSAJE("s) Tama–o Sol: ESCALA REAL");
		}
		glutPostRedisplay();
		break;

	case 'S': 
		escala_estrella+=10;
		MENSAJE("s) Tama–o Sol: Aumentados %d00 veces",escala_estrella);	   
		glutPostRedisplay();
		break;

	case 'T':
		escala_tiempo = getIncT() * 2.0;
		if(escala_tiempo>8388.608)
			escala_tiempo=8388.608;
		setIncT(escala_tiempo);
		MENSAJE("t) Escala de tiempos: %.3f",escala_tiempo);
		glutPostRedisplay();
		break;
	case 't':
		escala_tiempo = getIncT() / 2.0;
		if(escala_tiempo<0.001)
			   escala_tiempo=0.001;
		setIncT(escala_tiempo);
		MENSAJE("t) Escala de tiempos: %.3f",escala_tiempo);
		glutPostRedisplay();
		break;

	case 'e': 
		shaders= !shaders;
		MENSAJE("e) Shaders: %s", ACTIVO(shaders));
		break;
	case 'a': 
		atmosfera = !atmosfera;
		MENSAJE("a) Atmosfera terrestre: %s", ACTIVO(atmosfera));
  		glutChangeToMenuEntry(4,mensaje,4);
		break;
	case 'z': 
		plano = !plano;
		MENSAJE("z) Plano de referencia ( 1 cuadro = 1UA ): %s", ACTIVO(plano));
  		glutChangeToMenuEntry(5,mensaje,5);
		break;
	case 'x':
		ejes = !ejes;
		MENSAJE("x) Ejes de rotacion: %s", ACTIVO(ejes));
  		glutChangeToMenuEntry(6,mensaje,6);
		break;

	case 'g':
		mostrarfps = !mostrarfps;
		MENSAJE("g) Visualizar FPS: %s", ACTIVO(mostrarfps));
  		glutChangeToMenuEntry(7,mensaje,7);
		break;

	case 'v':
		etiquetas = !etiquetas;
		MENSAJE("v) Nombres de los planetas: %s", ACTIVO(etiquetas));
  		glutChangeToMenuEntry(9,mensaje,9);
		break;

	case 'c':
		animacion = !animacion;
		MENSAJE("c) Animacion en cambio de planeta: %s", ACTIVO(animacion));
		break;

	case '0':
		seguir_planeta = -1;
		MENSAJE("0) Centrando en Sol");
		if(zoom<minzoom[0]) nextzoom=gotozoom[0];
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		   if(seguir_planeta>=getNumPlanetas()) {
				seguir_planeta = -1;
				MENSAJE("0) Centrando en Sol");
				if(zoom<minzoom[seguir_planeta+1]) nextzoom=gotozoom[seguir_planeta+1];
		   } else {
			   seguir_planeta = key-'0'-1;
			   if(zoom<minzoom[seguir_planeta+1]) nextzoom=gotozoom[seguir_planeta+1];
			   MENSAJE("%d) Centrando en planeta: %s", seguir_planeta+1, getNombre(seguir_planeta));
		   }
		   break;

	case 'j':
		   rotx=-90;
		   roty=0;
		   MENSAJE("j) Vista superior");
		   break;
	case 'k':
		   rotx=-25;
		   roty=0;
		   MENSAJE("k) Vista perspectiva");
		   break;
	case 'l':
		   rotx=0;
		   roty=0;
		   MENSAJE("l) Vista lateral");
		   break;

	case 'b':
		   fondo = !fondo;
		   MENSAJE("b) Fondo de la galaxia: %s", ACTIVO(fondo));
     		   glutChangeToMenuEntry(8,mensaje,8);
		break;
	case 'f':
		if(!full_screen) {
			glutFullScreen();
		} else {
			glutReshapeWindow(VENT_WIDTH,VENT_HEIGHT);
		}
		full_screen= !full_screen;
		MENSAJE("f) Pantalla completa: %s", ACTIVO(full_screen));
     		glutChangeToMenuEntry(1,mensaje,1);
		break;

	case 'd':
		atm-=5;
		if(atm<0)
			atm+=360;
		MENSAJE("atm = %f",atm);
		glutPostRedisplay();
		break;

	case 'D':
		atm+=5;
		if(atm>360) atm-=360;
		MENSAJE("atm = %f",atm);
		glutPostRedisplay();
		break;
	case 'y':
		year = year - 5;
		glutPostRedisplay();
		break;
	case 'Y':
		year = year + 5;
		glutPostRedisplay();
		break;
	case 'o':
		orbitando = !orbitando;
		MENSAJE("o) Traslacion de los planetas: %s", ACTIVO(orbitando));
		glutChangeToMenuEntry(2,mensaje,2);
		break;

	case 'r':
		rotando = !rotando;
		MENSAJE("r) Rotacion de los planetas: %s", ACTIVO(rotando));
		glutChangeToMenuEntry(3,mensaje,3);
		break;

	case ' ':
		if( !rotando && !orbitando ) {
			rotando = 1;
			orbitando = 1;
		} else {
			rotando = 0;
			orbitando = 0;
		}
		MENSAJE("spc) Pausa: %s", ACTIVO( !rotando && !orbitando));
		glutChangeToMenuEntry(11,mensaje,11);
		break;
	case 'h':
		captura = capturar();
		if(captura!=NULL) {
			MENSAJE("h) Captura correcta: %s", captura);
		} else {
			MENSAJE("h) Error tomando captura");
		}
		break;
	case 27:	// Escape
		ParaShaders();
		exit(0);
	}
}



void teclado_especial(int key, int key_x, int key_y)
{
	switch(key) {
		case GLUT_KEY_LEFT:
			luzx-=0.1;
			MENSAJE("luzx: %f",luzx);
			break;
		case GLUT_KEY_RIGHT:
			luzx+=0.1;
			MENSAJE("luzx: %f",luzx);
			break;
		case GLUT_KEY_UP:
			luzy+=0.1;
			MENSAJE("luzy: %f",luzy);
			break;
		case GLUT_KEY_DOWN :
			luzy-=0.1;
			MENSAJE("luzy: %f",luzy);
			break;
		case GLUT_KEY_F1:
			camaraPredeterminada(0);
			break;
		case GLUT_KEY_F2:
			camaraPredeterminada(1);
			break;
		case GLUT_KEY_F3:
			camaraPredeterminada(2);
			break;
		case GLUT_KEY_F4:
			camaraPredeterminada(3);
			break;
		case GLUT_KEY_F5:
			camaraPredeterminada(4);
			break;
		case GLUT_KEY_F6:
			camaraPredeterminada(5);
			break;
		case GLUT_KEY_F7:
			camaraPredeterminada(6);
			break;
		case GLUT_KEY_F8:
			printf("rotx = %f;\n",rotx);
			printf("roty = %f;\n",roty);
			printf("zoom = %f;\n",zoom);
			printf("seguir_planeta = %d;\n",seguir_planeta);
			break;
	}
}

void mov_raton(int x,int y)
{
        int diffx=x-lastx;
        int diffy=y-lasty;
        lastx=x;
        lasty=y;
	
	if( Buttons[0] && !modifier )
	{
		rotx += (float) 0.5f * diffy;
		roty += (float) 0.5f * diffx;
	}
	else if( Buttons[1] || modifier )
	{
		zoom += (float)  0.005 * diffy * (abs(zoom/2)+1);
		if(zoom>MAX_ZOOM) zoom=MAX_ZOOM;
		if(zoom<minzoom[seguir_planeta+1]) zoom=minzoom[seguir_planeta+1];
		nextzoom=zoom;
	}

	glutPostRedisplay();

}


void click_raton(int b,int s,int x,int y)
{
        lastx=x;
        lasty=y;
		
		modifier=glutGetModifiers();
		switch(b)
        {
        case GLUT_LEFT_BUTTON:
                Buttons[0] = ((GLUT_DOWN==s)?1:0);
                break;
        case GLUT_MIDDLE_BUTTON:
                Buttons[1] = ((GLUT_DOWN==s)?1:0);
                break;
	case GLUT_RIGHT_BUTTON:
	        Buttons[2] = ((GLUT_DOWN==s)?1:0);
		break;
        case GLUT_WHEEL_UP:                
		if (zoom>1) zoom -=  0.15;
		camara();
                break;
        case GLUT_WHEEL_DOWN:                
		if (zoom<=100) zoom +=  0.15;
		camara();
                break;
        }
        glutPostRedisplay();
}




// Rutinas para los menus
void centraPlaneta(int planeta)
{
	seguir_planeta = planeta;
}

void vistaPredeterminada(int camara)
{
	switch(camara) {
	case 0:
		   rotx=-90;
		   roty=0;
		   break;
	case 1:
		   rotx=-25;
		   roty=0;
		   break;
	case 2:
		   rotx=0;
		   roty=0;
		   break;
	}
}

unsigned char atecla[] = { '_', 'f', 'o', 'r', 'a', 'z', 'x', 'g', 'b', 'v','h',' ',27 };

void menuapp(int value)
{
	teclado( atecla[value], 0, 0);
}

void timer(int valor) {
	Orbitar();
	glutPostRedisplay();
	glutTimerFunc( TIMER_DELAY, timer, 1);
}

void crearMenu()
{
	int i;
	int planetmenu,vistamenu,camaramenu;

	printf("---- Opciones del menu ----\n");

	// Submenu Planetas
	planetmenu = glutCreateMenu(centraPlaneta);	
	glutAddMenuEntry("0) Sol",-1);
	for(i=0;i<getNumPlanetas()-1;i++) {
		sprintf(tmp_str,"%d) %s", i+1, getNombre(i));
		glutAddMenuEntry(tmp_str,i);
	}
	
	// Submenu Vistas
	vistamenu=glutCreateMenu(vistaPredeterminada);
	
	MENSAJE("j) Vista superior");
	glutAddMenuEntry(mensaje,0);
	
	MENSAJE("k) Vista perspectiva");
	glutAddMenuEntry(mensaje,1);
	
	MENSAJE("l) Vista lateral");
	glutAddMenuEntry(mensaje,2);
	
	// Submenu Vistas
	camaramenu=glutCreateMenu(camaraPredeterminada);

	MENSAJE("F1) Vista global 1");
	glutAddMenuEntry(mensaje,0);

	MENSAJE("F2) Vista global 2");
	glutAddMenuEntry(mensaje,1);

	MENSAJE("F3) Vista de la Tierra");
	glutAddMenuEntry(mensaje,2);

	MENSAJE("F4) Vista de Saturno");
	glutAddMenuEntry(mensaje,3);

	MENSAJE("F5) Vista desde el Sol");
	glutAddMenuEntry(mensaje,4);

	// Menu Principal
	mainmenu=glutCreateMenu(menuapp);
	
	MENSAJE("f) Pantalla completa: %s", ACTIVO(full_screen));
	glutAddMenuEntry(mensaje,1);
	
	MENSAJE("o) Traslacion de planetas: %s", ACTIVO(orbitando));
	glutAddMenuEntry(mensaje,2);

	MENSAJE("r) Rotacion de planetas: %s", ACTIVO(rotando));
	glutAddMenuEntry(mensaje,3);

	MENSAJE("a) Atmosfera terrestre: %s", ACTIVO(atmosfera));
	glutAddMenuEntry(mensaje,4);


	MENSAJE("z) Plano de referencia ( 1 cuadro = 1UA ): %s", ACTIVO(plano));
	glutAddMenuEntry(mensaje,5);

	MENSAJE("x) Ejes de rotacion: %s", ACTIVO(ejes));
	glutAddMenuEntry(mensaje,6);

	MENSAJE("g) Visualizar FPS: %s", ACTIVO(mostrarfps));
	glutAddMenuEntry(mensaje,7);

	MENSAJE("b) Fondo de la galaxia: %s", ACTIVO(fondo));
	glutAddMenuEntry(mensaje,8);

	MENSAJE("v) Nombres de los planetas: %s", ACTIVO(etiquetas));
	glutAddMenuEntry(mensaje,9);

	MENSAJE("h) Captura de pantalla");
	glutAddMenuEntry(mensaje,10);

	MENSAJE("spc) Pausa: %s", ACTIVO(!orbitando && !rotando));
	glutAddMenuEntry(mensaje,11);

	glutAddSubMenu("c) Centrar en planeta",planetmenu);
	glutAddSubMenu(" > Vistas",vistamenu);
	glutAddSubMenu(" > Camara predeterminada",camaramenu);

	glutAddMenuEntry("Esc) Salir",12);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
  
}

void visible(int estado) {
	if(estado==GLUT_VISIBLE) {
		glutDisplayFunc(display);
	} else if (estado==GLUT_NOT_VISIBLE) {
		glutDisplayFunc(NULL);
	}
}


int main(int argc, char** argv)
{
	// Inicializar Opengl
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize (VENT_WIDTH , VENT_HEIGHT ); 
	glutInitWindowPosition (VENT_POSX, VENT_POSY);
	glutCreateWindow (TITULO_VENTANA);

	// Inicializar aplicacion
	init();
	crearMenu();

	// Manejadores de eventos
	glutDisplayFunc(display); 
	glutIdleFunc(Orbitar);
	//glutTimerFunc( TIMER_DELAY, timer, 1);
	glutMouseFunc(click_raton);
	glutMotionFunc(mov_raton);
	glutReshapeFunc(redimensionar);
	glutKeyboardFunc(teclado);
	glutSpecialFunc(teclado_especial);
	glutVisibilityFunc(visible);

	MENSAJE("Pulse el boton central del raton para el menu de opciones");
	msg_timestamp = time(NULL) + 5;

	glutMainLoop();
	return 0;
}
