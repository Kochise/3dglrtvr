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

#include "fixed_func.h"
#include "static_assert.h"
#include "duffsdevice.h"

#include <cmath>
#include <cassert>
#include <algorithm>

#ifdef _MSC_VER
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// utility functions

namespace swr {
namespace detail {
	inline void compute_gradients(
		int v0x, int v0y,
		int DX12, int DY12,
		int DX31, int DY31,
		int inv_area, // area in 24.8 and inv_area in 8.24
		int z0, int z1, int z2,
		int &dx, int &dy)
	{
		const int DZ12 = z0 - z1;
		const int DZ31 = z2 - z0;

		const int64_t a = (int64_t)DY12 * DZ31 - (int64_t)DZ12 * DY31;
		const int64_t b = (int64_t)DZ12 * DX31 - (int64_t)DX12 * DZ31;

		dx = (int32_t)((a * -inv_area) >> 28);
		dy = (int32_t)((b * -inv_area) >> 28);
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

	inline int ceil28_4(int value) {
		int Returnvalue;
		int Numerator = value - 1 + 16;
		if(Numerator >= 0) {
			Returnvalue = Numerator/16;
		} else {
			// deal with negative numerators correctly
			Returnvalue = -((-Numerator)/16);
			Returnvalue -= ((-Numerator) % 16) ? 1 : 0;
		}
		return Returnvalue;
	}
} // end namespace swr::detail

class Rasterizer {
public:
	static const int MAX_VARYING = 8;
	static const int AFFINE_LENGTH = 32;

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
		int varyings[MAX_VARYING];
	};

	struct FragmentDataStepping {
		int oow;
		FragmentData fd;
	};

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
	template <typename FragmentShader>
	void fragment_shader()
	{
		triangle_func_ = &Rasterizer::triangle_template<FragmentShader>;
		line_func_ = &Rasterizer::line_template<FragmentShader>;
		point_func_ = &Rasterizer::point_template<FragmentShader>;
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
	template <typename FragmentShader>
	void triangle_template(const Vertex &v1, const Vertex &v2, const Vertex &v3)
	{
		using namespace detail;

		// Deltas
		const int DX12 = v1.x - v2.x;
		const int DX31 = v3.x - v1.x;

		const int DY12 = v1.y - v2.y;
		const int DY31 = v3.y - v1.y;

		const int area = DX12 * DY31 - DX31 * DY12;

		if (area <= 0xf)
			return;
	
		// inv_area in 8.24
		const int inv_area = fixinv<8>(area);

		// sort vertices in y and determine on which side the middle vertex lies
		const Vertex *top, *middle, *bottom;
		bool middle_is_left;

		if (v1.y < v2.y) {
			if (v1.y < v3.y) {
				top = &v1;
				if (v2.y < v3.y) {
					middle = &v2;
					bottom = &v3;
					middle_is_left = true;
				} 
				else {
					middle = &v3;
					bottom = &v2;
					middle_is_left = false;
				}
			} 
			else {
				top = &v3;
				middle = &v1;
				bottom = &v2;
				middle_is_left = true;
			}			
		}
		else {
			if (v2.y < v3.y) {
				top = &v2;
				if (v1.y < v3.y) {
					middle = &v1;
					bottom = &v3;
					middle_is_left = false;
				} 
				else {
					middle = &v3;
					bottom = &v1;
					middle_is_left = true;
				}
			}
			else {
				top = &v3;
				middle = &v2;
				bottom = &v1;
				middle_is_left = false;
			}
		}

		// Helper functions for scanline drawing function
		struct Helper {
			static void step_fragment_data(const FragmentDataStepping &step,
				FragmentDataStepping &inout)
			{
				if (FragmentShader::interpolate_z)
					inout.fd.z += step.fd.z;

				if (FragmentShader::varying_count)
					inout.oow += step.oow;

				// using duffs device for loop unrolling can improve performace
				// and is useful even when using -funroll-loops in GCC
				DUFFS_DEVICE8(
					int i = 0, 
					inout.fd.varyings[i] += step.fd.varyings[i]; i++, 
					FragmentShader::varying_count,
					/**/)
			}

			static void step_fragment_data2(const FragmentDataStepping &step,
				FragmentDataStepping &inout)
			{
				if (FragmentShader::interpolate_z)
					inout.fd.z += AFFINE_LENGTH * step.fd.z;

				if (FragmentShader::varying_count)
					inout.oow += AFFINE_LENGTH * step.oow;

				// using duffs device for loop unrolling can improve performace
				// and is useful even when using -funroll-loops in GCC
				DUFFS_DEVICE8(
					int i = 0, 
					inout.fd.varyings[i] += AFFINE_LENGTH * step.fd.varyings[i];
					i++,
					FragmentShader::varying_count,
					/**/)
			}

			static void copy_fragment_data(FragmentDataStepping &dst,
				const FragmentDataStepping &src)
			{
				if (FragmentShader::interpolate_z)
					dst.fd.z = src.fd.z;
				
				if (FragmentShader::varying_count)
					dst.oow = src.oow;

				DUFFS_DEVICE8(
					int i = 0,
					dst.fd.varyings[i] = src.fd.varyings[i]; i++,
					FragmentShader::varying_count,
					/**/)
			}
		};

		// computes the gradients of the varyings to be used for stepping
		struct Gradients {
			FragmentDataStepping dx;
			FragmentDataStepping dy;

			Gradients(const Vertex &v1, const Vertex &v2, const Vertex &v3,
				int DX12, int DY12, int DX31, int DY31, int inv_area)
			{
				compute_gradients(v1.x, v1.y, DX12, DY12, DX31, DY31, 
					inv_area, v1.z, v2.z, v3.z, dx.fd.z, dy.fd.z);

				if (FragmentShader::varying_count) {
					int invw1 = fixinv<16>(v1.w);
					int invw2 = fixinv<16>(v2.w);
					int invw3 = fixinv<16>(v3.w);

					compute_gradients(v1.x, v1.y, DX12, DY12, DX31, DY31, 
						inv_area, invw1, invw2, invw3, dx.oow, dy.oow);

					for (int i = 0; i < FragmentShader::varying_count; ++i) {
						int var1 = fixmul<16>(v1.varyings[i], invw1);
						int var2 = fixmul<16>(v2.varyings[i], invw2);
						int var3 = fixmul<16>(v3.varyings[i], invw3);

						compute_gradients(v1.x, v1.y, DX12, DY12, DX31, DY31, 
							inv_area, var1, var2, var3, 
							dx.fd.varyings[i], dy.fd.varyings[i]);
					}
				}
			}
		};

		// Edge structure used to walk the edge of the triangle and fill the
		// scanlines
		struct Edge {
			int x, x_step, numerator, denominator, error_term; // DDA info for x
			int y, height;

			FragmentDataStepping fragment_data;
			FragmentDataStepping fragment_step;

			const Gradients& grad;		

			Edge(const Gradients &grad_, const Vertex* top, const Vertex *bottom)
				: grad(grad_)
			{
				y = ceil28_4(top->y);
				int yend = ceil28_4(bottom->y);
				height = yend - y;

				if (height) {
					int dn = bottom->y - top->y;
					int dm = bottom->x - top->x;

					int initial_numerator = dm*16*y - dm*top->y + 
						dn*top->x - 1 + dn*16;
					floor_divmod(initial_numerator, dn*16, &x, &error_term);
					floor_divmod(dm*16, dn*16, &x_step, &numerator);
					denominator = dn*16;

					int y_prestep = y*16 - top->y;
					int x_prestep = x*16 - top->x;

					if (FragmentShader::interpolate_z) {
						fragment_data.fd.z = top->z + 
							((y_prestep * grad.dy.fd.z) >> 4) + 
							((x_prestep * grad.dx.fd.z) >> 4);

						fragment_step.fd.z = x_step * grad.dx.fd.z + 
							grad.dy.fd.z;
					}

					if (FragmentShader::varying_count) {
						int invw = fixinv<16>(top->w);

						fragment_data.oow = invw + 
							((y_prestep * grad.dy.oow) >> 4) + 
							((x_prestep * grad.dx.oow) >> 4);

						fragment_step.oow = x_step * grad.dx.oow + 
							grad.dy.oow;

						for (int i = 0; i < FragmentShader::varying_count; ++i) {
							fragment_data.fd.varyings[i] = 
								fixmul<16>(top->varyings[i], invw) + 
								((y_prestep * grad.dy.fd.varyings[i]) >> 4) +
								((x_prestep * grad.dx.fd.varyings[i]) >> 4);

							fragment_step.fd.varyings[i] = 
								x_step * grad.dx.fd.varyings[i] + 
								grad.dy.fd.varyings[i];
						}
					}
				}
			}

			void step(bool step_varyings)
			{
				x += x_step; y++; height--;

				if (step_varyings)
					Helper::step_fragment_data(fragment_step, fragment_data);

				error_term += numerator;
				if (error_term >= denominator) {
					x++;
					if (step_varyings)
						Helper::step_fragment_data(grad.dx, fragment_data);
					error_term -= denominator;
				}
			}
		};

		Gradients grad(v1, v2, v3, DX12, DY12, DX31, DY31, inv_area);
		Edge top_middle(grad, top, middle);
		Edge top_bottom(grad, top, bottom);
		Edge middle_bottom(grad, middle, bottom);

		Edge *left, *right;
		if (middle_is_left) {
			left = &top_middle;
			right = &top_bottom;
		}
		else {
			left = &top_bottom;
			right = &top_middle;
		}

		struct Scanline {
			static void draw(const Edge *left, const Edge *right, int cl, int cr)
			{
				int y = left->y;
				int l = left->x;
				int r = std::min(right->x, cr);
				const Gradients &grad = left->grad;

				FragmentDataStepping fd;
				Helper::copy_fragment_data(fd, left->fragment_data);

				// skip pixels left of the clipping rectangle
				if (l < cl) {
					int d = cl - l;

					if (FragmentShader::interpolate_z)
						fd.fd.z += d * grad.dx.fd.z;
					
					if (FragmentShader::varying_count) {
						fd.oow += d * grad.dx.oow;
						
					DUFFS_DEVICE8(
						int i = 0,
						fd.fd.varyings[i] += d * grad.dx.fd.varyings[i]; i++,
						FragmentShader::varying_count,
						/**/)
					}
					
					l = cl;
				}
				
				int x = l;
				while (x + AFFINE_LENGTH <= r) {
					FragmentDataStepping fd_left, fd_step;

					if (FragmentShader::interpolate_z) {
						fd_left.fd.z = fd.fd.z; 
						fd_step.fd.z = fd.fd.z + AFFINE_LENGTH * grad.dx.fd.z;
						fd_step.fd.z -= fd_left.fd.z;
						fd_step.fd.z /= AFFINE_LENGTH;
					}

					if (FragmentShader::varying_count) {
						int wl = fixinv<16>(fd.oow);
						int wr = fixinv<16>(fd.oow + AFFINE_LENGTH * grad.dx.oow);

						DUFFS_DEVICE8(
							int i = 0,
							fd_left.fd.varyings[i] = fixmul<16>(fd.fd.varyings[i], wl);
							fd_step.fd.varyings[i] = fixmul<16>(fd.fd.varyings[i] +
								AFFINE_LENGTH * grad.dx.fd.varyings[i], wr);
							fd_step.fd.varyings[i] -= fd_left.fd.varyings[i];
							fd_step.fd.varyings[i] /= AFFINE_LENGTH;
							i++,
							FragmentShader::varying_count,
							/**/)
					}

					DUFFS_DEVICE8(
						/**/,
						FragmentShader::single_fragment(x, y, fd_left.fd);
						Helper::step_fragment_data(fd_step, fd_left);
						x++,
						AFFINE_LENGTH,
						/**/)

					Helper::step_fragment_data2(grad.dx, fd);
				}

				int delta = r - x;
				if (delta) {
					FragmentDataStepping fd_left, fd_step;

					if (FragmentShader::interpolate_z) {
						fd_left.fd.z = fd.fd.z; 
						fd_step.fd.z = fd.fd.z + delta * grad.dx.fd.z;
						fd_step.fd.z -= fd_left.fd.z;
						fd_step.fd.z /= delta;
					}

					if (FragmentShader::varying_count) {
						int wl = fixinv<16>(fd.oow);
						int wr = fixinv<16>(fd.oow + delta * grad.dx.oow);

						DUFFS_DEVICE8(
							int i = 0,
							fd_left.fd.varyings[i] = fixmul<16>(fd.fd.varyings[i], wl);
							fd_step.fd.varyings[i] = fixmul<16>(fd.fd.varyings[i] +
								delta * grad.dx.fd.varyings[i], wr);
							fd_step.fd.varyings[i] -= fd_left.fd.varyings[i];
							fd_step.fd.varyings[i] /= delta;
							i++,
							FragmentShader::varying_count,
							/**/)
					}

					DUFFS_DEVICE8(
						/**/,
						FragmentShader::single_fragment(x, y, fd_left.fd);
						Helper::step_fragment_data(fd_step, fd_left);
						x++,
						delta,
						/**/)
				}
			}
		};

		int height = middle_is_left ? left->height : right->height;

		// draw top triangle
		while (height) {
			if (left->y >= clip_rect_.y0 && left->y <= clip_rect_.y1) 
				Scanline::draw(left, right, clip_rect_.x0, clip_rect_.x1);
			left->step(true);
			right->step(false);
			height--;
		}

		if (middle_is_left) {
			left = &middle_bottom;
			height = left->height;
		}
 		else {
			right = &middle_bottom;
			height = right->height;
		}

		// draw bottom triangle
		while (height) {
			if (left->y >= clip_rect_.y0 && left->y <= clip_rect_.y1) 
				Scanline::draw(left, right, clip_rect_.x0, clip_rect_.x1);
			left->step(true);
			right->step(false);
			height--;
		}
	}

	template <typename FragmentShader>
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

			if (FragmentShader::interpolate_z) {
				fragment_data.z = v1.z;
				step_z.init(v1.z, v2.z, adx);
			}
			for (int i = 0; i < FragmentShader::varying_count; ++i) {
				fragment_data.varyings[i] = v1.varyings[i];
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
				
				if (FragmentShader::interpolate_z)
					fragment_data.z += step_z.do_step(adx);
				
				for (int i = 0; i < FragmentShader::varying_count; ++i)
					fragment_data.varyings[i] += step_v[i].do_step(adx);

				if (clip_test(x, y))
					FragmentShader::single_fragment(x, y, fragment_data);
			}
		}
		else {
			int ystep = dy > 0 ? 1 : -1;
			//int xstep_extra = dx > 0 ? 1 : -1;
			int xstep, remainder, error_term = 0;
			floor_divmod(dx, ady, &xstep, &remainder);

			if (FragmentShader::interpolate_z) {
				fragment_data.z = v1.z;
				step_z.init(v1.z, v2.z, ady);
			}
			for (int i = 0; i < FragmentShader::varying_count; ++i) {
				fragment_data.varyings[i] = v1.varyings[i];
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

				if (FragmentShader::interpolate_z)
					fragment_data.z += step_z.do_step(adx);

				for (int i = 0; i < FragmentShader::varying_count; ++i)
					fragment_data.varyings[i] += step_v[i].do_step(adx);

				if (clip_test(x, y))
					FragmentShader::single_fragment(x, y, fragment_data);
			}
		}
	}

	template <typename FragmentShader>
	void point_template(const Vertex &v1)
	{
		FragmentData fd;

		int x = v1.x >> 4;
		int y = v1.y >> 4;

		if (!clip_test(x, y)) 
			return;

		if (FragmentShader::interpolate_z)
			fd.z = v1.z;

		for (int i = 0; i < FragmentShader::varying_count; ++i)
			fd.varyings[i] = v1.varyings[i];

		FragmentShader::single_fragment(x, y, fd);
	}
};
} // end namespace swr

#undef STEP_FRAGMENTDATA
#undef EFFICIENT_COPY

#endif
