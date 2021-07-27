/* Práctica Final Informática Gráfica 2006
   Simulador Sistema Solar
   
   Mario Arias Gallego <mariari@lab.fi.uva.es>
   Álvaro Zubizarreta López <alvazub@lab.fi.uva.es>
*/

#include "supergl.h"

#include "texto.h"
#include "constantes.h"

// Dibuja texto sobreescribiendo el Framebuffer
void texto_encima( char *cadena, float x, float y, float z)
{
	glDisable(GL_DEPTH_TEST);
	texto( cadena, x, y, z );
	glEnable(GL_DEPTH_TEST);
}

/* Dibujar texto */
void texto( char *cadena, float x, float y, float z)
{
	char *c;

	glDisable(GL_LIGHTING);
	glColor4f(1,1,1,1);
	glRasterPos3f(x, y, z);
	for (c = cadena; *c; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}
	glEnable(GL_LIGHTING);
}

