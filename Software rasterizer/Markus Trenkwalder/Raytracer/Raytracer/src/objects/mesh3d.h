#ifndef MESH3D_H
#define MESH3D_H

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

class Mesh3D : public IObject {
public:
	//! m is the object to world space transformation matrix
	//! The 3D Mesh will be loaded from the file specified by
	//! @a filename.
	Mesh3D(
		const math3d::mat4& m,
		const IMaterialShader* shader,
		const char* filename,
		unsigned long hitmask = 0xffffffff
	);
	virtual ~Mesh3D();	
	
	virtual bool intersect(const Ray& ray, IntersectionInfo* iinfo = 0) const;
	
private:
	struct NodeData;
	struct Node;

	struct AABB {
		math3d::vec3 min;
		math3d::vec3 max;
	};
	
	bool kd_tree_intersect(
		const Ray& ray,
		const math3d::vec2& ray_interval,
		const Node *node,
		IntersectionInfo *iinfo
	) const;

	bool triangles_intersect(
		const Ray& ray,
		const math3d::vec2& ray_interval,
		const Node* node,
		IntersectionInfo *iinfo
	) const;

	void split_aabb(
		const AABB& aabb,
		unsigned axis,
		unsigned splitpos,
		AABB *naabb,
		AABB *paabb
	) const;

	Node* reconstruct_kd_tree(NodeData* data, const AABB& aabb, NodeData** next);
	void destruct_kd_tree(Node* node);
	
	AABB aabb;
	Node *root_node;
	void *file_memory;
	
	math3d::vec3* vlist;
	unsigned vlistsize;
};

#endif
