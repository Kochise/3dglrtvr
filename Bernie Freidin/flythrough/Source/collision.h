// **************************************************************************
// HCSG Demo
// (c) Bernie Freidin, 1999-2000
// **************************************************************************

#ifndef COLLISION_H
#define COLLISION_H

#include "hcsg.h"

void CDR_Initialize(hull_t *world);
void CDR_Collide(vec3 &position, vec3 &impulse, double radius);

#endif
