/* Práctica Final Informática Gráfica 2006
   Simulador Sistema Solar
   
   Mario Arias Gallego <mariari@lab.fi.uva.es>
   Álvaro Zubizarreta López <alvazub@lab.fi.uva.es>
*/

#ifndef _ORBIT_H_

#define _ORBIT_H_

#include <math.h>
#include <stdio.h>


typedef struct {
  double t;     //Tiempo
  double x;     // Coordenadas
  double y;
  double z;
  double w;     //Argumento del perihelio
  double wd;
  double a;     //Eje semi mayor
  double e;     //Excentriciudad (0=círculo, 0-1=elipse, 1=parabola)
  double ed;
  double M;     //Anomalía media (0 en el perihelio)
  double Md;
  double N;     //Longitud del nodo ascendiente
  double Nd;
  double i;     //Inclinación de la elíptica
  double id;
  double r;     //Radio del cuerpo celeste
  double p;		// Periodo en dias
  double ang;	// Angulo
  char nombre[100]; // Nombre del planeta
} Estado;

/* Obtencion de valores */
double getX(int i);
double getY(int i);
double getZ(int i);
double getIncT();
double getTiempo();
void setIncT(double inc);
double getPlanetRadius(int i);
double getStarRadius(); 
int    getNumPlanetas();
char  *getNombre(int i);
double getAng(int i);
double getPeriod(int i);
void incAng(int i);

/* Establecimiento de valores */
void setT(double tiempo);

/* Funciones especificas para la aplicacion */
int    cargarPlanetas();
void   mover(int i);


#endif
