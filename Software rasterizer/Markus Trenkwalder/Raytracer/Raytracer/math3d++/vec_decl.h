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

#ifndef MATH3DPP_VEC_DECL_H
#define MATH3DPP_VEC_DECL_H

// class forward declarations
class vec2;
class vec3;
class vec4;

class NoInitFlag;

/// 2d vector class
class vec2 {
private:
	Real data[2];
public:
	vec2(Real v = 0);
	vec2(Real x, Real y);
	explicit vec2(const vec3& v);
	
	/// Creates a vector without initializing its data members
	explicit vec2(NoInitFlag&) {}
	
	vec2& operator+= (const vec2& v);
	vec2& operator-= (const vec2& v);
	vec2& operator*= (const vec2& v);	
	vec2& operator/= (const vec2& v);
	
	Real& operator[] (int i);
	Real  operator[] (int i) const;
	
	// scalar operators for speed and resolving ambiguity problems
	// with implicit constructor calls
	vec2& operator+= (Real f);
	vec2& operator-= (Real f);
	vec2& operator*= (Real f);
	vec2& operator/= (Real f);
};


/// 3d vector class
class vec3 {
private:
	Real data[3];
public:
	vec3(Real v = 0);
	vec3(Real x, Real y, Real z);
	explicit vec3(const vec4& v);
	explicit vec3(const vec2& v, Real z = 0);
	
	/// Creates a vector without initializing its data members
	explicit vec3(NoInitFlag&) {}
	
	vec3& operator+= (const vec3& v);
	vec3& operator-= (const vec3& v);
	vec3& operator*= (const vec3& v);	
	vec3& operator/= (const vec3& v);
	
	Real& operator[] (int i);
	Real  operator[] (int i) const;
	
	// scalar operators for speed and resolving ambiguity problems
	// with implicit constructor calls
	vec3& operator+= (Real f);
	vec3& operator-= (Real f);
	vec3& operator*= (Real f);
	vec3& operator/= (Real f);
};


/// 4d vector class
class vec4 {
private:
	Real data[4];
public:
	vec4(Real v = 0);
	vec4(Real x, Real y, Real z, Real w);
	explicit vec4(const vec3& v, Real w = 1);
	
	/// Creates a vector without initializing its data members
	explicit vec4(NoInitFlag&) {}
	
	vec4& operator+= (const vec4& v);
	vec4& operator-= (const vec4& v);
	vec4& operator*= (const vec4& v);	
	vec4& operator/= (const vec4& v);
	
	Real& operator[] (int i);
	Real  operator[] (int i) const;
	
	// scalar operators for speed and resolving ambiguity problems
	// with implicit constructor calls
	vec4& operator+= (Real f);
	vec4& operator-= (Real f);
	vec4& operator*= (Real f);
	vec4& operator/= (Real f);
};

#endif
