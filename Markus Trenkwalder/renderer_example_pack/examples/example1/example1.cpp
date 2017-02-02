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

#include "SDL.h"

// Requires software renderer source
#include "renderer/geometry_processor.h"
#include "renderer/rasterizer_subdivaffine.h"
#include "renderer/span.h"

// the software renderer stuff is located in the namespace "swr" so include 
// that here
using namespace swr;

// Our vertex structure which will be used to store our triangle data.
struct Vertex {
	float x, y;
	int r, g, b;
};

// This is the vertex shader which is executed for each individial vertex that
// needs to ne processed.
struct VertexShader {

	// This specifies that this shader is only going to use 1 vertex attribute 
	// array. There you be used up to Renderer::MAX_ATTRIBUTES arrays.
	static const unsigned attribute_count = 1;
	
	// This specifies the number of varyings the shader will output. This is
	// for instance used when clipping.
	static const unsigned varying_count = 3;

	// This static function is called for each vertex to be processed.
	// "in" is an array of void* pointers with the location of the individial 
	// vertex attributes. The "out" structure has to be written to.
	static void shade(const GeometryProcessor::VertexInput in, GeometryProcessor::VertexOutput &out)
	{
		// cast the first attribute array to the input vertex type
		const Vertex &v = *static_cast<const Vertex*>(in[0]);

		// x, y, z and w are the components that must be written by the vertex
		// shader. They all have to be specified in 16.16 fixed point format.
		out.x = static_cast<int>((v.x * (1 << 16)));
		out.y = static_cast<int>((v.y * (1 << 16)));
		out.z = 0;
		out.w = 1 << 16;

		// The vertexoutput can have up to Rasterizer::MAX_VARYING varying 
		// parameters. These are just integer values which will be interpolated 
		// across the primitives. The higher bits of these integers will be
		// interpolated more precicely so the values [0, 255] are shifted left.
		out.varyings[0] = v.r << 16;
		out.varyings[1] = v.g << 16;
		out.varyings[2] = v.b << 16;
	}
};

#define USE_GENERIC_SPAN_DRAWER 0

// This is the fragment shader
#if USE_GENERIC_SPAN_DRAWER
struct FragmentShader : public GenericSpanDrawer<FragmentShader> {
#else
struct FragmentShader : public SpanDrawer16BitColorAndDepth<FragmentShader> {
#endif
	// varying_count = 3 tells the rasterizer that it only needs to interpolate
	// three varying values (the r, g and b in this context).
	static const unsigned varying_count = 3;

	// We don't need to interpolate z in this example
	static const bool interpolate_z = false;

	// Per triangle callback. This could for instance be used to select the 
	// mipmap level of detail. We don't need it but it still needs to be defined
	// for everything to work.
	static void begin_triangle(
		const IRasterizer::Vertex& v1,
		const IRasterizer::Vertex& v2,
		const IRasterizer::Vertex& v3,
		int area2)
	{}

#if USE_GENERIC_SPAN_DRAWER
	// This static function will be called for every pixel that is to be 
	// rasterized. x and y are the screen coordinates and fd hold the data for
	// the fragment that should be drawn
	static void single_fragment(int x, int y, const IRasterizer::FragmentData &fd)
	{
		// get the location of the pixel
		SDL_Surface *screen = SDL_GetVideoSurface();
		unsigned short *color_buffer = static_cast<unsigned short*>(screen->pixels) + 
			x + y * screen->w;

		// Convert from 16.16 color format to [0,255]
		// Here the colors are clamped to the range[0,255]. If this is not done
		// here we can get very small artifacts at the edges.
		int r = std::min(std::max(fd.varyings[0] >> 16, 0), 255);
		int g = std::min(std::max(fd.varyings[1] >> 16, 0), 255);
		int b = std::min(std::max(fd.varyings[2] >> 16, 0), 255);
		*color_buffer = SDL_MapRGB(screen->format, r, g, b);
	}
#else
	
	// the fragment shader is called for each pixel and has read/write access to 
	// the destination color and depth buffers.
	static void single_fragment(
		const IRasterizer::FragmentData &fd, 
		unsigned short &color, 
		unsigned short &depth)
	{
		SDL_Surface *screen = SDL_GetVideoSurface();

		// Convert from 16.16 color format to [0,255]
		// Here the colors are clamped to the range[0,255]. If this is not done
		// here we can get very small artifacts at the edges.
		int r = std::min(std::max(fd.varyings[0] >> 16, 0), 255);
		int g = std::min(std::max(fd.varyings[1] >> 16, 0), 255);
		int b = std::min(std::max(fd.varyings[2] >> 16, 0), 255);
		color = SDL_MapRGB(screen->format, r, g, b);
	}

	// this is called by the span drawing function to get the location of the color buffer
	static void* color_pointer(int x, int y)
	{
		SDL_Surface *screen = SDL_GetVideoSurface();
		return static_cast<unsigned short*>(screen->pixels) + x + y * screen->w;
	}

	// this is called by the span drawing function to get the location of the depth buffer
	static void* depth_pointer(int x, int y)
	{
		// We don't use a depth buffer
		return 0;
	}
#endif
};

int main(int ac, char *av[]) {
	// Intialize SDL without error handling an all
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface *screen = SDL_SetVideoMode(640, 480, 16, 0);

	// The three vertices of the triangle and the colors
	Vertex vertices[] = {
		{0.0f, 0.5f, 255, 0, 0},
		{-.5f, -.5f, 0, 255, 0},
		{0.5f, -.5f, 0, 0, 255},
	};

	// The indices we need for rendering
	unsigned indices[] = {0, 1, 2};

	// Create a rasterizer class that will be used to rasterize primitives
	RasterizerSubdivAffine r;
	// Create a geometry processor class used to feed vertex data.
	GeometryProcessor g(&r);
	// It is necessary to set the viewport
	g.viewport(0, 0, screen->w, screen->h);
	// Set the cull mode (CW is already the default mode)
	g.cull_mode(GeometryProcessor::CULL_CW);

	// It is also necessary to set the clipping rectangle
	r.clip_rect(0, 0, screen->w, screen->h);

	// Set the vertex and fragment shaders
	g.vertex_shader<VertexShader>();
	r.fragment_shader<FragmentShader>();

	// Specify where out data lies in memory
	g.vertex_attrib_pointer(0, sizeof(Vertex), vertices);

	// draw the triangle
	g.draw_triangles(3, indices);

	// Show everything on screen
	SDL_Flip(SDL_GetVideoSurface());

	// Wait for the user closing the application
	SDL_Event e;
	while (SDL_WaitEvent(&e) && e.type != SDL_QUIT);

	// Quit SDL
	SDL_Quit();
	return 0;
}
