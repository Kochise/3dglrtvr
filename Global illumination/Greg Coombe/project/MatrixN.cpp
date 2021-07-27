#include "MatrixN.h"
#include "MElementPool.h"


MatrixN::MatrixN( int s ):size_(s){ 
  //s_ = new Train<MElement>[size_]; 
  s_ = new MElement*[ size_ ];
  for( int i=0;i<size_; i++)
    s_[i] = NULL;

  // Make a big pool of Matrix Elements
  mepool_ = new MElementPool( (int)(0.4*size_*size_ ));

  // Create the full matrix
  m_ = new Vec3f[ size_*size_ ];
  for( i=0; i<size_*size_; i++){
  //m_[i] = canonicalRandom();
    m_[i] = Vec3f::ZERO;
  }
  printf("Created a %dx%d matrix.\n", size_, size_ );
}	


// Cleans up the memory
MatrixN::~MatrixN(){delete[] s_; }


void MatrixN::reset(){
  delete[] s_;
  s_ = new MElement*[ size_ ];
  for( int i=0;i<size_; i++)
    s_[i] = NULL;

  for( i=0; i<size_*size_; i++){
  //m_[i] = canonicalRandom();
    m_[i] = Vec3f::ZERO;
  }
  printf("Created a %dx%d matrix.\n", size_, size_ );
}


// Fills the matrix with the identity
void MatrixN::identity(){
  /*
  for( int i=0; i<size_; i++)
    for( int j=0; j<s_[i].size(); j++)
      if ( s_[i][j].col == i ) 
        s_[i][j].val = 1.0;
      else
        s_[i][j].val = 0.0;
        */
}    


//
// Adds one to the specified value. Does some bounds checking.
// Note that since this is a sparse matrix, we have to check whether
// we have an element already listed for this column. If we do, then
// just increment the values. Otherwise, append a new MElement.
//
void MatrixN::increment( int i, int j, Vec3f inc ){ 
  assert( i < size_ && j < size_ );
  
  m_[ index(i,j) ] += inc;
  /*

  MElement* ptr = s_[i];
  while ( ptr ){
    if ( ptr->col_ == j ){
      ptr->val_ += inc;
      return;
    }
    ptr = ptr->next_;
  }
  */
  // Looks like we need to add a new element. Using the linked list
  // just add it on the front.
  /*
  MElement* me = new MElement( j, inc );
  */
  /*
  MElement* me = mepool_->getMElement();
  me->col_ = j;
  me->val_ = inc;

  MElement* second = NULL;
  if ( s_[i] == NULL )
    s_[i] = me;
  else{
    second = s_[i]->next_;
    s_[i]->next_ = me;
    me->next_ = second;
  }
  */
}


// 
// Collapses a matrix in full format to a sparse 
// format
//
void MatrixN::sparsify(){
  // For each row
  for( int i=0; i<size_; i++){
    for( int j=0; j<size_; j++){
      if ( m_[ index(i,j) ].LengthSqr() != 0.0 ){
        MElement* me = mepool_->getMElement();
        me->col_ = j;
        me->val_ = m_[ index(i,j) ];
        
        MElement* second = NULL;
        if ( s_[i] == NULL )
          s_[i] = me;
        else{
          second = s_[i]->next_;
          s_[i]->next_ = me;
          me->next_ = second;
        }
      }
    }
  }
  
}


//
// Squares the matrix by mutliplying it by itself.
//
void MatrixN::square(){
  //clock_t tstart, tend;
  //float temp;
  
  /*
  // Create a new matrix (can't square a matrix in place!)
  float* c = new float[size_*size_];   
  
   // Time the matrix multiply
   tstart = clock();
   
    for( int i=0; i<size_; i++){
    for( int j=0; j<size_; j++){
    temp = 0.0;
    for( int k=0; k<size_; k++){
    temp += m_[index(i, k)]*m_[index(k, j)];
    }
    c[index(i, j)] = temp;
    }
    }
    tend = clock();
    float elapsed = (tend-tstart) / (float)CLOCKS_PER_SEC;
    cout << "\t" << elapsed << " secs - " << size_ << "x" << size_ << endl;
    
     // Now copy in our new matrix and delete the old one
     delete[] m_;
     m_ = c;
  */
}



// Multiplies this matrix by the vector v and put the result in r  
void MatrixN::apply( Vec3f* r,  const Vec3f* v ){
  Vec3f row;
 
  for( int i=0; i<size_; i++){
    row = Vec3f::ZERO;
    MElement* ptr = s_[i];
    while( ptr ){
      row.x += v[ ptr->col_ ].x * ptr->val_.x;
      row.y += v[ ptr->col_ ].y * ptr->val_.y;
      row.z += v[ ptr->col_ ].z * ptr->val_.z;
      ptr = ptr->next_;
    }
    r[i] = row;
  } 
}

//
// Prints entire matrix to screen
//
void MatrixN::print(){
  cout.precision( 6 );
  for( int i=0; i<size_; i++){
    MElement* ptr = s_[i];
    while( ptr ){
      cout << "[" << i << ", " << ptr->col_ << "] = " 
        << ptr->val_ << endl;
      ptr = ptr->next_;
    }
    
    cout << endl;
  }
}


//
// Prints a row to the screen
//
void MatrixN::printRow( int i ){
  MElement* ptr = s_[i];
  while( ptr ){
    cout << "[" << i << ", " << ptr->col_ << "] = " 
      << ptr->val_ << endl;
    ptr = ptr->next_;
  }
  cout << endl;
}


void MatrixN::dump( char* filename ){
  cout << "Dumping matrix to " << filename << endl;
  
  // Open the file.
  fstream out_file( filename, ios::out );
  
  // Now just dump the matrix information to the file.
  // This is just like the "print()" function.
  cout.precision( 6 );
  for( int i=0; i<size_; i++){
    for( int j=0; j<size_; j++){
      
      bool found = false; 
      MElement* ptr = s_[i];
      while( ptr ){
        if ( j == ptr->col_ ){
          out_file << ptr->val_.x  << " " << ptr->val_.y  << " " << ptr->val_.z  << " ";
          found = true;
        }
        ptr = ptr->next_;
      }
      if (!found)
        out_file << "0.0 0.0 0.0 ";
    }
    out_file << endl;
  }

  out_file.close();
}


void MatrixN::load( char* filename ){
  cout << "Loading matrix from " << filename << endl;
  Vec3f val;
  
  // Open the file.
  fstream in_file( filename, ios::in );

  // Now just dump the matrix information to the file.
  // This is just like the "print()" function.
  for( int i=0; i<size_; i++){
    for( int j=0; j<size_; j++){
      in_file >> val.x >> val.y >> val.z;
      m_[ index(i,j) ] = val;
    }
  }

  in_file.close();

  sparsify();
}


