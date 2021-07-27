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

#include "sphere.h"

#include "../core/intersectioninfo.h"

#include <cassert>

using namespace math3d;

Sphere::Sphere(const math3d::mat4& m, const IMaterialShader* shader, unsigned long hitmask) :
	IObject(m, shader, hitmask)
{
	assert(shader != 0);
}

bool Sphere::intersect(const Ray& ray, IntersectionInfo* iinfo) const {
	
	if ( (ray.typemask & hitmask) == 0 )
		return false;

	
	Ray r = transformToLocal(ray);
	
	// reciprocal of the tranformed direction vector's length
	const float rld = 1.0f / length(r.direction);
	
	const vec3 l = -r.origin;         // distance from origin to sphere center
	const float l2 = length2(l);      // squared distance
	const float s = dot(l, r.direction) * rld;	// length of the projection of l onto d
	
	// Early exit if sphere center behind ray (s < 0) and
	// ray origin outside sphere.
	if ( s < 0.0f && l2 > 1.0f) return false;
	
	// if distance from sphere center to nearest point on ray direction
	// vector larger than sphere radius we can exit
	const float m2 = l2 - s * s;
	if ( m2 > 1.0f ) return false;
	
	// calculate the intersection point if needed
	if ( iinfo != 0 ) {
		const float q = std::sqrt(1.0f - m2);
		float t;
		
		if ( l2 > 1.0f ) t = s - q;	// ray origin is outside sphere
		else t = s + q;           // ray origin is inside sphere
		
		t *= rld;  // abscissa in units of the direction vector's length
		
		iinfo->object = this;
		iinfo->abscissa = t;
		iinfo->normal = r.origin + r.direction * t;
		
		//! \todo fill other members of the IntersectionInfo structure
	}
	
	return true;
}
