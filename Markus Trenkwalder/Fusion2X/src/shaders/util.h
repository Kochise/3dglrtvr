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

#ifndef SHADERUTIL_H_
#define SHADERUTIL_H_

inline unsigned short color16_modulate(unsigned short color1, unsigned short color2)
{
	unsigned r1 = color1 >> 11;
	unsigned g1 = (color1 & 0x07e0) >> 5;
	unsigned b1 = color1 & 0x001f;

	unsigned r2 = color2 >> 11;
	unsigned g2 = (color2 & 0x07e0) >> 5;
	unsigned b2 = color2 & 0x001f;

	int rf = r1 * r2 >> 5;
	int gf = g1 * g2 >> 6;
	int bf = b1 * b2 >> 5;

	return rf << 11 | gf << 5 | bf;
}

inline unsigned short color16_add(unsigned short color1, unsigned short color2)
{
	static const unsigned OVERFLOW_BITS = (1 << 16) | (1 << 11) | (1 << 5);
	unsigned result = color1 + color2;
	unsigned overflow = (result ^ color1 ^ color2) & OVERFLOW_BITS;
	result -= overflow;
	overflow -= overflow >> 5;
	result |= overflow; 
	return result;
}

// scale must be in range [0, 32]
inline unsigned short color16_scale(unsigned short color, unsigned scale)
{
#if 0
	unsigned r = color >> 11;
	unsigned g = (color & 0x07e0) >> 5;
	unsigned b = color & 0x001f;

	r *= scale; r >>= 5;
	g *= scale; g >>= 5;
	b *= scale; b >>= 5;

	return (r << 11) | (g << 5) | b;
#else
	// this should be faster
	unsigned rb = color & 0xf81f;
	unsigned g  = color & 0x07e0;

	rb *= scale;
	rb >>= 5;
	g *= scale;
	g >>= 5;

	return (rb & 0xf81f) | (g & 0x07e0);
#endif
}

#endif
