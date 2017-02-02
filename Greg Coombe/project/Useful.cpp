#include "Useful.h"
#include <iostream.h>


/* Return a canonical random number: x in [0,1) */
float canonicalRandom(){
  double r = rand()/(double)(RAND_MAX);
  // double r = drand48();
  //assert( 0.0 <= r && r < 1.0 );
  if( r < 0.0 || r >= 1.0 )
    r = fabs( r/2.0 );

  return (float)r;
}


/* Return a canonical random vector: x,y,z in [0,1) */
Vec3f canonicalRandomVec(){
  Vec3f r( canonicalRandom(), canonicalRandom(), canonicalRandom() );
  //r.Print();
  return r;
}


// Funny thing: sometimes the canonicalRandom() returns a number 
// close enough to 1 that the floating point precision
// interprets it as 1, so the array is over indexed.
int getRandomIndex( int sizeOfArray ){
  int index = (int)(canonicalRandom()*(double)sizeOfArray);
  if ( index >= sizeOfArray ){
    cout << "Interesting! Overindex of the array!\n";
    index -= 1;
  }
  assert( 0 <= index && index < sizeOfArray );
  return index;
}


//
// Takes four RGB vectors and returns the sum of the vectors (channels are 
// independent) in the vector 'result'.
//
void sum4( Vec3f* result, const Vec3f* b1, const Vec3f* b2, const Vec3f* b3, 
          const Vec3f* b4, const int size ){
  for ( int j = 0; j<size; j++)
    result[ j ] = b1[ j ] + b2[ j ] + b3[ j ] + b4[ j ];
}

void sumv( Vec3f* result, const Vec3f* b1, const int size ){
  for ( int j = 0; j<size; j++)
    result[ j ] += b1[ j ];
}


void sumv( Vec3f* result, const Vec3f* b1, const Vec3f* b2, const int size ){
  for ( int j = 0; j<size; j++)
    result[ j ] = b1[ j ] + b2[ j ];
}

//
// Convert an int into an integer vector. This is some fancy bit operations
// to essentially pull out a RGB color from an id.
//
Vec3<int> idToVec( int id ){ 
  assert( id >= 0 );
  Vec3<int> c;
  c.x = ( id & 255 );
  c.y = ( (id >> 8 ) & 255);
  c.z = ( (id >> 16 ) & 255);
  //printf("ID %d converted to color <%d, %d, %d>\n", id, c.x, c.y, c.z);
  return c;
}

//
// Convert an integer vector into an int. This is some fancy bit operations
// to pack an RGB color into an id.
//
int vecToId( Vec3<int> col ){
  assert( col.x >= 0 && col.y >=0 && col.z >= 0 );
  int c = col.x + ( col.y << 8 ) + ( col.z << 16 );
  return c;
}

int rgbToId( int r, int g, int b ){
  return r + ( g << 8 ) + ( b << 16 );
}

