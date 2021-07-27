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

#ifndef CONTEXT_H_
#define CONTEXT_H_

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "fusion2x.h"
#include "GLES/gl.h"

#include "vmath.h"
#include "renderer/geometry_processor.h"
#include "renderer/rasterizer_subdivaffine.h"
#include "gltexobj.h"

#include "shaders/optimized/spanfuncs.h"

#include <map>
#include <utility>
#include <vector>

#ifndef NDEBUG
#	include <iostream>
#endif

extern F2X_Context* current_context;

struct F2X_Context {
	F2X_Context(F2X_ContextCreateParams *params);

	int set_param(unsigned pname, void *data);
	int get_param(unsigned pname, void *data);

//////////
	
	void update_scissor();
	void update_viewport();
	void update_culling();
	void update_span_buffers();
	void max_render_extents(unsigned &w, unsigned &h);

	bool select_optimized_shaders();

//////////

	swr::RasterizerSubdivAffine rasterizer;
	swr::GeometryProcessor geometry_processor;

	std::vector<unsigned short> span_buffers[5];

//////////

	F2X_RenderSurface color_buffer;
	F2X_RenderSurface depth_buffer;

//////////

	struct GLState {
		struct Implementation {
			static const GLint MAX_LIGHTS = 8;
			static const GLint MAX_MODELVIEW_STACK_DEPTH = 16;
			static const GLint MAX_PROJECTION_STACK_DEPTH = 2;
			static const GLint MAX_TEXTURE_STACK_DEPTH = 2;
			static const GLint SUBPIXEL_BITS = 4;
			static const GLint MAX_TEXTURE_SIZE = 256;
			static const GLint MAX_VIEWPORT_DIMS[2];
			static const GLfixed ALIASED_POINT_SIZE_RANGE[2];
			static const GLfixed SMOOTH_POINT_SIZE_RANGE[2];
			static const GLfixed ALIASED_LINE_WIDTH_RANGE[2];
			static const GLfixed SMOOTH_LINE_WIDTH_RANGE[2];
			static const GLint MAX_ELEMENTS_INDICES = 0x7fffffff;
			static const GLint MAX_ELEMENTS_VERTICES = 0x7fffffff;
			static const GLint MAX_TEXTURE_UNITS = 2;
			static const GLint SAMPLE_BUFFERS = 0;
			static const GLint SAMPLES = 0;
			static const GLint COMPRESSED_TEXTURE_FORMATS[];
			static const GLint NUM_COMPRESSED_TEXTURE_FORMATS = 0;
			static const GLint IMPLEMENTATION_COLOR_READ_TYPE_OES = GL_RGB;
			static const GLint IMPLEMENTATION_COLOR_READ_FORMAT_OES = GL_UNSIGNED_SHORT_5_6_5;
			GLint red_bits;
			GLint green_bits;
			GLint blue_bits;
			GLint alpha_bits;
			GLint depth_bits;
			GLint stencil_bits;

			Implementation() :
				red_bits(0),
				green_bits(0),
				blue_bits(0),
				alpha_bits(0),
				depth_bits(0),
				stencil_bits(0)
			{}

		} implementation;

		struct CurrentState {
			vec4x color;
			vec4x texture_coords[Implementation::MAX_TEXTURE_UNITS];
			vec3x normal;

			CurrentState () :
				color(1),
				normal(0)
			{
				for (int i = 0; i < Implementation::MAX_TEXTURE_UNITS; ++i)
					texture_coords[i] = vec4x(0);
			}
		} current;

		GLint client_active_texture;	

		struct DataArray {
			GLboolean enable;
			GLint size;
			GLenum type;
			GLsizei stride;
			const GLvoid *pointer;

			DataArray() :
				enable(GL_FALSE),
				size(4),
				type(GL_FIXED),
				stride(0),
				pointer(0)
			{}
		};

		DataArray vertex_array;
		DataArray normal_array;
		DataArray color_array;
		DataArray texture_coord_array[Implementation::MAX_TEXTURE_UNITS];

		struct TransformState {
			mat4x modelview_matrix[Implementation::MAX_MODELVIEW_STACK_DEPTH];
			mat4x projection_matrix[Implementation::MAX_PROJECTION_STACK_DEPTH];
			mat4x texture_matrix[Implementation::MAX_TEXTURE_UNITS][Implementation::MAX_TEXTURE_STACK_DEPTH];
			GLuint viewport[4];
			GLfixed depth_range[2];
			GLint modelview_stack_depth;
			GLint projection_stack_depth;
			GLint texture_stack_depth[Implementation::MAX_TEXTURE_UNITS];
			GLint matrix_mode;
			GLboolean normalize_enable;
			GLboolean rescale_normal_enable;

			TransformState() :
				modelview_stack_depth(1),
				projection_stack_depth(1),
				matrix_mode(GL_MODELVIEW),
				normalize_enable(GL_FALSE),
				rescale_normal_enable(GL_FALSE)
			{
				modelview_matrix[0] = vmath::identity4<fixed16_t>();
				projection_matrix[0] = vmath::identity4<fixed16_t>();

				for (int i = 0; i < 2; ++i) {
					texture_stack_depth[i] = 1;
					texture_matrix[i][0] = vmath::identity4<fixed16_t>();
				}

				for (int i = 0; i < 4; ++i)
					viewport[i] = 0;
		
				depth_range[0] = fixed16_t(0).intValue;
				depth_range[1] = fixed16_t(1).intValue;
			}
		} transform_state;

		struct Coloring {
			vec4x fog_color;
			GLfixed fog_density;
			GLfixed fog_start;
			GLfixed fog_end;
			GLint fog_mode;
			GLboolean fog_enable;
			GLint shade_model;
			
			Coloring() :
				fog_color(0),
				fog_density(fixed16_t(1).intValue),
				fog_start(0),
				fog_end(fixed16_t(1).intValue),
				fog_mode(GL_EXP),
				fog_enable(GL_FALSE),
				shade_model(GL_SMOOTH)
			{}

		} coloring;

		struct Lighting {
			GLboolean enable;
			GLboolean color_material_enable;

			struct MaterialParams {
				vec4x ambient;
				vec4x diffuse;
				vec4x specular;
				vec4x emission;
				GLfixed shininess;

				MaterialParams() :
					ambient(0.2f),
					diffuse(0.8f),
					specular(0, 0, 0, 1),
					emission(0, 0, 0, 1),
					shininess(0)
				{}

			// arrays size two if two sided lighting with different 
			// back and front material was supported.
			} material_params[1]; 
			

			vec4x light_model_ambient;
			GLboolean light_model_two_side;

			Lighting() : 
				enable(GL_FALSE),
				color_material_enable(GL_FALSE),
				light_model_ambient(0.2f, 0.2f, 0.2f, 1),
				light_model_two_side(GL_FALSE)
			{}

		} lighting;

		struct Light {
			vec4x ambient;
			vec4x diffuse;
			vec4x specular;
			vec4x position;
			GLfixed constant_attenuation;
			GLfixed linear_attenuation;
			GLfixed quadratic_attenuation;
			vec3x spot_direction;
			GLfixed spot_exponent;
			GLfixed spot_cutoff;
			GLboolean enable;

			Light() :
				ambient(0, 0, 0, 1),
				diffuse(0, 0, 0, 1),
				specular(0, 0, 0, 1),
				position(0, 0, 1, 0),
				constant_attenuation(fixed16_t(1).intValue),
				linear_attenuation(0),
				quadratic_attenuation(0),
				spot_direction(0, 0, -1),
				spot_exponent(0),
				spot_cutoff(fixed16_t(180).intValue),
				enable(GL_FALSE)
			{}

		} lights[8];

		struct Rasterization {
			GLfixed point_size;
			GLboolean point_smooth_enable;
			GLfixed point_size_min;
			GLfixed point_size_max;
			GLfixed point_fade_threshold;
			GLfixed point_distance_attenuation[3];
			GLfixed line_width;
			GLboolean line_smooth_enable;
			GLboolean cull_face_enable;
			GLint cull_face_mode;
			GLint front_face;

			struct PolygonOffset {
				GLfixed factor;
				GLfixed units;
				GLboolean fill_enable;

				PolygonOffset() : 
					factor(0),
					units(0),
					fill_enable(GL_FALSE)
				{}

			} polygon_offset;

			Rasterization() :
				point_size(fixed16_t(1).intValue),
				point_smooth_enable(GL_FALSE),
				line_width(fixed16_t(1).intValue),
				line_smooth_enable(GL_FALSE),
				cull_face_enable(GL_FALSE),
				cull_face_mode(GL_CCW),
				front_face(GL_BACK)
			{
				point_distance_attenuation[0] = fixed16_t(1).intValue;
				point_distance_attenuation[0] = 0;
				point_distance_attenuation[0] = 0;
			}
		} rasterization;

		struct Multisampling {
			GLboolean enable;
			GLboolean sample_alpha_to_coverage_enable;
			GLboolean sample_alpha_to_one_enable;
			GLboolean sample_coverage_enable;
			GLfixed sample_coverage_value;
			GLboolean sample_coverage_invert;

			Multisampling() :
				enable(GL_TRUE),
				sample_alpha_to_coverage_enable(GL_FALSE),
				sample_alpha_to_one_enable(GL_FALSE),
				sample_coverage_enable(GL_FALSE),
				sample_coverage_value(fixed16_t(1).intValue),
				sample_coverage_invert(GL_FALSE)
			{}
		} multisampling;

		GLint active_texture;

		struct TextureUnitState {
			GLboolean texture_2d_enable;
			GLint texture_binding_2d;
			GLuint texture_env_mode;
			vec4x texture_env_color;

			TextureUnitState() :
				texture_2d_enable(GL_FALSE),
				texture_binding_2d(0),
				texture_env_mode(GL_MODULATE),
				texture_env_color(0)
			{}

		} texture_units[Implementation::MAX_TEXTURE_UNITS];

		struct PixelOperations {
			struct ScissorState {
				GLboolean test_enable;
				GLint box[4];

				ScissorState() :
					test_enable(GL_FALSE)
				{
					for (int i = 0; i < 4; ++i)
						box[i] = 0;
				}
			} scissor;

			struct AlphaTest {
				GLboolean enable;
				GLint func;
				GLfixed ref;

				AlphaTest() :
					enable(GL_FALSE),
					func(GL_ALWAYS),
					ref(0)
				{}

			} alpha_test;

			struct StencilTest {
				GLboolean enable;
				GLint func;
				GLint mask;
				GLfixed ref;
				GLint fail;
				GLint pass_depth_fail;
				GLint pass_depth_pass;

				StencilTest() :
					enable(GL_FALSE),
					func(GL_ALWAYS),
					mask(0xffffffff),
					ref(0),
					fail(GL_KEEP),
					pass_depth_fail(GL_KEEP),
					pass_depth_pass(GL_KEEP)
				{}

			} stencil_test;

			struct DepthTest {
				GLboolean enable;
				GLint func;

				DepthTest() :
					enable(GL_FALSE),
					func(GL_LESS)
				{}

			} depth_test;

			struct Blend {
				GLboolean enable;
				GLint src;
				GLint dst;

				Blend() : 
					enable(GL_FALSE),
					src(GL_ONE),
					dst(GL_ZERO)
				{}
			} blend;

			GLboolean dither_enable;
			GLboolean color_logic_op_enable;
			GLint logic_op_mode;

			PixelOperations() :
				dither_enable(GL_TRUE),
				color_logic_op_enable(GL_FALSE),
				logic_op_mode(GL_COPY)
			{}

		} pixel_op;

		struct FrameBufferControl {
			GLboolean color_writemask[4];
			GLboolean depth_writemask;
			GLint stencil_writemask;
			vec4x color_clear_value;
			GLfixed depth_clear_value;
			GLint stencil_clear_value;

			FrameBufferControl() :
				depth_writemask(GL_TRUE),
				stencil_writemask(0xffffffff),
				color_clear_value(0),
				depth_clear_value(fixed16_t(1).intValue),
				stencil_clear_value(0)
			{
				for (int i = 0; i < 4; ++i)
					color_writemask[i] = GL_TRUE;
			}

		} framebuf_ctrl;

		struct PixelTransfer {
			GLint unpack_alignment;
			GLint pack_alignment;

			PixelTransfer() :
				unpack_alignment(4),
				pack_alignment(4)
			{}
		} pixel_transfer;

		struct Hints {
			GLint perspective_correction;
			GLint point_smooth;
			GLint line_smooth;
			GLint fog;

			Hints() :
				perspective_correction(GL_DONT_CARE),
				point_smooth(GL_DONT_CARE),
				line_smooth(GL_DONT_CARE),
				fog(GL_DONT_CARE)
			{}

		} hints;

		struct GenTexturesState {
			GLuint max;
			std::vector<GLuint> free;

			GenTexturesState() : 
				max(0) 
			{}

		} gentextures;

		struct TextureState {
			GLuint binding_2d;
			GLTextureObject texture0_2d;

			GLTextureObject *binding_2d_tex;

			typedef std::map<GLuint, GLTextureObject*> texture_id_map_t;
			texture_id_map_t texture_id_map;

			TextureState() 
				: binding_2d(0)
				, texture0_2d(GL_TEXTURE_2D)
				, binding_2d_tex(&texture0_2d)
			{}

			GLTextureObject *get_texture_object(GLenum target, GLuint texture)
			{
				if (target != GL_TEXTURE_2D) return 0;
				if (texture == 0) return &texture0_2d;
				texture_id_map_t::const_iterator i = texture_id_map.find(texture);
				if (i != texture_id_map.end()) return i->second;
				return 0;
			}

			const GLTextureObject *get_texture_object(GLenum target, GLuint texture) const
			{
				return const_cast<TextureState*>(this)->get_texture_object(target, texture);
			}

		} texture_state;

		GLenum error;

		GLState() :
			client_active_texture(GL_TEXTURE0),
			active_texture(GL_TEXTURE0),
			error(GL_NO_ERROR)
		{
			lights[0].diffuse = vec4x(1, 1, 1, 1);
			lights[0].specular = vec4x(1, 1, 1, 1);
		}

	} gl_state;

	struct DerivedState {
		F2X_Context *ctx;

		void init(F2X_Context *context);

		// transformation
		mat4x modelview_projection_matrix;
		mat3x normal_matrix;

		unsigned short current_color;
		unsigned short current_alpha;

		// colors
		unsigned short color_writemask;
		unsigned short fog_color;

		struct TextureUnitState {
			unsigned short texture_env_color;
		} texture_units[GLState::Implementation::MAX_TEXTURE_UNITS];

		// vertex fetch functions
		vec4x fetch_vertex(const void *p) const;
		vec4x fetch_color(const void *p) const;
		vec3x fetch_normal(const void *p) const;
		vec4x fetch_texcoord(const void *p, int texture_unit) const;

		// optimized span functions
		OptimizedSpanFunctions::texture_span_t texture_span;
		OptimizedSpanFunctions::texture_env_span_t texture_env_span;
		OptimizedSpanFunctions::alpha_test_span_t alpha_test_span;
		OptimizedSpanFunctions::depth_test_span_t depth_test_span;
		OptimizedSpanFunctions::blend_span_t blend_span;

		// update functions
		void update_color_writemask();
		void update_fog_color();
		void update_current_color();
		bool update_state_before_draw();
	} derived_state;

//////////

	void emit_error(GLenum error)
	{
		if (gl_state.error != GL_NO_ERROR) return;
		gl_state.error = error;

#	ifndef NDEBUG
		std::cout << "GL error generated" << std::endl;
#	endif

#	if defined(_DEBUG) && defined(_MSC_VER)
		__asm int 3;
#	endif
	}

	mat4x* top_matrix();

//////////

	void glActiveTexture (GLenum texture);
	void glAlphaFuncx (GLenum func, GLclampx ref);
	void glBindTexture (GLenum target, GLuint texture);
	void glBlendFunc (GLenum sfactor, GLenum dfactor);
	void glClear (GLbitfield mask);
	void glClearColorx (GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
	void glClearDepthx (GLclampx depth);
	void glClearStencil (GLint s);
	void glClientActiveTexture (GLenum texture);
	void glColor4x (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
	void glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
	void glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
	void glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
	void glCopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
	void glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	void glCullFace (GLenum mode);
	void glDeleteTextures (GLsizei n, const GLuint *textures);
	void glDepthFunc (GLenum func);
	void glDepthMask (GLboolean flag);
	void glDepthRangex (GLclampx zNear, GLclampx zFar);
	void glDisable (GLenum cap);
	void glDisableClientState (GLenum array);
	void glDrawArrays (GLenum mode, GLint first, GLsizei count);
	void glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
	void glEnable (GLenum cap);
	void glEnableClientState (GLenum array);
	void glFinish (void);
	void glFlush (void);
	void glFogx (GLenum pname, GLfixed param);
	void glFogxv (GLenum pname, const GLfixed *params);
	void glFrontFace (GLenum mode);
	void glFrustumx (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
	void glGenTextures (GLsizei n, GLuint *textures);
	GLenum glGetError (void);
	void glGetIntegerv (GLenum pname, GLint *params);
	const GLubyte * glGetString (GLenum name);
	void glHint (GLenum target, GLenum mode);
	void glLightModelx (GLenum pname, GLfixed param);
	void glLightModelxv (GLenum pname, const GLfixed *params);
	void glLightx (GLenum light, GLenum pname, GLfixed param);
	void glLightxv (GLenum light, GLenum pname, const GLfixed *params);
	void glLineWidthx (GLfixed width);
	void glLoadIdentity (void);
	void glLoadMatrixx (const GLfixed *m);
	void glLogicOp (GLenum opcode);
	void glMaterialx (GLenum face, GLenum pname, GLfixed param);
	void glMaterialxv (GLenum face, GLenum pname, const GLfixed *params);
	void glMatrixMode (GLenum mode);
	void glMultMatrixx (const GLfixed *m);
	void glMultiTexCoord4x (GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
	void glNormal3x (GLfixed nx, GLfixed ny, GLfixed nz);
	void glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer);
	void glOrthox (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
	void glPixelStorei (GLenum pname, GLint param);
	void glPointSizex (GLfixed size);
	void glPolygonOffsetx (GLfixed factor, GLfixed units);
	void glPopMatrix (void);
	void glPushMatrix (void);
	void glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
	void glRotatex (GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
	void glSampleCoveragex (GLclampx value, GLboolean invert);
	void glScalex (GLfixed x, GLfixed y, GLfixed z);
	void glScissor (GLint x, GLint y, GLsizei width, GLsizei height);
	void glShadeModel (GLenum mode);
	void glStencilFunc (GLenum func, GLint ref, GLuint mask);
	void glStencilMask (GLuint mask);
	void glStencilOp (GLenum fail, GLenum zfail, GLenum zpass);
	void glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void glTexEnvx (GLenum target, GLenum pname, GLfixed param);
	void glTexEnvxv (GLenum target, GLenum pname, const GLfixed *params);
	void glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	void glTexParameterx (GLenum target, GLenum pname, GLfixed param);
	void glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
	void glTranslatex (GLfixed x, GLfixed y, GLfixed z);
	void glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void glViewport (GLint x, GLint y, GLsizei width, GLsizei height);
};

#endif
