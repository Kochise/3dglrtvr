/*
 * Copyright (C) 1999-2005  Terence M. Welsh
 *
 * This file is part of Skyrocket.
 *
 * Skyrocket is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 *
 * Skyrocket is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef FLARE_H
#define FLARE_H



#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>


#define FLARESIZE 128


extern unsigned int flarelist[4];
extern int xsize, ysize;
extern float aspectRatio;
extern double modelMat[16], projMat[16];
extern int viewport[4];


// Generate textures for lens flares
// then applies textures to geometry in display lists
void initFlares();


struct flareData{
	float x, y, r, g, b, a;
};


// Draw a flare at a specified (x,y) location on the screen
// Screen corners are at (0,0) and (1,1)
// alpha = 0.0 for lowest intensity; alpha = 1.0 for highest intensity
void flare(float x, float y, float red, float green, float blue, float alpha);



#endif  // FLARE_H