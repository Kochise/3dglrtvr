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

#include "context.h"

void F2X_Context::glGetIntegerv (GLenum pname, GLint *params)
{
	switch (pname) {
	case GL_SMOOTH_POINT_SIZE_RANGE:
		for (int i = 0; i < 2; ++i)
			params[i] = gl_state.implementation.SMOOTH_POINT_SIZE_RANGE[i] >> 16;
		return;
	case GL_SMOOTH_LINE_WIDTH_RANGE:
		for (int i = 0; i < 2; ++i)
			params[i] = gl_state.implementation.SMOOTH_LINE_WIDTH_RANGE[i] >> 16;
		return;
	case GL_ALIASED_POINT_SIZE_RANGE:
		for (int i = 0; i < 2; ++i)
			params[i] = gl_state.implementation.ALIASED_POINT_SIZE_RANGE[i] >> 16;
		return;
	case GL_ALIASED_LINE_WIDTH_RANGE:
		for (int i = 0; i < 2; ++i)
			params[i] = gl_state.implementation.ALIASED_LINE_WIDTH_RANGE[i] >> 16;
		return;
	case GL_IMPLEMENTATION_COLOR_READ_TYPE_OES:
		*params = gl_state.implementation.IMPLEMENTATION_COLOR_READ_TYPE_OES;
		return;
	case GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES:
		*params = gl_state.implementation.IMPLEMENTATION_COLOR_READ_FORMAT_OES;
		return;
	case GL_MAX_LIGHTS:
		*params = gl_state.implementation.MAX_LIGHTS;
		return;
	case GL_MAX_TEXTURE_SIZE:
		*params = gl_state.implementation.MAX_TEXTURE_SIZE;
		return;
	case GL_MAX_MODELVIEW_STACK_DEPTH:
		*params = gl_state.implementation.MAX_MODELVIEW_STACK_DEPTH;
		return;
	case GL_MAX_PROJECTION_STACK_DEPTH:
		*params = gl_state.implementation.MAX_PROJECTION_STACK_DEPTH;
		return;
	case GL_MAX_TEXTURE_STACK_DEPTH:
		*params = gl_state.implementation.MAX_TEXTURE_STACK_DEPTH;
		return;
	case GL_MAX_VIEWPORT_DIMS:
		for (int i = 0; i < 4; ++i)
			params[i] = gl_state.implementation.MAX_VIEWPORT_DIMS[i];
		return;
	case GL_MAX_ELEMENTS_VERTICES:
		*params = gl_state.implementation.MAX_ELEMENTS_VERTICES;
		return;	
	case GL_MAX_ELEMENTS_INDICES:
		*params = gl_state.implementation.MAX_ELEMENTS_INDICES;
		return;
	case GL_MAX_TEXTURE_UNITS:
		*params = gl_state.implementation.MAX_TEXTURE_UNITS;
		return;
	case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
		*params = gl_state.implementation.NUM_COMPRESSED_TEXTURE_FORMATS;
		return;
	case GL_COMPRESSED_TEXTURE_FORMATS:
		for (int i = 0; i < gl_state.implementation.NUM_COMPRESSED_TEXTURE_FORMATS; ++i)
			params[i] = gl_state.implementation.COMPRESSED_TEXTURE_FORMATS[i];
		return;
	case GL_SUBPIXEL_BITS:
		*params = gl_state.implementation.SUBPIXEL_BITS;
		return;
	case GL_RED_BITS:
		*params = gl_state.implementation.red_bits;
		return;
	case GL_GREEN_BITS:
		*params = gl_state.implementation.green_bits;
		return;
	case GL_BLUE_BITS:
		*params = gl_state.implementation.blue_bits;
		return;
	case GL_ALPHA_BITS:
		*params = gl_state.implementation.alpha_bits;
		return;
	case GL_DEPTH_BITS:
		*params = gl_state.implementation.depth_bits;
		return;
	case GL_STENCIL_BITS:
		*params = gl_state.implementation.stencil_bits;
		return;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}
}

const GLubyte * F2X_Context::glGetString (GLenum name)
{
	switch (name) {
	case GL_VENDOR: return (GLubyte*)"Markus Trenkwalder (a.k.a. Trenki)";
	case GL_RENDERER: return (GLubyte*)"Fusion2X";
	case GL_VERSION: return (GLubyte*)"OpenGL ES-CL 1.0";
	case GL_EXTENSIONS: return 0;
	default: emit_error(GL_INVALID_ENUM);
	}

	return 0;
}

GLenum F2X_Context::glGetError (void)
{
	GLenum result = gl_state.error;
	gl_state.error = GL_NO_ERROR;
	return result;
}
