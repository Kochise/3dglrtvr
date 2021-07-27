#ifndef __MELEMENTPOOL_H__
#define __MELEMENTPOOL_H__

//
//
// This is a simple class to manage a pool of
// MElements. The idea is to create a bunch of 
// MElements upon initialization, then pass these out
// as needed. Because they are all allocted near to each
// other, this should impose some structure on the
// memory accesses.
//
// Author: Greg Coombe
// Date: April 16, 2001 - Tax Day!
//


#include "MatrixN.h"

#define DEFAULT_POOL_ 300

class MElementPool{
public:
  MElementPool(){
    pool_ = new MElement*[ DEFAULT_POOL_ ];
    size_ = DEFAULT_POOL_;
    curr_ = 0;
    for( int i=0; i< size_; i++)
      pool_[i] = new MElement();
  }

  MElementPool( const int hint ){
    pool_ = new MElement*[ hint ];
    size_ = hint;
    curr_ = 0;
    for( int i=0; i< size_; i++)
      pool_[i] = new MElement();
    cout << "Created MElementPool of " << hint << " elements." << endl;
  }

  
  // Get the next element. If we are beyond the pool,
  // just create a element.
  MElement* getMElement(){
    if ( curr_ < size_ ){
      //cout << "Accessing element #" << curr_-1 << endl;
      return pool_[curr_++ ];
    }
    else {
      //cout << "Exceeded MElementPool. Creating new MElement\n";
      return new MElement();
    }
  }
  
  
private:
  MElement** pool_; 
  int size_;
  int curr_;
};

#endif //__MELEMENTPOOL_H__