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

#ifndef MATH3DPP_MAT4_H
#define MATH3DPP_MAT4_H

#include "mat_decl.h"

/******************************************************************************/
// constructors

/// Initialize diagonal to v (default = 1) with all other elements set to zero;
inline mat4::mat4(Real v) {
	data[0][0] =    v; data[0][1] = 0.0f; data[0][2] = 0.0f; data[0][3] = 0.0f;
	data[1][0] = 0.0f; data[1][1] =    v; data[1][2] = 0.0f; data[1][3] = 0.0f;
	data[2][0] = 0.0f; data[2][1] = 0.0f; data[2][2] =    v; data[2][3] = 0.0f;
	data[3][0] = 0.0f; data[3][1] = 0.0f; data[3][2] = 0.0f; data[3][3] =    v;
}


inline mat4::mat4(
	Real a11, Real a12, Real a13, Real a14, 
	Real a21, Real a22, Real a23, Real a24, 
	Real a31, Real a32, Real a33, Real a34,
	Real a41, Real a42, Real a43, Real a44)
{
	data[0][0] = a11; data[0][1] = a12; data[0][2] = a13; data[0][3] = a14;
	data[1][0] = a21; data[1][1] = a22; data[1][2] = a23; data[1][3] = a24;
	data[2][0] = a31; data[2][1] = a32; data[2][2] = a33; data[2][3] = a34;
	data[3][0] = a41; data[3][1] = a42; data[3][2] = a43; data[3][3] = a44;
}

/// Take the 4x4 identity matrix and copy the elements from the 3x3 matrix
/// to the upper left.
inline mat4::mat4(const mat3& m) {
	data[0][0] = m[0][0]; data[0][1] = m[0][1]; data[0][2] = m[0][2]; data[0][3] = 0;
	data[1][0] = m[1][0]; data[1][1] = m[1][1]; data[1][2] = m[1][2]; data[1][3] = 0;
	data[2][0] = m[2][0]; data[2][1] = m[2][1]; data[2][2] = m[2][2]; data[2][3] = 0;
	data[3][0] = 0; data[3][1] = 0; data[3][2] = 0; data[3][3] = 1;
}


/******************************************************************************/
// arithmetic operators

inline mat4& mat4::operator+= (const mat4& m) {
	data[0][0] += m.data[0][0]; data[0][1] += m.data[0][1]; data[0][2] += m.data[0][2]; data[0][3] += m.data[0][3];
	data[1][0] += m.data[1][0]; data[1][1] += m.data[1][1]; data[1][2] += m.data[1][2]; data[1][3] += m.data[1][3];
	data[2][0] += m.data[2][0]; data[2][1] += m.data[2][1]; data[2][2] += m.data[2][2]; data[2][3] += m.data[2][3];
	data[3][0] += m.data[3][0]; data[3][1] += m.data[3][1]; data[3][2] += m.data[3][2]; data[3][3] += m.data[3][3];
	
	return *this;
}

inline mat4& mat4::operator-= (const mat4& m) {
	data[0][0] -= m.data[0][0]; data[0][1] -= m.data[0][1]; data[0][2] -= m.data[0][2]; data[0][3] -= m.data[0][3];
	data[1][0] -= m.data[1][0]; data[1][1] -= m.data[1][1]; data[1][2] -= m.data[1][2]; data[1][3] -= m.data[1][3];
	data[2][0] -= m.data[2][0]; data[2][1] -= m.data[2][1]; data[2][2] -= m.data[2][2]; data[2][3] -= m.data[2][3];
	data[3][0] -= m.data[3][0]; data[3][1] -= m.data[3][1]; data[3][2] -= m.data[3][2]; data[3][3] -= m.data[3][3];
		
	return *this;
}


/// Matrix multiplication (NOT componentwise)
inline mat4& mat4::operator*= (const mat4& m) {
	*this = mat4(
		data[0][0] * m[0][0] + data[0][1] * m[1][0] + data[0][2] * m[2][0] + data[0][3] * m[3][0],
		data[0][0] * m[0][1] + data[0][1] * m[1][1] + data[0][2] * m[2][1] + data[0][3] * m[3][1],
		data[0][0] * m[0][2] + data[0][1] * m[1][2] + data[0][2] * m[2][2] + data[0][3] * m[3][2],
		data[0][0] * m[0][3] + data[0][1] * m[1][3] + data[0][2] * m[2][3] + data[0][3] * m[3][3],
	
		data[1][0] * m[0][0] + data[1][1] * m[1][0] + data[1][2] * m[2][0] + data[1][3] * m[3][0],
		data[1][0] * m[0][1] + data[1][1] * m[1][1] + data[1][2] * m[2][1] + data[1][3] * m[3][1],
		data[1][0] * m[0][2] + data[1][1] * m[1][2] + data[1][2] * m[2][2] + data[1][3] * m[3][2],
		data[1][0] * m[0][3] + data[1][1] * m[1][3] + data[1][2] * m[2][3] + data[1][3] * m[3][3],

		data[2][0] * m[0][0] + data[2][1] * m[1][0] + data[2][2] * m[2][0] + data[2][3] * m[3][0],	
		data[2][0] * m[0][1] + data[2][1] * m[1][1] + data[2][2] * m[2][1] + data[2][3] * m[3][1],
		data[2][0] * m[0][2] + data[2][1] * m[1][2] + data[2][2] * m[2][2] + data[2][3] * m[3][2],
		data[2][0] * m[0][3] + data[2][1] * m[1][3] + data[2][2] * m[2][3] + data[2][3] * m[3][3],
	
		data[3][0] * m[0][0] + data[3][1] * m[1][0] + data[3][2] * m[2][0] + data[3][3] * m[3][0],
		data[3][0] * m[0][1] + data[3][1] * m[1][1] + data[3][2] * m[2][1] + data[3][3] * m[3][1],
		data[3][0] * m[0][2] + data[3][1] * m[1][2] + data[3][2] * m[2][2] + data[3][3] * m[3][2],
		data[3][0] * m[0][3] + data[3][1] * m[1][3] + data[3][2] * m[2][3] + data[3][3] * m[3][3]
	);
	
	return *this;
}

/// Componentwise division
inline mat4& mat4::operator/= (const mat4& m) {
	data[0][0] /= m.data[0][0]; data[0][1] /= m.data[0][1]; data[0][2] /= m.data[0][2]; data[0][3] /= m.data[0][3];
	data[1][0] /= m.data[1][0]; data[1][1] /= m.data[1][1]; data[1][2] /= m.data[1][2]; data[1][3] /= m.data[1][3];
	data[2][0] /= m.data[2][0]; data[2][1] /= m.data[2][1]; data[2][2] /= m.data[2][2]; data[2][3] /= m.data[2][3];
	data[3][0] /= m.data[3][0]; data[3][1] /= m.data[3][1]; data[3][2] /= m.data[3][2]; data[3][3] /= m.data[3][3];
		
	return *this;
}

/// Componentwise matrix multiplication
inline mat4& mat4::compMult(const mat4& m) {
	data[0][0] *= m.data[0][0]; data[0][1] *= m.data[0][1]; data[0][2] *= m.data[0][2]; data[0][3] *= m.data[0][3];
	data[1][0] *= m.data[1][0]; data[1][1] *= m.data[1][1]; data[1][2] *= m.data[1][2]; data[1][3] *= m.data[1][3];
	data[2][0] *= m.data[2][0]; data[2][1] *= m.data[2][1]; data[2][2] *= m.data[2][2]; data[2][3] *= m.data[2][3];
	data[3][0] *= m.data[3][0]; data[3][1] *= m.data[3][1]; data[3][2] *= m.data[3][2]; data[3][3] *= m.data[3][3];
	
	return *this;
}

/******************************************************************************/
// scalar operators for speed and resolving ambiguity problems
// with implicit constructor calls

/// Componentwise addition 
inline mat4& mat4::operator+= (Real f) {
	data[0][0] += f; data[0][1] += f; data[0][2] += f; data[0][3] += f;
	data[1][0] += f; data[1][1] += f; data[1][2] += f; data[1][3] += f;
	data[2][0] += f; data[2][1] += f; data[2][2] += f; data[2][3] += f;
	data[3][0] += f; data[3][1] += f; data[3][2] += f; data[3][3] += f;
		
	return *this;
}

/// Componentwise subtraction
inline mat4& mat4::operator-= (Real f) {
	data[0][0] -= f; data[0][1] -= f; data[0][2] -= f; data[0][3] -= f;
	data[1][0] -= f; data[1][1] -= f; data[1][2] -= f; data[1][3] -= f;
	data[2][0] -= f; data[2][1] -= f; data[2][2] -= f; data[2][3] -= f;
	data[3][0] -= f; data[3][1] -= f; data[3][2] -= f; data[3][3] -= f;
	
	return *this;
}

/// Componentwise multiplication
inline mat4& mat4::operator*= (Real f) {
	data[0][0] *= f; data[0][1] *= f; data[0][2] *= f; data[0][3] *= f;
	data[1][0] *= f; data[1][1] *= f; data[1][2] *= f; data[1][3] *= f;
	data[2][0] *= f; data[2][1] *= f; data[2][2] *= f; data[2][3] *= f;
	data[3][0] *= f; data[3][1] *= f; data[3][2] *= f; data[3][3] *= f;
		
	return *this;
}

/// Componentwise division
inline mat4& mat4::operator/= (Real f) {
	Real rf = 1.0f / f;
	
	data[0][0] *= rf; data[0][1] *= rf; data[0][2] *= rf; data[0][3] *= rf;
	data[1][0] *= rf; data[1][1] *= rf; data[1][2] *= rf; data[1][3] *= rf;
	data[2][0] *= rf; data[2][1] *= rf; data[2][2] *= rf; data[2][3] *= rf;
	data[3][0] *= rf; data[3][1] *= rf; data[3][2] *= rf; data[3][3] *= rf;
		
	return *this;
}

/******************************************************************************/
// Access to individual maxtix elements

/// Subscripting operator.
/// You can write code like m[0][0] = value
inline Real* mat4::operator[] (int i) {
	return data[i];
}

/// Subscripting operator.
/// You can write code like value = m[0][0]
inline const Real* mat4::operator[] (int i) const {
	return data[i];
}

/******************************************************************************/
// non-member operators

/// @relates mat4
/// Matrix addition
inline mat4 operator+ (const mat4& a, const mat4& b) {
	mat4 r = a;
	return r += b;
}

/// @relates mat4
/// Matrix subtraction
inline mat4 operator- (const mat4& a, const mat4& b) {
	mat4 r = a;
	return r -= b;
}

/// @relates mat4
/// Unary minus
inline mat4 operator- (const mat4& a) {
	return mat4(
		-a[0][0], -a[0][1], -a[0][2],	-a[0][3],
		-a[1][0], -a[1][1], -a[1][2],	-a[1][3],
		-a[2][0], -a[2][1], -a[2][2],	-a[2][3],
		-a[3][0], -a[3][1], -a[3][2],	-a[3][3]
	);
}

/// @relates mat4
/// Matrix multiplication
inline mat4 operator* (const mat4& a, const mat4& b) {
	mat4 r = a;	
	return r *= b;
}

/// @relates mat4
/// Componentwise matrix multiplication
inline mat4 matrixCompMult(const mat4& a, const mat4& b) {
	mat4 r = a;
	return r.compMult(b);
}

/// @relates mat4
/// Matrix componentwise division
inline mat4 operator/ (const mat4& a, const mat4& b) {
	mat4 r = a;
	return r /= b;
}

/// @relates mat4
/// Matrix comparison
inline bool operator== (const mat4& a, const mat4& b) {

	for ( int j = 0; j < 4; ++j )
	for ( int k = 0; k < 4; ++k )
	{
	#ifndef MATH3D_USE_FEQUAL_COMPARE	
		if ( a[j][k] != b[j][k] ) return false;
	#else
		if ( !fequal(a[j][k], b[j][k] ) ) return false;
	#endif
	}
	
	return true;
}

/// @relates mat4
/// Matrix comparison
inline bool operator!= (const mat4& a, const mat4& b) {
	return !operator==(a, b);
}

/******************************************************************************/
// non-member scalar operators for speed and resolving ambiguity problems
// with implicit constructor calls

/// Multiply each matrix component with a scalar
inline mat4 operator* (const mat4& m, Real f) {
	mat4 r = m;
	return r *= f;
}

/// Multiply each matrix component with a scalar
inline mat4 operator* (Real f, const mat4& m) {
	return operator*(m, f);
}

/// Divide each matrix component by a scalar
inline mat4 operator/ (const mat4& m, Real f) {
	mat4 r = m;
	return r /= f;
}

/******************************************************************************/
// additional matrix related functions

/// @relates mat4
/// Returns the transpose of the matrix passed as argument
inline mat4 transpose(const mat4& a) {
	return mat4(
		a[0][0], a[1][0], a[2][0], a[3][0], 
		a[0][1], a[1][1], a[2][1], a[3][1],
		a[0][2], a[1][2], a[2][2], a[3][2],
		a[0][3], a[1][3], a[2][3], a[3][3]
	);	
}

/// @relates mat4
/// Transposes the passed matrix and returns a reference to it
inline mat4& makeTranspose(mat4& a) {
	for ( int j = 0; j < 4; ++j )
	for ( int k = 0; k < j; ++k ) {
		Real tmp = a[j][k];
		a[j][k] = a[k][j];
		a[k][j] = tmp;
	}
	
	return a;
}


/// @relates mat4
/// Returns the 4x4 identity matrix
inline mat4 identity4() {
	return mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0, 
		0, 0, 0, 1
	);
}

/// @relates mat4
/// Make the passed matrix the identity matrix and returns a reference
inline mat4& makeIdentity(mat4& a) {
	a[0][0] = 1.0f; a[0][1] = 0.0f; a[0][2] = 0.0f; a[0][3] = 0.0f;
	a[1][0] = 0.0f; a[1][1] = 1.0f; a[1][2] = 0.0f; a[1][3] = 0.0f;
	a[2][0] = 0.0f; a[2][1] = 0.0f; a[2][2] = 1.0f; a[2][3] = 0.0f;
	a[3][0] = 0.0f; a[3][1] = 0.0f; a[3][2] = 0.0f; a[3][3] = 1.0f;
		
	return a;
}


/// @relates mat4
/// Returns the matrix determinant
inline Real det(const mat4& a) {
	return (
		a[0][0] * (
			a[1][1] * (a[2][2] * a[3][3] - a[2][3] * a[3][2]) +
			a[2][1] * (a[1][3] * a[3][2] - a[1][2] * a[3][3]) +
			a[3][1] * (a[1][2] * a[2][3] - a[1][3] * a[2][2])
		) -
		a[1][0] * (
			a[0][1] * (a[2][2] * a[3][3] - a[2][3] * a[3][2]) +
			a[2][1] * (a[0][3] * a[3][2] - a[0][2] * a[3][3]) +
			a[3][1] * (a[0][2] * a[2][3] - a[0][3] * a[2][2])
		) +
		a[2][0] * (
			a[0][1] * (a[1][2] * a[3][3] - a[1][3] * a[3][2]) +
			a[1][1] * (a[0][3] * a[3][2] - a[0][2] * a[3][3]) +
			a[3][1] * (a[0][2] * a[1][3] - a[0][3] * a[1][2])
		) -
		a[3][0] * (
			a[0][1] * (a[1][2] * a[2][3] - a[1][3] * a[2][2]) +
			a[1][1] * (a[0][3] * a[2][2] - a[0][2] * a[2][3]) +
			a[2][1] * (a[0][2] * a[1][3] - a[0][3] * a[1][2])
		)
	);
}

/// @relates mat4
/// Computes the trace of the matrix
inline Real trace(const mat4& m ) {
	return m[0][0] + m[1][1] + m[2][2] + m[3][3];
}

/// @relates mat4
/// Return a minor of the supplied 4x4 matrix by omitting the _jth row
/// and _kth column.
/// It defaults to return the upper left 3x3 matrix
inline mat3 minor(const mat4& m, int _j = 3, int _k = 3) {
	mat3 r(math3d_noInit);
	
	int jp = (0 >= _j) ? 1 : 0;
	int kp = (0 >= _k) ? 1 : 0;
	r[0][0] = m[jp][kp];
	
	kp = (1 >= _k) ? 1 : 0;
	r[0][1] = m[jp][1 + kp];
	
	kp = (2 >= _k) ? 1 : 0;
	r[0][2] = m[jp][2 + kp];
	
	jp = (1 >= _j) ? 1 : 0;
	kp = (0 >= _k) ? 1 : 0;
	r[1][0] = m[1 + jp][kp];
	
	kp = (1 >= _k) ? 1 : 0;
	r[1][1] = m[1 + jp][1 + kp];
	
	kp = (2 >= _k) ? 1 : 0;
	r[1][2] = m[1 + jp][2 + kp];
	
	jp = (2 >= _j) ? 1 : 0;
	kp = (0 >= _k) ? 1 : 0;
	r[2][0] = m[2 + jp][kp];
	
	kp = (1 >= _k) ? 1 : 0;
	r[2][1] = m[2 + jp][1 + kp];
	
	kp = (2 >= _k) ? 1 : 0;
	r[2][2] = m[2 + jp][2 + kp];
	
	return r;
}

/// @relates mat4
/// Returns the adjoint of the supplied matrix.
inline mat4 adjoint(const mat4& m) {
	return mat4(
		 det(minor(m, 0, 0)), -det(minor(m, 1, 0)),  det(minor(m, 2, 0)), -det(minor(m, 3, 0)), 
		-det(minor(m, 0, 1)),  det(minor(m, 1, 1)), -det(minor(m, 2, 1)),  det(minor(m, 3, 1)), 
		 det(minor(m, 0, 2)),	-det(minor(m, 1, 2)),  det(minor(m, 2, 2)), -det(minor(m, 3, 2)), 
		-det(minor(m, 0, 3)),  det(minor(m, 1, 3)), -det(minor(m, 2, 3)),  det(minor(m, 3, 3))
	);
}

/// @relates mat4
/// Returns the inverse of the matrix. By default the parameter detA is set to
/// zero and therefore the function will calculate the needed matrix determinant
/// on its own.
/// If you already know the matrix determinant you can pass its value in the
/// parameter detA so that the function doesn't have to calculate it.
/// If the matrix is singular it is impossible to calculate the inverse matrix
/// and the identity matrix is returned.
inline mat4 inverse(const mat4& a, Real detA = 0) {

	// only calculate the determinant if it has not been passed as 
	// argument
	if ( detA == 0 ) {
		detA = det(a);
	}
	
	//if ( !fequal(detA, Real(0)) ) {
		return (1.0f / detA) * adjoint(a);
	//}
	
	//return identity4();
}

/******************************************************************************/

#endif

