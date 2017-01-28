#ifndef VertexBuffer_hpp
#define VertexBuffer_hpp

#include "Lockable.hpp"

namespace swShader
{
	class Renderer;

	class VertexBuffer : public Lockable
	{
		friend Renderer;

	public:
		VertexBuffer(unsigned int flags, int n = 0);

		~VertexBuffer();

		void realloc(int n);

		int size() const;

	protected:
		void allocate(unsigned int flags, int n);

		bool hasRHW() const;
		bool hasNormal() const;
		bool hasDiffuse() const;
		bool hasSpecular() const;
		int numTexCoords() const;

		static int fourfold(int n);

		unsigned int flags;
		int numVertices;
		int padding;

		// Vertex Position
		float *Px;
		float *Py;
		float *Pz;
		float *Pw;   // RHW

		// Normal
		float *Nx;
		float *Ny;
		float *Nz;

		// Vertex color (diffuse)
		float *Cb;
		float *Cg;
		float *Cr;
		float *Ca;

		// Specular light
		float *Lb;
		float *Lg;
		float *Lr;
		float *La;

		// Texture coordinates
		float *Tu[8];
		float *Tv[8];
	};
}

#endif VertexBuffer_hpp