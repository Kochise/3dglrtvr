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
#ifndef OPTIMIZED_FRAGMENT_SHADER_H
#define OPTIMIZED_FRAGMENT_SHADER_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "../../context.h"
#include "../../renderer/span.h"
#include "spanfuncs.h"

#include <map>

template
<
	bool interp_color,
	bool interp_alpha,
	bool interp_texcoord,
	bool interp_fog,
	bool interp_z,
	bool color_write,
	bool depth_write
>
struct OptimizedFragmentShader : 
	public swr::SpanDrawerBase
	<
		OptimizedFragmentShader
		<
			interp_color,
			interp_alpha,
			interp_texcoord,
			interp_fog,
			interp_z,
			color_write,
			depth_write
		>
	>
{
	static const bool interpolate_z = interp_z;
	static const unsigned varying_count = 
		(interp_color ? 3 : 0) +
		(interp_alpha ? 1 : 0) + 
		(interp_texcoord ? 2 : 0) + 
		(interp_fog ? 1 : 0);

	static void begin_triangle(
		const swr::IRasterizer::Vertex& v1,
		const swr::IRasterizer::Vertex& v2,
		const swr::IRasterizer::Vertex& v3,
		int area2)
	{}
	
	static void affine_span(
		int x, 
		int y, 
		swr::IRasterizer::FragmentData fd, 
		const swr::IRasterizer::FragmentData &step, 
		unsigned n)
	{
		unsigned short *color16_dst = static_cast<unsigned short*>(color_pointer(x, y));
		unsigned short *depth16_dst = static_cast<unsigned short*>(depth_pointer(x, y));

		// this buffer holds information about whether or not to kill a fragment
		if (color_write || depth_write ) std::fill_n(ctx->span_buffers[4].begin(), n, 0);

		if (color_write) {
			int base = 0;
			
			if (interp_color) {
				color_span(&ctx->span_buffers[0][0], fd, step, base, n);
				base += 3;
			} else {
				if (!interp_texcoord || ctx->gl_state.texture_units[0].texture_env_mode != GL_REPLACE)
					std::fill_n(ctx->span_buffers[0].begin(), n, ctx->derived_state.current_color);
			}

			if (interp_alpha) {
				alpha_span(&ctx->span_buffers[1][0], fd, step, base, n);
				base++;
			} else {
				if (!interp_texcoord || ctx->gl_state.texture_units[0].texture_env_mode != GL_REPLACE)
					std::fill_n(ctx->span_buffers[1].begin(), n, ctx->derived_state.current_alpha);
			}

			if (interp_alpha) base++;
			
			if (interp_texcoord) {
				const GLTextureObject *tex = ctx->gl_state.texture_state.binding_2d_tex;
				
				assert(tex->complete());
				assert(ctx->gl_state.texture_units[0].texture_2d_enable);

				ctx->derived_state.texture_span(
					tex, 
					&ctx->span_buffers[2][0],
					&ctx->span_buffers[3][0],
					fd, step, base, n);

				ctx->derived_state.texture_env_span(
					ctx,
					&ctx->span_buffers[0][0],
					&ctx->span_buffers[1][0],
					&ctx->span_buffers[2][0],
					&ctx->span_buffers[3][0],
					n);

				base += 2;
			}

			if (interp_fog) {
				assert(ctx->gl_state.coloring.fog_enable);
				const unsigned short &fog_color = ctx->derived_state.fog_color;
				fog_span(&ctx->span_buffers[0][0], fog_color, fd, step, base, n);
				base++;
			}

			// blending
			if (ctx->gl_state.pixel_op.blend.enable) {
				ctx->derived_state.blend_span(
					ctx,
					&ctx->span_buffers[0][0],
					&ctx->span_buffers[1][0],
					color16_dst,
					n);
			}

			ctx->derived_state.alpha_test_span(ctx, &ctx->span_buffers[1][0], &ctx->span_buffers[4][0], n);
		}

		if (interp_z) {
			depth_span(&ctx->span_buffers[2][0], fd, step, n);
			ctx->derived_state.depth_test_span(
				ctx,
				&ctx->span_buffers[2][0], 
				depth16_dst, 
				&ctx->span_buffers[4][0],
				n);
		}

		// write to output
		const unsigned short *kill_fragment = &ctx->span_buffers[4][0];
		for (unsigned i = 0; i < n; ++i) {
			if (!*kill_fragment++) {
				if (color_write) {
					assert(ctx->derived_state.color_writemask == 0xffff);
					*color16_dst = ctx->span_buffers[0][i];
				}

				if (depth_write) {
					assert(ctx->gl_state.framebuf_ctrl.depth_writemask);
					*depth16_dst = ctx->span_buffers[2][i]; 
				}
			}

			color16_dst++;
			if (depth_write) depth16_dst++;
		}
	}

	static void* color_pointer(int x, int y)
	{
		return static_cast<unsigned short*>(ctx->color_buffer.data)
			+ x + y * ctx->color_buffer.pitch / 2;
	}

	static void* depth_pointer(int x, int y)
	{
		return static_cast<unsigned short*>(ctx->depth_buffer.data)
			+ x + y * ctx->depth_buffer.pitch / 2;
	}

	static F2X_Context *ctx;
};

// instantiate static class members

template
<
	bool interp_color,
	bool interp_alpha,
	bool interp_texcoord,
	bool interp_fog,
	bool interp_z,
	bool color_write,
	bool depth_write
>
F2X_Context *OptimizedFragmentShader
<
	interp_color, 
	interp_alpha, 
	interp_texcoord, 
	interp_fog, 
	interp_z,
	color_write,
	depth_write
>::ctx;

#endif
