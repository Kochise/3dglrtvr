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

#ifndef GENERIC_VERTEX_SHADER_H_
#define GENERIC_VERTEX_SHADER_H_

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "../context.h"
#include "util.h"

#include <cmath>

struct GenericVertexShader {
	static const unsigned attribute_count = 4;
	static const unsigned varying_count = 7;

	static void shade(const swr::GeometryProcessor::VertexInput in, swr::GeometryProcessor::VertexOutput &out)
	{
		// fetch vertex
		vec4x v = ctx->derived_state.fetch_vertex(in[0]);

		// fetch texture coordinate and transform it by the current texture matrix
		vec4x tc = ctx->derived_state.fetch_texcoord(in[3], 0);
		const int st = ctx->gl_state.transform_state.texture_stack_depth[0] - 1;
		tc = ctx->gl_state.transform_state.texture_matrix[0][st] * tc;

		vec4x c = ctx->derived_state.fetch_color(in[1]);

		// eye space vertex
		vec4x ve;

		// only compute the eye space vertex when needed
		if (ctx->gl_state.lighting.enable || ctx->gl_state.coloring.fog_enable) {
			const int top = ctx->gl_state.transform_state.modelview_stack_depth - 1;
			ve = ctx->gl_state.transform_state.modelview_matrix[top] * v;
			if (ve.w != 1 && ve.w != 0) 
				ve /= ve.w;
		}

		// lighting
		if (ctx->gl_state.lighting.enable) {
			vec3x n = ctx->derived_state.fetch_normal(in[2]);
			c = clamp(compute_lighting(ctx, c, n, ve));
		}

		fixed16_t fog = 0;

		// handle fog
		if (ctx->gl_state.coloring.fog_enable)
			fog = compute_fog(ctx, ve);

		// transform and write position
		v = ctx->derived_state.modelview_projection_matrix * v;

		out.x = v.x.intValue;
		out.y = v.y.intValue;
		out.z = v.z.intValue;
		out.w = v.w.intValue;

		// write color
		out.varyings[0] = c[0].intValue * 31;
		out.varyings[1] = c[1].intValue * 63;
		out.varyings[2] = c[2].intValue * 31;
		out.varyings[3] = c[3].intValue * 31;

		// write texture coordinates
		const GLTextureObject *tex = ctx->gl_state.texture_state.binding_2d_tex;

		// write texture coordinates and account for the y flip from OpenGL to the internal renderer
		out.varyings[4] = tc[0].intValue * tex->width_minus_one;
		out.varyings[5] = ((1 << 16) - tc[1].intValue) * tex->height_minus_one;

		// write fog
		out.varyings[6] = fog.intValue * 31;
	}

	static const F2X_Context *ctx;
};

#endif
