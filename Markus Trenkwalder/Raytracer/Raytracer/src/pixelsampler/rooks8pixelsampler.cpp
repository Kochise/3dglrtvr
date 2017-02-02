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

#include "rooks8pixelsampler.h"

#include "../core/renderer.h"

using namespace math3d;

color3 Rooks8PixelSampler::samplePixel(const Renderer* renderer, int x, int y) {
	color3 result = renderer->computeSample(vec2(x + 0.5625f, y + 0.0625f));
	result += renderer->computeSample(vec2(x + 0.3125f, y + 0.1875f));
	result += renderer->computeSample(vec2(x + 0.0625f, y + 0.3125f));
	result += renderer->computeSample(vec2(x + 0.8125f, y + 0.4375f));
	result += renderer->computeSample(vec2(x + 0.1875f, y + 0.5625f));
	result += renderer->computeSample(vec2(x + 0.9375f, y + 0.6875f));
	result += renderer->computeSample(vec2(x + 0.6875f, y + 0.8125f));
	result += renderer->computeSample(vec2(x + 0.4375f, y + 0.9375f));
			
	return result /= 8.0f;
}
