/* Práctica Final Informática Gráfica 2006
   Simulador Sistema Solar
   
   Mario Arias Gallego <mariari@lab.fi.uva.es>
   Álvaro Zubizarreta López <alvazub@lab.fi.uva.es>
*/

#include <stdio.h>
#include <string.h>
#include "supergl.h"

#include "orbit.h"
#include "constantes.h"

#define MAX_PLANETAS 20
#define STAR_RADIUS 696000.0
#define FORMATO "%s %lf %lf %lf %lf %lf  %lf %lf %lf %lf %lf %lf %lf %lf"

/******************************************************************************/

Estado estado[MAX_PLANETAS];
int numplanetas = 0;
double incT = 5.0;

/******************************************************************************/

inline double getX(int i) {
	if(i==-1)
		return 0.0;
	else
		return estado[i].x;
}

inline double getY(int i) {
	if(i==-1)
		return 0.0;
	else
		return estado[i].y;
}

inline double getZ(int i) {
	if(i==-1)
		return 0.0;
	else 
		return estado[i].z;
}

inline double getIncT () {
	return incT;
}

inline double getTiempo() {
	return estado[2].t;
}

inline double getPlanetRadius(int i) {
	return estado[i].r*1000/L; //Radio en UAs
}

inline double getStarRadius() {
	return STAR_RADIUS*1000/L; //Radio en UAs
}

inline int getNumPlanetas() {
	return numplanetas;
}

inline char *getNombre(int i) {
	return estado[i].nombre;
}

inline double getPeriod(int i) {
	return estado[i].p;
}

inline void incAng(int i) {
	estado[i].ang += 4*incT/getPeriod(i);
}

double getAng(int i) {
	return estado[i].ang;
}

/******************************************************************************/

inline void setIncT(double t) {
	incT = t;
}

/******************************************************************************/

/* Cálculo de la anomalía excéntrica en radianes*/
double AE (double M, double e) {
double E, E0, E1;
    E = M + e * sin(M) * ( 1.0 + e * cos(M) );
    
    /* Si E < 0.05 iteramos para una mayor precisión */
    if (E < 0.05) {
        //getchar();
        E0 = E;
        while ( (E0 - E1) > 0.0001 ) {
                E1 = E0 - ( E0 - e*(180/PI) * sin(E0) - M ) / ( 1 - e * cos(E0) );
                E1 = E0 - ( E0 - e * sin(E0) - M ) / ( 1 - e * cos(E0) );
                E0 = E1;
        }
        E = E1;
    }
    
    return E*PI/180;
}

/* Cálculo de la distancia */
double R (double a, double E, double e) {
double xv, yv;

    xv = a * ( cos(E) - e );
    yv = a * ( sqrt(1.0 - e*e) * sin(E) );
    return sqrt( xv*xv + yv*yv );
    
}


/* Cáclulo de la anomalía verdadera en radianes */
double V (double a, double E, double e) {
double xv, yv;

    xv = a * ( cos(E) - e );
    yv = a * ( sqrt(1.0 - e*e) * sin(E) );
    return atan2( yv, xv );
}
    

/* Cálculo de las coordenadas en 3D */
void Coord3D (double r, double v, double N, double w, double i, int planeta) {
double xh, yh, zh;

    xh = r * ( cos(N) * cos(v+w) - sin(N) * sin(v+w) * cos(i) );
    yh = r * ( sin(N) * cos(v+w) + cos(N) * sin(v+w) * cos(i) );
    zh = r * ( sin(v+w) * sin(i) );
    
    estado[planeta].x = xh;
    estado[planeta].y = yh;
    estado[planeta].z = zh;
    
   // if(planeta==0) printf("[Coordenadas] x %lf y %lf\t z %lf r %lf v %lf\n", xh,yh,zh,r,v*180/PI );
}

/******************************************************************************/

int cargarPlanetas () {
	int ret;
	FILE *fp;
	char cadena[256];

	char n[64]; //Nombre     
	double a,e,ed,w,wd,M,Md,N,Nd,i,id,r,p;

	// Abrir fichero
	fp = fopen( FICHERO_PLANETAS , "r");
	if (fp == NULL) return -1;

	printf("---- Cargando fichero de planetas ----\n");
	while (!feof(fp)) {
		// Leer linea
		fgets(cadena, 255, fp); 
		cadena[255] = 0;

		// Parsear
		if (cadena[0] != '#') {
			ret=sscanf(cadena, FORMATO, n, &a,&e,&ed,&w,&wd,&M,&Md,&N,&Nd,&i,&id,&r,&p);
				    
			if(ret==EOF) return 0;
				       
			// Guardar en la estructura
			estado[numplanetas].t  = 0.0;
			estado[numplanetas].a  = a;
			estado[numplanetas].e  = e;
			estado[numplanetas].ed = ed;
			estado[numplanetas].w  = w;
			estado[numplanetas].wd = wd;
			estado[numplanetas].M  = M;
			estado[numplanetas].Md = Md;
			estado[numplanetas].N  = N;
			estado[numplanetas].Nd = Nd;
			estado[numplanetas].i  = i;
			estado[numplanetas].id = id;          		
			estado[numplanetas].r  = r;
			estado[numplanetas].p  = p;
			estado[numplanetas].ang = 0.0;
			strcpy(estado[numplanetas].nombre, n);
			

			printf("Cargado planeta %s de radio %f\n", n, r);
			
			numplanetas++;
		}
	}

	return 1;
}

    
void mover (int planeta) {
	double M,e,r,v,N,w,i,t,E;

	t = estado[planeta].t;

	M = (estado[planeta].M + estado[planeta].Md * t) * PI/180;
	e = estado[planeta].e + estado[planeta].ed * t;
	E = AE (M, e);

	N = (estado[planeta].N + estado[planeta].Nd * t) * PI/180;
	w = (estado[planeta].w + estado[planeta].wd * t) * PI/180;
	i = (estado[planeta].i + estado[planeta].id * t) * PI/180;

	r = R(estado[planeta].a, E,estado[planeta].e);
	v = V(estado[planeta].a, E,estado[planeta].e);

	Coord3D (r, v, N, w, i, planeta);

	// if(planeta == 0) printf("[Params] t %.3lf N %.3lf w %.3lf i %.3lf E %.3lf\n", t, N, w, i, E);

	estado[planeta].t  += incT;
}

