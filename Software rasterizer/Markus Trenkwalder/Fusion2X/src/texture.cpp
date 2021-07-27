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

void F2X_Context::glTexParameterx(GLenum target, GLenum pname, GLfixed param)
{
	if (target != GL_TEXTURE_2D) {
		emit_error(GL_INVALID_ENUM);
		return;
	}

	GLTextureObject *tex = 
		gl_state.texture_state.get_texture_object(target, gl_state.texture_state.binding_2d);

	switch (pname) {
	case GL_TEXTURE_MIN_FILTER:
		switch (param) {
		case GL_NEAREST:
		case GL_LINEAR:
		case GL_NEAREST_MIPMAP_NEAREST:
		case GL_NEAREST_MIPMAP_LINEAR:
		case GL_LINEAR_MIPMAP_NEAREST:
		case GL_LINEAR_MIPMAP_LINEAR:
			tex->texture_min_filter = param;
			return;
		default:
			emit_error(GL_INVALID_VALUE);
			return;
		}
	case GL_TEXTURE_MAG_FILTER:
		switch (param) {
		case GL_NEAREST:
		case GL_LINEAR:
			tex->texture_mag_filter = param;
			return;
		default:
			emit_error(GL_INVALID_VALUE);
			return;
		}
	case GL_TEXTURE_WRAP_S:
		switch (param) {
		case GL_REPEAT:
		case GL_CLAMP_TO_EDGE:
			tex->texture_wrap_s = param;
			return;
		default:
			emit_error(GL_INVALID_VALUE);
			return;
		}
	case GL_TEXTURE_WRAP_T:
		switch (param) {
		case GL_REPEAT:
		case GL_CLAMP_TO_EDGE:
			tex->texture_wrap_t = param;
			return;
		default:
			emit_error(GL_INVALID_VALUE);
			return;
		}
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}	
}

void F2X_Context::glBindTexture(GLenum target, GLuint texture) try
{
	if (target != GL_TEXTURE_2D) {
		emit_error(GL_INVALID_ENUM);
		return;
	}

	GLTextureObject *tex = gl_state.texture_state.get_texture_object(target, texture);

	if (tex && tex->target != target) {
		emit_error(GL_INVALID_OPERATION);
		return;
	}

	// allocate texture object if it does not already exist
	if (!tex) {
		tex = new GLTextureObject(target);
		gl_state.texture_state.texture_id_map[texture] = tex;
	}

	gl_state.texture_state.binding_2d = texture;
	gl_state.texture_state.binding_2d_tex = tex;

} catch (...) {
	emit_error(GL_OUT_OF_MEMORY);
}

namespace {
	inline bool is_pot(unsigned i) 
	{
		return (i & (i - 1)) == 0;
	}

	unsigned log2_of_pot(unsigned i) 
	{
		unsigned r = 0;
		while (!(i & 1)) {
			i >>= 1;
			++r;
		}
		return r;
	}
};

void F2X_Context::glTexImage2D(
	GLenum target, 
	GLint level, 
	GLint internalformat, 
	GLsizei width, 
	GLsizei height, 
	GLint border, 
	GLenum format, 
	GLenum type, 
	const GLvoid *pixels) try
{
	if (target != GL_TEXTURE_2D) {
		emit_error(GL_INVALID_ENUM);
		return;
	}

	if (width < 0 || height < 0 || border != 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	// HACK: only support the first mip level
	if (level != 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	// size for each pixel in texture memory
	unsigned internal_pixel_size = 0;

	switch (internalformat) {
	case GL_RGB: 
	case GL_RGBA:
	case GL_LUMINANCE_ALPHA:
		internal_pixel_size = 2; 
		break;
	case GL_ALPHA:
	case GL_LUMINANCE:
		internal_pixel_size = 1;
		break;
	default:
		emit_error(GL_INVALID_ENUM);
		return;
	}

	if (width > gl_state.implementation.MAX_TEXTURE_SIZE || 
		height > gl_state.implementation.MAX_TEXTURE_SIZE ||
		width < 1 ||
		height < 1 ||
		!is_pot(width) || 
		!is_pot(height))
	{
		emit_error(GL_INVALID_VALUE);
		return;
	}

	if (static_cast<signed>(format) != internalformat) {
		emit_error(GL_INVALID_OPERATION);
		return;
	}

	// size of pixel from client memory
	unsigned external_pixel_size = 0;

	switch (format) {
	case GL_RGB:
		switch (type) {
		case GL_UNSIGNED_BYTE: external_pixel_size = 3; break;
		case GL_UNSIGNED_SHORT_5_6_5: external_pixel_size = 2; break;
		default:
			emit_error(GL_INVALID_OPERATION);
			return;
		}
		break;
	case GL_RGBA:
		switch (type) {
		case GL_UNSIGNED_BYTE: 
			external_pixel_size = 4; 
			break;
		case GL_UNSIGNED_SHORT_5_5_5_1:
		case GL_UNSIGNED_SHORT_4_4_4_4:
			external_pixel_size = 2;
			break;
		default:
			emit_error(GL_INVALID_OPERATION);
			return;
		}
		break;
	case GL_ALPHA:
	case GL_LUMINANCE:
	case GL_LUMINANCE_ALPHA:
		if (type != GL_UNSIGNED_BYTE) {
			emit_error(GL_INVALID_OPERATION);
			return;
		}
		external_pixel_size = format == GL_LUMINANCE_ALPHA ? 2 : 1;
		break;
	}

	// HACK: only support partial set of format and type combinations.
	// this block can be removed if everything is supported
	{
		switch (format) {
		case GL_RGB:
			switch (type) {
			case GL_UNSIGNED_BYTE:
			case GL_UNSIGNED_SHORT_5_6_5:
				break;
			default:
				emit_error(GL_INVALID_OPERATION);
				return;
			}
			break;
		case GL_RGBA:
			switch (type) {
			case GL_UNSIGNED_SHORT_5_5_5_1:
			case GL_UNSIGNED_BYTE:
				break;
			default:
				emit_error(GL_INVALID_OPERATION);
				return;
			}
			break;
		}
	}

	GLTextureObject *tex = 
		gl_state.texture_state.get_texture_object(target, gl_state.texture_state.binding_2d);

	// set texture object state
	tex->width = width;
	tex->height = height;
	tex->width_minus_one = width - 1;
	tex->height_minus_one = height - 1;
	tex->width_log_two = log2_of_pot(width);
	tex->height_log_two = log2_of_pot(height);
	tex->internalformat = internalformat;

	std::vector<unsigned char>(width * height * internal_pixel_size, 0).swap(tex->data);	

	const GLint upa = gl_state.pixel_transfer.unpack_alignment;
	const int row_skip_bytes = ((width * external_pixel_size + upa - 1) / upa) * upa;

	const int rowlength = row_skip_bytes;
	const unsigned char *row = static_cast<const unsigned char*>(pixels);
	unsigned short *col = reinterpret_cast<unsigned short*>(&tex->data[0]);
	for (int y = 0; y < height; ++y) {
		switch (type) {
		case GL_UNSIGNED_SHORT_5_6_5:
			{
				const unsigned short *p = reinterpret_cast<const unsigned short*>(row);
				unsigned short *c = col;
				for (int x = 0; x < width; ++x) {
					*c++ = *p++;
				}
				break;
			}
		case GL_UNSIGNED_SHORT_5_5_5_1:
			{
				const unsigned short *p = reinterpret_cast<const unsigned short*>(row);
				unsigned short *c = col;
				for (int x = 0; x < width; ++x) {
					*c++ = (*p & 0xf800) | (*p & 0x7c0) | ((*p & 1) << 5) | ((*p >> 1) & 0x1f); ++p;
				}
				break;
			}
		case GL_UNSIGNED_BYTE:
			{
				const unsigned char *p = row;
				unsigned short *c = col;
				if (format == GL_RGB) {
					for (int x = 0; x < width; ++x) {
						const int r = (*p++) >> 3;
						const int g = (*p++) >> 2;
						const int b = (*p++) >> 3;
						*c++ = (r << 11) | (g << 5) | b;
					}
				} else {
					for (int x = 0; x < width; ++x) {
						const int r = (*p++) >> 3;
						const int g = (*p++) >> 3;
						const int b = (*p++) >> 3;
						const int a = (*p++) >> 7;
						*c++ = (r << 11) | (g << 6) | (a << 5) | b;
					}
				}
				break;
			}
		}
		row += rowlength;
		col += width;
	}

	// TODO: implement other formats
} catch (...) {
	emit_error(GL_OUT_OF_MEMORY);
}

void F2X_Context::glTexSubImage2D(
	GLenum target, 
	GLint level, 
	GLint xoffset, 
	GLint yoffset, 
	GLsizei width,
	GLsizei height, 
	GLenum format, 
	GLenum type, 
	const GLvoid *pixels)
{
	if (width < 0 || height < 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	// TODO: implement

	emit_error(GL_INVALID_OPERATION);
}

void F2X_Context::glCompressedTexImage2D(
	GLenum target, 
	GLint level, 
	GLenum internalformat, 
	GLsizei width, 
	GLsizei height, 
	GLint border, 
	GLsizei imageSize, 
	const GLvoid *data)
{
	if (target != GL_TEXTURE_2D) {
		emit_error(GL_INVALID_ENUM);
		return;
	}

	if (width < 0 || height < 0 || border != 0 || imageSize < 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	// TODO: implement

	emit_error(GL_INVALID_OPERATION);
}

void F2X_Context::glCompressedTexSubImage2D(
	GLenum target, 
	GLint level, 
	GLint xoffset, 
	GLint yoffset, 
	GLsizei width, 
	GLsizei height, 
	GLenum format, 
	GLsizei imageSize, 
	const GLvoid *data)
{
	if (target != GL_TEXTURE_2D) {
		emit_error(GL_INVALID_ENUM);
		return;
	}

	if (width < 0 || height < 0 || imageSize < 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	// TODO: implement

	emit_error(GL_INVALID_OPERATION);
}

void F2X_Context::glCopyTexImage2D(
	GLenum target, 
	GLint level, 
	GLenum internalformat, 
	GLint x, 
	GLint y, 
	GLsizei width, 
	GLsizei height, 
	GLint border)
{
	if (target != GL_TEXTURE_2D) {
		emit_error(GL_INVALID_ENUM);
		return;
	}

	if (width < 0 || height < 0 || border != 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	// TODO: implement

	emit_error(GL_INVALID_OPERATION);
}

void F2X_Context::glCopyTexSubImage2D(
	GLenum target, 
	GLint level, 
	GLint xoffset, 
	GLint yoffset, 
	GLint x, 
	GLint y, 
	GLsizei width, 
	GLsizei height)
{
	if (target != GL_TEXTURE_2D) {
		emit_error(GL_INVALID_ENUM);
		return;
	}

	if (width < 0 || height < 0) {
		emit_error(GL_INVALID_VALUE);
		return;
	}

	// TODO: implement

	emit_error(GL_INVALID_OPERATION);
}

void F2X_Context::glGenTextures(GLsizei n, GLuint *textures)
{
	while (n--) {
		if (gl_state.gentextures.free.size() != 0) {
			GLuint id = gl_state.gentextures.free.back();
			gl_state.gentextures.free.pop_back();
			*textures++ = id;
		} else *textures++ = ++gl_state.gentextures.max;
	}
}

void F2X_Context::glDeleteTextures(GLsizei n, const GLuint *textures)
{
	while (n--) {
		if (*textures == 0) continue;

		gl_state.gentextures.free.push_back(*textures);

		GLState::TextureState::texture_id_map_t::iterator i = 
			gl_state.texture_state.texture_id_map.find(*textures);
		delete i->second;
		gl_state.texture_state.texture_id_map.erase(i);

		textures++;
	}
}

void F2X_Context::glActiveTexture(GLenum texture)
{
	if (texture < GL_TEXTURE0 || texture > GL_TEXTURE0 + gl_state.implementation.MAX_TEXTURE_UNITS) {
		emit_error(GL_INVALID_ENUM);
		return;
	}

	gl_state.active_texture = texture;
}

void F2X_Context::glClientActiveTexture(GLenum texture)
{
	if (texture < GL_TEXTURE0 || texture > GL_TEXTURE0 + gl_state.implementation.MAX_TEXTURE_UNITS) {
		emit_error(GL_INVALID_ENUM);
		return;
	}

	gl_state.client_active_texture = texture;
}
