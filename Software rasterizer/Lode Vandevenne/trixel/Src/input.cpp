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
This file contains all user input routines.  userInput() reads mouse and keyboard
input and performs the actions related to the input, such as moving the camera or
placing blocks.  See the comments in the function to see what all keys do.
*/

#include <SDL/SDL.h>
#include "trixel.h"
#include "input.h"
#include "world.h"
#include "init.h"
#include "extern.h"
#include "screen.h"
#include "camera.h"

bool mouseMayMove = false; //this way the mouse won't "jump" at startup if the mouse wasn't centered yet
double speed = 0.2;  //Your movement speed.  This value should never be greater than 1, or the collision detection won't work correctly (because it doesn't support "4-dimensional collision detection")
const double pi = 3.1415926535897932384626433832795;

void userInput()
{

    /////////////////////////////////////////
    //MOUSE
    /////////////////////////////////////////

    //Read relative mouse movement

    int mouseX;
    int mouseY;
    SDL_GetRelativeMouseState(&mouseX, &mouseY);

    if (mouseMayMove)
    {

       //Rotate camera depending on mouseX and mouseY movement
       //the order IS important!! FIRST do the pitch rotation, only then the yaw rotation! otherwise the pitch happens around the wrong axis
       //For both Pitch and Yaw, all 3 camera vectors must be rotated to make sure they remain perpendicular (otherwise the camera may skew)

       //Pitch
       cam.rotate(cam.u,-pi*mouseY/180);

       //Yaw
       cam.rotate(quickVector(0,0,1),-pi*mouseX/180);
    }
    else
    {
        //if the program just started, the mouse may be used the next frame
        mouseMayMove = true;
    }

    //////////////////////////////////
    //KEYS
    //////////////////////////////////

    Uint8 *keystate = SDL_GetKeyState(NULL);


    //Keys for camera movement

    if ( keystate[SDLK_UP] )
    {
        cam.move(multiplyBy(cam.dir, speed));
    }

    if ( keystate[SDLK_DOWN] )
    {
        cam.move(multiplyBy(minus(cam.dir), speed));
    }

    if ( keystate[SDLK_RIGHT] )
    {
        cam.move(multiplyBy(normalize(cam.u), speed));
    }

    if ( keystate[SDLK_LEFT] )
    {
        cam.move(multiplyBy(minus(normalize(cam.u)), speed));
    }

    if ( keystate[SDLK_KP1] )
    {
        vector moveCam = {0,0,speed};
        cam.move(moveCam);
    }

    if ( keystate[SDLK_KP0] )
    {
        vector moveCam = {0,0,-speed};
        cam.move(moveCam);
    }

    //Keys for camera roll

    if ( keystate[SDLK_KP6] )
    {
        cam.rotate(cam.dir,5*pi/180);

    }
    if ( keystate[SDLK_KP4] )
    {
        cam.rotate(cam.dir,-5*pi/180);
    }

    //Key to reset camera direction

    if ( keystate[SDLK_KP5] )
    {
        cam.dir = defaultCam.dir;
        cam.u = defaultCam.u;
        cam.v = defaultCam.v;
    }

    //Keys to skew camera

    if ( keystate[SDLK_KP2] )
    {
        cam.v = rotateAroundArbitrary(cam.v, cam.dir, pi/360);

    }

    if ( keystate[SDLK_KP3] )
    {
        cam.v = rotateAroundArbitrary(cam.v, cam.dir, -pi/360);

    }

    //Key to get funny random camera direction

    if ( keystate[SDLK_KP_PERIOD] )
    {
        cam.dir.x = rand1();
        cam.dir.y = rand1();
        cam.dir.z = rand1();
        cam.u.x = rand1();
        cam.u.y = rand1();
        cam.u.z = rand1();
        cam.v.x = rand1();
        cam.v.y = rand1();
        cam.v.z = rand1();
    }


    //Key to give mouse cursor back and allow the user to close the program

    if ( keystate[SDLK_ESCAPE] )
    {
        SDL_ShowCursor(SDL_ENABLE);
        SDL_WM_GrabInput(SDL_GRAB_OFF);
        gameShouldEnd = true;
    }

    //Keys to change drawing resolution

    if ( keystate[SDLK_KP7] )
    {
        uDraw = 160; vDraw=120;
    }

    if ( keystate[SDLK_KP8] )
    {
        uDraw = 320; vDraw=240;
    }

    if ( keystate[SDLK_KP9] )
    {
        uDraw = 640; vDraw=480;
    }


    //Keys to place and remove blocks

    if ( keystate[SDLK_b] )
    {
        vector blockPos = cam.dir;

        if (world[static_cast<int>(cam.pos.x+blockPos.x)][static_cast<int>(cam.pos.y + blockPos.y)][static_cast<int>(cam.pos.z + blockPos.z)].shape == 0)
        {
            world[static_cast<int>(cam.pos.x+blockPos.x)][static_cast<int>(cam.pos.y + blockPos.y)][static_cast<int>(cam.pos.z + blockPos.z)].shape = 2;
            world[static_cast<int>(cam.pos.x+blockPos.x)][static_cast<int>(cam.pos.y + blockPos.y)][static_cast<int>(cam.pos.z + blockPos.z)].r = 255;
            world[static_cast<int>(cam.pos.x+blockPos.x)][static_cast<int>(cam.pos.y + blockPos.y)][static_cast<int>(cam.pos.z + blockPos.z)].g = 255;
            world[static_cast<int>(cam.pos.x+blockPos.x)][static_cast<int>(cam.pos.y + blockPos.y)][static_cast<int>(cam.pos.z + blockPos.z)].b = 255;
        }

    }

    if ( keystate[SDLK_n] )
    {
        vector blockPos = cam.dir;

        if  (world[static_cast<int>(cam.pos.x+blockPos.x)][static_cast<int>(cam.pos.y + blockPos.y)][static_cast<int>(cam.pos.z + blockPos.z)].shape == 2)
        {
             world[static_cast<int>(cam.pos.x+blockPos.x)][static_cast<int>(cam.pos.y + blockPos.y)][static_cast<int>(cam.pos.z + blockPos.z)].shape = 0;
        }
    }

    //Keys to increase and decrease movement speed

    if ( keystate[SDLK_KP_PLUS] )
    {
        if (speed < 1) speed+=0.05;
    }

    if ( keystate[SDLK_KP_MINUS] )
    {
        speed /= 1.3;
    }

    //Keys to dynamicly build different predefined voxel worlds

    if ( keystate[SDLK_F1] )
    {
        buildIntroWorld();
        cam.pos = quickVector(2,14,14);
        speed = 0.1;
    }

    if ( keystate[SDLK_F2] )
    {
        buildStarFieldWorld();
        cam.pos = quickVector(32,32,32);
        speed = 0.4;
    }

    if ( keystate[SDLK_F3] )
    {
        buildWorldWithSPikes();
        cam.pos = quickVector(32,32,32);
        speed = 0.4;
    }

    if ( keystate[SDLK_F4] )
    {
        buildRasterWorld();
        cam.pos = quickVector(32,32,32);
        speed = 0.2;
    }

    if ( keystate[SDLK_F5] )
    {
        buildCubeMaze();
        speed = 0.1;
        cam.pos = quickVector(32,32,32);
    }

    if ( keystate[SDLK_F6] )
    {
        buildSphereWorld();
        cam.pos = quickVector(3,32,32);
        speed = 1;
    }
}
