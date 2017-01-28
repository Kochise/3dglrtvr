#ifndef Vertex_hpp
#define Vertex_hpp

#include "FVF.hpp"

namespace swShader
{
	struct LVertex;
	struct LTVertex;
	struct Matrix;

	struct Vertex : public FVF<FVF_VERTEX>
	{
		Vertex();
		Vertex(const Vertex &V);
		Vertex(const LVertex &V);
		Vertex(const LTVertex &V);
		Vertex(const Point &P);
		Vertex(const Vector &V);

		Vertex &operator=(const Vertex &V);

		friend Vertex operator*(const Matrix &M, const Vertex &V);
		friend Vertex operator*(const Vertex &V, const Matrix &M);
		friend Vertex &operator*=(Vertex &V, const Matrix &M);
	};
}

#include "Matrix.hpp"

namespace swShader
{
	inline Vertex::Vertex()
	{
	}

	inline Vertex::Vertex(const Vertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		N = V.N;
		u = V.u;
		v = V.v;
	}

	inline Vertex::Vertex(const Vector &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	inline Vertex::Vertex(const Point &P)
	{
		x = P.x;
		y = P.y;
		z = P.z;
	}

	inline Vertex &Vertex::operator=(const Vertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		N = V.N;
		u = V.u;
		v = V.v;

		return *this;
	}

	inline Vertex operator*(const Matrix &M, const Vertex &V)
	{
		Vertex W;

		W.x = M(1, 1) * V.x + M(1, 2) * V.y + M(1, 3) * V.z + M(1, 4);
		W.y = M(2, 1) * V.x + M(2, 2) * V.y + M(2, 3) * V.z + M(2, 4);
		W.z = M(3, 1) * V.x + M(3, 2) * V.y + M(3, 3) * V.z + M(3, 4);
	//	W.N = V.N;   // NOTE: Not needed when lighting in object space
		W.u = V.u;
		W.v = V.v;

		return W;
	}
	
	inline Vertex operator*(const Vertex &V, const Matrix &M)
	{
		Vertex W;

		W.x = V.x * M(1, 1) + V.x * M(2, 1) + V.z * M(3, 1) + M(4, 1);
		W.y = V.x * M(1, 2) + V.x * M(2, 2) + V.z * M(3, 2) + M(4, 2);
		W.z = V.x * M(1, 3) + V.x * M(2, 3) + V.z * M(3, 3) + M(4, 3);
	//	W.N = V.N;   // NOTE: Not needed when lighting in object space
		W.u = V.u;
		W.v = V.v;

		return W;
	}
	
	inline Vertex &operator*=(Vertex &V, const Matrix &M)
	{
		V.x = V.x * M(1, 1) + V.x * M(2, 1) + V.z * M(3, 1) + M(4, 1);
		V.y = V.x * M(1, 2) + V.x * M(2, 2) + V.z * M(3, 2) + M(4, 2);
		V.z = V.x * M(1, 3) + V.x * M(2, 3) + V.z * M(3, 3) + M(4, 3);

		return V;
	}
}

#endif   // Vertex_hpp