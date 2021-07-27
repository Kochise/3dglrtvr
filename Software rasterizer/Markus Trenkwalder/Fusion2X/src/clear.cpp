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
#include "util.h"

namespace {
	void fill_rect(F2X_RenderSurface &surface, int x, int y, unsigned w, unsigned h, unsigned value)
	{
		if (surface.format != F2X_FORMAT_UINT16_R5_G5_A1_B5) return;

		// restrict drawing area
		int x2 = x + w;
		int y2 = y + h;

		x = std::max(x, 0);
		y = std::max(y, 0);
		x2 = std::min(x2, static_cast<int>(surface.width));
		y2 = std::min(y2, static_cast<int>(surface.height));

		x2 -= x;
		y2 -= y;	

		// compute 32 bit value to write (2x as fast)
		value &= 0xffff;
		unsigned value32 = value << 16 | value;

		unsigned short *base = static_cast<unsigned short*>(surface.data) + y * surface.pitch / 2 + x;

		for (int i = 0; i < y2; ++i) {
			unsigned short *row = base;
			unsigned n = x2;

			// initial unaligned pixel
			if (x & 1) {
				*row++ = value;
				n -= 1;
			}

			// do 32 bit writes as long as possible
			unsigned *row32 = reinterpret_cast<unsigned*>(row);

			while (n > 1) {
				*row32++ = value32;
				n -= 2;	
			}

			// trailing unaligned pixel
			row = reinterpret_cast<unsigned short*>(row32);
			if (n) *row = value;

			base += surface.pitch / 2;
		}
	}
}

void F2X_Context::glClearColorx(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
	gl_state.framebuf_ctrl.color_clear_value[0].intValue = clamp_fixed(red);
	gl_state.framebuf_ctrl.color_clear_value[1].intValue = clamp_fixed(green);
	gl_state.framebuf_ctrl.color_clear_value[2].intValue = clamp_fixed(blue);
	gl_state.framebuf_ctrl.color_clear_value[3].intValue = clamp_fixed(alpha);
}

void F2X_Context::glClearDepthx(GLclampx depth)
{
	gl_state.framebuf_ctrl.depth_clear_value = clamp_fixed(depth);
}

void F2X_Context::glClearStencil(GLint s)
{
	gl_state.framebuf_ctrl.stencil_clear_value = s;
}

void F2X_Context::glClear(GLbitfield mask)
{
	if (mask & ~(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT)) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	int x = 0;
	int y = 0;
	unsigned w;
	unsigned h;
	max_render_extents(w, h);

	if (gl_state.pixel_op.scissor.test_enable) {
		x = gl_state.pixel_op.scissor.box[0];
		y = gl_state.pixel_op.scissor.box[1];
		w = gl_state.pixel_op.scissor.box[2];
		h = gl_state.pixel_op.scissor.box[3];
	}

	if (mask & GL_COLOR_BUFFER_BIT && color_buffer.data)
		fill_rect(color_buffer, x, y, w, h, color_to_ushort(gl_state.framebuf_ctrl.color_clear_value));

	if (mask & GL_DEPTH_BUFFER_BIT && depth_buffer.data) {
		unsigned short dcv = std::min(gl_state.framebuf_ctrl.depth_clear_value, 0xffff);
		fill_rect(depth_buffer, x, y, w, h, dcv);
	}
}
