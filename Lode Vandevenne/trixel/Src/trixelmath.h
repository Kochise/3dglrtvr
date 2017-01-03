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
This file contains two vector structs.  vector is a 3-dimensional vector with
doubles as coordinates.  intvector is a 3-dimensional vector with ints as
coordinates.  The intvector is needed for the fast raycasting algorithm.  If the
world is less than 256*256*256 units, you can use chars instead of ints.
*/


#ifndef __TRIXELMATH_H__
#define __TRIXELMATH_H__

struct vector {
  double x;
  double y;
  double z;
};

struct intvector {
  char x;
  char y;
  char z;
};

vector rotateAroundArbitrary(vector v, vector l, double a);  //rotate a vector around an arbitrary axis for a radians
vector quickVector(double x, double y, double z); //get a vector without having to declare one
double distance3D(vector v, vector w); //get the distance in 3D between two vectors
double length3D(vector v); //get the length of a vector
vector minus(vector v); //returns the opposite of a vector
vector normalize(vector v); //returns the normalized vector
vector substract(vector v, vector w);  //substract two vectors
vector add(vector v, vector w); //add two vectors
vector multiplyBy(vector v, double a); //multiply a vector by a scalar
double min3 (double a, double b, double c); //return the minimum of 3 doubles
double max3 (double a, double b, double c); //return the maximul of 3 doubles
double rand1(); //get a random number between 0 and 1

#endif
