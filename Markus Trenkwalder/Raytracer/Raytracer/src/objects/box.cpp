/*
	Raytracer
	Copyright (c) 2004-2005, Trenkwalder Markus
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

#include "box.h"

#include "../core/intersectioninfo.h"

#include <cassert>
#include <limits>

using namespace math3d;

Box::Box(const math3d::mat4& m, const IMaterialShader* shader, unsigned long hitmask) :
	IObject(m, shader, hitmask)
{
	assert(shader != 0);
}
	
bool Box::intersect(const Ray& ray, IntersectionInfo* iinfo) const {
	
	if ( (ray.typemask & hitmask) == 0 )
		return false;
	
	
	static vec3 normals[6] = {
		vec3(-1.0f,  0.0f,  0.0f),
		vec3( 1.0f,  0.0f,  0.0f),
		vec3( 0.0f, -1.0f,  0.0f),
		vec3( 0.0f,  1.0f,  0.0f),
		vec3( 0.0f,  0.0f, -1.0f),
		vec3( 0.0f,  0.0f,  1.0f)
	};	
	
	Ray r = transformToLocal(ray);
	
  // the use of infinity on a pentium is not good. it made the intersection test
  // always fail!
	// float tmin = -std::numeric_limits<float>::infinity();
	// float tmax =  std::numeric_limits<float>::infinity();
  float tmin = -std::numeric_limits<float>::max();
	float tmax =  std::numeric_limits<float>::max();
	int normalmin = 0;
	int normalmax = 1;
	
	for ( int i = 0; i < 3; ++i ) {
		if ( !math3d::fequal(r.direction[i], 0.0f) ) {
			float factor = 1.0f / r.direction[i];
			float t1 = (-0.5f - r.origin[i]) * factor;
			float t2 = ( 0.5f - r.origin[i]) * factor;
			
			if ( t1 > t2 ) {
				if ( t2 > tmin ) { tmin = t2;	normalmin = i * 2 + 1; }
				if ( t1 < tmax ) { tmax = t1;	normalmax = i * 2; }
			}
			else {
				if ( t1 > tmin ) { tmin = t1;	normalmin = i * 2; }
				if ( t2 < tmax ) { tmax = t2;	normalmax = i * 2 + 1; }
			}
			
			if ( tmin > tmax ) return false;
			if ( tmax < 0.0f ) return false;
		}
		else if ( r.origin[i] < -0.5f || 0.5f < r.origin[i] ) return false;
	}
	
	if ( iinfo != 0 ) {
		iinfo->object = this;
		if ( tmin > 0.0f ) {
			iinfo->abscissa = tmin;
			iinfo->normal = normals[normalmin];
		}
		else {
			iinfo->abscissa = tmax;
			iinfo->normal = normals[normalmax];
		}
		
		//! \todo Fill other members of IntersectionInfo
	}
	
	return true;
}
