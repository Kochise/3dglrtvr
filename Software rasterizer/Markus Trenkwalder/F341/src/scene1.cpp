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


#include "scene1.h"

#include "SDL.h"
#include "renderer/rasterizer_subdivaffine.h"
#include "renderer/geometry_processor.h"
#include "renderer/span.h"
#include "democommon.h"
#include "fixedpoint/fixed_func.h"
#include "vector_math.h"

#include <algorithm>

using namespace swr;

namespace {

	float vertex_data[] = {
		0.707107f, 0.707107f, 0.000000f, 0.886394f, 0.237509f, 0.397360f,
		0.000000f, 0.000000f, 2.000000f, 0.886394f, 0.237509f, 0.397360f,
		0.965926f, -0.258819f, 0.000000f, 0.886394f, 0.237509f, 0.397360f,
		0.965926f, -0.258819f, 0.000000f, 0.648886f, -0.648885f, 0.397360f,
		0.000000f, 0.000000f, 2.000000f, 0.648886f, -0.648885f, 0.397360f,
		0.258819f, -0.965926f, 0.000000f, 0.648886f, -0.648885f, 0.397360f,
		0.258819f, -0.965926f, 0.000000f, -0.237508f, -0.886395f, 0.397360f,
		0.000000f, 0.000000f, 2.000000f, -0.237508f, -0.886395f, 0.397360f,
		-0.707106f, -0.707107f, 0.000000f, -0.237508f, -0.886395f, 0.397360f,
		-0.707106f, -0.707107f, 0.000000f, -0.886394f, -0.237509f, 0.397360f,
		0.000000f, 0.000000f, 2.000000f, -0.886394f, -0.237509f, 0.397360f,
		-0.965926f, 0.258818f, 0.000000f, -0.886394f, -0.237509f, 0.397360f,
		-0.965926f, 0.258818f, 0.000000f, -0.648886f, 0.648885f, 0.397360f,
		0.000000f, 0.000000f, 2.000000f, -0.648886f, 0.648885f, 0.397360f,
		-0.258820f, 0.965926f, 0.000000f, -0.648886f, 0.648885f, 0.397360f,
		0.000000f, 0.000000f, 2.000000f, 0.237508f, 0.886395f, 0.397360f,
		0.707107f, 0.707107f, 0.000000f, 0.237508f, 0.886395f, 0.397360f,
		-0.258820f, 0.965926f, 0.000000f, 0.237508f, 0.886395f, 0.397360f,
		0.000000f, 0.000000f, -2.000000f, 0.886394f, 0.237509f, -0.397360f,
		0.707107f, 0.707107f, 0.000000f, 0.886394f, 0.237509f, -0.397360f,
		0.965926f, -0.258819f, 0.000000f, 0.886394f, 0.237509f, -0.397360f,
		0.258819f, -0.965926f, 0.000000f, 0.648886f, -0.648885f, -0.397360f,
		0.000000f, 0.000000f, -2.000000f, 0.648886f, -0.648885f, -0.397360f,
		0.965926f, -0.258819f, 0.000000f, 0.648886f, -0.648885f, -0.397360f,
		-0.707106f, -0.707107f, 0.000000f, -0.237508f, -0.886395f, -0.397360f,
		0.000000f, 0.000000f, -2.000000f, -0.237508f, -0.886395f, -0.397360f,
		0.258819f, -0.965926f, 0.000000f, -0.237508f, -0.886395f, -0.397360f,
		-0.965926f, 0.258818f, 0.000000f, -0.886394f, -0.237509f, -0.397360f,
		0.000000f, 0.000000f, -2.000000f, -0.886394f, -0.237509f, -0.397360f,
		-0.707106f, -0.707107f, 0.000000f, -0.886394f, -0.237509f, -0.397360f,
		-0.258820f, 0.965926f, 0.000000f, -0.648886f, 0.648885f, -0.397360f,
		0.000000f, 0.000000f, -2.000000f, -0.648886f, 0.648885f, -0.397360f,
		-0.965926f, 0.258818f, 0.000000f, -0.648886f, 0.648885f, -0.397360f,
		0.707107f, 0.707107f, 0.000000f, 0.237508f, 0.886395f, -0.397360f,
		0.000000f, 0.000000f, -2.000000f, 0.237508f, 0.886395f, -0.397360f,
		-0.258820f, 0.965926f, 0.000000f, 0.237508f, 0.886395f, -0.397360f,
	};

	unsigned index_data[] = {
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

	SDL_Surface *screen;

	class VertexShader {
		struct InputVertex {
			vec3x vertex;
			vec3x normal;
		};

	public:
		static const unsigned attribute_count = 1;
		static const unsigned varying_count = 3;

	public:
		static mat4x viewprojection_matrix;
		static mat4x model_matrix;

		static vec3x material_color;
		static vec3x ambient_color;

		static struct Light {
			vec3x dir;
			vec3x color;
		} lights[2];

		static void shade(const GeometryProcessor::VertexInput in, GeometryProcessor::VertexOutput &out)
		{
			const InputVertex &i = *static_cast<const InputVertex*>(in[0]);

			vec3x wpos = transform_point(model_matrix, i.vertex);
			vec3x wnor = transform_vector(model_matrix, i.normal);

			vec3x color = ambient_color * material_color;
			for (int i = 0; i < 2; ++i) {
				fixed16_t ndotl = std::max(X(0.0f), dot(wnor, -lights[i].dir));
				color += lights[i].color * ndotl;
			}
			color *= material_color;
			for (int i = 0; i < 3; ++i)
				color[i] = std::min(X(1.0f), color[i]);

			vec4x tvertex = viewprojection_matrix * vec4x(wpos, X(1.0f));

			out.x = tvertex.x.intValue;
			out.y = tvertex.y.intValue;
			out.z = tvertex.z.intValue;
			out.w = tvertex.w.intValue;			
			out.varyings[0] = color.x.intValue << 8;
			out.varyings[1] = color.y.intValue << 8;
			out.varyings[2] = color.z.intValue << 8;
		}
	};

	mat4x VertexShader::viewprojection_matrix;
	mat4x VertexShader::model_matrix;
	vec3x VertexShader::material_color;
	vec3x VertexShader::ambient_color;
	VertexShader::Light VertexShader::lights[2];


	class FragmentShader : public GenericSpanDrawer<FragmentShader> {
	public:
		static const unsigned varying_count = 3;
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
			
			// unfortunaltely at the corners of the triangle the values
			// can go outside the valid range
			int r = std::min(std::max(0, fd.varyings[0]>>16), 255) >> 3;
			int g = std::min(std::max(0, fd.varyings[1]>>16), 255) >> 2;
			int b = std::min(std::max(0, fd.varyings[2]>>16), 255) >> 3;
			unsigned short color = r << 11 | g << 5 | b;
			*color_buffer = color;
		}
	};
	
	class Noise {
	private:
		static const int NOISE_SIZE_X = 512;
		static const int NOISE_SIZE_Y = 256;
		SDL_Surface *noise;

	private:
		void generate_noise(unsigned short *mem, unsigned int len)
		{
			while (len) {
				unsigned int gray = rand() & 0x3f;
				unsigned int l = rand() % 2 + 1;
				while (len && l--) {
					len--;
					*mem++ = (gray >> 1) | (gray << 5) | ((gray >> 1) << 11);
				}
			}
		}

	public:
		Noise() {
			noise = SDL_CreateRGBSurface(SDL_HWSURFACE, NOISE_SIZE_X,
				NOISE_SIZE_Y, 16, 0, 0, 0, 0);
			SDL_Surface *temp = SDL_DisplayFormat(noise);
			SDL_FreeSurface(noise);
			noise = temp;

			generate_noise(static_cast<unsigned short*>(noise->pixels), 
				NOISE_SIZE_X * NOISE_SIZE_Y);
		}

		~Noise()
		{
			SDL_FreeSurface(noise);
		}

		void draw(SDL_Surface *dst)
		{
			int x = -(rand() % NOISE_SIZE_X-1);
			int y = -(rand() % NOISE_SIZE_Y-1);

			for (int iy = y; iy < dst->h; iy += NOISE_SIZE_Y){
				for (int ix = x; ix < dst->w; ix += NOISE_SIZE_X){
					SDL_Rect r = {ix, iy, NOISE_SIZE_X, NOISE_SIZE_Y};
					SDL_BlitSurface(noise, 0, dst, &r);
				}
			}
		}
	};

};


void scene1()
{
	screen = SDL_DisplayFormat(SDL_GetVideoSurface());


	RasterizerSubdivAffine r;
	GeometryProcessor g(&r);
	g.viewport(0, 0, 320, 240);
	g.vertex_shader<VertexShader>();
	r.clip_rect(0, 0, 320, 240);
	r.fragment_shader<FragmentShader>();
	r.perspective_correction(false);
	/*r.interlace(0, 1);*/

	VertexShader::ambient_color = vec3x(X(0.3f));
	VertexShader::material_color = vec3x(X(0.8f));
	VertexShader::lights[0].dir = normalize(vec3x(X(-4.0f), X(2.0f), X(-2.0f)));
	VertexShader::lights[0].color = vec3x(X(0.5f));
	VertexShader::lights[1].dir = normalize(vec3x(X(5.0f), X(5.0f), X(1.0f)));
	VertexShader::lights[1].color = vec3x(X(0.3f));

	for (size_t i = 0; i < sizeof(vertex_data)/sizeof(float); ++i) {
		int *v = reinterpret_cast<int*>(&vertex_data[i]);
		*v = float2fix<16>(vertex_data[i]);
	}	

	g.vertex_attrib_pointer(0, sizeof(float) * 6, vertex_data);

	mat4x projection_matrix = perspective_matrix(X(54.0f), X(4.0f/3.0f), X(0.1f), X(50.0f));

	Noise noise;


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

		noise.draw(screen);

		VertexShader::model_matrix = rotation_matrix<fixed16_t>(X(-(t / 1000.0f) * 90), X(0), X(0), X(1));
		VertexShader::viewprojection_matrix = projection_matrix * 
			lookat_matrix(
			vec3x(sin(X((t * 2)/1000.0f))*3, sin(X((t * 1.5f)/1000.0f))*2+X(4), sin(X(t/1000.0f))), 
			vec3x(X(0.0f), X(0.0f), X(0.0f)), 
			vec3x(X(0.0f), X(0.0f), X(1.0f)));

		SDL_LockSurface(screen);
		g.draw_triangles(36, index_data);	
		SDL_UnlockSurface(screen);

		flush_uppermem_cache(screen->pixels, (unsigned short*)screen->pixels + 320 * 240,0);

		if (t - demostart < 1000) {
			fade_effect(screen, false, 1.0f - (t-demostart)/1000.0f);
		}

		if (t - demostart > demolength) {
			float x = (t - demostart - demolength)/1000.0f;
			fade_effect(screen, true, x);
		}

		if (t-demostart > demolength + 1000)
			goto end;

		SDL_BlitSurface(screen, 0, SDL_GetVideoSurface(), 0);
		SDL_Flip(SDL_GetVideoSurface());

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
