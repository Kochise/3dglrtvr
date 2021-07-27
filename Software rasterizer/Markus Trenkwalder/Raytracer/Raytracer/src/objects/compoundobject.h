#ifndef COMPOUNDOBJECT_H
#define COMPOUNDOBJECT_H

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

#include "../core/iobject.h"

#include <vector>

//! An object composited from many other objects
class CompoundObject : public IObject {
public:	
	//! 'm' is the compounds object space to world space transformation matrix
	//! This object does not have a shader associated to it because it is solely
	//! intended to hold other objects.
	CompoundObject(const math3d::mat4& m) : IObject(m, 0) {}
	
	//! This composited object deletes its children on descruction
	~CompoundObject();
	
	//! Finds the nearest intersection of the ray with the objects in
	//! the composition. \n
	//! \copydoc IObject::intersect()
	virtual bool intersect(const Ray& ray, IntersectionInfo* iinfo = 0) const;

	void addObject(IObject *obj);
	
private:
	typedef std::vector<const IObject*> object_vector;
	typedef object_vector::const_iterator const_object_iterator;
	
	object_vector objects;
};


#endif
