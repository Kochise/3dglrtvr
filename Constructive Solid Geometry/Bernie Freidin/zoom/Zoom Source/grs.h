/* GRS.h 
 * Copyright (c) 1999 By James Beattie 
 */

#ifndef __GRS_H
#define __GRS_H

#include <stdio.h>
#include <string.h> 
#include <math.h> 
#include <stdlib.h>
//#include <Windows.h>

struct point_t 
{
	float x, y;
};

struct rect_t
{
	float l, t, r, b;
};

struct pline_t
{
	int seg_count;
	point_t * seg_list;
	pline_t * next;
};

struct grs_t 
{
	pline_t * start;
	int line_count;
	rect_t bounds;
	char filename[200];
	int linecount;
	int polylinecount;
};

int  GRS_Load(grs_t * grs, char * path);
void GRS_Draw(grs_t * grs); 
void GRS_Free(grs_t * grs);

#endif