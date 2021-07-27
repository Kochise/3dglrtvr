#include "Clipper.hpp"

#include "Error.hpp"

namespace swShader
{
	Clipper::Clipper()
	{
		clipFlags = CLIP_ALL;
		stage = 0;
	}

	Clipper::~Clipper()
	{
	}

	XVertex **Clipper::clipTriangle(const XVertex &V1, const XVertex &V2, const XVertex &V3, int FVF)
	{
		B[0][0] = V1;
		B[0][1] = V2;
		B[0][2] = V3;

		P[0][0] = &B[0][0];
		P[0][1] = &B[0][1];
		P[0][2] = &B[0][2];

		stage = 0;
		numVertices = 3;

		if(!(FVF & FVF_RHW)) throw INTERNAL_ERROR;
		this->FVF = FVF;

		if(clipFlags & CLIP_NEAR)   clipNear();
		if(numVertices >= 3) {
		if(clipFlags & CLIP_FAR)    clipFar();
		if(numVertices >= 3) {
		if(clipFlags & CLIP_LEFT)   clipLeft();
		if(numVertices >= 3) {
		if(clipFlags & CLIP_RIGHT)  clipRight();
		if(numVertices >= 3) {
		if(clipFlags & CLIP_TOP)    clipTop();
		if(numVertices >= 3) {
		if(clipFlags & CLIP_BOTTOM) clipBottom();
		}}}}}

		return (XVertex**)P[stage];
	}

	int Clipper::getNumVertices() const
	{
		return numVertices;
	}

	void Clipper::setClipFlags(int flags)
	{
		clipFlags = flags;
	}

	void Clipper::clipNear()
	{
		const XVertex **V = P[stage];
		const XVertex **T = P[stage + 1];

		int t = 0;

		for(int i = 0; i < numVertices; i++)
		{
			int j = i == numVertices - 1 ? 0 : i + 1;

			float di = V[i]->z;
			float dj = V[j]->z;

			if(di >= 0)
			{
				T[t++] = V[i];

				if(dj < 0)
				{
					clipEdge(t, i, j, di / (di - dj));
					T[t++] = &B[stage][t];
				}
			}
			else
			{
				if(dj > 0)
				{
					clipEdge(t, j, i, dj / (dj - di));
					T[t++] = &B[stage][t];
				}
			}
		}

		numVertices = t;
		stage += 1;
	}

	void Clipper::clipFar()
	{
		const XVertex **V = P[stage];
		const XVertex **T = P[stage + 1];

		int t = 0;

		for(int i = 0; i < numVertices; i++)
		{
			int j = i == numVertices - 1 ? 0 : i + 1;

			float di = V[i]->w - V[i]->z;
			float dj = V[j]->w - V[j]->z;

			if(di >= 0)
			{
				T[t++] = V[i];

				if(dj < 0)
				{
					clipEdge(t, i, j, di / (di - dj));
					T[t++] = &B[stage][t];
				}
			}
			else
			{
				if(dj > 0)
				{
					clipEdge(t, j, i, dj / (dj - di));
					T[t++] = &B[stage][t];
				}
			}
		}

		numVertices = t;
		stage += 1;
	}

	void Clipper::clipLeft()
	{
		const XVertex **V = P[stage];
		const XVertex **T = P[stage + 1];

		int t = 0;

		for(int i = 0; i < numVertices; i++)
		{
			int j = i == numVertices - 1 ? 0 : i + 1;

			float di = V[i]->x;
			float dj = V[j]->x;

			if(di >= 0)
			{
				T[t++] = V[i];

				if(dj < 0)
				{
					clipEdge(t, i, j, di / (di - dj));
					T[t++] = &B[stage][t];
				}
			}
			else
			{
				if(dj > 0)
				{
					clipEdge(t, j, i, dj / (dj - di));
					T[t++] = &B[stage][t];
				}
			}
		}

		numVertices = t;
		stage += 1;
	}

	void Clipper::clipRight()
	{
		const XVertex **V = P[stage];
		const XVertex **T = P[stage + 1];

		int t = 0;

		for(int i = 0; i < numVertices; i++)
		{
			int j = i == numVertices - 1 ? 0 : i + 1;

			float di = V[i]->w - V[i]->x;
			float dj = V[j]->w - V[j]->x;

			if(di >= 0)
			{
				T[t++] = V[i];

				if(dj < 0)
				{
					clipEdge(t, i, j, di / (di - dj));
					T[t++] = &B[stage][t];
				}
			}
			else
			{
				if(dj > 0)
				{
					clipEdge(t, j, i, dj / (dj - di));
					T[t++] = &B[stage][t];
				}
			}
		}

		numVertices = t;
		stage += 1;
	}

	void Clipper::clipTop()
	{
		const XVertex **V = P[stage];
		const XVertex **T = P[stage + 1];

		int t = 0;

		for(int i = 0; i < numVertices; i++)
		{
			int j = i == numVertices - 1 ? 0 : i + 1;

			float di = V[i]->w - V[i]->y;
			float dj = V[j]->w - V[j]->y;

			if(di >= 0)
			{
				T[t++] = V[i];

				if(dj < 0)
				{
					clipEdge(t, i, j, di / (di - dj));
					T[t] = &B[stage][t];
					t++;
				}
			}
			else
			{
				if(dj > 0)
				{
					clipEdge(t, j, i, dj / (dj - di));
					T[t++] = &B[stage][t];
				}
			}
		}

		numVertices = t;
		stage += 1;
	}

	void Clipper::clipBottom()
	{
		const XVertex **V = P[stage];
		const XVertex **T = P[stage + 1];

		int t = 0;

		for(int i = 0; i < numVertices; i++)
		{
			int j = i == numVertices - 1 ? 0 : i + 1;

			float di = V[i]->y;
			float dj = V[j]->y;

			if(di >= 0)
			{
				T[t++] = V[i];

				if(dj < 0)
				{
					clipEdge(t, i, j, di / (di - dj));
					T[t++] = &B[stage][t];
				}
			}
			else
			{
				if(dj > 0)
				{
					clipEdge(t, j, i, dj / (dj - di));
					T[t++] = &B[stage][t];
				}
			}
		}

		numVertices = t;
		stage += 1;
	}

	inline void Clipper::clipEdge(int t, int i, int j, float d)
	{
		const XVertex &Vi = *P[stage][i];
		const XVertex &Vj = *P[stage][j];
		XVertex &W = B[stage][t];

		W.x = Vi.x + d * (Vj.x - Vi.x);
		W.y = Vi.y + d * (Vj.y - Vi.y);
		W.z = Vi.z + d * (Vj.z - Vi.z);
		W.w = Vi.w + d * (Vj.w - Vi.w);

		if(FVF & FVF_DIFFUSE)
		{
			W.C = Color<float>::shade(Vi.C, Vj.C, d);
		}

		if(FVF & FVF_SPECULAR)
		{
			W.L =  Color<float>::shade(Vi.L, Vj.L, d);
		}

		const int texCount = (FVF & FVF_TEX_MASK) >> FVF_TEX_SHIFT;

		if(texCount > 0) {
		W.u0 = Vi.u0 + d * (Vj.u0 - Vi.u0);
		W.v0 = Vi.v0 + d * (Vj.v0 - Vi.v0);
		if(texCount > 1) {
		W.u1 = Vi.u1 + d * (Vj.u1 - Vi.u1);
		W.v1 = Vi.v1 + d * (Vj.v1 - Vi.v1);
		if(texCount > 2) {
		W.u2 = Vi.u2 + d * (Vj.u2 - Vi.u2);
		W.v2 = Vi.v2 + d * (Vj.v2 - Vi.v2);
		if(texCount > 3) {
		W.u3 = Vi.u3 + d * (Vj.u3 - Vi.u3);
		W.v3 = Vi.v3 + d * (Vj.v3 - Vi.v3);
		if(texCount > 4) {
		W.u4 = Vi.u4 + d * (Vj.u4 - Vi.u4);
		W.v4 = Vi.v4 + d * (Vj.v4 - Vi.v4);
		if(texCount > 5) {
		W.u5 = Vi.u5 + d * (Vj.u5 - Vi.u5);
		W.v5 = Vi.v5 + d * (Vj.v5 - Vi.v5);
		if(texCount > 6) {
		W.u6 = Vi.u6 + d * (Vj.u6 - Vi.u6);
		W.v6 = Vi.v6 + d * (Vj.v6 - Vi.v6);
		if(texCount > 7) {
		W.u7 = Vi.u7 + d * (Vj.u7 - Vi.u7);
		W.v7 = Vi.v7 + d * (Vj.v7 - Vi.v7);
		}}}}}}}}
	}
}