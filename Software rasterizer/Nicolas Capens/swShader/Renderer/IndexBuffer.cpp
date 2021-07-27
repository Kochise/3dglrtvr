#include "IndexBuffer.hpp"

#include "Error.hpp"

namespace swShader
{
	IndexBuffer::IndexBuffer(int n)
	{
		numFaces = n;

		face = new Face[n];
	}

	IndexBuffer::~IndexBuffer()
	{
		numFaces = 0;

		delete[] face;
		face = 0;
	}

	const IndexBuffer::Face &IndexBuffer::getFace(int i)
	{
		if(i < 0 || i >= numFaces) throw Error("Face index out of bounds");

		return face[i];
	}

	void IndexBuffer::realloc(int n)
	{
		Face *tFace = face;

		numFaces = n;
		face = new Face[n];

		for(int i = 0; i < numFaces && i < n; i++)
		{
			face[i][0] = tFace[i][0];
			face[i][1] = tFace[i][1];
			face[i][2] = tFace[i][2];
		}

		delete[] tFace;
	}

	int IndexBuffer::size() const
	{
		return numFaces;
	}
};