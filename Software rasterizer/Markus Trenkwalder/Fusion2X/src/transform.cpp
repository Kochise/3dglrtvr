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

mat4x* F2X_Context::top_matrix()
{
	GLState::TransformState &ts = gl_state.transform_state;

	mat4x *m = 0;

	switch (gl_state.transform_state.matrix_mode) {
	case GL_MODELVIEW: m = &ts.modelview_matrix[ts.modelview_stack_depth - 1]; break;
	case GL_PROJECTION: m = &ts.projection_matrix[ts.projection_stack_depth - 1]; break;
	case GL_TEXTURE: 
		const int at = gl_state.active_texture - GL_TEXTURE0;
		m = &ts.texture_matrix[at][ts.texture_stack_depth[at] - 1];
		break;
	}

	return m;
}

void F2X_Context::glMatrixMode(GLenum mode) 
{
	switch (mode) {
	case GL_MODELVIEW:
	case GL_PROJECTION:
	case GL_TEXTURE:
		gl_state.transform_state.matrix_mode = mode;
		return;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}

}

void F2X_Context::glOrthox(
						   GLfixed left, 
						   GLfixed right, 
						   GLfixed bottom, 
						   GLfixed top, 
						   GLfixed near_val, 
						   GLfixed far_val)
{
	mat4x *m = top_matrix();
	*m *= vmath::ortho_matrix<fixed16_t>(left, right, bottom, top, near_val, far_val);
}

void F2X_Context::glFrustumx(
							 GLfixed left, 
							 GLfixed right, 
							 GLfixed bottom, 
							 GLfixed top, 
							 GLfixed near_val,
							 GLfixed far_val) 
{
	mat4x *m = top_matrix();
	*m *= vmath::frustum_matrix<fixed16_t>(left, right, bottom, top, near_val, far_val);
}

void F2X_Context::glPushMatrix(void) 
{
	GLState::TransformState &ts = gl_state.transform_state;

	switch (ts.matrix_mode) {
	case GL_MODELVIEW:
		if (ts.modelview_stack_depth < gl_state.implementation.MAX_MODELVIEW_STACK_DEPTH) {
			ts.modelview_matrix[ts.modelview_stack_depth] = 
				ts.modelview_matrix[ts.modelview_stack_depth - 1];
			ts.modelview_stack_depth++;				
		} else emit_error(GL_STACK_OVERFLOW);
		break;
	case GL_PROJECTION:
		if (ts.projection_stack_depth < gl_state.implementation.MAX_PROJECTION_STACK_DEPTH) {
			ts.projection_matrix[ts.projection_stack_depth] = 
				ts.projection_matrix[ts.projection_stack_depth - 1];
			ts.projection_stack_depth++;
		} else emit_error(GL_STACK_OVERFLOW);
		break;
	case GL_TEXTURE:
		int at = gl_state.active_texture - GL_TEXTURE0;
		if (ts.texture_stack_depth[at] < gl_state.implementation.MAX_TEXTURE_STACK_DEPTH) {
			ts.texture_matrix[at][ts.texture_stack_depth[at]] = 
				ts.texture_matrix[at][ts.texture_stack_depth[at] - 1];
			ts.texture_stack_depth[at]++;
		} else emit_error(GL_STACK_OVERFLOW);
		break;
	}
}

void F2X_Context::glPopMatrix(void) 
{
	GLState::TransformState &ts = gl_state.transform_state;

	switch (ts.matrix_mode) {
	case GL_MODELVIEW:
		if (ts.modelview_stack_depth > 1)
			ts.modelview_stack_depth++;
		else emit_error(GL_STACK_UNDERFLOW);
		break;
	case GL_PROJECTION:
		if (ts.projection_stack_depth > 1)
			ts.projection_stack_depth++;
		else emit_error(GL_STACK_UNDERFLOW);
		break;
	case GL_TEXTURE:
		const int at = gl_state.active_texture - GL_TEXTURE0;
		if (ts.texture_stack_depth[at] > 1)
			ts.texture_stack_depth[at]++;
		else emit_error(GL_STACK_UNDERFLOW);
		break;
	}
}

void F2X_Context::glLoadIdentity(void) 
{
	mat4x *m = top_matrix();
	*m = vmath::identity4<fixed16_t>();
}

void F2X_Context::glLoadMatrixx(const GLfixed *m)
{
	mat4x *s = top_matrix();
	const mat4x *mm = reinterpret_cast<const mat4x*>(m);
	*s = vmath::transpose(*mm);
}

void F2X_Context::glMultMatrixx(const GLfixed *m)
{
	mat4x *s = top_matrix();
	const mat4x *mm = reinterpret_cast<const mat4x*>(m);
	(*s) *= vmath::transpose(*mm);
}

void F2X_Context::glRotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
	mat4x *s = top_matrix();

	fixed16_t a; a.intValue = angle;
	vec3x v; v.x.intValue = x; v.y.intValue = y; v.z.intValue = z;

	(*s) *= vmath::rotation_matrix<fixed16_t>(a, v);
}

void F2X_Context::glScalex(GLfixed x, GLfixed y, GLfixed z)
{
	mat4x *s = top_matrix();

	vec3x v;
	v.x.intValue = x;
	v.y.intValue = y;
	v.z.intValue = z;

	(*s) *= vmath::scaling_matrix<fixed16_t>(v);
}

void F2X_Context::glTranslatex(GLfixed x, GLfixed y, GLfixed z)
{
	mat4x *s = top_matrix();
	
	vec3x v;
	v.x.intValue = x;
	v.y.intValue = y;
	v.z.intValue = z;

	(*s) *= vmath::translation_matrix<fixed16_t>(v);
}
