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
This file contains the main function, sets up SDL, and then starts the gameloop.
*/

#include <SDL/SDL.h>

#include "world.h"
#include "init.h"
#include "screen.h"
#include "trixel.h"


int screenWidth = 640;  //width and height of the window that will be opened
int screenHeight = 480;

int main(int argc, char *argv[])
{
   setScreen(screenWidth,screenHeight,32);
   SDL_WM_GrabInput(SDL_GRAB_ON);
   SDL_WM_SetCaption("Trixel 3D Engine by Lode Vandevenne - Use Mouse & Arrows To Move, Hit esc to quit", NULL);
   SDL_ShowCursor(SDL_DISABLE);
   buildWorld();
   gameLoop();
   return 0;
}





