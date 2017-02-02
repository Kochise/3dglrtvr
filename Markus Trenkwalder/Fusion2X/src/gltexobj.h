/*
    Fusion2X - OpenGL ES-CL 1.0 Implementation
    Copyright (C) 2008 Markus Trenkwalder

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Markus Trenkwalder
    trenki2@gmx.net
*/

#ifndef GLTEXOBJ_H_
#define GLTEXOBJ_H_

#include <GLES/gl.h>

#include <vector>
#include <algorithm>

struct GLTextureObject {
	GLsizei width;
	GLsizei height;

	GLenum target;

	// queryable parameters
	GLint texture_mag_filter;
	GLint texture_min_filter;
	GLint texture_wrap_s;
	GLint texture_wrap_t;

	GLenum internalformat;

	std::vector<unsigned char> data;

	// derived state
	unsigned width_minus_one;
	unsigned height_minus_one;
	unsigned width_log_two;
	unsigned height_log_two;

	unsigned short sample(int x, int y, unsigned *alpha = 0) const 
	{
		x >>= 16;
		y >>= 16;
		if (texture_wrap_s == GL_REPEAT) x &= width_minus_one;
		else x = std::min(std::max(x, 0), static_cast<int>(width_minus_one));
		if (texture_wrap_t == GL_REPEAT) y &= height_minus_one;
		else x =std::min(std::max(y, 0), static_cast<int>(height_minus_one));
		
		const unsigned short color = 
			*(reinterpret_cast<const unsigned short*>(&data[0]) + x + (y << width_log_two));
		
		if (alpha) {
			switch (internalformat) {
			case GL_RGB: *alpha = 32; break;
			case GL_RGBA: *alpha = color & 0x20 ? 32 : 0; break;
			}
		}

		return color;
	}

	bool complete() const
	{
		if (width == 0 || height == 0) return false;

		// HACK: no mipmap support
		switch (texture_min_filter) {
		case GL_LINEAR:
		case GL_NEAREST:
			break;
		default:
			return false;
		}
		return true;
	}

	GLTextureObject(GLenum t) 
		: width(0)
		, height(0)
		, target(t)
		, texture_mag_filter(GL_LINEAR)
		, texture_min_filter(GL_NEAREST_MIPMAP_LINEAR)
		, texture_wrap_s(GL_REPEAT)
		, texture_wrap_t(GL_REPEAT)
	{}
};

#endif
