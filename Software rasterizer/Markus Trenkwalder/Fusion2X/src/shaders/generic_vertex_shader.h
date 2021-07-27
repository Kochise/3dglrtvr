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

			// fetch normal needed for lighting
			const vec3x n = ctx->derived_state.fetch_normal(in[2]);

			const F2X_Context::GLState::Lighting::MaterialParams &m = 
				ctx->gl_state.lighting.material_params[0];

			// check for color material
			const vec4x ambient = ctx->gl_state.lighting.color_material_enable ? c : m.ambient;
			const vec4x diffuse = ctx->gl_state.lighting.color_material_enable ? c : m.diffuse;

			// accumulate lit color into this variable
			vec4x accumulated_lit_color = m.emission + ambient * ctx->gl_state.lighting.light_model_ambient;
			accumulated_lit_color.w = diffuse.w; // alpha channel taken from material

			for (int i = 0; i < ctx->gl_state.implementation.MAX_LIGHTS; ++i) {
				if (!ctx->gl_state.lights[i].enable) continue;

				const F2X_Context::GLState::Light &l = ctx->gl_state.lights[i];

				// direction vector to light
				vec3x vpl = direction(ve, l.position);

				// attenuation factor
				fixed16_t att = 1;
				if (l.position.w != 0 && (
					l.constant_attenuation != 1 || 
					l.linear_attenuation != 0 || 
					l.quadratic_attenuation != 0))
				{
					const fixed16_t len2 = dot(vpl, vpl);
					att = l.constant_attenuation +
						l.linear_attenuation * sqrt(len2) +
						l.quadratic_attenuation * len2;
					att.intValue = swr::detail::invert(att.intValue);
				}

				// spotlight factor
				fixed16_t spot = 1;
				if (l.spot_cutoff != (180 << 16)) {
					const fixed16_t i = lit(-vpl, l.spot_direction);
					const fixed16_t c = fixedpoint::fixcos16(
						fixedpoint::fixmul<16>(
							l.spot_cutoff, 
							static_cast<int>((3.141592f / 180.0f) * 65536)
						)
					);

					if (i >= c) spot = i;
					else spot = 0;
				}

				// lambert shading
				const fixed16_t ndotl = lit(n, vpl);

				// compute diffuse component
				vec3x light = 
					vec3x(ambient) *  vec3x(l.ambient) + 
					ndotl * vec3x(diffuse) * vec3x(l.diffuse);

				// compute specular component (potentially expensive)
				if (ndotl > 0 && m.specular != vec4x(0) && l.specular != vec4x(0)) {
					const vec3x h = normalize(vec3x(vpl.x, vpl.y, vpl.z + 1));

					fixed16_t spec = pow(
						fixedpoint::fix2float<16>(lit(n, h).intValue), 
						fixedpoint::fix2float<16>(l.spot_exponent)
					);
				
					light += spec * vec3x(m.specular) * vec3x(l.specular);
				}

				light *= att * spot;

				accumulated_lit_color.x += light.x;
				accumulated_lit_color.y += light.y;
				accumulated_lit_color.z += light.z;
			}

			// copy lighting result to output color
			c = accumulated_lit_color;
		}

		c = clamp(c);

		fixed16_t fog;

		// handle fog
		if (ctx->gl_state.coloring.fog_enable) {
			fixed16_t s; s.intValue = ctx->gl_state.coloring.fog_start;
			fixed16_t e; e.intValue = ctx->gl_state.coloring.fog_end;
			fixed16_t d; d.intValue = ctx->gl_state.coloring.fog_density;

			switch (ctx->gl_state.coloring.fog_mode) {
			case GL_LINEAR: 
				{
					fixed16_t tmp; tmp.intValue = swr::detail::invert((e - s).intValue);
					fog = (e + ve.z) * tmp;
					break;
				}
			case GL_EXP:
				{
					fog.intValue = 
						fixedpoint::float2fix<16>(std::exp(fixedpoint::fix2float<16>((d * ve.z).intValue)));
					break;
				}
			case GL_EXP2:
				{
					fixed16_t i = (d * -ve.z); i *= i; i = -i;
					fog.intValue = fixedpoint::float2fix<16>(std::exp(fixedpoint::fix2float<16>(i.intValue)));
					break;
				}
			}

			// clamp fog
			fog = std::min(std::max(fog, fixed16_t(0)), fixed16_t(1));
		}

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

	static inline vec3x direction(const vec4x &from, const vec4x &to)
	{
		if (to.w == 0 && from.w != 0) return normalize(vec3x(to));
		else if (from.w == 0 && to.w != 0) return normalize(-vec3x(from));
		else {
			vec3x pp1 = vec3x(from);
			vec3x pp2 = vec3x(to);
			if (from.w != 1 && from.w != 0) pp1 /= from.w;
			if (to.w != 1 && to.w != 0) pp2 /= to.w;
			return normalize(pp2 - pp1);
		}
	}

	static inline fixed16_t lit(const vec3x &n, const vec3x &l)
	{
		return std::max(dot(n, l), fixed16_t(0));
	}

	static inline vec4x clamp(const vec4x &in)
	{
		vec4x r;
		r.x = std::min(std::max(in.x, fixed16_t(0)), fixed16_t(1));
		r.y = std::min(std::max(in.y, fixed16_t(0)), fixed16_t(1));
		r.z = std::min(std::max(in.z, fixed16_t(0)), fixed16_t(1));
		r.w = std::min(std::max(in.w, fixed16_t(0)), fixed16_t(1));
		return r;
	}

	static const F2X_Context *ctx;
};

#endif
