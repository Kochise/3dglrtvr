#ifndef RENDERER_H
#define RENDERER_H

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

#include "math3d++.h"
#include "color3.h"

#include <cassert>

class Scene;
class ICamera;
class IPixelSampler;
class IPixelSelector;
class Framebuffer;

class Renderer {
public:
	Renderer();
	
	void setFramebuffer(Framebuffer* fb);
	void setCamera(const ICamera* c);
	void setPixelSampler(IPixelSampler* sampler);
	void setScene(const Scene* s);
	void setPixelSelector(IPixelSelector* pixelselector);
	void setMaxRecursion(int r);
	
	//! \brief Renders the scene into the framebuffer.
	//!
	//! Only the pixels that are selected for rendering by the pixelselector
	//! are going to be written. All the other pixels will keep their value.
	void renderScene() const;
	
	//! \brief Generates a Ray that passes through pixelcoord on the image plane
	//! and traces it through the scene.
	//!
	//! This method is intended to be used by IPixelSampler objects to compute
	//! individual samples for a pixel.
	color3 computeSample(const math3d::vec2& pixelcoord) const;
	
private:	
	Framebuffer *framebuffer;
	const ICamera *camera;
	IPixelSampler *pixelsampler;
	IPixelSelector* pixelselector;
	const Scene* scene;
	int max_recursion;
};

inline Renderer::Renderer() :
	framebuffer(0),
	camera(0),
	pixelsampler(0),
	pixelselector(0),
	scene(0),
	max_recursion(10)
{}

inline void Renderer::setFramebuffer(Framebuffer* fb) {
	assert(fb != 0);
	framebuffer = fb;
}

inline void Renderer::setCamera(const ICamera* c) {
	assert(c != 0);
	camera = c;
}

inline void Renderer::setPixelSampler(IPixelSampler* sampler) {
	assert(sampler != 0);
	pixelsampler = sampler;
}

inline void Renderer::setScene(const Scene* s) {
	assert(s != 0);
	scene = s;
}

inline void Renderer::setPixelSelector(IPixelSelector* s) {
	pixelselector = s;
}

inline void Renderer::setMaxRecursion(int r) {
	max_recursion = r;
}

#endif
