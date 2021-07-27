#include "GLModel.h"

static int light_num = 0;

GLModel::GLModel( char* filename ){
  load( filename );
  
  //light_energy_ = 4000.0; // Venus
  //light_energy_ = 5000.0; // hypersheet
  //light_energy_ = 1100.0; // cornell box
  light_energy_ = 1000.0; // cornell box w/sphere
  //light_energy_ = 2300.0; // occluder
  
  initialize();

  //print();
}

//
// Prints out the ith triangle
//
void GLModel::print( int i ){
   GVertex* v0 = verts_[tris_[i]->v_[0]];
   GVertex* v1 = verts_[tris_[i]->v_[1]];
   GVertex* v2 = verts_[tris_[i]->v_[2]];
   cout << "Pos: " << v0->pos_ << v1->pos_ << v2->pos_ << endl;
}


// 
// Accumulates colors on the sample points
//
/*
void GLModel::accumulate( int row, int col, float percent ){
  int tri_index = slist_[col]->my_tri_;
  Vec3f color = percent*(verts_[tris_[tri_index]->v_[0]]->color_);
  //Vec3f color = percent*(verts_[ col ]->color_);
  //slist_[row]->color_ += color;
 
}
*/

// 
// Distributes colors from sample points to the vertices.
//
void GLModel::distribute( Vec3f* color ){
  int i;
  //float scale = 1.0/750.0;
  float scale = 1.0/550.0;
  //float scale = 1.0/350.0;
  // First clear all of the colors
  for( i=0; i<verts_.size(); i++){
    verts_[i]->color_ = Vec3f::ZERO;
  }

  // Now distribute color from sample points to vertices based on distance
  for( i=0; i<slist_.size(); i++){ 
    //Vec3f color = slist_[i]->color_;
    Vec3f pos = slist_[i]->pos_;
    int tri_index = slist_[i]->my_tri_;
    
    //cout << "Sample point " << i << " color is " << color[i] << endl;
    
    for ( int k=0; k<3; k++){
      GVertex* v0 = verts_[tris_[tri_index]->v_[k]];
      //float dist = scale/(10*(v0->pos_ -  pos).Length());
      float dist = scale;// * slist_[i]->bary_[k];
      if ( slist_[i]->isLight_ )
        v0->color_ += dist*slist_[i]->initial_;
      else
        v0->color_ += dist*color[i];
    }
  }
  
  
}


//
// Generate normals. Assume equal areas. 
// Only generate these normals if they are not already set.
//
void GLModel::generateNormals(){
  // Run through each triangle and compute the cross product
  for( int i=0; i<tris_.size(); i++){  
    GVertex* v0 = verts_[tris_[i]->v_[0]];
    GVertex* v1 = verts_[tris_[i]->v_[1]];
    GVertex* v2 = verts_[tris_[i]->v_[2]];
    Vec3f n =  (v1->pos_ - v0->pos_ ) ^ ( v2->pos_ - v0->pos_ );
    n.Normalize();
    if ( v0->normal_ == Vec3f::ZERO )
      v0->normal_ = n;
    if ( v1->normal_ == Vec3f::ZERO )
      v1->normal_ = n;
    if ( v2->normal_ == Vec3f::ZERO )
      v2->normal_ = n;
  } 
}


//
// Generates a random sample point on a triangle. This method is presented by
// Shirley in "Realistic Ray Tracing". He credits Pattanaik's thesis.
//
// The number of points on a triangle is chosen in proportion to the area of
// the triangle. 
//
void GLModel::generateSamplePoints(){
  
  // Run through each triangle, compute the area using the cross product
  for( int i=0; i<tris_.size(); i++){  
    GVertex* v0 = verts_[tris_[i]->v_[0]];
    GVertex* v1 = verts_[tris_[i]->v_[1]];
    GVertex* v2 = verts_[tris_[i]->v_[2]];
    
    Vec3f edge1 = v1->pos_ - v0->pos_;
    Vec3f edge2 = v2->pos_ - v0->pos_;

    // The area of a triangle is approximately 1/2 the length of the 
    // cross-product of the edges. 
    float area = (edge1 ^ edge2).Length()/2.0;
    
    //int num_samples = area/1000.0;
    int num_samples = sample_freq;

    //cout << v0->pos_ << ", " << v1->pos_ << ", " << v2->pos_ << endl;

    for( int k=0; k<num_samples; k++){
      float rand1 = sqrt(1.0 - canonicalRandom());
      float rand2 = canonicalRandom();
      
      Vec3f p = v0->pos_ + rand2*rand1*edge1 + (1.0-rand1)*edge2;
      Vec3f n = (1.0/3.0)*(v0->normal_ + v1->normal_ + v2->normal_);
      GSample* s = new GSample( p, n, i, tris_[i]->color_ );
      int index = slist_.append( s );
      tris_[i]->s_[k] = index;

      // While we are here, we should compute the barycentric coordinates of 
      // the sample point
      Vec3f h_vec = p - v0->pos_;
      float area_small = 0.5*((p - v0->pos_) ^ edge1).Length();
      float gamma = area_small / area;  // Compute ratio of areas
      
      area_small = 0.5*((p - v0->pos_) ^ edge2).Length();
      float beta = area_small / area;   // Compute ratio of areas

      float alpha = 1.0 - ( gamma + beta );
      s->bary_ = Vec3f( alpha, beta, gamma );
      //cout << p << endl;
      //cout << "Barycentric coordinates - " << s->bary_ << endl;
    }
  }

  // Just for memory effciency, shrink the sample array to tightly fit
  // the data points.
  slist_.fit();
}


//
// Generate random colors.
//
void GLModel::generateColors(){
  int i = 0;

  // Compute an id color for each triangle
  for( i=0; i<tris_.size(); i++){
    // Get next color
    Vec3<int> current_id = idToVec( i+1 );
    tris_[i]->id_color_ = current_id;
  }

  // Compute an per-vertex color for each triangle
  for( i=0; i<tris_.size(); i++){
    Vec3f color = tris_[i]->color_;
    verts_[tris_[i]->v_[0]]->color_ = color;
    verts_[tris_[i]->v_[1]]->color_ = color;
    verts_[tris_[i]->v_[2]]->color_ = color;
  }
}


void GLModel::generateLightTable( Vec3f light_color, bool random ){
  int i,j;
 
  cout << "Light energy is at " << light_energy_ << endl;

  if ( !random ){
    // Create and initialize the light values in the sample list
    for( i=0; i<slist_.size(); i++){
      slist_[i]->initial_ = Vec3f::ZERO;
      slist_[i]->isLight_ = false;
      for( j=0; j<light_table_.size(); j++){
        if ( slist_[i]->my_tri_ == light_table_[j] ){
          slist_[i]->initial_ = light_energy_*light_color;
          slist_[i]->isLight_ = true;
        }
      }
    }
  }
  else{
    // Randomly choose a light out of the table.
    int index = getRandomIndex( light_table_.size() );
    // Cycle through the lights
    //int index = light_num++;
    //if ( light_num >= light_table_.size() ) light_num = 0;
    cout << "Picked light " << index << endl;
    for( i=0; i<slist_.size(); i++){
      slist_[i]->initial_ = Vec3f::ZERO;
      slist_[i]->isLight_ = false;
      if ( slist_[i]->my_tri_ == light_table_[index] ){
        slist_[i]->initial_ = 5.0*light_energy_*light_color;
        slist_[i]->isLight_ = true;
     }
    }
  }
}


//
// Generate random colors.
//
void GLModel::generateRandomColors(){
  int i = 0;
  // Run through each triangle and compute the color per vertex
  for( i=0; i<tris_.size(); i++){
    verts_[tris_[i]->v_[0]]->color_ = canonicalRandomVec();
    verts_[tris_[i]->v_[1]]->color_ = canonicalRandomVec();
    verts_[tris_[i]->v_[2]]->color_ = canonicalRandomVec();
  }
}


//
// Neat trick! Move some of the vertices
//
void GLModel::move( const Vec3f dist ){
  for( int i=0; i<verts_.size(); i++){
    if ( verts_[i]->isMoveable_ )
      verts_[i]->pos_ += dist;
  }
}


//
// The main display function for this class. Uses OpenGL calls to
// display the object on the screen.
//
void GLModel::Display(unsigned int Attribs){
  // save state
  glPushAttrib(GL_LIGHTING_BIT);
  // set new state
  glEnable(GL_COLOR_MATERIAL);
  
  int i =0;

  switch (Attribs){
  case OBJ_NONE :
    //printf("Id rendering...\n");
    glDisable( GL_LIGHTING );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glBegin( GL_TRIANGLES );
    for( i=0; i<tris_.size(); i++){
      GVertex* v0 = verts_[tris_[i]->v_[0]];
      GVertex* v1 = verts_[tris_[i]->v_[1]];
      GVertex* v2 = verts_[tris_[i]->v_[2]];
  
      Vec3<int> col = tris_[i]->id_color_;
      glColor3f( col.x/255.0, col.y/255.0, col.z/255.0 );
      glNormal3fv( v0->normal_ );
      
      glVertex3fv( v0->pos_ );      
      glVertex3fv( v1->pos_ );      
      glVertex3fv( v2->pos_ );
    }
    glEnd();
    
    /*
    // Now draw the sample points
    glPointSize( 5.0 );
    glBegin( GL_POINTS );
      glColor3f( 0, 0, 1.0 );
      for( i=0; i<tris_.size(); i++)
        glVertex3f( tris_[i]->sample_.x, tris_[i]->sample_.y, tris_[i]->sample_.z );
    glEnd();
    */

    break;
    
    case OBJ_COLORS:
    //printf("Color cycle mode...\n");
      /*
    glDisable( GL_LIGHTING );
    glBegin( GL_TRIANGLES );
    for( i=0; i<tris_.size(); i++){
      GVertex* v0 = verts_[tris_[i]->v_[0]];
      GVertex* v1 = verts_[tris_[i]->v_[1]];
      GVertex* v2 = verts_[tris_[i]->v_[2]];
  
      Vec3<int> col = tris_[i]->id_color_;
      glColor3f( col.x/255.0, col.y/255.0, col.z/255.0 );
      glNormal3fv( v0->normal_ );
      
      glVertex3fv( v0->pos_ );      
      glVertex3fv( v1->pos_ );      
      glVertex3fv( v2->pos_ );
    }
    glEnd();
    */

    /*
    // Now draw the sample points
    glPointSize( 5.0 );
    glBegin( GL_POINTS );
      glColor3f( 0, 0, 1.0 );
      for( i=0; i<tris_.size(); i++)
        glVertex3f( tris_[i]->sample_.x, tris_[i]->sample_.y, tris_[i]->sample_.z );
    glEnd();
    */

    break;
    
    
  case OBJ_ALL : 
    //printf("Triangle rendering...\n");
    glBegin( GL_TRIANGLES );
    for( i=0; i<tris_.size(); i++){
      GVertex* v0 = verts_[tris_[i]->v_[0]];
      GVertex* v1 = verts_[tris_[i]->v_[1]];
      GVertex* v2 = verts_[tris_[i]->v_[2]];

      glColor3fv( v0->color_ );
      glNormal3fv( v0->normal_ );
      glVertex3fv( v0->pos_ );
      
      glColor3fv( v1->color_ );
      glNormal3fv( v1->normal_ );
      glVertex3fv( v1->pos_ );
      
      glColor3fv( v2->color_ );
      glNormal3fv( v2->normal_ );
      glVertex3fv( v2->pos_ );
    }
    glEnd();
    
 
    
    // Now draw the sample points
  /*
    glPointSize( 5.0 );
    glBegin( GL_POINTS );
    glColor3f( 0.0, 0.0, 1.0 );
      for( i=0; i<slist_.size(); i++){
        //glColor3fv( slist_[i]->color_ );
        glVertex3fv( slist_[i]->pos_ );
      }
    glEnd();
  */

    break;    
  }
  
  
  // restore material state
  glPopAttrib();
}


void GLModel::print(){
  for( int i=0; i<tris_.size(); i++){
    GVertex* v0 = verts_[tris_[i]->v_[0]];
    GVertex* v1 = verts_[tris_[i]->v_[1]];
    GVertex* v2 = verts_[tris_[i]->v_[2]];
    printf("Tri #%d: %d %d %d\n", i, tris_[i]->v_[0], tris_[i]->v_[1], tris_[i]->v_[2] );
    printf("\tpos    "); v0->pos_.Print();
    printf("\tnormal "); v0->normal_.Print();
    printf("\tcolor "); v0->color_.Print();
  }
}



//
// Discretize the given quadrilateral into an
// nxn grid of points, then tesselate into triangles
//
void GLModel::tesselate( int v[4], int n, Vec3f color ){

  cout << "Tesselating " << v[0] << ' '<< v[1] << ' '<< v[2] << ' '<< v[3] << endl;

  // Store the initial vertex index of this plane
  int start_vert = verts_.size();

  // First compute the side length. Vertices must be specified in ccw order
  //Vec3f length = verts_[ v[2] ]->pos_ - verts_[ v[0] ]->pos_;
  Vec3f edge1 = verts_[ v[1] ]->pos_ - verts_[ v[0] ]->pos_;
  Vec3f edge2 = verts_[ v[3] ]->pos_ - verts_[ v[0] ]->pos_;
  Vec3f i_inc = (1.0/n)*edge2;
  Vec3f j_inc = (1.0/n)*edge1;
  edge1.Normalize();
  edge2.Normalize();
  Vec3f normal = edge1 ^ edge2;
  cout << "Normal " << normal << endl;

  float nmax = MAX( normal.x, MAX( normal.y, normal.z ));
  cout << "Nmax = " << nmax << endl;
  if ( nmax > 0.0 ){
    Vec3f tmp = i_inc; i_inc = j_inc; j_inc = tmp;
  }

  // Now actually put all of the points in the vertex array
  Vec3f current = verts_[ v[0] ]->pos_;

  for( int i=0; i<=n; i++){
    for( int j=0; j<=n; j++){
      verts_.append( new GVertex( current, normal ) );
      current += j_inc;
    }
    // Typewriter: pop back to beginning of line and start next line.
    current += -(n+1)*j_inc;
    current += i_inc;
  }

  
  for(i=0; i<n; i++){
    for(int j=0; j<n; j++){
      int row = i*(n+1) + j + start_vert;
      tris_.append( new GTri(row, row+n+2, row+n+1, color ) );
      //cout << "New triangle at " << verts_[row]->pos_ << " " << verts_[row+n+2]->pos_ << " " << verts_[row+n+1]->pos_ << endl;
      tris_.append( new GTri(row, row+1, row+n+2, color) );
      //cout << "New triangle at " << verts_[row]->pos_ << " " << verts_[row+1]->pos_ << " " << verts_[row+n+2]->pos_ << endl << endl;
    }
  }
}


//
// Loads a generic TRI model file.
//
void GLModel::load( char* filename ){
  // First do some sanity checks.
  assert( filename != NULL );

  char tstr1[512];

  // Open the file.
  fstream in_file( filename, ios::in );
  
  int num_verts = 0;
  int num_tris = 0;
  // The first two parameters are the number of vertices 
  // and the number of faces
  in_file >> num_verts >> num_tris;

  printf("Got %d verts and %d tris.\n", num_verts, num_tris);
  //verts_ = new GVertex*[ num_verts_ ];
  //tris_ = new GTri*[ num_tris_ ];

  //bool isLight = false;

  Vec3f current_color(1.0, 1.0, 1.0);
  float a1, a2, a3, a4;
  int v[4];
  int i1, i2, i3, i4;
  Vec3f p;
  char c;

  while( !in_file.eof() ){

    in_file >> c;
    if ( c == 'v' ){
      in_file >> p.x >> p.y >> p.z;
      verts_.append( new GVertex( p ) );
    }

    else if ( c == 'm' ){
      // A moveable vertex
      in_file >> p.x >> p.y >> p.z;
      GVertex* gv = new GVertex( p );
      gv->isMoveable_ = true;
      verts_.append( gv );
    }

    else if ( c == 'f' ){
      in_file >> i1 >> i2 >> i3;      
      // Convert to int with 1-offset
      i1 = i1-1; i2 = i2-1; i3 = i3-1;
      //cout << "Reading triangle " << i << ' ' << i1 << ' ' << i2 << ' ' << i3 << endl;
      //cout << "Vertices at " << verts_[i1]->pos_ << endl;
      tris_.append( new GTri( i1, i2, i3, current_color ) );
    }

    else if ( c == 'l' ){
      in_file >> a1 >> a2 >> a3;
      // If we get a light, then set the light color and grab the next
      // face
      Vec3f light_color( a1, a2, a3 );
      light_table_.append( tris_.size() );

      in_file >> c >> a1 >> a2 >> a3;
      if ( c != 'f' ){
        cout << "Triangle description must follow a light specifier" << endl;
        exit(0);
      }
      // Convert to int with 1-offset
      i1 = a1-1; i2 = a2-1; i3 = a3-1;
      tris_.append( new GTri( i1, i2, i3, light_color ) );
    }

    else if ( c == 't' ){
      // A tesselated plane. The format is
      // <vert1> <vert2> <vert3> <vert4> <n>
      // where the vertices are specified in ccw order, and the tesselated quad is
      // nxn.
      int resolution = 0;
      in_file >> i1 >> i2 >> i3 >> i4 >> resolution;
      cout << "Got vertices " << i1 << ' '<< i2 << ' '<< i3 << ' '<< i4 << endl;
      v[0] = i1-1; v[1] = i2-1; v[2] = i3-1; v[3] = i4-1;
      tesselate( v, resolution, current_color );
    }

    else if ( c == 'c' ){
      // Sets the working color.
      in_file >> a1 >> a2 >> a3;
      current_color = Vec3f( a1, a2, a3 );
    }

    else{
      // Grab the color
      in_file.getline( tstr1, 512, '\n');
    }


  }

  in_file.close();

}




  /*
  if ( normal.x > normal.y && normal.x > normal.z ){
  //if (normal.x == 0.0 ){
    cout << "Looks like the plane is in yz!" << endl;
    i_inc = Vec3f(0, length.y/(double)n, 0);
    j_inc = Vec3f(0, 0, length.z/(double)n);
  }
  else if ( normal.y > normal.z && normal.y > normal.x ){
  //else if ( length.y == 0.0 ){
    cout << "Looks like the plane is in xz!" << endl;
    i_inc = Vec3f(length.x/(double)n, 0, 0);
    j_inc = Vec3f(0, 0, length.z/(double)n);
  }
  else{
    cout << "Looks like the plane is in xy!" << endl;
    i_inc = Vec3f(length.x/(double)n, 0, 0);
    j_inc = Vec3f(0, length.y/(double)n, 0);
  }
  */

