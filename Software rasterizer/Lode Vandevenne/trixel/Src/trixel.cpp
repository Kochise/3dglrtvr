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
This file contains the gameloop and the raycasting function.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>

#include "init.h"
#include "world.h"
#include "trixel.h"
#include "input.h"
#include "extern.h"
#include "screen.h"


SDL_Event event;


//Definition of some external variables:
double uDraw = 100;
double vDraw = 75;
camera cam = {2,14,14, 1,0,0, 0,0,0.75, 0,1,0};
camera defaultCam = {32,32,32, 1,0,0, 0,0,0.75, 0,1,0};
bool gameShouldEnd = false; //gebruikt voor muis terug te krijgen op einde


//The gameloop; this loop draws the scene, updates the screen and gets input commands from the user each frame.  The loop ends when the user ends the game.

void gameLoop()
{
    int done=0;
#ifdef FULLSCREEN
    Slock();
#endif
    while(done == 0)
    {
         while ( SDL_PollEvent(&event) )
         {
               if ( event.type == SDL_QUIT || gameShouldEnd ) done = 1;
         }
         cameraCorrection();
         rayCast();
#ifndef FULLSCREEN
         updateScreen(static_cast<int>(screenWidth/2-uDraw/2),static_cast<int>(screenHeight/2-vDraw/2),static_cast<int>(uDraw),static_cast<int>(vDraw));
#endif
         userInput();
    }
}

void cameraCorrection()
{
       //There are some problems if the position of the camera is an exact integer position, so we move it a little bit if that's the case
       if (cam.pos.x - static_cast<int>(cam.pos.x) == 0) cam.pos.x += 0.01;
       if (cam.pos.y - static_cast<int>(cam.pos.y) == 0) cam.pos.y += 0.01;
       if (cam.pos.z - static_cast<int>(cam.pos.z) == 0) cam.pos.z += 0.01;
}

//Here follows the long rayCast function.  It's all in one function to increase performance.  The function is a big double for loop: for every pixel on screen the same calculations must be redone.
//The function consists out of 3 main parts inside the double for loop:
//1: Calculate the direction of the ray, based on the position of the pixel on screen and the shape of the camera plane
//2: Perform the actual raycasting, using a 3DDDA algorithm
//3: Calculate the color of the pixel based on what kind of cube was hit, and place the pixel on screen

void rayCast()
{
       //All variables are initiated at the part of this function where they're needed
     
       //Start a double for loop to reach each pixel of the screen

       for (double u=-uDraw/2;u<uDraw/2;u++)
       {
            for (double v=-vDraw/2;v<vDraw/2;v++)
            {

               //Calculate the direction of the ray

               vector rayPos; //the location of origin point of the ray vector while it's traced
               vector rayDir; //each ray has it's own raydir
               double uPos; //position on screen in 2d coordinates
               double vPos;

               uPos = 2*u/uDraw;
               vPos = 2*v/vDraw;

               rayDir.x = cam.u.x*uPos + cam.v.x*vPos + cam.dir.x;
               rayDir.y = cam.u.y*uPos + cam.v.y*vPos + cam.dir.y;
               rayDir.z = cam.u.z*uPos + cam.v.z*vPos + cam.dir.z;

               //The following part of this function exists out of the 3DDDA Algorithm
               //based on "A Fast Voxel Traversal Algorithm" by John Amanatides and Andrew Woo: "http://www.cs.yorku.ca/~amana/research/grid.pdf"

               //Initial Calculations for the 3DDDA Algorithm

               intvector cube;
               vector tMax;
               vector tDelta;
               intvector step;

               cube.x = static_cast<int>((cam.pos.x));
               cube.y = static_cast<int>((cam.pos.y));
               cube.z = static_cast<int>((cam.pos.z));

               tDelta.x = sqrt(1 + (rayDir.y*rayDir.y)/(rayDir.x*rayDir.x) + (rayDir.z*rayDir.z)/(rayDir.x*rayDir.x));
               tDelta.y = sqrt(1 + (rayDir.x*rayDir.x)/(rayDir.y*rayDir.y) + (rayDir.z*rayDir.z)/(rayDir.y*rayDir.y));
               tDelta.z = sqrt(1 + (rayDir.x*rayDir.x)/(rayDir.z*rayDir.z) + (rayDir.y*rayDir.y)/(rayDir.z*rayDir.z));
 
               if (rayDir.x < 0)
               {
                    step.x = -1;
                    tMax.x = (cam.pos.x - floor(cam.pos.x)) *  tDelta.x;
               }
               else
               {
                    step.x = 1;
                    tMax.x = (ceil(cam.pos.x) - cam.pos.x) *  tDelta.x;
               }

               if (rayDir.y < 0)
               {
                    step.y = -1;
                    tMax.y = (cam.pos.y - floor(cam.pos.y)) *  tDelta.y;
               }
               else
               {
                    step.y = 1;
                    tMax.y = (ceil(cam.pos.y) - cam.pos.y)  *  tDelta.y;
               }

               if (rayDir.z < 0)
               {
                    step.z = -1;
                    tMax.z = (cam.pos.z - floor(cam.pos.z))  *  tDelta.z;
               }
               else
               {
                    step.z = 1;
                    tMax.z = (ceil(cam.pos.z) - cam.pos.z) *   tDelta.z;
               }

               //Second part of 3DDDA Algorithm starts here: let the ray move on until it hits a filled cube

               int hitSide; //0=none(outside world), 1=x, 2=y, 3=z
               int hitCubeShape; //wat soort kubus geraakt is, 0 = geen, 1 = muur, 2 = blok, enz
               int hit = 0; //bepaalt of iets geraakt is; binnen of buiten array.

               while (hit == 0)
               {
                   if (world[cube.x][cube.y][cube.z].shape > 0)
                   {
                      hit = 1;
                      hitCubeShape = world[cube.x][cube.y][cube.z].shape;
                      break;
                   }
                   if (tMax.x < tMax.y)
                   {
                      if (tMax.x < tMax.z)
                      {
                         cube.x += step.x;
                         tMax.x = tMax.x + tDelta.x;
                         hitSide = 1;
                      }
                      else
                      {
                         cube.z += step.z;
                         tMax.z = tMax.z + tDelta.z;
                         hitSide = 3;
                      }
                   }
                   else
                   {
                      if (tMax.y < tMax.z)
                      {
                         cube.y += step.y;
                         tMax.y = tMax.y + tDelta.y;
                         hitSide = 2;
                      }

                      else

                      {
                         cube.z += step.z;
                         tMax.z = tMax.z + tDelta.z;
                         hitSide = 3;
                      }
                   }
               }

               //End of the 3DDDA Algorithm; we now know what cube was hit and what side of it was hit

               //Now we calculate the color of the pixel that will be placed on screen.
               //There are differend kinds of cubes (voxel.shape), here follows the calculation of the color depending on what kind of cube was hit.

               char screenR;
               char screenG;
               char screenB;

               switch (hitCubeShape)
               {
               //Shape 0 = empty cube, but an empty cube can never be hit according to the 3DDDA algorithm

               //Shape 1 = cube with a single color determinated by R, G and B
               case 1:
                    screenR = world[cube.x][cube.y][cube.z].r;
                    screenG = world[cube.x][cube.y][cube.z].g;
                    screenB = world[cube.x][cube.y][cube.z].b;
                    break;

               //Shape 2 = cube with 3 different colors for X, Y and Z side. Colors are Cyan, Yellow and Magenta.
               //This sort of cube is a "block" and can be placed and removed
               case 2:
                    if (hitSide == 1)
                    {
                       screenR = world[cube.x][cube.y][cube.z].r;
                       screenG = world[cube.x][cube.y][cube.z].g;
                       screenB = 0;
                    }
                    else if (hitSide == 2)
                    {
                       screenR = 0;
                       screenG = world[cube.x][cube.y][cube.z].g;
                       screenB = world[cube.x][cube.y][cube.z].b;
                    }
                    else
                    {
                       screenR = world[cube.x][cube.y][cube.z].r;
                       screenG = 0;
                       screenB = world[cube.x][cube.y][cube.z].b;
                    }
                    break;

               //Shape 3 = cube with 3 different colors for X, Y and Z side. Colors are Red, Green and Blue.
               case 3:
                    if (hitSide == 1)
                    {  
                       screenR = world[cube.x][cube.y][cube.z].r;
                       screenG = 0;
                       screenB = 0;
                    }
                    else if (hitSide == 2)
                    {
                       screenR = 0;
                       screenG = world[cube.x][cube.y][cube.z].g;
                       screenB = 0;
                    }
                    else
                    {
                       screenR = 0;
                       screenG = 0;
                       screenB = world[cube.x][cube.y][cube.z].b;
                    }
                    break;
               //Shape 4 = cube with 3 different colors for X, Y and Z side. Colors are 3 different grey values.
               case 4:
                    if (hitSide == 1)
                    {
                       screenR = world[cube.x][cube.y][cube.z].r;
                       screenG = world[cube.x][cube.y][cube.z].r;
                       screenB = world[cube.x][cube.y][cube.z].r;
                    }
                    else if (hitSide == 2)
                    {
                       screenR = world[cube.x][cube.y][cube.z].g;
                       screenG = world[cube.x][cube.y][cube.z].g;
                       screenB = world[cube.x][cube.y][cube.z].g;
                    }  
                    else
                    {
                       screenR = world[cube.x][cube.y][cube.z].b;
                       screenG = world[cube.x][cube.y][cube.z].b;
                       screenB = world[cube.x][cube.y][cube.z].b;
                    }
                    break;

               //Shape 5 = Cube with fake EnvironmentMap effect: color depends on location of the pixel
               case 5:
                    screenR = 255;
                    screenG = static_cast<int>(255*u/uDraw+128);
                    screenB = static_cast<int>(255*v/vDraw+128);
                    break;

               //Shape 6 = Cube with random colored noise
               case 6:
                    screenR = static_cast<int>(255*rand1());
                    screenG = static_cast<int>(255*rand1());
                    screenB = static_cast<int>(255*rand1());
                    break;

               //Shape 7 = Cube with random black and white noise
               case 7:
                    screenR = static_cast<int>(255*rand1());
                    screenG = screenR;
                    screenB = screenR;
                    break;

               //Shape 8 = Cube with random red noise and a fake blue EnvironmentMap
               case 8:
                    screenR = static_cast<int>(255*rand1());
                    screenG = static_cast<int>(255*u/uDraw+128);
                    screenB = static_cast<int>(255*u/uDraw+128);
                    break;

               //Shape 9 = Cube with a color that depends on the position of the camera
               case 9:
                    screenR = static_cast<char>(4*cam.pos.x);
                    screenG = static_cast<char>(4*cam.pos.y);
                    screenB = static_cast<char>(4*cam.pos.z);
                    break;
               }

               //Now the raytracing of one pixel is done, the color is calculated, so we can finally draw the pixel!

               drawPixel(static_cast<int>(screenWidth/2+u),static_cast<int>(screenHeight/2-v),screenR,screenG,screenB);

               //Move on to the next pixel

            }
       }
}







