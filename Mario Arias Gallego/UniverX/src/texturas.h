/* Práctica Final Informática Gráfica 2006
   Simulador Sistema Solar
   
   Mario Arias Gallego <mariari@lab.fi.uva.es>
   Álvaro Zubizarreta López <alvazub@lab.fi.uva.es>
*/

#ifndef TEXTURAS_H
#define TEXTURAS_H

#include "constantes.h"

#ifndef TEXTURAS_C
extern int textura[0];
#endif

#define MAX_TEXTURAS 30 

struct Imagen {
    unsigned long tamX;
    unsigned long tamY;
    char *datos;
    int has_alpha;
};
typedef struct Imagen Imagen;

// Cargar todas las texturas
void CargarTexturas();

// Cargador de imagenes
char *getExt(char *str);
void CargarTextura(int pos, char *fichero);
int cargarBMP(char *filename, Imagen *imagen);
int cargarJPG(char *filename, Imagen *imagen);
int cargarPNG(char *filename, Imagen *imagen);

// Capturas
char *capturar();
void capturaJPG(char *filename, int calidad);
    
#endif
