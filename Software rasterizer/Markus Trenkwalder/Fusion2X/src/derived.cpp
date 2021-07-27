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
#include "shaders/generic_fragment_shader.h"
#include "shaders/generic_vertex_shader.h"
#include "util.h"

namespace {
	size_t sizeof_gl_type(GLenum type) {
		switch (type) {
		case GL_BYTE: return sizeof(GLbyte);
		case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
		case GL_SHORT: return sizeof(GLshort);
		case GL_UNSIGNED_SHORT: return sizeof(GLushort);
		case GL_FLOAT: return sizeof(GLfloat);
		case GL_FIXED: return sizeof(GLfixed);
		}
		return 0;
	}
}

void F2X_Context::DerivedState::init(F2X_Context *context)
{
	ctx = context;

	for (int i = 0; i < GLState::Implementation::MAX_TEXTURE_UNITS; ++i) {
		texture_units[i].texture_env_color = 
			color_to_ushort(ctx->gl_state.texture_units[i].texture_env_color);
	}

	update_color_writemask();
	update_fog_color();
}

void F2X_Context::DerivedState::update_color_writemask()
{
	color_writemask = 0;
	if (ctx->gl_state.framebuf_ctrl.color_writemask[0]) color_writemask |= 0xf800;
	if (ctx->gl_state.framebuf_ctrl.color_writemask[1]) color_writemask |= 0x07e0;
	if (ctx->gl_state.framebuf_ctrl.color_writemask[2]) color_writemask |= 0x001f;	
}

void F2X_Context::DerivedState::update_fog_color()
{
	fog_color = color_to_ushort(ctx->gl_state.coloring.fog_color);
}

bool F2X_Context::DerivedState::update_state_before_draw()
{
	const GLState &gl = ctx->gl_state;
	const GLState::TransformState &ts = gl.transform_state;

	// no drawing will happen if vertex array is disabled
	if (!gl.vertex_array.enable) return false;

	// compute derived matrices
	const mat4x &mv = ts.modelview_matrix[ts.modelview_stack_depth - 1];
	const mat4x &pr = ts.projection_matrix[ts.projection_stack_depth - 1];
	modelview_projection_matrix = pr * mv;
	normal_matrix = vmath::inverse(vmath::transpose(mat3x(mv)));

	// set the appropriate shader state
	GenericVertexShader::ctx = ctx;
	GenericFragmentShader::ctx = ctx;

	ctx->rasterizer.fragment_shader<GenericFragmentShader>();
	ctx->geometry_processor.vertex_shader<GenericVertexShader>();

	// vertex array always enabled or no drawing will happen
	int vstride = gl.vertex_array.stride;
	if (!vstride) vstride = sizeof_gl_type(gl.vertex_array.type) * gl.vertex_array.size;
	ctx->geometry_processor.vertex_attrib_pointer(0, vstride, gl.vertex_array.pointer);

	// color array
	if (gl.color_array.enable) {
		int stride = gl.color_array.stride;
		if (!stride) stride = sizeof_gl_type(gl.color_array.type) * gl.color_array.size;
		ctx->geometry_processor.vertex_attrib_pointer(1, stride, gl.color_array.pointer);
	} else {
		ctx->geometry_processor.vertex_attrib_pointer(1, 0, 0);
	}

	// normal array
	if (gl.normal_array.enable) {
		int stride = gl.normal_array.stride;
		if (!stride) stride = sizeof_gl_type(gl.normal_array.type) * gl.normal_array.size;
		ctx->geometry_processor.vertex_attrib_pointer(2, stride, gl.normal_array.pointer);
	} else {
		ctx->geometry_processor.vertex_attrib_pointer(2, 0, 0);
	}

	// texture coord array
	if (gl.texture_coord_array[0].enable) {
		int stride = gl.texture_coord_array[0].stride;
		if (!stride) stride = sizeof_gl_type(gl.texture_coord_array[0].type) * gl.texture_coord_array[0].size;
		ctx->geometry_processor.vertex_attrib_pointer(3, stride, gl.texture_coord_array[0].pointer);
	} else {
		ctx->geometry_processor.vertex_attrib_pointer(3, 0, 0);
	}

	return true;
}

namespace {
	vec4x fetch_array(const F2X_Context::GLState::DataArray &array, const void *p)
	{
		vec4x result(0, 0, 0, 1);

		switch (array.type) {
		case GL_FIXED:
			{
				const GLfixed *pp = static_cast<const GLfixed*>(p);
				switch (array.size) {
				case 4: result[3].intValue = pp[3];
				case 3: result[2].intValue = pp[2];
				case 2: result[1].intValue = pp[1];
				case 1: result[0].intValue = pp[0];
				}
			}
			break;
		case GL_BYTE:
			{
				const GLbyte *pp = static_cast<const GLbyte*>(p);
				switch (array.size) {
				case 4: result[3] = pp[3];
				case 3: result[2] = pp[2];
				case 2: result[1] = pp[1];
				case 1: result[0] = pp[0];
				}
				break;
			}
		case GL_SHORT:
			{
				const GLshort *pp = static_cast<const GLshort*>(p);
				switch (array.size) {
				case 4: result[3] = pp[3];
				case 3: result[2] = pp[2];
				case 2: result[1] = pp[1];
				case 1: result[0] = pp[0];
				}
				break;
			}
		case GL_UNSIGNED_BYTE:
			{
				// HACK: this assumes it is a color array and needs conversion
				const GLubyte *pp = static_cast<const GLubyte*>(p);
				switch (array.size) {
				case 4: result[3].intValue = (pp[3] << 16) / 255;
				case 3: result[2].intValue = (pp[2] << 16) / 255;
				case 2: result[1].intValue = (pp[1] << 16) / 255;
				case 1: result[0].intValue = (pp[0] << 16) / 255;
				}
				break;
			}
		break;
		}

		return result;
	}
}

vec4x F2X_Context::DerivedState::fetch_vertex(const void *p) const
{
	const F2X_Context::GLState::DataArray &array = ctx->gl_state.vertex_array;
	return fetch_array(array, p);
}

vec4x F2X_Context::DerivedState::fetch_color(const void *p) const
{
	const F2X_Context::GLState::DataArray &array = ctx->gl_state.color_array;
	if (!array.enable) return ctx->gl_state.current.color;
	return fetch_array(array, p);
}

vec4x F2X_Context::DerivedState::fetch_texcoord(const void *p, int texture_unit) const
{
	const F2X_Context::GLState::DataArray &array = ctx->gl_state.texture_coord_array[texture_unit];
	if (!array.enable) return ctx->gl_state.current.texture_coords[texture_unit];
	return fetch_array(array, p);
}

vec3x F2X_Context::DerivedState::fetch_normal(const void *p) const
{
	const F2X_Context::GLState::DataArray &array = ctx->gl_state.normal_array;
	if (!array.enable) return ctx->gl_state.current.normal;
	return vec3x(fetch_array(array, p));
}
