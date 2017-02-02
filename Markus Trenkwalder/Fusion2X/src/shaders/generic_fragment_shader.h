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

#ifndef GENERIC_FRAGMENT_SHADER_H_
#define GENERIC_FRAGMENT_SHADER_H_

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "../context.h"
#include "../renderer/span.h"

#include "util.h"

struct GenericFragmentShader : public swr::GenericSpanDrawer<GenericFragmentShader> {
	static const unsigned varying_count = 7;
	static const bool interpolate_z = true;

	static void begin_triangle(
		const swr::IRasterizer::Vertex& v1,
		const swr::IRasterizer::Vertex& v2,
		const swr::IRasterizer::Vertex& v3,
		int area2)
	{}

	static void single_fragment(
		int x,
		int y,
		const swr::IRasterizer::FragmentData &fd)
	{
		// depth test moved before color computations
		if (ctx->gl_state.pixel_op.depth_test.enable && ctx->depth_buffer.data) {
			unsigned short *depth_buffer = 
				static_cast<unsigned short*>(ctx->depth_buffer.data) + x + y * ctx->depth_buffer.pitch / 2;

			// only shift by 15 to take the most significant bit.
			// the highest bit is just the sign and should always be positive
			const unsigned short depth = fd.z >> 15;

			switch (ctx->gl_state.pixel_op.depth_test.func) {
			case GL_NEVER: return;
			case GL_LESS:
				if (!(depth < *depth_buffer)) return; 
				break;
			case GL_EQUAL: 
				if (!(depth == *depth_buffer)) return; 
				break;
			case GL_LEQUAL: 
				if (!(depth <= *depth_buffer)) return; 
				break;
			case GL_GREATER: 
				if (!(depth > *depth_buffer)) return; 
				break;
			case GL_NOTEQUAL: 
				if (!(depth != *depth_buffer)) return; 
				break;
			case GL_GEQUAL: 
				if (!(depth >= *depth_buffer)) return; 
				break;
			case GL_ALWAYS: 
				break;
			}

			// handle write mask
			if (ctx->gl_state.framebuf_ctrl.depth_writemask)
				*depth_buffer = depth;
		}

		// skip color computations when there is no color buffer
		if (!ctx->color_buffer.data) return;
		
		// skip all color computation if writemask indicates so
		if (!ctx->derived_state.color_writemask) return;

		int red = std::min(std::max(fd.varyings[0] >> 16, 0), 31);
		int green = std::min(std::max(fd.varyings[1] >> 16, 0), 63);
		int blue = std::min(std::max(fd.varyings[2] >> 16, 0), 31);
		int alpha = std::min(std::max(fd.varyings[3] >> 16, 0), 31);
		unsigned short color = (red << 11) | (green << 5) | blue;

		const F2X_Context::GLState::TextureUnitState &tu = ctx->gl_state.texture_units[0];
		if (tu.texture_2d_enable) {
			const GLTextureObject *tex = ctx->gl_state.texture_state.binding_2d_tex;

			if (tex->complete()) {
				unsigned talpha = 0;
				unsigned short tcolor = tex->sample(fd.varyings[4], fd.varyings[5], &talpha);				

				switch (tu.texture_env_mode) {
				case GL_DECAL:
					color = color16_add(color16_scale(color, 31 - talpha), color16_scale(tcolor, talpha));
					break;
				case GL_BLEND:
					{
						const unsigned short a = color16_modulate(color, ~tcolor);
						const unsigned short b = color16_modulate(
							ctx->derived_state.texture_units[0].texture_env_color, 
							tcolor);
						color = color16_add(a, b);
						alpha = (alpha * talpha) >> 5;
						break;
					}
				case GL_ADD:
					color = color16_add(color, tcolor);
					alpha = (alpha * talpha) >> 5;
					break;
				case GL_MODULATE:
					color = color16_modulate(color, tcolor);
					alpha = (alpha * talpha) >> 5;
					break;
				case GL_REPLACE:
					color = tcolor;
					alpha = talpha;
					break;
				}
			}
		}

		// alpha test
		if (ctx->gl_state.pixel_op.alpha_test.enable) {
			const int ref = std::min(ctx->gl_state.pixel_op.alpha_test.ref >> 11, 31);

			switch (ctx->gl_state.pixel_op.alpha_test.func) {
			case GL_NEVER:
				return;
			case GL_LESS: 
				if (!(alpha < ref)) return;
				break;
			case GL_EQUAL:
				if (!(alpha == ref)) return;
				break;
			case GL_LEQUAL:
				if (!(alpha <= ref)) return;
				break;
			case GL_GREATER:
				if (!(alpha > ref)) return;
				break;
			case GL_NOTEQUAL:
				if (!(alpha != ref)) return;
				break;
			case GL_GEQUAL:
				if (!(alpha >= ref)) return;
				break;
			case GL_ALWAYS:
				break;
			}
		}

		// fog
		if (ctx->gl_state.coloring.fog_enable) {
			const int factor = std::min(std::max(fd.varyings[6] >> 16, 0), 31);
			const unsigned short &fc = ctx->derived_state.fog_color;
			color = color16_add(color16_scale(color, factor), color16_scale(fc, 31 - factor));
		}

		unsigned short *color_buffer = 
			static_cast<unsigned short*>(ctx->color_buffer.data) + x + y * ctx->color_buffer.pitch / 2;

		if (ctx->gl_state.pixel_op.color_logic_op_enable) {
			unsigned short d = *color_buffer;
			switch (ctx->gl_state.pixel_op.logic_op_mode) {
			case GL_CLEAR: color = 0; break;
			case GL_AND: color &= d; break;
			case GL_AND_REVERSE: color &= ~d; break;
			case GL_COPY: break;
			case GL_AND_INVERTED: color = ~color & d; break;
			case GL_NOOP: return;
			case GL_XOR: color ^= d; break;
			case GL_OR: color |= d; break;
			case GL_NOR: color = ~(color | d); break;
			case GL_EQUIV: color = ~(color ^ d); break;
			case GL_INVERT: color = ~d; break;
			case GL_OR_REVERSE: color |= ~d; break;
			case GL_COPY_INVERTED: color = ~color; break;
			case GL_OR_INVERTED: color = ~color | d; break;
			case GL_NAND: color = ~(color & d); break;
			case GL_SET: color = 0xffff; break;
			}
		} else if (ctx->gl_state.pixel_op.blend.enable) {
			unsigned short src = color;
			unsigned short dst = *color_buffer;

			switch (ctx->gl_state.pixel_op.blend.src) {
			case GL_ZERO:
			case GL_SRC_ALPHA_SATURATE:
			case GL_ONE_MINUS_DST_ALPHA:
				src = 0;
				break;
			case GL_ONE: break;
			case GL_DST_ALPHA: break;
			case GL_DST_COLOR: src = color16_modulate(src, dst); break;
			case GL_ONE_MINUS_DST_COLOR: src = color16_modulate(src, ~dst); break;
			case GL_SRC_ALPHA: src = color16_scale(src, alpha); break;
			case GL_ONE_MINUS_SRC_ALPHA: src = color16_scale(src, 31 - alpha); break;
			}

			switch (ctx->gl_state.pixel_op.blend.dst) {
			case GL_ZERO:
			case GL_ONE_MINUS_DST_ALPHA: 
				dst = 0; 
				break;
			case GL_ONE: break;
			case GL_DST_ALPHA: break;
			case GL_SRC_COLOR: dst = color16_modulate(dst, src); break;
			case GL_ONE_MINUS_SRC_COLOR: dst = color16_modulate(dst, ~src); break;
			case GL_SRC_ALPHA: dst = color16_scale(dst, alpha); break;
			case GL_ONE_MINUS_SRC_ALPHA: dst = color16_scale(dst, 31 - alpha); break;
			}

			color = color16_add(src, dst);
		}

		// write color to output buffer and also honor the color_writemask
		if (ctx->derived_state.color_writemask != 0xffff) {
			*color_buffer = 
				(color & ctx->derived_state.color_writemask) | 
				(*color_buffer & ~ctx->derived_state.color_writemask);
		} else {
			*color_buffer = color;
		}
	}

	static const F2X_Context *ctx;
};

#endif
