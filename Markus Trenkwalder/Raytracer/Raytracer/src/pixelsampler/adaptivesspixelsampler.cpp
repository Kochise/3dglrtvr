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

#include "adaptivesspixelsampler.h"

#include "../core/renderer.h"
#include "../core/framebuffer.h"

using namespace math3d;

AdaptiveSSPixelSampler::AdaptiveSSPixelSampler(const Framebuffer& framebuffer, float _threshold, unsigned depth) :
	threshold(_threshold),
	sampleCache((framebuffer.width() + 1) * (framebuffer.height() + 1), color3(-1.0f)),
	width(framebuffer.width() + 1),
	maxdepth(depth)
{
}
	
color3 AdaptiveSSPixelSampler::samplePixel(const Renderer* renderer, int x, int y) {
	this->renderer = renderer;
	
	if ( !isCached(x, y) )
		cacheSample(x, y, renderer->computeSample(vec2(x, y)));
	if ( !isCached(x + 1, y) )
		cacheSample(x + 1, y, renderer->computeSample(vec2(x + 1.0f, y)));
	if ( !isCached(x, y + 1) )
		cacheSample(x, y + 1, renderer->computeSample(vec2(x, y + 1.0f)));
	if ( !isCached(x + 1, y + 1) )
		cacheSample(x + 1, y + 1, renderer->computeSample(vec2(x + 1.0f, y + 1.0f)));
	
	color3 A = getCachedSample(x, y);
	color3 B = getCachedSample(x + 1, y);
	color3 C = getCachedSample(x + 1, y + 1);
	color3 D = getCachedSample(x, y + 1);
	color3 E = renderer->computeSample(vec2(x + 0.5f, y + 0.5f));
	
	vec2 min(x, y);
	vec2 max(x + 1, y + 1);
	vec2 mid = (min + max) * 0.5f;
	
	color3 result = adaptiveSampling(A, E, 0, min, mid, maxdepth);
	result += adaptiveSampling(B, E, 1, vec2(mid[0], min[1]), vec2(max[0], mid[1]), maxdepth);
	result += adaptiveSampling(C, E, 1, mid, max, maxdepth);
	result += adaptiveSampling(D, E, 1, vec2(min[0], mid[1]), vec2(mid[0], max[1]), maxdepth);
	
	return result *= 0.25f;
}

color3 AdaptiveSSPixelSampler::adaptiveSampling(const color3& corner, const color3& center, int cornerToSample, const vec2& min, const vec2& max, unsigned maxdepth) {
	
	if ( maxdepth <= 0 || similarEnough(corner, center) ) {
		return 0.5f * (corner + center);
	}
	
	color3 A, B, C, D, E;
	E = renderer->computeSample((min + max) * 0.5f);
	
	switch ( cornerToSample ) {
	case 0:
		A = corner;
		B = renderer->computeSample(vec2(max[0], min[1]));
		C = center;
		D = renderer->computeSample(vec2(min[0], max[1]));
		break;
	case 1:
		A = renderer->computeSample(min);
		B = corner;
		C = renderer->computeSample(max);
		D = center;
		break;
	case 2:
		A = center;
		B = renderer->computeSample(vec2(max[0], min[1]));
		C = corner;
		D = renderer->computeSample(vec2(min[0], max[1]));
		break;
	case 3:
		A = renderer->computeSample(min);
		B = center;
		C = renderer->computeSample(max);
		D = corner;
		break;
	}
	
	vec2 mid = (min + max) * 0.5f;
	
	color3 result= adaptiveSampling(A, E, 0, min, mid, maxdepth - 1);
	result += adaptiveSampling(B, E, 1, vec2(mid[0], min[1]), vec2(max[0], mid[1]), maxdepth - 1);
	result += adaptiveSampling(C, E, 1, mid, max, maxdepth - 1);
	result += adaptiveSampling(D, E, 1, vec2(min[0], mid[1]), vec2(mid[0], max[1]), maxdepth - 1);
	
	return result *= 0.25f;
}

bool AdaptiveSSPixelSampler::isCached(int x, int y) {
	return !(sampleCache[x + y * width][0] == -1.0f);
}

void AdaptiveSSPixelSampler::cacheSample(int x, int y, color3 c) {
	sampleCache[x + y * width] = c;
}

color3 AdaptiveSSPixelSampler::getCachedSample(int x, int y) {
	return sampleCache[x + y * width];
}

bool AdaptiveSSPixelSampler::similarEnough(const color3& c1, const color3& c2) {
	color3 tmp = c2 - c1;
	tmp[0] = std::abs(tmp[0]);
	tmp[1] = std::abs(tmp[1]);
	tmp[2] = std::abs(tmp[2]);
	
	float max = tmp[0];
	if ( tmp[1] > max ) max = tmp[1];
	if ( tmp[2] > max ) max = tmp[2];
	
	return (max < threshold);
}
