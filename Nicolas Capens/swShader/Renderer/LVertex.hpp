#ifndef LVertex_hpp
#define LVertex_hpp

#include "FVF.hpp"

namespace swShader
{
	struct Vertex;
	struct LTVertex;
	struct Matrix;

	struct LVertex : public FVF<FVF_LVERTEX>
	{
		LVertex();
		LVertex(const Vertex &V);
		LVertex(const LVertex &V);
		LVertex(const LTVertex &V);
		LVertex(const Point &P);
		LVertex(const Vector &V);

		LVertex &operator=(const LVertex &V);

		friend LVertex operator*(const Matrix &M, const LVertex &V);
		friend LVertex operator*(const LVertex &V, const Matrix &M);
		friend LVertex &operator*=(LVertex &V, const Matrix &M);
	};
}

#include "Vertex.hpp"
#include "LTVertex.hpp"
#include "Matrix.hpp"

namespace swShader
{
	inline LVertex::LVertex()
	{
	}

	inline LVertex::LVertex(const Vertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		u = V.u;
		v = V.v;
	}

	inline LVertex::LVertex(const LVertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		C = V.C;
		L = V.L;
		u = V.u;
		v = V.v;
	}

	inline LVertex::LVertex(const LTVertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		C = V.C;
		L = V.L;
		u = V.u;
		v = V.v;
	}

	inline LVertex::LVertex(const Vector &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	inline LVertex::LVertex(const Point &P)
	{
		x = P.x;
		y = P.y;
		z = P.z;
	}

	inline LVertex &LVertex::operator=(const LVertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		C = V.C;
		L = V.L;
		u = V.u;
		v = V.v;

		return *this;
	}

	inline LVertex operator*(const Matrix &M, const LVertex &V)
	{
		LVertex W;

		W.x = M(1, 1) * V.x + M(1, 2) * V.y + M(1, 3) * V.z + M(1, 4);
		W.y = M(2, 1) * V.x + M(2, 2) * V.y + M(2, 3) * V.z + M(2, 4);
		W.z = M(3, 1) * V.x + M(3, 2) * V.y + M(3, 3) * V.z + M(3, 4);
		W.C = V.C;
		W.L = V.L;
		W.u = V.u;
		W.v = V.v;

		return W;
	}
	
	inline LVertex operator*(const LVertex &V, const Matrix &M)
	{
		LVertex W;

		W.x = V.x * M(1, 1) + V.x * M(2, 1) + V.z * M(3, 1) + M(4, 1);
		W.y = V.x * M(1, 2) + V.x * M(2, 2) + V.z * M(3, 2) + M(4, 2);
		W.z = V.x * M(1, 3) + V.x * M(2, 3) + V.z * M(3, 3) + M(4, 3);
		W.C = V.C;
		W.L = V.L;
		W.u = V.u;
		W.v = V.v;

		return W;
	}
	
	inline LVertex &operator*=(LVertex &V, const Matrix &M)
	{
		V.x = V.x * M(1, 1) + V.x * M(2, 1) + V.z * M(3, 1) + M(4, 1);
		V.y = V.x * M(1, 2) + V.x * M(2, 2) + V.z * M(3, 2) + M(4, 2);
		V.z = V.x * M(1, 3) + V.x * M(2, 3) + V.z * M(3, 3) + M(4, 3);

		return V;
	}
}

#endif   // LVertex_hpp