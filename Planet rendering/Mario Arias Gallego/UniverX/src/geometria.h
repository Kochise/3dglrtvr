/*
   Práctica Final Informática Gráfica 2006
   Simulador Sistema Solar
   
   Mario Arias Gallego <mariari@lab.fi.uva.es>
   Álvaro Zubizarreta López <alvazub@lab.fi.uva.es>
*/

typedef struct {
	float x,y,z;
} XYZ;


void DibujaEsfera(double radio);
void DibujaEsferaExterna(int n);
void DibujarEsfera(XYZ c,double r,int n);

void generaEsfera();
void muestraEsfera();
void muestraEsferaExterna();


void dibujaEjes(GLfloat dist);
void dibuja_cuadrado();
void dibuja_eje_rotacion(float dist);
void dibujarAnillos(int n, float minr, float maxr);

