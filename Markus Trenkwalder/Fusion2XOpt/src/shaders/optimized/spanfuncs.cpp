/*
    Fusion2X - OpenGL ES-CL 1.0 Implementation
    Copyright (C) 2008 Markus Trenkwalder

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Markus Trenkwalder
    trenki2@gmx.net
*/

#include "spanfuncs.h"
#include <algorithm>
#include <map>

#include "../util.h"

namespace
{
	int faster_div(int num, int den)
	{
		switch (den) {
		case 1: return num;
		case 2: return num / 2;
		case 3: return num / 3;
		case 4: return num / 4;
		case 5: return num / 5;
		case 6: return num / 6;
		case 7: return num / 7;
		case 8: return num / 8;
		case 9: return num / 9;
		case 10: return num / 10;
		case 11: return num / 11;
		case 12: return num / 12;
		case 13: return num / 13;
		case 14: return num / 14;
		case 15: return num / 15;
		case 16: return num / 16;
		case 17: return num / 17;
		case 18: return num / 18;
		case 19: return num / 19;
		case 20: return num / 20;
		case 21: return num / 21;
		case 22: return num / 22;
		case 23: return num / 23;
		case 24: return num / 24;
		case 25: return num / 25;
		case 26: return num / 26;
		case 27: return num / 27;
		case 28: return num / 28;
		case 29: return num / 29;
		case 30: return num / 30;
		case 31: return num / 31;
		case 32: return num / 32;
		case 33: return num / 33;
		case 34: return num / 34;
		case 35: return num / 35;
		case 36: return num / 36;
		case 37: return num / 37;
		case 38: return num / 38;
		case 39: return num / 39;
		case 40: return num / 40;
		case 41: return num / 41;
		case 42: return num / 42;
		case 43: return num / 43;
		case 44: return num / 44;
		case 45: return num / 45;
		case 46: return num / 46;
		case 47: return num / 47;
		case 48: return num / 48;
		case 49: return num / 49;
		case 50: return num / 50;
		case 51: return num / 51;
		case 52: return num / 52;
		case 53: return num / 53;
		case 54: return num / 54;
		case 55: return num / 55;
		case 56: return num / 56;
		case 57: return num / 57;
		case 58: return num / 58;
		case 59: return num / 59;
		case 60: return num / 60;
		case 61: return num / 61;
		case 62: return num / 62;
		case 63: return num / 63;
		case 64: return num / 64;
		case 65: return num / 65;
		case 66: return num / 66;
		case 67: return num / 67;
		case 68: return num / 68;
		case 69: return num / 69;
		case 70: return num / 70;
		case 71: return num / 71;
		case 72: return num / 72;
		case 73: return num / 73;
		case 74: return num / 74;
		case 75: return num / 75;
		case 76: return num / 76;
		case 77: return num / 77;
		case 78: return num / 78;
		case 79: return num / 79;
		case 80: return num / 80;
		case 81: return num / 81;
		case 82: return num / 82;
		case 83: return num / 83;
		case 84: return num / 84;
		case 85: return num / 85;
		case 86: return num / 86;
		case 87: return num / 87;
		case 88: return num / 88;
		case 89: return num / 89;
		case 90: return num / 90;
		case 91: return num / 91;
		case 92: return num / 92;
		case 93: return num / 93;
		case 94: return num / 94;
		case 95: return num / 95;
		case 96: return num / 96;
		case 97: return num / 97;
		case 98: return num / 98;
		case 99: return num / 99;
		case 100: return num / 100;
		case 101: return num / 101;
		case 102: return num / 102;
		case 103: return num / 103;
		case 104: return num / 104;
		case 105: return num / 105;
		case 106: return num / 106;
		case 107: return num / 107;
		case 108: return num / 108;
		case 109: return num / 109;
		case 110: return num / 110;
		case 111: return num / 111;
		case 112: return num / 112;
		case 113: return num / 113;
		case 114: return num / 114;
		case 115: return num / 115;
		case 116: return num / 116;
		case 117: return num / 117;
		case 118: return num / 118;
		case 119: return num / 119;
		case 120: return num / 120;
		case 121: return num / 121;
		case 122: return num / 122;
		case 123: return num / 123;
		case 124: return num / 124;
		case 125: return num / 125;
		case 126: return num / 126;
		case 127: return num / 127;
		case 128: return num / 128;
		case 129: return num / 129;
		case 130: return num / 130;
		case 131: return num / 131;
		case 132: return num / 132;
		case 133: return num / 133;
		case 134: return num / 134;
		case 135: return num / 135;
		case 136: return num / 136;
		case 137: return num / 137;
		case 138: return num / 138;
		case 139: return num / 139;
		case 140: return num / 140;
		case 141: return num / 141;
		case 142: return num / 142;
		case 143: return num / 143;
		case 144: return num / 144;
		case 145: return num / 145;
		case 146: return num / 146;
		case 147: return num / 147;
		case 148: return num / 148;
		case 149: return num / 149;
		case 150: return num / 150;
		case 151: return num / 151;
		case 152: return num / 152;
		case 153: return num / 153;
		case 154: return num / 154;
		case 155: return num / 155;
		case 156: return num / 156;
		case 157: return num / 157;
		case 158: return num / 158;
		case 159: return num / 159;
		case 160: return num / 160;
		case 161: return num / 161;
		case 162: return num / 162;
		case 163: return num / 163;
		case 164: return num / 164;
		case 165: return num / 165;
		case 166: return num / 166;
		case 167: return num / 167;
		case 168: return num / 168;
		case 169: return num / 169;
		case 170: return num / 170;
		case 171: return num / 171;
		case 172: return num / 172;
		case 173: return num / 173;
		case 174: return num / 174;
		case 175: return num / 175;
		case 176: return num / 176;
		case 177: return num / 177;
		case 178: return num / 178;
		case 179: return num / 179;
		case 180: return num / 180;
		case 181: return num / 181;
		case 182: return num / 182;
		case 183: return num / 183;
		case 184: return num / 184;
		case 185: return num / 185;
		case 186: return num / 186;
		case 187: return num / 187;
		case 188: return num / 188;
		case 189: return num / 189;
		case 190: return num / 190;
		case 191: return num / 191;
		case 192: return num / 192;
		case 193: return num / 193;
		case 194: return num / 194;
		case 195: return num / 195;
		case 196: return num / 196;
		case 197: return num / 197;
		case 198: return num / 198;
		case 199: return num / 199;
		case 200: return num / 200;
		case 201: return num / 201;
		case 202: return num / 202;
		case 203: return num / 203;
		case 204: return num / 204;
		case 205: return num / 205;
		case 206: return num / 206;
		case 207: return num / 207;
		case 208: return num / 208;
		case 209: return num / 209;
		case 210: return num / 210;
		case 211: return num / 211;
		case 212: return num / 212;
		case 213: return num / 213;
		case 214: return num / 214;
		case 215: return num / 215;
		case 216: return num / 216;
		case 217: return num / 217;
		case 218: return num / 218;
		case 219: return num / 219;
		case 220: return num / 220;
		case 221: return num / 221;
		case 222: return num / 222;
		case 223: return num / 223;
		case 224: return num / 224;
		case 225: return num / 225;
		case 226: return num / 226;
		case 227: return num / 227;
		case 228: return num / 228;
		case 229: return num / 229;
		case 230: return num / 230;
		case 231: return num / 231;
		case 232: return num / 232;
		case 233: return num / 233;
		case 234: return num / 234;
		case 235: return num / 235;
		case 236: return num / 236;
		case 237: return num / 237;
		case 238: return num / 238;
		case 239: return num / 239;
		case 240: return num / 240;
		case 241: return num / 241;
		case 242: return num / 242;
		case 243: return num / 243;
		case 244: return num / 244;
		case 245: return num / 245;
		case 246: return num / 246;
		case 247: return num / 247;
		case 248: return num / 248;
		case 249: return num / 249;
		case 250: return num / 250;
		case 251: return num / 251;
		case 252: return num / 252;
		case 253: return num / 253;
		case 254: return num / 254;
		case 255: return num / 255;
		case 256: return num / 256;
		case 257: return num / 257;
		case 258: return num / 258;
		case 259: return num / 259;
		case 260: return num / 260;
		case 261: return num / 261;
		case 262: return num / 262;
		case 263: return num / 263;
		case 264: return num / 264;
		case 265: return num / 265;
		case 266: return num / 266;
		case 267: return num / 267;
		case 268: return num / 268;
		case 269: return num / 269;
		case 270: return num / 270;
		case 271: return num / 271;
		case 272: return num / 272;
		case 273: return num / 273;
		case 274: return num / 274;
		case 275: return num / 275;
		case 276: return num / 276;
		case 277: return num / 277;
		case 278: return num / 278;
		case 279: return num / 279;
		case 280: return num / 280;
		case 281: return num / 281;
		case 282: return num / 282;
		case 283: return num / 283;
		case 284: return num / 284;
		case 285: return num / 285;
		case 286: return num / 286;
		case 287: return num / 287;
		case 288: return num / 288;
		case 289: return num / 289;
		case 290: return num / 290;
		case 291: return num / 291;
		case 292: return num / 292;
		case 293: return num / 293;
		case 294: return num / 294;
		case 295: return num / 295;
		case 296: return num / 296;
		case 297: return num / 297;
		case 298: return num / 298;
		case 299: return num / 299;
		case 300: return num / 300;
		case 301: return num / 301;
		case 302: return num / 302;
		case 303: return num / 303;
		case 304: return num / 304;
		case 305: return num / 305;
		case 306: return num / 306;
		case 307: return num / 307;
		case 308: return num / 308;
		case 309: return num / 309;
		case 310: return num / 310;
		case 311: return num / 311;
		case 312: return num / 312;
		case 313: return num / 313;
		case 314: return num / 314;
		case 315: return num / 315;
		case 316: return num / 316;
		case 317: return num / 317;
		case 318: return num / 318;
		case 319: return num / 319;
		case 320: return num / 320;
		}

		return num / den;
	}

	void clamp_span(int &left, int &step, int min, int max, unsigned n)
	{
		bool fixit = false; 

		if (left < min || left > max) {
			fixit = true;
			left = std::min(std::max(left, min), max);
		}

		int s = left + n * step;
		if (s < min || s> max) {
			fixit = true;
			s = std::min(std::max(s, min) , max);
		} 

#	if 0
		if (fixit) step = (s - left) / signed(n);
#	else
		if (fixit) step = faster_div(s - left, signed(n));
#	endif	
	}
}

void color_span(
	unsigned short *out, 
	const swr::IRasterizer::FragmentData &fd, 
	const swr::IRasterizer::FragmentData &step,
	unsigned base,
	unsigned n)
{
	int r = fd.varyings[base + 0];
	int g = fd.varyings[base + 1];
	int b = fd.varyings[base + 2];

	int rs = step.varyings[base + 0];
	int gs = step.varyings[base + 1];
	int bs = step.varyings[base + 2];

	clamp_span(r, rs, 0, 31 << 16, n);
	clamp_span(g, gs, 0, 63 << 16, n);
	clamp_span(b, bs, 0, 31 << 16, n);

	while (n--) {
		*out++ = ((r >> 16) << 11) | ((g >> 16) << 5) | (b >> 16);

		r += rs;
		g += gs;
		b += bs;
	}
}

void alpha_span(
	unsigned short *out,
	const swr::IRasterizer::FragmentData &fd,
	const swr::IRasterizer::FragmentData &step,
	unsigned base,
	unsigned n)
{
	int a = fd.varyings[base];
	int as = step.varyings[base];

	clamp_span(a, as, 0, 31 << 16, n);

	while (n--) {
		*out++ = a >> 16; a += as; 
	}
}

void fog_span(
	unsigned short *inout,
	unsigned short fog_color,
	const swr::IRasterizer::FragmentData &fd, 
	const swr::IRasterizer::FragmentData &step, 
	unsigned base, 
	unsigned n)
{
	int f = fd.varyings[base];
	int fs = step.varyings[base];

	clamp_span(f, fs, 0, 31 << 16, n);

	while (n--) {
		*inout = color16_add(color16_scale(*inout, f), color16_scale(fog_color, 31 - f));
		inout++;
		f += fs;
	}
}

void depth_span(
	unsigned short *out,
	const swr::IRasterizer::FragmentData &fd,
	const swr::IRasterizer::FragmentData &step,
	unsigned n)
{
	int z = fd.z;
	const int zs = step.z;

	while (n--) { *out++ = (z >> 15); z += zs; }
}

namespace {
	template <unsigned pot>
	unsigned log2_of_pot()
	{
		unsigned i = pot;
		unsigned r = 0;
		while (!(i & 1)) {
			i >>= 1;
			++r;
		}
		return r;
	}
}

namespace OptimizedSpanFunctions
{
	template 
	<
		int width, 
		int height, 
		GLenum texture_wrap_s, 
		GLenum texture_wrap_t,
		GLenum internal_format
	>
	static void texture_span(
		const GLTextureObject *tex,
		unsigned short *color_out,
		unsigned short *alpha_out,
		const swr::IRasterizer::FragmentData &fd, 
		const swr::IRasterizer::FragmentData &step,
		unsigned base,
		unsigned n)
	{
		int s = fd.varyings[base + 0];
		int t = fd.varyings[base + 1];

		const int ss = step.varyings[base + 0];
		const int ts = step.varyings[base + 1];

		while (n--) {
			int x = s >> 16;
			int y = t >> 16;

			if (texture_wrap_s == GL_REPEAT) x &= width - 1;
			else x = std::min(std::max(x, 0), width - 1);
			if (texture_wrap_t == GL_REPEAT) y &= height - 1;
			else y = std::min(std::max(y, 0), height - 1);

			const unsigned short color =
				*(reinterpret_cast<const unsigned short*>(&tex->data[0]) + x + (y << log2_of_pot<width>()));

			*color_out++ = color;

			if (alpha_out) {
				switch (internal_format) {
				case GL_RGB: *alpha_out++ = 31;	break;
				case GL_RGBA: *alpha_out++ = color & 0x20 ? 31 : 0;	break;
				}
			}

			s += ss;
			t += ts;
		}
	}

	static void texture_span_generic(
		const GLTextureObject *tex,
		unsigned short *color_out,
		unsigned short *alpha_out,
		const swr::IRasterizer::FragmentData &fd, 
		const swr::IRasterizer::FragmentData &step,
		unsigned base,
		unsigned n)
	{
		int s = fd.varyings[base + 0];
		int t = fd.varyings[base + 1];

		const int ss = step.varyings[base + 0];
		const int ts = step.varyings[base + 1];

		while (n--) {
			if (alpha_out) { 
				unsigned alpha = 0;
				*color_out++ = tex->sample(s, t, &alpha); 
				*alpha_out++ = alpha; 
			} else *color_out = tex->sample(s, t);

			s += ss;
			t += ts;
		}
	}

	template
	<
		GLenum texture_env_mode,
		bool do_alpha
	>
	static void texture_env_span(
		const F2X_Context *ctx,
		unsigned short *fcolor,
		unsigned short *falpha,
		const unsigned short *tcolor,
		const unsigned short *talpha,
		unsigned short n)
	{
		while (n--) {
			unsigned short fa;
			unsigned short ta;

			if (do_alpha) {
				fa = falpha ? *falpha : 31;
				ta = talpha ? *talpha : 31;
			}

			switch (texture_env_mode) {
			case GL_DECAL:
				if (do_alpha) 
					*fcolor = color16_add(color16_scale(*fcolor, 31 - ta), color16_scale(*tcolor, ta));
				else 
					*fcolor = *tcolor;
				break;
			case GL_BLEND:
				{
					const unsigned short texture_env_color = 
						ctx->derived_state.texture_units[0].texture_env_color;
					const unsigned short a = color16_modulate(*fcolor, ~*tcolor);
					const unsigned short b = color16_modulate(texture_env_color, *tcolor);
					*fcolor = color16_add(a, b);
					if (do_alpha) fa = (fa * ta) >> 5;
					break;
				}
			case GL_ADD:
				*fcolor = color16_add(*fcolor, *tcolor);
				if (do_alpha) fa = (fa * ta) >> 5;
				break;
			case GL_MODULATE:
				*fcolor = color16_modulate(*fcolor, *tcolor);
				if (do_alpha) fa = (fa * ta) >> 5;
				break;
			case GL_REPLACE:
				*fcolor = *tcolor;
				if (do_alpha) fa = ta;
				break;
			}

			fcolor++;
			tcolor++;

			if (do_alpha) {
				if (falpha) *falpha++ = fa;
				if (talpha) talpha++;
			}
		}
	}

	static void texture_env_span_generic(
		const F2X_Context *ctx,
		unsigned short *fcolor,
		unsigned short *falpha,
		const unsigned short *tcolor,
		const unsigned short *talpha,
		unsigned short n)
	{
		const GLenum texture_env_mode = ctx->gl_state.texture_units[0].texture_env_mode;

		while (n--) {
			unsigned short fa = falpha ? *falpha : 31;
			unsigned short ta = talpha ? *talpha : 31;

			switch (texture_env_mode) {
			case GL_DECAL:
				*fcolor = color16_add(color16_scale(*fcolor, 31 - ta), color16_scale(*tcolor, ta));
				break;
			case GL_BLEND:
				{
					const unsigned short texture_env_color = 
						ctx->derived_state.texture_units[0].texture_env_color;
					const unsigned short a = color16_modulate(*fcolor, ~*tcolor);
					const unsigned short b = color16_modulate(texture_env_color, *tcolor);
					*fcolor = color16_add(a, b);
					fa = (fa * ta) >> 5;
					break;
				}
			case GL_ADD:
				*fcolor = color16_add(*fcolor, *tcolor);
				fa = (fa * ta) >> 5;
				break;
			case GL_MODULATE:
				*fcolor = color16_modulate(*fcolor, *tcolor);
				fa = (fa * ta) >> 5;
				break;
			case GL_REPLACE:
				*fcolor = *tcolor;
				fa = ta;
				break;
			}

			fcolor++;
			tcolor++;

			if (falpha) *falpha++ = fa;
			if (talpha) talpha++;
		}
	}

	// the alpha test span functions take the alpha (computed from interpolated alpha and texture) and
	// do the alpha test. it will write a 1 if the fragment is to be killed
	template
	<
		GLenum alpha_test_func
	>
	void alpha_test_span(
		const F2X_Context *ctx,
		const unsigned short *alpha,
		unsigned short *out,
		unsigned n)
	{
		const int ref = std::min(ctx->gl_state.pixel_op.alpha_test.ref >> 11, 31);

		while (n--) {
			switch (alpha_test_func) {
			case GL_NEVER: 
				*out = 1; 
				break;
			case GL_LESS: 
				if (!(*alpha < ref)) *out = 1;
				break;
			case GL_EQUAL:
				if (!(*alpha == ref)) *out = 1;
				break;
			case GL_LEQUAL:
				if (!(*alpha <= ref)) *out = 1;
				break;
			case GL_GREATER:
				if (!(*alpha > ref)) *out = 1;
				break;
			case GL_NOTEQUAL:
				if (!(*alpha != ref)) *out = 1;
				break;
			case GL_GEQUAL:
				if (!(*alpha >= ref)) *out = 1;
				break;
			case GL_ALWAYS:
				break;
			}

			alpha++;
			out++;
		}
	}

	void alpha_test_span_generic(
		const F2X_Context *ctx,
		const unsigned short *alpha,
		unsigned short *out,
		unsigned n)
	{
		const int ref = std::min(ctx->gl_state.pixel_op.alpha_test.ref >> 11, 31);

		while (n--) {
			switch (ctx->gl_state.pixel_op.alpha_test.func) {
			case GL_NEVER: 
				*out = 1; 
				break;
			case GL_LESS: 
				if (!(*alpha < ref)) *out = 1;
				break;
			case GL_EQUAL:
				if (!(*alpha == ref)) *out = 1;
				break;
			case GL_LEQUAL:
				if (!(*alpha <= ref)) *out = 1;
				break;
			case GL_GREATER:
				if (!(*alpha > ref)) *out = 1;
				break;
			case GL_NOTEQUAL:
				if (!(*alpha != ref)) *out = 1;
				break;
			case GL_GEQUAL:
				if (!(*alpha >= ref)) *out = 1;
				break;
			case GL_ALWAYS:
				break;
			}

			alpha++;
			out++;
		}
	}

	template
	<
		GLenum depth_func
	>
	void depth_test_span(
		const F2X_Context *ctx,
		const unsigned short *src,
		const unsigned short *dst,
		unsigned short *out,
		unsigned n)
	{
		while (n--) {
			switch (depth_func) {
			case GL_NEVER: 
				*out = 1;
				break;
			case GL_LESS:
				if (!(*src < *dst)) *out = 1;
				break;
			case GL_EQUAL:
				if (!(*src == *dst)) *out = 1;
				break;
			case GL_LEQUAL:
				if (!(*src <= *dst)) *out = 1;
				break;
			case GL_GREATER:
				if (!(*src > *dst)) *out = 1;
				break;
			case GL_NOTEQUAL:
				if (!(*src != *dst)) *out = 1;
				break;
			case GL_GEQUAL:
				if (!(*src >= *dst)) *out = 1;
				break;
			case GL_ALWAYS:
				break;
			}

			src++;
			dst++;
			out++;
		}	
	}

	void depth_test_span_generic(
		const F2X_Context *ctx,
		const unsigned short *src,
		const unsigned short *dst,
		unsigned short *out,
		unsigned n)
	{
		while (n--) {
			switch (ctx->gl_state.pixel_op.depth_test.func) {
			case GL_NEVER: 
				*out = 1;
				break;
			case GL_LESS:
				if (!(*src < *dst)) *out = 1;
				break;
			case GL_EQUAL:
				if (!(*src == *dst)) *out = 1;
				break;
			case GL_LEQUAL:
				if (!(*src <= *dst)) *out = 1;
				break;
			case GL_GREATER:
				if (!(*src > *dst)) *out = 1;
				break;
			case GL_NOTEQUAL:
				if (!(*src != *dst)) *out = 1;
				break;
			case GL_GEQUAL:
				if (!(*src >= *dst)) *out = 1;
				break;
			case GL_ALWAYS:
				break;
			}

			src++;
			dst++;
			out++;
		}	
	}

	template
	<
		GLenum src_factor,
		GLenum dst_factor
	>
	void blend_span(
		const F2X_Context *ctx,
		unsigned short *src_color,
		const unsigned short *src_alpha,
		const unsigned short *dst_color,
		unsigned n)
	{
		while (n--) {
			unsigned short src = *src_color;
			unsigned short dst = *dst_color;
			unsigned short alpha = *src_alpha;

			switch (src_factor) {
				case GL_ZERO:
				case GL_SRC_ALPHA_SATURATE:
				case GL_ONE_MINUS_DST_ALPHA:
					src = 0;
					break;
				case GL_ONE: break;
				case GL_DST_ALPHA: break;
				case GL_DST_COLOR: src = color16_modulate(src, dst); break;
				case GL_ONE_MINUS_DST_COLOR: src = color16_modulate(src, ~dst); break;
				case GL_SRC_ALPHA: src = color16_scale(src, alpha); break;
				case GL_ONE_MINUS_SRC_ALPHA: src = color16_scale(src, 31 - alpha); break;
			}

			switch (dst_factor) {
				case GL_ZERO:
				case GL_ONE_MINUS_DST_ALPHA: 
					dst = 0; 
					break;
				case GL_ONE: break;
				case GL_DST_ALPHA: break;
				case GL_SRC_COLOR: dst = color16_modulate(dst, src); break;
				case GL_ONE_MINUS_SRC_COLOR: dst = color16_modulate(dst, ~src); break;
				case GL_SRC_ALPHA: dst = color16_scale(dst, alpha); break;
				case GL_ONE_MINUS_SRC_ALPHA: dst = color16_scale(dst, 31 - alpha); break;
			}

			*src_color = color16_add(src, dst);

			src_color++;
			src_alpha++;
			dst_color++;
		}
	}

	void blend_span_generic(
		const F2X_Context *ctx,
		unsigned short *src_color,
		const unsigned short *src_alpha,
		const unsigned short *dst_color,
		unsigned n)
	{
		while (n--) {
			unsigned short src = *src_color;
			unsigned short dst = *dst_color;
			unsigned short alpha = *src_alpha;

			switch (ctx->gl_state.pixel_op.blend.src) {
				case GL_ZERO:
				case GL_SRC_ALPHA_SATURATE:
				case GL_ONE_MINUS_DST_ALPHA:
					src = 0;
					break;
				case GL_ONE: break;
				case GL_DST_ALPHA: break;
				case GL_DST_COLOR: src = color16_modulate(src, dst); break;
				case GL_ONE_MINUS_DST_COLOR: src = color16_modulate(src, ~dst); break;
				case GL_SRC_ALPHA: src = color16_scale(src, alpha); break;
				case GL_ONE_MINUS_SRC_ALPHA: src = color16_scale(src, 31 - alpha); break;
			}

			switch (ctx->gl_state.pixel_op.blend.dst) {
				case GL_ZERO:
				case GL_ONE_MINUS_DST_ALPHA: 
					dst = 0; 
					break;
				case GL_ONE: break;
				case GL_DST_ALPHA: break;
				case GL_SRC_COLOR: dst = color16_modulate(dst, src); break;
				case GL_ONE_MINUS_SRC_COLOR: dst = color16_modulate(dst, ~src); break;
				case GL_SRC_ALPHA: dst = color16_scale(dst, alpha); break;
				case GL_ONE_MINUS_SRC_ALPHA: dst = color16_scale(dst, 31 - alpha); break;
			}

			*src_color = color16_add(src, dst);

			src_color++;
			src_alpha++;
			dst_color++;
		}
	}

	struct TextureSpanDescCompare {
		bool operator()(const TextureSpanDesc &lhs, const TextureSpanDesc &rhs) const
		{
			bool x = true;

			return
				lhs.width < rhs.width || (x = x && lhs.width == rhs.width) &&
				lhs.height < rhs.height || (x = x && lhs.height == rhs.height) &&
				lhs.texture_wrap_s < rhs.texture_wrap_s || (x = x && lhs.texture_wrap_s == rhs.texture_wrap_s) &&
				lhs.texture_wrap_t < rhs.texture_wrap_t || (x = x && lhs.texture_wrap_t == rhs.texture_wrap_t) &&
				lhs.internal_format < rhs.internal_format;
		}
	};

	struct TextureEnvSpanCompare {
		bool operator() (const TextureEnvSpanDesc &lhs, const TextureEnvSpanDesc &rhs) const
		{
			bool x = true;

			return
				lhs.texture_env_mode < rhs.texture_env_mode || (x = x && lhs.texture_env_mode == rhs.texture_env_mode) &&
				lhs.do_alpha < rhs.do_alpha;
		}
	};

	struct BlendSpanCompare {
		bool operator() (const BlendSpanDesc &lhs, const BlendSpanDesc &rhs) const
		{
			bool x = true;

			return
				lhs.src_factor < rhs.src_factor || (x = x && lhs.src_factor < rhs.src_factor) && 
				lhs.dst_factor < rhs.dst_factor;
		}
	};

	typedef std::map<TextureSpanDesc, texture_span_t, TextureSpanDescCompare> texture_span_map_t;
	static texture_span_map_t texture_spans;
	
	texture_span_t get_texture_span(const TextureSpanDesc &span_desc)
	{
		texture_span_map_t::const_iterator i = texture_spans.find(span_desc);
		if (i == texture_spans.end()) return &texture_span_generic;
		else return i->second;
	}

	typedef std::map<TextureEnvSpanDesc, texture_env_span_t, TextureEnvSpanCompare> texture_env_span_map_t;
	static texture_env_span_map_t texture_env_spans;

	texture_env_span_t get_texture_env_span(const TextureEnvSpanDesc &desc)
	{
		texture_env_span_map_t::const_iterator i = texture_env_spans.find(desc);
		if (i == texture_env_spans.end()) return &texture_env_span_generic;
		else return i->second;
	}

	typedef std::map<GLenum, alpha_test_span_t> alpha_test_span_map_t;
	static alpha_test_span_map_t alpha_test_spans;

	alpha_test_span_t get_alpha_test_span(GLenum func)
	{
		alpha_test_span_map_t::const_iterator i = alpha_test_spans.find(func);
		if (i == alpha_test_spans.end()) return &alpha_test_span_generic;
		else return i->second;
	}

	typedef std::map<GLenum, depth_test_span_t> depth_test_span_map_t;
	static depth_test_span_map_t depth_test_spans;

	depth_test_span_t get_depth_test_span(GLenum func)
	{
		depth_test_span_map_t::const_iterator i = depth_test_spans.find(func);
		if (i == depth_test_spans.end()) return &depth_test_span_generic;
		else return i->second;
	}

	typedef std::map<BlendSpanDesc, blend_span_t, BlendSpanCompare> blend_span_map_t;
	static blend_span_map_t blend_spans;

	blend_span_t get_blend_span(const BlendSpanDesc &func)
	{
		blend_span_map_t::const_iterator i = blend_spans.find(func);
		if (i == blend_spans.end()) return &blend_span_generic;
		else return i->second;
	}

#define TEXTURE_SPAN_FUNC(W, H, WS, WT, FMT) \
	do \
	{ \
		TextureSpanDesc desc; \
		desc.width = W; \
		desc.height = H; \
		desc.texture_wrap_s = WS; \
		desc.texture_wrap_t = WT; \
		desc.internal_format = FMT; \
		texture_spans[desc] = &texture_span<W, H, WS, WT, FMT>; \
	} while (0)

#define TEXTURE_ENV_SPAN_FUNC(M, A) \
	do \
	{ \
		TextureEnvSpanDesc desc; \
		desc.texture_env_mode = M; \
		desc.do_alpha = A; \
		texture_env_spans[desc] = &texture_env_span<M, A>; \
	} while (0)

#define ALPHA_SPAN_FUNC(F) \
	do \
	{ \
		GLenum func = F; \
		alpha_test_spans[func] = &alpha_test_span<F>; \
	} while (0)

#define DEPTH_SPAN_FUNC(F) \
	do \
	{ \
		GLenum func = F; \
		depth_test_spans[func] = &depth_test_span<F>; \
	} while (0)

#define BLEND_SPAN_FUNC(S, D) \
	do \
	{ \
		BlendSpanDesc desc; \
		desc.src_factor = S; \
		desc.dst_factor = D; \
		blend_spans[desc] = &blend_span<S, D>; \
	} while (0);

	struct Init {
		Init()
		{
			// TODO: add more specialized functions
			TEXTURE_SPAN_FUNC(16, 16, GL_REPEAT, GL_REPEAT, GL_RGB);
			TEXTURE_SPAN_FUNC(16, 16, GL_REPEAT, GL_REPEAT, GL_RGBA);
			TEXTURE_SPAN_FUNC(32, 32, GL_REPEAT, GL_REPEAT, GL_RGB);
			TEXTURE_SPAN_FUNC(32, 32, GL_REPEAT, GL_REPEAT, GL_RGBA);
			TEXTURE_SPAN_FUNC(64, 64, GL_REPEAT, GL_REPEAT, GL_RGB);
			TEXTURE_SPAN_FUNC(64, 64, GL_REPEAT, GL_REPEAT, GL_RGBA);
			TEXTURE_SPAN_FUNC(128, 128, GL_REPEAT, GL_REPEAT, GL_RGB);
			TEXTURE_SPAN_FUNC(128, 128, GL_REPEAT, GL_REPEAT, GL_RGBA);
			TEXTURE_SPAN_FUNC(256, 256, GL_REPEAT, GL_REPEAT, GL_RGB);
			TEXTURE_SPAN_FUNC(256, 256, GL_REPEAT, GL_REPEAT, GL_RGBA);

			TEXTURE_ENV_SPAN_FUNC(GL_DECAL, true);
			TEXTURE_ENV_SPAN_FUNC(GL_BLEND, true);
			TEXTURE_ENV_SPAN_FUNC(GL_ADD, true);
			TEXTURE_ENV_SPAN_FUNC(GL_MODULATE, true);
			TEXTURE_ENV_SPAN_FUNC(GL_REPLACE, true);

			ALPHA_SPAN_FUNC(GL_NEVER);
			ALPHA_SPAN_FUNC(GL_LESS);
			ALPHA_SPAN_FUNC(GL_EQUAL);
			ALPHA_SPAN_FUNC(GL_LEQUAL);
			ALPHA_SPAN_FUNC(GL_GREATER);
			ALPHA_SPAN_FUNC(GL_NOTEQUAL);
			ALPHA_SPAN_FUNC(GL_GEQUAL);
			ALPHA_SPAN_FUNC(GL_ALWAYS);

			DEPTH_SPAN_FUNC(GL_NEVER);
			DEPTH_SPAN_FUNC(GL_LESS);
			DEPTH_SPAN_FUNC(GL_EQUAL);
			DEPTH_SPAN_FUNC(GL_LEQUAL);
			DEPTH_SPAN_FUNC(GL_GREATER);
			DEPTH_SPAN_FUNC(GL_NOTEQUAL);
			DEPTH_SPAN_FUNC(GL_GEQUAL);
			DEPTH_SPAN_FUNC(GL_ALWAYS);

			BLEND_SPAN_FUNC(GL_ONE, GL_ONE);
			BLEND_SPAN_FUNC(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			BLEND_SPAN_FUNC(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	} init;

#undef TEXTURE_SPAN_FUNC
#undef TEXTURE_ENV_SPAN_FUNC
#undef ALPHA_SPAN_FUNC
#undef BLEND_SPAN_FUNC

}
