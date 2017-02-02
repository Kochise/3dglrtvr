#ifndef STRIPEPIXELSELECTOR_H
#define STRIPEPIXELSELECTOR_H

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

#include "../core/ipixelselector.h"
#include "../core/framebuffer.h"

class StripePixelSelector : public IPixelSelector {
	int stripenum;
	int maxstripes;
	int pixel;
	int start;
	int width;
	int end;
public:
	StripePixelSelector(int stnum, int stmax) :
		stripenum(stnum),
		maxstripes(stmax)
	{}
	
	void reset(const Framebuffer *fb) {
		const int h = fb->height();
		width = fb->width();
		
		start = width * (unsigned)(stripenum * (float)h/maxstripes + 0.5f);
		end = width * (unsigned)((stripenum + 1) * (float)h/maxstripes + 0.5f);
	}
	
	bool finished() const {
		return start >= end;
	}
	
	void getPixel(int *x, int *y) {
		*x = start % width;
		*y = start / width;
		++start;
	}
};

#endif
