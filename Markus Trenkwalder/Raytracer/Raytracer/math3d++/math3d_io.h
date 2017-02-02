/*
	math3d++ - A C++ 3d math library
	Copyright (c) 2004, Trenkwalder Markus
	All rights reserved. 
	
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:
	
	- Redistributions of source code must retain the above copyright notice,
	  this list of conditions and the following disclaimer.
	  
	- Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	  
	- Neither the name of the library's copyright owner nor the names
	  of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.
	  
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
	TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
	PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	
	Contact info:
	email: trenki2@gmx.net
*/

#ifndef MATH3DPP_IO_H
#define MATH3DPP_IO_H

inline std::ostream& operator << (std::ostream& os, const vec2& v) {
	os << "[" << v[0] << ", " << v[1] << "]";
	return os;
}

inline std::ostream& operator << (std::ostream& os, const vec3& v) {
	os << "[" << v[0] << ", " << v[1] << ", " << v[2] << "]";
	return os;
}

inline std::ostream& operator << (std::ostream& os, const vec4& v) {
	os << "[" << v[0] << ", " << v[1] << ", " << v[2] <<  ", " << v[3] << "]";
	return os;
}

inline std::ostream& operator << (std::ostream& os, const mat2& m) {
	os << "[" << m[0][0] << ",\t" << m[0][1] << "]" << std::endl;
	os << "[" << m[1][0] << ",\t" << m[1][1] << "]" << std::endl;
	return os;
}

inline std::ostream& operator << (std::ostream& os, const mat3& m) {
	os << "[" << m[0][0] << ",\t" << m[0][1] << ",\t" << m[0][2] << "]" << std::endl;
	os << "[" << m[1][0] << ",\t" << m[1][1] << ",\t" << m[1][2] << "]" << std::endl;
	os << "[" << m[2][0] << ",\t" << m[2][1] << ",\t" << m[2][2] << "]" << std::endl;
	return os;
}

inline std::ostream& operator << (std::ostream& os, const mat4& m) {
	os << "[" << m[0][0] << ",\t" << m[0][1] << ",\t" << m[0][2] << ",\t" << m[0][3] <<"]" << std::endl;
	os << "[" << m[1][0] << ",\t" << m[1][1] << ",\t" << m[1][2] << ",\t" << m[1][3] <<"]" << std::endl;
	os << "[" << m[2][0] << ",\t" << m[2][1] << ",\t" << m[2][2] << ",\t" << m[2][3] <<"]" << std::endl;
	os << "[" << m[3][0] << ",\t" << m[3][1] << ",\t" << m[3][2] << ",\t" << m[3][3] <<"]" << std::endl;
	return os;
}

inline std::ostream& operator << (std::ostream& os, const quat& q) {
	os << "[" << q[0] << ", " << q[1] << ", " << q[2] <<  ", " << q[3] << "]";
	return os;
}

#endif
