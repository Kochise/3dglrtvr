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
#include <algorithm>

const GLint F2X_Context::GLState::Implementation::MAX_VIEWPORT_DIMS[2] = {2048, 2048};
const GLfixed F2X_Context::GLState::Implementation::ALIASED_POINT_SIZE_RANGE[2] = {fixed16_t(1).intValue, fixed16_t(1).intValue};
const GLfixed F2X_Context::GLState::Implementation::SMOOTH_POINT_SIZE_RANGE[2] = {fixed16_t(1).intValue, fixed16_t(1).intValue};
const GLfixed F2X_Context::GLState::Implementation::ALIASED_LINE_WIDTH_RANGE[2] = {fixed16_t(1).intValue, fixed16_t(1).intValue};
const GLfixed F2X_Context::GLState::Implementation::SMOOTH_LINE_WIDTH_RANGE[2] = {fixed16_t(1).intValue, fixed16_t(1).intValue};
const GLint F2X_Context::GLState::Implementation::COMPRESSED_TEXTURE_FORMATS[] = {
	GL_PALETTE4_RGB8_OES,
	GL_PALETTE4_RGBA8_OES,
	GL_PALETTE4_R5_G6_B5_OES,
	GL_PALETTE4_RGBA4_OES,
	GL_PALETTE4_RGB5_A1_OES,
	GL_PALETTE8_RGB8_OES,
	GL_PALETTE8_RGBA8_OES,
	GL_PALETTE8_R5_G6_B5_OES,
	GL_PALETTE8_RGBA4_OES,
	GL_PALETTE8_RGB5_A1_OES,
};

F2X_Context *current_context = 0;

F2X_Context::F2X_Context(F2X_ContextCreateParams *params) :
	geometry_processor(&rasterizer)
{
	memset(&color_buffer, 0, sizeof(color_buffer));
	memset(&depth_buffer, 0, sizeof(depth_buffer));

	derived_state.init(this);

	// this is here to initialize the internal rasterizer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_DONT_CARE);

	update_culling();
}

int F2X_Context::set_param(unsigned pname, void *data)
{
	switch (pname) {
	case F2X_COLOR_BUFFER: 
		{
			if (!data) {
				memset(&color_buffer, 0, sizeof(color_buffer));
				gl_state.implementation.red_bits = 0;
				gl_state.implementation.green_bits = 0;
				gl_state.implementation.blue_bits = 0;
				gl_state.implementation.alpha_bits = 0;

				update_scissor();
				update_viewport();
				update_span_buffers();

				return 0;
			}

			F2X_RenderSurface *s = static_cast<F2X_RenderSurface*>(data);

			if ((signed)s->width > gl_state.implementation.MAX_VIEWPORT_DIMS[0] ||
				(signed)s->height > gl_state.implementation.MAX_VIEWPORT_DIMS[1] ||
				s->format != F2X_FORMAT_UINT16_R5_G5_A1_B5 ||
				s->data == 0)
			{
				return -1;
			}
			
			// data pointer must be 4 byte aligned
			if (reinterpret_cast<intptr_t>(s->data) & 3)
				return -1;
			
			color_buffer = *s;

			gl_state.implementation.red_bits = 5;
			gl_state.implementation.green_bits = 6;
			gl_state.implementation.blue_bits = 5;
			gl_state.implementation.alpha_bits = 0;

			update_scissor();
			update_viewport();
			update_span_buffers();			

			return 0;
		}
	case F2X_DEPTH_BUFFER: 
		{
			if (!data) {
				memset(&depth_buffer, 0, sizeof(depth_buffer));
				gl_state.implementation.depth_bits = 0;

				update_scissor();
				update_viewport();
				update_span_buffers();

				return 0;
			}

			F2X_RenderSurface *s = static_cast<F2X_RenderSurface*>(data);

			if (static_cast<signed>(s->width) > gl_state.implementation.MAX_VIEWPORT_DIMS[0] ||
				static_cast<signed>(s->height) > gl_state.implementation.MAX_VIEWPORT_DIMS[1] ||
				s->format != F2X_FORMAT_UINT16_R5_G5_A1_B5 ||
				s->data == 0)
			{
				return -1;
			}

			// data pointer must be 4 byte aligned
			if (reinterpret_cast<intptr_t>(s->data) & 3)
				return -1;

			depth_buffer = *s;

			gl_state.implementation.depth_bits = 16;

			update_scissor();
			update_viewport();
			update_span_buffers();

			return 0;
		}
	}

	return -1;
}

int F2X_Context::get_param(unsigned pname, void *data)
{
	switch (pname) {
	case F2X_COLOR_BUFFER: *static_cast<F2X_RenderSurface*>(data) = color_buffer; return 0;
	case F2X_DEPTH_BUFFER: *static_cast<F2X_RenderSurface*>(data) = depth_buffer; return 0;
	}

	return -1;
}

void F2X_Context::max_render_extents(unsigned &w, unsigned &h)
{
	w = 0xffffffff;
	h = 0xffffffff;

	if (color_buffer.data) {
		w = std::min(w, color_buffer.width);
		h = std::min(h, color_buffer.height);
	}

	if (depth_buffer.data) {
		w = std::min(w, depth_buffer.width);
		h = std::min(h, depth_buffer.height);
	}
}

void F2X_Context::update_viewport()
{
	unsigned w, h;
	max_render_extents(w, h);
	geometry_processor.viewport(0, 0, w, h);
}

void F2X_Context::update_scissor()
{
	if (gl_state.pixel_op.scissor.test_enable) {
		GLint x = gl_state.pixel_op.scissor.box[0];
		GLint y = gl_state.pixel_op.scissor.box[1];
		GLsizei width = gl_state.pixel_op.scissor.box[2];
		GLsizei height = gl_state.pixel_op.scissor.box[3];

		unsigned w, h;
		max_render_extents(w, h);

		GLint x2 = x + width;
		GLint y2 = y + height;
		x2 = std::min(x2, static_cast<int>(w));
		y2 = std::min(y2, static_cast<int>(h));
		x = std::max(x, 0);
		y = std::max(y, 0);

		rasterizer.clip_rect(x, y, x2 - x, y2 - y);
	} else {
		unsigned w, h;
		max_render_extents(w, h);
		rasterizer.clip_rect(0, 0, w, h);
	}
}

void F2X_Context::update_culling()
{
	if (gl_state.rasterization.cull_face_enable && 
		gl_state.rasterization.cull_face_mode != GL_FRONT_AND_BACK) 
	{
		bool cull_ccw = gl_state.rasterization.cull_face_mode == GL_CCW;
		if (gl_state.rasterization.front_face == GL_BACK) cull_ccw = !cull_ccw;
		
		geometry_processor.cull_mode(
			cull_ccw ? 
			swr::GeometryProcessor::CULL_CCW : 
			swr::GeometryProcessor::CULL_CW);

	} else geometry_processor.cull_mode(swr::GeometryProcessor::CULL_NONE);
}

void F2X_Context::update_span_buffers()
{
	unsigned w, h;
	max_render_extents(w, h);

	for (int i = 0; i < 5; ++i) span_buffers[i].resize(w);
}
