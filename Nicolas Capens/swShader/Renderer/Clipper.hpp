#ifndef Clipper_hpp
#define Clipper_hpp

#include "XVertex.hpp"

namespace swShader
{
	class Clipper
	{
	public:
		enum ClipFlags
		{
			CLIP_NEAR    = 1 << 0,
			CLIP_FAR     = 1 << 1,
			CLIP_LEFT    = 1 << 2,
			CLIP_RIGHT   = 1 << 3,
			CLIP_TOP     = 1 << 4,
			CLIP_BOTTOM  = 1 << 5,

			CLIP_ALL     = CLIP_NEAR |
			               CLIP_FAR |
		                   CLIP_LEFT |
		                   CLIP_RIGHT |
		                   CLIP_TOP |
		                   CLIP_BOTTOM
		};

		Clipper();

		~Clipper();

		XVertex **clipTriangle(const XVertex &V1, const XVertex &V2, const XVertex &V3, int FVF);
		int getNumVertices() const;

		void setClipFlags(int flags);

	private:
		void clipNear();
		void clipFar();
		void clipLeft();
		void clipRight();
		void clipTop();
		void clipBottom();

		int FVF;
		int clipFlags;

		void clipEdge(int t, int i, int j, float d);

		XVertex B[16][16];          // Buffer for clipped vertices
		const XVertex *P[16][16];   // Pointers to polygon's vertices
		int numVertices;
		int stage;
	};
}

#endif   // Clipper_hpp