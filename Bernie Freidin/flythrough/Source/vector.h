// **************************************************************************
// HCSG Demo
// (c) Bernie Freidin, 1999-2000
// **************************************************************************

#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <math.h>

// ****************************
// Two-Dimensional Vector Class
// ****************************

// prototypes

class vec2;
class vec3;
class vec4;

class mat2;
class mat3;
class mat4;

class vec2
{
	public:
	
	double x, y;
	
	inline vec2(void) {}
	
	inline vec2(double _x, double _y)
	{
		x = _x, y = _y;
	}
	inline vec2(double theta)
	{
		x = cos(theta), y = sin(theta);
	}
	inline void zero(void)
	{
		x = y = 0.0;
	}
	inline int operator ==(const vec2& a) const
	{
		return x == a.x && y == a.y;
	}
	inline int operator !=(const vec2& a) const
	{
		return x != a.x || y != a.y;
	}
	inline vec2 operator +(const vec2& a) const
	{
		return vec2(x + a.x, y + a.y);
	}
	inline vec2 operator -(const vec2& a) const
	{
		return vec2(x - a.x, y - a.y);
	}
	inline double operator *(const vec2& a) const
	{
		return x*a.x + y*a.y;
	}
	inline vec2 operator *(const double a) const
	{
		return vec2(x*a, y*a);
	}
	inline vec2 operator /(const double a) const
	{
		return (*this) * (1.0 / a);
	}
	inline double operator ^(const vec2& a) const
	{
		return y*a.x - x*a.y;
	}
	inline const vec2& operator +=(const vec2& a)
	{
		(*this) = (*this) + a; return (*this);
	}
	inline const vec2& operator -=(const vec2& a)
	{
		(*this) = (*this) - a; return (*this);
	}
	inline const vec2& operator *=(const double a)
	{
		(*this) = (*this) * a; return (*this);
	}
	inline const vec2& operator /=(const double a)
	{
		(*this) = (*this) / a; return (*this);
	}
	inline double operator [](int i) const
	{
		return ((double*)this)[i];
	}
	inline double& operator [](int i)
	{
		return ((double*)this)[i];
	}
	inline double len(void) const
	{
		return sqrt(x*x + y*y);
	}
	inline vec2 normalized(void) const
	{
		return (*this) / len();
	}
	inline void normalize(void)
	{
		(*this) /= len();
	}
	inline vec2 hklein2poincare(void) const
	{
		double q = sqrt(1.0 - x*x - y*y);
		return (*this) / (1.0 + q);
	}
	inline vec2 hpoincare2klein(void) const
	{
		double q = 2.0 / (1.0 + x*x + y*y);
		return (*this) * q;
	}
	inline const vec2& operator =(const vec3& a);
	static vec2 hmidpoint(vec2& p1, vec2& p2);
	// note: "static" refers to lack of "this"
};

// ******************************
// Three-Dimensional Vector Class
// ******************************

class vec3
{
	public:
	
	double x, y, z;
	
	inline vec3(void) {}
	
	inline vec3(double _x, double _y, double _z)
	{
		x = _x, y = _y, z = _z;
	}
	inline void zero(void)
	{
		x = y = z = 0.0;
	}
	inline int operator ==(const vec3& a) const
	{
		return x == a.x && y == a.y && z == a.z;
	}
	inline int operator !=(const vec3& a) const
	{
		return x != a.x || y != a.y || z != a.z;
	}
	inline vec3 operator +(const vec3& a) const
	{
		return vec3(x + a.x, y + a.y, z + a.z);
	}
	inline vec3 operator +(const vec2& a) const
	{
		return vec3(x + a.x, y + a.y, z);
	}
	inline vec3 operator -(const vec3& a) const
	{
		return vec3(x - a.x, y - a.y, z - a.z);
	}
	inline vec3 operator -(const vec2& a) const
	{
		return vec3(x - a.x, y - a.y, z);
	}
	inline double operator *(const vec3& a) const
	{
		return x*a.x + y*a.y + z*a.z;
	}
	inline double operator *(const vec2& a) const
	{
		return x*a.x + y*a.y + z;
	}
	inline vec3 operator *(const double a) const
	{
		return vec3(x*a, y*a, z*a);
	}
	inline vec3 operator /(const double a) const
	{
		return (*this) * (1.0 / a);
	}
	inline vec3 operator ^(const vec3& a) const
	{
		return vec3(y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x);
	}
	inline const vec3& operator +=(const vec3& a)
	{
		(*this) = (*this) + a; return (*this);
	}
	inline const vec3& operator +=(const vec2& a)
	{
		(*this) = (*this) + a; return (*this);
	}
	inline const vec3& operator -=(const vec3& a)
	{
		(*this) = (*this) - a; return (*this);
	}
	inline const vec3& operator -=(const vec2& a)
	{
		(*this) = (*this) - a; return (*this);
	}
	inline const vec3& operator *=(const double a)
	{
		(*this) = (*this) * a; return (*this);
	}
	inline const vec3& operator /=(const double a)
	{
		(*this) = (*this) / a; return (*this);
	}
	inline const vec3& operator ^=(const vec3& a)
	{
		(*this) = (*this) ^ a; return (*this);
	}
	inline double operator [](int i) const
	{
		return ((double*)this)[i];
	}
	inline double& operator [](int i)
	{
		return ((double*)this)[i];
	}
	inline double len(void) const
	{
		return sqrt(x*x + y*y + z*z);
	}
	inline vec3 normalized(void) const
	{
		return (*this) / len();
	}
	inline void normalize(void)
	{
		(*this) /= len();
	}
	inline vec3 hklein2poincare(void) const
	{
		double q = sqrt(1.0 - x*x - y*y - z*z);
		return (*this) / (1.0 + q);
	}
	inline vec3 hpoincare2klein(void) const
	{
		double q = 2.0 / (1.0 + x*x + y*y + z*z);
		return (*this) * q;
	}
	static vec3 random(void)
	{
		double x = (double)((rand()*rand()) & 0x7FFF) / 32767.0;
		double y = (double)((rand()^rand()) & 0x7FFF) / 32767.0;
		double z = (double)((rand()+rand()) & 0x7FFF) / 32767.0;
		return vec3(x, y, z);
	}
	static double mincomp(const vec3& a)
	{
		double ax = fabs(a.x);
		double ay = fabs(a.y);
		double az = fabs(a.z);
		if(ax < ay && ax < az) return ax;
		if(ay < az) return ay;
		return az;
	}
	static double maxcomp(const vec3& a)
	{
		double ax = fabs(a.x);
		double ay = fabs(a.y);
		double az = fabs(a.z);
		if(ax > ay && ax > az) return ax;
		if(ay > az) return ay;
		return az;
	}
	inline void operator =(const vec2& a)
	{
		(*this) = vec3(a.x, a.y, 1.0);
	}
//	inline const vec3& vec3::operator =(const vec4& a);
	static vec3 hmidpoint(vec3& p1, vec3& p2);
	// note: "static" refers to lack of "this"
};

// *****************************
// Four-Dimensional Vector Class
// *****************************

class vec4
{
	public:
	
	double x, y, z, w;
	
	inline vec4(void) {}
	
	inline vec4(double _x, double _y, double _z, double _w)
	{
		x = _x, y = _y, z = _z, w = _w;
	}
	inline void zero(void)
	{
		x = y = z = w = 0.0;
	}
	inline int operator ==(const vec4& a) const
	{
		return x == a.x && y == a.y && z == a.z && w == a.w;
	}
	inline int operator !=(const vec4& a) const
	{
		return x != a.x || y != a.y || z != a.z || w != a.w;
	}
	inline vec4 operator +(const vec4& a) const
	{
		return vec4(x + a.x, y + a.y, z + a.z, w + a.w);
	}
	inline vec4 operator +(const vec3& a) const
	{
		return vec4(x + a.x, y + a.y, z + a.z, w);
	}
	inline vec4 operator -(const vec4& a) const
	{
		return vec4(x - a.x, y - a.y, z - a.z, w - a.w);
	}
	inline vec4 operator -(const vec3& a) const
	{
		return vec4(x - a.x, y - a.y, z - a.z, w);
	}
	inline double operator *(const vec4& a) const
	{
		return x*a.x + y*a.y + z*a.z + w*a.w;
	}
	inline double operator *(const vec3& a) const
	{
		return x*a.x + y*a.y + z*a.z + w;
	}
	inline vec4 operator *(const double a) const
	{
		return vec4(x*a, y*a, z*a, w*a);
	}
	inline vec4 operator /(const double a) const
	{
		return (*this) * (1.0 / a);
	}
	inline const vec4& operator +=(const vec4& a)
	{
		(*this) = (*this) + a; return (*this);
	}
	inline const vec4& operator +=(const vec3& a)
	{
		(*this) = (*this) + a; return (*this);
	}
	inline const vec4& operator -=(const vec4& a)
	{
		(*this) = (*this) - a; return (*this);
	}
	inline const vec4& operator -=(const vec3& a)
	{
		(*this) = (*this) - a; return (*this);
	}
	inline const vec4& operator *=(const double a)
	{
		(*this) = (*this) * a; return (*this);
	}
	inline const vec4& operator /=(const double a)
	{
		(*this) = (*this) / a; return (*this);
	}
	inline double operator [](int i) const
	{
		return ((double*)this)[i];
	}
	inline double& operator [](int i)
	{
		return ((double*)this)[i];
	}
	inline double len(void) const
	{
		return sqrt(x*x + y*y + z*z + w*w);
	}
	inline vec4 normalized(void) const
	{
		return (*this) / len();
	}
	inline void normalize(void)
	{
		(*this) /= len();
	}
	inline void operator =(const vec3& a)
	{
		(*this) = vec4(a.x, a.y, a.z, 1.0);
	}
};

//inline const vec2& vec2::operator =(const vec3& a)
//{
//	(*this) = vec2(a.x, a.y) / a.z;
//}

//inline const vec3& vec3::operator =(const vec4& a)
//{
//	(*this) = vec3(a.x, a.y, a.z) / a.w;
//}

// ****************************
// Two-Dimensional Matrix Class
// ****************************

class mat2
{
	public:
	
	vec2 x, y;
	
	inline mat2(void) {}
	
	inline mat2(double m11, double m12,
	            double m21, double m22)
	{
		x = vec2(m11, m12);
		y = vec2(m21, m22);
	}
	inline mat2(const vec2& mx, const vec2& my)
	{
		x = mx, y = my;
	}
	inline void identity(void)
	{
		x = vec2(1.0, 0.0);
		y = vec2(0.0, 1.0);
	}
	inline void zero(void)
	{
		x.zero();
		y.zero();
	}
	inline const int operator ==(const mat2& a) const
	{
		return x == a.x && y == a.y;
	}
	inline const int operator !=(const mat2& a) const
	{
		return x != a.x || y != a.y;
	}
	inline mat2 operator +(const mat2& a) const
	{
		return mat2(x + a.x, y + a.y);
	}
	inline mat2 operator -(const mat2& a) const
	{
		return mat2(x - a.x, y - a.y);
	}
	inline mat2 operator *(const mat2& a) const
	{
		mat2 tmp = a.transposed();
		
		return mat2(tmp*x, tmp*y);
	}
	inline vec2 operator *(const vec2& a) const
	{
		return vec2(x*a, y*a);
	}
	inline mat2 operator *(const double a) const
	{
		return mat2(x*a, y*a);
	}
	inline mat2 operator /(const double a) const
	{
		return (*this) * (1.0 / a);
	}
	inline const mat2& operator +=(const mat2& a)
	{
		(*this) = (*this) + a; return (*this);
	}
	inline const mat2& operator -=(const mat2& a)
	{
		(*this) = (*this) - a; return (*this);
	}
	inline const mat2& operator *=(const mat2& a)
	{
		(*this) = (*this) * a; return (*this);
	}
	inline const mat2& operator *=(const double a)
	{
		(*this) = (*this) * a; return (*this);
	}
	inline const mat2& operator /=(const double a)
	{
		(*this) = (*this) / a; return (*this);
	}
	inline const vec2& operator [](int i) const
	{
		return ((vec2*)this)[i];
	}
	inline vec2& operator [](int i)
	{
		return ((vec2*)this)[i];
	}
	inline double det(void) const
	{
		return 0.0;
	}
	inline mat2 transposed(void) const
	{
		return mat2(x.x, y.x, x.y, y.y);
	}
	inline void transpose(void)
	{
		(*this) = mat2(x.x, y.x, x.y, y.y);
	}
	inline mat2 inverse(void) const
	{
		return mat2(0.0, 0.0, 0.0, 0.0);
	}
	inline mat2 invert(void)
	{
		return mat2(0.0, 0.0, 0.0, 0.0);
	}
	static mat2 rot(double theta)
	{
		// Same as constructor mat2(theta)
		
		double c = cos(theta);
		double s = sin(theta);
		
		return mat2(c, s, -s, c);
	}
};

// ******************************
// Three-Dimensional Matrix Class
// ******************************

class mat3
{
	public:
	
	vec3 x, y, z;
	
	inline mat3(void) {}
	
	inline mat3(double m11, double m12, double m13,
	            double m21, double m22, double m23,
	            double m31, double m32, double m33)
	{
		x = vec3(m11, m12, m13);
		y = vec3(m21, m22, m23);
		z = vec3(m31, m32, m33);
	}
	inline mat3(const vec3& mx, const vec3& my, const vec3& mz)
	{
		x = mx, y = my, z = mz;
	}
	inline void identity(void)
	{
		x = vec3(1.0, 0.0, 0.0);
		y = vec3(0.0, 1.0, 0.0);
		z = vec3(0.0, 0.0, 1.0);
	}
	inline void zero(void)
	{
		x.zero();
		y.zero();
		z.zero();
	}
	inline const int operator ==(const mat3& a) const
	{
		return x == a.x && y == a.y && z == a.z;
	}
	inline const int operator !=(const mat3& a) const
	{
		return x != a.x || y != a.y || z != a.z;
	}
	inline mat3 operator +(const mat3& a) const
	{
		return mat3(x + a.x, y + a.y, z + a.z);
	}
	inline mat3 operator +(const mat2& a) const
	{
		return mat3(x + a.x, y + a.y, z);
	}
	inline mat3 operator -(const mat3& a) const
	{
		return mat3(x - a.x, y - a.y, z - a.z);
	}
	inline mat3 operator -(const mat2& a) const
	{
		return mat3(x - a.x, y - a.y, z);
	}
	inline mat3 operator *(const mat3& a) const
	{
		mat3 tmp = a.transposed();
		
		return mat3(tmp*x, tmp*y, tmp*z);
	}
	inline vec3 operator *(const vec3& a) const
	{
		return vec3(x*a, y*a, z*a);
	}
	inline vec2 operator *(const vec2& a) const
	{
		return vec2(x*a, y*a) / (z*a);
	}
	inline mat3 operator *(const double a) const
	{
		return mat3(x*a, y*a, z*a);
	}
	inline mat3 operator /(const double a) const
	{
		return (*this) * (1.0 / a);
	}
	inline const mat3& operator +=(const mat3& a)
	{
		(*this) = (*this) + a; return (*this);
	}
	inline const mat3& operator +=(const mat2& a)
	{
		(*this) = (*this) + a; return (*this);
	}
	inline const mat3& operator -=(const mat3& a)
	{
		(*this) = (*this) - a; return (*this);
	}
	inline const mat3& operator -=(const mat2& a)
	{
		(*this) = (*this) - a; return (*this);
	}
	inline const mat3& operator *=(const mat3& a)
	{
		(*this) = (*this) * a; return (*this);
	}
	inline const mat3& operator *=(const double a)
	{
		(*this) = (*this) * a; return (*this);
	}
	inline const mat3& operator /=(const double a)
	{
		(*this) = (*this) / a; return (*this);
	}
	inline const vec3& operator [](int i) const
	{
		return ((vec3*)this)[i];
	}
	inline vec3& operator [](int i)
	{
		return ((vec3*)this)[i];
	}
	inline double det(void) const
	{
		return x.y*y.z*z.x - x.z*y.y*z.x + x.z*y.x*z.y +
		       x.x*y.y*z.z - x.x*y.z*z.y - x.y*y.x*z.z ;
	}
	inline mat3 transposed(void) const
	{
		return mat3(x.x, y.x, z.x,
		            x.y, y.y, z.y,
		            x.z, y.z, z.z);
	}
	inline void transpose(void)
	{
		(*this) = mat3(x.x, y.x, z.x,
		               x.y, y.y, z.y,
		               x.z, y.z, z.z);
	}
	inline mat3 inverse(void) const
	{
		double d = (*this).det();
		
		return mat3(y.y*z.z - y.z*z.y, x.z*z.y - x.y*z.z, x.y*y.z - x.z*y.y,
		            y.z*z.x - y.x*z.z, x.x*z.z - x.z*z.x, x.z*y.x - x.x*y.z,
		            y.x*z.y - y.y*z.x, x.y*z.x - x.x*z.y, x.x*y.y - x.y*y.x) / d;
	}
	inline void invert(void)
	{
		double d = (*this).det();
		
		(*this) = mat3(y.y*z.z - y.z*z.y, x.z*z.y - x.y*z.z, x.y*y.z - x.z*y.y,
		               y.z*z.x - y.x*z.z, x.x*z.z - x.z*z.x, x.z*y.x - x.x*y.z,
		               y.x*z.y - y.y*z.x, x.y*z.x - x.x*z.y, x.x*y.y - x.y*y.x) / d;
	}
	inline mat3 inverse2(void) const
	{
		return mat3(y.y*z.z - y.z*z.y, x.z*z.y - x.y*z.z, x.y*y.z - x.z*y.y,
		            y.z*z.x - y.x*z.z, x.x*z.z - x.z*z.x, x.z*y.x - x.x*y.z,
		            y.x*z.y - y.y*z.x, x.y*z.x - x.x*z.y, x.x*y.y - x.y*y.x);
	}
	inline void invert2(void)
	{
		(*this) = mat3(y.y*z.z - y.z*z.y, x.z*z.y - x.y*z.z, x.y*y.z - x.z*y.y,
		               y.z*z.x - y.x*z.z, x.x*z.z - x.z*z.x, x.z*y.x - x.x*y.z,
		               y.x*z.y - y.y*z.x, x.y*z.x - x.x*z.y, x.x*y.y - x.y*y.x);
	}
	static mat3 hrotate(vec2& p, double theta);
	// note: "static" refers to lack of "this"
	static mat3 htranslate(vec2& p);
	// note: "static" refers to lack of "this"
	static mat3 hntranslate(vec2& p);
	// note: "static" refers to lack of "this"
	
	static mat3 rotX(double theta)
	{
		double c = cos(theta);
		double s = sin(theta);
		
		return mat3(1.0, 0.0, 0.0, 0.0, c, s, 0.0, -s, c);
	}
	static mat3 rotY(double theta)
	{
		double c = cos(theta);
		double s = sin(theta);
		
		return mat3(c, 0.0, -s, 0.0, 1.0, 0.0, s, 0.0, c);
	}
	static mat3 rotZ(double theta)
	{
		double c = cos(theta);
		double s = sin(theta);
		
		return mat3(c, s, 0.0, -s, c, 0.0, 0.0, 0.0, 1.0);
	}
	static mat3 ypr(double yaw, double pitch, double roll)
	{
		double cy = cos(yaw);
		double sy = sin(yaw);
		double cp = cos(pitch);
		double sp = sin(pitch);
		double cr = cos(roll);
		double sr = sin(roll);
		
		return mat3(sp*sr*sy + cr*cy, cp*sr, cy*sp*sr - cr*sy,
		            cr*sp*sy - cy*sr, cp*cr, cr*cy*sp + sr*sy,
		            cp*sy, - sp, cp*cy);
	}
	static mat3 horthogonalized(mat3 &m)
	{
		// Decomposes a hyperbolic matrix into a translation and a
		// rotation, and reconstructs the matrix. This process
		// removes accumulative denormalization of the matrix.
		
		vec2 p = vec2(m.x.z, m.y.z) / m.z.z;
		vec2 q = vec2(m.x.x+m.x.z, m.y.x+m.y.z) / (m.z.x+m.z.z);
		mat3 t = hntranslate(p); q = t*q;
		t.x.z = t.z.x = -t.x.z; // invert translation
		t.y.z = t.z.y = -t.y.z;
		return t * rotZ(-atan2(q.y, q.x));
	}
	static mat3 htransline(vec2 &c1, vec2 &v1, vec2 &c2, vec2 &v2)
	{
		// Computes a matrix which takes one line segment (c1,v1)
		// to another (c2,v2). The line segments must be congruent.
		
		mat3 t1 = hntranslate(c1);
		mat3 t2 = hntranslate(c2);
		double a1 = atan2(t1.y*v1, t1.x*v1);
		double a2 = atan2(t2.y*v2, t2.x*v2);
		t2.x.z = t2.z.x = -t2.x.z; // invert translation
		t2.y.z = t2.z.y = -t2.y.z;
		return t2 * rotZ(a1 - a2) * t1;
	}
	static void hdecompose(double &theta, vec2 &transl, mat3 &m)
	{
		// Decomposes a matrix (m) into a rotation followed by a
		// translation.
		
		// Working?!?
		
		vec2 q = vec2(m.x.x+m.x.z, m.y.x+m.y.z) / (m.z.x+m.z.z);
		transl = vec2(m.x.z, m.y.z) / m.z.z;
		q = htranslate(transl)*q;
		transl.x = -transl.x;
		transl.y = -transl.y;
		theta = -atan2(q.y, q.x);
	}
	static mat3 hinterpolate(mat3 &m1, mat3 &m2, double factor)
	{
		// Decomposes two matrices and interpolates between them.
		
		// Working?!?
		
		double a1, a2;
		vec2  t1, t2;
		
		hdecompose(a1, t1, m1);
		hdecompose(a2, t2, m2);
		a1 += (a2 - a1)*factor;
		t1 += (t2 - t1)*factor;
		return htranslate(t1)*rotZ(a1);
	}
};

// *****************************
// Four-Dimensional Matrix Class
// *****************************

class mat4
{
	public:
	
	vec4 x, y, z, w;
	
	inline mat4(void) {}
	
	inline mat4(double m11, double m12, double m13, double m14,
	            double m21, double m22, double m23, double m24,
	            double m31, double m32, double m33, double m34,
	            double m41, double m42, double m43, double m44)
	{
		x = vec4(m11, m12, m13, m14);
		y = vec4(m21, m22, m23, m24);
		z = vec4(m31, m32, m33, m34);
		w = vec4(m41, m42, m43, m44);
	}
	inline mat4(const vec4& mx, const vec4& my, const vec4& mz, const vec4 &mw)
	{
		x = mx, y = my, z = mz, w = mw;
	}
	inline void identity(void)
	{
		x = vec4(1.0, 0.0, 0.0, 0.0);
		y = vec4(0.0, 1.0, 0.0, 0.0);
		z = vec4(0.0, 0.0, 1.0, 0.0);
		w = vec4(0.0, 0.0, 0.0, 1.0);
	}
	inline void zero(void)
	{
		x.zero();
		y.zero();
		z.zero();
		w.zero();
	}
	inline const int operator ==(const mat4& a) const
	{
		return x == a.x && y == a.y && z == a.z && w == a.w;
	}
	inline const int operator !=(const mat4& a) const
	{
		return x != a.x || y != a.y || z != a.z || w != a.w;
	}
	inline mat4 operator +(const mat4& a) const
	{
		return mat4(x + a.x, y + a.y, z + a.z, w + a.w);
	}
	inline mat4 operator +(const mat3& a) const
	{
		return mat4(x + a.x, y + a.y, z + a.z, w);
	}
	inline mat4 operator -(const mat4& a) const
	{
		return mat4(x - a.x, y - a.y, z - a.z, w - a.w);
	}
	inline mat4 operator -(const mat3& a) const
	{
		return mat4(x - a.x, y - a.y, z - a.z, w);
	}
	inline mat4 operator *(const mat4& a) const
	{
		mat4 tmp = a.transposed();
		
		return mat4(tmp*x, tmp*y, tmp*z, tmp*w);
	}
	inline vec4 operator *(const vec4& a) const
	{
		return vec4(x*a, y*a, z*a, w*a);
	}
	inline vec3 operator *(const vec3& a) const
	{
		return vec3(x*a, y*a, z*a) / (w*a);
	}
	inline mat4 operator *(const double a) const
	{
		return mat4(x*a, y*a, z*a, w*a);
	}
	inline mat4 operator /(const double a) const
	{
		return (*this) * (1.0 / a);
	}
	inline const mat4& operator +=(const mat4& a)
	{
		(*this) = (*this) + a; return (*this);
	}
	inline const mat4& operator +=(const mat3& a)
	{
		(*this) = (*this) + a; return (*this);
	}
	inline const mat4& operator -=(const mat4& a)
	{
		(*this) = (*this) - a; return (*this);
	}
	inline const mat4& operator -=(const mat3& a)
	{
		(*this) = (*this) - a; return (*this);
	}
	inline const mat4& operator *=(const mat4& a)
	{
		(*this) = (*this) * a; return (*this);
	}
	inline const mat4& operator *=(const double a)
	{
		(*this) = (*this) * a; return (*this);
	}
	inline const mat4& operator /=(const double a)
	{
		(*this) = (*this) / a; return (*this);
	}
	inline const vec4& operator [](int i) const
	{
		return ((vec4*)this)[i];
	}
	inline vec4& operator [](int i)
	{
		return ((vec4*)this)[i];
	}
	inline mat4 transposed(void) const
	{
		return mat4(x.x, y.x, z.x, w.x,
		            x.y, y.y, z.y, w.y,
		            x.z, y.z, z.z, w.z,
		            x.w, y.w, z.w, w.w);
	}
	inline void transpose(void)
	{
		(*this) = mat4(x.x, y.x, z.x, w.x,
		               x.y, y.y, z.y, w.y,
		               x.z, y.z, z.z, w.z,
		               x.w, y.w, z.w, w.w);
	}
	static mat4 rotX(double c, double s)
	{
		return mat4(1.0, 0.0, 0.0, 0.0,
		            0.0, +c,  +s,  0.0,
		            0.0, -s,  +c,  0.0,
		            0.0, 0.0, 0.0, 1.0);
	}
	static mat4 rotY(double c, double s)
	{
		return mat4(+c,  0.0, -s,  0.0,
		            0.0, 1.0, 0.0, 0.0,
		            +s,  0.0, +c,  0.0,
		            0.0, 0.0, 0.0, 1.0);
	}
	static mat4 rotZ(double c, double s)
	{
		return mat4(+c,  +s,  0.0, 0.0,
		            -s,  +c,  0.0, 0.0,
		            0.0, 0.0, 1.0, 0.0,
		            0.0, 0.0, 0.0, 1.0);
	}
};

// *****************
// Vertex Definition
// *****************

struct vertex_t
{
	vec3   v; // spacial coordinate
	vec3   c; // color value
	vec2   t; // texture coordinate
	double a; // alpha
	//
	int code; // Cohen-Sutherland clip code
};

#endif
 