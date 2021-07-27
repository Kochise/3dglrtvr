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

#include "compoundobject.h"

#include "../core/intersectioninfo.h"

#include <cassert>

class ProxyObject : public IObject {
	IObject *obj;
public:
	ProxyObject(const math3d::mat4& m, IObject *object) :
		IObject( m * object->getTransform(), 0),
		obj(object)
	{
	}
	
	virtual ~ProxyObject() {
		delete obj;
		
		// this prevents the base destructor from deleting the material a second
		// time (our material is just a reference to the proxied object's material)
		material_shader = 0;	
	}
	
	virtual bool intersect(const Ray& ray, IntersectionInfo* iinfo = 0) const {
		bool result = obj->intersect(ray, iinfo);
		
		if ( result && iinfo && iinfo->object ) {
			
			//! \todo This is a SEVERE HACK to also be able to use compound
			//! objects in another compound.
			ProxyObject *self = const_cast<ProxyObject*>(this);
			self->material_shader = iinfo->object->materialShader();
			
			iinfo->object = this;
		}
		
		return result;
	};
};

// Destructor destroys its children
CompoundObject::~CompoundObject() {
	for ( object_vector::iterator i = objects.begin(); i != objects.end(); ++i )
		delete *i;
}

bool CompoundObject::intersect(const Ray& ray, IntersectionInfo* iinfo) const {
	Ray r = transformToLocal(ray);	
	
	// iinfo is 0 when no intersection information is needed; we can exit if
	// we found any intersection
	if ( iinfo == 0 ) {
		for ( const_object_iterator i = objects.begin(); i != objects.end(); ++i ) {
			if ( (*i)->intersect(r, 0) )	return true;
		}
		return false;
	}
	
	// Intersection information is to be computed; we must find the closest
	// intersection
	
	IntersectionInfo info;
	const_object_iterator i;
	
	info.object = 0; // signals that no intersection has been found
	
	// first find any intersection
	for ( i = objects.begin(); i != objects.end(); ++i ) {
		if ( (*i)->intersect(r, &info) ) {
			++i;
			break;
		}
	}
		
	IntersectionInfo info2;
	IntersectionInfo *info_ptr = &info, *info2_ptr = &info2;
	
	// cycle through the remaining objects to see if there is any closer intersection
	for ( ; i != objects.end(); ++i ) {
		if ( (*i)->intersect(r, info2_ptr) && info2_ptr->abscissa < info_ptr->abscissa) {
			// swapping the pointers is faster than copying the data of the nearest
			// found object. (Especially important when IntersectionInfo is extended)
			IntersectionInfo *tmp = info_ptr;
			info_ptr = info2_ptr;
			info2_ptr = tmp;
		}
	}
	
	*iinfo = *info_ptr;
	
	return iinfo->object != 0;
}

void CompoundObject::addObject(IObject *obj) {
	assert(obj != 0);
	objects.push_back(new ProxyObject(getTransform(), obj));
}


