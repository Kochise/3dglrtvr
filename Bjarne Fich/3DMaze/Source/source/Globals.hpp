#ifndef __GLOBALS_HPP_
#define __GLOBALS_HPP_

/*
  Globals to be used by several places
  Most is initialized by WinMain
*/

// Set this to invoke debug, logs etc...
//#define DEBUG_MODE

#include "GLWindow.hpp"
extern OPENGLWINDOW *glWindow;

#include "Texture.hpp"
extern TEXTURELIST *TextureList;

#define FLAG_WALL_SOLID 0
#define FLAG_WALL_LINES 1
#define FLAG_WALL_TEXTURES 2
#define FLAG_HELP_ARROWS 3
#define FLAG_GROUND 4
#define FLAG_SKY 5
#define FLAG_AUTOLEVEL 6
#define FLAG_DEBUG 7
#define FLAGS_END 8
extern bool GlobalMazeFlags[FLAGS_END];

#endif // __GLOBALS_HPP_

