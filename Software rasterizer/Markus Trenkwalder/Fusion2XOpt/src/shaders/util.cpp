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

#include "util.h"

namespace 
{
	inline vec3x direction(const vec4x &from, const vec4x &to)
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

	inline fixed16_t lit(const vec3x &n, const vec3x &l)
	{
		return std::max(dot(n, l), fixed16_t(0));
	}
}

vec4x compute_lighting(
	const F2X_Context *ctx,
	const vec4x &color,
	const vec3x &normal, 
	const vec4x &v_eye)
{
	const F2X_Context::GLState::Lighting::MaterialParams &m = 
		ctx->gl_state.lighting.material_params[0];

	// check for color material
	const vec4x ambient = ctx->gl_state.lighting.color_material_enable ? color : m.ambient;
	const vec4x diffuse = ctx->gl_state.lighting.color_material_enable ? color : m.diffuse;

	// accumulate lit color into this variable
	vec4x accumulated_lit_color = m.emission + ambient * ctx->gl_state.lighting.light_model_ambient;
	accumulated_lit_color.w = diffuse.w; // alpha channel taken from material

	for (int i = 0; i < ctx->gl_state.implementation.MAX_LIGHTS; ++i) {
		if (!ctx->gl_state.lights[i].enable) continue;

		const F2X_Context::GLState::Light &l = ctx->gl_state.lights[i];

		// direction vector to light
		vec3x vpl = direction(v_eye, l.position);

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
		const fixed16_t ndotl = lit(normal, vpl);

		// compute diffuse component
		vec3x light = 
			vec3x(ambient) *  vec3x(l.ambient) + 
			ndotl * vec3x(diffuse) * vec3x(l.diffuse);

		// compute specular component (potentially expensive)
		if (ndotl > 0 && m.specular != vec4x(0) && l.specular != vec4x(0)) {
			const vec3x h = normalize(vec3x(vpl.x, vpl.y, vpl.z + 1));

			fixed16_t spec = pow(
				fixedpoint::fix2float<16>(lit(normal, h).intValue), 
				fixedpoint::fix2float<16>(l.spot_exponent)
				);

			light += spec * vec3x(m.specular) * vec3x(l.specular);
		}

		light *= att * spot;

		accumulated_lit_color.x += light.x;
		accumulated_lit_color.y += light.y;
		accumulated_lit_color.z += light.z;
	}

	return accumulated_lit_color;
}

fixed16_t compute_fog(const F2X_Context *ctx, vec4x &v_eye)
{
	fixed16_t s; s.intValue = ctx->gl_state.coloring.fog_start;
	fixed16_t e; e.intValue = ctx->gl_state.coloring.fog_end;
	fixed16_t d; d.intValue = ctx->gl_state.coloring.fog_density;

	fixed16_t fog;

	switch (ctx->gl_state.coloring.fog_mode) {
		case GL_LINEAR: 
			{
				fixed16_t tmp; tmp.intValue = swr::detail::invert((e - s).intValue);
				fog = (e + v_eye.z) * tmp;
				break;
			}
		case GL_EXP:
			{
				fog.intValue = 
					fixedpoint::float2fix<16>(std::exp(fixedpoint::fix2float<16>((d * v_eye.z).intValue)));
				break;
			}
		case GL_EXP2:
			{
				fixed16_t i = (d * -v_eye.z); i *= i; i = -i;
				fog.intValue = fixedpoint::float2fix<16>(std::exp(fixedpoint::fix2float<16>(i.intValue)));
				break;
			}
	}

	// clamp fog
	return std::min(std::max(fog, fixed16_t(0)), fixed16_t(1));
}
