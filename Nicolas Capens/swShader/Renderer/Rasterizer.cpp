#include "Rasterizer.hpp"

#include "RenderTarget.hpp"
#include "VertexBuffer.hpp"
#include "XVertex.hpp"
#include "Error.hpp"
#include "Math.h"
#include "Texture.hpp"
#include "PS20Shader.hpp"

namespace swShader
{
	Rasterizer::Rasterizer(RenderTarget *renderTarget)
	{
		if(!renderTarget) throw Error("Unable to initialize rasterizer");

		this->renderTarget = renderTarget;

		textureDetail = 0.0f;   // Round to highest LOD [0.5, 1.0]: 0.0
	                            // Round to nearest LOD [0.7, 1.4]: 0.5
	                            // Round to lowest LOD  [1.0, 2.0]: 1.0
	                            // Nyquist rate         [2.0, 4.0]: 2.0
	}

	Rasterizer::~Rasterizer()
	{
	}

	void Rasterizer::renderTriangle(const XVertex *V1, const XVertex *V2, const XVertex *V3, int FVF)
	{
		if(!renderTarget) throw Error("No render target specified");
		if(!scanline) throw INTERNAL_ERROR;

		colorBuffer = (unsigned int*)renderTarget->getColorBuffer();
		depthBuffer = (float*)renderTarget->getDepthBuffer();

		// Sort: V1 top, V2 middle, V3 bottom
		if(V1->y > V3->y) swap(V1, V3);
		if(V2->y > V3->y) swap(V2, V3);
		if(V1->y > V2->y) swap(V1, V2);

		const XVertex &v1 = *V1;
		const XVertex &v2 = *V2;
		const XVertex &v3 = *V3;

		// 28.4 fixed-point coordinates, top-left screen corner at (-0.5, -0.5)
		const int X1 = iround(16.0f * (v1.x - 0.5f)); const int Y1 = iround(16.0f * (v1.y - 0.5f));
		const int X2 = iround(16.0f * (v2.x - 0.5f)); const int Y2 = iround(16.0f * (v2.y - 0.5f));
		const int X3 = iround(16.0f * (v3.x - 0.5f)); const int Y3 = iround(16.0f * (v3.y - 0.5f));

		// Deltas
		const int DX12 = X2 - X1; const int DY12 = Y2 - Y1;
		const int DX13 = X3 - X1; const int DY13 = Y3 - Y1;
		const int DX23 = X3 - X2; const int DY23 = Y3 - Y2;

		const int FDX12 = DX12 << 4; const int FDX13 = DX13 << 4; const int FDX23 = DX23 << 4;
		const int FDY12 = DY12 << 4; const int FDY13 = DY13 << 4; const int FDY23 = DY23 << 4;

		const float Dx12 = 0.0625f * DX12; const float Dy12 = 0.0625f * DY12;
		const float Dx13 = 0.0625f * DX13; const float Dy13 = 0.0625f * DY13;

		// Middle on long edge
		const int X0 = ceilDiv(DX13 * (Y2 - Y1) + X1 * DY13, DY13);

		// Top, middle and bottom
		const int y1 = ceilInt4(Y1);
		const int y2 = ceilInt4(Y2);
		const int y3 = ceilInt4(Y3);

		colorBuffer += y1 * renderTarget->getStride();
		depthBuffer += y1 * renderTarget->getStride();

		// Gradients
		const float _C = -1 / (Dx12 * Dy13 - Dx13 * Dy12);

		const float Dz12 = v2.z - v1.z;
		const float Dz13 = v3.z - v1.z;
		dz_dx = _C * (Dz13 * Dy12 - Dz12 * Dy13);
		dz_dy = _C * (Dz12 * Dx13 - Dz13 * Dx12);

		const float Dw12 = v2.w - v1.w;
		const float Dw13 = v3.w - v1.w;
		dw_dx = _C * (Dw13 * Dy12 - Dw12 * Dy13);
		dw_dy = _C * (Dw12 * Dx13 - Dw13 * Dx12);

		if(FVF & FVF_DIFFUSE)
		{
			const Color<float> DC12 = v2.C - v1.C;
			const Color<float> DC13 = v3.C - v1.C;
			dC_dx = _C * (Dy12 * DC13 - Dy13 * DC12);
			dC_dy = _C * (Dx13 * DC12 - Dx12 * DC13);
		}

		if(FVF & FVF_SPECULAR)
		{
			const Color<float> DL12 = v2.L - v1.L;
			const Color<float> DL13 = v3.L - v1.L;
			dL_dx = _C * (Dy12 * DL13 - Dy13 * DL12);
			dL_dy = _C * (Dx13 * DL12 - Dx12 * DL13);
		}

		const int texCount = (FVF & FVF_TEX_MASK) >> FVF_TEX_SHIFT;

		// Mipmap LOD
		float m1[8] = {0};
		float m2[8] = {0};
		float m3[8] = {0};

		for(int t = 0; t < texCount; t++)
		{
			const UVW &T1 = v1.T[t];
			const UVW &T2 = v2.T[t];
			const UVW &T3 = v3.T[t];

			float &du_dx = dT_dx[t].u;
			float &du_dy = dT_dy[t].u;
			float &dv_dx = dT_dx[t].v;
			float &dv_dy = dT_dy[t].v;
			float &dm_dx = dT_dx[t].m;
			float &dm_dy = dT_dy[t].m;

			const float Du12 = T2.u - T1.u;
			const float Du13 = T3.u - T1.u;
			du_dx = _C * (Du13 * Dy12 - Du12 * Dy13);
			du_dy = _C * (Du12 * Dx13 - Du13 * Dx12);

			const float Dv12 = T2.v - T1.v;
			const float Dv13 = T3.v - T1.v;
			dv_dx = _C * (Dv13 * Dy12 - Dv12 * Dy13);
			dv_dy = _C * (Dv12 * Dx13 - Dv13 * Dx12);

			// u, v and w at screen origin
			const float u0 = T1.u - du_dx * v1.x - du_dy * v1.y;
			const float v0 = T1.v - dv_dx * v1.x - dv_dy * v1.y;
			const float w0 = v1.w - dw_dx * v1.x - dw_dy * v1.y;

			//Mipmap constants
			const float scale = exp2(bias);

			const float Cu = scale * (du_dx * dw_dy - du_dy * dw_dx);
			const float Cv = scale * (dv_dx * dw_dy - dv_dy * dw_dx);

			const float Ux = scale * (du_dx * w0 - dw_dx * u0);
			const float Vx = scale * (dv_dx * w0 - dw_dx * v0);
			const float Uy = scale * (du_dy * w0 - dw_dy * u0);
			const float Vy = scale * (dv_dy * w0 - dw_dy * v0);

			// Mipmap LOD per vertex
			const float xMip1 = fsqrt(pow2(Ux + Cu * v1.y) + pow2(Vx + Cv * v1.y));
			const float yMip1 = fsqrt(pow2(Uy - Cu * v1.x) + pow2(Vy - Cv * v1.x));
			m1[t] = max(xMip1, yMip1);

			const float xMip2 = fsqrt(pow2(Ux + Cu * v2.y) + pow2(Vx + Cv * v2.y));
			const float yMip2 = fsqrt(pow2(Uy - Cu * v2.x) + pow2(Vy - Cv * v2.x));
			m2[t] = max(xMip2, yMip2);

			const float xMip3 = fsqrt(pow2(Ux + Cu * v3.y) + pow2(Vx + Cv * v3.y));
			const float yMip3 = fsqrt(pow2(Uy - Cu * v3.x) + pow2(Vy - Cv * v3.x));
			m3[t] = max(xMip3, yMip3);

			// Mipmap LOD gradients
			const float Dm12 = m2[t] - m1[t];
			const float Dm13 = m3[t] - m1[t];
			dm_dx = _C * (Dm13 * Dy12 - Dm12 * Dy13);
			dm_dy = _C * (Dm12 * Dx13 - Dm13 * Dx12);
		}

		// Quotient
		const int Q12 = floorDiv(FDX12, FDY12);
		const int Q13 = floorDiv(FDX13, FDY13);
		const int Q23 = floorDiv(FDX23, FDY23);

		// Remainder
		const int R12 = floorMod(FDX12, FDY12);
		const int R13 = floorMod(FDX13, FDY13);
		const int R23 = floorMod(FDX23, FDY23);

		int ld; int rd;   // error-term
		int lQ; int rQ;   // edge-step
		int lR; int rR;   // error-step
		int lD; int rD;   // error-overflow

		// Edge stepping
		float zS;
		float wS;
		Color<float> CS;
		Color<float> LS;
		UVW TS[8];

		// Prestep
		float Dx;
		float Dy;

		if(X2 < X0)
		{
			lx = ceilDiv(DX12 * (ceilFix4(Y1) - Y1) + X1 * DY12, FDY12);
			ld = ceilMod(DX12 * (ceilFix4(Y1) - Y1) + X1 * DY12, FDY12);
			rx = ceilDiv(DX13 * (ceilFix4(Y1) - Y1) + X1 * DY13, FDY13);
			rd = ceilMod(DX13 * (ceilFix4(Y1) - Y1) + X1 * DY13, FDY13);
			lQ = Q12;   rQ = Q13;
			lR = R12;   rR = R13;
			lD = FDY12; rD = FDY13;

			Dx = 0.0625f * (float)((lx << 4) - X1);
			Dy = 0.0625f * (float)((y1 << 4) - Y1);

			z = v1.z + Dx * dz_dx + Dy * dz_dy;
			zS = (float)lQ * dz_dx + dz_dy;

			w = v1.w + Dx * dw_dx + Dy * dw_dy;
			wS = (float)lQ * dw_dx + dw_dy;

			if(FVF & FVF_DIFFUSE)
			{
				C = v1.C + Dx * dC_dx + Dy * dC_dy;
				CS = (float)lQ * dC_dx + dC_dy;
			}

			if(FVF & FVF_SPECULAR)
			{
				L = v1.L + Dx * dL_dx + Dy * dL_dy;
				LS = (float)lQ * dL_dx + dL_dy;
			}

			for(int t = 0; t < texCount; t++)
			{
				const UVW T1 = v1.T[t];

				T[t].u = T1.u + Dx * dT_dx[t].u + Dy * dT_dy[t].u;
				T[t].v = T1.v + Dx * dT_dx[t].v + Dy * dT_dy[t].v;
				T[t].m = m1[t] + Dx * dT_dx[t].m + Dy * dT_dy[t].m;

				TS[t].u = (float)lQ * dT_dx[t].u + dT_dy[t].u;
				TS[t].v = (float)lQ * dT_dx[t].v + dT_dy[t].v;
				TS[t].m = (float)lQ * dT_dx[t].m + dT_dy[t].m;
			}
		}
		else
		{
			lx = ceilDiv(DX13 * (ceilFix4(Y1) - Y1) + X1 * DY13, FDY13);
			ld = ceilMod(DX13 * (ceilFix4(Y1) - Y1) + X1 * DY13, FDY13);
			rx = ceilDiv(DX12 * (ceilFix4(Y1) - Y1) + X1 * DY12, FDY12);
			rd = ceilMod(DX12 * (ceilFix4(Y1) - Y1) + X1 * DY12, FDY12);
			lQ = Q13;   rQ = Q12;
			lR = R13;   rR = R12;
			lD = FDY13; rD = FDY12;

			Dx = 0.0625f * (float)((lx << 4) - X1);
			Dy = 0.0625f * (float)((y1 << 4) - Y1);

			z = v1.z + Dx * dz_dx + Dy * dz_dy;
			zS = (float)lQ * dz_dx + dz_dy;

			w = v1.w + Dx * dw_dx + Dy * dw_dy;
			wS = (float)lQ * dw_dx + dw_dy;

			if(FVF & FVF_DIFFUSE)
			{
				C = v1.C + Dx * dC_dx + Dy * dC_dy;
				CS = (float)lQ * dC_dx + dC_dy;
			}

			if(FVF & FVF_SPECULAR)
			{
				L = v1.L + Dx * dL_dx + Dy * dL_dy;
				LS = (float)lQ * dL_dx + dL_dy;
			}

			for(int t = 0; t < texCount; t++)
			{
				const UVW T1 = v1.T[t];

				T[t].u = T1.u + Dx * dT_dx[t].u + Dy * dT_dy[t].u;
				T[t].v = T1.v + Dx * dT_dx[t].v + Dy * dT_dy[t].v;
				T[t].m = m1[t] + Dx * dT_dx[t].m + Dy * dT_dy[t].m;

				TS[t].v = (float)lQ * dT_dx[t].v + dT_dy[t].v;
				TS[t].u = (float)lQ * dT_dx[t].u + dT_dy[t].u;
				TS[t].m = (float)lQ * dT_dx[t].m + dT_dy[t].m;
			}
		}

		for(y = y1; y < y2; y++)   // Top to Middle
		{
			scanline();
	
			// Next scanline
			colorBuffer += renderTarget->getStride();
			depthBuffer += renderTarget->getWidth();

			lx += lQ;
			ld += lR;

			z += zS;
			w += wS;

			if(FVF & FVF_DIFFUSE)
			{
				C += CS;
			}

			if(FVF & FVF_DIFFUSE)
			{
				L += LS;
			}

			for(int t = 0; t < texCount; t++)
			{
				T[t].u += TS[t].u;
				T[t].v += TS[t].v;
				T[t].m += TS[t].m;
			}

			if(ld > 0)
			{
				ld -= lD;
				lx += 1;

				// Extra step
				z += dz_dx;
				w += dw_dx;

				if(FVF & FVF_DIFFUSE)
				{
					C += dC_dx;
				}

				if(FVF & FVF_DIFFUSE)
				{
					L += dL_dx;
				}

				for(int t = 0; t < texCount; t++)
				{
					T[t].u += dT_dx[t].u;
					T[t].v += dT_dx[t].v;
					T[t].m += dT_dx[t].m;
				}
			}

			rx += rQ;
			rd += rR;

			if(rd > 0)
			{
				rd -= rD;
				rx += 1;
			}
		}

		if(X2 < X0)
		{
			lx = ceilDiv(DX23 * (ceilFix4(Y2) - Y2) + X2 * DY23, FDY23);
			ld = ceilMod(DX23 * (ceilFix4(Y2) - Y2) + X2 * DY23, FDY23);
			rx = ceilDiv(DX13 * (ceilFix4(Y2) - Y1) + X1 * DY13, FDY13);
			rd = ceilMod(DX13 * (ceilFix4(Y2) - Y1) + X1 * DY13, FDY13);
			lQ = Q23;
			lR = R23;
			lD = FDY23;
		//	rQ = Q13;
		//	rR = R13;
		//	rD = FDY13;

			Dx = 0.0625f * (float)((lx << 4) - X2);
			Dy = 0.0625f * (float)((y2 << 4) - Y2);

			z = v2.z + Dx * dz_dx + Dy * dz_dy;
			zS = (float)lQ * dz_dx + dz_dy;

			w = v2.w + Dx * dw_dx + Dy * dw_dy;
			wS = (float)lQ * dw_dx + dw_dy;

			if(FVF & FVF_DIFFUSE)
			{
				C = v2.C + Dx * dC_dx + Dy * dC_dy;
				CS = (float)lQ * dC_dx + dC_dy;
			}

			if(FVF & FVF_SPECULAR)
			{
				L = v2.L + Dx * dL_dx + Dy * dL_dy;
				LS = (float)lQ * dL_dx + dL_dy;
			}

			for(int t = 0; t < texCount; t++)
			{
				const UVW T2 = v2.T[t];

				T[t].u = T2.u + Dx * dT_dx[t].u + Dy * dT_dy[t].u;
				T[t].v = T2.v + Dx * dT_dx[t].v + Dy * dT_dy[t].v;
				T[t].m = m2[t] + Dx * dT_dx[t].m + Dy * dT_dy[t].m;

				TS[t].u = (float)lQ * dT_dx[t].u + dT_dy[t].u;
				TS[t].v = (float)lQ * dT_dx[t].v + dT_dy[t].v;
				TS[t].m = (float)lQ * dT_dx[t].m + dT_dy[t].m;
			}
		}
		else
		{
		//	lx = ceilDiv(DX13 * (ceilFix4(Y2) - Y1) + X1 * DY13, FDY13);
		//	ld = ceilMod(DX13 * (ceilFix4(Y2) - Y1) + X1 * DY13, FDY13);
			rx = ceilDiv(DX23 * (ceilFix4(Y2) - Y2) + X2 * DY23, FDY23);
			rd = ceilMod(DX23 * (ceilFix4(Y2) - Y2) + X2 * DY23, FDY23);
		//	lQ = Q13;
		//	lR = R13;
		//	lD = FDY13;
			rQ = Q23;
			rR = R23;
			rD = FDY23;

		//	float Dx = 0.0625f * (float)((lx << 4) - X1);
		//	float Dy = 0.0625f * (float)((y1 << 4) - Y1);

		//	z = v1.z + Dx * dz_dx + Dy * dz_dy;
		//	zS = (float)lQ * dz_dx + dz_dy;

		//	w = v1.w + Dx * dw_dx + Dy * dw_dy;
		//	wS = (float)lQ * dw_dx + dw_dy;

		//	if(FVF & FVF_DIFFUSE)
		//	{
		//		C = v1.C + Dx * dC_dx + Dy * dC_dy;
		//		CS = (float)lQ * dC_dx + dC_dy;
		//	}

		//	if(FVF & FVF_SPECULAR)
		//	{
		//		L = v1.L + Dx * dL_dx + Dy * dL_dy;
		//		LS = (float)lQ * dL_dx + dL_dy;
		//	}

		//	for(int t = 0; t < texCount; t++)
		//	{
		//		const UVW T1 = v1.T[t];

		//		T[t].u = T1.u + Dx * dT_dx[t].u + Dy * dT_dy[t].u;
		//		T[t].v = T1.v + Dx * dT_dx[t].v + Dy * dT_dy[t].v;
		//		T[t].m = m1[t] + Dx * dT_dx[t].m + Dy * dT_dy[t].m;

		//		TS[t].u = (float)lQ * dT_dx[t].u + dT_dy[t].u;
		//		TS[t].v = (float)lQ * dT_dx[t].v + dT_dy[t].v;
		//		TS[t].m = (float)lQ * dT_dx[t].m + dT_dy[t].m;
		//	}
		}

		for(y = y2; y < y3; y++)   // Middle to bottom
		{
			scanline();

			// Next scanline
			colorBuffer += renderTarget->getStride();
			depthBuffer += renderTarget->getWidth();

			lx += lQ;
			ld += lR;

			z += zS;
			w += wS;

			if(FVF & FVF_DIFFUSE)
			{
				C += CS;
			}

			if(FVF & FVF_DIFFUSE)
			{
				L += LS;
			}

			for(int t = 0; t < texCount; t++)
			{
				T[t].u += TS[t].u;
				T[t].v += TS[t].v;
				T[t].m += TS[t].m;
			}

			if(ld > 0)
			{
				ld -= lD;
				lx += 1;

				// Extra step
				z += dz_dx;
				w += dw_dx;

				if(FVF & FVF_DIFFUSE)
				{
					C += dC_dx;
				}

				if(FVF & FVF_DIFFUSE)
				{
					L += dL_dx;
				}

				for(int t = 0; t < texCount; t++)
				{
					T[t].u += dT_dx[t].u;
					T[t].v += dT_dx[t].v;
					T[t].m += dT_dx[t].m;
				}
			}

			rx += rQ;
			rd += rR;

			if(rd > 0)
			{
				rd -= rD;
				rx += 1;
			}
		}
	}
}