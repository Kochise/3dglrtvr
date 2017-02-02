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

#include "GLES/gl.h"
#include "context.h"

void glActiveTexture(GLenum texture)
{
	if (!current_context) return;
	current_context->glActiveTexture(texture);
}

void glAlphaFuncx(GLenum func, GLclampx ref)
{
	if (!current_context) return;
	current_context->glAlphaFuncx(func, ref);
}

void glBindTexture(GLenum target, GLuint texture)
{
	if (!current_context) return;
	current_context->glBindTexture(target, texture);
}

void glBlendFunc(GLenum sfactor, GLenum dfactor)
{
	if (!current_context) return;
	current_context->glBlendFunc(sfactor, dfactor);
}

void glClear(GLbitfield mask)
{
	if (!current_context) return;
	current_context->glClear(mask);
}

void glClearColorx(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
	if (!current_context) return;
	current_context->glClearColorx(red, green, blue, alpha);
}

void glClearDepthx(GLclampx depth)
{
	if (!current_context) return;
	current_context->glClearDepthx(depth);
}

void glClearStencil(GLint s)
{
	if (!current_context) return;
	current_context->glClearStencil(s);
}

void glClientActiveTexture(GLenum texture)
{
	if (!current_context) return;
	current_context->glClientActiveTexture(texture);
}

void glColor4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
	if (!current_context) return;
	current_context->glColor4x(red, green, blue, alpha);
}

void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	if (!current_context) return;
	current_context->glColorMask(red, green, blue, alpha);
}

void glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	if (!current_context) return;
	current_context->glColorPointer(size, type, stride, pointer);
}

void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
{
	if (!current_context) return;
	current_context->glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
}

void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data)
{
	if (!current_context) return;
	current_context->glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
	if (!current_context) return;
	current_context->glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
}

void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	if (!current_context) return;
	current_context->glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

void glCullFace(GLenum mode)
{
	if (!current_context) return;
	current_context->glCullFace(mode);
}

void glDeleteTextures(GLsizei n, const GLuint *textures)
{
	if (!current_context) return;
	current_context->glDeleteTextures(n, textures);
}

void glDepthFunc(GLenum func)
{
	if (!current_context) return;
	current_context->glDepthFunc(func);
}

void glDepthMask(GLboolean flag)
{
	if (!current_context) return;
	current_context->glDepthMask(flag);
}

void glDepthRangex(GLclampx zNear, GLclampx zFar)
{
	if (!current_context) return;
	current_context->glDepthRangex(zNear, zFar);
}

void glDisable(GLenum cap)
{
	if (!current_context) return;
	current_context->glDisable(cap);
}

void glDisableClientState(GLenum array)
{
	if (!current_context) return;
	current_context->glDisableClientState(array);
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	if (!current_context) return;
	current_context->glDrawArrays(mode, first, count);
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	if (!current_context) return;
	current_context->glDrawElements(mode, count, type, indices);
}

void glEnable(GLenum cap)
{
	if (!current_context) return;
	current_context->glEnable(cap);
}

void glEnableClientState(GLenum array)
{
	if (!current_context) return;
	current_context->glEnableClientState(array);
}

void glFinish(void)
{
	if (!current_context) return;
	current_context->glFinish();
}

void glFlush(void)
{
	if (!current_context) return;
	current_context->glFlush();
}

void glFogx(GLenum pname, GLfixed param)
{
	if (!current_context) return;
	current_context->glFogx(pname, param);
}

void glFogxv(GLenum pname, const GLfixed *params)
{
	if (!current_context) return;
	current_context->glFogxv(pname, params);
}

void glFrontFace(GLenum mode)
{
	if (!current_context) return;
	current_context->glFrontFace(mode);
}

void glFrustumx(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
	if (!current_context) return;
	current_context->glFrustumx(left, right, bottom, top, zNear, zFar);
}

void glGenTextures(GLsizei n, GLuint *textures)
{
	if (!current_context) return;
	current_context->glGenTextures(n, textures);
}

GLenum glGetError(void)
{
	if (!current_context) return GL_INVALID_OPERATION;
	return current_context->glGetError();
}

void glGetIntegerv(GLenum pname, GLint *params)
{
	if (!current_context) return;
	current_context->glGetIntegerv(pname, params);
}

const GLubyte * glGetString(GLenum name)
{
	if (!current_context) return 0;
	return current_context->glGetString(name);
}

void glHint(GLenum target, GLenum mode)
{
	if (!current_context) return;
	current_context->glHint(target, mode);
}

void glLightModelx(GLenum pname, GLfixed param)
{
	if (!current_context) return;
	current_context->glLightModelx(pname, param);
}

void glLightModelxv(GLenum pname, const GLfixed *params)
{
	if (!current_context) return;
	current_context->glLightModelxv(pname, params);
}

void glLightx(GLenum light, GLenum pname, GLfixed param)
{
	if (!current_context) return;
	current_context->glLightx(light, pname, param);
}

void glLightxv(GLenum light, GLenum pname, const GLfixed *params)
{
	if (!current_context) return;
	current_context->glLightxv(light, pname, params);
}

void glLineWidthx(GLfixed width)
{
	if (!current_context) return;
	current_context->glLineWidthx(width);
}

void glLoadIdentity(void)
{
	if (!current_context) return;
	current_context->glLoadIdentity();
}

void glLoadMatrixx(const GLfixed *m)
{
	if (!current_context) return;
	current_context->glLoadMatrixx(m);
}

void glLogicOp(GLenum opcode)
{
	if (!current_context) return;
	current_context->glLogicOp(opcode);
}

void glMaterialx(GLenum face, GLenum pname, GLfixed param)
{
	if (!current_context) return;
	current_context->glMaterialx(face, pname, param);
}

void glMaterialxv(GLenum face, GLenum pname, const GLfixed *params)
{
	if (!current_context) return;
	current_context->glMaterialxv(face, pname, params);
}

void glMatrixMode(GLenum mode)
{
	if (!current_context) return;
	current_context->glMatrixMode(mode);
}

void glMultMatrixx(const GLfixed *m)
{
	if (!current_context) return;
	current_context->glMultMatrixx(m);
}

void glMultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
	if (!current_context) return;
	current_context->glMultiTexCoord4x(target, s, t, r, q);
}

void glNormal3x(GLfixed nx, GLfixed ny, GLfixed nz)
{
	if (!current_context) return;
	current_context->glNormal3x(nx, ny, nz);
}

void glNormalPointer(GLenum type, GLsizei stride, const GLvoid *pointer)
{
	if (!current_context) return;
	current_context->glNormalPointer(type, stride, pointer);
}

void glOrthox(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
	if (!current_context) return;
	current_context->glOrthox(left, right, bottom, top, zNear, zFar);
}

void glPixelStorei(GLenum pname, GLint param)
{
	if (!current_context) return;
	current_context->glPixelStorei(pname, param);
}

void glPointSizex(GLfixed size)
{
	if (!current_context) return;
	current_context->glPointSizex(size);
}

void glPolygonOffsetx(GLfixed factor, GLfixed units)
{
	if (!current_context) return;
	current_context->glPolygonOffsetx(factor, units);
}

void glPopMatrix(void)
{
	if (!current_context) return;
	current_context->glPopMatrix();
}

void glPushMatrix(void)
{
	if (!current_context) return;
	current_context->glPushMatrix();
}

void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
	if (!current_context) return;
	current_context->glReadPixels(x, y, width, height, format, type, pixels);
}

void glRotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
	if (!current_context) return;
	current_context->glRotatex(angle, x, y, z);
}

void glSampleCoveragex(GLclampx value, GLboolean invert)
{
	if (!current_context) return;
	current_context->glSampleCoveragex(value, invert);
}

void glScalex(GLfixed x, GLfixed y, GLfixed z)
{
	if (!current_context) return;
	current_context->glScalex(x, y, z);
}

void glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
	if (!current_context) return;
	current_context->glScissor(x, y, width, height);
}

void glShadeModel(GLenum mode)
{
	if (!current_context) return;
	current_context->glShadeModel(mode);
}

void glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
	if (!current_context) return;
	current_context->glStencilFunc(func, ref, mask);
}

void glStencilMask(GLuint mask)
{
	if (!current_context) return;
	current_context->glStencilMask(mask);
}

void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
	if (!current_context) return;
	current_context->glStencilOp(fail, zfail, zpass);
}

void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	if (!current_context) return;
	current_context->glTexCoordPointer(size, type, stride, pointer);
}

void glTexEnvx(GLenum target, GLenum pname, GLfixed param)
{
	if (!current_context) return;
	current_context->glTexEnvx(target, pname, param);
}

void glTexEnvxv(GLenum target, GLenum pname, const GLfixed *params)
{
	if (!current_context) return;
	current_context->glTexEnvxv(target, pname, params);
}

void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
	if (!current_context) return;
	current_context->glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

void glTexParameterx(GLenum target, GLenum pname, GLfixed param)
{
	if (!current_context) return;
	current_context->glTexParameterx(target, pname, param);
}

void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
	if (!current_context) return;
	current_context->glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

void glTranslatex(GLfixed x, GLfixed y, GLfixed z)
{
	if (!current_context) return;
	current_context->glTranslatex(x, y, z);
}

void glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	if (!current_context) return;
	current_context->glVertexPointer(size, type, stride, pointer);
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	if (!current_context) return;
	current_context->glViewport(x, y, width, height);
}

