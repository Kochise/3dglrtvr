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

void F2X_Context::glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	switch (type) {
	case GL_UNSIGNED_BYTE:
	case GL_FIXED:
		break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}

	if (size != 4) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	gl_state.color_array.size = size;
	gl_state.color_array.type = type;
	gl_state.color_array.stride = stride;
	gl_state.color_array.pointer = pointer;	
}

void F2X_Context::glNormalPointer(GLenum type, GLsizei stride, const GLvoid *pointer)
{
	switch (type) {
	case GL_BYTE:
	case GL_SHORT:
	case GL_FIXED:
		break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}

	if (stride < 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	gl_state.normal_array.type = type;
	gl_state.normal_array.stride = stride;
	gl_state.normal_array.pointer = pointer;
}

void F2X_Context::glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	switch (type) {
	case GL_BYTE:
	case GL_SHORT:
	case GL_FIXED:
		break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}

	if (size < 2 || size > 4 || stride < 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	const int at = gl_state.client_active_texture - GL_TEXTURE0;
	gl_state.texture_coord_array[at].size = size;
	gl_state.texture_coord_array[at].type = type;
	gl_state.texture_coord_array[at].stride = stride;
	gl_state.texture_coord_array[at].pointer = pointer;	
}

void F2X_Context::glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	switch (type) {
	case GL_BYTE:
	case GL_SHORT:
	case GL_FIXED:
		break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}

	if (size < 2 || size > 4 || stride < 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	gl_state.vertex_array.size = size;
	gl_state.vertex_array.type = type;
	gl_state.vertex_array.stride = stride;
	gl_state.vertex_array.pointer = pointer;
}
