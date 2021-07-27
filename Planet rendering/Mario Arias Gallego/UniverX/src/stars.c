/* Práctica Final Informática Gráfica 2006
   Simulador Sistema Solar
   
   Mario Arias Gallego <mariari@lab.fi.uva.es>
   Álvaro Zubizarreta López <alvazub@lab.fi.uva.es>
*/

#include "supergl.h"
#include "shader.h"

#include "star_data.h"

int listnum=1;
float wavepos = 0.0f;

// Mostrar todas las estrellas
void drawStars() {
	int i;

	glDisable(GL_LIGHTING);

	glBegin(GL_POINTS);
	for(i=0;i<NUMSTARS;i+=1) {
		StarVShaderSetColor( stardata[i].mag, stardata[i].mag, stardata[i].mag, 1.0);
		StarVShaderSetWave( wavepos );
		glVertex3d(stardata[i].x, stardata[i].y, stardata[i].z);
		wavepos += 0.00001;
	}
	glEnd();

	glEnable(GL_LIGHTING);
}


