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

#ifndef OPTIMIZED_VERTEX_SHADER_H
#define OPTIMIZED_VERTEX_SHADER_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "../../context.h"
#include "../util.h"

template
<	
	bool interp_color,
	bool interp_alpha,
	bool interp_texcoord,
	bool interp_fog
>
struct OptimizedVertexShader {
	static const unsigned attribute_count = 4;
	static const unsigned varying_count = 
		(interp_color ? 3 : 0) +
		(interp_alpha ? 1 : 0) + 
		(interp_texcoord ? 2 : 0) + 
		(interp_fog ? 1 : 0);
		
	static void shade(const swr::GeometryProcessor::VertexInput in, swr::GeometryProcessor::VertexOutput &out)
	{
		vec4x vertex;
		vec4x color;
		vec4x texcoord;
		fixed16_t fog;

		// fetch vertex
		vertex = ctx->derived_state.fetch_vertex(in[0]);

		// fetch texture coordinate and transform it by the current texture matrix
		if (interp_texcoord) {
			texcoord = ctx->derived_state.fetch_texcoord(in[3], 0);
			const int st = ctx->gl_state.transform_state.texture_stack_depth[0] - 1;
			texcoord = ctx->gl_state.transform_state.texture_matrix[0][st] * texcoord;
		}

		if (interp_color)
			color = ctx->derived_state.fetch_color(in[1]);

		// eye space vertex
		vec4x ve;

		// only compute the eye space vertex when needed
		if ((interp_color && ctx->gl_state.lighting.enable) || 
			(interp_fog && ctx->gl_state.coloring.fog_enable)) 
		{
			const int top = ctx->gl_state.transform_state.modelview_stack_depth - 1;
			ve = ctx->gl_state.transform_state.modelview_matrix[top] * vertex;
			if (ve.w != 1 && ve.w != 0) 
				ve /= ve.w;
		}

		// lighting
		if (interp_color && ctx->gl_state.lighting.enable) {
			vec3x normal = ctx->derived_state.fetch_normal(in[2]);
			color = clamp(compute_lighting(ctx, color, normal, ve));
		}

		// handle fog
		if (interp_fog) {
			assert(ctx->gl_state.coloring.fog_enable);
			fog = compute_fog(ctx, ve);
		}

		// transform and write position
		vertex = ctx->derived_state.modelview_projection_matrix * vertex;

		out.x = vertex.x.intValue;
		out.y = vertex.y.intValue;
		out.z = vertex.z.intValue;
		out.w = vertex.w.intValue;

		int base = 0;

		if (interp_color) {
			out.varyings[base + 0] = color[0].intValue * 31;
			out.varyings[base + 1] = color[1].intValue * 63;
			out.varyings[base + 2] = color[2].intValue * 31;
			base += 3;
		}

		if (interp_alpha) {
			out.varyings[base + 0] = color[3].intValue * 31;
			base++;
		}

		if (interp_texcoord) {
			const GLTextureObject *tex = ctx->gl_state.texture_state.binding_2d_tex;

			// write texture coordinates and account for the y flip from OpenGL to the internal renderer
			out.varyings[base + 0] = texcoord[0].intValue * tex->width_minus_one;
			out.varyings[base + 1] = ((1 << 16) - texcoord[1].intValue) * tex->height_minus_one;

			base += 2;
		}

		if (interp_fog) {
			out.varyings[base + 0] = fog.intValue * 31;
			base++;
		}
	}

	static const F2X_Context *ctx;
};

// instantiate static class members

template
<
	bool interp_color,
	bool interp_alpha,
	bool interp_texcoord,
	bool interp_fog
>
const F2X_Context *OptimizedVertexShader
<
	interp_color, 
	interp_alpha, 
	interp_texcoord, 
	interp_fog
>::ctx;

#endif
