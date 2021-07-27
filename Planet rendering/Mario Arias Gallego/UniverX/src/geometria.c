/*
   Práctica Final Informática Gráfica 2006
   Simulador Sistema Solar
   
   Mario Arias Gallego <mariari@lab.fi.uva.es>
   Álvaro Zubizarreta López <alvazub@lab.fi.uva.es>
*/

#include "supergl.h"
#include <stdio.h>

#include <math.h>
#include <stdlib.h>

#ifdef MINGW32
#include <malloc.h>
#endif

#include "geometria.h"
#include "constantes.h"
#include "shader.h"

// Vars
XYZ origen = {0.0, 0.0, 0.0};

// Esfera almacenada
XYZ *puntos_esfera=NULL;
int n_esfera;


// Genera los puntos de una esfera y los guarda en un array
void generaEsfera()
{
	int taman,n;
	int i,j;
	int count;
	double theta1,theta2,theta3;

	n = CARAS;
	taman= (n+1) * n *3;

	printf("Generando esfera de %d posiciones y %d bytes... ", taman, taman * sizeof(XYZ) );

	/* Reservar espacio para los puntos de la esfera */
	puntos_esfera = (XYZ *) malloc( taman * sizeof(XYZ));
	n_esfera = n;
	
	// Recorrer
	count=0;
	for (j=0;j<n/2;j++) {
		theta1 = j * 2*PI / n - (PI/2);
		theta2 = (j + 1) * 2*PI / n - (PI/2);

		for (i=n;i>=0;i--) {
			theta3 = (i) * 2*PI / n;

			//Primer punto
			puntos_esfera[count].x = cos(theta2) * cos(theta3);
			puntos_esfera[count].y = sin(theta2);
			puntos_esfera[count].z = cos(theta2) * sin(theta3);
			
		        //printf( "Punto A: %f,%f,%f\n",puntos_esfera[count].x, puntos_esfera[count].y, puntos_esfera[count].z );
			 
			count++;

			// Segundo punto
			puntos_esfera[count].x = cos(theta1) * cos(theta3);
			puntos_esfera[count].y = sin(theta1);
			puntos_esfera[count].z = cos(theta1) * sin(theta3);
		        //printf( "Punto B: %f,%f,%f\n",puntos_esfera[count].x, puntos_esfera[count].y, puntos_esfera[count].z );
			 
			count++;

			// BiNormal
			puntos_esfera[count].x = cos(theta2+PIMED);
			puntos_esfera[count].y = sin(theta2+PIMED);
			puntos_esfera[count].z = 0;
		        //printf( "BiNormal: %f,%f,%f\n",puntos_esfera[count].x, puntos_esfera[count].y, puntos_esfera[count].z );
			 
			count++;

			// Tangente
			puntos_esfera[count].x = cos(theta3+PIMED);
			puntos_esfera[count].y = 0.0;
			puntos_esfera[count].z = sin(theta3+PIMED);
		        //printf( "Tangente: %f,%f,%f\n\n\n",puntos_esfera[count].x, puntos_esfera[count].y, puntos_esfera[count].z );
			 
			count++;

		}
		//printf("*****************************\n\n\n\n");
	}

	printf("OK!\n");
}

// Mostrar una esfera generada
void muestraEsfera()
{
	int i,j,count,n;

	if(puntos_esfera==NULL) generaEsfera();

	n = n_esfera;

	count=0;
	for (j=0;j<n/2;j++) {
      		glBegin(GL_QUAD_STRIP);
		for (i=n;i>=0;i--) {
			// Primer punto 
		         glNormal3f( puntos_esfera[count].x,
			             puntos_esfera[count].y, 
				     puntos_esfera[count].z );
			 
		         glTexCoord2f((n-i)/(double)n,2*(n-j-1)/(double)n);

		         glVertex3f( puntos_esfera[count].x,
			             puntos_esfera[count].y, 
				     puntos_esfera[count].z );

			 count++;

			 // Segundo punto
			 glNormal3f( puntos_esfera[count].x,
			             puntos_esfera[count].y, 
				     puntos_esfera[count].z );
			 
		         glTexCoord2f((n-i)/(double)n,2*(n-j)/(double)n);

		         glVertex3f( puntos_esfera[count].x,
			             puntos_esfera[count].y, 
				     puntos_esfera[count].z );

			 count++;

			
			 NormalVShaderSetBinormal( puntos_esfera[count].x,
					puntos_esfera[count].y,
					puntos_esfera[count].z,
					1.0);

			 count++;

			 
			 NormalVShaderSetTangent( puntos_esfera[count].x,
					puntos_esfera[count].y,
					puntos_esfera[count].z,
					1.0);

			 count++;
			 
		}
		glEnd();
	}


}


// Dibujar ejes de coordenadas
void dibujaEjes(GLfloat dist) {
	glDisable(GL_LIGHTING);
	// Eje X Red
        glColor3f (1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f (0.0,0.0,0.0); 
        glVertex3f (dist,0.0,0.0);
        glEnd();

	// Eje Y Green
        glColor3f (0.0, 1.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f (0.0,0.0,0.0);
        glVertex3f (0.0,dist,0.0); 
        glEnd();

	// Eje Z Blue
        glColor3f (0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex3f (0.0,0.0,0.0);
        glVertex3f (0.0,0.0,dist);
        glEnd();        
 
	glEnable(GL_LIGHTING);
}


// Dibujar plano base
#define TAM_CUAD 15
void dibuja_cuadrado()
{
	int i;


	glColor3f(0.6, 0.6, 0.6);
	glBindTexture(GL_TEXTURE_2D, 0);   // Quitar textura
        glBegin( GL_LINES );
                glNormal3f(0,1,0);
                for(i=-TAM_CUAD;i<=TAM_CUAD;i+=1)
                {
                        glVertex3f(i,0,-TAM_CUAD);
                        glVertex3f(i,0,TAM_CUAD);
                        glVertex3f(-TAM_CUAD,0,i);
                        glVertex3f(TAM_CUAD,0,i);
                }

        glEnd();
}


// Dibujar eje de rotaciÃ³n de un planeta
void dibuja_eje_rotacion(float dist)
{
        glColor3f (1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        glVertex3f (0.0,-dist,0.0);
        glVertex3f (0.0,dist,0.0); 
        glEnd();
}

// Dibuja los anillos de Saturno
void dibujarAnillos(int n, float minr, float maxr)
{
	int i;
	float theta;
	XYZ p;

	glDisable( GL_CULL_FACE );
	glBegin(GL_QUAD_STRIP);

	for(i=0;i<=n;i++) {
		theta = i * 2 * PI / n;

		p.x= sin( theta );
		p.y=0.0;
		p.z= cos( theta );

		glNormal3f( p.x, p.y, p.z);
		glTexCoord2f( 0.0, 0.5);     // Horizontal
		glVertex3f( p.x *minr, p.y*minr, p.z*minr );


		glNormal3f( p.x, p.y, p.z);
		glTexCoord2f( 1.0, 0.5);
		glVertex3f( p.x *maxr, p.y*maxr, p.z*maxr );
	}
	glEnd();
	glEnable( GL_CULL_FACE );
}
