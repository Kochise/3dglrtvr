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
For an explanation on the voxel struct, see file world.cpp.
*/


#ifndef __WORLD_H__
#define __WORLD_H__

struct voxel {
  char r;
  char g;
  char b;
  char shape;
};



void buildWorld();
void buildWorldWithSPikes();
void buildStarFieldWorld();
void buildIntroWorld();
void buildRasterWorld();
void buildCubeMaze();
void buildSphereWorld();

#endif
