#ifndef Context_hpp
#define Context_hpp

#include "Sampler.hpp"
#include "FVF.hpp"

#include <map>
#include <vector>

namespace swShader
{
	class RenderTarget;
	class VertexBuffer;
	struct LTVertex;
	class PS20Shader;
	class Renderer;

	class Context
	{
	protected:
		static RenderTarget *renderTarget;
		static VertexBuffer *vertexBuffer;
		static void (*scanline)();

		static Sampler sampler[16];

		// Mipmap bias
		static float bias;

		// Gradients
		static float dz_dx;
		static float dz_dy;
		static float dw_dx;
		static float dw_dy;

		static Color<float> dC_dx;
		static Color<float> dC_dy;
		static Color<float> dL_dx;
		static Color<float> dL_dy;

		static UVW dT_dx[8];
		static UVW dT_dy[8];

		// Pixel location
	//	static int x;   // ecx
		static int y;

		// Interpolants
		static int lx;
		static int rx;
		
		static float z;
		static float w;

		static Color<float> C;
		static Color<float> L;

		static UVW T[8];

		// Color and depth buffer scanlines
		static unsigned int *colorBuffer;
		static float *depthBuffer;
	};
}

#endif   // Context_hpp