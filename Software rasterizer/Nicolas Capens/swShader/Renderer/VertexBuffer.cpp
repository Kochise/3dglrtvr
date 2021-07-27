#include "VertexBuffer.hpp"

#include "FVF.hpp"

#include <malloc.h>

namespace swShader
{
	VertexBuffer::VertexBuffer(unsigned int flags, int n)
	{
		Px = 0;
		Py = 0;
		Pz = 0;
		Pw = 0;
		Nx = 0;
		Ny = 0;
		Nz = 0;
		Cb = 0;
		Cg = 0;
		Cr = 0;
		Ca = 0;
		Lb = 0;
		Lg = 0;
		Lr = 0;
		La = 0;
		for(int i = 0; i < 8; i++) Tu[i] = 0;
		for(    i = 0; i < 8; i++) Tv[i] = 0;

		allocate(flags, n);
	}

	VertexBuffer::~VertexBuffer()
	{
		_aligned_free(Px);
		_aligned_free(Py);
		_aligned_free(Pz);
		_aligned_free(Pw);
		_aligned_free(Nx);
		_aligned_free(Ny);
		_aligned_free(Nz);
		_aligned_free(Cb);
		_aligned_free(Cg);
		_aligned_free(Cr);
		_aligned_free(Ca);
		_aligned_free(Lb);
		_aligned_free(Lg);
		_aligned_free(Lr);
		_aligned_free(La);
		for(int i = 0; i < 8; i++) _aligned_free(Tu[i]);
		for(    i = 0; i < 8; i++) _aligned_free(Tv[i]);
	}

	void VertexBuffer::allocate(unsigned int flags, int n)
	{
		this->flags = flags;
		numVertices = n;

		int x = fourfold(n);
		padding = x - n;

		Px = (float*)_aligned_malloc(x * sizeof(float), 16);
		Py = (float*)_aligned_malloc(x * sizeof(float), 16);
		Pz = (float*)_aligned_malloc(x * sizeof(float), 16);

		if(hasRHW()) Pw = (float*)_aligned_malloc(x * sizeof(float), 16);

		if(hasNormal())
		{
			Nx = (float*)_aligned_malloc(x * sizeof(float), 16);
			Ny = (float*)_aligned_malloc(x * sizeof(float), 16);
			Nz = (float*)_aligned_malloc(x * sizeof(float), 16);
		}

		if(hasDiffuse())
		{
			Cb = (float*)_aligned_malloc(x * sizeof(float), 16);
			Cg = (float*)_aligned_malloc(x * sizeof(float), 16);
			Cr = (float*)_aligned_malloc(x * sizeof(float), 16);
			Ca = (float*)_aligned_malloc(x * sizeof(float), 16);
		}

		if(hasSpecular())
		{
			Lb = (float*)_aligned_malloc(x * sizeof(float), 16);
			Lg = (float*)_aligned_malloc(x * sizeof(float), 16);
			Lr = (float*)_aligned_malloc(x * sizeof(float), 16);
			La = (float*)_aligned_malloc(x * sizeof(float), 16);
		}

		for(int t = 0; t < numTexCoords(); t++)
		{
			Tu[t] = (float*)_aligned_malloc(x * sizeof(float), 16);
			Tv[t] = (float*)_aligned_malloc(x * sizeof(float), 16);
		}
	}

	void VertexBuffer::realloc(int n)
	{
		numVertices = n;

		int x = fourfold(n);
		padding = x - n;

		Px = (float*)_aligned_realloc(Px, x * sizeof(float), sizeof(float));
		Py = (float*)_aligned_realloc(Py, x * sizeof(float), sizeof(float));
		Pz = (float*)_aligned_realloc(Pz, x * sizeof(float), sizeof(float));
		Pw = (float*)_aligned_realloc(Pw, x * sizeof(float), sizeof(float));

		if(hasNormal())
		{
			Nx = (float*)_aligned_realloc(Nx, x * sizeof(float), sizeof(float));
			Ny = (float*)_aligned_realloc(Ny, x * sizeof(float), sizeof(float));
			Nz = (float*)_aligned_realloc(Nz, x * sizeof(float), sizeof(float));
		}

		if(hasDiffuse())
		{
			Cb = (float*)_aligned_realloc(Cb, x * sizeof(float), sizeof(float));
			Cg = (float*)_aligned_realloc(Cg, x * sizeof(float), sizeof(float));
			Cr = (float*)_aligned_realloc(Cr, x * sizeof(float), sizeof(float));
			Ca = (float*)_aligned_realloc(Ca, x * sizeof(float), sizeof(float));
		}

		if(hasSpecular())
		{
			Lb = (float*)_aligned_realloc(Lb, x * sizeof(float), sizeof(float));
			Lg = (float*)_aligned_realloc(Lg, x * sizeof(float), sizeof(float));
			Lr = (float*)_aligned_realloc(Lr, x * sizeof(float), sizeof(float));
			La = (float*)_aligned_realloc(La, x * sizeof(float), sizeof(float));
		}

		for(int t = 0; t < numTexCoords(); t++)
		{
			Tu[t] = (float*)_aligned_realloc(Tu[t], x * sizeof(float), sizeof(float));
			Tv[t] = (float*)_aligned_realloc(Tv[t], x * sizeof(float), sizeof(float));
		}
	}

	bool VertexBuffer::hasRHW() const
	{
		return (flags & FVF_RHW) != 0;
	}

	bool VertexBuffer::hasNormal() const
	{
		return (flags & FVF_NORMAL) != 0;
	}

	bool VertexBuffer::hasDiffuse() const
	{
		return (flags & FVF_DIFFUSE) != 0;
	}

	bool VertexBuffer::hasSpecular() const
	{
		return (flags & FVF_SPECULAR) != 0;
	}

	int VertexBuffer::numTexCoords() const
	{
		return (flags & FVF_TEX_MASK) >> FVF_TEX_SHIFT;
	}

	int VertexBuffer::fourfold(int n)
	{
		return (n + 3) & 0xFFFFFFFC;
	}

	int VertexBuffer::size() const
	{
		return numVertices;
	}
};