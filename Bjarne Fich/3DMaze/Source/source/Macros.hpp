#ifndef __MACROS_HPP_
#define __MACROS_HPP_

// Constants
#define TINY 1e-12
#define PI 3.1415926535897932384626433832795f
#define PIdiv2 1.5707963279489
#define PIdiv180 0.0174532925199432957692369076848861f
#define _180divPI 57.2957795130823208767981548141052f

// Macros
#define FSWAP(a, b) { float f = a; a = b; b = f; }

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define ABS(a)  (((a) > (0)) ? (a) : (-a))

#define RANDOM_FLOAT ( (float)(rand()) / (float)(RAND_MAX+1) )
#define SET_RANGE(x,min,max) (x = ( (x)<(min) ? (min) : ((x)>(max) ? (max) : (x))) )
#define DEG_TO_RAD(angle)  ((angle)*PIdiv180)
#define RAD_TO_DEG(radians) ((radians)*_180divPI)
#define SQUARE(number) (number*number)

#define IS_ZERO(x) ((((x)<TINY) && ((x)>-TINY)) ? true : false)

#endif // __MACROS_HPP_

