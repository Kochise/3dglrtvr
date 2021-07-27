#ifndef SCENE_H
#define SCENE_H

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

#include "color3.h"

class IObject;
class IBackgroundShader;
class Ray;
class IntersectionInfo;

class Scene {
public:
	//! \brief Creates a scene object with the specified object as its root.
	//!
	//! \param root     Specifies the root object. This parameter cannot be null.
	//! \param bgShader Specifies the BackgroundShader object to be used for
	//!                 shading rays that don't hit any objects. The default
	//!                 behavior is to use a black background.
	Scene(const IObject* root, const IBackgroundShader *bgShader = 0);
	
	//! On destruction the scene object deletes the root object and the
	//! background shader object
	~Scene();
	
	//! Intersect this ray with the scene's root object.
	bool intersect(const Ray& r, IntersectionInfo* iinfo = 0) const;
	
	//! \brief Traces a ray through the scene and returns a color value.
	//! 
	//! The scene is traced recursively with a maximum recursion depth
	//! given by maxdepth. A recursion depth of 0 means that only the first
	//! intersection point on a surface is shaded by a material shader.
	//! The material shader does not generate reflections because that would
	//! require the this trace method to be called recursively.
	color3 trace(const Ray& r, int maxdepth) const;
	
private:
	const IObject *root_object;
	const IBackgroundShader *background_shader;
};

#endif
