#ifndef QUINCUNXPIXELSAMPLER_H
#define QUINCUNXPIXELSAMPLER_H

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

#include "../core/ipixelsampler.h"

#include <vector>

class Framebuffer;

//! \brief The QuincunxPixelSampler uses 5 samples per pixel where 4 are located
//! on the corners of the pixel and can be shared by the neighboring pixels.
//! This way only an average of 2 samples per pixel need to be computed.
class QuincunxPixelSampler : public IPixelSampler {
public:
	
	//! The Framebuffer object is used to determine the size an internal cache for
	//! the samples and must be the same as the one used for rendering.
	explicit QuincunxPixelSampler(const Framebuffer& framebuffer);
	
	virtual color3 samplePixel(const Renderer* renderer, int x, int y);
	
private:
	bool isCached(int x, int y);
	void cacheSample(int x, int y, color3 c);	
	color3 getCachedSample(int x, int y);
	
private:
	std::vector<color3> sampleCache;
	int width;
};

#endif
