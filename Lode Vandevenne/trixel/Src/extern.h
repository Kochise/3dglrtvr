/*
Trixel 3D Engine

Copyright (C) 2002 Lode Vandevenne (lode@vandevenne.net)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

/*
This file contains some external variables.
*/

#ifndef __EXTERN_H__
#define __EXTERN_H__

#include <SDL/SDL.h>

#include "world.h"

extern int screenWidth; //width of the window that will be opened
extern int screenHeight; //height of the window that will be opened
extern double uDraw; //actual drawing width
extern double vDraw; //actual drawing height
const extern double pi;  //3.14159...
extern voxel world[64][64][64]; //this is the world, made out of voxels
extern bool gameShouldEnd; //whenever something sets this bool to true, the gameloop will end

#include "camera.h"  //This is placed here because world must be defined before the camera struct can be loaded.  I will try to make things more OOP in a next version.

extern camera cam; //The camera
extern camera defaultCam; //A container for default camera settings

#endif


