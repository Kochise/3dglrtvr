#ifndef __USEFUL_H__
#define __USEFUL_H__

/* 
* A simple little file to hold often used functions
*
* Author: Greg Coombe
* Date: Feb. 4th, 2001
*
*/
#include <stdlib.h>
#include <vec3f.hpp>
#include <assert.h>

/* Return a canonical random number: x in [0,1) */
float canonicalRandom();

/* Return a canonical random vector: x,y,z in [0,1) */
Vec3f canonicalRandomVec();

// Pack and unpack colors from vectors to ints.
Vec3<int> idToVec( int id );
int vecToId( Vec3<int> col );
int rgbToId( int r, int g, int b );

void sum4( Vec3f* result, const Vec3f* b1, const Vec3f* b2, const Vec3f* b3, 
          const Vec3f* b4, const int size );
void sumv( Vec3f* result, const Vec3f* b1, const int size );
void sumv( Vec3f* result, const Vec3f* b1, const Vec3f* b2, const int size );


// Funny thing: sometimes the canonicalRandom() returns a number 
// close enough to 1 that the floating point precision
// interprets it as 1, so the array is over indexed.
int getRandomIndex( int sizeOfArray );


#endif //__USEFUL_H__