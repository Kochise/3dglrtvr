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

#ifndef MATH3DPP_GL_H
#define MATH3DPP_GL_H

#if MATH3D_REAL_IS_FLOAT == 1
	#define GL_FUNC(fctname) fctname ## f
#else
	#define GL_FUNC(fctname) fctname ## d
#endif

#ifndef MATH3D_USE_EXTGL

/// @defgroup OpenGLUtil OpenGL utility functions
/// The math3d++ library proviedes some useful OpenGL overloads
/// so that you can use vectors and matrices from the library.
//@{

inline void glLoadMatrix(const mat4& m) {
	mat4 mt = transpose(m);
	GL_FUNC(glLoadMatrix)(&mt[0][0]);
}

inline void glLoadTransposeMatrix(const mat4& m) {
	GL_FUNC(glLoadMatrix)(&m[0][0]);
}

inline void glMultMatrix(const mat4& m) {
	mat4 mt = transpose(m);
	GL_FUNC(glMultMatrix)(&mt[0][0]);
}

inline void glMultTransposeMatrix(const mat4& m) {
	GL_FUNC(glMultMatrix)(&m[0][0]);
}

#else /* MATH3D_USE_EXTGL */

inline void glLoadMatrix(const mat4& m) {
	GL_FUNC(glLoadTransposeMatrix)(&m[0][0]);
}

inline void glLoadTransposeMatrix(const mat4& m) {
	GL_FUNC(glLoadMatrix)(&m[0][0]);
}

inline void glMultMatrix(const mat4& m) {
	GL_FUNC(glMultTransposeMatrix)(&m[0][0]);
}

inline void glMultTransposeMatrix(const mat4& m) {
	GL_FUNC(glMultMatrix)(&m[0][0]);
}

#endif

inline void glTranslate(const vec3& v) {
	GL_FUNC(glTranslate)(v[0], v[1], v[2]);
}

inline void glScale(const vec3& v) {
	GL_FUNC(glScale)(v[0], v[1], v[2]);
}

inline void glRotate(Real angle, const vec3& v) {
	GL_FUNC(glRotate)(angle, v[0], v[1], v[2]);
}

inline void glVertex(const vec2& v) {
	GL_FUNC(glVertex2)(v[0], v[1]);
}

inline void glVertex(const vec3& v) {
	GL_FUNC(glVertex3)(v[0], v[1], v[2]);
}

inline void glVertex(const vec4& v) {
	GL_FUNC(glVertex4)(v[0], v[1], v[2], v[3]);
}

inline void glNormal(const vec3& v) {
	GL_FUNC(glNormal3)(v[0], v[1], v[2]);
}

inline void glTexCoord(const vec2& v) {
	GL_FUNC(glTexCoord2)(v[0], v[1]);
}

inline void glTexCoord(const vec3& v) {
	GL_FUNC(glTexCoord3)(v[0], v[1], v[2]);
}

inline void glTexCoord(const vec4& v) {
	GL_FUNC(glTexCoord4)(v[0], v[1], v[2], v[3]);
}

inline void glColor(const vec3& v) {
	GL_FUNC(glColor3)(v[0], v[1], v[2]);
}

inline void glColor(const vec4& v) {
	GL_FUNC(glColor4)(v[0], v[1], v[2], v[3]);
}

#ifdef MATH3D_USE_EXTGL

inline void glMultiTexCoord(GLenum texunit, const vec2& v) {
	GL_FUNC(glMultiTexCoord2)(texunit, v[0], v[1]);
}

inline void glMultiTexCoord(GLenum texunit, const vec3& v) {
	GL_FUNC(glMultiTexCoord3)(texunit, v[0], v[1], v[2]);
}

inline void glMultiTexCoord(GLenum texunit, const vec4& v) {
	GL_FUNC(glMultiTexCoord4)(texunit, v[0], v[1], v[2], v[3]);
}

inline void glSecondaryColor(const vec3& v) {
	GL_FUNC(glSecondaryColor3)(v[0], v[1], v[2]);
}

#endif

//@}

#undef GL_FUNC

#endif
