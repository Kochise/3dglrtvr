#ifndef _TRAIN_
#define _TRAIN_

/*
 *
 * Train class.
 *
 * Your basic append-only, not-bounds safe, dynamic array.
 * Note: if debugging is turned on in the compiler, this will load
 * bounds checking.
 * 
 * Author: Greg Coombe
 * Date: Wed Nov 16 10:35:59 MST 1998
 *
 */

#include <assert.h>

#define INIT_SIZE 5

template<class T>
class Train {
  
public:
  Train() {
    item = new T[INIT_SIZE];
    allocated = INIT_SIZE;
    mySize = 0;
  }

  Train(int num_elements) {
    allocated = num_elements;
    item = new T[num_elements];
    mySize = 0;
  }
  
  // Try to conserve some time by not recopying the elements
  // if they are the same size
  Train(const Train& s) {
    if ( s.mySize == mySize && s.allocated == allocated ){
      for ( int i=0; i<mySize; i++ )
        item[i] = s.item[i];
    }
    else{
      allocated = s.size();
      mySize = s.size();
      item = new T[allocated];
      for ( int i=0; i<mySize; i++ )
        item[i] = s.item[i];
    }
  }
  
  ~Train() { delete[] item; }
  
  T& operator[](const int i) const { 
    //assert( i >= 0 && i < mySize );
    return item[i];
  }
  
  // Appends element to the end of the list
  int append(T a){
    if (mySize >= allocated){
      // size of item doubles each time more elements are needed.
      // cout << "array too small, increasing size to " << allocated*2 << endl;
      T* tmp = new T[allocated*2];
      for(int i=0; i<mySize; i++)
        tmp[i] = item[i]; 
      delete[] item;
      item = tmp;
      allocated*=2;
    }
    item[mySize++] = a;
    return (mySize-1);
  }
  
  int size() const { return mySize; }

  void shrink() {
    mySize--;
    if ( mySize < 0 ) mySize = 0;
  }
    
  /*
   * Swaps the two elements
   */
  void swap( int a, int b ){
    assert( a >= 0 && a < mySize  );
    assert( b >= 0 && b < mySize  );
    T temp = item[a];
    item[a] = item[b];
    item[b] = temp;
  }


  // Clears out the entire array and frees up memory
  void clear(){
    mySize = 0;
    delete[] item;
    allocated = INIT_SIZE;
    item = new T[allocated];
  }

  // return a copy of the Train as an array
  T* toArray(){
    T* copy = new T[mySize];
    for ( int i=0; i<mySize; i++ ){
      copy[i] = item[i];
    }
    return copy;
  }
    

  // Removes the excess empty elements off the end
  // of the array. A small memory effciency trick.
  void fit(){
  }

private:
  T* item;
  int allocated;
  int mySize;
};

#endif /* _TRAIN_ */