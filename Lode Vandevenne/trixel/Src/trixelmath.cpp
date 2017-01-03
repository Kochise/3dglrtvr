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
This file contains all the math that was needed for Trixel.  These are mainly
operations with a simple vector struct.
This can be improved with a better vector class and operator overloading.
*/


#include <math.h>
#include "trixelmath.h"
#include <time.h>
#include <stdlib.h>

//Rotate vector v around arbitrary axis l for a radians
/////////////////////////////////////////////////////
//Used Matrix
/////////////////////////////////////////////////////
//
//      [tx²+c   txy+sz   txz-sy   0]
//  R = [txy-sz  ty²+c    tyz+sx   0]
//      [txz+sy  tyz-sx   tz²+c    0]
//      [0       0        0        1]
//
//  where c = cos(a), s = sin(a), t = 1-cos(a)
//        l = unit vector on rotation axis
//        a = angle of rotation
//        x, y and z are the coordinates of v
//
//  final formula: w = R*v
//
//  where w = returned vector, v = given vector
//
////////////////////////////////////////////////////

vector rotateAroundArbitrary(vector v, vector l, double a)
{



    vector w;
    double c, s, t;

    l = normalize(l);

    c = cos(a);
    s = sin(a);
    t = 1 - c;

    w.x = (t*l.x*l.x +     c)*v.x + (t*l.x*l.y + s*l.z)*v.y + (t*l.x*l.z - s*l.y)*v.z;
    w.y = (t*l.x*l.y - s*l.z)*v.x + (t*l.y*l.y + c    )*v.y + (t*l.y*l.z + s*l.x)*v.z;
    w.z = (t*l.x*l.z + s*l.y)*v.x + (t*l.y*l.z - s*l.x)*v.y + (t*l.z*l.z +     c)*v.z;

    w.x = w.x*length3D(v)/length3D(w);
    w.y = w.y*length3D(v)/length3D(w);
    w.z = w.z*length3D(v)/length3D(w);

    return w;
}

//calculates distance between two vectors.
double distance3D(vector v, vector w)
{
    double d;
    d = sqrt((v.x - w.x)*(v.x - w.x) +  (v.y - w.y)*(v.y - w.y)+  (v.z - w.z)*(v.z - w.z));
    return d;
}

//calculates length of a vector.
double length3D(vector v)
{
    double l;
    l = sqrt(v.x*v.x +  v.y*v.y +  v.z * v.z);
    return l;
}

//Quicly get a vector with asked coordinates
vector quickVector(double x, double y, double z)
{
    vector v = {x,y,z};
    return v;
}

//Returns the opposite vector
vector minus(vector v)
{
    vector w;
    w.x=-v.x;
    w.y=-v.y;
    w.z=-v.z;
    return w;
}

//Give a vector length 1
vector normalize(vector v)
{
    vector w;
    double l;
    l = length3D(v);
    w.x = v.x/l;
    w.y = v.y/l;
    w.z = v.z/l;
    return w;
}

//Substract two vectors
vector substract(vector v, vector w)
{
    vector u;
    u.x = v.x-w.x;
    u.y = v.y-w.y;
    u.z = v.z-w.z;
    return u;
}

//Add two vectors
vector add(vector v, vector w)
{
    vector u;
    u.x = v.x+w.x;
    u.y = v.y+w.y;
    u.z = v.z+w.z;
    return u;
}

//Multiplies a vector by a double
vector multiplyBy(vector v, double a)
{
    vector w;
    w.x = v.x*a;
    w.y = v.y*a;
    w.z = v.z*a;
    return w;
}

//Quicly calculates the maximum of 3 doubles
double max3 (double a, double b, double c)
{
    if(b>a) a = b;
    if(c>a) a = c;
    return a;
}

//Quickly calculates the mininum of 3 doubles
double min3 (double a, double b, double c)
{
    if(b<a) a = b;
    if(c<a) a = c;
    return a;
}

//Gives a random number between 0 and 1
double rand1()
{
    double randomNumber;
    randomNumber = rand();
    randomNumber /=32768;
    return randomNumber;
}
