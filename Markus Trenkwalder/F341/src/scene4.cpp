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

#include "scene4.h"

#include "SDL.h"
#include "SDL_image.h"
#include "renderer/geometry_processor.h"
#include "renderer/rasterizer_subdivaffine.h"
#include "renderer/span.h"
#include "vector_math.h"
#include "fixedpoint/fixed_func.h"
#include "democommon.h"

using namespace swr;

namespace {
	float cube_vertices[] = {
		-1.000000f, 1.000000f, -1.000000f, 0.000000f, 1.000000f, 
		1.000000f, 1.000000f, -1.000000f, 0.000000f, 0.000000f, 
		1.000000f, 0.999999f, 1.000000f, 1.000000f, 0.000000f, 
		-1.000000f, 1.000000f, -1.000000f, 0.000000f, 1.000000f, 
		1.000000f, 0.999999f, 1.000000f, 1.000000f, 0.000000f, 
		-1.000000f, 1.000000f, 1.000000f, 1.000000f, 1.000000f, 
		-1.000000f, -1.000000f, -1.000000f, 0.000000f, 1.000000f, 
		-1.000000f, 1.000000f, -1.000000f, 0.000000f, 0.000000f, 
		-1.000000f, 1.000000f, 1.000000f, 1.000000f, 0.000000f, 
		-1.000000f, -1.000000f, -1.000000f, 0.000000f, 1.000000f, 
		-1.000000f, 1.000000f, 1.000000f, 1.000000f, 0.000000f, 
		-1.000000f, -1.000000f, 1.000000f, 1.000000f, 1.000000f, 
		1.000000f, -1.000000f, -1.000000f, 0.000000f, 1.000000f, 
		-1.000000f, -1.000000f, -1.000000f, 0.000000f, 0.000000f, 
		0.999999f, -1.000001f, 1.000000f, 1.000000f, 1.000000f, 
		-1.000000f, -1.000000f, -1.000000f, 0.000000f, 0.000000f, 
		-1.000000f, -1.000000f, 1.000000f, 1.000000f, 0.000000f, 
		0.999999f, -1.000001f, 1.000000f, 1.000000f, 1.000000f, 
		1.000000f, 1.000000f, -1.000000f, 0.000000f, 1.000000f, 
		1.000000f, -1.000000f, -1.000000f, 0.000000f, 0.000000f, 
		1.000000f, 0.999999f, 1.000000f, 1.000000f, 1.000000f, 
		1.000000f, -1.000000f, -1.000000f, 0.000000f, 0.000000f, 
		0.999999f, -1.000001f, 1.000000f, 1.000000f, 0.000000f, 
		1.000000f, 0.999999f, 1.000000f, 1.000000f, 1.000000f, 
		1.000000f, 0.999999f, 1.000000f, 0.000000f, 1.000000f, 
		0.999999f, -1.000001f, 1.000000f, 0.000000f, 0.000000f, 
		-1.000000f, 1.000000f, 1.000000f, 1.000000f, 1.000000f, 
		0.999999f, -1.000001f, 1.000000f, 0.000000f, 0.000000f, 
		-1.000000f, -1.000000f, 1.000000f, 1.000000f, 0.000000f, 
		-1.000000f, 1.000000f, 1.000000f, 1.000000f, 1.000000f, 
		1.000000f, 1.000000f, -1.000000f, 0.000000f, 1.000000f, 
		-1.000000f, 1.000000f, -1.000000f, 0.000000f, 0.000000f, 
		-1.000000f, -1.000000f, -1.000000f, 1.000000f, 0.000000f, 
		1.000000f, 1.000000f, -1.000000f, 0.000000f, 1.000000f, 
		-1.000000f, -1.000000f, -1.000000f, 1.000000f, 0.000000f, 
		1.000000f, -1.000000f, -1.000000f, 1.000000f, 1.000000f, 
	};

	unsigned cube_indices[] = {
		0, 
		1, 
		2, 
		3, 
		4, 
		5, 
		6, 
		7, 
		8, 
		9, 
		10, 
		11, 
		12, 
		13, 
		14, 
		15, 
		16, 
		17, 
		18, 
		19, 
		20, 
		21, 
		22, 
		23, 
		24, 
		25, 
		26, 
		27, 
		28, 
		29, 
		30, 
		31, 
		32, 
		33, 
		34, 
		35, 

	};

	float star_vertices[] = {
		0.000000f, 0.000000f, 0.200000f, 0.894427f, 0.000000f, 0.447214f,
		0.100000f, -0.100000f, 0.000000f, 0.894427f, 0.000000f, 0.447214f,
		0.100000f, 0.100000f, 0.000000f, 0.894427f, 0.000000f, 0.447214f,
		0.100000f, -0.100000f, 0.000000f, 0.000000f, -0.894427f, 0.447214f,
		0.000000f, 0.000000f, 0.200000f, 0.000000f, -0.894427f, 0.447214f,
		-0.100000f, -0.100000f, 0.000000f, 0.000000f, -0.894427f, 0.447214f,
		-0.100000f, -0.100000f, 0.000000f, -0.894427f, 0.000000f, 0.447214f,
		0.000000f, 0.000000f, 0.200000f, -0.894427f, 0.000000f, 0.447214f,
		-0.100000f, 0.100000f, 0.000000f, -0.894427f, 0.000000f, 0.447214f,
		0.000000f, 0.000000f, 0.200000f, 0.000000f, 0.894427f, 0.447214f,
		0.100000f, 0.100000f, 0.000000f, 0.000000f, 0.894427f, 0.447214f,
		-0.100000f, 0.100000f, 0.000000f, 0.000000f, 0.894427f, 0.447214f,
		0.300000f, 0.000000f, -0.100000f, 0.447214f, 0.894427f, 0.000000f,
		0.100000f, 0.100000f, -0.200000f, 0.447214f, 0.894427f, 0.000000f,
		0.100000f, 0.100000f, 0.000000f, 0.447214f, 0.894427f, 0.000000f,
		0.100000f, -0.100000f, 0.000000f, 0.447213f, 0.000000f, 0.894427f,
		0.300000f, 0.000000f, -0.100000f, 0.447213f, 0.000000f, 0.894427f,
		0.100000f, 0.100000f, 0.000000f, 0.447213f, 0.000000f, 0.894427f,
		0.300000f, 0.000000f, -0.100000f, 0.447213f, -0.894427f, 0.000000f,
		0.100000f, -0.100000f, 0.000000f, 0.447213f, -0.894427f, 0.000000f,
		0.100000f, -0.100000f, -0.200000f, 0.447213f, -0.894427f, 0.000000f,
		0.100000f, 0.100000f, -0.200000f, 0.447213f, 0.000000f, -0.894427f,
		0.300000f, 0.000000f, -0.100000f, 0.447213f, 0.000000f, -0.894427f,
		0.100000f, -0.100000f, -0.200000f, 0.447213f, 0.000000f, -0.894427f,
		-0.100000f, 0.100000f, 0.000000f, -0.447213f, 0.000000f, 0.894427f,
		-0.300000f, 0.000000f, -0.100000f, -0.447213f, 0.000000f, 0.894427f,
		-0.100000f, -0.100000f, 0.000000f, -0.447213f, 0.000000f, 0.894427f,
		-0.100000f, -0.100000f, 0.000000f, -0.447214f, -0.894427f, 0.000000f,
		-0.300000f, 0.000000f, -0.100000f, -0.447214f, -0.894427f, 0.000000f,
		-0.100000f, -0.100000f, -0.200000f, -0.447214f, -0.894427f, 0.000000f,
		-0.100000f, -0.100000f, -0.200000f, -0.447214f, 0.000000f, -0.894427f,
		-0.300000f, 0.000000f, -0.100000f, -0.447214f, 0.000000f, -0.894427f,
		-0.100000f, 0.100000f, -0.200000f, -0.447214f, 0.000000f, -0.894427f,
		-0.300000f, 0.000000f, -0.100000f, -0.447214f, 0.894427f, 0.000000f,
		-0.100000f, 0.100000f, 0.000000f, -0.447214f, 0.894427f, 0.000000f,
		-0.100000f, 0.100000f, -0.200000f, -0.447214f, 0.894427f, 0.000000f,
		0.000000f, 0.000000f, -0.400000f, 0.000000f, 0.894427f, -0.447214f,
		-0.100000f, 0.100000f, -0.200000f, 0.000000f, 0.894427f, -0.447214f,
		0.100000f, 0.100000f, -0.200000f, 0.000000f, 0.894427f, -0.447214f,
		0.100000f, -0.100000f, -0.200000f, 0.894427f, 0.000000f, -0.447213f,
		0.000000f, 0.000000f, -0.400000f, 0.894427f, 0.000000f, -0.447213f,
		0.100000f, 0.100000f, -0.200000f, 0.894427f, 0.000000f, -0.447213f,
		-0.100000f, -0.100000f, -0.200000f, 0.000000f, -0.894427f, -0.447214f,
		0.000000f, 0.000000f, -0.400000f, 0.000000f, -0.894427f, -0.447214f,
		0.100000f, -0.100000f, -0.200000f, 0.000000f, -0.894427f, -0.447214f,
		-0.100000f, 0.100000f, -0.200000f, -0.894427f, 0.000000f, -0.447214f,
		0.000000f, 0.000000f, -0.400000f, -0.894427f, 0.000000f, -0.447214f,
		-0.100000f, -0.100000f, -0.200000f, -0.894427f, 0.000000f, -0.447214f,
		-0.100000f, -0.100000f, -0.200000f, -0.894427f, -0.447214f, 0.000000f,
		0.000000f, -0.300000f, -0.100000f, -0.894427f, -0.447214f, 0.000000f,
		-0.100000f, -0.100000f, 0.000000f, -0.894427f, -0.447214f, 0.000000f,
		0.000000f, -0.300000f, -0.100000f, 0.000000f, -0.447213f, 0.894427f,
		0.100000f, -0.100000f, 0.000000f, 0.000000f, -0.447213f, 0.894427f,
		-0.100000f, -0.100000f, 0.000000f, 0.000000f, -0.447213f, 0.894427f,
		0.100000f, -0.100000f, 0.000000f, 0.894427f, -0.447214f, 0.000000f,
		0.000000f, -0.300000f, -0.100000f, 0.894427f, -0.447214f, 0.000000f,
		0.100000f, -0.100000f, -0.200000f, 0.894427f, -0.447214f, 0.000000f,
		0.000000f, -0.300000f, -0.100000f, 0.000000f, -0.447214f, -0.894427f,
		-0.100000f, -0.100000f, -0.200000f, 0.000000f, -0.447214f, -0.894427f,
		0.100000f, -0.100000f, -0.200000f, 0.000000f, -0.447214f, -0.894427f,
		0.000000f, 0.300000f, -0.100000f, 0.000000f, 0.447214f, 0.894427f,
		-0.100000f, 0.100000f, 0.000000f, 0.000000f, 0.447214f, 0.894427f,
		0.100000f, 0.100000f, 0.000000f, 0.000000f, 0.447214f, 0.894427f,
		0.100000f, 0.100000f, -0.200000f, 0.894427f, 0.447214f, 0.000000f,
		0.000000f, 0.300000f, -0.100000f, 0.894427f, 0.447214f, 0.000000f,
		0.100000f, 0.100000f, 0.000000f, 0.894427f, 0.447214f, 0.000000f,
		-0.100000f, 0.100000f, -0.200000f, 0.000000f, 0.447213f, -0.894427f,
		0.000000f, 0.300000f, -0.100000f, 0.000000f, 0.447213f, -0.894427f,
		0.100000f, 0.100000f, -0.200000f, 0.000000f, 0.447213f, -0.894427f,
		-0.100000f, 0.100000f, 0.000000f, -0.894427f, 0.447214f, 0.000000f,
		0.000000f, 0.300000f, -0.100000f, -0.894427f, 0.447214f, 0.000000f,
		-0.100000f, 0.100000f, -0.200000f, -0.894427f, 0.447214f, 0.000000f,
	};

	unsigned star_indices[] = {
		0, 
		1, 
		2, 
		3, 
		4, 
		5, 
		6, 
		7, 
		8, 
		9, 
		10, 
		11, 
		12, 
		13, 
		14, 
		15, 
		16, 
		17, 
		18, 
		19, 
		20, 
		21, 
		22, 
		23, 
		24, 
		25, 
		26, 
		27, 
		28, 
		29, 
		30, 
		31, 
		32, 
		33, 
		34, 
		35, 
		36, 
		37, 
		38, 
		39, 
		40, 
		41, 
		42, 
		43, 
		44, 
		45, 
		46, 
		47, 
		48, 
		49, 
		50, 
		51, 
		52, 
		53, 
		54, 
		55, 
		56, 
		57, 
		58, 
		59, 
		60, 
		61, 
		62, 
		63, 
		64, 
		65, 
		66, 
		67, 
		68, 
		69, 
		70, 
		71, 
	};

	SDL_Surface *screen;
	SDL_Surface *zbuffer;
	
	class Texture {
		SDL_Surface *tex;
	public:
		Texture()
		{
			SDL_Surface *tmp = IMG_Load("data/lava.png");
			tex = SDL_DisplayFormat(tmp);
			SDL_FreeSurface(tmp);
		}

		~Texture()
		{
			SDL_FreeSurface(tex);
		}

		unsigned short sample(int x, int y)
		{
			int tx = x >> (24 - 8);
			int ty = y >> (24 - 8);

			tx &= 256 - 1;
			ty &= 256 - 1;

			return *((unsigned short*)tex->pixels + tx + (ty << 8));
		}
	};

	Texture *tex;
	
	mat4x modelviewprojection_matrix;
	vec3x pos;


	class VertexShader {
		struct InputVertex {
			vec3x vertex;
			int tx, ty;
		};

	public:
		static const unsigned attribute_count = 1;
		static const unsigned varying_count = 2;

	public:
		static void shade(const GeometryProcessor::VertexInput in, GeometryProcessor::VertexOutput &out)
		{
			const InputVertex &i = *static_cast<const InputVertex*>(in[0]);

			vec4x tvertex = modelviewprojection_matrix * vec4x(i.vertex, X(1.0f));

			out.x = tvertex.x.intValue;
			out.y = tvertex.y.intValue;
			out.z = tvertex.z.intValue;
			out.w = tvertex.w.intValue;
			out.varyings[0] = i.tx << 8;
			out.varyings[1] = i.ty << 8;
		}
	};


	class FragmentShader : public GenericSpanDrawer<FragmentShader> {
	public:
		
		static const unsigned varying_count = 2;
		static const bool interpolate_z = false;

		// per triangle callback
		static void begin_triangle(
			const IRasterizer::Vertex& v1, 
			const IRasterizer::Vertex& v2, 
			const IRasterizer::Vertex& v3,
			int area2)
		{}

		static void single_fragment(int x, int y, const IRasterizer::FragmentData &fd)
		{
			unsigned short* color_buffer = (unsigned short*)screen->pixels + 
				(x) + (y)*screen->w;

			*color_buffer = tex->sample(fd.varyings[0], fd.varyings[1]);
		}
	};
	
	class StarVertexShader {
		struct InputVertex {
			vec3x vertex;
			vec3x normal;
		};

	public:
		static const unsigned attribute_count = 1;
		static const unsigned varying_count = 3;

	public:
		static void shade(const GeometryProcessor::VertexInput in, GeometryProcessor::VertexOutput &out)
		{
			const InputVertex &i = *static_cast<const InputVertex*>(in[0]);

			vec4x tvertex = modelviewprojection_matrix * vec4x(i.vertex, X(1.0f));
			vec3x r = normalize(reflect(i.vertex - pos, i.normal));

			out.x = tvertex.x.intValue;
			out.y = tvertex.y.intValue;
			out.z = tvertex.z.intValue;
			out.w = tvertex.w.intValue;
			out.varyings[0] = r.x.intValue << 8;
			out.varyings[1] = r.y.intValue << 8;
			out.varyings[2] = r.z.intValue << 8;
		}
	};


	class StarFragmentShader : public GenericSpanDrawer<StarFragmentShader> {
	public:
		
		static const unsigned varying_count = 3;
		static const bool interpolate_z = true;

		// per triangle callback
		static void begin_triangle(
			const IRasterizer::Vertex& v1, 
			const IRasterizer::Vertex& v2, 
			const IRasterizer::Vertex& v3,
			int area2)
		{}

		static void single_fragment(int x, int y, const IRasterizer::FragmentData &fd)
		{
			unsigned short* color_buffer = (unsigned short*)screen->pixels + 
				(x) + (y)*screen->w;

			unsigned short* zb = (unsigned short*)zbuffer->pixels + 
				(x) + (y)*zbuffer->w;

			unsigned short z = fd.z >> 16;
			if ( z < *zb) {
				int tx = 0, ty = 0;
				int coord[3] = { fd.varyings[0], fd.varyings[1],
					fd.varyings[2]};

				int ma = 0;
				if (std::abs(coord[1]) > std::abs(coord[ma])) ma = 1;
				if (std::abs(coord[2]) > std::abs(coord[ma])) ma = 2;

				switch (ma) {
				case 0:
					if (coord[0] > 0) tx = -coord[2];
					else tx = coord[2];
					ty = -coord[1];
					break;
				case 1:
					if (coord[1] > 0) ty = coord[1];
					else ty = -coord[1];
					tx = coord[0];
					break;
				case 2:
					if (coord[2] > 0) tx = coord[0];
					else tx = -coord[0];
					ty = -coord[1];
					break;
				}

				tx += (1 << 23);
				ty += (1 << 23);

				unsigned short color = tex->sample(tx, ty);
				color &= ~(1 << 5| 1 << 11);
				color >>= 1;
				*color_buffer = color;
				*zb = z;
			}
		}
	};
} // end namespace

void scene4()
{
	screen = SDL_DisplayFormat(SDL_GetVideoSurface());
	zbuffer = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 16, 0, 0, 0, 0);

	for (size_t i = 0; i < sizeof(cube_vertices)/sizeof(float); ++i) {
		int *v = reinterpret_cast<int*>(&cube_vertices[i]);
		*v = float2fix<16>(cube_vertices[i]);
	}

	for (size_t i = 0; i < sizeof(star_vertices)/sizeof(float); ++i) {
		int *v = reinterpret_cast<int*>(&star_vertices[i]);
		*v = float2fix<16>(star_vertices[i]);
	}

	RasterizerSubdivAffine r;
	GeometryProcessor g(&r);
	r.clip_rect(0, 0, 320, 240);
	g.viewport(0, 0, 320, 240);
	g.vertex_shader<VertexShader>();
	r.fragment_shader<FragmentShader>();
	/*r.interlace(0, 1);*/
	
	Texture t;
	tex = &t;
	mat4x projection_matrix = perspective_matrix(X(50.0f), X(4.0f/3.0f), X(0.1f), X(10.0f));

	Uint32 demolength = 15000;
	Uint32 demostart = SDL_GetTicks();
	Uint32 framecount = 0;
	while( true ) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT: 
				case SDL_JOYBUTTONDOWN:
					exit(1);
			}
		}
		Uint32 t = SDL_GetTicks();
		Uint32 demotime = t - demostart;

		SDL_FillRect(zbuffer, 0, INT_MAX);

		float x = demotime/1000.0f;
		pos = vec3x(sin(X(x)/2), cos(X(x)/2), sin(X(x)/2) / 4);
		modelviewprojection_matrix = projection_matrix * 
			lookat_matrix(pos, vec3x(X(0.0f)), vec3x(X(0.0f), X(0.0f), X(1.0f)));

		SDL_LockSurface(screen);
		g.vertex_shader<VertexShader>();
		r.fragment_shader<FragmentShader>();
		g.vertex_attrib_pointer(0, 5 * sizeof(float), cube_vertices);
		g.draw_triangles(36, cube_indices);

		g.vertex_shader<StarVertexShader>();
		r.fragment_shader<StarFragmentShader>();
		g.vertex_attrib_pointer(0, 6 * sizeof(float), star_vertices);
		g.draw_triangles(72, star_indices);
		SDL_UnlockSurface(screen);
		

		flush_uppermem_cache(screen->pixels, (unsigned short*)screen->pixels + 320 * 240,0);

		if (demotime < 1000) {
			fade_effect(screen, false, 1.0f - (t-demostart)/1000.0f);
		}

		if (demotime > demolength) {
			float x = (demotime - demolength)/1000.0f;
			fade_effect(screen, true, x);
		}
		
		SDL_BlitSurface(screen, 0, SDL_GetVideoSurface(), 0);
		SDL_Flip(SDL_GetVideoSurface());

		if (t-demostart > demolength + 1000)
			goto end;

		static int fpscounter = 0;
		static Uint32 fpst = SDL_GetTicks();
		fpscounter++;
		if (SDL_GetTicks() >= fpst + 500) {
			fpst += 500;
			printf("%4i fps\n", fpscounter*2);
			fpscounter = 0;
		}

		framecount++;
	}
end:;

	printf("avg. fps: %f\n", (float)framecount * 1000.0f/(SDL_GetTicks() - demostart));

	SDL_FreeSurface(screen);
	SDL_FreeSurface(zbuffer);
}
