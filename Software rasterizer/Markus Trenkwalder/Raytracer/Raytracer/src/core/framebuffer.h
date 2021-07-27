#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

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

#include <vector>

class Framebuffer {
public:
	Framebuffer(int width, int height);
	
	int width() const;
	int height() const;

	//! Gives access to the pixel at (x, y)
	color3& pixelAt(int x, int y);
	const color3& pixelAt(int x, int y) const;
	
	//! clears the framebuffer with the specified color
	void clear(const color3& color);
	
	//! \brief Converts the pixel coordinates to NDC coordinates.
	//! 
	//! The lower left corner maps to (-1, -1) and the upper right
	//! corner maps to (1, 1). \n
	//! A pixel coordinate of (0.5, 0.5) lies exactly at the center of the pixel.
	math3d::vec2 convertToNDC(const math3d::vec2& coord);
	
	//! Converts from NDC coordinates back to pixel coordinates
	math3d::vec2 convertFromNDC(const math3d::vec2& coord);
	
	//! returns a reference to the internal data
	std::vector<color3>& getData();
private:
	int w;
	int h;
	std::vector<color3> data;
};

inline int Framebuffer::width() const {
	return w;
}

inline int Framebuffer::height() const {
	return h;
}

inline color3& Framebuffer::pixelAt(int x, int y) {
	return data[y * w + x];
}

inline const color3& Framebuffer::pixelAt(int x, int y) const {
	return data[y * w + x];
}

inline math3d::vec2 Framebuffer::convertToNDC(const math3d::vec2& coord) {
	// this maps (0, 0) to (-1, 1). A Pixel coordinate of (0, 0) is located
	// in the upper left corner. 
	return math3d::vec2(
		(2.0f * coord[0] - width()) / width(),
		-(2.0f * coord[1] - height()) / height()
	);
}

inline math3d::vec2 Framebuffer::convertFromNDC(const math3d::vec2& coord) {
	return math3d::vec2(
		(width() / 2.0f) * (coord[0] + 1.0f),
		(height() / 2.0f) * (1.0f - coord[1])
	);
}

inline std::vector<color3>& Framebuffer::getData() {
	return data;
}

#endif
