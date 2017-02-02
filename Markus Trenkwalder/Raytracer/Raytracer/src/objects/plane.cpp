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

#include "plane.h"

#include "../core/ray.h"
#include "../core/intersectioninfo.h"

#include <cassert>

using namespace math3d;

Plane::Plane(const math3d::mat4& m, const IMaterialShader* shader, unsigned long hitmask) :
	IObject(m, shader, hitmask)
{
	assert(shader != 0);
}

bool Plane::intersect(const Ray& ray, IntersectionInfo* iinfo) const {
	
	if ( (ray.typemask & hitmask) == 0 )
		return false;
	
	/*
	Ray r = transformToLocal(ray);

	// dot product of plane normal with ray direction.
	const float vd = dot(vec3(0.0f, 1.0f, 0.0f), r.direction);
	*/
	
	const float vd = 
		inv_m[1][0] * ray.direction[0] + 
		inv_m[1][1] * ray.direction[1] + 
		inv_m[1][2] * ray.direction[2];
	
	// no intersection if plane is parallel to ray direction.
	// the case where the ray lies in the plane is not considered since
	// it has no impact on rendering.
	if ( vd == 0.0f ) return false;
	
	/*
	const float v0 = dot(vec3(0.0f, 1.0f, 0.0f), r.origin) 
	*/

	const float v0 = 
		inv_m[1][0] * ray.origin[0] + 
		inv_m[1][1] * ray.origin[1] + 
		inv_m[1][2] * ray.origin[2] +
		inv_m[1][3];
	
	const float t =  - v0 / vd;
	
	// the intersection point is behind the ray, so no real intersection
	if ( t < 0.0f ) return false;
	
	// calculate the actual intersection point if needed
	if ( iinfo != 0 ) {
		iinfo->object = this;
		iinfo->abscissa = t;
		iinfo->normal = vec3(0.0f, 1.0f, 0.0f);	//normal in object space
		
		//! \todo Fill other members of the IntersectionInfo structure
	}
	
	return true;
}
