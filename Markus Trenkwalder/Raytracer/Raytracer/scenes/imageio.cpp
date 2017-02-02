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

#include "imageio.h"

#include "core/framebuffer.h"

#include <cstdio>

void saveFramebufferToPPM(const char* filename, const Framebuffer& framebuffer) {
	
	size_t buffersize = framebuffer.width() * framebuffer.height() * 3;
	unsigned char* buffer = new unsigned char[buffersize];
	unsigned char* dataptr = buffer;
	
	// convert the values in the framebuffer to the range [0, 255]
	for ( int y = 0; y < framebuffer.height(); ++y )
	for ( int x = 0; x < framebuffer.width(); ++x )
	{
		const color3& color = framebuffer.pixelAt(x, y);
		
		unsigned char r = static_cast<unsigned char>(std::min(1.0f, std::max(0.0f, color[0])) * 255.0f + 0.5f);
		unsigned char g = static_cast<unsigned char>(std::min(1.0f, std::max(0.0f, color[1])) * 255.0f + 0.5f);
		unsigned char b = static_cast<unsigned char>(std::min(1.0f, std::max(0.0f, color[2])) * 255.0f + 0.5f);
		
		*dataptr++ = r;
		*dataptr++ = g;
		*dataptr++ = b;			
	}
	
	FILE *f = fopen(filename, "wb");
	
	fprintf(f, "P6\n%i %i\n255\n", framebuffer.width(), framebuffer.height());
	fwrite(buffer, 1, buffersize, f);
	fclose(f);
	
	delete [] buffer;
}

void saveFramebufferToPFM(const char* filename, const Framebuffer& framebuffer) {
	
	size_t buffersize = framebuffer.width() * framebuffer.height() * 3;
	float *buffer = new float[buffersize];
	float *dataptr = buffer;
	
	for ( int y = framebuffer.height() - 1; y >= 0; --y )
	for ( int x = 0; x < framebuffer.width(); ++x )
	{
		const color3& color = framebuffer.pixelAt(x, y);
		
		*dataptr++ = color[0];
		*dataptr++ = color[1];
		*dataptr++ = color[2];
	}
	
	FILE *f = fopen(filename, "wb");
	fprintf(f, "PF\n%i %i\n-1.0\n", framebuffer.width(), framebuffer.height());
	fwrite(buffer, sizeof(float), buffersize, f);
	fclose(f);
}

static float Expose(float light, float exposure) {
	return 1.0f - exp(-light * exposure);
}

void saveFramebufferToPPMExposure(const char* filename, const Framebuffer& framebuffer, float exposure) {
	
	size_t buffersize = framebuffer.width() * framebuffer.height() * 3;
	unsigned char* buffer = new unsigned char[buffersize];
	unsigned char* dataptr = buffer;
	
	// convert the values in the framebuffer to the range [0, 255]
	for ( int y = 0; y < framebuffer.height(); ++y )
	for ( int x = 0; x < framebuffer.width(); ++x )
	{
		const color3& color = framebuffer.pixelAt(x, y);
		
		unsigned char r = static_cast<unsigned char>(Expose(color[0], exposure) * 255.0f + 0.5f);
		unsigned char g = static_cast<unsigned char>(Expose(color[1], exposure) * 255.0f + 0.5f);
		unsigned char b = static_cast<unsigned char>(Expose(color[2], exposure) * 255.0f + 0.5f);
		
		*dataptr++ = r;
		*dataptr++ = g;
		*dataptr++ = b;			
	}
	
	FILE *f = fopen(filename, "wb");
	
	fprintf(f, "P6\n%i %i\n255\n", framebuffer.width(), framebuffer.height());
	fwrite(buffer, 1, buffersize, f);
	fclose(f);
	
	delete [] buffer;
}
