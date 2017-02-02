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

#include "quincunxpixelsampler.h"

#include "../core/renderer.h"
#include "../core/framebuffer.h"

using namespace math3d;

QuincunxPixelSampler::QuincunxPixelSampler(const Framebuffer& framebuffer) :
	sampleCache((framebuffer.width() + 1) * (framebuffer.height() + 1), color3(-1.0f)),
	width(framebuffer.width() + 1)
{
}
	
color3 QuincunxPixelSampler::samplePixel(const Renderer* renderer, int x, int y) {
	
	if ( !isCached(x, y) )
		cacheSample(x, y, renderer->computeSample(vec2(x, y)) * 0.125f);
	if ( !isCached(x + 1, y) )
		cacheSample(x + 1, y, renderer->computeSample(vec2(x + 1.0f, y)) * 0.125f);
	if ( !isCached(x, y + 1) )
		cacheSample(x, y + 1, renderer->computeSample(vec2(x, y + 1.0f)) * 0.125f);
	if ( !isCached(x + 1, y + 1) )
		cacheSample(x + 1, y + 1, renderer->computeSample(vec2(x + 1.0f, y + 1.0f)) * 0.125f);
	
	color3 result = getCachedSample(x, y);
	result += getCachedSample(x + 1, y);
	result += getCachedSample(x, y + 1);
	result += getCachedSample(x + 1, y + 1);
	
	result += renderer->computeSample(vec2(x + 0.5f, y + 0.5f)) * 0.5f;
	
	return result;
}

bool QuincunxPixelSampler::isCached(int x, int y) {
	return !(sampleCache[x + y * width][0] == -1.0f);
}

void QuincunxPixelSampler::cacheSample(int x, int y, color3 c) {
	sampleCache[x + y * width] = c;
}

color3 QuincunxPixelSampler::getCachedSample(int x, int y) {
	return sampleCache[x + y * width];
}
