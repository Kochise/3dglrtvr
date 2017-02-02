#ifndef __MATRIXN_H__
#define __MATRIXN_H__

/*
 *
 * matrix_mult
 *
 * Purpose:
 *  An NxN matrix class.  
 *
 * Author: Greg Coombe
 * Date: Fri Feb  2 18:18:26 2001
 *
*/

#include <fstream.h>
#include <time.h>

#include "Useful.h"
#include "Train.h"
//#include "MElementPool.h"
class MElementPool;

#define index(a,b) ( a + b*size_ )


class MElement{
public:
  MElement( int c = -1, Vec3f v = Vec3f::ZERO ){ 
    col_ = c; val_ = v; next_ = NULL; 
  }

  int col_;
  Vec3f val_;
  MElement* next_;
};

class MatrixN {
public:

  /* Constructor/destructor */
  MatrixN( int s );
  ~MatrixN();
  
  // Resets the matrix to the identity matrix
  void identity();
  
  // Squares the matrix
  void square();
  
  // Multiplies this matrix by the vector v and put the result in r  
  void apply( Vec3f* r, const Vec3f* v );
  
  /* Dumps the matrix to the screen */
  void print();
  void printRow( int i );

  // Dumps matrix to a file
  void dump( char* filename );
  void load( char* filename );

  void sparsify();

  /*
  float get( int i, int j){ return m_[ index(i,j) ]; }
  // allows access as an array.
  const float operator [] (const int i) const { 
    return( m_[i] ); 
  }
  float operator [] (const int i){ 
    return( m_[i] ); 
  }
  */

  //
  // Adds one to the specified value. Does some bounds checking.
  //
  void increment( int i, int j, Vec3f v = Vec3f(1.0f, 1.0f, 1.0f) );

  void reset();

  int size(){ return size_; }

private:

  Vec3f* m_;

  MElement** s_;

  // Defines the size_*size_ matrix
  const int size_;

  // A pool of MElements
  MElementPool* mepool_;
};


#endif //__MATRIXN_H__
