/*
Copyright (c) 2007, 2008 Markus Trenkwalder

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

// requires software renderer source
#include "renderer/geometry_processor.h"
#include "renderer/rasterizer_subdivaffine.h"
#include "renderer/span.h"

// other includes
#include "vector_math.h"
#include "objdata.h"
#include "fixedpoint/fixed_class.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

#ifdef GP2X
#include "gp2x.h"
#endif

using namespace swr; // namespace for software renderer stuff
using namespace vmath; // vector math library
using namespace fixedpoint; // fixed point library

// some typedefs for easier use
typedef fixed_point<16> fixed16_t;

typedef vec2<fixed16_t> vec2x;
typedef vec3<fixed16_t> vec3x;
typedef vec4<fixed16_t> vec4x;

typedef mat2<fixed16_t> mat2x;
typedef mat3<fixed16_t> mat3x;
typedef mat4<fixed16_t> mat4x;

// vertex structure which will be used to store per vertex data which
// is passed to the vertex shader.
struct MyVertex {
	vec3x position;
	vec3x normal;
};

// this is the vertex shader which is executed for each individual vertex that
// needs to be processed.
struct MyVertexShader {

	// this specifies that this shader is only going to use 1 vertex attribute 
	// array. there can be used up to GeometryProcessor::MAX_ATTRIBUTES arrays.
	static const unsigned attribute_count = 1;

	// this specifies the number of varying outputs the vertex shader will write.
	// this information is used for clipping purposes.
	static const unsigned varying_count = 1;

	// this static function is called for each vertex to be processed.
	// "in" is an array of void* pointers with the location of the individial 
	// vertex attributes. the "out" structure has to be written to.
	static void shade(const GeometryProcessor::VertexInput in, GeometryProcessor::VertexOutput &out)
	{
		// some constants used
		static const fixed16_t half = 0.5f;
		static const fixed16_t one = 1.0f;

		// cast the first attribute array to the input vertex type.
		const MyVertex &v = *static_cast<const MyVertex*>(in[0]);

		// transform the vertex by the transformation matrix
		vec4x t = model_view_projection_matrix_ * vec4x(v.position, one);

		// x, y, z and w are the components that must be written by the vertex
		// shader. they all have to be specified in 16.16 fixed point format.
		out.x = t.x.intValue;
		out.y = t.y.intValue;
		out.z = t.z.intValue;
		out.w = t.w.intValue;

		// calculate the lighting. doing it this way we will also get some nice
		// shading on the back side of the model.
		fixed16_t lighting = dot(v.normal, light_dir_) * half + half;
		
		// write the varying which will be interpolated across the triangle.
		out.varyings[0] = 31 * lighting.intValue;
	}

	// variables the shader will use.
	static vec3x light_dir_;
	static mat4x model_view_projection_matrix_;
};

// define the vertex shader variables so the linker does not complain.
vec3x MyVertexShader::light_dir_= normalize<fixed16_t>(vec3x(10.0f, 10.0f, 10.f));
mat4x MyVertexShader::model_view_projection_matrix_;

// global depth buffer we will use.
SDL_Surface * depth_buffer;

// global drawing surface.
SDL_Surface * shadow;

#define USE_GENERIC_SPAN_DRAWER 0

// this is the fragment shader.
#if USE_GENERIC_SPAN_DRAWER
struct MyFragmentShader : public GenericSpanDrawer<MyFragmentShader> {
#else
struct MyFragmentShader : public SpanDrawer16BitColorAndDepth<MyFragmentShader> {
#endif
	// varying_count = 1 tells the rasterizer that it only needs to interpolate
	// one varying value which will be the lighting value computed per vertex.
	static const unsigned varying_count = 1;
	
	// we also want to interpolate the depth value because we do depth testing.
	static const bool interpolate_z = true;

	// per triangle callback. this could for instance be used to select the 
	// mipmap level of detail. we don't need it but it still needs to be defined
	// for everything to work.
	static void begin_triangle(
		const IRasterizer::Vertex& /*v1*/,
		const IRasterizer::Vertex& /*v2*/,
		const IRasterizer::Vertex& /*v3*/,
		int /*area2*/)
	{}


	// this static function will be called for every pixel that is to be 
	// rasterized. x and y are the screen coordinates and fd holds the data for
	// the fragment that should be drawn.
#if USE_GENERIC_SPAN_DRAWER
	// this one is to be used with the GenericSpanDrawer
	static void single_fragment(int x, int y, const IRasterizer::FragmentData &fd)
	{
		// get the depth buffer pixel from x, y parameters.
		unsigned short *db = static_cast<unsigned short*>(depth_buffer->pixels) +
			x + y * depth_buffer->w;

		// do the depth test (note: we have a 16bit depth buffer, therefore the shift).
		unsigned short depth = fd.z >> 16;
		if (*db < depth) return;
		*db = depth;

		// get the color buffer pixel.
		unsigned short *color_buffer = static_cast<unsigned short*>(shadow->pixels) + 
			x + y * shadow->w;

		// make sure the interpolated value lies in the correct range.
		int s = std::min(std::max(fd.varyings[0] >> 16, 0), 31);

		// write the color to the buffer.
		*color_buffer = (s << 11) | (s << 6) | s;
	}
#else
	
	// this is the fragment shader. you can read the color and depth variables to access the destination 
	// buffers and write to them to update the buffers.
	static void single_fragment(
		const IRasterizer::FragmentData &fd, 
		unsigned short &color, 
		unsigned short &depth)
	{
		unsigned short d = fd.z >> 16;
		if (depth < d) return;
		depth = d;
	
		int s = std::min(std::max(fd.varyings[0] >> 16, 0), 31);
		color = (s << 11) | (s << 6) | s;
	}

	// this is called by the span drawing function to get the location of the color buffer
	static void* color_pointer(int x, int y)
	{
		return static_cast<unsigned short*>(shadow->pixels) + x + y * shadow->w;
	}

	// this is called by the span drawing function to get the location of the depth buffer
	static void* depth_pointer(int x, int y)
	{
		return static_cast<unsigned short*>(depth_buffer->pixels) + x + y * depth_buffer->w;
	}
#endif
};

// conversion function used to convert the float vectors 
// loaded from the .obj file to fixed point.
inline vec3x convert(const vec3<float> & rhs)
{
	return vec3x(rhs.x, rhs.y, rhs.z);
}

int main(int /*ac*/, char * /*av*/[])
{
	Uint32 flags = SDL_INIT_VIDEO;

#ifdef GP2X
	flags |= SDL_INIT_JOYSTICK;
#endif

	// Initialize SDL without error handling
	SDL_Init(flags);
	SDL_Surface * screen = SDL_SetVideoMode(320, 240, 16, SDL_HWSURFACE);

#ifdef GP2X
	// disable the cursor on the GP2X
	SDL_ShowCursor(SDL_DISABLE);

	// used to access GP2X buttons.
	SDL_JoystickOpen(0);
#endif
	// we will render into the shadow surface and blit to the main screen at the end.
	// doing it this way one can circumvent double buffering on the GP2X 
	// and thus render faster.
	shadow = SDL_DisplayFormat(screen);

	// create a 16bit depth buffer. making it a hw surface will allow us 
	// to do fast clears and using the mmuhack on the GP2X also gives us fast 
	// access to the memory.
	depth_buffer = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 16, 0, 0, 0, 0);

#ifdef GP2X
	gp2x_init();
#endif

	// create a rasterizer class that will be used to rasterzie primitives.
	RasterizerSubdivAffine r;
	// create a geometry processor class used to feed vertex data.
	GeometryProcessor g(&r);
	// it is necessary to set the viewport.
	g.viewport(0, 0, screen->w, screen->h);
	// set the cull mode (cw is already the default mode).
	g.cull_mode(GeometryProcessor::CULL_CW);

	// it is also necessary to set the clipping rectangle.
	r.clip_rect(0, 0, screen->w, screen->h);

	// set the vertex and fragment shaders.
	g.vertex_shader<MyVertexShader>();
	r.fragment_shader<MyFragmentShader>();

	// disable perspective correction for faster rendering.
	// in this demo the difference won't be noticeable since the triangles 
	// are all small enough and we only use vertex colors and no textures.
	r.perspective_correction(false);

	std::vector<unsigned> idata;
	std::vector<MyVertex> vdata;

	{
		ObjData mesh = ObjData::load_from_file("data/cow.obj");
		std::vector<ObjData::VertexArrayData> vdata_tmp;
		mesh.to_vertex_array(vdata_tmp, idata);
		vdata.resize(vdata_tmp.size());
		for (size_t i = 0; i < vdata_tmp.size(); ++i) {
			vdata[i].position = convert(vdata_tmp[i].vertex);
			vdata[i].normal = convert(vdata_tmp[i].normal);
		}
	}

	// specify where out data lies in memory.
	g.vertex_attrib_pointer(0, sizeof(MyVertex), &vdata[0]);

	// output some information
	std::cout << "vertices: " << vdata.size() << std::endl;
	std::cout << "faces: " << idata.size() / 3 << std::endl;

	// rendering loop
	while (true) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_JOYBUTTONDOWN: /* fall through */
			case SDL_QUIT: goto end;
			}
		}

		// clear the screen and the depth buffer.
		SDL_FillRect(shadow, 0, 0);
		SDL_FillRect(depth_buffer, 0, 0xffffffff);

		// create a transformation depending on the time to rotate around the object.
		fixed16_t time(SDL_GetTicks() / 1000.0f);
		vec3x eye(cos(time) * fixed16_t(10.0f), 0.0f, sin(time) * fixed16_t(10.0f));

		MyVertexShader::model_view_projection_matrix_ = 
			perspective_matrix<fixed16_t>(60.0f, 4.0f/3.0f, 0.5f, 100.0f) *
			lookat_matrix(eye, vec3x(0.0f), vec3x(0.0f, 1.0f, 0.0f));

		// lock the surfaces as we want to directly write to them.
		const int r1 = SDL_LockSurface(depth_buffer);
		const int r2 = SDL_LockSurface(shadow);
		
		// draw the mesh by sending the vertex data to the pipeline (only when locking worked).
		if (!r1 && !r2)	{
			g.draw_triangles(idata.size(), &idata[0]);
		#ifdef GP2X
			flush_uppermem_cache(
				shadow->pixels, 
				static_cast<char*>(shadow->pixels) + shadow->pitch * shadow->h,
				0);
		#endif
		}

		// unlock them again.
		if (!r2) SDL_UnlockSurface(shadow);
		if (!r2) SDL_UnlockSurface(depth_buffer);

		// blit shadow surface to screen and show the screen.
		SDL_BlitSurface(shadow, 0, screen, 0);
		SDL_Flip(screen);

		// fps counter
		{			
			static int frames = 0;
			frames++;

			static Uint32 last_call = SDL_GetTicks();
			if (SDL_GetTicks() - last_call >= 1000) {
				std::cout << "fps: " << frames << std::endl;
				last_call = SDL_GetTicks();
				frames = 0;
			}
		}
	}
end:
	// free the created surfaces.
	SDL_FreeSurface(shadow);
	SDL_FreeSurface(depth_buffer);

	// quit SDL.
	SDL_Quit();
	return 0;
}
