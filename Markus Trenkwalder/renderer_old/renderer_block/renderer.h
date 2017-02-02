/*
Copyright (c) 2007, Markus Trenkwalder

All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

* Neither the name of the library's copyright owner nor the names of its 
  contributors may be used to endorse or promote products derived from this 
  software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef RENDERER_H_35BBBF6B_5A83_41c4_B56A_726F99C116CB
#define RENDERER_H_35BBBF6B_5A83_41c4_B56A_726F99C116CB

#include "rasterizer.h"
#include "vertex_processor.h"

#include <vector>

#ifdef _MSC_VER
#pragma once
#endif

namespace swr {

class Renderer : private VertexProcessor<Rasterizer::Vertex, Renderer> {
	typedef VertexProcessor<Rasterizer::Vertex, Renderer> Base;
	friend class VertexProcessor<Rasterizer::Vertex, Renderer>;
public:
	enum CullMode {
		CULL_NONE, 
		CULL_CCW,
		CULL_CW
	};

	// make these inherited types and contants public
	typedef Base::VertexInput VertexInput;
	typedef Base::VertexOutput VertexOutput;

	static const int MAX_ATTRIBUTES = Base::MAX_ATTRIBUTES;

public:
	Renderer();

	// public interface

	// upper left is (0,0)
	void viewport(int x, int y, int w, int h);

	// the depth range to use. Normally from 0 to a value less than MAX_INT
	void depth_range(int n, int f);

	void vertex_attrib_pointer(int n, int stride, void* buffer);

	// count gives the number of indices
	void draw_triangles(int count, int *indices);
	void draw_lines(int count, int *indices);
	void draw_points(int count, int *indices);

	// upper left is (0,0)
	void clip_rect(int x, int y, int w, int h);
	
	void cull_mode(CullMode m);

	template <typename VertexShader>
	void vertex_shader()
	{
		Base::vertex_shader<VertexShader>();
	}

	template <typename BlockShader>
	void block_shader()
	{
		varying_count_ = BlockShader::varying_count;
		rasterizer_.block_shader<BlockShader>();
	}

private:
	void add_interp_vertex(int t, int out, int in);

	void pdiv_and_vt();

	void clip_triangles();
	void process_triangles();

	void clip_lines();
	void process_lines();

	void clip_points();
	void process_points();

private:
	// interface inherited from the vertex processor
	void process_begin();
	VertexOutput* acquire_output_location();
	bool push_vertex_index(int i);
	void process_end();

private:
	// maximim number of triangles to accumulate before going on with
	// clipping and triangle setup
	static const int MAX_TRIANGLES = 32;
	static const int MAX_LINES = 32;
	static const int MAX_POINTS = 32;

	enum DrawMode {
		DM_TRIANGLES,
		DM_LINES,
		DM_POINTS
	};

	DrawMode draw_mode;

	std::vector<VertexOutput> vertices_;
	std::vector<int> indices_;

	struct {
		int ox, oy; // origin x and y
		int px, py; // width and height divided by 2
	} viewport_;

	struct {
		int fmndiv2, npfdiv2; // (f - n)/2 | (n + f)/2
	} depth_range_;

	CullMode cull_mode_;

	Rasterizer rasterizer_;
	int varying_count_;
};

} // end namespace swr

#endif
