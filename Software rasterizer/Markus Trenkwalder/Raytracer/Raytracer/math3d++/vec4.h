/*
	math3d++ - A C++ 3d math library
	Copyright (c) 2004, Trenkwalder Markus
	All rights reserved. 
	
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:
	
	- Redistributions of source code must retain the above copyright notice,
	  this list of conditions and the following disclaimer.
	  
	- Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	  
	- Neither the name of the library's copyright owner nor the names
	  of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.
	  
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
	TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
	PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	
	Contact info:
	email: trenki2@gmx.net
*/

#ifndef MATH3DPP_VEC4_H
#define MATH3DPP_VEC4_H

#include "vec_decl.h"

/******************************************************************************/
// constructors

/// Initialization of all components to v (default = 0)
inline vec4::vec4(Real v) {
	data[0] = v;
	data[1] = v;
	data[2] = v;
	data[3] = v;
}

/// Initialization to (x,y,z,w)
inline vec4::vec4(Real x, Real y, Real z, Real w) {
	data[0] = x;
	data[1] = y;
	data[2] = z;
	data[3] = w;
}

/// Costruction from a vec3; w defaults to 1
inline vec4::vec4(const vec3& v, Real w) {
	data[0] = v[0];
	data[1] = v[1];
	data[2] = v[2];
	data[3] = w;
}

/******************************************************************************/
// Vector arithmetic operators

inline vec4& vec4::operator+= (const vec4& v) {
	data[0] += v.data[0];
	data[1] += v.data[1];
	data[2] += v.data[2];
	data[3] += v.data[3];
		
	return *this;
}

inline vec4& vec4::operator-= (const vec4& v) {
	data[0] -= v.data[0];
	data[1] -= v.data[1];
	data[2] -= v.data[2];
	data[3] -= v.data[3];
		
	return *this;
}

inline vec4& vec4::operator*= (const vec4& v) {
	data[0] *= v[0];
	data[1] *= v[1];
	data[2] *= v[2];
	data[3] *= v[3];
		
	return *this;
}

inline vec4& vec4::operator/= (const vec4& v) {
	data[0] /= v[0];
	data[1] /= v[1];
	data[2] /= v[2];
	data[3] /= v[3];
		
	return *this;
}

/******************************************************************************/
// scalar operators for speed and resolving ambiguity problems
// with implicit constructor calls

/// Componentwise addition 
inline vec4& vec4::operator+= (Real f) {
	data[0] += f;
	data[1] += f;
	data[2] += f;
	data[3] += f;

	return *this;
}

/// Componentwise subtraction
inline vec4& vec4::operator-= (Real f) {
	data[0] -= f;
	data[1] -= f;
	data[2] -= f;
	data[3] -= f;
	
	return *this;
}

/// Componentwise multiplication
inline vec4& vec4::operator*= (Real f) {
	data[0] *= f;
	data[1] *= f;
	data[2] *= f;
	data[3] *= f;
		
	return *this;
}

/// Componentwise division
inline vec4& vec4::operator/= (Real f) {
	Real rf = 1.0f / f;
	
	data[0] *= rf;
	data[1] *= rf;
	data[2] *= rf;
	data[3] *= rf;
		
	return *this;
}

/******************************************************************************/
// Access to individual vector elements

inline Real& vec4::operator[] (int i) {
	return data[i];
}

inline Real vec4::operator[] (int i) const {
	return data[i];
}

/******************************************************************************/
// non-member operators

/// @relates vec4
/// Vector addition
inline vec4 operator+ (const vec4& a, const vec4& b) {
	vec4 r = a;
	return r += b;
}

/// @relates vec4
/// Vector subtraction
inline vec4 operator- (const vec4& a, const vec4& b) {
	vec4 r = a;
	return r -= b;
}

/// @relates vec4
/// Unary minus
inline vec4 operator- (const vec4& v) {
	return vec4(
		-v[0],
		-v[1],
		-v[2],
		-v[3]
	);
}

/// @relates vec4
/// Vector componentwise multiplication
inline vec4 operator* (const vec4& a, const vec4& b) {
	vec4 r = a;
	return r *= b;
}

/// @relates vec4
/// Vector componentwise division
inline vec4 operator/ (const vec4& a, const vec4& b) {
	vec4 r = a;
	return r /= b;
}

/// @relates vec4
/// Vector comparison
inline bool operator== (const vec4& a, const vec4& b) {
	for ( int i = 0; i < 3; ++i ) {
	#ifndef MATH3D_USE_FEQUAL_COMPARE
		if ( a[i] != b[i] ) return false;
	#else
		if ( !fequal(a[i], b[i]) ) return false;
	#endif
	}
	
	return true;
}

/// @relates vec4
/// Vector comparison
inline bool operator!= (const vec4& a, const vec4& b) {
	return !operator==(a,b);
}

/******************************************************************************/
// non-member scalar operators for speed and resolving ambiguity problems
// with implicit constructor calls

/// Multiply each matrix component with a scalar
inline vec4 operator* (const vec4& m, Real f) {
	vec4 r = m;
	return r *= f;
}

/// Multiply each matrix component with a scalar
inline vec4 operator* (Real f, const vec4& m) {
	return operator*(m, f);
}

/// Divide each matrix component by a scalar
inline vec4 operator/ (const vec4& m, Real f) {
	vec4 r = m;
	return r /= f;
}

/******************************************************************************/
// additional vector related functions

/// @relates vec4
/// Vector dot product
inline Real dot(const vec4& a, const vec4&b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

/// @relates vec4
/// Returns length^2
inline Real length2(const vec4& v) {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
}

/// @relates vec4
/// Returns the length of the vector
inline Real length(const vec4& v) {
	return std::sqrt(length2(v));
}

/// @relates vec4
/// Scales the vector passed as argument to unit length and return a reference
inline vec4& normalize(vec4& v) {
	Real l = length(v);
	
	if ( l != 0.0 ) {
		Real rl = 1.0f / l;
		v[0] *= rl;
		v[1] *= rl;
		v[2] *= rl;
		v[3] *= rl;
	}
	
	return v;
}

/// @relates vec4
/// Returns the vector scaled to unit length
inline vec4 normalized(const vec4& v)  {
	vec4 r = v;
	return normalize(r);
}

#endif
