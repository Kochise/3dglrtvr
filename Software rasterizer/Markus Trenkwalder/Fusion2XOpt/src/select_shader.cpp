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
#include "shaders/optimized/spanfuncs.h"
#include "shaders/optimized/vertex_shader.h"
#include "shaders/optimized/fragment_shader.h"

#if 0
OptimizedVertexShader<true, true, true, true>::ctx = ctx;
OptimizedFragmentShader<true, true, true, true, true>::ctx = ctx;

ctx->geometry_processor.vertex_shader<OptimizedVertexShader<true, true, true, true> >();
ctx->rasterizer.fragment_shader<OptimizedFragmentShader<true, true, true, true, true> >();

{
	const GLTextureObject *tex = ctx->gl_state.texture_state.binding_2d_tex;
	OptimizedSpanFunctions::TextureSpanDesc desc;
	desc.width = tex->width;
	desc.height = tex->height;
	desc.texture_wrap_s = tex->texture_wrap_s;
	desc.texture_wrap_t = tex->texture_wrap_t;
	desc.internal_format = tex->internalformat;
	texture_span = OptimizedSpanFunctions::get_texture_span(desc);
}

{
	OptimizedSpanFunctions::TextureEnvSpanDesc desc;
	desc.do_alpha = false;
	desc.texture_env_mode = ctx->gl_state.texture_units[0].texture_env_mode;
	texture_env_span = OptimizedSpanFunctions::get_texture_env_span(desc);
}

{
	GLenum func = ctx->gl_state.pixel_op.alpha_test.func;
	// a disabled alpha test behaves just like GL_ALWAYS
	if (!ctx->gl_state.pixel_op.alpha_test.enable) func = GL_ALWAYS;
	alpha_test_span = OptimizedSpanFunctions::get_alpha_test_span(func);
}

{
	GLenum func = ctx->gl_state.pixel_op.depth_test.func;
	if (!ctx->gl_state.pixel_op.depth_test.enable) func = GL_ALWAYS;
	depth_test_span = OptimizedSpanFunctions::get_depth_test_span(func);
}

{
	OptimizedSpanFunctions::BlendSpanDesc desc;
	desc.src_factor = ctx->gl_state.pixel_op.blend.src;
	desc.dst_factor = ctx->gl_state.pixel_op.blend.dst;
	blend_span = OptimizedSpanFunctions::get_blend_span(desc);
}

#endif

namespace
{

#define OP_HELPER(A) \
	A < rhs.A || (x = x && A == rhs.A) &&
#define OP_HELPER_LAST(A) \
	A < rhs.A;

	struct VertexShaderDesc {
		bool interp_color;
		bool interp_alpha;
		bool interp_texcoord;
		bool interp_fog;

		bool operator < (const VertexShaderDesc &rhs) const
		{
			bool x = true;

			return
				OP_HELPER(interp_color)
				OP_HELPER(interp_alpha)
				OP_HELPER(interp_texcoord)
				OP_HELPER_LAST(interp_fog)
		}
	};

	struct FragmentShaderDesc {
		bool interp_color;
		bool interp_alpha;
		bool interp_texcoord;
		bool interp_fog;
		bool interp_z;
		bool color_write;
		bool depth_write;

		bool operator < (const FragmentShaderDesc &rhs) const
		{
			bool x = true;

			return
				OP_HELPER(interp_color)
				OP_HELPER(interp_alpha)
				OP_HELPER(interp_texcoord)
				OP_HELPER(interp_fog)
				OP_HELPER(interp_z)
				OP_HELPER(color_write)
				OP_HELPER_LAST(depth_write)
		}
	};

#undef OP_HELPER
#undef OP_HELPER_LAST

	template
	<
		bool interp_color,
		bool interp_alpha,
		bool interp_texcoord,
		bool interp_fog
	>
	void vertex_shader_apply(F2X_Context *ctx)
	{
		typedef OptimizedVertexShader
			<
				interp_color, 
				interp_alpha, 
				interp_texcoord, 
				interp_fog
			> 
		vertex_shader_t;

		vertex_shader_t::ctx = ctx;
		ctx->geometry_processor.vertex_shader<vertex_shader_t>();			
	}

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
	void fragment_shader_apply(F2X_Context *ctx)
	{
		typedef OptimizedFragmentShader
			<
				interp_color,
				interp_alpha,
				interp_texcoord,
				interp_fog,
				interp_z,
				color_write,
				depth_write
			>
		fragment_shader_t;
		
		fragment_shader_t::ctx = ctx;
		ctx->rasterizer.fragment_shader<fragment_shader_t>();
	}

	typedef void (*shader_apply_function_t)(F2X_Context *ctx);
	
	std::map<VertexShaderDesc, shader_apply_function_t> vertex_shaders;
	std::map<FragmentShaderDesc, shader_apply_function_t> fragment_shaders;

#define ADD_VERTEX_SHADER_SPECIALIZATION(A, B, C, D) \
	do \
	{ \
		VertexShaderDesc desc; \
		desc.interp_color = A; \
		desc.interp_alpha = B; \
		desc.interp_texcoord = C; \
		desc.interp_fog = D; \
		vertex_shaders[desc] = &vertex_shader_apply<A, B, C, D>; \
	} while (0)

#define ADD_FRAGMENT_SHADER_SPECIALIZATION(A, B, C, D, E, F, G) \
	do \
	{ \
		FragmentShaderDesc desc; \
		desc.interp_color = A; \
		desc.interp_alpha = B; \
		desc.interp_texcoord = C; \
		desc.interp_fog = D; \
		desc.interp_z = E; \
		desc.color_write = F; \
		desc.depth_write = G; \
		fragment_shaders[desc] = &fragment_shader_apply<A, B, C, D, E, F, G>; \
	} while (0)

	struct Init {
		Init()
		{
			ADD_VERTEX_SHADER_SPECIALIZATION(false, false, false, false);
			ADD_VERTEX_SHADER_SPECIALIZATION(false, false, false, true);
			ADD_VERTEX_SHADER_SPECIALIZATION(false, false, true, false);
			ADD_VERTEX_SHADER_SPECIALIZATION(false, false, true, true);
			ADD_VERTEX_SHADER_SPECIALIZATION(false, true, false, false);
			ADD_VERTEX_SHADER_SPECIALIZATION(false, true, false, true);
			ADD_VERTEX_SHADER_SPECIALIZATION(false, true, true, false);
			ADD_VERTEX_SHADER_SPECIALIZATION(false, true, true, true);
			ADD_VERTEX_SHADER_SPECIALIZATION(true, false, false, false);
			ADD_VERTEX_SHADER_SPECIALIZATION(true, false, false, true);
			ADD_VERTEX_SHADER_SPECIALIZATION(true, false, true, false);
			ADD_VERTEX_SHADER_SPECIALIZATION(true, false, true, true);
			ADD_VERTEX_SHADER_SPECIALIZATION(true, true, false, false);
			ADD_VERTEX_SHADER_SPECIALIZATION(true, true, false, true);
			ADD_VERTEX_SHADER_SPECIALIZATION(true, true, true, false);
			ADD_VERTEX_SHADER_SPECIALIZATION(true, true, true, true);

			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, false, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, false, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, false, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, false, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, false, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, false, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, false, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, false, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, true, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, true, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, true, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, true, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, true, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, true, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, true, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, false, true, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, false, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, false, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, false, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, false, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, false, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, false, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, false, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, false, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, true, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, true, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, true, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, true, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, true, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, true, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, true, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, false, true, true, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, false, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, false, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, false, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, false, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, false, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, false, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, false, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, false, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, true, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, true, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, true, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, true, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, true, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, true, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, true, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, false, true, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, false, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, false, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, false, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, false, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, false, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, false, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, false, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, false, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, true, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, true, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, true, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, true, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, true, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, true, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, true, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(false, true, true, true, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, false, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, false, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, false, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, false, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, false, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, false, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, false, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, false, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, true, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, true, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, true, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, true, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, true, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, true, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, true, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, false, true, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, false, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, false, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, false, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, false, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, false, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, false, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, false, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, false, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, true, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, true, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, true, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, true, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, true, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, true, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, true, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, false, true, true, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, false, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, false, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, false, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, false, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, false, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, false, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, false, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, false, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, true, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, true, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, true, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, true, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, true, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, true, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, true, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, false, true, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, false, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, false, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, false, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, false, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, false, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, false, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, false, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, false, true, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, true, false, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, true, false, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, true, false, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, true, false, true, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, true, true, false, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, true, true, false, true);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, true, true, true, false);
			ADD_FRAGMENT_SHADER_SPECIALIZATION(true, true, true, true, true, true, true);
		}
	} init;

#undef ADD_VERTEX_SHADER_SPECIALIZATION
#undef ADD_FRAGMENT_SHADER_SPECIALIZATION
}

bool F2X_Context::select_optimized_shaders()
{
	// reject incompatible states
	if (gl_state.pixel_op.color_logic_op_enable) return false;

	{
		const GLTextureObject *tex = gl_state.texture_state.binding_2d_tex;
		OptimizedSpanFunctions::TextureSpanDesc desc;
		desc.width = tex->width;
		desc.height = tex->height;
		desc.texture_wrap_s = tex->texture_wrap_s;
		desc.texture_wrap_t = tex->texture_wrap_t;
		desc.internal_format = tex->internalformat;
		derived_state.texture_span = OptimizedSpanFunctions::get_texture_span(desc);
	}

	{
		OptimizedSpanFunctions::TextureEnvSpanDesc desc;
		desc.do_alpha = true;
		desc.texture_env_mode = gl_state.texture_units[0].texture_env_mode;
		derived_state.texture_env_span = OptimizedSpanFunctions::get_texture_env_span(desc);
	}

	{
		GLenum func = gl_state.pixel_op.alpha_test.func;
		// a disabled alpha test behaves just like GL_ALWAYS
		if (!gl_state.pixel_op.alpha_test.enable) func = GL_ALWAYS;
		derived_state.alpha_test_span = OptimizedSpanFunctions::get_alpha_test_span(func);
	}

	{
		GLenum func = gl_state.pixel_op.depth_test.func;
		if (!gl_state.pixel_op.depth_test.enable) func = GL_ALWAYS;
		derived_state.depth_test_span = OptimizedSpanFunctions::get_depth_test_span(func);
	}

	{
		OptimizedSpanFunctions::BlendSpanDesc desc;
		desc.src_factor = gl_state.pixel_op.blend.src;
		desc.dst_factor = gl_state.pixel_op.blend.dst;
		derived_state.blend_span = OptimizedSpanFunctions::get_blend_span(desc);
	}

	// TODO: better heuristics for when to enable/disable interpolants
	// (especially for the alpha interpolant)
	bool interp_color = gl_state.color_array.enable == GL_TRUE;
	bool interp_alpha = interp_color;
	bool interp_texcoord = gl_state.texture_units[0].texture_2d_enable == GL_TRUE;
	bool interp_fog = gl_state.coloring.fog_enable == GL_TRUE;

	VertexShaderDesc vdesc;
	vdesc.interp_color = interp_color;
	vdesc.interp_alpha = interp_alpha;
	vdesc.interp_texcoord = interp_texcoord;
	vdesc.interp_fog = interp_fog;
	vertex_shaders[vdesc](this);

	FragmentShaderDesc fdesc;
	fdesc.interp_color = interp_color;
	fdesc.interp_alpha = interp_alpha;
	fdesc.interp_texcoord = interp_texcoord;
	fdesc.interp_fog = interp_fog;
	fdesc.interp_z = true;
	fdesc.color_write = true;
	fdesc.depth_write = true;
	fragment_shaders[fdesc](this);

	return true;
}
