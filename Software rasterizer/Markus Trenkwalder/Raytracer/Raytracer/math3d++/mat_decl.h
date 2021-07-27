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

#ifndef MATH3DPP_MAT_DECL_H
#define MATH3DPP_MAT_DECL_H

// matrix forwad declarations
class mat2;
class mat3;
class mat4;

class NoInitFlag;

/// 2x2 matrix class
class mat2 {
	Real data[2][2];
public:
	explicit mat2(Real v = 1);
	mat2(Real a11, Real a12,
	     Real a21, Real a22);
	explicit mat2(const mat3& m);
	
	/// Create a matrix without initializing its data members
	explicit mat2(NoInitFlag&) {}
	
	mat2& operator+= (const mat2& m);
	mat2& operator-= (const mat2& m);
	mat2& operator*= (const mat2& m);
	mat2& operator/= (const mat2& m);
	
	mat2& compMult(const mat2& m);

	Real* operator[] (int i);
	const Real* operator[] (int i) const;
	
	// scalar operators for speed and resolving ambiguity problems
	// with implicit constructor calls
	mat2& operator+= (Real f);
	mat2& operator-= (Real f);
	mat2& operator*= (Real f);
	mat2& operator/= (Real f);
};


/// 3x3 matrix class
class mat3 {
	Real data[3][3];
public:
	explicit mat3(Real v = 1);
	mat3(Real a11, Real a12, Real a13,
       Real a21, Real a22, Real a23,
		   Real a31, Real a32, Real a33);
	explicit mat3(const mat4& m);
	explicit mat3(const mat2& m);
	
	/// Create a matrix without initializing its data members
	explicit mat3(NoInitFlag&) {}
	
	mat3& operator+= (const mat3& m);
	mat3& operator-= (const mat3& m);
	mat3& operator*= (const mat3& m);
	mat3& operator/= (const mat3& m);
	
	mat3& compMult(const mat3& m);

	Real* operator[] (int i);
	const Real* operator[] (int i) const;

	// scalar operators for speed and resolving ambiguity problems
	// with implicit constructor calls
	mat3& operator+= (Real f);
	mat3& operator-= (Real f);
	mat3& operator*= (Real f);
	mat3& operator/= (Real f);
};

/// 4x4 matrix class
class mat4 {
	Real data[4][4];
public:
	explicit mat4(Real v = 1);
	mat4(Real a11, Real a12, Real a13, Real a14, 
       Real a21, Real a22, Real a23, Real a24, 
       Real a31, Real a32, Real a33, Real a34,
       Real a41, Real a42, Real a43, Real a44);
	explicit mat4(const mat3& m);
	
	/// Create a matrix without initializing its data members
	explicit mat4(NoInitFlag&) {}
	
	mat4& operator+= (const mat4& m);
	mat4& operator-= (const mat4& m);
	mat4& operator*= (const mat4& m);
	mat4& operator/= (const mat4& m);
	
	mat4& compMult(const mat4& m);

	Real* operator[] (int i);
	const Real* operator[] (int i) const;

	// scalar operators for speed and resolving ambiguity problems
	// with implicit constructor calls
	mat4& operator+= (Real f);
	mat4& operator-= (Real f);
	mat4& operator*= (Real f);
	mat4& operator/= (Real f);
};

#endif

