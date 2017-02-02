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

#ifndef MATH3DPP_H
#define MATH3DPP_H

/** @file
 ** Main file of the math3d++ library 
 */

#include <cmath>
#include <limits>
//#include <iostream>

// iostream under linux includes some system headers that define this macro
// it interferes with the library so it is beeing undefined.
#undef minor

#ifdef _MSC_VER
	#pragma warning ( push )
	#pragma warning ( disable : 4244 )	// conversion from <x> to <y>, possible loss of data
#endif

// math3d++ OpenGL support with some overloaded functions
// The Microsoft OpenGL header requires windows.h (at least some definitios made therein)
// to be included.
#ifdef _MSC_VER
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#include <GL/gl.h>

#ifdef USE_EXTGL
	#warning USE_EXTGL has been renamed to MATH3D_USE_EXTGL
#endif

#ifdef USE_FEQUAL_COMPARE
	#warning USE_FEQUAL_COMPARE has been renamed to MATH3D_USE_FEQUAL_COMPARE
#endif

#ifdef MATH3D_USE_EXTGL
	#include "extgl.h"
#endif

#ifndef MATH3D_NO_NAMESPACE
/// All the classes and functions of the library are included in this namespace
namespace math3d {
#endif

	
	template <class _T>
	inline bool fequal(_T a, _T b, _T epsilon = std::numeric_limits<_T>::epsilon()) {
		return std::abs(a - b) <= epsilon;
	}
	
	// if defined the function fequal defined above will be
	// used to compare for equality of vectors, matrices and quaternions
	// #define MATH3D_USE_FEQUAL_COMPARE
	
	/// The type Real is used in the whole math3d++ library
	#define MATH3D_REAL_IS_FLOAT 1
	typedef float Real;
	
	/// Datatype used in constructors of vectors, matrices and quatrenions to state
	/// that the data members should not be initialized with default values.
	/// @see math3d_noInit
	class NoInitFlag {};
	
	/// Use this variable in constructors of vectors, matrices and quaternions to state
	/// that the data members should not be initialized with default values.
	static NoInitFlag math3d_noInit;
	
	#include "vec_decl.h"
	#include "vec2.h"
	#include "vec3.h"
	#include "vec4.h"
	
	#include "mat_decl.h"
	#include "mat2.h"
	#include "mat3.h"
	#include "mat4.h"
	
	#include "matvec_util.h"
	
	#include "quat.h"
	#include "quat_util.h"
	
	#include "math3dGL.h"
	
	//#include "math3d_io.h"

#ifndef MATH3D_NO_NAMESPACE
}	// end namespace
#endif


// Reset the warning state
#ifdef _MSC_VER
	#pragma warning ( pop )
#endif



/** @mainpage
 * Documentation for the main page has still to be done.:-)
 * 
 * <a href="http://trenki.al.msshost.com/math3d/">Download page</a>
 */

#endif
