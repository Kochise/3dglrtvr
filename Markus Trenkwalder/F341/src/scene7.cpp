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

#include "scene7.h"

#include "SDL.h"
#include "renderer/geometry_processor.h"
#include "renderer/rasterizer_subdivaffine.h"
#include "renderer/span.h"
#include "vector_math.h"
#include "fixedpoint/fixed_func.h"
#include "democommon.h"

#include <algorithm>
#include <vector>

using namespace std;
using namespace swr;

namespace {
	SDL_Surface *screen;

	struct InputVertex {
		vec3x vertex;
		vec3x speed;
		vec3x target;
	};

	class VertexShader {
	public:
		static const unsigned attribute_count = 1;
		static const unsigned varying_count = 1;

	public:
		static mat4x modelview_matrix;
		static mat4x modelviewprojection_matrix;

		static void shade(const GeometryProcessor::VertexInput in, GeometryProcessor::VertexOutput &out)
		{
			const InputVertex &i = *static_cast<const InputVertex*>(in[0]);

			vec4x tvertex = modelviewprojection_matrix * vec4x(i.vertex, X(1));

			const vec3x &v = i.vertex;
			const mat4x &m = modelview_matrix;
			fixed16_t d = -(v.x * m.elem[2][0] + v.y * m.elem[2][1] + 
				v.z * m.elem[2][2] + m.elem[2][3]);

			const fixed16_t start = 2;
			const fixed16_t end = 6;

			if (d < start) d = 1;
			else if (d > end) d = 0;
			else {
				d = X(1) - (d-start)/(end-start);
			}

			out.x = tvertex.x.intValue;
			out.y = tvertex.y.intValue;
			out.z = tvertex.z.intValue;
			out.w = tvertex.w.intValue;
			out.varyings[0] = d.intValue << 8;
		}
	};

	mat4x VertexShader::modelview_matrix;
	mat4x VertexShader::modelviewprojection_matrix;


	class FragmentShader : public GenericSpanDrawer<FragmentShader> {
	public:
		
		static const unsigned varying_count = 1;
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

			int color = std::min(std::max(fd.varyings[0] >> (16 + 3), 0), 31);
			color = color | color << 6 | color << 11;

			*color_buffer = color;
		}
	};
	
	const unsigned FragmentShader::varying_count;

	fixed16_t abs(fixed16_t t)
	{
		t.intValue = std::abs(t.intValue);
		return t;
	}

	fixed16_t fixrand()
	{
		return static_cast<float>(rand())/RAND_MAX;
	}
} // end namespace


void scene7()
{
	screen = SDL_DisplayFormat(SDL_GetVideoSurface());

	RasterizerSubdivAffine r;
	GeometryProcessor g(&r);
	r.clip_rect(0, 0, 320, 240);
	g.viewport(0, 0, 320, 240);
	g.vertex_shader<VertexShader>();
	r.fragment_shader<FragmentShader>();
	/*r.interlace(0, 1);*/

	std::vector<InputVertex> points(500);

	for (int i = 0; i < 500; ++i) {
		fixed16_t x = fixrand() * 2 - X(1);
		fixed16_t y = fixrand() * 2 - X(1);
		fixed16_t z = fixrand() * 2 - X(1);
		points[i].vertex = vec3x(x, y, z);
		points[i].target = points[i].vertex;
	}

	std::vector<unsigned> indices(500);
	for (size_t i = 0; i < indices.size(); ++i)
		indices[i] = (int)i;

	g.vertex_attrib_pointer(0, sizeof(InputVertex), &points[0]);

	mat4x pm = perspective_matrix<fixed16_t>(60.0f, 4.0f/3.0f, 0.1f, 20.0f);

	Uint32 demolength = 15000;
	Uint32 demostart = SDL_GetTicks();
	Uint32 framecount = 0;

	Uint32 now = SDL_GetTicks();

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

		SDL_FillRect(screen, 0, 0);
		SDL_LockSurface(screen);

		mat4x model_matrix = 
			rotation_matrix<fixed16_t>(-(t/20.0f), 1, 0, 0) * 
			rotation_matrix<fixed16_t>((t/40.0f), 0, 1, 0) * 
			rotation_matrix<fixed16_t>(-(t/30.0f), 0, 0, 1);

		VertexShader::modelview_matrix = 
			lookat_matrix(vec3x(2, -3, 0), vec3x(0), vec3x(0, 0, 1)) *
			model_matrix;

		VertexShader::modelviewprojection_matrix = pm * VertexShader::modelview_matrix;

		static int state = 0;

		if (demotime > 5000 && state == 0) {
			for (size_t i = 0; i < points.size(); ++i) {
				fixed16_t x = fixrand() * 2 - X(1);
				fixed16_t y = fixrand() * 2 - X(1);
				fixed16_t z = fixrand() * 2 - X(1);
				points[i].target = normalize(vec3x(x, y, z));
				points[i].speed = (points[i].target - points[i].vertex)/X(2000/30);
			}
			state = 1;
		}

		if (demotime > 10000 && state == 1) {
			for (size_t i = 0; i < points.size(); ++i) {
				fixed16_t x = fixrand() * 2 - X(1);
				fixed16_t y = fixrand() * 2 - X(1);
				fixed16_t z = fixrand() * 2 - X(1);
				vec3x t(x, y, z);
				t = normalize(t);

				int ma = 0;
				if (abs(t[1]) > abs(t[ma])) ma = 1;
				if (abs(t[2]) > abs(t[ma])) ma = 2;

				if (t[ma] > X(0)) t[ma] = 1;
				else t[ma] = -1;

				points[i].target = t;
				points[i].speed = (points[i].target - points[i].vertex)/X(2000/30);
			}
			state = 2;
		}

		while (t - now >= 30) {
			for (size_t i = 0; i < points.size(); ++i) {
				vec3x diff = points[i].target - points[i].vertex;
				if (diff[0] < X(0)) diff[0] = -diff[0];
				if (diff[1] < X(0)) diff[1] = -diff[1];
				if (diff[2] < X(0)) diff[2] = -diff[2];
				if (diff[0] < X(0.05f) && diff[1] < X(0.05f) && diff[2] < X(0.05f)) 
					continue;

				points[i].vertex += points[i].speed;
			}
			now += 30;
		}

		g.draw_points(500, &indices[0]);

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
		if (SDL_GetTicks() >= fpst + 1000) {
			fpst += 1000;
			printf("%4i fps\n", fpscounter);
			fpscounter = 0;
		}

		framecount++;
	}
end:;

	printf("avg. fps: %f\n", (float)framecount * 1000.0f/(SDL_GetTicks() - demostart));

	SDL_FreeSurface(screen);
}
