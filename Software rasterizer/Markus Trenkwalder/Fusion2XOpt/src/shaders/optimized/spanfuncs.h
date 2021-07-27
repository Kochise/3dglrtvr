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

#ifndef OPTIMIZED_SPANFUNCS_SHADER_H
#define OPTIMIZED_SPANFUNCS_SHADER_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "../../renderer/irasterizer.h"
#include "../../gltexobj.h"

// forward declare
struct F2X_Context;

void color_span(
	unsigned short *out, 
	const swr::IRasterizer::FragmentData &fd, 
	const swr::IRasterizer::FragmentData &step,
	unsigned base,
	unsigned n);

void alpha_span(
	unsigned short *out,
	const swr::IRasterizer::FragmentData &fd,
	const swr::IRasterizer::FragmentData &step,
	unsigned base,
	unsigned n);

void fog_span(
	unsigned short *inout,
	unsigned short fog_color,
	const swr::IRasterizer::FragmentData &fd,
	const swr::IRasterizer::FragmentData &step,
	unsigned base,
	unsigned n);

void depth_span(
	unsigned short *out,
	const swr::IRasterizer::FragmentData &fd,
	const swr::IRasterizer::FragmentData &step,
	unsigned n);

namespace OptimizedSpanFunctions 
{
	struct TextureSpanDesc {
		int width;
		int height;
		GLenum texture_wrap_s;
		GLenum texture_wrap_t;
		GLenum internal_format;
	};

	typedef void (*texture_span_t) (
		const GLTextureObject *tex,
		unsigned short *color_out,
		unsigned short *alpha_out,
		const swr::IRasterizer::FragmentData &fd,
		const swr::IRasterizer::FragmentData &step,
		unsigned base,
		unsigned n);

	struct TextureEnvSpanDesc {
		GLenum texture_env_mode;
		bool do_alpha;
	};

	typedef void (*texture_env_span_t)(
		const F2X_Context *ctx,
		unsigned short *fcolor,
		unsigned short *falpha,
		const unsigned short *tcolor,
		const unsigned short *talpha,
		unsigned short n);

	typedef void (*alpha_test_span_t)(
		const F2X_Context *ctx,
		const unsigned short *alpha,
		unsigned short *out,
		unsigned n);

	typedef void (*depth_test_span_t)(
		const F2X_Context *ctx,
		const unsigned short *src,
		const unsigned short *dst,
		unsigned short *out,
		unsigned n);

	struct BlendSpanDesc {
		GLenum src_factor;
		GLenum dst_factor;
	};

	typedef void (*blend_span_t)(
		const F2X_Context *ctx,
		unsigned short *src_color,
		const unsigned short *src_alpha,
		const unsigned short *dst_color,
		unsigned n);

	texture_span_t get_texture_span(const TextureSpanDesc &span_desc);
	texture_env_span_t get_texture_env_span(const TextureEnvSpanDesc &desc);
	alpha_test_span_t get_alpha_test_span(GLenum func);
	depth_test_span_t get_depth_test_span(GLenum func);
	blend_span_t get_blend_span(const BlendSpanDesc &func);
}

#endif
