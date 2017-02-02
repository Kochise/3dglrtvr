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

#include "triangle.h"

#include "../core/intersectioninfo.h"

#include <cassert>

using namespace math3d;

Triangle::Triangle(const vec3& _v0, const vec3& _v1, const vec3& _v2, const IMaterialShader* shader, unsigned long hitmask):
	IObject(identity4(), shader, hitmask),
	v0(_v0),
	e1(_v1 - _v0),
	e2(_v2 - _v0),
	normal(cross(e1, e2))
{
	assert(shader != 0);
}
	
bool Triangle::intersect(const Ray& ray, IntersectionInfo* iinfo) const {
	
	if ( (ray.typemask & hitmask) == 0 )
		return false;
	
	vec3 p = cross(ray.direction, e2);
	float a = dot(e1, p);
	if ( a > -1e-5f && a < 1e-5f ) return false;

	a = 1.0f / a;

	vec3 s = ray.origin - v0;
	float u = a * dot(s, p);
	if ( u < 0.0f || 1.0f < u ) return false;

	vec3 q = cross(s, e1);
	float v = a * dot(ray.direction, q);
	if ( v < 0.0f || 1.0f < u + v ) return false;
	
	if ( iinfo ) {
		iinfo->object = this;
		iinfo->abscissa = a * dot(e2, q);
		iinfo->normal = normal;
	}
	
	return true;
}
