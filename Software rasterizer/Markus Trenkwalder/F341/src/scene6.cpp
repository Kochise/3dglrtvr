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

#include "scene6.h"

#include "SDL.h"
#include "renderer/geometry_processor.h"
#include "renderer/rasterizer_subdivaffine.h"
#include "renderer/span.h"
#include "vector_math.h"
#include "fixedpoint/fixed_func.h"
#include "democommon.h"

#include <algorithm>

using namespace std;
using namespace swr;

namespace {
	SDL_Surface *screen;


	struct InputVertex {
		vec3x vertex;
	};

	InputVertex vertex_data[] = {
		{vec3x(-1, -1, -1)}, 
		{vec3x( 1, -1, -1)},
		{vec3x( 1,  1, -1)},
		{vec3x(-1,  1, -1)},
		{vec3x(-1, -1,  1)}, 
		{vec3x( 1, -1,  1)},
		{vec3x( 1,  1,  1)},
		{vec3x(-1,  1,  1)}
	};
	
	unsigned index_data[] = {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
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
	
	void Zoom(SDL_Surface *dest, const SDL_Surface* s, Sint32 x, Sint32 y, Uint32 sx, Uint32 sy)
	{
		const Uint32 w = (s->w * sx) / 100;
		const Uint32 h = (s->h * sy) / 100;
		x -= w >> 1;
		y -= h >> 1;	
		Sint32 r  = min(x + int(w), dest->w);
		Sint32 b  = min(y + int(h), dest->h);	

		Uint16 *dst = (Uint16*)dest->pixels;
		Uint16 *src = (Uint16*)s->pixels;
		Uint32 dst_skip;

		Uint32 start_sdx = 0;
		Uint32 start_skip = 0;
		Uint32 sdy = 0;

		if ( x < 0 ) {
			div_t r = div(-x * 100, sx);
			start_skip = r.quot;
			start_sdx  = r.rem;
			x = 0;
		}
		if ( y < 0 ) {
			div_t r = div(-y * 100, sy);
			sdy = r.rem;
			src += r.quot * s->w;
			y = 0;
		}

		Sint32 dtx = r - x;
		Sint32 dty = b - y;

		dst_skip = dest->w - dtx;
		dst += x + y * dest->w;

		for ( int i = 0; i < dty; ++i ) {
			Uint16 *srcc = src + start_skip;
			Uint16 color = *srcc++;
			Uint32 sdx = start_sdx;
			for ( int j = 0; j < dtx; ++j ) {
				if ( color ) *dst++ = color;
				else dst++;

				sdx += 100;
				while ( sdx >= sx ) {
					color = *srcc++;
					sdx -= sx;
				}
			}
			dst += dst_skip;
			sdy += 100;
			while ( sdy >= sy ) {
				src += s->w;
				sdy -= sy;
			}
		}
	}


	uint16_t color_scale(uint16_t c, int scale)
	{
		int rb = c & (0xf800 | 0x1f);
		int g  = c & (0x7c0);
		rb = ((rb * scale) >> 5) & (0xf800 | 0x1f);
		g = ((g * scale) >> 5) & (0x7e0);
		return rb | g;
	}
};

void scene6()
{
	screen = SDL_DisplayFormat(SDL_GetVideoSurface());
	SDL_Surface *screen2 = SDL_DisplayFormat(screen);

	RasterizerSubdivAffine r;
	GeometryProcessor g(&r);
	r.clip_rect(0, 0, 320, 240);
	g.viewport(0, 0, 320, 240);
	g.vertex_shader<VertexShader>();
	r.fragment_shader<FragmentShader>();
	/*r.interlace(0, 1);*/
	g.vertex_attrib_pointer(0, sizeof(InputVertex), vertex_data);

	mat4x pm = perspective_matrix<fixed16_t>(60.0f, 4.0f/3.0f, 0.1, 20.0f);

	Uint32 demolength = 13000;
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

		Zoom(screen2, screen, 160, 120, 105, 105);

		int scale = (int)(sin(t/50.0f) * 6 + 24);

		Uint16* p = (Uint16*)screen2->pixels;
		for (int i = 0, n = screen2->w * screen2->h; i < n; ++i) {
			Uint16 c = *p;
			*p++ = color_scale(c, scale);
		}
			
		SDL_BlitSurface(screen2, 0, screen, 0);
		SDL_LockSurface(screen);

		mat4x model_matrix = 
			rotation_matrix<fixed16_t>(-(t/20.0f), 1, 0, 0) * 
			rotation_matrix<fixed16_t>((t/40.0f), 0, 1, 0) * 
			rotation_matrix<fixed16_t>(-(t/30.0f), 0, 0, 1);

		VertexShader::modelview_matrix = 
			lookat_matrix(vec3x(2, -3, 0), vec3x(0), vec3x(0, 0, 1)) *
			model_matrix;

		VertexShader::modelviewprojection_matrix = pm * VertexShader::modelview_matrix;
		
		g.draw_lines(24, index_data);

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
	SDL_FreeSurface(screen2);
}

