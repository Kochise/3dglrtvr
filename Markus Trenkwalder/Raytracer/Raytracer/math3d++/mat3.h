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

#ifndef MATH3DPP_MAT3_H
#define MATH3DPP_MAT3_H

#include "mat_decl.h"

/******************************************************************************/
// constructors

/// Initialize diagonal to v (default = 1) with all other elements set to zero;
inline mat3::mat3(Real v) {
	data[0][0] =    v; data[0][1] = 0.0f; data[0][2] = 0.0f;
	data[1][0] = 0.0f; data[1][1] =    v; data[1][2] = 0.0f;
	data[2][0] = 0.0f; data[2][1] = 0.0f; data[2][2] = v;
}

inline mat3::mat3(
	Real a11, Real a12, Real a13,
	Real a21, Real a22, Real a23,
	Real a31, Real a32, Real a33)
{
	data[0][0] = a11; data[0][1] = a12; data[0][2] = a13;
	data[1][0] = a21; data[1][1] = a22; data[1][2] = a23;
	data[2][0] = a31; data[2][1] = a32; data[2][2] = a33;
}

/// Take the upper left 3x3 matrix
inline mat3::mat3(const mat4& m) {
	data[0][0] = m[0][0]; data[0][1] = m[0][1]; data[0][2] = m[0][2];
	data[1][0] = m[1][0]; data[1][1] = m[1][1]; data[1][2] = m[1][2];
	data[2][0] = m[2][0]; data[2][1] = m[2][1]; data[2][2] = m[2][2];
}

/// Take the 3x3 identity matrix and copy the elements from the 2x2 matrix
/// to the upper left.
inline mat3::mat3(const mat2& m) {
	data[0][0] = m[0][0]; data[0][1] = m[0][1]; data[0][2] = 0;
	data[1][0] = m[1][0]; data[1][1] = m[1][1]; data[1][2] = 0;
	data[2][0] = 0; data[2][1] = 0; data[2][2] = 1;
}


/******************************************************************************/
// arithmetic operators

inline mat3& mat3::operator+= (const mat3& m) {
	data[0][0] += m.data[0][0]; data[0][1] += m.data[0][1]; data[0][2] += m.data[0][2];
	data[1][0] += m.data[1][0]; data[1][1] += m.data[1][1]; data[1][2] += m.data[1][2];
	data[2][0] += m.data[2][0]; data[2][1] += m.data[2][1]; data[2][2] += m.data[2][2];
	
	return *this;
}

inline mat3& mat3::operator-= (const mat3& m) {
	data[0][0] -= m.data[0][0]; data[0][1] -= m.data[0][1]; data[0][2] -= m.data[0][2];
	data[1][0] -= m.data[1][0]; data[1][1] -= m.data[1][1]; data[1][2] -= m.data[1][2];
	data[2][0] -= m.data[2][0]; data[2][1] -= m.data[2][1]; data[2][2] -= m.data[2][2];
	
	return *this;
}

/// Matrix multiplication (NOT componentwise)
inline mat3& mat3::operator*= (const mat3& m) {
	*this = mat3(
		data[0][0] * m[0][0] + data[0][1] * m[1][0] + data[0][2] * m[2][0],
		data[0][0] * m[0][1] + data[0][1] * m[1][1] + data[0][2] * m[2][1],
		data[0][0] * m[0][2] + data[0][1] * m[1][2] + data[0][2] * m[2][2],
	
		data[1][0] * m[0][0] + data[1][1] * m[1][0] + data[1][2] * m[2][0],
		data[1][0] * m[0][1] + data[1][1] * m[1][1] + data[1][2] * m[2][1],
		data[1][0] * m[0][2] + data[1][1] * m[1][2] + data[1][2] * m[2][2],
	
		data[2][0] * m[0][0] + data[2][1] * m[1][0] + data[2][2] * m[2][0],
		data[2][0] * m[0][1] + data[2][1] * m[1][1] + data[2][2] * m[2][1],
		data[2][0] * m[0][2] + data[2][1] * m[1][2] + data[2][2] * m[2][2]
	);
	
	return *this;
}

/// Componentwise division
inline mat3& mat3::operator/= (const mat3& m) {
	data[0][0] /= m.data[0][0]; data[0][1] /= m.data[0][1]; data[0][2] /= m.data[0][2];
	data[1][0] /= m.data[1][0]; data[1][1] /= m.data[1][1]; data[1][2] /= m.data[1][2];
	data[2][0] /= m.data[2][0]; data[2][1] /= m.data[2][1]; data[2][2] /= m.data[2][2];
	
	return *this;
}

/// Componentwise matrix multiplication
inline mat3& mat3::compMult(const mat3& m) {
	data[0][0] *= m.data[0][0]; data[0][1] *= m.data[0][1]; data[0][2] *= m.data[0][2];
	data[1][0] *= m.data[1][0]; data[1][1] *= m.data[1][1]; data[1][2] *= m.data[1][2];
	data[2][0] *= m.data[2][0]; data[2][1] *= m.data[2][1]; data[2][2] *= m.data[2][2];
	
	return *this;
}

/******************************************************************************/
// scalar operators for speed and resolving ambiguity problems
// with implicit constructor calls

/// Componentwise addition 
inline mat3& mat3::operator+= (Real f) {
	data[0][0] += f; data[0][1] += f; data[0][2] += f;
	data[1][0] += f; data[1][1] += f; data[1][2] += f;
	data[2][0] += f; data[2][1] += f; data[2][2] += f;
		
	return *this;
}

/// Componentwise subtraction
inline mat3& mat3::operator-= (Real f) {
	data[0][0] -= f; data[0][1] -= f; data[0][2] -= f;
	data[1][0] -= f; data[1][1] -= f; data[1][2] -= f;
	data[2][0] -= f; data[2][1] -= f; data[2][2] -= f;
		
	return *this;
}

/// Componentwise multiplication
inline mat3& mat3::operator*= (Real f) {
	data[0][0] *= f; data[0][1] *= f; data[0][2] *= f;
	data[1][0] *= f; data[1][1] *= f; data[1][2] *= f;
	data[2][0] *= f; data[2][1] *= f; data[2][2] *= f;
		
	return *this;
}

/// Componentwise division
inline mat3& mat3::operator/= (Real f) {
	Real rf = 1.0f / f;
	data[0][0] *= rf; data[0][1] *= rf; data[0][2] *= rf;
	data[1][0] *= rf; data[1][1] *= rf; data[1][2] *= rf;
	data[2][0] *= rf; data[2][1] *= rf; data[2][2] *= rf;
		
	return *this;
}

/******************************************************************************/
// Access to individual maxtix elements

/// Subscripting operator.
/// You can write code like m[0][0] = value
inline Real* mat3::operator[] (int i) {
	return data[i];
}

/// Subscripting operator.
/// You can write code like value = m[0][0]
inline const Real* mat3::operator[] (int i) const {
	return data[i];
}

/******************************************************************************/
// non-member operators

/// @relates mat3
/// Matrix addition
inline mat3 operator+ (const mat3& a, const mat3& b) {
	mat3 r = a;
	return r += b;
}

/// @relates mat3
/// Matrix subtraction
inline mat3 operator- (const mat3& a, const mat3& b) {
	mat3 r = a;
	return r -= b;
}

/// @relates mat3
/// Unary minus
inline mat3 operator- (const mat3& a) {
	return mat3(
		-a[0][0], -a[0][1], -a[0][2],
		-a[1][0], -a[1][1], -a[1][2],
		-a[2][0], -a[2][1], -a[2][2]
	);
}

/// @relates mat3
/// Matrix multiplication
inline mat3 operator* (const mat3& a, const mat3& b) {
	mat3 r = a;
	return r *= b;
}

/// @relates mat3
/// Componentwise matrix multiplication
inline mat3 matrixCompMult(const mat3& a, const mat3& b) {
	mat3 r = a;
	return r.compMult(b);
}


/// @relates mat3
/// Matrix componentwise division
inline mat3 operator/ (const mat3& a, const mat3& b) {
	mat3 r = a;
	return r /= b;
}

/// @relates mat3
/// Matrix comparison
inline bool operator== (const mat3& a, const mat3& b) {

	for ( int j = 0; j < 3; ++j )
	for ( int k = 0; k < 3; ++k )
	{
	#ifndef MATH3D_USE_FEQUAL_COMPARE	
		if ( a[j][k] != b[j][k] ) return false;
	#else
		if ( !fequal(a[j][k], b[j][k] ) ) return false;
	#endif
	}
	
	return true;
}

/// @relates mat3
/// Matrix comparison
inline bool operator!= (const mat3& a, const mat3& b) {
	return !operator==(a, b);
}

/******************************************************************************/
// non-member scalar operators for speed and resolving ambiguity problems
// with implicit constructor calls

/// Multiply each matrix component with a scalar
inline mat3 operator* (const mat3& m, Real f) {
	mat3 r = m;
	return r *= f;
}

/// Multiply each matrix component with a scalar
inline mat3 operator* (Real f, const mat3& m) {
	return operator*(m, f);
}

/// Divide each matrix component by a scalar
inline mat3 operator/ (const mat3& m, Real f) {
	mat3 r = m;
	return r /= f;
}


/******************************************************************************/
// additional matrix related functions

/// @relates mat3
/// Returns the transpose of the matrix passed as argument
inline mat3 transpose(const mat3& a) {
	return mat3(
		a[0][0], a[1][0], a[2][0], 
		a[0][1], a[1][1], a[2][1], 
		a[0][2], a[1][2], a[2][2]
	);	
}

/// @relates mat3
/// Transposes the passed matrix and returns a reference to it
inline mat3& makeTranspose(mat3& a) {
	for ( int j = 0; j < 3; ++j )
	for ( int k = 0; k < j; ++k ) {
		Real tmp = a[j][k];
		a[j][k] = a[k][j];
		a[k][j] = tmp;
	}
	
	return a;
}

/// @relates mat3
/// Returns the 3x3 identity matrix
inline mat3 identity3() {
	return mat3(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	);
}

/// @relates mat3
/// Make the passed matrix the identity matrix and returns a reference
inline mat3& makeIdentity(mat3& a) {
	a[0][0] = 1.0f; a[0][1] = 0.0f; a[0][2] = 0.0f;
	a[1][0] = 0.0f; a[1][1] = 1.0f; a[1][2] = 0.0f;
	a[2][0] = 0.0f; a[2][1] = 0.0f; a[2][2] = 1.0f;
		
	return a;
}

/// @relates mat3
/// Returns the matrix determinant
inline Real det(const mat3& a) {
	return (
		a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) +
		a[1][0] * (a[0][2] * a[2][1] - a[0][1] * a[2][2]) +
		a[2][0] * (a[0][1] * a[1][2] - a[0][2] * a[1][1])
	);
}

/// @relates mat3
/// Computes the trace of the matrix
inline Real trace(const mat3& m ) {
	return m[0][0] + m[1][1] + m[2][2];
}

/// @relates mat3
/// Return a minor of the supplied 3x3 matrix by omitting the _jth row
/// and _kth column.
/// It defaults to return the upper left 2x2 matrix
inline mat2 minor(const mat3& m, int _j = 2, int _k = 2) {
	mat2 r(math3d_noInit);
	
	int jp = (0 >= _j) ? 1 : 0;
	int kp = (0 >= _k) ? 1 : 0;
	r[0][0] = m[jp][kp];
	
	kp = (1 >= _k) ? 1 : 0;
	r[0][1] = m[jp][1 + kp];
	
	jp = (1 >= _j) ? 1 : 0;
	kp = (0 >= _k) ? 1 : 0;
	r[1][0] = m[1 + jp][kp];
	
	kp = (1 >= _k) ? 1 : 0;
	r[1][1] = m[1 + jp][1 + kp];
	
	return r;
}

/// @relates mat3
/// Returns the adjoint of the supplied matrix.
inline mat3 adjoint(const mat3& m) {
	return mat3(
		 det(minor(m, 0, 0)),	-det(minor(m, 1, 0)),  det(minor(m, 2, 0)),
		-det(minor(m, 0, 1)),  det(minor(m, 1, 1)), -det(minor(m, 2, 1)),
		 det(minor(m, 0, 2)), -det(minor(m, 1, 2)),  det(minor(m, 2, 2))
	);
}

/// @relates mat3
/// Returns the inverse of the matrix. By default the parameter detA is set to
/// zero and therefore the function will calculate the needed matrix determinant
/// on its own.
/// If you already know the matrix determinant you can pass its value in the
/// parameter detA so that the function doesn't have to calculate it.
/// If the matrix is singular it is impossible to calculate the inverse matrix
/// and the identity matrix is returned.
inline mat3 inverse(const mat3& a, Real detA = 0) {

	// only calculate the determinant if it has not been passed as 
	// argument
	if ( detA == 0 ) {
		detA = det(a);
	}
	
	if ( !fequal(detA, Real(0)) ) {
		mat3 r(
			(a[1][1] * a[2][2] - a[1][2] * a[2][1]), (a[0][2] * a[2][1] - a[0][1] * a[2][2]), (a[0][1] * a[1][2] - a[0][2] * a[1][1]),
			(a[1][2] * a[2][0] - a[1][0] * a[2][2]), (a[0][0] * a[2][2] - a[0][2] * a[2][0]), (a[0][2] * a[1][0] - a[0][0] * a[1][2]),
			(a[1][0] * a[2][1] - a[1][1] * a[2][0]), (a[0][1] * a[2][0] - a[0][0] * a[2][1]), (a[0][0] * a[1][1] - a[0][1] * a[1][0])
		);
		
		r /= detA;
		
		return r;
	}
	
	return identity3();
}

#endif
