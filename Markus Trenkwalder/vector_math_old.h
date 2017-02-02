/*
Copyright (c) 2007, Markus Trenkwalder

All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

* Neither the name of the library's copyright owner nor the names of its 
  contributors may be used to endorse or promote products derived from this 
  software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#include <cmath>

#undef minor

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace vmath {

template <typename T>
T rsqrt(T x)
{
	return T(1) / sqrt(x);
}

template <typename T>
T inv(T x)
{
	return T(1) / x;
}

template <typename T> struct vec2;
template <typename T> struct vec3;
template <typename T> struct vec4;

template <typename T> 
struct vec2 {
	T x, y;
	
	vec2() {};
	explicit vec2(const T i) : x(i), y(i) {}
	vec2(const T ix, const T iy) : x(ix), y(iy) {}
	
	vec2(const vec3<T>& v);
	vec2(const vec4<T>& v);
	
	operator const T* () const { return &x; }
	operator T* () { return &x; }	
	
	vec2& operator += (const vec2& v) { x += v.x; y += v.y; return *this; }
	vec2& operator -= (const vec2& v) { x -= v.x; y -= v.y; return *this; }
	vec2& operator *= (const vec2& v) { x *= v.x; y *= v.y; return *this; }
	vec2& operator /= (const vec2& v) { x /= v.x; y /= v.y; return *this; }
	
	// prevents implicit constructor calls and may therefore be faster
	vec2& operator += (const T s) { x += s; y += s; return *this; }
	vec2& operator -= (const T s) { x -= s; y -= s; return *this; }
	vec2& operator *= (const T s) { x *= s; y *= s; return *this; }
	vec2& operator /= (const T s) { x /= s; y /= s; return *this; }
};

template <typename T> inline vec2<T> operator + (const vec2<T>& u, const vec2<T>& v) { return vec2<T>(u.x + v.x, u.y + v.y); }
template <typename T> inline vec2<T> operator - (const vec2<T>& u, const vec2<T>& v) { return vec2<T>(u.x - v.x, u.y - v.y); }
template <typename T> inline vec2<T> operator - (const vec2<T>& u)                { return vec2<T>(-u.x, -u.y); }
template <typename T> inline vec2<T> operator * (const vec2<T>& u, const vec2<T>& v) { return vec2<T>(u.x * v.x, u.y * v.y); }
template <typename T> inline vec2<T> operator / (const vec2<T>& u, const vec2<T>& v) { return vec2<T>(u.x / v.x, u.y / v.y); }

// prevents implicit constructr calls and may therefore be faster
template <typename T> inline vec2<T> operator + (const vec2<T>& u, const T s) { return vec2<T>(u.x + s, u.y + s); }
template <typename T> inline vec2<T> operator - (const vec2<T>& u, const T s) { return vec2<T>(u.x - s, u.y - s); }
template <typename T> inline vec2<T> operator * (const vec2<T>& u, const T s) { return vec2<T>(u.x * s, u.y * s); }
template <typename T> inline vec2<T> operator / (const vec2<T>& u, const T s) { return vec2<T>(u.x / s, u.y / s); }

template <typename T> inline vec2<T> operator + (const T s, const vec2<T>& u) { return vec2<T>(s + u.x, s + u.y); }
template <typename T> inline vec2<T> operator - (const T s, const vec2<T>& u) { return vec2<T>(s - u.x, s - u.y); }
template <typename T> inline vec2<T> operator * (const T s, const vec2<T>& u) { return vec2<T>(s * u.x, s * u.y); }
template <typename T> inline vec2<T> operator / (const T s, const vec2<T>& u) { return vec2<T>(s / u.x, s / u.y); }

template <typename T> inline bool operator == (const vec2<T>& u, const vec2<T>& v) { return (u.x == v.x && u.y == v.y); }
template <typename T> inline bool operator != (const vec2<T>& u, const vec2<T>& v) { return (u.x != v.x || u.y != v.y); }

template <typename T> 
struct vec3 {
	T x, y, z;
	
	vec3() {};
	explicit vec3(const T i) : x(i), y(i), z(i) {}
	vec3(const vec2<T>& xy, const T iz) : x(xy.x), y(xy.y), z(iz) {}
	vec3(const T ix, const T iy, const T iz) : x(ix), y(iy), z(iz) {}
	
	vec3(const vec4<T>& v);
	
	operator const T* () const { return &x; }
	operator T* () { return &x; }	
	
	vec3& operator += (const vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	vec3& operator -= (const vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	vec3& operator *= (const vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
	vec3& operator /= (const vec3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
	
	// prevents implicit constructor calls and may therefore be faster
	vec3& operator += (const T s) { x += s; y += s; z += s; return *this; }
	vec3& operator -= (const T s) { x -= s; y -= s; z -= s; return *this; }
	vec3& operator *= (const T s) { x *= s; y *= s; z *= s; return *this; }
	vec3& operator /= (const T s) { x /= s; y /= s; z /= s; return *this; }
};

template <typename T> inline vec3<T> operator + (const vec3<T>& u, const vec3<T>& v) { return vec3<T>(u.x + v.x, u.y + v.y, u.z + v.z); }
template <typename T> inline vec3<T> operator - (const vec3<T>& u, const vec3<T>& v) { return vec3<T>(u.x - v.x, u.y - v.y, u.z - v.z); }
template <typename T> inline vec3<T> operator - (const vec3<T>& u)                { return vec3<T>(-u.x, -u.y, -u.z); }
template <typename T> inline vec3<T> operator * (const vec3<T>& u, const vec3<T>& v) { return vec3<T>(u.x * v.x, u.y * v.y, u.z * v.z); }
template <typename T> inline vec3<T> operator / (const vec3<T>& u, const vec3<T>& v) { return vec3<T>(u.x / v.x, u.y / v.y, u.z / v.z); }

// prevents implicit constructr calls and may therefore be faster
template <typename T> inline vec3<T> operator + (const vec3<T>& u, const T s) { return vec3<T>(u.x + s, u.y + s, u.z + s); }
template <typename T> inline vec3<T> operator - (const vec3<T>& u, const T s) { return vec3<T>(u.x - s, u.y - s, u.z - s); }
template <typename T> inline vec3<T> operator * (const vec3<T>& u, const T s) { return vec3<T>(u.x * s, u.y * s, u.z * s); }
template <typename T> inline vec3<T> operator / (const vec3<T>& u, const T s) { return vec3<T>(u.x / s, u.y / s, u.z / s); }

template <typename T> inline vec3<T> operator + (const T s, const vec3<T>& u) { return vec3<T>(s + u.x, s + u.y, s + u.z); }
template <typename T> inline vec3<T> operator - (const T s, const vec3<T>& u) { return vec3<T>(s - u.x, s - u.y, s - u.z); }
template <typename T> inline vec3<T> operator * (const T s, const vec3<T>& u) { return vec3<T>(s * u.x, s * u.y, s * u.z); }
template <typename T> inline vec3<T> operator / (const T s, const vec3<T>& u) { return vec3<T>(s / u.x, s / u.y, s / u.z); }

template <typename T> inline bool operator == (const vec3<T>& u, const vec3<T>& v) { return (u.x == v.x && u.y == v.y && u.z == v.z); }
template <typename T> inline bool operator != (const vec3<T>& u, const vec3<T>& v) { return (u.x != v.x || u.y != v.y || u.z != v.z); }

template <typename T> 
struct vec4 {
	T x, y, z, w;
	
	vec4() {};
	explicit vec4(const T i) : x(i), y(i), z(i), w(i) {}
	vec4(const vec2<T>& xy, const T iz, const T iw) : x(xy.x), y(xy.y), z(iz), w(iw) {}
	vec4(const vec2<T>& xy, const vec2<T>& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
	vec4(const vec3<T>& xyz,const T iw) : x(xyz.x), y(xyz.y), z(xyz.z), w(iw) {}
	vec4(const T ix, const T iy, const T iz, const T iw) : x(ix), y(iy), z(iz), w(iw) {}
	
	operator const T* () const { return &x; }
	operator T* () { return &x; }	
	
	vec4& operator += (const vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	vec4& operator -= (const vec4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	vec4& operator *= (const vec4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	vec4& operator /= (const vec4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
	
	// prevents implicit constructor calls and may therefore be faster
	vec4& operator += (const T s) { x += s; y += s; z += s; w += s; return *this; }
	vec4& operator -= (const T s) { x -= s; y -= s; z -= s; w -= s; return *this; }
	vec4& operator *= (const T s) { x *= s; y *= s; z *= s; w *= s; return *this; }
	vec4& operator /= (const T s) { x /= s; y /= s; z /= s; w /= s; return *this; }
};

template <typename T> inline vec4<T> operator + (const vec4<T>& u, const vec4<T>& v) { return vec4<T>(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w); }
template <typename T> inline vec4<T> operator - (const vec4<T>& u, const vec4<T>& v) { return vec4<T>(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w); }
template <typename T> inline vec4<T> operator - (const vec4<T>& u)                { return vec4<T>(-u.x, -u.y, -u.z, -u.w); }
template <typename T> inline vec4<T> operator * (const vec4<T>& u, const vec4<T>& v) { return vec4<T>(u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w); }
template <typename T> inline vec4<T> operator / (const vec4<T>& u, const vec4<T>& v) { return vec4<T>(u.x / v.x, u.y / v.y, u.z / v.z, u.w / v.w); }

// prevents implicit constructr calls and may therefore be faster
template <typename T> inline vec4<T> operator + (const vec4<T>& u, const T s) { return vec4<T>(u.x + s, u.y + s, u.z + s, u.w + s); }
template <typename T> inline vec4<T> operator - (const vec4<T>& u, const T s) { return vec4<T>(u.x - s, u.y - s, u.z - s, u.w - s); }
template <typename T> inline vec4<T> operator * (const vec4<T>& u, const T s) { return vec4<T>(u.x * s, u.y * s, u.z * s, u.w * s); }
template <typename T> inline vec4<T> operator / (const vec4<T>& u, const T s) { return vec4<T>(u.x / s, u.y / s, u.z / s, u.w / s); }

template <typename T> inline vec4<T> operator + (const T s, const vec4<T>& u) { return vec4<T>(s + u.x, s + u.y, s + u.z, s + u.w); }
template <typename T> inline vec4<T> operator - (const T s, const vec4<T>& u) { return vec4<T>(s - u.x, s - u.y, s - u.z, s - u.w); }
template <typename T> inline vec4<T> operator * (const T s, const vec4<T>& u) { return vec4<T>(s * u.x, s * u.y, s * u.z, s * u.w); }
template <typename T> inline vec4<T> operator / (const T s, const vec4<T>& u) { return vec4<T>(s / u.x, s / u.y, s / u.z, s / u.w); }

template <typename T> inline bool operator == (const vec4<T>& u, const vec4<T>& v) { return (u.x == v.x && u.y == v.y && u.z == v.z && u.w == v.w); }
template <typename T> inline bool operator != (const vec4<T>& u, const vec4<T>& v) { return (u.x != v.x || u.y != v.y || u.z != v.z || u.w != v.w); }

// additional constructors that omit the last element
template <typename T> inline vec2<T>::vec2(const vec3<T>& v) : x(v.x), y(v.y) {}
template <typename T> inline vec2<T>::vec2(const vec4<T>& v) : x(v.x), y(v.y) {}
template <typename T> inline vec3<T>::vec3(const vec4<T>& v) : x(v.x), y(v.y), z(v.z) {}

// vector functions
template <typename T> inline T dot(const vec2<T>& u, const vec2<T>& v) { return u.x * v.x + u.y * v.y; }
template <typename T> inline T dot(const vec3<T>& u, const vec3<T>& v) { return u.x * v.x + u.y * v.y + u.z * v.z; }
template <typename T> inline T dot(const vec4<T>& u, const vec4<T>& v) { return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w; }

template <typename T> inline T lerp(const T u, const T v, const T x) { return u * (T(1) - x) + v * x; }
template <typename T> inline vec2<T>  lerp(const vec2<T>& u, const vec2<T>& v, const T x) { return u * (T(1) - x) + v * x; }
template <typename T> inline vec3<T>  lerp(const vec3<T>& u, const vec3<T>& v, const T x) { return u * (T(1) - x) + v * x; }
template <typename T> inline vec4<T>  lerp(const vec4<T>& u, const vec4<T>& v, const T x) { return u * (T(1) - x) + v * x; }

template <typename T> inline T length2(const vec2<T>& v) { return v.x * v.x + v.y * v.y; }
template <typename T> inline T length2(const vec3<T>& v) { return v.x * v.x + v.y * v.y + v.z * v.z; }
template <typename T> inline T length2(const vec4<T>& v) { return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w; }

template <typename T> inline T length(const vec2<T>& v) { return sqrt(v.x * v.x + v.y * v.y); }
template <typename T> inline T length(const vec3<T>& v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }
template <typename T> inline T length(const vec4<T>& v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w); }

template <typename T> inline vec3<T> cross(const vec3<T>& u, const vec3<T>& v)
{
	return vec3<T>(
		u.y * v.z - v.y * u.z,
		u.z * v.x - v.z * u.x,
		u.x * v.y - v.x * u.y
	);
}

template <typename T> inline vec2<T> normalize(const vec2<T>& v)
{
	return v * rsqrt(v.x * v.x + v.y * v.y);	
}

template <typename T> inline vec3<T> normalize(const vec3<T>& v)
{
	return v * rsqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}


template <typename T> inline vec4<T> normalize(const vec4<T>& v)
{
	return v * rsqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

template <typename T> inline vec2<T> reflect(const vec2<T>& I, const vec2<T>& N) { return I - T(2) * dot(N, I) * N; }
template <typename T> inline vec3<T> reflect(const vec3<T>& I, const vec3<T>& N) { return I - T(2) * dot(N, I) * N; }
template <typename T> inline vec4<T> reflect(const vec4<T>& I, const vec4<T>& N) { return I - T(2) * dot(N, I) * N; }

template <typename T> inline vec2<T> refract(const vec2<T>& I, const vec2<T>& N, T eta)
{
	const T d = dot(N, I);
	const T k = T(1) - eta * eta * (T(1) - d * d);
	
	if ( k < T(0) )
		return vec2<T>(T(0));
	else
		return eta * I - (eta * d + sqrt(k)) * N;
}

template <typename T> inline vec3<T> refract(const vec3<T>& I, const vec3<T>& N, T eta)
{
	const T d = dot(N, I);
	const T k = T(1) - eta * eta * (T(1) - d * d);
	
	if ( k < T(0) )
		return vec3<T>(T(0));
	else
		return eta * I - (eta * d + sqrt(k)) * N;
}

template <typename T> inline vec4<T> refract(const vec4<T>& I, const vec4<T>& N, T eta)
{
	const T d = dot(N, I);
	const T k = T(1) - eta * eta * (T(1) - d * d);
	
	if ( k < T(0) )
		return vec4<T>(T(0));
	else
		return eta * I - (eta * d + sqrt(k)) * N;
}



template <typename T> struct mat2;
template <typename T> struct mat3;
template <typename T> struct mat4;

template <typename T> 
struct mat2 {
	T elem[2][2];
	
	mat2() {}
	explicit mat2(const T m)
	{
		elem[0][0] = elem[1][1] = m;
		elem[0][1] = elem[1][0] = T(0);
	}
	mat2(const T m00, const T m01,
       const T m10, const T m11)
	{
		elem[0][0] = m00; elem[0][1] = m01;
		elem[1][0] = m10; elem[1][1] = m11;
	}
	mat2(const vec2<T>& v0, const vec2<T>& v1)
	{
		elem[0][0] = v0.x; elem[0][1] = v0.y;
		elem[1][0] = v1.x; elem[1][1] = v1.y;
	}
	mat2(const mat3<T>& m);
	
	operator const T* () const { return (const T*) elem; }
	operator T* () { return (T *) elem; }
	
	mat2& operator += (const mat2& m)
	{
		elem[0][0] += m.elem[0][0]; elem[0][1] += m.elem[0][1];		
		elem[1][0] += m.elem[1][0]; elem[1][1] += m.elem[1][1];		
		return *this;
	}
	mat2& operator -= (const mat2& m)
	{
		elem[0][0] -= m.elem[0][0]; elem[0][1] -= m.elem[0][1];		
		elem[1][0] -= m.elem[1][0]; elem[1][1] -= m.elem[1][1];		
		return *this;
	}
	mat2& operator /= (const mat2& m)
	{
		elem[0][0] /= m.elem[0][0]; elem[0][1] /= m.elem[0][1];		
		elem[1][0] /= m.elem[1][0]; elem[1][1] /= m.elem[1][1];		
		return *this;
	}
	
	mat2& operator += (const T s) { elem[0][0] += s; elem[0][1] += s; elem[1][0] += s; elem[1][1] += s; return *this; }
	mat2& operator -= (const T s) { elem[0][0] -= s; elem[0][1] -= s; elem[1][0] -= s; elem[1][1] -= s; return *this; }
	mat2& operator *= (const T s) {	elem[0][0] *= s; elem[0][1] *= s; elem[1][0] *= s; elem[1][1] *= s; return *this; }
	mat2& operator /= (const T s) { elem[0][0] /= s; elem[0][1] /= s; elem[1][0] /= s; elem[1][1] /= s; return *this; }
};

// matrix op matrix
template <typename T> inline mat2<T> operator + (const mat2<T>& a, const mat2<T>& b)
{
	return mat2<T>(
		a.elem[0][0] + b.elem[0][0], a.elem[0][1] + b.elem[0][1],
		a.elem[1][0] + b.elem[1][0], a.elem[1][1] + b.elem[1][1]);
}
template <typename T> inline mat2<T> operator - (const mat2<T>& a)
{
	return mat2<T>(
		-a.elem[0][0], -a.elem[0][1],
		-a.elem[1][0], -a.elem[1][1]);
}
template <typename T> inline mat2<T> operator - (const mat2<T>& a, const mat2<T>& b)
{
	return mat2<T>(
		a.elem[0][0] - b.elem[0][0], a.elem[0][1] - b.elem[0][1],
		a.elem[1][0] - b.elem[1][0], a.elem[1][1] - b.elem[1][1]);
}
template <typename T> inline mat2<T> operator / (const mat2<T>& a, const mat2<T>& b)
{
	return mat2<T>(
		a.elem[0][0] / b.elem[0][0], a.elem[0][1] / b.elem[0][1],
		a.elem[1][0] / b.elem[1][0], a.elem[1][1] / b.elem[1][1]);
}

// matrix op scalar
template <typename T> inline mat2<T> operator + (const mat2<T>& a, const T s)
{
	return mat2<T>(
		a.elem[0][0] + s, a.elem[0][1] + s,
		a.elem[1][0] + s, a.elem[1][1] + s);
}
template <typename T> inline mat2<T> operator - (const mat2<T>& a, const T s)
{
	return mat2<T>(
		a.elem[0][0] - s, a.elem[0][1] - s,
		a.elem[1][0] - s, a.elem[1][1] - s);
}
template <typename T> inline mat2<T> operator * (const mat2<T>& a, const T s)
{
	return mat2<T>(
		a.elem[0][0] * s, a.elem[0][1] * s,
		a.elem[1][0] * s, a.elem[1][1] * s);
}
template <typename T> inline mat2<T> operator / (const mat2<T>& a, const T s)
{
	return mat2<T>(
		a.elem[0][0] / s, a.elem[0][1] / s,
		a.elem[1][0] / s, a.elem[1][1] / s);
}

// scalar op matrix
template <typename T> inline mat2<T> operator + (const T s, const mat2<T>& a)
{
	return mat2<T>(
		s + a.elem[0][0], s + a.elem[0][1],
		s + a.elem[1][0], s + a.elem[1][1]);
}
template <typename T> inline mat2<T> operator - (const T s, const mat2<T>& a)
{
	return mat2<T>(
		s - a.elem[0][0], s - a.elem[0][1],
		s - a.elem[1][0], s - a.elem[1][1]);
}
template <typename T> inline mat2<T> operator * (const T s, const mat2<T>& a)
{
	return mat2<T>(
		s * a.elem[0][0], s * a.elem[0][1],
		s * a.elem[1][0], s * a.elem[1][1]);
}
template <typename T> inline mat2<T> operator / (const T s, const mat2<T>& a)
{
	return mat2<T>(
		s / a.elem[0][0], s / a.elem[0][1],
		s / a.elem[1][0], s / a.elem[1][1]);
}

// matrix multiplication
template <typename T> inline mat2<T> operator * (const mat2<T>& a, const mat2<T>& b)
{
	return mat2<T>(
		a.elem[0][0] * b.elem[0][0] + a.elem[0][1] * b.elem[1][0], a.elem[0][0] * b.elem[0][1] + a.elem[0][1] * b.elem[1][1],
		a.elem[1][0] * b.elem[0][0] + a.elem[1][1] * b.elem[1][0], a.elem[1][0] * b.elem[0][1] + a.elem[1][1] * b.elem[1][1]);
}

template <typename T> 
struct mat3 {
	T elem[3][3];
	
	mat3() {}
	explicit mat3(const T m)
	{
		elem[0][0] = elem[1][1] = elem[2][2] = m;
		elem[0][1] = elem[0][2] = elem[1][2] = T(0);
		elem[1][0] = elem[2][0] = elem[2][1] = T(0);
	}
	mat3(const T m00, const T m01, const T m02,
       const T m10, const T m11, const T m12,
			 const T m20, const T m21, const T m22)
	{
		elem[0][0] = m00; elem[0][1] = m01; elem[0][2] = m02;
		elem[1][0] = m10; elem[1][1] = m11; elem[1][2] = m12;
		elem[2][0] = m20; elem[2][1] = m21; elem[2][2] = m22;
	}
	mat3(const vec3<T>& v0, const vec3<T>& v1, const vec3<T>& v2)
	{
		elem[0][0] = v0.x; elem[0][1] = v0.y; elem[0][2] = v0.z;
		elem[1][0] = v1.x; elem[1][1] = v1.y; elem[1][2] = v1.z;
		elem[2][0] = v2.x; elem[2][1] = v2.y; elem[2][2] = v2.z;
	}
	mat3(const mat2<T>& m)
	{
		elem[0][0] = m.elem[0][0]; elem[0][1] = m.elem[0][1]; elem[0][2] = T(0);
		elem[1][0] = m.elem[1][0]; elem[1][1] = m.elem[1][1]; elem[1][2] = T(0);
		elem[2][0] = elem[2][1] = T(0); elem[2][2] = T(1);
	}
	mat3(const mat4<T>& m);

	operator const T* () const { return (const T*) elem; }
	operator T* () { return (T *) elem; }
	
	mat3& operator += (const mat3& m)
	{
		elem[0][0] += m.elem[0][0]; elem[0][1] += m.elem[0][1]; elem[0][2] += m.elem[0][2];		
		elem[1][0] += m.elem[1][0]; elem[1][1] += m.elem[1][1]; elem[1][2] += m.elem[1][2];				
		elem[2][0] += m.elem[2][0]; elem[2][1] += m.elem[2][1]; elem[2][2] += m.elem[2][2];				
		return *this;
	}
	mat3& operator -= (const mat3& m)
	{
		elem[0][0] -= m.elem[0][0]; elem[0][1] -= m.elem[0][1];	elem[0][2] -= m.elem[0][2];
		elem[1][0] -= m.elem[1][0]; elem[1][1] -= m.elem[1][1];	elem[1][2] -= m.elem[1][2];		
		elem[2][0] -= m.elem[2][0]; elem[2][1] -= m.elem[2][1];	elem[2][2] -= m.elem[2][2];		
		return *this;
	}
	mat3& operator /= (const mat3& m)
	{
		elem[0][0] /= m.elem[0][0]; elem[0][1] /= m.elem[0][1]; elem[0][2] /= m.elem[0][2];
		elem[1][0] /= m.elem[1][0]; elem[1][1] /= m.elem[1][1]; elem[1][2] /= m.elem[1][2];
		elem[2][0] /= m.elem[2][0]; elem[2][1] /= m.elem[2][1]; elem[2][2] /= m.elem[2][2];
		return *this;
	}
	
	mat3& operator += (const T s)
	{
		elem[0][0] += s; elem[0][1] += s; elem[0][2] += s;
		elem[1][0] += s; elem[1][1] += s; elem[1][2] += s;
		elem[2][0] += s; elem[2][1] += s; elem[2][2] += s;
		return *this;
	}
	mat3& operator -= (const T s)
	{
		elem[0][0] -= s; elem[0][1] -= s; elem[0][2] -= s;
		elem[1][0] -= s; elem[1][1] -= s; elem[1][2] -= s;
		elem[2][0] -= s; elem[2][1] -= s; elem[2][2] -= s;
		return *this;
	}
	mat3& operator *= (const T s)
	{
		elem[0][0] *= s; elem[0][1] *= s; elem[0][2] *= s;
		elem[1][0] *= s; elem[1][1] *= s; elem[1][2] *= s;
		elem[2][0] *= s; elem[2][1] *= s; elem[2][2] *= s;
		return *this;
	}
	mat3& operator /= (const T s)
	{
		elem[0][0] /= s; elem[0][1] /= s; elem[0][2] /= s;
		elem[1][0] /= s; elem[1][1] /= s; elem[1][2] /= s;
		elem[2][0] /= s; elem[2][1] /= s; elem[2][2] /= s;
		return *this;
	}
};

// matrix op matrix
template <typename T> inline mat3<T> operator + (const mat3<T>& a, const mat3<T>& b)
{
	return mat3<T>(
		a.elem[0][0] + b.elem[0][0], a.elem[0][1] + b.elem[0][1], a.elem[0][2] + b.elem[0][2],
		a.elem[1][0] + b.elem[1][0], a.elem[1][1] + b.elem[1][1], a.elem[1][2] + b.elem[1][2],
		a.elem[2][0] + b.elem[2][0], a.elem[2][1] + b.elem[2][1], a.elem[2][2] + b.elem[2][2]);
}
template <typename T> inline mat3<T> operator - (const mat3<T>& a)
{
	return mat3<T>(
		-a.elem[0][0], -a.elem[0][1], -a.elem[0][2],
		-a.elem[1][0], -a.elem[1][1], -a.elem[1][2],
		-a.elem[2][0], -a.elem[2][1], -a.elem[2][2]);
}
template <typename T> inline mat3<T> operator - (const mat3<T>& a, const mat3<T>& b)
{
	return mat3<T>(
		a.elem[0][0] - b.elem[0][0], a.elem[0][1] - b.elem[0][1], a.elem[0][2] - b.elem[0][2],
		a.elem[1][0] - b.elem[1][0], a.elem[1][1] - b.elem[1][1], a.elem[1][2] - b.elem[1][2],
		a.elem[2][0] - b.elem[2][0], a.elem[2][1] - b.elem[2][1], a.elem[2][2] - b.elem[2][2]);
}
template <typename T> inline mat3<T> operator / (const mat3<T>& a, const mat3<T>& b)
{
	return mat3<T>(
		a.elem[0][0] / b.elem[0][0], a.elem[0][1] / b.elem[0][1], a.elem[0][2] / b.elem[0][2],
		a.elem[1][0] / b.elem[1][0], a.elem[1][1] / b.elem[1][1], a.elem[1][2] / b.elem[1][2],
		a.elem[2][0] / b.elem[2][0], a.elem[2][1] / b.elem[2][1], a.elem[2][2] / b.elem[2][2]);
}

// matrix op scalar
template <typename T> inline mat3<T> operator + (const mat3<T>& a, const T s)
{
	return mat3<T>(
		a.elem[0][0] + s, a.elem[0][1] + s, a.elem[0][2] + s,
		a.elem[1][0] + s, a.elem[1][1] + s, a.elem[1][2] + s,
		a.elem[2][0] + s, a.elem[2][1] + s, a.elem[2][2] + s);
}
template <typename T> inline mat3<T> operator - (const mat3<T>& a, const T s)
{
	return mat3<T>(
		a.elem[0][0] - s, a.elem[0][1] - s, a.elem[0][2] - s,
		a.elem[1][0] - s, a.elem[1][1] - s, a.elem[1][2] - s,
		a.elem[2][0] - s, a.elem[2][1] - s, a.elem[2][2] - s);
}
template <typename T> inline mat3<T> operator * (const mat3<T>& a, const T s)
{
	return mat3<T>(
		a.elem[0][0] * s, a.elem[0][1] * s, a.elem[0][2] * s,
		a.elem[1][0] * s, a.elem[1][1] * s, a.elem[1][2] * s,
		a.elem[2][0] * s, a.elem[2][1] * s, a.elem[2][2] * s);
}
template <typename T> inline mat3<T> operator / (const mat3<T>& a, const T s)
{
	return mat3<T>(
		a.elem[0][0] / s, a.elem[0][1] / s, a.elem[0][2] / s,
		a.elem[1][0] / s, a.elem[1][1] / s, a.elem[1][2] / s,
		a.elem[2][0] / s, a.elem[2][1] / s, a.elem[2][2] / s);
}

// scalar op matrix
template <typename T> inline mat3<T> operator + (const T s, const mat3<T>& a)
{
	return mat3<T>(
		s + a.elem[0][0], s + a.elem[0][1], s + a.elem[0][2],
		s + a.elem[1][0], s + a.elem[1][1], s + a.elem[1][2],
		s + a.elem[2][0], s + a.elem[2][1], s + a.elem[2][2]);
}
template <typename T> inline mat3<T> operator - (const T s, const mat3<T>& a)
{
	return mat3<T>(
		s - a.elem[0][0], s - a.elem[0][1], s - a.elem[0][2],
		s - a.elem[1][0], s - a.elem[1][1], s - a.elem[1][2],
		s - a.elem[2][0], s - a.elem[2][1], s - a.elem[2][2]);
}
template <typename T> inline mat3<T> operator * (const T s, const mat3<T>& a)
{
	return mat3<T>(
		s * a.elem[0][0], s * a.elem[0][1], s * a.elem[0][2],
		s * a.elem[1][0], s * a.elem[1][1], s * a.elem[1][2],
		s * a.elem[2][0], s * a.elem[2][1], s * a.elem[2][2]);
}
template <typename T> inline mat3<T> operator / (const T s, const mat3<T>& a)
{
	return mat3<T>(
		s / a.elem[0][0], s / a.elem[0][1], s / a.elem[0][2],
		s / a.elem[1][0], s / a.elem[1][1], s / a.elem[1][2],
		s / a.elem[2][0], s / a.elem[2][1], s / a.elem[2][2]);
}

// matrix multiplication
template <typename T> inline mat3<T> operator * (const mat3<T>& a, const mat3<T>& b)
{
	return mat3<T>(
		a.elem[0][0] * b.elem[0][0] + a.elem[0][1] * b.elem[1][0] + a.elem[0][2] * b.elem[2][0],
		a.elem[0][0] * b.elem[0][1] + a.elem[0][1] * b.elem[1][1] + a.elem[0][2] * b.elem[2][1],
		a.elem[0][0] * b.elem[0][2] + a.elem[0][1] * b.elem[1][2] + a.elem[0][2] * b.elem[2][2],

		a.elem[1][0] * b.elem[0][0] + a.elem[1][1] * b.elem[1][0] + a.elem[1][2] * b.elem[2][0],
		a.elem[1][0] * b.elem[0][1] + a.elem[1][1] * b.elem[1][1] + a.elem[1][2] * b.elem[2][1],
		a.elem[1][0] * b.elem[0][2] + a.elem[1][1] * b.elem[1][2] + a.elem[1][2] * b.elem[2][2],

		a.elem[2][0] * b.elem[0][0] + a.elem[2][1] * b.elem[1][0] + a.elem[2][2] * b.elem[2][0],
		a.elem[2][0] * b.elem[0][1] + a.elem[2][1] * b.elem[1][1] + a.elem[2][2] * b.elem[2][1],
		a.elem[2][0] * b.elem[0][2] + a.elem[2][1] * b.elem[1][2] + a.elem[2][2] * b.elem[2][2]);
}

template <typename T> 
struct mat4 {
	T elem[4][4];
	
	mat4() {}
	explicit mat4(const T m)
	{
		elem[0][0] = elem[1][1] = elem[2][2] = elem[3][3] = m;
		
		elem[0][1] = elem[0][2] = elem[0][3] = T(0);
		elem[1][2] = elem[1][3] = elem[2][3] = T(0);

		elem[1][0] = elem[2][0] = elem[3][0] = T(0);
		elem[2][1] = elem[3][1] = elem[3][2] = T(0);
	}
	mat4(const T m00, const T m01, const T m02, const T m03,
       const T m10, const T m11, const T m12, const T m13,
			 const T m20, const T m21, const T m22, const T m23,
			 const T m30, const T m31, const T m32, const T m33)
	{
		elem[0][0] = m00; elem[0][1] = m01; elem[0][2] = m02; elem[0][3] = m03;
		elem[1][0] = m10; elem[1][1] = m11; elem[1][2] = m12; elem[1][3] = m13;
		elem[2][0] = m20; elem[2][1] = m21; elem[2][2] = m22; elem[2][3] = m23;
		elem[3][0] = m30; elem[3][1] = m31; elem[3][2] = m32; elem[3][3] = m33;
	}
	mat4(const vec4<T>& v0, const vec4<T>& v1, const vec4<T>& v2, const vec4<T>& v3)
	{
		elem[0][0] = v0.x; elem[0][1] = v0.y; elem[0][2] = v0.z; elem[0][3] = v0.w;
		elem[1][0] = v1.x; elem[1][1] = v1.y; elem[1][2] = v1.z; elem[1][3] = v1.w;
		elem[2][0] = v2.x; elem[2][1] = v2.y; elem[2][2] = v2.z; elem[2][3] = v2.w;
		elem[3][0] = v3.x; elem[3][1] = v3.y; elem[3][2] = v3.z; elem[3][3] = v3.w;
	}
	mat4(const mat3<T>& m)
	{
		elem[0][0] = m.elem[0][0]; elem[0][1] = m.elem[0][1]; elem[0][2] = m.elem[0][2]; elem[0][3] = T(0);
		elem[1][0] = m.elem[1][0]; elem[1][1] = m.elem[1][1]; elem[1][2] = m.elem[1][2]; elem[1][3] = T(0);
		elem[2][0] = m.elem[2][0]; elem[2][1] = m.elem[2][1]; elem[2][2] = m.elem[2][2]; elem[2][3] = T(0);
		elem[3][0] = elem[3][1] = elem[3][2] = T(0); elem[3][3] = T(1);
	}
	
	operator const T* () const { return (const T*) elem; }
	operator T* () { return (T *) elem; }
	
	mat4& operator += (const mat4& m)
	{
		elem[0][0] += m.elem[0][0]; elem[0][1] += m.elem[0][1]; elem[0][2] += m.elem[0][2]; elem[0][3] += m.elem[0][3];
		elem[1][0] += m.elem[1][0]; elem[1][1] += m.elem[1][1]; elem[1][2] += m.elem[1][2]; elem[1][3] += m.elem[1][3];				
		elem[2][0] += m.elem[2][0]; elem[2][1] += m.elem[2][1]; elem[2][2] += m.elem[2][2]; elem[2][3] += m.elem[2][3];
		elem[3][0] += m.elem[3][0]; elem[3][1] += m.elem[3][1]; elem[3][2] += m.elem[3][2]; elem[3][3] += m.elem[3][3];
		return *this;
	}
	mat4& operator -= (const mat4& m)
	{
		elem[0][0] -= m.elem[0][0]; elem[0][1] -= m.elem[0][1]; elem[0][2] -= m.elem[0][2]; elem[0][3] -= m.elem[0][3];
		elem[1][0] -= m.elem[1][0]; elem[1][1] -= m.elem[1][1]; elem[1][2] -= m.elem[1][2]; elem[1][3] -= m.elem[1][3];				
		elem[2][0] -= m.elem[2][0]; elem[2][1] -= m.elem[2][1]; elem[2][2] -= m.elem[2][2]; elem[2][3] -= m.elem[2][3];
		elem[3][0] -= m.elem[3][0]; elem[3][1] -= m.elem[3][1]; elem[3][2] -= m.elem[3][2]; elem[3][3] -= m.elem[3][3];
		return *this;
	}
	mat4& operator /= (const mat4& m)
	{
		elem[0][0] /= m.elem[0][0]; elem[0][1] /= m.elem[0][1]; elem[0][2] /= m.elem[0][2]; elem[0][3] /= m.elem[0][3];
		elem[1][0] /= m.elem[1][0]; elem[1][1] /= m.elem[1][1]; elem[1][2] /= m.elem[1][2]; elem[1][3] /= m.elem[1][3];				
		elem[2][0] /= m.elem[2][0]; elem[2][1] /= m.elem[2][1]; elem[2][2] /= m.elem[2][2]; elem[2][3] /= m.elem[2][3];
		elem[3][0] /= m.elem[3][0]; elem[3][1] /= m.elem[3][1]; elem[3][2] /= m.elem[3][2]; elem[3][3] /= m.elem[3][3];
		return *this;
	}
	
	mat4& operator += (const T s)
	{
		elem[0][0] += s; elem[0][1] += s; elem[0][2] += s; elem[0][3] += s;
		elem[1][0] += s; elem[1][1] += s; elem[1][2] += s; elem[1][3] += s;
		elem[2][0] += s; elem[2][1] += s; elem[2][2] += s; elem[2][3] += s;
		elem[3][0] += s; elem[3][1] += s; elem[3][2] += s; elem[3][3] += s;
		return *this;
	}
	mat4& operator -= (const T s)
	{
		elem[0][0] -= s; elem[0][1] -= s; elem[0][2] -= s; elem[0][3] -= s;
		elem[1][0] -= s; elem[1][1] -= s; elem[1][2] -= s; elem[1][3] -= s;
		elem[2][0] -= s; elem[2][1] -= s; elem[2][2] -= s; elem[2][3] -= s;
		elem[3][0] -= s; elem[3][1] -= s; elem[3][2] -= s; elem[3][3] -= s;
		return *this;
	}
	mat4& operator *= (const T s)
	{
		elem[0][0] *= s; elem[0][1] *= s; elem[0][2] *= s; elem[0][3] *= s;
		elem[1][0] *= s; elem[1][1] *= s; elem[1][2] *= s; elem[1][3] *= s;
		elem[2][0] *= s; elem[2][1] *= s; elem[2][2] *= s; elem[2][3] *= s;
		elem[3][0] *= s; elem[3][1] *= s; elem[3][2] *= s; elem[3][3] *= s;
		return *this;
	}
	mat4& operator /= (const T s)
	{
		elem[0][0] /= s; elem[0][1] /= s; elem[0][2] /= s; elem[0][3] /= s;
		elem[1][0] /= s; elem[1][1] /= s; elem[1][2] /= s; elem[1][3] /= s;
		elem[2][0] /= s; elem[2][1] /= s; elem[2][2] /= s; elem[2][3] /= s;
		elem[3][0] /= s; elem[3][1] /= s; elem[3][2] /= s; elem[3][3] /= s;
		return *this;
	}
};

// matrix op matrix
template <typename T> inline mat4<T> operator + (const mat4<T>& a, const mat4<T>& b)
{
	return mat4<T>(
		a.elem[0][0] + b.elem[0][0], a.elem[0][1] + b.elem[0][1], a.elem[0][2] + b.elem[0][2], a.elem[0][3] + b.elem[0][3],
		a.elem[1][0] + b.elem[1][0], a.elem[1][1] + b.elem[1][1], a.elem[1][2] + b.elem[1][2], a.elem[1][3] + b.elem[1][3],
		a.elem[2][0] + b.elem[2][0], a.elem[2][1] + b.elem[2][1], a.elem[2][2] + b.elem[2][2], a.elem[2][3] + b.elem[2][3],
		a.elem[3][0] + b.elem[3][0], a.elem[3][1] + b.elem[3][1], a.elem[3][2] + b.elem[3][2], a.elem[3][3] + b.elem[3][3]);
}
template <typename T> inline mat4<T> operator - (const mat4<T>& a)
{
	return mat4<T>(
		-a.elem[0][0], -a.elem[0][1], -a.elem[0][2], -a.elem[0][3],
		-a.elem[1][0], -a.elem[1][1], -a.elem[1][2], -a.elem[1][3],
		-a.elem[2][0], -a.elem[2][1], -a.elem[2][2], -a.elem[2][3],
		-a.elem[3][0], -a.elem[3][1], -a.elem[3][2], -a.elem[3][3]);
}
template <typename T> inline mat4<T> operator - (const mat4<T>& a, const mat4<T>& b)
{
	return mat4<T>(
		a.elem[0][0] - b.elem[0][0], a.elem[0][1] - b.elem[0][1], a.elem[0][2] - b.elem[0][2], a.elem[0][3] - b.elem[0][3],
		a.elem[1][0] - b.elem[1][0], a.elem[1][1] - b.elem[1][1], a.elem[1][2] - b.elem[1][2], a.elem[1][3] - b.elem[1][3],
		a.elem[2][0] - b.elem[2][0], a.elem[2][1] - b.elem[2][1], a.elem[2][2] - b.elem[2][2], a.elem[2][3] - b.elem[2][3],
		a.elem[3][0] - b.elem[3][0], a.elem[3][1] - b.elem[3][1], a.elem[3][2] - b.elem[3][2], a.elem[3][3] - b.elem[3][3]);
}
template <typename T> inline mat4<T> operator / (const mat4<T>& a, const mat4<T>& b)
{
	return mat4<T>(
		a.elem[0][0] / b.elem[0][0], a.elem[0][1] / b.elem[0][1], a.elem[0][2] / b.elem[0][2], a.elem[0][3] / b.elem[0][3],
		a.elem[1][0] / b.elem[1][0], a.elem[1][1] / b.elem[1][1], a.elem[1][2] / b.elem[1][2], a.elem[1][3] / b.elem[1][3],
		a.elem[2][0] / b.elem[2][0], a.elem[2][1] / b.elem[2][1], a.elem[2][2] / b.elem[2][2], a.elem[2][3] / b.elem[2][3],
		a.elem[3][0] / b.elem[3][0], a.elem[3][1] / b.elem[3][1], a.elem[3][2] / b.elem[3][2], a.elem[3][3] / b.elem[3][3]);
}

// matrix op scalar
template <typename T> inline mat4<T> operator + (const mat4<T>& a, const T s)
{
	return mat4<T>(
		a.elem[0][0] + s, a.elem[0][1] + s, a.elem[0][2] + s, a.elem[0][3] + s,
		a.elem[1][0] + s, a.elem[1][1] + s, a.elem[1][2] + s, a.elem[1][3] + s,
		a.elem[2][0] + s, a.elem[2][1] + s, a.elem[2][2] + s, a.elem[2][3] + s,
		a.elem[3][0] + s, a.elem[3][1] + s, a.elem[3][2] + s, a.elem[3][3] + s);
}
template <typename T> inline mat4<T> operator - (const mat4<T>& a, const T s)
{
	return mat4<T>(
		a.elem[0][0] - s, a.elem[0][1] - s, a.elem[0][2] - s, a.elem[0][3] - s,
		a.elem[1][0] - s, a.elem[1][1] - s, a.elem[1][2] - s, a.elem[1][3] - s,
		a.elem[2][0] - s, a.elem[2][1] - s, a.elem[2][2] - s, a.elem[2][3] - s,
		a.elem[3][0] - s, a.elem[3][1] - s, a.elem[3][2] - s, a.elem[3][3] - s);
}
template <typename T> inline mat4<T> operator * (const mat4<T>& a, const T s)
{
	return mat4<T>(
		a.elem[0][0] * s, a.elem[0][1] * s, a.elem[0][2] * s, a.elem[0][3] * s,
		a.elem[1][0] * s, a.elem[1][1] * s, a.elem[1][2] * s, a.elem[1][3] * s,
		a.elem[2][0] * s, a.elem[2][1] * s, a.elem[2][2] * s, a.elem[2][3] * s,
		a.elem[3][0] * s, a.elem[3][1] * s, a.elem[3][2] * s, a.elem[3][3] * s);
}
template <typename T> inline mat4<T> operator / (const mat4<T>& a, const T s)
{
	return mat4<T>(
		a.elem[0][0] / s, a.elem[0][1] / s, a.elem[0][2] / s, a.elem[0][3] / s,
		a.elem[1][0] / s, a.elem[1][1] / s, a.elem[1][2] / s, a.elem[1][3] / s,
		a.elem[2][0] / s, a.elem[2][1] / s, a.elem[2][2] / s, a.elem[2][3] / s,
		a.elem[3][0] / s, a.elem[3][1] / s, a.elem[3][2] / s, a.elem[3][3] / s);
}

// scalar op matrix
template <typename T> inline mat4<T> operator + (const T s, const mat4<T>& a)
{
	return mat4<T>(
		s + a.elem[0][0], s + a.elem[0][1], s + a.elem[0][2], s + a.elem[0][3],
		s + a.elem[1][0], s + a.elem[1][1], s + a.elem[1][2], s + a.elem[1][3],
		s + a.elem[2][0], s + a.elem[2][1], s + a.elem[2][2], s + a.elem[2][3],
		s + a.elem[3][0], s + a.elem[3][1], s + a.elem[3][2], s + a.elem[3][3]);
}
template <typename T> inline mat4<T> operator - (const T s, const mat4<T>& a)
{
	return mat4<T>(
		s - a.elem[0][0], s - a.elem[0][1], s - a.elem[0][2], s - a.elem[0][3],
		s - a.elem[1][0], s - a.elem[1][1], s - a.elem[1][2], s - a.elem[1][3],
		s - a.elem[2][0], s - a.elem[2][1], s - a.elem[2][2], s - a.elem[2][3],
		s - a.elem[3][0], s - a.elem[3][1], s - a.elem[3][2], s - a.elem[3][3]);
}
template <typename T> inline mat4<T> operator * (const T s, const mat4<T>& a)
{
	return mat4<T>(
		s * a.elem[0][0], s * a.elem[0][1], s * a.elem[0][2], s * a.elem[0][3],
		s * a.elem[1][0], s * a.elem[1][1], s * a.elem[1][2], s * a.elem[1][3],
		s * a.elem[2][0], s * a.elem[2][1], s * a.elem[2][2], s * a.elem[2][3],
		s * a.elem[3][0], s * a.elem[3][1], s * a.elem[3][2], s * a.elem[3][3]);
}
template <typename T> inline mat4<T> operator / (const T s, const mat4<T>& a)
{
	return mat4<T>(
		s / a.elem[0][0], s / a.elem[0][1], s / a.elem[0][2], s / a.elem[0][3],
		s / a.elem[1][0], s / a.elem[1][1], s / a.elem[1][2], s / a.elem[1][3],
		s / a.elem[2][0], s / a.elem[2][1], s / a.elem[2][2], s / a.elem[2][3],
		s / a.elem[3][0], s / a.elem[3][1], s / a.elem[3][2], s / a.elem[3][3]);
}

// matrix multiplication
template <typename T> inline mat4<T> operator * (const mat4<T>& a, const mat4<T>& b)
{
	return mat4<T>(
		a.elem[0][0] * b.elem[0][0] + a.elem[0][1] * b.elem[1][0] + a.elem[0][2] * b.elem[2][0] + a.elem[0][3] * b.elem[3][0],
		a.elem[0][0] * b.elem[0][1] + a.elem[0][1] * b.elem[1][1] + a.elem[0][2] * b.elem[2][1] + a.elem[0][3] * b.elem[3][1],
		a.elem[0][0] * b.elem[0][2] + a.elem[0][1] * b.elem[1][2] + a.elem[0][2] * b.elem[2][2] + a.elem[0][3] * b.elem[3][2],
		a.elem[0][0] * b.elem[0][3] + a.elem[0][1] * b.elem[1][3] + a.elem[0][2] * b.elem[2][3] + a.elem[0][3] * b.elem[3][3],

		a.elem[1][0] * b.elem[0][0] + a.elem[1][1] * b.elem[1][0] + a.elem[1][2] * b.elem[2][0] + a.elem[1][3] * b.elem[3][0],
		a.elem[1][0] * b.elem[0][1] + a.elem[1][1] * b.elem[1][1] + a.elem[1][2] * b.elem[2][1] + a.elem[1][3] * b.elem[3][1],
		a.elem[1][0] * b.elem[0][2] + a.elem[1][1] * b.elem[1][2] + a.elem[1][2] * b.elem[2][2] + a.elem[1][3] * b.elem[3][2],
		a.elem[1][0] * b.elem[0][3] + a.elem[1][1] * b.elem[1][3] + a.elem[1][2] * b.elem[2][3] + a.elem[1][3] * b.elem[3][3],

		a.elem[2][0] * b.elem[0][0] + a.elem[2][1] * b.elem[1][0] + a.elem[2][2] * b.elem[2][0] + a.elem[2][3] * b.elem[3][0],
		a.elem[2][0] * b.elem[0][1] + a.elem[2][1] * b.elem[1][1] + a.elem[2][2] * b.elem[2][1] + a.elem[2][3] * b.elem[3][1],
		a.elem[2][0] * b.elem[0][2] + a.elem[2][1] * b.elem[1][2] + a.elem[2][2] * b.elem[2][2] + a.elem[2][3] * b.elem[3][2],
		a.elem[2][0] * b.elem[0][3] + a.elem[2][1] * b.elem[1][3] + a.elem[2][2] * b.elem[2][3] + a.elem[2][3] * b.elem[3][3],

		a.elem[3][0] * b.elem[0][0] + a.elem[3][1] * b.elem[1][0] + a.elem[3][2] * b.elem[2][0] + a.elem[3][3] * b.elem[3][0],
		a.elem[3][0] * b.elem[0][1] + a.elem[3][1] * b.elem[1][1] + a.elem[3][2] * b.elem[2][1] + a.elem[3][3] * b.elem[3][1],
		a.elem[3][0] * b.elem[0][2] + a.elem[3][1] * b.elem[1][2] + a.elem[3][2] * b.elem[2][2] + a.elem[3][3] * b.elem[3][2],
		a.elem[3][0] * b.elem[0][3] + a.elem[3][1] * b.elem[1][3] + a.elem[3][2] * b.elem[2][3] + a.elem[3][3] * b.elem[3][3]);
}

template <typename T> inline mat2<T>::mat2(const mat3<T>& m)
{
	elem[0][0] = m.elem[0][0]; elem[0][1] = m.elem[0][1];
	elem[1][0] = m.elem[1][0]; elem[1][1] = m.elem[1][1];
}

template <typename T> inline mat3<T>::mat3(const mat4<T>& m)
{
	elem[0][0] = m.elem[0][0]; elem[0][1] = m.elem[0][1]; elem[0][2] = m.elem[0][2];
	elem[1][0] = m.elem[1][0]; elem[1][1] = m.elem[1][1]; elem[1][2] = m.elem[1][2];
	elem[2][0] = m.elem[2][0]; elem[2][1] = m.elem[2][1]; elem[2][2] = m.elem[2][2];
}

// common matrix funcitons

template <typename T> inline mat2<T> transpose(const mat2<T>& m)
{
	return mat2<T>(
		m.elem[0][0], m.elem[1][0],
		m.elem[0][1], m.elem[1][1]
	);	
}

template <typename T> inline mat3<T> transpose(const mat3<T>& m)
{
	return mat3<T>(
		m.elem[0][0], m.elem[1][0], m.elem[2][0], 
		m.elem[0][1], m.elem[1][1], m.elem[2][1], 
		m.elem[0][2], m.elem[1][2], m.elem[2][2]
	);	
}

template <typename T> inline mat4<T> transpose(const mat4<T>& m)
{
	return mat4<T>(
		m.elem[0][0], m.elem[1][0], m.elem[2][0], m.elem[3][0], 
		m.elem[0][1], m.elem[1][1], m.elem[2][1], m.elem[3][1],
		m.elem[0][2], m.elem[1][2], m.elem[2][2], m.elem[3][2],
		m.elem[0][3], m.elem[1][3], m.elem[2][3], m.elem[3][3]
	);	
}

template <typename T> inline mat2<T> identity2()
{
	return mat2<T>(
		T(1), T(0),
		T(0), T(1)
	);
}

template <typename T> inline mat3<T> identity3()
{
	return mat3<T>(
		T(1), T(0), T(0),
		T(0), T(1), T(0),
		T(0), T(0), T(1)
	);
}

template <typename T> inline mat4<T> identity4()
{
	return mat4<T>(
		T(1), T(0), T(0), T(0),
		T(0), T(1), T(0), T(0),
		T(0), T(0), T(1), T(0),
		T(0), T(0), T(0), T(1)
	);
}

template <typename T> inline T det(const mat2<T>& m)
{
	return m.elem[0][0] * m.elem[1][1] - m.elem[0][1] * m.elem[1][0];
}

template <typename T> inline T det(const mat3<T>& m)
{
	return (
		m.elem[0][0] * (m.elem[1][1] * m.elem[2][2] - m.elem[1][2] * m.elem[2][1]) +
		m.elem[1][0] * (m.elem[0][2] * m.elem[2][1] - m.elem[0][1] * m.elem[2][2]) +
		m.elem[2][0] * (m.elem[0][1] * m.elem[1][2] - m.elem[0][2] * m.elem[1][1])
	);
}

template <typename T> inline T det(const mat4<T>& m)
{
	return (
		m.elem[0][0] * (
			m.elem[1][1] * (m.elem[2][2] * m.elem[3][3] - m.elem[2][3] * m.elem[3][2]) +
			m.elem[2][1] * (m.elem[1][3] * m.elem[3][2] - m.elem[1][2] * m.elem[3][3]) +
			m.elem[3][1] * (m.elem[1][2] * m.elem[2][3] - m.elem[1][3] * m.elem[2][2])
		) -
		m.elem[1][0] * (
			m.elem[0][1] * (m.elem[2][2] * m.elem[3][3] - m.elem[2][3] * m.elem[3][2]) +
			m.elem[2][1] * (m.elem[0][3] * m.elem[3][2] - m.elem[0][2] * m.elem[3][3]) +
			m.elem[3][1] * (m.elem[0][2] * m.elem[2][3] - m.elem[0][3] * m.elem[2][2])
		) +
		m.elem[2][0] * (
			m.elem[0][1] * (m.elem[1][2] * m.elem[3][3] - m.elem[1][3] * m.elem[3][2]) +
			m.elem[1][1] * (m.elem[0][3] * m.elem[3][2] - m.elem[0][2] * m.elem[3][3]) +
			m.elem[3][1] * (m.elem[0][2] * m.elem[1][3] - m.elem[0][3] * m.elem[1][2])
		) -
		m.elem[3][0] * (
			m.elem[0][1] * (m.elem[1][2] * m.elem[2][3] - m.elem[1][3] * m.elem[2][2]) +
			m.elem[1][1] * (m.elem[0][3] * m.elem[2][2] - m.elem[0][2] * m.elem[2][3]) +
			m.elem[2][1] * (m.elem[0][2] * m.elem[1][3] - m.elem[0][3] * m.elem[1][2])
		)
	);
}

template <typename T> inline T trace(const mat2<T>& m)
{
	return m.elem[0][0] + m.elem[1][1];
}

template <typename T> inline T trace(const mat3<T>& m)
{
	return m.elem[0][0] + m.elem[1][1] + m.elem[2][2];
}

template <typename T> inline T trace(const mat4<T>& m)
{
	return m.elem[0][0] + m.elem[1][1] + m.elem[2][2] + m.elem[3][3];
}

template <typename T> inline mat2<T> adjoint(const mat2<T>& m)
{
	return mat2<T>(
		m.elem[1][1], -m.elem[0][1],
		-m.elem[1][0], m.elem[0][0]
	);
}

// Return a minor of the supplied 3x3 matrix by omitting the _jth row
// and _kth column.
// It defaults to return the upper left 2x2 matrix
template <typename T> inline mat2<T> minor(const mat3<T>& m, int _j = 2, int _k = 2) {
	mat2<T> r;
	
	int jp = (0 >= _j) ? 1 : 0;
	int kp = (0 >= _k) ? 1 : 0;
	r.elem[0][0] = m.elem[jp][kp];
	
	kp = (1 >= _k) ? 1 : 0;
	r.elem[0][1] = m.elem[jp][1 + kp];
	
	jp = (1 >= _j) ? 1 : 0;
	kp = (0 >= _k) ? 1 : 0;
	r.elem[1][0] = m.elem[1 + jp][kp];
	
	kp = (1 >= _k) ? 1 : 0;
	r.elem[1][1] = m.elem[1 + jp][1 + kp];
	
	return r;
}

// Return a minor of the supplied 4x4 matrix by omitting the _jth row
// and _kth column.
// It defaults to return the upper left 3x3 matrix
template <typename T> inline mat3<T> minor(const mat4<T>& m, int _j = 3, int _k = 3) {
	mat3<T> r;
	
	int jp = (0 >= _j) ? 1 : 0;
	int kp = (0 >= _k) ? 1 : 0;
	r.elem[0][0] = m.elem[jp][kp];
	
	kp = (1 >= _k) ? 1 : 0;
	r.elem[0][1] = m.elem[jp][1 + kp];
	
	kp = (2 >= _k) ? 1 : 0;
	r.elem[0][2] = m.elem[jp][2 + kp];
	
	jp = (1 >= _j) ? 1 : 0;
	kp = (0 >= _k) ? 1 : 0;
	r.elem[1][0] = m.elem[1 + jp][kp];
	
	kp = (1 >= _k) ? 1 : 0;
	r.elem[1][1] = m.elem[1 + jp][1 + kp];
	
	kp = (2 >= _k) ? 1 : 0;
	r.elem[1][2] = m.elem[1 + jp][2 + kp];
	
	jp = (2 >= _j) ? 1 : 0;
	kp = (0 >= _k) ? 1 : 0;
	r.elem[2][0] = m.elem[2 + jp][kp];
	
	kp = (1 >= _k) ? 1 : 0;
	r.elem[2][1] = m.elem[2 + jp][1 + kp];
	
	kp = (2 >= _k) ? 1 : 0;
	r.elem[2][2] = m.elem[2 + jp][2 + kp];
	
	return r;

}

template <typename T> inline mat3<T> adjoint(const mat3<T>& m) {
	return mat3<T>(
		 det(minor(m, 0, 0)), -det(minor(m, 1, 0)),  det(minor(m, 2, 0)),
		-det(minor(m, 0, 1)),  det(minor(m, 1, 1)), -det(minor(m, 2, 1)),
		 det(minor(m, 0, 2)), -det(minor(m, 1, 2)),  det(minor(m, 2, 2))
	);
}

template <typename T> inline mat4<T> adjoint(const mat4<T>& m) {
	return mat4<T>(
		 det(minor(m, 0, 0)), -det(minor(m, 1, 0)),  det(minor(m, 2, 0)), -det(minor(m, 3, 0)), 
		-det(minor(m, 0, 1)),  det(minor(m, 1, 1)), -det(minor(m, 2, 1)),  det(minor(m, 3, 1)), 
		 det(minor(m, 0, 2)), -det(minor(m, 1, 2)),  det(minor(m, 2, 2)), -det(minor(m, 3, 2)), 
		-det(minor(m, 0, 3)),  det(minor(m, 1, 3)), -det(minor(m, 2, 3)),  det(minor(m, 3, 3))
	);
}

template <typename T> inline mat2<T> inverse(const mat2<T>& m)
{
	return inv(det(m)) * adjoint(m) ;
}

template <typename T> inline mat3<T> inverse(const mat3<T>& m)
{
	return inv(det(m)) * adjoint(m);
}

template <typename T> inline mat4<T> inverse(const mat4<T>& m)
{
	return inv(det(m)) * adjoint(m);
}

// Matrix * Vector multiplication

template <typename T> inline vec2<T> operator * (const mat2<T>& m, const vec2<T>& v)
{
	return vec2<T>(
		m.elem[0][0] * v[0] + m.elem[0][1] * v[1],
		m.elem[1][0] * v[0] + m.elem[1][1] * v[1]
	);
}

template <typename T> inline vec3<T> operator * (const mat3<T>& m, const vec3<T>& v)
{
	return vec3<T>(
		m.elem[0][0] * v[0] + m.elem[0][1] * v[1] + m.elem[0][2] * v[2],	
		m.elem[1][0] * v[0] + m.elem[1][1] * v[1] + m.elem[1][2] * v[2],
		m.elem[2][0] * v[0] + m.elem[2][1] * v[1] + m.elem[2][2] * v[2]
	);
}

template <typename T> inline vec4<T> operator * (const mat4<T>& m, const vec4<T>& v)
{
	return vec4<T>(
		m.elem[0][0] * v[0] + m.elem[0][1] * v[1] + m.elem[0][2] * v[2] + m.elem[0][3] * v[3],
		m.elem[1][0] * v[0] + m.elem[1][1] * v[1] + m.elem[1][2] * v[2] + m.elem[1][3] * v[3],
		m.elem[2][0] * v[0] + m.elem[2][1] * v[1] + m.elem[2][2] * v[2] + m.elem[2][3] * v[3],
		m.elem[3][0] * v[0] + m.elem[3][1] * v[1] + m.elem[3][2] * v[2] + m.elem[3][3] * v[3]
	);
}

// Vector * Matrix multiplication

template <typename T> inline vec2<T> operator* (const vec2<T>& v, const mat2<T>& m)
{
	return vec2<T>(	
		v[0] * m.elem[0][0] + v[1] * m.elem[1][0],
		v[0] * m.elem[0][1] + v[1] * m.elem[1][1]
	);
}

template <typename T> inline vec3<T> operator* (const vec3<T>& v, const mat3<T>& m)
{
	return vec3<T>(	
		v[0] * m.elem[0][0] + v[1] * m.elem[1][0] + v[2] * m.elem[2][0],
		v[0] * m.elem[0][1] + v[1] * m.elem[1][1] + v[2] * m.elem[2][1],
		v[0] * m.elem[0][2] + v[1] * m.elem[1][2] + v[2] * m.elem[2][2]
	);
}

template <typename T> inline vec4<T> operator* (const vec4<T>& v, const mat4<T>& m)
{
	return vec4<T>(
		v[0] * m.elem[0][0] + v[1] * m.elem[1][0] + v[2] * m.elem[2][0] + v[3] * m.elem[3][0],
		v[0] * m.elem[0][1] + v[1] * m.elem[1][1] + v[2] * m.elem[2][1] + v[3] * m.elem[3][1],	
		v[0] * m.elem[0][2] + v[1] * m.elem[1][2] + v[2] * m.elem[2][2] + v[3] * m.elem[3][2],
		v[0] * m.elem[0][3] + v[1] * m.elem[1][3] + v[2] * m.elem[2][3] + v[3] * m.elem[3][3]
	);
}

// transformations for points and vectors
// (potentially faster than a full matrix * vector multiplication)

// computes vec3(m * vec4(v, 1.0))
template <typename T> inline vec3<T> transform_point(const mat4<T>& m, const vec3<T>& v)
{
	return vec3<T>(
		m.elem[0][0] * v[0] + m.elem[0][1] * v[1] + m.elem[0][2] * v[2] + m.elem[0][3],
		m.elem[1][0] * v[0] + m.elem[1][1] * v[1] + m.elem[1][2] * v[2] + m.elem[1][3],
		m.elem[2][0] * v[0] + m.elem[2][1] * v[1] + m.elem[2][2] * v[2] + m.elem[2][3]		
	);
}


// computes vec3<T>(m * vec4<T>(v, 0.0))
template <typename T> inline vec3<T> transform_vector(const mat4<T>& m, const vec3<T>& v)
{
	return vec3<T>(
		m.elem[0][0] * v[0] + m.elem[0][1] * v[1] + m.elem[0][2] * v[2],
		m.elem[1][0] * v[0] + m.elem[1][1] * v[1] + m.elem[1][2] * v[2],
		m.elem[2][0] * v[0] + m.elem[2][1] * v[1] + m.elem[2][2] * v[2]		
	);
}

// computes vec3<T>(transpose(m) * vec4<T>(v, 1.0))
template <typename T> inline vec3<T> transform_point_transpose(const mat4<T>& m, const vec3<T>& v)
{
	return vec3<T>(
		v[0] * m.elem[0][0] + v[1] * m.elem[1][0] + v[2] * m.elem[2][0] + m.elem[3][0],
		v[0] * m.elem[0][1] + v[1] * m.elem[1][1] + v[2] * m.elem[2][1] + m.elem[3][1],
		v[0] * m.elem[0][2] + v[1] * m.elem[1][2] + v[2] * m.elem[2][2] + m.elem[3][2]
	);
}

// computes vec3<T>(transpose(m) * vec4<T>(v, 0.0))
template <typename T> inline vec3<T> transform_vector_transpose(const mat4<T>& m, const vec3<T>& v)
{
	return vec3<T>(
		v[0] * m.elem[0][0] + v[1] * m.elem[1][0] + v[2] * m.elem[2][0],
		v[0] * m.elem[0][1] + v[1] * m.elem[1][1] + v[2] * m.elem[2][1],
		v[0] * m.elem[0][2] + v[1] * m.elem[1][2] + v[2] * m.elem[2][2]
	);
}

// computes vec2<T>(m * vec3<T>(v, 1.0))
template <typename T> inline vec2<T> transform_point(const mat3<T>& m, const vec2<T>& v)
{
	return vec2<T>(
		m.elem[0][0] * v[0] + m.elem[0][1] * v[1] + m.elem[0][2],
		m.elem[1][0] * v[0] + m.elem[1][1] * v[1] + m.elem[1][2]
	);
}

// computes vec2<T>(m * vec3<T>(v, 0.0))
template <typename T> inline vec2<T> transform_vector(const mat3<T>& m, const vec2<T>& v)
{
	return vec2<T>(
		m.elem[0][0] * v[0] + m.elem[0][1] * v[1],
		m.elem[1][0] * v[0] + m.elem[1][1] * v[1]
	);
}

// computes vec2<T>(transpose(m) * vec3<T>(v, 1.0))
template <typename T> inline vec2<T> transform_point_transpose(const mat3<T>& m, const vec2<T>& v)
{
	return vec2<T>(
		v[0] * m.elem[0][0] + v[1] * m.elem[1][0] + m.elem[2][0],
		v[0] * m.elem[0][1] + v[1] * m.elem[1][1] + m.elem[2][1]
	);
}

// computes vec2<T>(transpose(m) * vec3<T>(v, 0.0))
template <typename T> inline vec2<T> transform_vector_transpose(const mat3<T>& m, const vec2<T>& v)
{
	return vec2<T>(
		v[0] * m.elem[0][0] + v[1] * m.elem[1][0],
		v[0] * m.elem[0][1] + v[1] * m.elem[1][1]
	);
}

// Returns the inverse of a 4x4 matrix. It is assumed that the matrix passed
// as argument describes a rigid-body transformation.
template <typename T> inline mat4<T> fast_inverse(const mat4<T>& m)
{
	float tx, ty, tz;
	
	tx = -m.elem[0][0] * m.elem[0][3] - m.elem[1][0] * m.elem[1][3] - m.elem[2][0] * m.elem[2][3];
	ty = -m.elem[0][1] * m.elem[0][3] - m.elem[1][1] * m.elem[1][3] - m.elem[2][1] * m.elem[2][3];
	tz = -m.elem[0][2] * m.elem[0][3] - m.elem[1][2] * m.elem[1][3] - m.elem[2][2] * m.elem[2][3];
	
	return mat4<T>(
		m.elem[0][0], m.elem[1][0], m.elem[2][0], tx,
		m.elem[0][1], m.elem[1][1], m.elem[2][1], ty,
		m.elem[0][2], m.elem[1][2], m.elem[2][2], tz,
		0, 0, 0, 1
	);
}

// Multiplies the vector v1 by the transposed vector v2 and returns the
// resulting matrix.
//   x1                 m00 m01 
// ( y1 ) * (x2 y2) = ( m10 m11 )
template <typename T> inline mat2<T> outer_product(const vec2<T>& v1, const vec2<T>& v2) {
	mat2<T> r;
	
	for ( int j = 0; j < 2; ++j )
	for ( int k = 0; k < 2; ++k )
		r.elem[j][k] = v1[j] * v2[k];
	
	return r;
}
// Multiplies the vector v1 by the transposed vector v2 and returns the
// resulting matrix.
//   x1                    m00 m01 m02
// ( y1 ) * (x2 y2 z2) = ( m10 m11 m12 )
//   z1                    m20 m21 m22
template <typename T> inline mat3<T> outer_product(const vec3<T>& v1, const vec3<T>& v2) {
	mat3<T> r;
	
	for ( int j = 0; j < 3; ++j )
	for ( int k = 0; k < 3; ++k )
		r.elem[j][k] = v1[j] * v2[k];
	
	return r;
}

// Multiplies the vector v1 by the transposed vector v2 and returns the
// resulting matrix.
//   x1                       m00 m01 m02 m03
// ( y1 ) * (x2 y2 z2 w2) = ( m10 m11 m12 m13 )
//   z1                       m20 m21 m22 m13
//   w1                       m30 m31 m32 m33
template <typename T> inline mat4<T> outer_product(const vec4<T>& v1, const vec4<T>& v2) {
	mat4<T> r;
	
	for ( int j = 0; j < 4; ++j )
	for ( int k = 0; k < 4; ++k )
		r.elem[j][k] = v1[j] * v2[k];
	
	return r;
}

template <typename T> inline mat4<T> translation_matrix(const T x, const T y, const T z)
{
	return mat4<T>(
		T(1), T(0), T(0), x,
		T(0), T(1), T(0), y,
		T(0), T(0), T(1), z,
		T(0), T(0), T(0), T(1)
	);
}

template <typename T> inline mat4<T> translation_matrix(const vec3<T>& v)
{
	return translation_matrix(v.x, v.y, v.z);
}

template <typename T> inline mat4<T> scaling_matrix(const T x, const T y, const T z)
{
	return mat4<T>(
		x, T(0), T(0), T(0),
		T(0), y, T(0), T(0),
		T(0), T(0), z, T(0),
		T(0), T(0), T(0), T(1)
	);
}

template <typename T> inline mat4<T> scaling_matrix(const vec3<T>& v)
{
	return scaling_matrix(v.x, v.y, v.z);
}

template <typename T> inline mat4<T> rotation_matrix(const T angle, const vec3<T>& v)
{
	const T a = angle * T(M_PI/180) ;
	const vec3<T> u = normalize(v);
	
	const mat3<T> S(
	   T(0), -u[2],  u[1],
		 u[2],  T(0), -u[0],
		-u[1],  u[0],  T(0) 
	);
	
	const mat3<T> uut = outer_product(u, u);
	const mat3<T> R = uut + T(cos(a)) * (identity3<T>() - uut) + T(sin(a)) * S;
	
	return mat4<T>(R);
}


template <typename T> inline mat4<T> rotation_matrix(const T angle, const T x, const T y, const T z)
{
	return rotation_matrix(angle, vec3<T>(x, y, z));
}

// Constructs a shear-matrix that shears component i by factor with
// Respect to component j.
template <typename T> inline mat4<T> shear(const int i, const int j, const T factor) {
	mat4<T> m = identity4<T>();
	
	m.elem[i][j] = factor;
	
	return m;
}

template <typename T> inline mat4<T> euler(const T head, const T pitch, const T roll) {
	return rotation_matrix(roll, 0, 0, 1) * rotation_matrix(pitch, 1, 0, 0) * rotation_matrix(head, 0, 1, 0);
}

template <typename T> inline mat4<T> frustum_matrix(const T l, const T r, const T b, const T t, const T n, const T f)
{
	return mat4<T>(
		(2 * n)/(r - l), T(0), (r + l)/(r - l), T(0),
		T(0), (2 * n)/(t - b), (t + b)/(t - b), T(0),
		T(0), T(0), -(f + n)/(f - n), -(2 * f * n)/(f - n),
		T(0), T(0), -T(1), T(0)
	);
}

template <typename T> inline mat4<T> perspective_matrix(const T fovy, const T aspect, const T zNear, const T zFar) {
		const T dz = zFar - zNear;
		const T rad = fovy / T(2) * T(M_PI/180);
		const T s = sin(rad);
		
		if ( ( dz == T(0) ) || ( s == T(0) ) || ( aspect == T(0) ) ) {
			return identity4<T>();
		}
		
		const T cot = cos(rad) / s;
		
		mat4<T> m = identity4<T>();
		m[0]  = cot / aspect;
		m[5]  = cot;
		m[10] = -(zFar + zNear) / dz;
		m[14] = T(-1);
		m[11] = -2 * zNear * zFar / dz;
		m[15] = T(0);
		
		return m;
}

template <typename T> inline mat4<T> ortho_matrix(const T l, const T r, const T b, const T t, const T n, const T f)
{
	return mat4<T>(
		T(2)/(r - l), T(0), T(0), -(r + l)/(r - l),
		T(0), T(2)/(t - b), T(0), -(t + b)/(t - b),
		T(0), T(0), -T(2)/(f - n), -(f + n)/(f - n),
		T(0), T(0), T(0), T(1)
	);
}

template <typename T> inline mat4<T> lookat_matrix(const vec3<T>& eye, const vec3<T>& center, const vec3<T>& up) {
	const vec3<T> forward = normalize(center - eye);
	const vec3<T> side = normalize(cross(forward, up));
	
	const vec3<T> up2 = cross(side, forward);
	
	mat4<T> m = identity4<T>();
	
	m.elem[0][0] = side[0];
	m.elem[0][1] = side[1];
	m.elem[0][2] = side[2];
	
	m.elem[1][0] = up2[0];
	m.elem[1][1] = up2[1];
	m.elem[1][2] = up2[2];
	
	m.elem[2][0] = -forward[0];
	m.elem[2][1] = -forward[1];
	m.elem[2][2] = -forward[2];
	
	return m * translation_matrix(-eye);
}

template <typename T> inline mat4<T> picking_matrix(const T x, const T y, const T dx, const T dy, int viewport[4]) {
	if (dx <= 0 || dy <= 0) { 
		return identity4<T>();
	}

	return 
		translation_matrix(
			(viewport[2] - 2 * (x - viewport[0])) / dx,
			(viewport[3] - 2 * (y - viewport[1])) / dy,
			0
		) *
		scaling_matrix(
			viewport[2] / dx,
			viewport[2] / dy,	
			1
		);
}

// Constructs a shadow matrix. q is the light source and p is the plane.
template <typename T> inline mat4<T> shadow_matrix(const vec4<T>& q, const vec4<T>& p) {
	mat4<T> m;
	
	m.elem[0][0] =  p.y * q[1] + p.z * q[2] + p.w * q[3];
	m.elem[0][1] = -p.y * q[0];
	m.elem[0][2] = -p.z * q[0];
	m.elem[0][3] = -p.w * q[0];
	
	m.elem[1][0] = -p.x * q[1];
	m.elem[1][1] =  p.x * q[0] + p.z * q[2] + p.w * q[3];
	m.elem[1][2] = -p.z * q[1];
	m.elem[1][3] = -p.w * q[1];
	

	m.elem[2][0] = -p.x * q[2];
	m.elem[2][1] = -p.y * q[2];
	m.elem[2][2] =  p.x * q[0] + p.y * q[1] + p.w * q[3];
	m.elem[2][3] = -p.w * q[2];

	m.elem[3][1] = -p.x * q[3];
	m.elem[3][2] = -p.y * q[3];
	m.elem[3][3] = -p.z * q[3];
	m.elem[3][0] =  p.x * q[0] + p.y * q[1] + p.z * q[2];

	return m;
}

// Quaternion class
template <typename T> 
struct quat {
	vec3<T>	v;
	T w;

	quat() {}
	quat(const vec3<T>& iv, const T iw)
		: v(iv), w(iw)
	{}
	quat(const T vx, const T vy, const T vz, const T iw)
		: v(vx, vy, vz), w(iw)
	{}

	operator const T* () const { return &(v[0]); }
	operator T* () { return &(v[0]); }	

	quat& operator += (const quat& q)
	{
		v += q.v;
		w += q.w;
		return *this;
	}
	quat& operator -= (const quat& q)
	{
		v -= q.v;
		w -= q.w;
		return *this;
	}
	quat& operator *= (const quat& r)
	{
		//q1 x q2 = [s1,v1] x [s2,v2] = [(s1*s2 - v1*v2),(s1*v2 + s2*v1 + v1xv2)].
		quat q;
		
		q.v = cross(v, r.v) + r.w * v + w * r.v;
		q.w = w * r.w - dot(v, r.v);
		
		*this = q;
		
		return *this;
	}
	quat& operator /= (const quat& q);

	quat& operator *= (const T s)
	{
		v *= s;
		w *= s;
		return *this;
	}
	quat& operator /= (const T s)
	{
		v /= s;
		w /= s;
		return *this;
	}
};

// forward declaration
template <typename T>
quat<T> inverse(const quat<T>& q);

template <typename T> inline quat<T>& quat<T>::operator /=(const quat<T> &q)
{
	return (*this) *= inverse(q);
}

// non member operators

template <typename T> inline quat<T> operator + (const quat<T>& q1, const quat<T>& q2)
{
	return quat<T>(q1.v + q2.v, q1.w + q2.w);
}

template <typename T> inline quat<T> operator - (const quat<T>& q1, const quat<T>& q2)
{
	return quat<T>(q1.v - q2.v, q1.w - q2.w);
}

template <typename T> inline quat<T> operator * (const quat<T>& q1, const quat<T>& q2)
{
	quat<T> q;
	
	q.v = cross(q1.v, q2.v) + q2.w * q1.v + q1.w * q2.v;
	q.w = q1.w * q2.w - dot(q1.v, q2.v);
	
	return q;
}

template <typename T> inline quat<T> operator / (const quat<T>& q1, const quat<T>& q2)
{
	return q1 * inverse(q2);
}

template <typename T> inline quat<T> operator * (const quat<T>& q, const T s)
{
	return quat<T>(q.v * s, q.w * s);
}

template <typename T> inline quat<T> operator * (const T s, const quat<T>& q)
{
	return quat<T>(s * q.v, q.w * s);
}

template <typename T> inline quat<T> operator / (const quat<T>& q, const T s)
{
	return quat<T>(q.v / s, q.w / s);
}


// Quaternion functions

template <typename T> inline T length2(const quat<T>& q)
{
	return q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
}

template <typename T> inline T length(const quat<T>& q)
{
	return sqrt(length2(q));
}

template <typename T> inline quat<T> normalize(const quat<T>& q)
{
	const T invl = T(1) / length(q);
	
	return q * invl;
}

template <typename T> inline quat<T> identityq()
{
	return quat<T>(T(0), T(0), T(0), T(1));
}

template <typename T> inline quat<T> conjugate(const quat<T>& q)
{
	return quat<T>(-q.v, q.w);
}

template <typename T> inline quat<T> inverse(const quat<T>& q)
{
	const T l = length2(q);

	if ( l > T(0) )
		return conjugate(q) / l;
	else
		return identityq<T>();
}


// quaternion utility functions

// the input quaternion is assumed to be normalized
template <typename T> inline mat3<T> quat_to_mat3(const quat<T>& q)
{
	// const quat<T> q = normalize(qq);

	const T xx = q[0] * q[0];
	const T xy = q[0] * q[1];
	const T xz = q[0] * q[2];
	const T xw = q[0] * q[3];
	
	const T yy = q[1] * q[1];
	const T yz = q[1] * q[2];
	const T yw = q[1] * q[3];
	
	const T zz = q[2] * q[2];
	const T zw = q[2] * q[3];
	
	return mat3<T>(
		1 - 2*(yy + zz), 2*(xy - zw), 2*(xz + yw),
		2*(xy + zw), 1 - 2*(xx + zz), 2*(yz - xw),
		2*(xz - yw), 2*(yz + xw), 1 - 2*(xx + yy)
	);
}

// the input quat<T>ernion is assumed to be normalized
template <typename T> inline mat4<T> quat_to_mat4(const quat<T>& q)
{
	// const quat<T> q = normalize(qq);

	const T xx = q[0] * q[0];
	const T xy = q[0] * q[1];
	const T xz = q[0] * q[2];
	const T xw = q[0] * q[3];
	
	const T yy = q[1] * q[1];
	const T yz = q[1] * q[2];
	const T yw = q[1] * q[3];
	
	const T zz = q[2] * q[2];
	const T zw = q[2] * q[3];
	
	return mat4<T>(
		1 - 2*(yy + zz), 2*(xy - zw), 2*(xz + yw), T(0),
		2*(xy + zw), 1 - 2*(xx + zz), 2*(yz - xw), T(0),
		2*(xz - yw), 2*(yz + xw), 1 - 2*(xx + yy), T(0),
		T(0), T(0), T(0), T(1)
	);
}

template <typename T> inline quat<T> mat_to_quat(const mat4<T>& m)
{
	const T t = m.elem[0][0] + m.elem[1][1] + m.elem[2][2] + T(1);
	quat<T> q;

	if ( t > 0 ) {
		const T s = T(0.5) / sqrt(t);
		q[3] = T(0.25) / s;
		q[0] = (m.elem[2][1] - m.elem[1][2]) * s;
		q[1] = (m.elem[0][2] - m.elem[2][0]) * s;
		q[2] = (m.elem[1][0] - m.elem[0][1]) * s;
	} else {
		if ( m.elem[0][0] > m.elem[1][1] && m.elem[0][0] > m.elem[2][2] ) {
			const T s = T(2) * sqrt( T(1) + m.elem[0][0] - m.elem[1][1] - m.elem[2][2]);
			q[0] = T(0.25) * s;
			q[1] = (m.elem[0][1] + m.elem[1][0] ) / s;
			q[2] = (m.elem[0][2] + m.elem[2][0] ) / s;
			q[3] = (m.elem[1][2] - m.elem[2][1] ) / s;
		} else if (m.elem[1][1] > m.elem[2][2]) {
			const T s = T(2) * sqrt( T(1) + m.elem[1][1] - m.elem[0][0] - m.elem[2][2]);
			q[0] = (m.elem[0][1] + m.elem[1][0] ) / s;
			q[1] = T(0.25) * s;
			q[2] = (m.elem[1][2] + m.elem[2][1] ) / s;
			q[3] = (m.elem[0][2] - m.elem[2][0] ) / s;
		} else {
			const T s = T(2) * sqrt( T(1) + m.elem[2][2] - m.elem[0][0] - m.elem[1][1] );
			q[0] = (m.elem[0][2] + m.elem[2][0] ) / s;
			q[1] = (m.elem[1][2] + m.elem[2][1] ) / s;
			q[2] = T(0.25) * s;
			q[3] = (m.elem[0][1] - m.elem[1][0] ) / s;
		}
	}
	
	return q;
}

template <typename T> inline quat<T> mat_to_quat(const mat3<T>& m)
{
	return mat_to_quat(mat4<T>(m));
}

// the angle is in radians
template <typename T> inline quat<T> quat_from_axis_angle(const vec3<T>& axis, const T a)
{
	quat<T> r;
	
	r.v = sin(a / T(2)) * normalize(axis);
	r.w = cos(a / T(2));
	
	return r;
}

// the angle is in radians
template <typename T> inline quat<T> quat_from_axis_angle(const T x, const T y, const T z, const T angle)
{
	return quat_from_axis_angle<T>(vec3<T>(x, y, z), angle);
}

// the angle is stored in radians
template <typename T> inline void quat_to_axis_angle(const quat<T>& qq, vec3<T>* axis, T *angle)
{
	quat<T> q = normalize(qq);
	
	*angle = 2 * acos(q.w);
	
	const T s = sin((*angle) / T(2));
	if ( s != T(0) ) {
		*axis = q.v / s;
	} else {
		* axis = vec3<T>(T(0), T(0), T(0));
	}
}

// quaternion dot product (quaternions are treated like a vec4)
template <typename T> inline T dot(const quat<T>& a, const quat<T>& b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

// Spherical linear interpolation
template <typename T> inline quat<T> slerp(const quat<T>& qq1, const quat<T>& qq2, const T t) {
	// slerp(q1,q2) = sin((1-t)*a)/sin(a) * q1  +  sin(t*a)/sin(a) * q2
	const quat<T> q1 = normalize(qq1);
	const quat<T> q2 = normalize(qq2);
	
	const T a = acos(dot(q1, q2));
	const T s = sin(a);
	
	#define EPS T(1e-5)

	if ( !(-EPS <= s && s <= EPS) ) {
		return sin((T(1)-t)*a)/s * q1  +  sin(t*a)/s * q2;
	} else {
		// if the angle is to small use a linear interpolation
		return (T(1)-t) * q1 + t * q2;
	}

	#undef EPS
}

// Sperical quadtratic interpolation using a smooth cubic spline
// The parameters a and b are the control points.
template <typename T> inline quat<T> squad(const quat<T>& q0, const quat<T>& a, const quat<T>& b, const quat<T>& q1, const T t) {
	return slerp(slerp(q0, q1, t),slerp(a, b, t), 2 * t * (1 - t));
}

} // end namespace vmath

#endif
