/*
 * Copyright (C) 1999-2005  Terence M. Welsh
 *
 * This file is part of Lattice.
 *
 * Lattice is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 *
 * Lattice is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef CAMERA_H
#define CAMERA_H



#include <math.h>
//#include <fstream.h>



class camera{
public:
	//float projmat[16];
	float farplane;
	float cullVec[4][3];  // vectors perpendicular to viewing volume planes

	camera(){};
	~camera(){};
	void init(float* mat, float f);
	bool inViewVolume(float* pos, float radius);
};



#endif