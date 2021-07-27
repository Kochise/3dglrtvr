#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <math.h>

// ****************************
// Two-Dimensional Vector Class
// ****************************

class vec3;
class vec2
{
	public:
	
	float x, y;
	
	inline vec2(void) {}
	
	inline vec2(float _x, float _y)
	{
		x = _x, y = _y;
	}
	inline vec2(float theta)
	{
		x = (float)cos(theta), y = (float)sin(theta);
	}
	inline void zero(void)
	{
		x = y = 0.f;
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
	inline float operator *(const vec2& a) const
	{
		return x*a.x + y*a.y;
	}
	inline vec2 operator *(const float a) const
	{
		return vec2(x*a, y*a);
	}
	inline vec2 operator /(const float a) const
	{
		return (*this) * (1.f / a);
	}
	inline float operator ^(const vec2& a) const
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
	inline const vec2& operator *=(const float a)
	{
		(*this) = (*this) * a; return (*this);
	}
	inline const vec2& operator /=(const float a)
	{
		(*this) = (*this) / a; return (*this);
	}
	inline float operator [](int i) const
	{
		return ((float*)this)[i];
	}
	inline float& operator [](int i)
	{
		return ((float*)this)[i];
	}
	inline float len(void) const
	{
		return (float)sqrt(x*x + y*y);
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
		float q = (float)sqrt(1.f - x*x - y*y);
		return (*this) / (1.f + q);
	}
	inline vec2 hpoincare2klein(void) const
	{
		float q = 2.f / (1.f + x*x + y*y);
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
	
	float x, y, z;
	
	inline vec3(void) {}
	
	inline vec3(float _x, float _y, float _z)
	{
		x = _x, y = _y, z = _z;
	}
	inline void zero(void)
	{
		x = y = z = 0.f;
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
	inline vec3 operator -(const vec3& a) const
	{
		return vec3(x - a.x, y - a.y, z - a.z);
	}
	inline float operator *(const vec3& a) const
	{
		return x*a.x + y*a.y + z*a.z;
	}
	inline float operator *(const vec2& a) const
	{
		return x*a.x + y*a.y + z;
	}
	inline vec3 operator *(const float a) const
	{
		return vec3(x*a, y*a, z*a);
	}
	inline vec3 operator /(const float a) const
	{
		return (*this) * (1.f / a);
	}
	inline vec3 operator ^(const vec3& a) const
	{
		return vec3(y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x);
	}
	inline const vec3& operator +=(const vec3& a)
	{
		(*this) = (*this) + a; return (*this);
	}
	inline const vec3& operator -=(const vec3& a)
	{
		(*this) = (*this) - a; return (*this);
	}
	inline const vec3& operator *=(const float a)
	{
		(*this) = (*this) * a; return (*this);
	}
	inline const vec3& operator /=(const float a)
	{
		(*this) = (*this) / a; return (*this);
	}
	inline const vec3& operator ^=(const vec3& a)
	{
		(*this) = (*this) ^ a; return (*this);
	}
	inline float operator [](int i) const
	{
		return ((float*)this)[i];
	}
	inline float& operator [](int i)
	{
		return ((float*)this)[i];
	}
	inline float len(void) const
	{
		return (float)sqrt(x*x + y*y + z*z);
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
		float q = (float)sqrt(1.f - x*x - y*y - z*z);
		return (*this) / (1.f + q);
	}
	inline vec3 hpoincare2klein(void) const
	{
		float q = 2.f / (1.f + x*x + y*y + z*z);
		return (*this) * q;
	}
	static vec3 random(void)
	{
		float x = (float)((rand()*rand()) & 0x7FFF) / 32767.f;
		float y = (float)((rand()^rand()) & 0x7FFF) / 32767.f;
		float z = (float)((rand()+rand()) & 0x7FFF) / 32767.f;
		return vec3(x, y, z);
	}
	inline void operator =(const vec2& a)
	{
		(*this) = vec3(a.x, a.y, 1.f);
	}
	static vec3 hmidpoint(vec3& p1, vec3& p2);
	// note: "static" refers to lack of "this"
};

inline const vec2& vec2::operator =(const vec3& a)
{
	(*this) = vec2(a.x, a.y) / a.z;
}

// ****************************
// Two-Dimensional Matrix Class
// ****************************

struct mat2
{
	public:
	
	vec2 x, y;
	
	inline mat2(void) {}
	
	inline mat2(float m11, float m12,
	              float m21, float m22)
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
		x = vec2(1.f, 0.f);
		y = vec2(0.f, 1.f);
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
	inline mat2 operator *(const float a) const
	{
		return mat2(x*a, y*a);
	}
	inline mat2 operator /(const float a) const
	{
		return (*this) * (1.f / a);
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
	inline const mat2& operator *=(const float a)
	{
		(*this) = (*this) * a; return (*this);
	}
	inline const mat2& operator /=(const float a)
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
	inline float det(void) const
	{
		;
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
		;
	}
	inline mat2 invert(void)
	{
		;
	}
	static mat2 rot(float theta)
	{
		// Same as constructor mat2(theta)
		
		float c = (float)cos(theta);
		float s = (float)sin(theta);
		
		return mat2(c, s, -s, c);
	}
};

// ******************************
// Three-Dimensional Matrix Class
// ******************************

struct mat3
{
	public:
	
	vec3 x, y, z;
	
	inline mat3(void) {}
	
	inline mat3(float m11, float m12, float m13,
	            float m21, float m22, float m23,
	            float m31, float m32, float m33)
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
		x = vec3(1.f, 0.f, 0.f);
		y = vec3(0.f, 1.f, 0.f);
		z = vec3(0.f, 0.f, 1.f);
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
	inline mat3 operator -(const mat3& a) const
	{
		return mat3(x - a.x, y - a.y, z - a.z);
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
	inline mat3 operator *(const float a) const
	{
		return mat3(x*a, y*a, z*a);
	}
	inline mat3 operator /(const float a) const
	{
		return (*this) * (1.f / a);
	}
	inline const mat3& operator +=(const mat3& a)
	{
		(*this) = (*this) + a; return (*this);
	}
	inline const mat3& operator -=(const mat3& a)
	{
		(*this) = (*this) - a; return (*this);
	}
	inline const mat3& operator *=(const mat3& a)
	{
		(*this) = (*this) * a; return (*this);
	}
	inline const mat3& operator *=(const float a)
	{
		(*this) = (*this) * a; return (*this);
	}
	inline const mat3& operator /=(const float a)
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
	inline float det(void) const
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
		float d = (*this).det();
		
		return mat3(y.y*z.z - y.z*z.y, x.z*z.y - x.y*z.z, x.y*y.z - x.z*y.y,
		            y.z*z.x - y.x*z.z, x.x*z.z - x.z*z.x, x.z*y.x - x.x*y.z,
		            y.x*z.y - y.y*z.x, x.y*z.x - x.x*z.y, x.x*y.y - x.y*y.x) / d;
	}
	inline void invert(void)
	{
		float d = (*this).det();
		
		(*this) = mat3(y.y*z.z - y.z*z.y, x.z*z.y - x.y*z.z, x.y*y.z - x.z*y.y,
		               y.z*z.x - y.x*z.z, x.x*z.z - x.z*z.x, x.z*y.x - x.x*y.z,
		               y.x*z.y - y.y*z.x, x.y*z.x - x.x*z.y, x.x*y.y - x.y*y.x) / d;
	}
	static mat3 hrotate(vec2& p, float theta);
	// note: "static" refers to lack of "this"
	static mat3 htranslate(vec2& p);
	// note: "static" refers to lack of "this"
	static mat3 hntranslate(vec2& p);
	// note: "static" refers to lack of "this"
	
	static mat3 rotX(float theta)
	{
		float c = (float)cos(theta);
		float s = (float)sin(theta);
		
		return mat3(1.f, 0.f, 0.f, 0.f, c, s, 0.f, -s, c);
	}
	static mat3 rotY(float theta)
	{
		float c = (float)cos(theta);
		float s = (float)sin(theta);
		
		return mat3(c, 0.f, -s, 0.f, 1.f, 0.f, s, 0.f, c);
	}
	static mat3 rotZ(float theta)
	{
		float c = (float)cos(theta);
		float s = (float)sin(theta);
		
		return mat3(c, s, 0.f, -s, c, 0.f, 0.f, 0.f, 1.f);
	}
	static mat3 ypr(float yaw, float pitch, float roll)
	{
		float cy = (float)cos(yaw);
		float sy = (float)sin(yaw);
		float cp = (float)cos(pitch);
		float sp = (float)sin(pitch);
		float cr = (float)cos(roll);
		float sr = (float)sin(roll);
		
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
		return t * rotZ(-(float)atan2(q.y, q.x));
	}
	static mat3 htransline(vec2 &c1, vec2 &v1, vec2 &c2, vec2 &v2)
	{
		// Computes a matrix which takes one line segment (c1,v1)
		// to another (c2,v2). The line segments must be congruent.
		
		mat3 t1 = hntranslate(c1);
		mat3 t2 = hntranslate(c2);
		float a1 = (float)atan2(t1.y*v1, t1.x*v1);
		float a2 = (float)atan2(t2.y*v2, t2.x*v2);
		t2.x.z = t2.z.x = -t2.x.z; // invert translation
		t2.y.z = t2.z.y = -t2.y.z;
		return t2 * rotZ(a1 - a2) * t1;
	}
	static void hdecompose(float &theta, vec2 &transl, mat3 &m)
	{
		// Decomposes a matrix (m) into a rotation followed by a
		// translation.
		
		// Working?!?
		
		vec2 q = vec2(m.x.x+m.x.z, m.y.x+m.y.z) / (m.z.x+m.z.z);
		transl = vec2(m.x.z, m.y.z) / m.z.z;
		q = htranslate(transl)*q;
		transl.x = -transl.x;
		transl.y = -transl.y;
		theta = -(float)atan2(q.y, q.x);
	}
	static mat3 hinterpolate(mat3 &m1, mat3 &m2, float factor)
	{
		// Decomposes two matrices and interpolates between them.
		
		// Working?!?
		
		float a1, a2;
		vec2  t1, t2;
		
		hdecompose(a1, t1, m1);
		hdecompose(a2, t2, m2);
		a1 += (a2 - a1)*factor;
		t1 += (t2 - t1)*factor;
		return htranslate(t1)*rotZ(a1);
	}
};

#endif
