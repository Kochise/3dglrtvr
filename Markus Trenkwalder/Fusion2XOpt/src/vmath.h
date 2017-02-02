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

#ifndef VMATH_H_
#define VMATH_H_

#include "vector_math/vector_math.h"
#include "fixedpoint/fixed_class.h"

typedef fixedpoint::fixed_point<16> fixed16_t;
typedef vmath::vec2<fixed16_t> vec2x;
typedef vmath::vec3<fixed16_t> vec3x;
typedef vmath::vec4<fixed16_t> vec4x;
typedef vmath::mat2<fixed16_t> mat2x;
typedef vmath::mat3<fixed16_t> mat3x;
typedef vmath::mat4<fixed16_t> mat4x;

#endif
