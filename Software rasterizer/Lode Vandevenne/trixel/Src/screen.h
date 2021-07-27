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

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "init.h"
#include "extern.h"

void setScreen(int width, int height, int colorDepth); //create the SDL surface
void Slock(); //lock the screen
void Sulock(); //unlock the screen
void drawPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B); //draw one pixel at location x, y with an RGB color
void updateScreen(int u,int v,int width,int height); //refresh a rectangular section of the screen after everything was drawn
void clearScreen(); //make the screen black

extern SDL_Surface *screen; //the single SDL surface Trixel uses


#endif
