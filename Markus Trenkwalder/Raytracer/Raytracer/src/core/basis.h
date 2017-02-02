#ifndef BASIS_H
#define BASIS_H

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

#include "ray.h"

//! \brief This class holds an object's modelling transformation
//!
//! This class provides methods to transform points, vectors, normals 
//! and rays from world space to an objects local space and vice versa. */
class Basis {
public:
	Basis(const math3d::mat4& mat);
	
	//! \brief Tranforms a vector to object space.
	//!
	//! In projective space a vector has its w = 0, e.g. (1, 0, 0, 0).
	math3d::vec3 transformVectorToLocal(const math3d::vec3& v) const;

	//! Transforms a vector from object to world space
	math3d::vec3 transformVectorToWorld(const math3d::vec3& v) const;	
	

	//! \brief Transform a point from world to object space
	//!
	//! A point in projective space has ist w components = 1, e.g. (1, 0, 0, 1)
	math3d::vec3 transformPointToLocal(const math3d::vec3& v) const;
	
	//! Transform a point from object to world space
	math3d::vec3 transformPointToWorld(const math3d::vec3& v) const;
	
	//! Transforms a point from world space to object space
	math3d::vec3 transformNormalToLocal(const math3d::vec3& n) const;
	
	//! Transforms a point from object space to world space
	math3d::vec3 transformNormalToWorld(const math3d::vec3& n) const;
	
	
	//! Transforms a ray in world space to object space
	Ray transformToLocal(const Ray& ray) const;
	
	//! Transforms a ray expressed in the object space to world space
	Ray transformToWorld(const Ray& ray) const;
	
	//! Returns a reference to the object to world space transformation matrix
	const math3d::mat4& getTransform() const;
	
protected:
	math3d::mat4 m; //!< object space to world space transformation matrix
	math3d::mat4 inv_m; //!< world space to object space transformation matrix
	
	//math3d::mat4 tr_inv_m; does not need to be stored sinve v * M == transpose(M) * v
	//math3d::mat4 tr_m;
};

inline math3d::vec3 Basis::transformVectorToLocal(const math3d::vec3& v) const {
	return math3d::transformVector(inv_m, v);
}

inline math3d::vec3 Basis::transformVectorToWorld(const math3d::vec3& v) const {
	return math3d::transformVector(m, v);
}

inline math3d::vec3 Basis::transformPointToLocal(const math3d::vec3& v) const {
	return math3d::transformPoint(inv_m, v);
}

inline math3d::vec3 Basis::transformPointToWorld(const math3d::vec3& v) const {
	return math3d::transformPoint(m, v);
}

inline math3d::vec3 Basis::transformNormalToLocal(const math3d::vec3& n) const {
	return math3d::transformVectorTranspose(m, n);
}

inline math3d::vec3 Basis::transformNormalToWorld(const math3d::vec3& n) const {
	return math3d::transformVectorTranspose(inv_m, n);
}

inline Ray Basis::transformToLocal(const Ray& ray) const {
	return Ray(transformPointToLocal(ray.origin), transformVectorToLocal(ray.direction), ray.typemask);
}

inline Ray Basis::transformToWorld(const Ray& ray) const {
	return Ray(transformPointToWorld(ray.origin), transformVectorToWorld(ray.direction), ray.typemask);
}

inline const math3d::mat4& Basis::getTransform() const {
	return m;
}

#endif
