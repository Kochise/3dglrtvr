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

#include "mesh3d.h"

#include "../core/intersectioninfo.h"

#include <cassert>
#include <limits>
#include <algorithm>
#include <cstdlib>

using namespace math3d;


// type definitions which were forwarded from mesh3d.h

struct Mesh3D::NodeData {
	unsigned splitaxis : 2;
	unsigned sp_or_tc  : 30;	// split position or triangle count
};

// constant which hold the maximum representable value of the
// sp_or_tc member in the above Mesh3D::NodeData structure.
// this is needed for the split_aabb() routine.
const unsigned PF_MAX_SPLITPOS = (1<<30)-1;
	
struct Mesh3D::Node {
	float plane_d;
	NodeData *data;
	Node *pchild;
	Node *nchild;
	
	bool is_leaf() const { return ( pchild == 0 && nchild == 0 ); }
};

//! \brief Intersects a ray with an axis aligned bounding box.
//!
//! If an intersection was found true is returned and the minimum and maximum
//! t-coordinates of the intersections are retrieved in the tmin and tmax
//! variables.
static bool intersect_aabb(
  const Ray& r,
  const vec3& aabb_min,
  const vec3& aabb_max,
  float &tmin,
  float &tmax
)
{
  // the use of infinity on a pentium is not good. it made the intersection test
  // always fail!
  // tmin = -std::numeric_limits<float>::infinity();
	// tmax =  std::numeric_limits<float>::infinity();
  tmin = -std::numeric_limits<float>::max();
	tmax =  std::numeric_limits<float>::max();
  
  const vec3 aabb_center = (aabb_min + aabb_max) / 2;
	const vec3 p = aabb_center - r.origin;
	
	for ( int i = 0; i < 3; ++i ) {
		const float h = aabb_max[i] - aabb_center[i];
		if ( !math3d::fequal(r.direction[i], 0.0f) ) {
			const float factor = 1.0f / r.direction[i];
			float t1 = (-h + p[i]) * factor;
			float t2 = ( h + p[i]) * factor;
			
			if ( t1 > t2 ) std::swap(t1, t2);
			if ( t1 > tmin ) tmin = t1;
			if ( t2 < tmax ) tmax = t2;
			
			if ( tmin > tmax ) return false;
			if ( tmax < 0.0f ) return false;
		}
		else if ( -p[i] + h < 0 || -p[i] - h > 0 ) return false;
	}
	return true;
}	

//! \brief Computes the intersection parameter \a t between a plane and a ray.
//! 
//! \param ray      The ray for which the intersection is to be computed
//! \param planeid  Either 0, 1 or 2 for the planes with normals
//!                 (1, 0, 0), (0, 1, 0) or (0, 0, 1)
//! \param plane_d  The parameter d in the plane equation a*x + b*y + c*z + d = 0
//! 
//! \return  Returns the t value of the intersection. (May be infinity)
static inline float plane_intersect(const Ray& ray, int planeid, float plane_d) {
	return -(ray.origin[planeid] + plane_d) / ray.direction[planeid];
}


Mesh3D::Mesh3D (
	const math3d::mat4& m,
	const IMaterialShader* shader,
	const char *filename,
	unsigned long hitmask
) :
	IObject(m, shader, hitmask)
{
	assert(shader != 0);
	
	// read the whole file into memory
	FILE *f = fopen(filename, "rb");
	if ( !f ) {
		perror(filename);
		exit(-1);
	}
	fseek(f, 0, SEEK_END);
	long filesize = ftell(f);
	fseek(f, 0, SEEK_SET);
	file_memory = malloc(filesize);
	fread(file_memory, filesize, 1, f);
	fclose(f);
	
	char *ptr = (char*)file_memory;
	aabb = *(AABB*)ptr;
	ptr += sizeof(AABB);
	vlistsize = *(unsigned*)ptr;
	ptr += sizeof(vlistsize);
	vlist = (vec3*)ptr;
	ptr += sizeof(vec3) * vlistsize;
	NodeData *root_data = (NodeData*)ptr;
	
	root_node = reconstruct_kd_tree(root_data, aabb, NULL);
}

Mesh3D::~Mesh3D() {
	destruct_kd_tree(root_node);
	free(file_memory);
}

void Mesh3D::destruct_kd_tree(Node* node)
{
	if ( !node->is_leaf() ) {
		destruct_kd_tree(node->nchild);
		destruct_kd_tree(node->pchild);
	}
	
	delete node;
}

void Mesh3D::split_aabb(
	const AABB& aabb,
	unsigned axis,
	unsigned splitpos,
	AABB *naabb,
	AABB *paabb
) const
{
	float d = (aabb.max[axis] - aabb.min[axis]);
	d *= (float)splitpos / PF_MAX_SPLITPOS;
	d += aabb.min[axis];
	
	(*naabb) = (*paabb) = aabb;
	naabb->max[axis] = d;
	paabb->min[axis] = d;
}

Mesh3D::Node* Mesh3D::reconstruct_kd_tree(NodeData* data, const AABB& aabb, NodeData** next)
{
	Node *node = new Node;
	node->data = data;
	
	assert(data->splitaxis <= 3 /*&& data->splitaxis >= 0*/);
	
	if ( data->splitaxis != 3 ) {
		NodeData *pnode;
		AABB naabb, paabb;
		
		// the floating point split position is stored in the
		// node for rendering efficiency. For this we have to compute AABBs
		// for the subtrees and extract the actual floating point position.
		
		split_aabb(aabb, data->splitaxis, data->sp_or_tc, &naabb, &paabb);
		node->plane_d = -naabb.max[data->splitaxis];
		
		node->nchild = reconstruct_kd_tree(data + 1, naabb, &pnode);
		node->pchild = reconstruct_kd_tree(pnode, paabb, next);
	}
	else {
		node->nchild = 0;
		node->pchild = 0;
		if ( next ) {
			char *tmp = (char*)data;
			tmp += sizeof(NodeData);
			tmp += sizeof(int) * data->sp_or_tc * 3;
			(*next) = (NodeData*)tmp;
		}
	}
	
	return node;
}

bool Mesh3D::intersect(const Ray& ray, IntersectionInfo* iinfo) const
{	
	if ( (ray.typemask & hitmask) == 0 )
		return false;
	
	Ray r = transformToLocal(ray);
	
	// First test the ray against the objects bounding box
	// tmin and tmax will be written to by this routine
	float tmin, tmax;
	if ( !intersect_aabb(r, aabb.min, aabb.max, tmin, tmax) )
		return false;
	
	vec2 ray_interval(std::max(0.0f, tmin), tmax);
	
	//if ( iinfo ) iinfo->object = this;
	//return true;
	
	// bounding box is hit -> test the kd tree
	return kd_tree_intersect(r, ray_interval, root_node, iinfo);
}

static inline bool interval_empty(const vec2& interval)
{
	return interval[0] >= interval[1];
}

bool Mesh3D::kd_tree_intersect(
	const Ray& ray,
	const vec2& ray_interval,
	const Node *node,
	IntersectionInfo *iinfo
)
const
{
	if ( interval_empty(ray_interval) || node == NULL )
		return false;
	
	if ( node->is_leaf() ) {
		return triangles_intersect(ray, ray_interval, node, iinfo);
	}
	else {
		const NodeData *ndata = node->data;
		const float t = plane_intersect(ray, ndata->splitaxis, node->plane_d);
		
		// determine the near and far nodes
		bool tmp = ray.origin[ndata->splitaxis] + node->plane_d < 0;
	  if ( t < 0 ) tmp = !tmp;
		const Node *near = tmp ? node->nchild : node->pchild;
		const Node *far = tmp ? node->pchild : node->nchild;
		
		// interval clipped to near side
		vec2 new_interval(ray_interval[0], std::min(t, ray_interval[1]));
		
		bool isect_found = kd_tree_intersect(ray, new_interval, near, iinfo);
		
		if ( !isect_found ) {
			// interval clipped to far side
			new_interval[0] = std::max(t, ray_interval[0]); 
			new_interval[1] = ray_interval[1];
			
			isect_found = kd_tree_intersect(ray, new_interval, far, iinfo);
		}
		
		return isect_found;
	}
}

bool Mesh3D::triangles_intersect(
	const Ray& ray,
	const vec2& ray_interval,
	const Node* node,
	IntersectionInfo *iinfo
) const
{
	if ( interval_empty(ray_interval) ) return false;
	
	bool     isect_found = false;
	float    tmin = ray_interval[1];
	unsigned triindex = 0;	
	
	// retrieve the index list for this node (it is stored directly after the
	// node's data)
	
	const unsigned *ilist = (unsigned*)(node->data + 1);
	
	for ( unsigned i = 0; i < node->data->sp_or_tc * 3; i += 3 ) {
		const vec3& v0 = vlist[ilist[i + 0]];
		const vec3  e1 = vlist[ilist[i + 1]] - v0;
		const vec3  e2 = vlist[ilist[i + 2]] - v0;
		
		const vec3 p = cross(ray.direction, e2);
		const float a = dot(e1, p);
		if ( a > -1e-5f && a < 1e-5f ) continue;
		
		const float f = 1.0f / a;
		
		const vec3 s = ray.origin - v0;
		const float u = f * dot(s, p);
		if ( u < 0.0f || 1.0f < u ) continue;
		
		const vec3 q = cross(s, e1);
		const float v = f * dot(ray.direction, q);
		if ( v < 0.0f || 1.0f < u + v ) continue;
		
		const float t = f * dot(e2, q);
		
		if ( ray_interval[0] <= t && t <= tmin ) {
			isect_found = true;
			tmin = t;
			triindex = i;
			
			// break if we do not need the nearest intersection
			if ( iinfo == 0 ) break;
		}
	}
	
	if ( isect_found && iinfo != 0 ) {
		iinfo->object = this;
		iinfo->abscissa = tmin;
			
		const vec3& v0 = vlist[ilist[triindex + 0]];
		const vec3  e1 = vlist[ilist[triindex + 1]] - v0;
		const vec3  e2 = vlist[ilist[triindex + 2]] - v0;
		iinfo->normal = cross(e1, e2);
	}
	
	return isect_found;
}

