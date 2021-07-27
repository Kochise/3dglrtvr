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

#ifndef MATH3DPP_QUAT_UTIL_H
#define MATH3DPP_QUAT_UTIL_H

/// @defgroup QuatUtil Quaternion utility functions
/// Here are some functions for constructing and using quaternions.
//@{

/// Quaternion to matrix converstion
inline mat3 quat2mat(const quat& qq) {

	quat q = normalized(qq);

	Real xx = q[0] * q[0];
	Real xy = q[0] * q[1];
	Real xz = q[0] * q[2];
	Real xw = q[0] * q[3];
	
	Real yy = q[1] * q[1];
	Real yz = q[1] * q[2];
	Real yw = q[1] * q[3];
	
	Real zz = q[2] * q[2];
	Real zw = q[2] * q[3];
	
	return mat3(
		1 - 2*(yy + zz), 2*(xy - zw), 2*(xz + yw),
		2*(xy + zw), 1 - 2*(xx + zz), 2*(yz - xw),
		2*(xz - yw), 2*(yz + xw), 1 - 2*(xx + yy)
	);
}

/// Matrix to quaternion conversion
inline quat mat2quat(const mat3& m) {

	Real t = m[0][0] + m[1][1] + m[2][2] + 1;
	quat q(math3d_noInit);

	if ( t > 0 ) {
		Real s = 0.5 / std::sqrt(t);
		q[3] = 0.25 / s;
		q[0] = (m[2][1] - m[1][2]) * s;
		q[1] = (m[0][2] - m[2][0]) * s;
		q[2] = (m[1][0] - m[0][1]) * s;
	} else {
		if ( m[0][0] > m[1][1] && m[0][0] > m[2][2] ) {
			Real s = 2.0 * std::sqrt( 1.0 + m[0][0] - m[1][1] - m[2][2]);
			q[0] = 0.25 * s;
			q[1] = (m[0][1] + m[1][0] ) / s;
			q[2] = (m[0][2] + m[2][0] ) / s;
			q[3] = (m[1][2] - m[2][1] ) / s;
		} else if (m[1][1] > m[2][2]) {
			Real s = 2.0 * std::sqrt( 1.0 + m[1][1] - m[0][0] - m[2][2]);
			q[0] = (m[0][1] + m[1][0] ) / s;
			q[1] = 0.25 * s;
			q[2] = (m[1][2] + m[2][1] ) / s;
			q[3] = (m[0][2] - m[2][0] ) / s;
		} else {
			Real s = 2.0 * std::sqrt( 1.0 + m[2][2] - m[0][0] - m[1][1] );
			q[0] = (m[0][2] + m[2][0] ) / s;
			q[1] = (m[1][2] + m[2][1] ) / s;
			q[2] = 0.25 * s;
			q[3] = (m[0][1] - m[1][0] ) / s;
		}
	}
	
	return q;
}

/// Quaternion from axis and angle
inline quat quatFromAA(const vec3& axis, Real angle) {
	Real a = angle * M_PI / 180.0;
	quat r(math3d_noInit);
	
	r.v = std::sin(a / 2.0) * normalized(axis);
	r.w = std::cos(a / 2.0);
	
	return r;
}

inline quat quatFromAA(Real x, Real y, Real z, Real angle) {
	return quatFromAA(vec3(x, y, z), angle);
}

/// Quaternion to axis angle
inline void quatToAA(const quat& qq, vec3* axis, Real* angle) {
	quat q = normalized(qq);
	
	*angle = 2.0 * std::acos(q.w);
	
	Real s = std::sin((*angle) / 2.0);
	if ( s != 0 ) {
		*axis = q.v / s;
	} else {
		* axis = vec3(0, 0, 0);
	}
	
	// convert from radians to degrees
	*angle = *angle * 180.0 / M_PI;
}

/// Quaternion dot product (quaternion is treated as 4d vector)
inline Real qdot(const quat& a, const quat& b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

/// Spherical linear interpolation
inline quat slerp(const quat& qq1, const quat& qq2, Real t) {
	// slerp(q1,q2) = sin((1-t)*a)/sin(a) * q1  +  sin(t*a)/sin(a) * q2
	using std::sin;
	
	quat q1 = normalized(qq1);
	quat q2 = normalized(qq2);
	
	Real a = std::acos(qdot(q1, q2));
	Real s = sin(a);
	
	if ( !fequal(s, Real(0)) ) {
		return sin((1-t)*a)/s * q1  +  sin(t*a)/s * q2;
	} else {
		// if the angle is to small use a linear interpolation
		return t * q1 + (1-t) * q2;
	}
}

/// Sperical quadtratic interpolation using a smooth cubic spline
/// The parameters @a a and @a b are the control points.
inline quat squad(const quat& q0, const quat& a, const quat& b, const quat& q1, Real t) {
	return slerp(slerp(q0, q1, t),slerp(a, b, t), 2 * t * (1 - t));
}

//@}

#endif
