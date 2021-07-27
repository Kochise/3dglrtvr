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

namespace {

	// takes line loop or strip indices and returns 2 indices per line segment
	std::vector<unsigned> indices_from_line_loop_or_strip(const std::vector<unsigned> &uindices, bool loop)
	{
		std::vector<unsigned> llindices;

		// security check
		if (uindices.size() < 2) return llindices;

		for (size_t i = 0; i < uindices.size() - 1; ++i) {
			llindices.push_back(uindices[i]);
			llindices.push_back(uindices[i + 1]);
		}
		if (loop) {
			llindices.push_back(uindices.back());
			llindices.push_back(uindices[0]);
		}

		return llindices;
	}

	// takes triangle strip indices and converts them to 3 indices per triangle.
	std::vector<unsigned> indices_from_triangle_strip(const std::vector<unsigned> &uindices)
	{
		std::vector<unsigned> tsindices;

		if (uindices.size() < 3) return tsindices;

		unsigned temp[2] = { uindices[0], uindices[1] };
		unsigned p = 0;

		for (size_t i = 2; i < uindices.size(); ++i) {
			tsindices.push_back(temp[p]);
			tsindices.push_back(temp[1 - p]);
			tsindices.push_back(uindices[i]);
			temp[p] = uindices[i]; 
			p = 1 - p;
		}

		return tsindices;
	}

	std::vector<unsigned> indices_from_triangle_fan(const std::vector<unsigned> &uindices)
	{
		std::vector<unsigned> tfindices;

		if (uindices.size() < 3) return tfindices;

		unsigned last = uindices[1];

		for (size_t i = 2; i < uindices.size(); ++i) {
			tfindices.push_back(uindices[0]);
			tfindices.push_back(last);
			tfindices.push_back(uindices[i]);
			last = uindices[i]; 
		}

		return tfindices;
	}
}

void F2X_Context::glDrawArrays(GLenum mode, GLint first, GLsizei count) try
{
	if (count < 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	if (!derived_state.update_state_before_draw()) return;

	// construct index array since we can only do indexed rendering
	std::vector<unsigned> uindices(count);
	for (int i = 0; i < count; ++i) uindices[i] = first++;

	switch (mode) {
	case GL_POINTS:
	case GL_LINES:
		if (mode == GL_POINTS) geometry_processor.draw_points(count, &uindices[0]);
		else geometry_processor.draw_lines(count, &uindices[0]);
		break;
	case GL_LINE_STRIP: // fall through
	case GL_LINE_LOOP:
		{
			std::vector<unsigned> llindices = 
				indices_from_line_loop_or_strip(uindices, mode == GL_LINE_LOOP);
			geometry_processor.draw_lines(llindices.size(), &llindices[0]);
			break;
		}
	case GL_TRIANGLES:
		geometry_processor.draw_triangles(uindices.size(), &uindices[0]);
		break;
	case GL_TRIANGLE_STRIP:
		{
			std::vector<unsigned> tsindices = indices_from_triangle_strip(uindices);
			geometry_processor.draw_triangles(tsindices.size(), &tsindices[0]);
			break;
		}
	case GL_TRIANGLE_FAN:
		{
			std::vector<unsigned> tfindices = indices_from_triangle_fan(uindices);
			geometry_processor.draw_triangles(tfindices.size(), &tfindices[0]);
			break;
		}
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}
} catch (...) {
	emit_error(GL_OUT_OF_MEMORY);
}

void F2X_Context::glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) try
{
	if (count < 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	if (!derived_state.update_state_before_draw()) return;

	// will hold indices converted to unsigned
	std::vector<unsigned> uindices(count);

	switch (type) {
	case GL_UNSIGNED_BYTE:
		{
			const GLubyte *i = static_cast<const GLubyte*>(indices);
			std::copy(i, i + count, uindices.begin());
			break;
		}
	case GL_UNSIGNED_SHORT:
		{
			const GLushort *i = static_cast<const GLushort*>(indices);
			std::copy(i, i + count, uindices.begin());
			break;
		}
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}

	switch (mode) {
	case GL_POINTS: 
		geometry_processor.draw_points(count, &uindices[0]); 
		break;
	case GL_LINES: 
		geometry_processor.draw_lines(count, &uindices[0]); 
		break;
	case GL_LINE_STRIP: // fall through
	case GL_LINE_LOOP: 
		{
			std::vector<unsigned> llindices = 
				indices_from_line_loop_or_strip(uindices, mode == GL_LINE_LOOP);
			geometry_processor.draw_lines(llindices.size(), &llindices[0]);
			break;
		}
	case GL_TRIANGLES:
		geometry_processor.draw_triangles(uindices.size(), &uindices[0]);
		break;
	case GL_TRIANGLE_STRIP:
		{
			std::vector<unsigned> tsindices = indices_from_triangle_strip(uindices);
			geometry_processor.draw_triangles(tsindices.size(), &tsindices[0]);
			break;
		}
	case GL_TRIANGLE_FAN:
		{
			std::vector<unsigned> tfindices = indices_from_triangle_fan(uindices);
			geometry_processor.draw_triangles(tfindices.size(), &tfindices[0]);
			break;
		}
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}
} catch (...) {
	emit_error(GL_OUT_OF_MEMORY);
}
