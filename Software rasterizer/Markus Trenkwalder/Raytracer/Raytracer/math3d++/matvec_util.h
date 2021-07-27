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

#ifndef MATH3DPP_MATVEC_UTIL_H
#define MATH3DPP_MATVEC_UTIL_H

/******************************************************************************/

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

/******************************************************************************/

/// @defgroup MatrixVector Matrix and vector interoperability functions
/// Functions for matrices and vectors.
//@{

/// Multiplies the vector v1 by the transposed vector v2 and returns the
/// resulting matrix.
/// <PRE>
///   x1                 m00 m01 
/// ( y1 ) * (x2 y2) = ( m10 m11 )
///                      
/// </PRE>
inline mat2 multvvt(const vec2& v1, const vec2& v2) {
	mat2 r;
	
	for ( int j = 0; j < 2; ++j )
	for ( int k = 0; k < 2; ++k )
		r[j][k] = v1[j] * v2[k];
	
	return r;
}
/// Multiplies the vector v1 by the transposed vector v2 and returns the
/// resulting matrix.
/// <PRE>
///   x1                    m00 m01 m02
/// ( y1 ) * (x2 y2 z2) = ( m10 m11 m12 )
///   z1                    m20 m21 m22
/// </PRE>
inline mat3 multvvt(const vec3& v1, const vec3& v2) {
	mat3 r;
	
	for ( int j = 0; j < 3; ++j )
	for ( int k = 0; k < 3; ++k )
		r[j][k] = v1[j] * v2[k];
	
	return r;
}

/// Multiplies the vector v1 by the transposed vector v2 and returns the
/// resulting matrix.
/// <PRE>
///   x1                       m00 m01 m02 m03
/// ( y1 ) * (x2 y2 z2 w2) = ( m10 m11 m12 m13 )
///   z1                       m20 m21 m22 m13
///   w1                       m30 m31 m32 m33
/// </PRE>
inline mat4 multvvt(const vec4& v1, const vec4& v2) {
	mat4 r;
	
	for ( int j = 0; j < 4; ++j )
	for ( int k = 0; k < 4; ++k )
		r[j][k] = v1[j] * v2[k];
	
	return r;
}



/******************************************************************************/
/// Matrix-Vector multiplication functions

/// matrix * vector multiplication
inline vec2 operator* (const mat2& m, const vec2& v) {
	return vec2(
		m[0][0] * v[0] + m[0][1] * v[1],
		m[1][0] * v[0] + m[1][1] * v[1]
	);
}

/// matrix * vector multiplication
inline vec3 operator* (const mat3& m, const vec3& v) {
	return vec3(
		m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2],	
		m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2],
		m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2]
	);
}

/// matrix * vector multiplication
inline vec4 operator* (const mat4& m, const vec4& v) {
	return vec4(
		m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2] + m[0][3] * v[3],
		m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2] + m[1][3] * v[3],
		m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2] + m[2][3] * v[3],
		m[3][0] * v[0] + m[3][1] * v[1] + m[3][2] * v[2] + m[3][3] * v[3]
	);
}

/// vector * matrix multiplication
inline vec2 operator* (const vec2& v, const mat2& m) {
	return vec2(	
		v[0] * m[0][0] + v[1] * m[1][0],
		v[0] * m[0][1] + v[1] * m[1][1]
	);
}

/// vector * matrix multiplication
inline vec3 operator* (const vec3& v, const mat3& m) {
	return vec3(	
		v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0],
		v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1],
		v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2]
	);
}

/// vector * matrix multiplication
inline vec4 operator* (const vec4& v, const mat4& m) {
	return vec4(
		v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0] + v[3] * m[3][0],
		v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1] + v[3] * m[3][1],	
		v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2] + v[3] * m[3][2],
		v[0] * m[0][3] + v[1] * m[1][3] + v[2] * m[2][3] + v[3] * m[3][3]
	);
}

/******************************************************************************/
// Transformations for 3d points and vectors

/// Trasforms a point using the 4x4 transformation matrix. The w coordinate is
/// assumed to be 1; (v.x, v.y, v.z, 1). Because of this assumption this is faster
/// than a matrix multiplication. \n
/// Computes vec3(m * vec4(v, 1));
inline vec3 transformPoint(const mat4& m, const vec3& v) {
	return vec3(
		m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2] + m[0][3],
		m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2] + m[1][3],
		m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2] + m[2][3]		
	);
}

/// Transforms a point by the transpose of the supplied 4x4 matrix. The point's w
/// coordinate is assumed to be 1. \n
/// Computes vec3(transpose(m) * vec4(v, 1));
inline vec3 transformPointTranspose(const mat4& m, const vec3& v) {
	return vec3(
		v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0] + m[3][0],
		v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1] + m[3][1],
		v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2] + m[3][2]
	);
}

/// Transforms a vector by the 4x4 matrix. The w coordinate is assumed to be 0.
/// (v.x, v.y, v.z, 0) \n
/// Computes vec3(m * vec4(v, 0));
inline vec3 transformVector(const mat4& m, const vec3& v) {
	return vec3(
		m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2],
		m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2],
		m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2]		
	);
}

/// Transforms a vector by the transpose of the supplied matrix. The w coordinate
/// of the vector is assumed to be 0. \n
/// Computes vec3(transpose(m) * vec4(v, 1));
inline vec3 transformVectorTranspose(const mat4& m, const vec3& v) {
	return vec3(
		v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0],
		v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1],
		v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2]
	);
}

/******************************************************************************/
// Transformations for 2d points and vectors

/// Transforms the point using the 3x3 matrix. The point's z coordinate is
/// assumed to be 1; \n
/// Computes vec2(m * vec3(v, 1));
inline vec2 transformPoint(const mat3& m, const vec2& v) {
	return vec2(
		m[0][0] * v[0] + m[0][1] * v[1] + m[0][2],
		m[1][0] * v[0] + m[1][1] * v[1] + m[1][2]
	);
}

/// Transforms the point using the 3x3 matrix's transpose. The point's z
/// coordinate is assumed to be 1. \n
/// Computes vec2(transpose(m) * vec3(v, 1));
inline vec2 transformPointTranspose(const mat3& m, const vec2& v) {
	return vec2(
		v[0] * m[0][0] + v[1] * m[1][0] + m[2][0],
		v[0] * m[0][1] + v[1] * m[1][1] + m[2][1]
	);
}

/// Transform the vector using the 3x3 matrix. The vector's z coordinate
/// is assumed to be 0. \n
/// Computes vec2(m * vec3(v, 0));
inline vec2 transformVector(const mat3& m, const vec2& v) {
	return vec2(
		m[0][0] * v[0] + m[0][1] * v[1],
		m[1][0] * v[0] + m[1][1] * v[1]
	);
}

/// Transform the vector using the transpose of the supplied 3x3 matrix. The
/// vector's z coordinate is assumed to be 0. \n
/// Computes vec2(transpose(m) * vec3(v, 0));
inline vec2 transformVectorTranspose(const mat3& m, const vec2& v) {
	return vec2(
		v[0] * m[0][0] + v[1] * m[1][0],
		v[0] * m[0][1] + v[1] * m[1][1]
	);
}

/******************************************************************************/

/// Returns the inverse of a 4x4 matrix. It is assumed that the matrix passed
/// as argument describes a rigid-body transformation.
inline mat4 fastInverse(const mat4& m) {
	Real tx, ty, tz;
	
	tx = -m[0][0] * m[0][3] - m[1][0] * m[1][3] - m[2][0] * m[2][3];
	ty = -m[0][1] * m[0][3] - m[1][1] * m[1][3] - m[2][1] * m[2][3];
	tz = -m[0][2] * m[0][3] - m[1][2] * m[1][3] - m[2][2] * m[2][3];
	
	return mat4(
		m[0][0], m[1][0], m[2][0], tx,
		m[0][1], m[1][1], m[2][1], ty,
		m[0][2], m[1][2], m[2][2], tz,
		0, 0, 0, 1
	);
}

//@}

/******************************************************************************/
// matrix and vector utility functions for graphics programming

/// @defgroup MatrixVectorUtils Matrix and vector utility functions for graphics programming
/// The following functions are especially useful for graphics programming to create
/// some transformation matrices.
//@{

/// Returns a translation matrix
inline mat4 translate(Real x, Real y, Real z) {
	return mat4(
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	);
}

/// Returns a translation matrix
inline mat4 translate(const vec3& v) {
	return translate(v[0], v[1], v[2]);
}


/// Returns a scaling matrix
inline mat4 scale(Real x, Real y, Real z) {
return mat4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	);
}

/// Returns a scaling matrix
inline mat4 scale(const vec3& v) {
	return scale(v[0], v[1], v[2]);
}

/// Returns a frustum transformation matrix
inline mat4 frustum(Real l, Real r, Real b, Real t, Real n, Real f) {
	return mat4(
		(2.0*n)/(r-l), 0.0, (r+l)/(r-l), 0.0,
		0.0, (2.0*n)/(t-b), (t+b)/(t-b), 0.0,
		0.0, 0.0, -(f+n)/(f-n), -(2.0*f*n)/(f-n),
		0.0, 0.0, -1.0, 0.0
	);
}

/// Returns an orthograhic transformation matrix
inline mat4 ortho(Real l, Real r, Real b, Real t, Real n, Real f) {
	return mat4(
		2.0/(r-l), 0.0, 0.0, -(r+l)/(r-l),
		0.0, 2.0/(t-b), 0.0, -(t+b)/(t-b),
		0.0, 0.0, -2.0/(f-n), -(f+n)/(f-n),
		0.0, 0.0, 0.0, 1.0
	);
}

/// Returns a rotation matrix
inline mat4 rotate(Real angle, const vec3& v) {
	Real a = angle * M_PI / 180.0;
	vec3 u = normalized(v);
	
	mat3 S(
		    0, -u[2],  u[1],
		 u[2],     0, -u[0],
		-u[1],  u[0],     0 
	);
	
	mat3 uut = multvvt(u, u);
	mat3 R = uut + std::cos(a) * (identity3() - uut) + std::sin(a) * S;
	
	return mat4(R);
}

/// Returns a rotation matrix
inline mat4 rotate(Real a, Real x, Real y, Real z) {
	return rotate(a, vec3(x, y, z));
}

/// Returns a perspective projection matrix
inline mat4 perspective(Real fovy, Real aspect, Real zNear, Real zFar) {
		Real dz = zFar - zNear;
		Real rad = fovy / 2 * M_PI / 180;
		Real s = std::sin(rad);
		
		if ( ( dz == 0 ) || ( s == 0 ) || ( aspect == 0 ) ) {
			return identity4();
		}
		
		Real cot = std::cos(rad) / s;
		
		mat4 m = identity4();
		m[0][0] = cot / aspect;
		m[1][1] = cot;
		m[2][2] = -(zFar + zNear) / dz;
		m[3][2] = -1;
		m[2][3] = -2 * zNear * zFar / dz;
		m[3][3] = 0;
		
		return m;
}

/// Returns a lookat transformation matrix
inline mat4 lookat(const vec3& eye, const vec3& center, const vec3& up) {
	vec3 forward = normalized(center - eye);
	vec3 side = normalized(cross(forward, up));
	
	vec3 up2 = cross(side, forward);
	
	mat4 m = identity4();
	
	m[0][0] = side[0];
	m[0][1] = side[1];
	m[0][2] = side[2];
	
	m[1][0] = up2[0];
	m[1][1] = up2[1];
	m[1][2] = up2[2];
	
	m[2][0] = -forward[0];
	m[2][1] = -forward[1];
	m[2][2] = -forward[2];
	
	return m * translate(-eye);
	
}

/// Returns a pick matrix
inline mat4 pickmatrix(Real x, Real y, Real dx, Real dy, int viewport[4]) {
	if (dx <= 0 || dy <= 0) { 
		return identity4();
  }

	return 
		translate(
			(viewport[2] - 2 * (x - viewport[0])) / dx,
			(viewport[3] - 2 * (y - viewport[1])) / dy,
			0
		) *
		scale(
			viewport[2] / dx,
			viewport[2] / dy,	
			1
		);
}

/// Constructs and returns a shadow transformation matrix.
/// The parameter @a q describes the light source and the parameters
/// @a a, @a b, @a c and @a d describe the plane onto which the geometry will b
/// projected.
/// The plane has the equation a*x + b*y + c*z + d = 0 
inline mat4 shadowmatrix(const vec4& q, Real a, Real b, Real c, Real d) {
	mat4 m;
	
	m[0][0] =  b * q[1] + c * q[2] + d * q[3];
	m[0][1] = -b * q[0];
	m[0][2] = -c * q[0];
	m[0][3] = -d * q[0];
	
	m[1][0] = -a * q[1];
	m[1][1] =  a * q[0] + c * q[2] + d * q[3];
	m[1][2] = -c * q[1];
	m[1][3] = -d * q[1];
	

	m[2][0] = -a * q[2];
	m[2][1] = -b * q[2];
	m[2][2] =  a * q[0] + b * q[1] + d * q[3];
	m[2][3] = -d * q[2];

	m[3][1] = -a * q[3];
	m[3][2] = -b * q[3];
	m[3][3] = -c * q[3];
	m[3][0] =  a * q[0] + b * q[1] + c * q[2];

	return m;
}

/// Constructs a shear-matrix that shears component @a i by @a factor with
/// Respect to component @a j.
inline mat4 shear(int i, int j, Real factor) {
	mat4 m = identity4();
	
	m[i][j] = factor;
	
	return m;
}

/// Constructs a orientation matrix given the Euler angles head (yaw),
/// pitch and roll.
inline mat4 euler(Real head, Real pitch, Real roll) {
	return rotate(roll, 0, 0, 1) * rotate(pitch, 1, 0, 0) * rotate(head, 0, 1, 0);
}


/******************************************************************************/

/// Returns the reflection direction for the incident vector @a I and the
/// surface normal @a N. @a N must already be normalized.
inline vec2 reflect(const vec2& I, const vec2& N) {
	return I - 2 * dot(N, I) * N;
}

/// Returns the reflection direction for the incident vector @a I and the
/// surface normal @a N. @a N must already be normalized.
inline vec3 reflect(const vec3& I, const vec3& N) {
	return I - 2 * dot(N, I) * N;
}

/// Returns the reflection direction for the incident vector @a I and the
/// surface normal @a N. @a N must already be normalized.
inline vec4 reflect(const vec4& I, const vec4& N) {
	return I - 2 * dot(N, I) * N;
}

/// Returns the refraction vector for the incident vector @a I, the surface
/// normal @a N and the ratio of indices of refraction @a eta. \n
/// If the angle is to big a vector with all zeros is returned.
/// The input parameters @a I and @a N must already be normalized.
inline vec2 refract(const vec2& I, const vec2& N, float eta) {
	const float d = dot(N, I);
	const float k = 1.0f - eta * eta * (1.0f - d * d);
	
	if ( k < 0.0f )
		return vec2(0.0f);
	else
		return eta * I - (eta * d + std::sqrt(k)) * N;
}

/// Returns the refraction vector for the incident vector @a I, the surface
/// normal @a N and the ratio of indices of refraction @a eta. \n
/// If the angle is to big a vector with all zeros is returned.
/// The input parameters @a I and @a N must already be normalized.
inline vec3 refract(const vec3& I, const vec3& N, float eta) {
	const float d = dot(N, I);
	const float k = 1.0f - eta * eta * (1.0f - d * d);
	
	if ( k < 0.0f )
		return vec3(0.0f);
	else
		return eta * I - (eta * d + std::sqrt(k)) * N;
}

/// Returns the refraction vector for the incident vector @a I, the surface
/// normal @a N and the ratio of indices of refraction @a eta. \n
/// If the angle is to big a vector with all zeros is returned.
/// The input parameters @a I and @a N must already be normalized.
inline vec4 refract(const vec4& I, const vec4& N, float eta) {
	const float d = dot(N, I);
	const float k = 1.0f - eta * eta * (1.0f - d * d);
	
	if ( k < 0.0f )
		return vec4(0.0f);
	else
		return eta * I - (eta * d + std::sqrt(k)) * N;
}

//@}

#endif
