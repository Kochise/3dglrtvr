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

#ifndef MATH3DPP_VEC2_H
#define MATH3DPP_VEC2_H

#include "vec_decl.h"

/******************************************************************************/
// constructors

/// Initialization of all components to v (default = 0)
inline vec2::vec2(Real v) {
	data[0] = v;
	data[1] = v;
}

/// initialization to (x,y)
inline vec2::vec2(Real x, Real y) {
	data[0] = x;
	data[1] = y;
}

/// Initialization from a vec3 by dropping the third component
inline vec2::vec2(const vec3& v) {
	data[0] = v[0];
	data[1] = v[1];
}

/******************************************************************************/
// Vector arithmetic operators

inline vec2& vec2::operator+= (const vec2& v) {
	data[0] += v.data[0];
	data[1] += v.data[1];
		
	return *this;
}

inline vec2& vec2::operator-= (const vec2& v) {
	data[0] -= v.data[0];
	data[1] -= v.data[1];
		
	return *this;
}

inline vec2& vec2::operator*= (const vec2& v) {
	data[0] *= v[0];
	data[1] *= v[1];
		
	return *this;
}

inline vec2& vec2::operator/= (const vec2& v) {
	data[0] /= v[0];
	data[1] /= v[1];
		
	return *this;
}

/******************************************************************************/
// scalar operators for speed and resolving ambiguity problems
// with implicit constructor calls

/// Componentwise addition 
inline vec2& vec2::operator+= (Real f) {
	data[0] += f;
	data[1] += f;

	return *this;
}

/// Componentwise subtraction
inline vec2& vec2::operator-= (Real f) {
	data[0] -= f;
	data[1] -= f;
	
	return *this;
}

/// Componentwise multiplication
inline vec2& vec2::operator*= (Real f) {
	data[0] *= f;
	data[1] *= f;
		
	return *this;
}

/// Componentwise division
inline vec2& vec2::operator/= (Real f) {
	Real rf =  1.0f / f;
	data[0] *= rf;
	data[1] *= rf;
		
	return *this;
}

/******************************************************************************/
// Access to individual vector elements

inline Real& vec2::operator[] (int i) {
	return data[i];
}

inline Real vec2::operator[] (int i) const {
	return data[i];
}

/******************************************************************************/
// non-member operators

/// @relates vec2
/// Vector addition
inline vec2 operator+ (const vec2& a, const vec2& b) {
	vec2 r = a;
	return r += b;
}

/// @relates vec2
/// Vector subtraction
inline vec2 operator- (const vec2& a, const vec2& b) {
	vec2 r = a;
	return r -= b;
}

/// @relates vec2
/// Unary minus
inline vec2 operator- (const vec2& v) {
	return vec2(
		-v[0],
		-v[1]
	);
}

/// @relates vec2
/// Vector componentwise multiplication
inline vec2 operator* (const vec2& a, const vec2& b) {
	vec2 r = a;
	return r *= b;
}

/// @relates vec2
/// Vector componentwise division
inline vec2 operator/ (const vec2& a, const vec2& b) {
	vec2 r = a;
	return r /= b;
}

/// @relates vec2
/// Comparison operators
inline bool operator== (const vec2& a, const vec2& b) {
	for ( int i = 0; i < 2; ++i ) {
	#ifndef MATH3D_USE_FEQUAL_COMPARE
		if ( a[i] != b[i] ) return false;
	#else
		if ( !fequal(a[i], b[i]) ) return false;
	#endif
	}
	
	return true;
}

/// @relates vec2
/// Vector comparison
inline bool operator!= (const vec2& a, const vec2& b) {
	return !operator==(a,b);
}

/******************************************************************************/
// non-member scalar operators for speed and resolving ambiguity problems
// with implicit constructor calls

/// Multiply each matrix component with a scalar
inline vec2 operator* (const vec2& m, Real f) {
	vec2 r = m;
	return r *= f;
}

/// Multiply each matrix component with a scalar
inline vec2 operator* (Real f, const vec2& m) {
	return operator*(m, f);
}

/// Divide each matrix component by a scalar
inline vec2 operator/ (const vec2& m, Real f) {
	vec2 r = m;
	return r /= f;
}

/******************************************************************************/
// additional vector related functions

/// @relates vec2
/// Vector dot product
inline Real dot(const vec2& a, const vec2& b) {
	return a[0] * b[0] + a[1] * b[1];
}

/// @relates vec2
/// Returns the length^2
inline Real length2(const vec2& v) {
	return v[0] * v[0] + v[1] * v[1];
}

/// @relates vec2
/// Returns the vector length
inline Real length(const vec2& v) {
	return std::sqrt(length2(v));
}

/// @relates vec2
/// Scales the passed vector to unit length and returns a reference to it
inline vec2& normalize(vec2& v) {
	Real l = length(v);
	
	if ( l != 0.0 ) {
		Real rl = 1.0f / l;
		v[0] *= rl;
		v[1] *= rl;
	}
	
	return v;
}

/// @relates vec2
/// Returns the vector scaled to unit length
inline vec2 normalized(const vec2& v)  {
	vec2 r = v;
	return normalize(r);
}


#endif
