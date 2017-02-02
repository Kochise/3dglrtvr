#ifndef __GLMODEL_H__
#define __GLMODEL_H__
/*
* A simple data structure to hold my triangle models.
* I tried to use Nate Robins code, but my application was
* just too different. This file provides the following:
*
* A Vertex class, which stores:
*    - vertex position 
*    - normal
*    - color
*
* A GLTri class, which stores:
*    - 3 pointers to vertices
*
* A GLModel class, which stores:
*    - A list of GTri's
*    - A list of GVertex's
*
* Notice that this inherits from the Object class. This is so that we can
* display the model inside GLVU.
*
*/

#define MAX( A, B ) (A) > (B) ? (A):(B)

// Hard code the triangle sampling frequency
#define sample_freq 1

#include <vec3f.hpp>
#include <object.hpp>
#include "Useful.h"
#include "MatrixN.h"

#include "GSample.h"

#include <fstream.h>
#include <GL/glut.h>
#include <assert.h>

class GVertex{
public:
  GVertex( Vec3f p = Vec3f::ZERO, Vec3f n = Vec3f::ZERO, Vec3f c = Vec3f::ZERO){ 
    pos_ = p; normal_ = n; color_ = c; isMoveable_ = false;
  }
  GVertex( const GVertex& v ){
    pos_ = v.pos_; normal_ = v.normal_; color_ = v.color_; isMoveable_ = false;
  }

  Vec3f pos_;
  Vec3f normal_;
  Vec3f color_;
  bool isMoveable_;
};



class GTri{
public:
  GTri(int a1 = 0, int a2 = 0, int a3 = 0, Vec3f c = Vec3f::ZERO ){ 
    v_[0] = a1; v_[1] = a2; v_[2] = a3; color_ = c;
  }
  GTri( const GTri& a ){
    v_[0] = a.v_[0]; v_[1] = a.v_[1]; v_[2] = a.v_[2]; color_ = a.color_;
  }


  Vec3<int> id_color_;

  Vec3f color_;
  
  // The indices of the vertices
  int v_[3];
  // The indices of the sample points
  int s_[ sample_freq ];

  // Index into the big GSample list
  Train<int> s_index_;
};


class GLModel: public Object{
public:
  GLModel( char* filename );
  
  virtual void Display(unsigned int Attribs=OBJ_ALL);
  virtual void UpdateMinMax(){
    Min = verts_[0]->pos_;
    Max = verts_[0]->pos_;

    for( int i=0; i<verts_.size(); i++){
      verts_[i]->pos_.UpdateMinMax( Min, Max );
    }
    printf("Model bounds updated.\n");
    printf( "Min - "); Min.Print();
    printf( "Max - "); Max.Print();
  }

  void print();
  void print( int i );

  //void accumulate( int row, int col, float percent );
  void distribute( Vec3f* color );

  void generateRandomColors();

  //  GVertex* getVertex( int i ){ if ( i < num_verts_ ) return verts_[i]; }

  Train<GVertex*> verts_;
  Train<GTri*> tris_;

  MatrixN* matrix_;

  //GSampleList* slist_;
  Train<GSample*> slist_;

  // A list of the lights in the scene
  Train<int> light_table_;

  // An initial light distribution. Initialized in generateLightTable().
  float* initial_;

  // This function depends on generateSamplePoints()
  void generateLightTable( Vec3f light_color = Vec3f(1.0,1.0,1.0), bool random = false );


  // Adjust the light intensity
  void brighten(){ light_energy_ *= 1.1; generateLightTable(); }
  void darken(){ light_energy_ *= .9; generateLightTable(); }
  void move( const Vec3f dist );

private:
  void load( char* filename );
  void generateColors();
  void generateNormals();
  void generateSamplePoints(); // depends on generateNormals()

  void tesselate( int v[4], int n, Vec3f color );


  void initialize(){ 
    // Make a Sample List;
    //slist_ = new GSampleList();
    generateColors();
    generateNormals();
    generateSamplePoints(); 
    generateLightTable();   
    UpdateMinMax();
    // Make a matrix with an element for each vertex
    //matrix_ = new MatrixN( num_verts_ );
    // Make a matrix with an element for each polygon
    matrix_ = new MatrixN( slist_.size() );
  }

  float light_energy_;

};
#endif //__GLMODEL_H__