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

void F2X_Context::glAlphaFuncx(GLenum func, GLclampx ref)
{
	switch (func) {
	case GL_NEVER:
	case GL_LESS:
	case GL_EQUAL:
	case GL_LEQUAL:
	case GL_GREATER:
	case GL_NOTEQUAL:
	case GL_GEQUAL:
	case GL_ALWAYS:
		gl_state.pixel_op.alpha_test.func = func;
		gl_state.pixel_op.alpha_test.ref = clamp_fixed(ref);
		break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}
}

void F2X_Context::glBlendFunc(GLenum sfactor, GLenum dfactor)
{
	switch (sfactor) {
	case GL_ZERO:
	case GL_ONE:
	case GL_DST_COLOR:
	case GL_ONE_MINUS_DST_COLOR:
	case GL_SRC_ALPHA_SATURATE:
	case GL_SRC_ALPHA:
	case GL_ONE_MINUS_SRC_ALPHA:
	case GL_DST_ALPHA:
	case GL_ONE_MINUS_DST_ALPHA:
		break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}

	switch (dfactor) {
	case GL_ZERO:
	case GL_ONE:
	case GL_SRC_COLOR:
	case GL_ONE_MINUS_SRC_COLOR:
	case GL_SRC_ALPHA:
	case GL_ONE_MINUS_SRC_ALPHA:
	case GL_DST_ALPHA:
	case GL_ONE_MINUS_DST_ALPHA:
		break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}

	// only set state when both parameters are correct
	gl_state.pixel_op.blend.src = sfactor;
	gl_state.pixel_op.blend.dst = dfactor;
}

void F2X_Context::glColor4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
	gl_state.current.color[0].intValue = clamp_fixed(red);
	gl_state.current.color[1].intValue = clamp_fixed(green);
	gl_state.current.color[2].intValue = clamp_fixed(blue);
	gl_state.current.color[3].intValue = clamp_fixed(alpha);

	derived_state.update_current_color();
}

void F2X_Context::glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	gl_state.framebuf_ctrl.color_writemask[0] = red;
	gl_state.framebuf_ctrl.color_writemask[1] = green;
	gl_state.framebuf_ctrl.color_writemask[2] = blue;
	gl_state.framebuf_ctrl.color_writemask[3] = alpha;

	derived_state.update_color_writemask();
}

void F2X_Context::glCullFace(GLenum mode)
{
	switch (mode) {
	case GL_FRONT:
	case GL_BACK:
	case GL_FRONT_AND_BACK:
		gl_state.rasterization.cull_face_mode = mode;
		update_culling();
		break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}	
}

void F2X_Context::glDepthFunc(GLenum func)
{
	switch (func) {
	case GL_NEVER:
	case GL_LESS:
	case GL_EQUAL:
	case GL_LEQUAL:
	case GL_GREATER:
	case GL_NOTEQUAL:
	case GL_GEQUAL:
	case GL_ALWAYS:
		gl_state.pixel_op.depth_test.func = func;
		return;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}
}

void F2X_Context::glDepthMask(GLboolean flag)
{
	gl_state.framebuf_ctrl.depth_writemask = flag;
}

void F2X_Context::glEnable(GLenum cap)
{
	switch (cap) {
	// state for EnableClientState
	case GL_TEXTURE_COORD_ARRAY: 
		{
			int at = gl_state.client_active_texture - GL_TEXTURE0;
			gl_state.texture_coord_array[at].enable = GL_TRUE; 
			return;
		}
	case GL_VERTEX_ARRAY: gl_state.vertex_array.enable = GL_TRUE; return;
	case GL_NORMAL_ARRAY: gl_state.normal_array.enable = GL_TRUE; return;
	case GL_COLOR_ARRAY: gl_state.color_array.enable = GL_TRUE; return;

	// other state
	case GL_NORMALIZE: gl_state.transform_state.normalize_enable = GL_TRUE; return;
	case GL_RESCALE_NORMAL: gl_state.transform_state.rescale_normal_enable = GL_TRUE; return;
	case GL_FOG: gl_state.coloring.fog_enable = GL_TRUE; return;
	case GL_LIGHTING: gl_state.lighting.enable = GL_TRUE; return;
	case GL_COLOR_MATERIAL: gl_state.lighting.color_material_enable = GL_TRUE; return;
	case GL_LIGHT0: gl_state.lights[0].enable = GL_TRUE; return;
	case GL_LIGHT1: gl_state.lights[1].enable = GL_TRUE; return;
	case GL_LIGHT2: gl_state.lights[2].enable = GL_TRUE; return;
	case GL_LIGHT3: gl_state.lights[3].enable = GL_TRUE; return;
	case GL_LIGHT4: gl_state.lights[4].enable = GL_TRUE; return;
	case GL_LIGHT5: gl_state.lights[5].enable = GL_TRUE; return;
	case GL_LIGHT6: gl_state.lights[6].enable = GL_TRUE; return;
	case GL_LIGHT7: gl_state.lights[7].enable = GL_TRUE; return;
	case GL_POINT_SMOOTH: gl_state.rasterization.point_smooth_enable = GL_TRUE; return;
	case GL_LINE_SMOOTH: gl_state.rasterization.line_smooth_enable = GL_TRUE; return;
	case GL_POLYGON_OFFSET_FILL: gl_state.rasterization.polygon_offset.fill_enable = GL_TRUE; return;
	case GL_MULTISAMPLE: gl_state.multisampling.enable = GL_TRUE; return;
	case GL_SAMPLE_ALPHA_TO_COVERAGE: gl_state.multisampling.sample_alpha_to_coverage_enable = GL_TRUE; return;
	case GL_SAMPLE_ALPHA_TO_ONE: gl_state.multisampling.sample_alpha_to_one_enable = GL_TRUE; return;
	case GL_SAMPLE_COVERAGE: gl_state.multisampling.sample_coverage_enable = GL_TRUE; return;
	case GL_TEXTURE_2D: gl_state.texture_units[gl_state.active_texture - GL_TEXTURE0].texture_2d_enable = GL_TRUE; return;
	case GL_ALPHA_TEST: gl_state.pixel_op.alpha_test.enable = GL_TRUE; return;
	case GL_STENCIL_TEST: gl_state.pixel_op.stencil_test.enable = GL_TRUE; return;
	case GL_DEPTH_TEST: gl_state.pixel_op.depth_test.enable = GL_TRUE; return;
	case GL_BLEND: gl_state.pixel_op.blend.enable = GL_TRUE; return;
	case GL_DITHER: gl_state.pixel_op.dither_enable = GL_TRUE; return;
	case GL_COLOR_LOGIC_OP: gl_state.pixel_op.color_logic_op_enable = GL_TRUE; return;
	case GL_SCISSOR_TEST: 
		gl_state.pixel_op.scissor.test_enable = GL_TRUE; 
		update_scissor();
		return;
	case GL_CULL_FACE: 
		gl_state.rasterization.cull_face_enable = GL_TRUE; 
		update_culling();
		return;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}	
}

void F2X_Context::glDisable(GLenum cap)
{
	switch (cap) {
	// state for DisableClientState
	case GL_TEXTURE_COORD_ARRAY:
		{
			int at = gl_state.client_active_texture - GL_TEXTURE0;
			gl_state.texture_coord_array[at].enable = GL_FALSE; 
			return;
		}
	case GL_COLOR_ARRAY: gl_state.color_array.enable = GL_FALSE; return;
	case GL_NORMAL_ARRAY: gl_state.normal_array.enable = GL_FALSE; return;
	case GL_VERTEX_ARRAY: gl_state.vertex_array.enable = GL_FALSE; return;

	// other state
	case GL_NORMALIZE: gl_state.transform_state.normalize_enable = GL_FALSE; return;
	case GL_RESCALE_NORMAL: gl_state.transform_state.rescale_normal_enable = GL_FALSE; return;
	case GL_FOG: gl_state.coloring.fog_enable = GL_FALSE; return;
	case GL_LIGHTING: gl_state.lighting.enable = GL_FALSE; return;
	case GL_COLOR_MATERIAL: gl_state.lighting.color_material_enable = GL_FALSE; return;
	case GL_LIGHT0: gl_state.lights[0].enable = GL_FALSE; return;
	case GL_LIGHT1: gl_state.lights[1].enable = GL_FALSE; return;
	case GL_LIGHT2: gl_state.lights[2].enable = GL_FALSE; return;
	case GL_LIGHT3: gl_state.lights[3].enable = GL_FALSE; return;
	case GL_LIGHT4: gl_state.lights[4].enable = GL_FALSE; return;
	case GL_LIGHT5: gl_state.lights[5].enable = GL_FALSE; return;
	case GL_LIGHT6: gl_state.lights[6].enable = GL_FALSE; return;
	case GL_LIGHT7: gl_state.lights[7].enable = GL_FALSE; return;
	case GL_POINT_SMOOTH: gl_state.rasterization.point_smooth_enable = GL_FALSE; return;
	case GL_LINE_SMOOTH: gl_state.rasterization.line_smooth_enable = GL_FALSE; return;
	case GL_POLYGON_OFFSET_FILL: gl_state.rasterization.polygon_offset.fill_enable = GL_FALSE; return;
	case GL_MULTISAMPLE: gl_state.multisampling.enable = GL_FALSE; return;
	case GL_SAMPLE_ALPHA_TO_COVERAGE: gl_state.multisampling.sample_alpha_to_coverage_enable = GL_FALSE; return;
	case GL_SAMPLE_ALPHA_TO_ONE: gl_state.multisampling.sample_alpha_to_one_enable = GL_FALSE; return;
	case GL_SAMPLE_COVERAGE: gl_state.multisampling.sample_coverage_enable = GL_FALSE; return;
	case GL_TEXTURE_2D: gl_state.texture_units[gl_state.active_texture - GL_TEXTURE0].texture_2d_enable = GL_FALSE; return;
	case GL_ALPHA_TEST: gl_state.pixel_op.alpha_test.enable = GL_FALSE; return;
	case GL_STENCIL_TEST: gl_state.pixel_op.stencil_test.enable = GL_FALSE; return;
	case GL_DEPTH_TEST: gl_state.pixel_op.depth_test.enable = GL_FALSE; return;
	case GL_BLEND: gl_state.pixel_op.blend.enable = GL_FALSE; return;
	case GL_DITHER: gl_state.pixel_op.dither_enable = GL_FALSE; return;
	case GL_COLOR_LOGIC_OP: gl_state.pixel_op.color_logic_op_enable = GL_FALSE; return;
	case GL_SCISSOR_TEST: 
		gl_state.pixel_op.scissor.test_enable = GL_FALSE; 
		update_scissor();
		return;
	case GL_CULL_FACE:
		gl_state.rasterization.cull_face_enable = GL_FALSE; 
		update_culling();
		return;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}
}

void F2X_Context::glEnableClientState(GLenum array)
{
	glEnable(array);
}

void F2X_Context::glDisableClientState(GLenum array)
{
	glDisable(array);
}

void F2X_Context::glFinish(void)
{
}

void F2X_Context::glFlush(void)
{
}

void F2X_Context::glFogx(GLenum pname, GLfixed param)
{
	glFogxv(pname, &param);
}

void F2X_Context::glFogxv(GLenum pname, const GLfixed *params)
{
	switch (pname) {
	case GL_FOG_MODE: 
		switch (*params) {
		case GL_LINEAR:
		case GL_EXP:
		case GL_EXP2:
			gl_state.coloring.fog_mode = *params;
			return;
		default:
			emit_error(GL_INVALID_VALUE);
			return;
		}
	case GL_FOG_DENSITY:
		if (*params < 0) {
			emit_error(GL_INVALID_VALUE);
			return;
		}
		gl_state.coloring.fog_density = *params;
		return;
	case GL_FOG_START: gl_state.coloring.fog_start = *params; return;
	case GL_FOG_END: gl_state.coloring.fog_end = *params; return;
	case GL_FOG_COLOR:
		for (int i = 0; i < 4; ++i)
			gl_state.coloring.fog_color[i].intValue = clamp_fixed(params[i]);
		derived_state.update_fog_color();
		return;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}
}

void F2X_Context::glFrontFace(GLenum mode)
{
	switch (mode) {
	case GL_CW:
	case GL_CCW:
		gl_state.rasterization.front_face = mode;
		update_culling();
		break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}	
}


void F2X_Context::glLineWidthx(GLfixed width)
{
	gl_state.rasterization.line_width = width;
}

void F2X_Context::glLogicOp(GLenum opcode)
{
	switch (opcode) {
	case GL_CLEAR:
	case GL_AND:
	case GL_AND_REVERSE:
	case GL_COPY:
	case GL_AND_INVERTED:
	case GL_NOOP:
	case GL_XOR:
	case GL_OR:
	case GL_NOR:
	case GL_EQUIV:
	case GL_INVERT:
	case GL_OR_REVERSE:
	case GL_COPY_INVERTED:
	case GL_OR_INVERTED:
	case GL_NAND:
	case GL_SET:
		gl_state.pixel_op.logic_op_mode = opcode;
		break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}
}

void F2X_Context::glMultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
	if (target < GL_TEXTURE0 || target > GL_TEXTURE0 + gl_state.implementation.MAX_TEXTURE_UNITS) {
		emit_error(GL_INVALID_ENUM);
		return;
	}

	target -= GL_TEXTURE0;

	gl_state.current.texture_coords[target][0].intValue = s;
	gl_state.current.texture_coords[target][1].intValue = t;
	gl_state.current.texture_coords[target][2].intValue = r;
	gl_state.current.texture_coords[target][3].intValue = q;
}

void F2X_Context::glNormal3x(GLfixed nx, GLfixed ny, GLfixed nz)
{
	gl_state.current.normal[0].intValue = nx;
	gl_state.current.normal[1].intValue = ny;
	gl_state.current.normal[2].intValue = nz;
}

void F2X_Context::glPointSizex(GLfixed size)
{
	if (size <= 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	gl_state.rasterization.point_size = size;
}

void F2X_Context::glPolygonOffsetx(GLfixed factor, GLfixed units)
{
	gl_state.rasterization.polygon_offset.factor = factor;
	gl_state.rasterization.polygon_offset.units = units;
}

void F2X_Context::glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
	if (width < 0 || height < 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	if (!color_buffer.data) {
		emit_error(GL_INVALID_OPERATION);
		return;
	};

	// TODO: implement

	emit_error(GL_INVALID_OPERATION);
}

void F2X_Context::glSampleCoveragex(GLclampx value, GLboolean invert)
{
	value = clamp_fixed(value);
	gl_state.multisampling.sample_coverage_value = value;
	gl_state.multisampling.sample_coverage_invert = invert;
}

void F2X_Context::glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
	if (width < 0 || height < 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	gl_state.pixel_op.scissor.box[0] = x;
	gl_state.pixel_op.scissor.box[1] = y;
	gl_state.pixel_op.scissor.box[2] = width;
	gl_state.pixel_op.scissor.box[3] = height;

	update_scissor();
}

void F2X_Context::glTexEnvx(GLenum target, GLenum pname, GLfixed param)
{
	glTexEnvxv(target, pname, &param);
}

void F2X_Context::glTexEnvxv(GLenum target, GLenum pname, const GLfixed *params)
{
	if (target != GL_TEXTURE_ENV) {
		emit_error(GL_INVALID_ENUM);
		return;
	}
	
	const int at = gl_state.active_texture - GL_TEXTURE0;

	switch (pname) {
	case GL_TEXTURE_ENV_MODE: 
		switch (*params) {
		case GL_MODULATE:
		case GL_REPLACE:
		case GL_DECAL:
		case GL_BLEND:
		case GL_ADD:
			gl_state.texture_units[at].texture_env_mode = *params;
			return;
		default:
			emit_error(GL_INVALID_VALUE);
			return;
		}
	case GL_TEXTURE_ENV_COLOR: 
		{
			vec4x &color = gl_state.texture_units[at].texture_env_color;
			for (int i = 0; i < 4; ++i)
				color[i].intValue = params[i];
			derived_state.texture_units[at].texture_env_color = color_to_ushort(color);
			return;
		}
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}
}

void F2X_Context::glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
	switch (func) {
	case GL_NEVER:
	case GL_LESS: 
	case GL_EQUAL: 
	case GL_LEQUAL: 
	case GL_GREATER: 
	case GL_NOTEQUAL: 
	case GL_GEQUAL: 
	case GL_ALWAYS:
		gl_state.pixel_op.stencil_test.func = func;
		gl_state.pixel_op.stencil_test.ref = ref;
		gl_state.pixel_op.stencil_test.mask = mask;
		break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}
}

void F2X_Context::glStencilMask(GLuint mask) 
{
	gl_state.framebuf_ctrl.stencil_writemask = mask;
}

void F2X_Context::glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
#define TEST(VAR) \
	switch (VAR) { \
	case GL_ZERO: \
	case GL_KEEP: \
	case GL_REPLACE: \
	case GL_INCR: \
	case GL_DECR: \
	case GL_INVERT: \
		break; \
	default: \
		emit_error(GL_INVALID_ENUM); \
		return; \
	}

	TEST(fail);
	TEST(zfail);
	TEST(zpass);

#undef TEST

	gl_state.pixel_op.stencil_test.fail = fail;
	gl_state.pixel_op.stencil_test.pass_depth_fail = zfail;
	gl_state.pixel_op.stencil_test.pass_depth_pass = zpass;
}

void F2X_Context::glHint(GLenum target, GLenum mode)
{
	switch (mode) {
	case GL_DONT_CARE:
	case GL_FASTEST:
	case GL_NICEST:
		break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}

	switch (target) {
	case GL_POINT_SMOOTH_HINT: gl_state.hints.point_smooth = mode; break;
	case GL_LINE_SMOOTH_HINT: gl_state.hints.line_smooth = mode; break;
	case GL_FOG_HINT: gl_state.hints.fog = mode; break;
	case GL_PERSPECTIVE_CORRECTION_HINT: 
		gl_state.hints.perspective_correction = mode;
		switch (mode) {
		case GL_FASTEST: 
			rasterizer.perspective_correction(false);
			break;
		case GL_NICEST:
			rasterizer.perspective_correction(true);
			rasterizer.perspective_threshold(0, 0);
			break;
		case GL_DONT_CARE:
			rasterizer.perspective_correction(true);
			rasterizer.perspective_threshold(24, 24);
			break;
		}
		break;
	default:
		emit_error(GL_INVALID_ENUM);
	}
}

void F2X_Context::glPixelStorei(GLenum pname, GLint param)
{
	GLint *p = 0;

	switch (pname) {
	case GL_PACK_ALIGNMENT: p = &gl_state.pixel_transfer.pack_alignment; break;
	case GL_UNPACK_ALIGNMENT: p = &gl_state.pixel_transfer.unpack_alignment; break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}

	switch (param) {
	case 1:
	case 2:
	case 4:
	case 8:
		*p = param;
		break;
	default:
		emit_error(GL_INVALID_VALUE);
		return;
	}
}

void F2X_Context::glShadeModel(GLenum mode) 
{
	switch (mode) {
	case GL_FLAT: 
	case GL_SMOOTH:
		gl_state.coloring.shade_model = mode;
		break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}
}

void F2X_Context::glLightx(GLenum light, GLenum pname, GLfixed param) 
{
	glLightxv(light, pname, &param);
}

void F2X_Context::glLightxv(GLenum light, GLenum pname, const GLfixed *params)
{
	if (light < GL_LIGHT0 || light > GL_LIGHT0 + gl_state.implementation.MAX_LIGHTS) {
		emit_error(GL_INVALID_ENUM);
		return;
	}

	light -= GL_LIGHT0;

	switch (pname) {
	case GL_AMBIENT: for (int i = 0; i < 3; ++i) gl_state.lights[light].ambient[i].intValue = params[i]; return;
	case GL_DIFFUSE: for (int i = 0; i < 3; ++i) gl_state.lights[light].diffuse[i].intValue = params[i]; return;
	case GL_SPECULAR: for (int i = 0; i < 3; ++i) gl_state.lights[light].specular[i].intValue = params[i]; return;
	case GL_CONSTANT_ATTENUATION: gl_state.lights[light].constant_attenuation = *params; return;
	case GL_LINEAR_ATTENUATION: gl_state.lights[light].linear_attenuation = *params; return;
	case GL_QUADRATIC_ATTENUATION: gl_state.lights[light].quadratic_attenuation = *params; return;
	case GL_SPOT_EXPONENT: gl_state.lights[light].spot_exponent = *params; return;
	case GL_SPOT_CUTOFF: gl_state.lights[light].spot_cutoff = *params; return;
	case GL_POSITION: 
		{
			vec4x p;

			for (int i = 0; i < 4; ++i)
				p[i].intValue = params[i];
			
			mat4x &m = gl_state.transform_state.modelview_matrix[
				gl_state.transform_state.modelview_stack_depth - 1];
			
			p = m * p;

			for (int i = 0; i < 4; ++i)
				gl_state.lights[light].position[i] = p[i]; 

			return;
		}
	case GL_SPOT_DIRECTION: 
		{
			vec3x d;

			for (int i = 0; i < 3; ++i) 
				d[i].intValue = params[i];

			mat4x &m = gl_state.transform_state.modelview_matrix[
				gl_state.transform_state.modelview_stack_depth - 1];

			d = vmath::transform_vector(m, d);

			for (int i = 0; i < 3; ++i) 
				gl_state.lights[light].spot_direction[i] = d[i];

			return;
		}
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}	
}

void F2X_Context::glLightModelx(GLenum pname, GLfixed param)
{
	glLightModelxv(pname, &param);
}

void F2X_Context::glLightModelxv(GLenum pname, const GLfixed *params)
{
	switch (pname) {
	case GL_LIGHT_MODEL_TWO_SIDE: gl_state.lighting.light_model_two_side = *params; return;
	case GL_LIGHT_MODEL_AMBIENT: for (int i = 0; i < 4; ++i) gl_state.lighting.light_model_ambient[i].intValue = params[i]; return;
	default: 
		emit_error(GL_INVALID_ENUM); 
		return;
	}


}

void F2X_Context::glMaterialx(GLenum face, GLenum pname, GLfixed param)
{
	glMaterialxv(face, pname, &param);
}

void F2X_Context::glMaterialxv(GLenum face, GLenum pname, const GLfixed *params)
{
	if (face != GL_FRONT_AND_BACK) {
		emit_error(GL_INVALID_ENUM);
		return;
	}

	switch (pname) {
	case GL_AMBIENT: for (int i = 0; i < 3; ++i) gl_state.lighting.material_params[0].ambient[i].intValue = params[i]; return;
	case GL_DIFFUSE: for (int i = 0; i < 3; ++i) gl_state.lighting.material_params[0].diffuse[i].intValue = params[i]; return;
	case GL_SPECULAR: for (int i = 0; i < 3; ++i) gl_state.lighting.material_params[0].specular[i].intValue = params[i]; return;
	case GL_EMISSION: for (int i = 0; i < 3; ++i) gl_state.lighting.material_params[0].emission[i].intValue = params[i]; return;
	case GL_SHININESS: gl_state.lighting.material_params[0].shininess = *params; return;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}
}

void F2X_Context::glDepthRangex(GLclampx zNear, GLclampx zFar)
{
	gl_state.transform_state.depth_range[0] = zNear;
	gl_state.transform_state.depth_range[1] = zFar;

	geometry_processor.depth_range(
		fixedpoint::fixmul<16>(zNear, 0x3fffffff),
		fixedpoint::fixmul<16>(zFar, 0x3fffffff)
	);
}

void F2X_Context::glViewport(GLint x, GLint y, GLsizei width, GLsizei height) 
{
	if (width < 0 || height < 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	width = std::min(width, gl_state.implementation.MAX_VIEWPORT_DIMS[0]);
	height = std::min(height, gl_state.implementation.MAX_VIEWPORT_DIMS[1]);

	geometry_processor.viewport(x, y, width, height);
}
