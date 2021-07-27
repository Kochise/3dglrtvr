#include "Context.hpp"

#include "Texture.hpp"
#include "Error.hpp"

namespace swShader
{
	RenderTarget *Context::renderTarget = 0;
	VertexBuffer *Context::vertexBuffer = 0;
	void (*Context::scanline)() = 0;

	Sampler Context::sampler[16];

	float Context::bias = 0;

	float Context::dz_dx = 0;
	float Context::dz_dy = 0;
	float Context::dw_dx = 0;
	float Context::dw_dy = 0;

	ALIGN(16) Color<float> Context::dC_dx = Color<float>::BLACK;
	ALIGN(16) Color<float> Context::dC_dy = Color<float>::BLACK;
	ALIGN(16) Color<float> Context::dL_dx = Color<float>::BLACK;
	ALIGN(16) Color<float> Context::dL_dy = Color<float>::BLACK;

	UVW Context::dT_dx[8];
	UVW Context::dT_dy[8];

//	int Context::x = 0;
	int Context::y = 0;

	int Context::lx = 0;
	int Context::rx = 0;

	float Context::z = 0;
	float Context::w = 0;

	ALIGN(16) Color<float> Context::C = Color<float>::CYAN;
	ALIGN(16) Color<float> Context::L = Color<float>::GREEN;

	UVW Context::T[8];

	unsigned int *Context::colorBuffer = 0;
	float *Context::depthBuffer = 0;
}