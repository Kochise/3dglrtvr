#include "illuminate.h"

static int tri_cam_num = 0;
int width_ = 256;
int height_ = 256;

const int vport_width = 256;
const int vport_height = 256;


// Some vectors for storage
Vec3f* b1, *b2, *b3, *b4;
Vec3f* initial;
Vec3f* result;


//
// These vectors are used to store the results of the
// matrix-multiply operations in illuminate()
//
void init( int size ){
  b1 = new Vec3f[ size ];
  b2 = new Vec3f[ size ];
  b3 = new Vec3f[ size ];
  b4 = new Vec3f[ size ];
  initial = new Vec3f[ size ];
  result = new Vec3f[ size ];
}  


//
//
// Compute the form factor matrix for the scene using hemicubes
// 
//
void visibility(){

  // switch to alternate cam
  glvu.SelectCam( 2 ); 
  glViewport(0, 0, vport_width, vport_height);  

  float* tris = NULL;
  float* normals = NULL;
  
  Camera* vertex_cam = new Camera();
  
  GLModel* gm = (GLModel*)Obj;
  MatrixN* matrix = gm->matrix_;
  
  // For now, hardcode the size of the image that we are rendering. This may
  // change if in the future.
  GLubyte pixels[ vport_width * vport_height ][ 3 ];
  
  // Do setup for lighting calculations
  glDisable( GL_LIGHTING );
  
  // Make sure to read from the back buffer (saves time)
  glReadBuffer( GL_BACK );
  
  // Start a timer
  clock_t tstart, tend;
  tstart = clock();
  
  float inc = 0.5*1.0/(float)(vport_width*vport_height);
  //Vec3f increment( inc, inc, inc );
  int size = gm->slist_.size();
  
  // for each triangle in the mesh
  for( int i=0; i<size; i++){
    
    // Grab the next vertex and place a camera at that vertex
    GSample* point = gm->slist_[i];
    getCam( point, vertex_cam );
    glvu.SetCurrentCam( vertex_cam );
    
    // Render our scene from the new camera perspective
    glvu.BeginFrame();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Obj->Display( OBJ_NONE );
    
    // Read back the image, convert the colors to ids, and load matrix.
    glReadPixels( 0, 0, vport_width, vport_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    for( int p=0; p<vport_width*vport_height; p++){
      int id = rgbToId( pixels[p][0], pixels[p][1], pixels[p][2] );
      if ( id != 0 ){
        int j = id - 1; // compensate for stupid 1-off error
        
        // Increment our matrix thusly: The probability that light travels
        // from polygon i to polyon j is the percent of hemisphere around
        // polygon i covered by polygon j.
        // However, remember that we have several samples per polygon. So
        // we have to distribute light to all of them
        for( int k=0; k<sample_freq; k++){
          //for( int k2=0; k2<sample_freq; k2++){
          int s_id_i = gm->tris_[i]->s_[k];
          int s_id_j = gm->tris_[j]->s_[k];
          matrix->increment(s_id_i, j, gm->slist_[s_id_i]->color_*inc);
          matrix->increment(s_id_j, i, gm->slist_[s_id_j]->color_*inc);

          matrix->increment(j, s_id_i, gm->slist_[s_id_i]->color_*inc);
          matrix->increment(i, s_id_j, gm->slist_[s_id_j]->color_*inc);
          //}
        }
        
      }
    }
    
    //matrix->printRow( i );
  }
  
  // Normalize the matrix (divide by the number of pixels)
  //printf("Normalizing matrix...\n");
  //matrix->normalize( vport_width*vport_height );
  
  printf("Visibility matrix:\n");
  //matrix->print();
  //matrix->dump("output.dat");
  
  /*
  // This method is slow and ineffcient. Instead, of squaring the matrix,
  // we can repeatedly apply the initial distribution ala Horner's Rule.
  printf("Squaring matrix...\n");
  matrix->square();
  printf("Squaring matrix...\n");
  matrix->square();
  */
  
  tend = clock();
  float elapsed = (tend-tstart) / (float)CLOCKS_PER_SEC;
  cout << "\tThe total elapsed time for visibility is " << elapsed << " secs." << endl;
  
  matrix->sparsify();
  
  tend = clock();
  elapsed = ((tend-tstart) / (float)CLOCKS_PER_SEC) - elapsed;
  cout << "\tThe total elapsed time for sparsify is " << elapsed << " secs." << endl;

  // switch back to original cam and reset the viewport
  glvu.SelectCam( 0 ); 
  glViewport(0, 0, width_, height_);  

}


void illuminate(){
  GLModel* gm = (GLModel*)Obj;
  MatrixN* matrix = gm->matrix_;
  int size = gm->slist_.size();

  clock_t tstart, tend;
  
  // Time the matrix multiply
  tstart = clock();


  for( int ii=0; ii<size; ii++){
    //b1[ ii ] = Vec3f::ZERO;
    //b2[ ii ] = Vec3f::ZERO;
    //b3[ ii ] = Vec3f::ZERO;
    //b4[ ii ] = Vec3f::ZERO;
    //result[ ii ] = Vec3f::ZERO;
    initial[ii] = gm->slist_[ii]->initial_;
  }

  cout << "Vectors are " << size << "x1" << endl;

  // Repeatedly apply the matrix to the initial distribution. 
  matrix->apply( b1, initial);
  matrix->apply( b2, b1 );
  sumv( result, b1, b2, size );
  matrix->apply( b1, b2 );
  sumv( result, b1, size );
  matrix->apply( b2, b1 );
  sumv( result, b2, size );
  
  /*// Repeatedly apply the matrix to the initial distribution. 
  matrix->apply( b1, initial );
  matrix->apply( b2, b1,);
  matrix->apply( b3, b2 );
  matrix->apply( b4, b3 );

  // Sum all of the vectors together
  sum4( result, b1, b2, b3, b4, size );
  */


  //
  // Now we add all of the contributions from all of the lights. This involves adding
  // the energy transmitted between polygons. For instance, suppose that the ith-row of
  // the matrix looks like:
  //
  // [ 0.0 0.1 0.7 0.0 0.1]
  //
  // So this says that 0.0 percent of light from polygon 0 reaches polygon i, 0.1 percent of light 
  // from polygon 1 reaches polygon i, etc. To approximate
  // this, we just scale the color of polygon j and add it to polygon i.
  /*
  for( int row = 0; row<gm->num_tris_; row++){
    for( int col = 0; col<gm->num_tris_; col++){
      gm->accumulate( row, col, matrix->get(row,col) );
    }
    
  }
  */
  gm->distribute( result );

  tend = clock();
  float fps = (float)CLOCKS_PER_SEC/(tend-tstart);
  cout << "\t" << fps << " fps - " << size << "x1" << endl;

  /*
  delete[] b1;
  delete[] b2;
  delete[] b3;
  delete[] b4;
  delete[] initial;
  delete[] result;
  */
}



//
// The idea of this function is to dynamically re-color the
// scene. Since we have already computed the visibility (independent
// of lighting) we should be able to simply apply this matrix to
// our new lighting.
//
void set_colors( const Vec3f color ){
  cout << "Got color " << color << endl;
  GLModel* gm = (GLModel*)Obj;
  gm->generateLightTable( color );
  illuminate();
}

//
// The idea of this function is to dynamically relight the
// scene. Since we have already computed the visibility (independent
// of lighting) we should be able to simply apply this matrix to
// our new lighting.
//
void set_lights(){
  cout << "New light" << endl;
  GLModel* gm = (GLModel*)Obj;
  gm->generateLightTable( Vec3f(1.0, 1.0, 1.0), true );
  illuminate();
}

//
// Move an object around. This is kind of tricky.
// Have to recalculate the visibility and lighting every time
//
void move_objects( const Vec3f dist ){
  cout << "Moving objects" << endl;
  GLModel* gm = (GLModel*)Obj;
  gm->move( dist );
  gm->matrix_->reset();
  visibility();
  illuminate();
}


//
//
// Returns a vector which is non-colinear with
// this one. Just adds 1 to the smallest element,
// Pretty slow.
//
Vec3f getNonColinear( Vec3f a ){
  float xf = fabs( a.x );
  float yf = fabs( a.y );
  float zf = fabs( a.z );

  if ( xf < yf && xf < zf )
    return Vec3f( (float)(a.x+1.0+canonicalRandom()), a.y, a.z );
  else if ( yf < zf )
    return Vec3f( a.x, (float)(a.y+1.0+canonicalRandom()), a.z );
  else
    return Vec3f( a.x, a.y, (float)(a.z+1.0+canonicalRandom()) );
}


//
// Does all of the work of setting the camera parameters based on the normal
// of the vertex, etc.
//
void getCam( const GSample* point, Camera* cam ){
  Vec3f lookat = point->pos_ + 5.0*point->normal_;
  Vec3f tmp = getNonColinear( point->normal_ );
  tmp.Normalize();
  Vec3f leftv = point->normal_ ^ tmp;

  //printf("Look from"); vert->pos_.Print();
  //printf("Look at"); lookat.Print();
  //printf("Side"); leftv.Print();
  
  cam->LookAt( point->pos_, lookat, leftv );
  // HACK HACK HACK
  //cam->Perspective( 90.0, 1.0, 1.0, 1000.0 );
  cam->Perspective( 120.0, 1.0, .10, 1000.0 );
}


//
// Given the vertex number, converts this into a vertex pointer, creates
// a new camera, then calls getCam() to load up the parameters. This function
// exists primarily for demonstartion purposes.
//
void getVertexCam(){
  glDisable( GL_LIGHTING );

  GLModel* gm = (GLModel*)Obj;
  Camera* tri_cam = new Camera(); 
  
  printf("Camera at vertex %d\n", tri_cam_num);
  GSample* point = gm->slist_[tri_cam_num ];
  
  // Call real function to load all of the camera parameters
  getCam( point, tri_cam );

  // Tell the scene to set this camera as the viewing camera
  glvu.SetCurrentCam( tri_cam );

  // Render one frame in frame buffer
  glvu.BeginFrame();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Obj->Display( OBJ_NONE );
    Obj->Display();

  glvu.EndFrame();


  // Increment to the next camera
  tri_cam_num++;
  if ( tri_cam_num == ((GLModel*)Obj)->slist_.size() ) tri_cam_num = 0;
}

