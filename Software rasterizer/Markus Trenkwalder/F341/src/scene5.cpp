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

#include "scene5.h"

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

	struct InputVertex {
		vec2x vertex;
		fixed16_t tx, ty;
	};

	InputVertex vertex_data[] = {
		{vec2x(X(-128), X(-128)), X(0), X(0)},
		{vec2x(X(127), X(-128)), X(1), X(0)},
		{vec2x(X(127), X(127)), X(1), X(1)},
		{vec2x(X(-128), X(127)), X(0), X(1)}
	};

	SDL_Surface *screen;

	class Texture {
		SDL_Surface *tex;
	public:
		Texture()
		{
			SDL_Surface *tmp = IMG_Load("data/star.png");
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

	class VertexShader {

	public:
		static const unsigned attribute_count = 1;
		static const unsigned varying_count = 2;

	public:
		static mat4x modelviewprojection_matrix;

		static void shade(const GeometryProcessor::VertexInput in, GeometryProcessor::VertexOutput &out)
		{
			const InputVertex &i = *static_cast<const InputVertex*>(in[0]);

			vec4x tvertex = modelviewprojection_matrix * vec4x(i.vertex, vec2x(X(0), X(1)));

			out.x = tvertex.x.intValue;
			out.y = tvertex.y.intValue;
			out.z = tvertex.z.intValue;
			out.w = tvertex.w.intValue;

			out.varyings[0] = i.tx.intValue << 8;
			out.varyings[1] = i.ty.intValue << 8;
		}
	};

	mat4x VertexShader::modelviewprojection_matrix;

	uint16_t sadd(uint16_t color1, uint16_t color2)
	{
		static const unsigned OVERFLOW_BITS = (1 << 16) | (1 << 11) | (1 << 5);
		int result = color1 + color2;
		int overflow = (result ^ color1 ^ color2) & OVERFLOW_BITS;
		result -= overflow;
		overflow -= overflow >> 5;
		result |= overflow; 
		return (uint16_t)result;
	}

	class FragmentShader : public GenericSpanDrawer<FragmentShader> {
	public:
		static Texture *tex;
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

			uint16_t color = tex->sample(fd.varyings[0], fd.varyings[1]); 
			if (color != 0)
				*color_buffer = sadd(*color_buffer, color);
		}
	};
	
	Texture* FragmentShader::tex;
} // end namespace

void scene5()
{	
	screen = SDL_DisplayFormat(SDL_GetVideoSurface());

	RasterizerSubdivAffine r;
	GeometryProcessor g(&r);
	r.clip_rect(0, 0, 320, 240);
	g.viewport(0, 0, 320, 240);
	g.vertex_shader<VertexShader>();
	r.fragment_shader<FragmentShader>();
	r.perspective_correction(false);
	/*r.interlace(0, 1);*/
	g.vertex_attrib_pointer(0, sizeof(InputVertex), vertex_data);

	Texture tex;
	FragmentShader::tex = &tex;

	SDL_Surface *tmp = IMG_Load("data/background.png");
	SDL_Surface *background = SDL_DisplayFormat(tmp);
	SDL_FreeSurface(tmp);

	mat4x pm = ortho_matrix(X(0), X(320), X(0), X(240), X(-1), X(1));
	

	unsigned indices[] = {0, 1, 2, 0, 2, 3};

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

		SDL_BlitSurface(background, 0, screen, 0);
		SDL_LockSurface(screen);
		

		{
			vec2x pos = vec2x(sin(X(t/1000.0f)) * X(125) + X(160), 
				sin(X(t/2000.0f)) * X(100) + X(120));
			VertexShader::modelviewprojection_matrix = pm * 
				translation_matrix(vec3x(pos, X(0))) *
				rotation_matrix(X(t/40.0f), X(0), X(0), X(1));

			g.draw_triangles(6, indices);
		}

		{
			vec2x pos = vec2x(sin(X((t + 5123)/1000.0f)) * X(75) + X(160), 
				-cos(X((t + 85645)/2000.0f)) * X(80) + X(120));
			VertexShader::modelviewprojection_matrix = pm * 
				translation_matrix(vec3x(pos, X(0))) *
				rotation_matrix(X(t/30.0f), X(0), X(0), X(1));

			g.draw_triangles(6, indices);
		}
		
		{
			vec2x pos = vec2x(-sin(X((t + 8875)/1000.0f)) * X(125) + X(160), 
				-sin(X((t + 451)/2000.0f)) * X(100) + X(120));
			VertexShader::modelviewprojection_matrix = pm * 
				translation_matrix(vec3x(pos, X(0))) *
				rotation_matrix(X(t/20.0f), X(0), X(0), X(1));

			g.draw_triangles(6, indices);
		}
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
