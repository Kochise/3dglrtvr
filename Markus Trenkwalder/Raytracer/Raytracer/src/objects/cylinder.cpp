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

#include "cylinder.h"

#include "../core/intersectioninfo.h"

#include <cassert>
#include <limits>
#include <algorithm>

using namespace math3d;

Cylinder::Cylinder(const math3d::mat4& m, const IMaterialShader* shader, unsigned long hitmask) :
	IObject(m, shader, hitmask)
{
	assert(shader != 0);
}

bool Cylinder::intersect(const Ray& ray, IntersectionInfo* iinfo) const {
	
	if ( (ray.typemask & hitmask) == 0 )
		return false;
	
	
	Ray r = transformToLocal(ray);
	
	const float odist = r.origin[0] * r.origin[0] + r.origin[2] * r.origin[2];
	
	const float a = r.direction[0] * r.direction[0] + r.direction[2] * r.direction[2];
	const float b = 2.0f * (r.origin[0] * r.direction[0] + r.origin[2] * r.direction[2]);
	const float c = odist - 1.0f;
	
	const float d = b * b - 4.0f * a * c;
	
	if ( d >= 0 || odist < 1.0f ) {
		
		float t1, t2, y = 0.0f;
		
		if ( d >= 0.0f ) {
			const float ra2 = 1.0f / (2.0f * a);
			const float mb = -b;
			const float root = std::sqrt(d);
			
			t1 = (mb - root) * ra2;
			t2 = (mb + root) * ra2;
			
			// choose the nearest intersection that is not behind the ray origin.
			// If both are behind we don't report an intersection
			if ( t2 > 0 && t2 < t1 ) std::swap(t1, t2);			
			if ( t1 < 0 ) return false;
			
			y = r.origin[1] + t1 * r.direction[1];
		}
		else {
			t1 = 0.0f;
			t2 = std::numeric_limits<float>::max();
		}

		// The potential intersection point does not lie on the cylinder but on the
		// caps
		if ( d < 0.0f || (y < -0.5f || 0.5f < y) ) {
			static vec3 normals[2] = {
				vec3(0, -1, 0),
				vec3(0,  1, 0)
			};
			
			const float rld = 1.0f / r.direction[1];
			
			int normalIdx = 0;
			
			float tt1 = (-0.5f - r.origin[1]) * rld;
			float tt2 = ( 0.5f - r.origin[1]) * rld;
			
			if ( tt2 > 0 && tt2 < tt1 ) { tt1 = tt2; normalIdx = 1; }
			
			// if the intersection with the nearest plane is not between the two 
			// intersection points of the ray on the cylinder the intersection with
			// the plane is not located on the cylinder cap
			if ( tt1 < t1 || t2 < tt1 ) return false;
				
			if ( iinfo ) {
				iinfo->object = this;
				iinfo->abscissa = tt1;
				iinfo->normal = normals[normalIdx];
			}
			
			return true;
		}
		
		// The ray hits the cylinder and not the caps
		if ( iinfo ) {
			iinfo->object = this;
			iinfo->abscissa = t1;
			iinfo->normal = vec3(r.direction[0] * t1 + r.origin[0], 0.0f, r.direction[2] * t1 + r.origin[2]);
		}
		
		return true;
	}

	return false;
 }
