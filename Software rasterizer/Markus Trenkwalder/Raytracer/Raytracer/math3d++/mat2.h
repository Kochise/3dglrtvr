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

#ifndef MATH3DPP_MAT2_H
#define MATH3DPP_MAT2_H

#include "mat_decl.h"

/******************************************************************************/
// constructors

/// Initialize diagonal to v (default = 1) with all other elements set to zero;
inline mat2::mat2(Real v) {
	data[0][0] =    v; data[0][1] = 0.0f;
	data[1][0] = 0.0f; data[1][1] =   v;
}

inline mat2::mat2(
	Real a11, Real a12,
	Real a21, Real a22)
{
	data[0][0] = a11; data[0][1] = a12; 
	data[1][0] = a21; data[1][1] = a22; 
}

/// Take the upper left 2x2 matrix
inline mat2::mat2(const mat3& m) {
	data[0][0] = m[0][0]; data[0][1] = m[0][1]; 
	data[1][0] = m[1][0]; data[1][1] = m[1][1]; 
}

/******************************************************************************/
// arithmetic operators

inline mat2& mat2::operator+= (const mat2& m) {
	data[0][0] += m.data[0][0]; data[0][1] += m.data[0][1];
	data[1][0] += m.data[1][0];	data[1][1] += m.data[1][1];
	
	return *this;
}

inline mat2& mat2::operator-= (const mat2& m) {
	data[0][0] -= m.data[0][0];	data[0][1] -= m.data[0][1];
	data[1][0] -= m.data[1][0];	data[1][1] -= m.data[1][1];
	
	return *this;
}

/// Matrix multiplication (NOT componentwise)
inline mat2& mat2::operator*= (const mat2& m) {
	*this = mat2(
		data[0][0] * m[0][0] + data[0][1] * m[1][0], data[0][0] * m[0][1] + data[0][1] * m[1][1],
		data[1][0] * m[0][0] + data[1][1] * m[1][0], data[1][0] * m[0][1] + data[1][1] * m[1][1]
	);
	
	return *this;
}

/// Componentwise division
inline mat2& mat2::operator/= (const mat2& m) {
	data[0][0] /= m.data[0][0];	data[0][1] /= m.data[0][1];
	data[1][0] /= m.data[1][0]; data[1][1] /= m.data[1][1];
	
	return *this;
}

/// Componentwise matrix multiplication
inline mat2& mat2::compMult(const mat2& m) {
	data[0][0] *= m[0][0]; data[0][1] *= m[0][1];
	data[1][0] *= m[1][0]; data[1][1] *= m[1][1];
	
	return *this;
}

/******************************************************************************/
// scalar operators for speed and resolving ambiguity problems
// with implicit constructor calls

/// Componentwise addition 
inline mat2& mat2::operator+= (Real f) {
	data[0][0] += f; data[0][1] += f;
	data[1][0] += f; data[1][1] += f;

	return *this;
}

/// Componentwise subtraction
inline mat2& mat2::operator-= (Real f) {
	data[0][0] -= f; data[0][1] -= f;
	data[1][0] -= f; data[1][1] -= f;
	
	return *this;
}

/// Componentwise multiplication
inline mat2& mat2::operator*= (Real f) {
	data[0][0] *= f; data[0][1] *= f;
	data[1][0] *= f; data[1][1] *= f;
		
	return *this;
}

/// Componentwise division
inline mat2& mat2::operator/= (Real f) {
	data[0][0] /= f; data[0][1] /= f;
	data[1][0] /= f; data[1][1] /= f;
		
	return *this;
}

/******************************************************************************/
// Access to individual maxtix elements

/// Subscripting operator.
/// You can write code like m[0][0] = value
inline Real* mat2::operator[] (int i) {
	return data[i];
}

/// Subscripting operator.
/// You can write code like value = m[0][0]
inline const Real* mat2::operator[] (int i) const {
	return data[i];
}

/******************************************************************************/
// non-member operators

/// @relates mat2
/// Matrix addition
inline mat2 operator+ (const mat2& a, const mat2& b) {
	mat2 r = a;
	return r += b;
}

/// @relates mat2
/// Matrix subtraction
inline mat2 operator- (const mat2& a, const mat2& b) {
	mat2 r = a;
	return r -= b;
}

/// @relates mat2
/// Unary minus
inline mat2 operator- (const mat2& a) {
	return mat2(
		-a[0][0], -a[0][1],
		-a[1][0], -a[1][1]
	);
}

/// @relates mat2
/// Matrix multiplication
inline mat2 operator* (const mat2& a, const mat2& b) {
	mat2 r = a;
	return r *= b;
}

/// @relates mat2
/// Componentwise matrix multiplication
inline mat2 matrixCompMult(const mat2& a, const mat2& b) {
	mat2 r = a;
	return r.compMult(b);
}

/// @relates mat2
/// Matrix componentwise division
inline mat2 operator/ (const mat2& a, const mat2& b) {
	mat2 r = a;
	return r /= b;
}

/// @relates mat2
/// Matrix comparison
inline bool operator== (const mat2& a, const mat2& b) {

	for ( int j = 0; j < 2; ++j )
	for ( int k = 0; k < 2; ++k )
	{
	#ifndef MATH3D_USE_FEQUAL_COMPARE	
		if ( a[j][k] != b[j][k] ) return false;
	#else
		if ( !fequal(a[j][k], b[j][k] ) ) return false;
	#endif
	}
	
	return true;
}

/// @relates mat2
/// Matrix comparison
inline bool operator!= (const mat2& a, const mat2& b) {
	return !operator==(a, b);
}

/******************************************************************************/
// non-member scalar operators for speed and resolving ambiguity problems
// with implicit constructor calls

/// Multiply each matrix component with a scalar
inline mat2 operator* (const mat2& m, Real f) {
	mat2 r = m;
	return r *= f;
}

/// Multiply each matrix component with a scalar
inline mat2 operator* (Real f, const mat2& m) {
	return operator*(m, f);
}

/// Divide each matrix component by a scalar
inline mat2 operator/ (const mat2& m, Real f) {
	mat2 r = m;
	return r /= f;
}

/******************************************************************************/
// additional matrix related functions

/// @relates mat2
/// Returns the transpose of the matrix passed as argument
inline mat2 transpose(const mat2& a) {
	return mat2(
		a[0][0], a[1][0],
		a[0][1], a[1][1]
	);	
}

/// @relates mat2
/// Transposes the passed matrix and returns a reference to it
inline mat2& makeTranspose(mat2& a) {
	for ( int j = 0; j < 2; ++j )
	for ( int k = 0; k < j; ++k ) {
		Real tmp = a[j][k];
		a[j][k] = a[k][j];
		a[k][j] = tmp;
	}
	
	return a;
}

/// @relates mat2
/// Returns the 2x2 identity matrix
inline mat2 identity2() {
	return mat2(
		1, 0,
		0, 1
	);
}

/// @relates mat2
/// Make the passed matrix the identity matrix and returns a reference
inline mat2& makeIdentity(mat2& a) {
	a[0][0] = 1.0f; a[0][1] = 0.0f;
	a[1][0] = 0.0f; a[1][1] = 1.0f;
		
	return a;
}

/// @relates mat2
/// Returns the matrix determinant
inline Real det(const mat2& a) {
	return a[0][0] * a[1][1] - a[0][1] * a[1][0];
}

/// @relates mat2
/// Computes the trace of the matrix
inline Real trace(const mat2& m ) {
	return m[0][0] + m[1][1];
}

/// @relates mat2
/// Returns the adjoint of the supplied matrix.
inline mat2 adjoint(const mat2& m) {
	return mat2(
		m[1][1], -m[0][1],
		-m[1][0], m[0][0]
	);
}

/// @relates mat2
/// Returns the inverse of the matrix. By default the parameter detA is set to
/// zero and therefore the function will calculate the needed matrix determinant
/// on its own.
/// If you already know the matrix determinant you can pass its value in the
/// parameter detA so that the function doesn't have to calculate it.
/// If the matrix is singular it is impossible to calculate the inverse matrix
/// and the identity matrix is returned.
inline mat2 inverse(const mat2& a, Real detA = 0) {

	// only calculate the determinant if it has not been passed as 
	// argument
	if ( detA == 0 ) {
		detA = det(a);
	}
	
	if ( !fequal(detA, Real(0)) ) {
		mat2 r(
			 a[1][1], -a[0][1],
			-a[1][0],  a[0][0]
		);
		
		r /= detA;
		
		return r;
	}
	
	return identity2();
}

#endif
