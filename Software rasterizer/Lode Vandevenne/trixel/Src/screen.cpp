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
This file contains all functions related to the screen.  These include
setting up the SDL surface, plotting a pixel and clearing the screen.
*/

#include "screen.h"

SDL_Surface *screen;

void setScreen(int width, int height, int colorDepth)
{
  if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    printf("Unable to init SDL: %s\n", SDL_GetError());
    SDL_Quit;
    exit(1);
  }
  atexit(SDL_Quit);
#ifdef FULLSCREEN
  screen=SDL_SetVideoMode(width,height,colorDepth,SDL_HWSURFACE|SDL_FULLSCREEN);
#else
  screen=SDL_SetVideoMode(width,height,colorDepth,SDL_HWSURFACE|SDL_DOUBLEBUF);
#endif
  if ( screen == NULL )
  {
    printf("Unable to set video: %s\n", SDL_GetError());
    SDL_Quit;
    exit(1);
  }
}


void Slock()
{
  if ( SDL_MUSTLOCK(screen) )
  {
    if ( SDL_LockSurface(screen) < 0 )
    {
      return;
    }
  }
}

void Sulock()
{
  if ( SDL_MUSTLOCK(screen) )
  {
    SDL_UnlockSurface(screen);
  }
}


//this function was grabbed from the SDL homepage, http://www.libsdl.org/index.php and can be improved by removing all cases except the last one (32 bit) or making it inline.
void drawPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  Uint32 color = SDL_MapRGB(screen->format, R, G, B);
  switch (screen->format->BytesPerPixel)
  {
    case 1: // Assuming 8-bpp
      {
        Uint8 *bufp;
        bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
        *bufp = color;
      }
      break;
    case 2: // Probably 15-bpp or 16-bpp
      {
        Uint16 *bufp;
        bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
        *bufp = color;
      }
      break;
    case 3: // Slow 24-bpp mode, usually not used
      {
        Uint8 *bufp;
        bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
        if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
          bufp[0] = color;
          bufp[1] = color >> 8;
          bufp[2] = color >> 16;
        } else {
          bufp[2] = color;
          bufp[1] = color >> 8;
          bufp[0] = color >> 16;
        }
      }
      break;
    case 4: // Probably 32-bpp
      {
        Uint32 *bufp;
        bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
        *bufp = color;
      }
      break;
  }
}

void updateScreen(int u,int v,int width,int height)
{
    SDL_UpdateRect(screen,u,v,width,height);
}

void clearScreen()
{
    for (int x=0;x<screenWidth;x++)
    {
     for (int y = 0; y<screenHeight;y++)
     {
            drawPixel(x,y,0,0,0);
     }
    }
}
