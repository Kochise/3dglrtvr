
//
// A small class to hold a sample. The samples are effectively points
// in space with a color and a normal. It is these points which are the states
// of the Markov chain. 
//
#include <vec3f.hpp>
#include "Train.h"

class GSample{
public:
  GSample( Vec3f p = Vec3f::ZERO, Vec3f n = Vec3f::ZERO, int mt = -1, Vec3f c = Vec3f::ZERO){
    pos_ = p; normal_ = n; my_tri_ = mt; color_ = c; initial_ = Vec3f::ZERO; isLight_ = false;
  }
  GSample( const GSample& v ){
    pos_ = v.pos_; normal_ = v.normal_; initial_ = v.initial_; my_tri_ = v.my_tri_;
    isLight_ = v.isLight_;
  }

  Vec3f pos_;
  Vec3f normal_;
  int my_tri_;
  Vec3f initial_; // initial energy of this point
  Vec3f color_;   // color of this point
  Vec3f bary_;    // barycentric coordinates of this point
  bool isLight_;  // Is this a sample point on a light?
};

