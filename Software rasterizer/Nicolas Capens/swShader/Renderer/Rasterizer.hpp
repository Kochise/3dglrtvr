#ifndef Rasterizer_hpp
#define Rasterizer_hpp

#include "Context.hpp"

namespace swShader
{
	class RenderTarget;
	struct XVertex;

	class Rasterizer : public Context
	{
	public:
		Rasterizer(RenderTarget *renderTarget);

		~Rasterizer();

		void renderTriangle(const XVertex *V1, const XVertex *V2, const XVertex *V3, int FVF);

		void update();

	private:
		float textureDetail;   // Global mipmap bias
	};
}

#endif   // Rasterizer_hpp