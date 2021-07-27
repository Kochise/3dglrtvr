#ifndef __ILLUMINATE_H__
#define __ILLUMINATE_H__

/*
* A simple file to control the distribution of global illumination information.
*
* Author: Greg Coombe
* Date: Feb 4th, 2001
*
*/

#include <glvu.hpp>
#include <objmodel/objmodel.hpp>
#include <assert.h>
#include <math.h>
#include "GLModel.h"


#define MIN(A, B) ( (A) < (B) ? (A):(B) )

extern Object *Obj;
extern GLVU glvu;

void visibility();
void init( int size );
void illuminate();
void set_colors( const Vec3f color );
void set_lights();
void move_objects( const Vec3f dist);
void getVertexCam();
void getCam( const GSample* point, Camera* cam );

#endif //__ILLUMINATE_H__