#ifndef __COLORCYCLER_H__
#define __COLORCYCLER_H__

//
// A color cycler. Basically, sets up three sine waves of different frequency,
// and cycles through each of them for R, G, B colors. This gives the appearance
// of randomly chosen lights that nicely transition.
//
// Author: Greg Coombe
// Date: March 27, 2001
//

#include <math.h>
#include "Useful.h"

#define G_PI 3.141592653589793234

class ColorCycler{
public:
  ColorCycler(){ 
    r_freq = 0.5*canonicalRandom() + 0.5;
    g_freq = 0.5*canonicalRandom() + 0.5;
    b_freq = 0.5*canonicalRandom() + 0.5;
    time = 0.0;
    //cout << "Rfeq = " << r_freq << ", Gfreq = "<< g_freq << ", Bfreq = "<<b_freq << endl;
  }
  
  Vec3f getNextColor( float step ){
    time += step;
    if ( time > 10000) time = 0;
    Vec3f c;
    // Get colors in range [0...1]
    c.x = (sin( r_freq*time ) + 1.0 )/2.0;
    c.y = (sin( g_freq*time ) + 1.0 )/2.0;
    c.z = (sin( b_freq*time ) + 1.0 )/2.0;
    return c;
  }
  
private:
  float r_freq, g_freq, b_freq;
  float time;
};



#endif //__COLORCYCLER_H__