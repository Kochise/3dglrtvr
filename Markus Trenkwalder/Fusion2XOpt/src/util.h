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

#ifndef UTIL_H_
#define UTIL_H_

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <algorithm>

inline GLfixed clamp_fixed(GLfixed value)
{
	return std::min(std::max(value, 0), 1 << 16);
}

inline unsigned short color_to_ushort(const vec4x &color)
{
	int r = (color[0].intValue * ((1 << 5) - 1)) >> 16;
	int g = (color[1].intValue * ((1 << 6) - 1)) >> 16;
	int b = (color[2].intValue * ((1 << 5) - 1)) >> 16;
	return (r << 11) | (g << 5) | (b);
}

#endif
