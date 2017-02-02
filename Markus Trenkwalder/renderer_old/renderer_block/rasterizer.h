/*
Copyright (c) 2007, Markus Trenkwalder

All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

* Neither the name of the library's copyright owner nor the names of its 
  contributors may be used to endorse or promote products derived from this 
  software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef RASTERIZER_EE4F8987_BDA2_434d_A01F_BB3446E535C3
#define RASTERIZER_EE4F8987_BDA2_434d_A01F_BB3446E535C3

#include <cmath>
#include <cassert>
#include <algorithm>

#include "fixed_func.h"
#include "static_assert.h"

#ifdef _MSC_VER
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// utility functions

namespace swr {
namespace detail {
	template <int block_size>
	inline void compute_gradients(
		int v0x, int v0y,
		int DX12, int DY12,
		int DX31, int DY31,
		int area, int inv_area, // area in 24.8 and inv_area in 8.24
		int z0, int z1, int z2,
		int step[3])
	{
		const int DZ12 = z0 - z1;
		const int DZ31 = z2 - z0;

		const int64_t a = (int64_t)DY12 * DZ31 - (int64_t)DZ12 * DY31;
		const int64_t b = (int64_t)DZ12 * DX31 - (int64_t)DX12 * DZ31;
		const int64_t d = -(a * v0x + b * v0y + (int64_t)area * z0);

		// why shift by 28 and not 24 as inv_area is in 8.24 ???
		// this works
		step[0] = (int32_t)((a * (block_size << 4) * -inv_area) >> 28);
		step[1] = (int32_t)((b * (block_size << 4) * -inv_area) >> 28);
		step[2] = (int32_t)((d * -inv_area) >> 28);
	}

	// input is screen x and screen y of upper left corner for the current block
	template <int block_size_log2>
	inline int solve_step(int step[3], int x, int y)
	{
		return step[0] * (x >> block_size_log2) + step[1] * 
			(y >> block_size_log2) + step[2];
	}

	template <int block_size, int block_size_log2>
	inline void floor_divmod(int numerator, int &floor, int &mod)
	{
		assert(block_size > 0);
		if(numerator >= 0) {
			// positive case, C is okay
			floor = numerator >> block_size_log2;
			mod = numerator & (block_size-1);
		} else {
			// Numerator is negative, do the right thing
			floor = -((-numerator) >> block_size_log2);
			mod = (-numerator) & (block_size-1);
			if(mod) {
				// there is a remainder
				floor--; mod = block_size - mod;
			}
		}
	}

	inline void floor_divmod(int numerator, int denominator, int *quotient, int *remainder)
	{
		int q, r;

		assert(denominator > 0);
		if (numerator >= 0) {
			q = numerator / denominator;
			r = numerator % denominator;
		} 
		else {
			q = -((-numerator) / denominator);
			r = (-numerator) % denominator;
			if (r != 0) {
				q--;
				r = denominator - r;
			}
		}

		*quotient = q;
		*remainder = r;
	}
} // end namespace swr::detail

class Rasterizer {
public:
	// MAX_VARYINGS is a hard constant now. If you change this you also need
	// to change the code in the implementation file
	static const int MAX_VARYING = 8;
	static const int BLOCK_SIZE = 8;	
	static const int BLOCK_SIZE_LOG2 = 3;

public:
	// Type definitions
	struct Vertex {
		int x, y; // in 28.4 fixed point
		int z; // range from 0 to 0x7fffffff
		int w; // in 16.16 fixed point
		int varyings[MAX_VARYING];
	};

	// This is the data the fragment shader gets
	struct FragmentData {
		int z;
		int varying[MAX_VARYING];
	};

	typedef FragmentData PixelBlock[BLOCK_SIZE][BLOCK_SIZE];
	typedef bool SkipFlag[BLOCK_SIZE][BLOCK_SIZE];

	struct {
		int x0, y0, x1, y1;
	} clip_rect_;

public:
	// constructor
	Rasterizer() : 
		triangle_func_(0), 
		line_func_(0),
		point_func_(0)
	{
		clip_rect(0, 0, 0, 0);
	}

public:
	// main interface

	void clip_rect(int x, int y, int w, int h)
	{
		clip_rect_.x0 = std::max(0, x);
		clip_rect_.y0 = std::max(0, y);
		clip_rect_.x1 = std::max(0, x + w);
		clip_rect_.y1 = std::max(0, y + h);
	}

	// set the fragment shader
	template <typename BlockShader>
	void block_shader()
	{
		triangle_func_ = &Rasterizer::triangle_template<BlockShader>;
		line_func_ = &Rasterizer::line_template<BlockShader>;
		point_func_ = &Rasterizer::point_template<BlockShader>;
	}

	void draw_triangle(const Vertex &v1, const Vertex &v2, const Vertex &v3)
	{
		if (triangle_func_)
			(this->*triangle_func_)(v1, v2, v3);
	}

	void draw_line(const Vertex &v1, const Vertex &v2)
	{
		if (line_func_)
			(this->*line_func_)(v1, v2);
	}

	void draw_point(const Vertex &v1)
	{
		if (point_func_)
			(this->*point_func_)(v1);
	}

private:
	void (Rasterizer::*triangle_func_)(const Vertex &v1, const Vertex &v2, 
		const Vertex &v3);
	void (Rasterizer::*line_func_)(const Vertex &v1, const Vertex &v2);
	void (Rasterizer::*point_func_)(const Vertex &v1);

	inline bool clip_test(int x, int y)
	{
		return (x >= clip_rect_.x0 && x < clip_rect_.x1 &&
			x >= clip_rect_.y0 && y < clip_rect_.y1);
	}

	// The triangle must be counter clockwise in screen space in order to be
	// drawn.
	template <typename BlockShader>
	void triangle_template(const Vertex &v1, const Vertex &v2, const Vertex &v3)
	{
		STATIC_ASSERT(BlockShader::varying_count >= 0);
		STATIC_ASSERT(BlockShader::varying_count <= MAX_VARYING);

		using namespace detail;

		// Deltas
		const int DX12 = v1.x - v2.x;
		const int DX23 = v2.x - v3.x;
		const int DX31 = v3.x - v1.x;

		const int DY12 = v1.y - v2.y;
		const int DY23 = v2.y - v3.y;
		const int DY31 = v3.y - v1.y;

		// Fixed-point deltas in 24.8
		const int FDX12 = DX12 << 4;
		const int FDX23 = DX23 << 4;
		const int FDX31 = DX31 << 4;

		const int FDY12 = DY12 << 4;
		const int FDY23 = DY23 << 4;
		const int FDY31 = DY31 << 4;

		const int area = DX12 * DY31 - DX31 * DY12;

		if (area <= 0xf)
			return;

		// Bounding rectangle
		int minx = (std::min(std::min(v1.x, v2.x), v3.x) + 0xF) >> 4;
		int maxx = (std::max(std::max(v1.x, v2.x), v3.x) + 0xF) >> 4;
		int miny = (std::min(std::min(v1.y, v2.y), v3.y) + 0xF) >> 4;
		int maxy = (std::max(std::max(v1.y, v2.y), v3.y) + 0xF) >> 4;

		// consider clipping rectangle
		minx = std::max(minx, clip_rect_.x0);
		maxx = std::min(maxx, clip_rect_.x1);
		miny = std::max(miny, clip_rect_.y0);
		maxy = std::min(maxy, clip_rect_.y1);

		// skip if fully clipped
		if (!(minx < maxx && miny < maxy)) 
			return;

		// Start in corner of 8x8 block
		minx &= ~(BLOCK_SIZE - 1);
		miny &= ~(BLOCK_SIZE - 1);

		// Half-edge constants
		int C1 = DY12 * v1.x - DX12 * v1.y;
		int C2 = DY23 * v2.x - DX23 * v2.y;
		int C3 = DY31 * v3.x - DX31 * v3.y;

		// Correct for fill convention
		if(DY12 < 0 || (DY12 == 0 && DX12 > 0)) C1++;
		if(DY23 < 0 || (DY23 == 0 && DX23 > 0)) C2++;
		if(DY31 < 0 || (DY31 == 0 && DX31 > 0)) C3++;

		int zStep[3];
		int wStep[3];
		int vStep[MAX_VARYING][3];

		// inv_area in 8.24
		int inv_area = fixinv<8>(area);

		if (BlockShader::interpolate_z)
			compute_gradients<BLOCK_SIZE>(v1.x, v1.y, DX12, DY12, DX31, DY31, 
			area, inv_area, v1.z, v2.z, v3.z, zStep);

		if (BlockShader::varying_count) {
			// interpolate 1/w across triangle
			int invw1 = fixinv<16>(v1.w);
			int invw2 = fixinv<16>(v2.w);
			int invw3 = fixinv<16>(v3.w);
			compute_gradients<BLOCK_SIZE>(v1.x, v1.y, DX12, DY12, DX31, DY31, 
				area, inv_area, invw1, invw2, invw3, wStep);

			for (int i = 0; i < BlockShader::varying_count; ++i) {
				int vv1 = fixmul<16>(v1.varyings[i], invw1);
				int vv2 = fixmul<16>(v2.varyings[i], invw2);
				int vv3 = fixmul<16>(v3.varyings[i], invw3);
				compute_gradients<BLOCK_SIZE>(v1.x, v1.y, DX12, DY12, DX31, DY31,
					area, inv_area, vv1, vv2, vv3, vStep[i]);
			}
		}

		// Loop through blocks
		for(int y = miny; y < maxy; y += BLOCK_SIZE)
		{
			for(int x = minx; x < maxx; x += BLOCK_SIZE)
			{
				// Corners of block
				int x0 = x << 4;
				int x1 = (x + BLOCK_SIZE - 1) << 4;
				int y0 = y << 4;
				int y1 = (y + BLOCK_SIZE - 1) << 4;

				// Evaluate half-space functions
				bool a00 = C1 + DX12 * y0 - DY12 * x0 > 0;
				bool a10 = C1 + DX12 * y0 - DY12 * x1 > 0;
				bool a01 = C1 + DX12 * y1 - DY12 * x0 > 0;
				bool a11 = C1 + DX12 * y1 - DY12 * x1 > 0;
				int a = (a00 << 0) | (a10 << 1) | (a01 << 2) | (a11 << 3);

				bool b00 = C2 + DX23 * y0 - DY23 * x0 > 0;
				bool b10 = C2 + DX23 * y0 - DY23 * x1 > 0;
				bool b01 = C2 + DX23 * y1 - DY23 * x0 > 0;
				bool b11 = C2 + DX23 * y1 - DY23 * x1 > 0;
				int b = (b00 << 0) | (b10 << 1) | (b01 << 2) | (b11 << 3);

				bool c00 = C3 + DX31 * y0 - DY31 * x0 > 0;
				bool c10 = C3 + DX31 * y0 - DY31 * x1 > 0;
				bool c01 = C3 + DX31 * y1 - DY31 * x0 > 0;
				bool c11 = C3 + DX31 * y1 - DY31 * x1 > 0;
				int c = (c00 << 0) | (c10 << 1) | (c01 << 2) | (c11 << 3);

				// Skip block when outside an edge
				if(a == 0x0 || b == 0x0 || c == 0x0) continue;

				// test for the clipping rectangle
				bool clip00 = clip_test(x, y);
				bool clip10 = clip_test(x + 7, y);
				bool clip01 = clip_test(x, y + 7);
				bool clip11 = clip_test(x + 7, y + 7);

				bool clip_all_in = clip00 && clip10 && clip01 && clip11;

				// compute attribute interpolants at corners
				FragmentData f00; 
				FragmentData f10; 
				FragmentData f01; 
				FragmentData f11; 

				if (BlockShader::interpolate_z) {
					f00.z = solve_step<BLOCK_SIZE_LOG2>(zStep, x, y);
					f10.z = f00.z + zStep[0];
					f11.z = f10.z + zStep[1];
					f01.z = f11.z - zStep[0];

					int minz = std::min(std::min(std::min(f00.z, f10.z), f01.z), 
						f11.z);
					if (!BlockShader::early_depth_test(x, y, minz))
						continue;
				}

				int w00 = 0, w10 = 0, w01 = 0, w11 = 0;

				if (BlockShader::varying_count) {
					int oneOverW00 = solve_step<BLOCK_SIZE_LOG2>(wStep, x, y);
					int oneOverW10 = oneOverW00 + wStep[0];
					int oneOverW11 = oneOverW10 + wStep[1];
					int oneOverW01 = oneOverW11 - wStep[0];

					// this compute 1/w in fixed point without the 64bit 
					// intermediate this is significantly faster than the 
					// version with the fixdiv<16> call
					w00 = fixinv<16>(oneOverW00);
					w10 = fixinv<16>(oneOverW10);
					w01 = fixinv<16>(oneOverW01);
					w11 = fixinv<16>(oneOverW11);

					for (int i = 0; i < BlockShader::varying_count; ++i) {
						f00.varying[i] = solve_step<BLOCK_SIZE_LOG2>(vStep[i], 
							x, y);
						f10.varying[i] = f00.varying[i] + vStep[i][0];
						f11.varying[i] = f10.varying[i] + vStep[i][1];
						f01.varying[i] = f11.varying[i] - vStep[i][0];			

						f00.varying[i] = fixmul<16>(f00.varying[i],w00);
						f10.varying[i] = fixmul<16>(f10.varying[i],w10);
						f11.varying[i] = fixmul<16>(f11.varying[i],w11);
						f01.varying[i] = fixmul<16>(f01.varying[i],w01);
					}
				}

				// compute attribute step y left and right
				struct varying_step_t {
					struct step_info_t {
						int step;
						int rem;
						int error_term;

						int dostep() {
							// only do the stepping with error term and 
							// remainder if the shader asks so, because it is 
							// much slower. without the error term the lower
							// BLOCK_SIZE_LOG2 bits of precision are lost
							// but if the varyings have sufficiently large
							// values this is unnoticeable (e.g. have colors in 
							// fixed point 2.30 when interpolating).
							// But beware of overflows that might happen at the
							// corners of the triangles.
							if (BlockShader::high_precision_varyings) {
								int r = step;
								error_term += rem;
								if (error_term >= BLOCK_SIZE) { 
									error_term -= BLOCK_SIZE;
									r++;
								}
								return r;
							}
							return step;
						}

						// this is used for stepping z.
						// I'm unsure wether it is a good idea to step z at a 
						// lower precision but it is a lot faster
						int dostep_z() {
							if (BlockShader::high_precision_z) {
								int r = step;
								error_term += rem;
								if (error_term >= BLOCK_SIZE) { 
									error_term -= BLOCK_SIZE;
									r++;
								}
								return r;
							}
							return step;
						}
					};

					step_info_t z;
					step_info_t varying[MAX_VARYING];

					varying_step_t(FragmentData& p1, FragmentData& p2)
					{
						if (BlockShader::interpolate_z) {
							if (BlockShader::high_precision_z)
								floor_divmod<BLOCK_SIZE, BLOCK_SIZE_LOG2>(
									p2.z - p1.z, z.step, z.rem);
							else
								z.step = (p2.z - p1.z) >> BLOCK_SIZE_LOG2;
						}
						for (int i = 0; i < BlockShader::varying_count; ++i) {
							if (BlockShader::high_precision_varyings)
								floor_divmod<BLOCK_SIZE, BLOCK_SIZE_LOG2>(
									p2.varying[i] - p1.varying[i],
									varying[i].step, varying[i].rem);
							else
								varying[i].step = 
									(p2.varying[i] - p1.varying[i]) >> 
									BLOCK_SIZE_LOG2;
						}
					}
				};

				varying_step_t step_left(f00, f01);
				varying_step_t step_right(f10, f11);			

				// Not using a for loop for STEP_FRAGMENTDATA and EFFICIENT_COPY
				// made things 10% faster in my tests (even when using 
				// -funroll-loops). So don't try to convert these to loops.

				#define STEP_FRAGMENTDATA(FDVAR, STEPVAR) \
				{ \
				if (BlockShader::interpolate_z) \
					FDVAR.z += STEPVAR.z.dostep_z(); \
				switch (BlockShader::varying_count) { \
					case 8: FDVAR.varying[7] += STEPVAR.varying[7].dostep(); \
					case 7: FDVAR.varying[6] += STEPVAR.varying[6].dostep(); \
					case 6: FDVAR.varying[5] += STEPVAR.varying[5].dostep(); \
					case 5: FDVAR.varying[4] += STEPVAR.varying[4].dostep(); \
					case 4: FDVAR.varying[3] += STEPVAR.varying[3].dostep(); \
					case 3: FDVAR.varying[2] += STEPVAR.varying[2].dostep(); \
					case 2: FDVAR.varying[1] += STEPVAR.varying[1].dostep(); \
					case 1: FDVAR.varying[0] += STEPVAR.varying[0].dostep(); \
					default:; \
				} \
				}

				// only copy the neccessary varyings
				#define EFFICIENT_COPY(SRC, DST) \
				{ \
				if (BlockShader::interpolate_z) DST.z = SRC.z; \
				switch (BlockShader::varying_count) { \
					case 8: DST.varying[7] = SRC.varying[7]; \
					case 7: DST.varying[6] = SRC.varying[6]; \
					case 6: DST.varying[5] = SRC.varying[5]; \
					case 5: DST.varying[4] = SRC.varying[4]; \
					case 4: DST.varying[3] = SRC.varying[3]; \
					case 3: DST.varying[2] = SRC.varying[2]; \
					case 2: DST.varying[1] = SRC.varying[1]; \
					case 1: DST.varying[0] = SRC.varying[0]; \
					default:; \
				} \
				}

				PixelBlock pixel_block;

				bool skip_flag[BLOCK_SIZE][BLOCK_SIZE] = {{false}};

				if (!clip_all_in) {
					for (int iy = 0; iy < BLOCK_SIZE; ++iy)
						for (int ix = 0; ix < BLOCK_SIZE; ++ix)
							skip_flag[iy][ix] = !clip_test(ix + x, iy + y);
				}

				// Accept whole block when totally covered
				if(a == 0xF && b == 0xF && c == 0xF)
				{
					// first compute all fragment data
					for(int iy = 0; iy < BLOCK_SIZE; iy++)
					{
						// compute attribute step x for this scanline
						varying_step_t stepx(f00, f10);
						FragmentData fragment_data;
						EFFICIENT_COPY(f00, fragment_data);

						for(int ix = 0; ix < BLOCK_SIZE; ix++)
						{
							EFFICIENT_COPY(fragment_data, pixel_block[iy][ix]);
							STEP_FRAGMENTDATA(fragment_data, stepx);
						}

						// step left and right attrib y
						STEP_FRAGMENTDATA(f00, step_left);
						STEP_FRAGMENTDATA(f10, step_right);
					}

					if (clip_all_in)
						BlockShader::template block<true>(pixel_block, 
							skip_flag, x, y);
					else
						BlockShader::template block<false>(pixel_block, 
							skip_flag, x, y);

				}
				else // Partially covered block
				{
					int CY1 = C1 + DX12 * y0 - DY12 * x0;
					int CY2 = C2 + DX23 * y0 - DY23 * x0;
					int CY3 = C3 + DX31 * y0 - DY31 * x0;

					for(int iy = 0; iy < BLOCK_SIZE; iy++)
					{
						int CX1 = CY1;
						int CX2 = CY2;
						int CX3 = CY3;

						// compute attribute step x for this scanline
						varying_step_t stepx(f00, f10);

						FragmentData fragment_data;
						EFFICIENT_COPY(f00, fragment_data);

						for(int ix = 0; ix < BLOCK_SIZE; ++ix)
						{
							if(CX1 <= 0 || CX2 <= 0 || CX3 <= 0)
								skip_flag[iy][ix] = true;

							// we still need to do this since the fragment
							// shader might want to compute the derivative of 
							// attibutes
							EFFICIENT_COPY(fragment_data, pixel_block[iy][ix]);

							CX1 -= FDY12;
							CX2 -= FDY23;
							CX3 -= FDY31;

							STEP_FRAGMENTDATA(fragment_data, stepx);
						}

						CY1 += FDX12;
						CY2 += FDX23;
						CY3 += FDX31;

						// step left and right attrib y
						STEP_FRAGMENTDATA(f00, step_left);
						STEP_FRAGMENTDATA(f10, step_right);
					}

					BlockShader::template block<false>(pixel_block, 
							skip_flag, x, y);
				}
			}
		}
	}


	template <typename BlockShader>
	void line_template(const Vertex &v1, const Vertex &v2)
	{
		using namespace detail;
		
		int x0 = v1.x >> 4;
		int y0 = v1.y >> 4;

		int x1 = v2.x >> 4;
		int y1 = v2.y >> 4;

		int dx = x1 - x0;
		int dy = y1 - y0;

		int adx = abs(dx);
		int ady = abs(dy);

		if (dx == 0 && dy == 0) 
			return;

		FragmentData fragment_data;

		struct step_info {
			int step;
			int remainder;
			int error_term;

			void init(int start, int end, int delta)
			{
				floor_divmod(end-start, delta, &step, &remainder);
				error_term = 0;
			}

			int do_step(int absdelta) 
			{
				int r = step;
				error_term += remainder;
				if (error_term >= absdelta) {
					error_term -= absdelta;
					r++;
				}
				return r;
			}
		} step_z, step_v[MAX_VARYING];

		if (adx > ady) {
			int xstep = dx > 0 ? 1 : -1;
			//int ystep_extra = dy > 0 ? 1 : -1;
			int ystep, remainder, error_term = 0;
			floor_divmod(dy, adx, &ystep, &remainder);

			if (BlockShader::interpolate_z) {
				fragment_data.z = v1.z;
				step_z.init(v1.z, v2.z, adx);
			}
			for (int i = 0; i < BlockShader::varying_count; ++i) {
				fragment_data.varying[i] = v1.varyings[i];
				step_v[i].init(v1.varyings[i], v2.varyings[i], adx);
			}

			int x = x0;
			int y = y0;
			while (x != x1) {
				x += xstep;
				y += ystep;
				error_term += remainder;
				if (error_term >= adx) {
					error_term -= adx;
					y++;
				}
				
				if (BlockShader::interpolate_z)
					fragment_data.z += step_z.do_step(adx);
				
				for (int i = 0; i < BlockShader::varying_count; ++i)
					fragment_data.varying[i] += step_v[i].do_step(adx);

				if (clip_test(x, y))
					BlockShader::single_fragment(x, y, fragment_data);
			}
		}
		else {
			int ystep = dy > 0 ? 1 : -1;
			//int xstep_extra = dx > 0 ? 1 : -1;
			int xstep, remainder, error_term = 0;
			floor_divmod(dx, ady, &xstep, &remainder);

			if (BlockShader::interpolate_z) {
				fragment_data.z = v1.z;
				step_z.init(v1.z, v2.z, ady);
			}
			for (int i = 0; i < BlockShader::varying_count; ++i) {
				fragment_data.varying[i] = v1.varyings[i];
				step_v[i].init(v1.varyings[i], v2.varyings[i], ady);
			}

			int x = x0;
			int y = y0;
			while (y != y1) {
				y += ystep;
				x += xstep;
				error_term += remainder;
				if (error_term >= ady) {
					error_term -= ady;
					x++;
				}

				if (BlockShader::interpolate_z)
					fragment_data.z += step_z.do_step(adx);

				for (int i = 0; i < BlockShader::varying_count; ++i)
					fragment_data.varying[i] += step_v[i].do_step(adx);

				if (clip_test(x, y))
					BlockShader::single_fragment(x, y, fragment_data);
			}
		}
	}

	template <typename BlockShader>
	void point_template(const Vertex &v1)
	{
		FragmentData fd;

		int x = v1.x >> 4;
		int y = v1.y >> 4;

		if (!clip_test(x, y)) 
			return;

		if (BlockShader::interpolate_z)
			fd.z = v1.z;

		for (int i = 0; i < BlockShader::varying_count; ++i)
			fd.varying[i] = v1.varyings[i];

		BlockShader::single_fragment(x, y, fd);
	}
};
} // end namespace swr

#undef STEP_FRAGMENTDATA
#undef EFFICIENT_COPY

#endif
