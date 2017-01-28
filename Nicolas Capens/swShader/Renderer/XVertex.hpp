#ifndef XVertex_hpp
#define XVertex_hpp

#include "FVF.hpp"

namespace swShader
{
	struct Vertex;
	struct LVertex;
	struct LTVertex;
	struct Matrix;

	struct XVertex : public FVF<FVF_XVERTEX>
	{
		XVertex();
		XVertex(const Vertex &V);
		XVertex(const LVertex &V);
		XVertex(const LTVertex &V);
		XVertex(const XVertex &V);
		XVertex(const Point &P);
		XVertex(const Vector &v);

		XVertex& operator=(const XVertex &v);

		friend XVertex operator*(const Matrix &M, const XVertex &V);
		friend XVertex operator*(const XVertex &V, const Matrix &M);
		friend XVertex &operator*=(XVertex &V, const Matrix &M);
	};
}

#include "Vertex.hpp"
#include "LVertex.hpp"
#include "LTVertex.hpp"
#include "Matrix.hpp"

namespace swShader
{
	inline XVertex::XVertex()
	{
	}

	inline XVertex::XVertex(const Vertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		w = 1;
		u = V.u;
		v = V.v;
	}

	inline XVertex::XVertex(const LVertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		w = 1;
		C = V.C;
		L = V.L;
		u = V.u;
		v = V.v;
	}

	inline XVertex::XVertex(const LTVertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		w = V.w;
		C = V.C;
		L = V.L;
		u = V.u;
		v = V.v;
	}

	inline XVertex::XVertex(const XVertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		w = V.w;
		C = V.C;
		L = V.L;
		u0 = V.u0;
		v0 = V.v0;
		u1 = V.u1;
		v1 = V.v1;
		u2 = V.u2;
		v2 = V.v2;
		u3 = V.u3;
		v3 = V.v3;
		u4 = V.u4;
		v4 = V.v4;
		u5 = V.u5;
		v5 = V.v5;
		u6 = V.u6;
		v6 = V.v6;
		u7 = V.u7;
		v7 = V.v7;
	}

	inline XVertex::XVertex(const Vector &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = 0;
	}

	inline XVertex::XVertex(const Point &P)
	{
		x = P.x;
		y = P.y;
		z = P.z;
		w = 1;
	}

	inline XVertex &XVertex::operator=(const XVertex &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		w = V.w;
		C = V.C;
		L = V.L;
		u0 = V.u0;
		v0 = V.v0;
		u1 = V.u1;
		v1 = V.v1;
		u2 = V.u2;
		v2 = V.v2;
		u3 = V.u3;
		v3 = V.v3;
		u4 = V.u4;
		v4 = V.v4;
		u5 = V.u5;
		v5 = V.v5;
		u6 = V.u6;
		v6 = V.v6;
		u7 = V.u7;
		v7 = V.v7;

		return *this;
	}

	inline XVertex operator*(const Matrix &M, const XVertex &V)
	{
		XVertex W;

		W.x = M(1, 1) * V.x + M(1, 2) * V.y + M(1, 3) * V.z + M(1, 4) * V.w;
		W.y = M(2, 1) * V.x + M(2, 2) * V.y + M(2, 3) * V.z + M(2, 4) * V.w;
		W.z = M(3, 1) * V.x + M(3, 2) * V.y + M(3, 3) * V.z + M(3, 4) * V.w;
		W.w = M(4, 1) * V.x + M(4, 2) * V.y + M(4, 3) * V.z + M(4, 4) * V.w;
		W.C = V.C;
		W.L = V.L;
		W.u0 = V.u0;
		W.v0 = V.v0;
		W.u1 = V.u1;
		W.v1 = V.v1;
		W.u2 = V.u2;
		W.v2 = V.v2;
		W.u3 = V.u3;
		W.v3 = V.v3;
		W.u4 = V.u4;
		W.v4 = V.v4;
		W.u5 = V.u5;
		W.v5 = V.v5;
		W.u6 = V.u6;
		W.v6 = V.v6;
		W.u7 = V.u7;
		W.v7 = V.v7;

		return W;
	}
	
	inline XVertex operator*(const XVertex &V, const Matrix &M)
	{
		XVertex W;

		W.x = V.x * M(1, 1) + V.x * M(2, 1) + V.z * M(3, 1) + V.w * M(4, 1);
		W.y = V.x * M(1, 2) + V.x * M(2, 2) + V.z * M(3, 2) + V.w * M(4, 2);
		W.z = V.x * M(1, 3) + V.x * M(2, 3) + V.z * M(3, 3) + V.w * M(4, 3);
		W.w = V.x * M(1, 4) + V.x * M(2, 4) + V.z * M(3, 4) + V.w * M(4, 4);
		W.C = V.C;
		W.L = V.L;
		W.u0 = V.u0;
		W.v0 = V.v0;
		W.u1 = V.u1;
		W.v1 = V.v1;
		W.u2 = V.u2;
		W.v2 = V.v2;
		W.u3 = V.u3;
		W.v3 = V.v3;
		W.u4 = V.u4;
		W.v4 = V.v4;
		W.u5 = V.u5;
		W.v5 = V.v5;
		W.u6 = V.u6;
		W.v6 = V.v6;
		W.u7 = V.u7;
		W.v7 = V.v7;

		return W;
	}
	
	inline XVertex &operator*=(XVertex &V, const Matrix &M)
	{
		V.x = V.x * M(1, 1) + V.x * M(2, 1) + V.z * M(3, 1) + V.w * M(4, 1);
		V.y = V.x * M(1, 2) + V.x * M(2, 2) + V.z * M(3, 2) + V.w * M(4, 2);
		V.z = V.x * M(1, 3) + V.x * M(2, 3) + V.z * M(3, 3) + V.w * M(4, 3);
		V.w = V.x * M(1, 4) + V.x * M(2, 4) + V.z * M(3, 4) + V.w * M(4, 4);

		return V;
	}
}

#endif   // Vertex_hpp