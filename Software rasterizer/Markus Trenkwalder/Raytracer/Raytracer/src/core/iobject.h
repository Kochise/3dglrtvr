#ifndef IOBJECT_H
#define IOBJECT_H

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

#include "basis.h"

class IMaterialShader;
class Ray;
class IntersectionInfo;

//! Base class of all objects
class IObject : public Basis {
public:
	//! m is the object to world space transformation matrix
	//! Each object has a MaterialShader assiciated with it, which knows how
	//! a Ray is to be shaded when the object is hit. 
	//! Not all objects need a MaterialShader e.g. CompoundObject does not.
	//! The objects hitmask can be used with a ray's typemask to test if they can intersect
	IObject(const math3d::mat4& m, const IMaterialShader* shader, unsigned long hitmask_ = 0xffffffff);
	
	//! The assiciated MaterialShader object is deleted when an object is destructed.
	virtual ~IObject();
	
	//! \brief Returns true if the ray intersects the object, false otherwise.
	//!
	//! If \a iinfo is not null then intersection information is reported through
	//! it if an intersection was found.\n
	//! if ( (ray.typemask & this->hitmask) == 0) than no intersection testing should
	//! occur and false should be returned.
	//!
	//! \see IntersectionInfo
	virtual bool intersect(const Ray& ray, IntersectionInfo* iinfo = 0) const = 0;
	
	//! Returns the material associated with this object
	const IMaterialShader* materialShader() const;
	
protected:
	const IMaterialShader* material_shader;
	
	//! The objects hitmask which was specified in the constructor.
	//! Use this together with the ray's typemask in the intersect method
	unsigned long hitmask;
};

inline const IMaterialShader* IObject::materialShader() const {
	return material_shader;
}

#endif
