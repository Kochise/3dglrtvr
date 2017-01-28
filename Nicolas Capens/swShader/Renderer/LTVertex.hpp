#ifndef LTVertex_hpp
#define LTVertex_hpp

#include "FVF.hpp"

namespace swShader
{
	struct Vertex;
	struct LVertex;
	struct Matrix;

	struct LTVertex : public FVF<FVF_LTVERTEX>
	{
		LTVertex();
		LTVertex(const Vertex &V);
		LTVertex(const LVertex &V);
		LTVertex(const LTVertex &V);
		LTVertex(const Point &P);
		LTVertex(const Vector &v);

		LTVertex &operator=(const LTVertex &v);

		friend LTVertex operator*(const Matrix &M, const LTVertex &V);
		friend LTVertex operator*(const LTVertex &V, const Matrix &M);
		friend LTVertex &operator*=(LTVertex &V, const Matrix &M);
	};
}

#include "Vertex.hpp"
#include "LVertex.hpp"
#include "Matrix.hpp"

namespace swShader
{
	inline LTVertex::LTVertex()
	{
	}

	inline LTVertex::LTVertex(const Vertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		w = 1;
		u = V.u;
		v = V.v;
	}

	inline LTVertex::LTVertex(const LVertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		w = 1;
		u = V.u;
		v = V.v;
		C = V.C;
		L = V.L;
	}

	inline LTVertex::LTVertex(const LTVertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		w = V.w;
		u = V.u;
		v = V.v;
		C = V.C;
		L = V.L;
	}

	inline LTVertex::LTVertex(const Vector &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = 0;
	}

	inline LTVertex::LTVertex(const Point &P)
	{
		x = P.x;
		y = P.y;
		z = P.z;
		w = 1;
	}

	inline LTVertex &LTVertex::operator=(const LTVertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		w = V.w;
		u = V.u;
		v = V.v;
		C = V.C;
		L = V.L;

		return *this;
	}

	inline LTVertex operator*(const Matrix &M, const LTVertex &V)
	{
		LTVertex W;

		W.x = M(1, 1) * V.x + M(1, 2) * V.y + M(1, 3) * V.z + M(1, 4) * V.w;
		W.y = M(2, 1) * V.x + M(2, 2) * V.y + M(2, 3) * V.z + M(2, 4) * V.w;
		W.z = M(3, 1) * V.x + M(3, 2) * V.y + M(3, 3) * V.z + M(3, 4) * V.w;
		W.w = M(4, 1) * V.x + M(4, 2) * V.y + M(4, 3) * V.z + M(4, 4) * V.w;
		W.u = V.u;
		W.v = V.v;
		W.C = V.C;
		W.L = V.L;

		return W;
	}
	
	inline LTVertex operator*(const LTVertex &V, const Matrix &M)
	{
		LTVertex W;

		W.x = V.x * M(1, 1) + V.x * M(2, 1) + V.z * M(3, 1) + V.w * M(4, 1);
		W.y = V.x * M(1, 2) + V.x * M(2, 2) + V.z * M(3, 2) + V.w * M(4, 2);
		W.z = V.x * M(1, 3) + V.x * M(2, 3) + V.z * M(3, 3) + V.w * M(4, 3);
		W.w = V.x * M(1, 4) + V.x * M(2, 4) + V.z * M(3, 4) + V.w * M(4, 4);
		W.u = V.u;
		W.v = V.v;
		W.C = V.C;
		W.L = V.L;

		return W;
	}
	
	inline LTVertex &operator*=(LTVertex &V, const Matrix &M)
	{
		V.x = V.x * M(1, 1) + V.x * M(2, 1) + V.z * M(3, 1) + V.w * M(4, 1);
		V.y = V.x * M(1, 2) + V.x * M(2, 2) + V.z * M(3, 2) + V.w * M(4, 2);
		V.z = V.x * M(1, 3) + V.x * M(2, 3) + V.z * M(3, 3) + V.w * M(4, 3);
		V.w = V.x * M(1, 4) + V.x * M(2, 4) + V.z * M(3, 4) + V.w * M(4, 4);

		return V;
	}
}

#endif   // LTVertex_hpp