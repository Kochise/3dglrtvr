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

#ifndef __INPUT_H__
#define __INPUT_H__

extern Uint8 SDL_GetRelativeMouseState (int *mouseX, int *mouseY);
extern SDL_GrabMode SDL_WM_GrabInput(SDL_GrabMode mode);
extern int SDL_ShowCursor(int toggle);
extern SDL_MouseMotionEvent *mouseMotion;
extern void SDL_WarpMouse(Uint16 x, Uint16 y);

void userInput();


#endif
