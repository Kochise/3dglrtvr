#ifndef IndexBuffer_hpp
#define IndexBuffer_hpp

#include "Lockable.hpp"

namespace swShader
{
	class Renderer;

	class IndexBuffer : public Lockable
	{
		friend Renderer;

	public:
		IndexBuffer(int n = 0);

		~IndexBuffer();

		typedef int Face[3];

		const Face &getFace(int i);

		void realloc(int n);
		int size() const;

	protected:
		int numFaces;

		Face *face;
	};
}

#endif   // IndexBuffer_hpp