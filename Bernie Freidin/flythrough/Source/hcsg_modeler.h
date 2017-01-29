// **************************************************************************
// HCSG Demo
// (c) Bernie Freidin, 1999-2000
// **************************************************************************

#ifndef HCSG_MODELER_H
#define HCSG_MODELER_H

#include "hcsg.h"

void CSG_InitializeModeler(hull_t *world);
int  CSG_GetMemInUse_Modeler(void);
void CSG_RunScript(char fname[]);

int _CSG_AddHull(void); // recent code

#endif
 