#ifndef ICAMERA_H
#define ICAMERA_H

/*
	Raytracer
	Copyright (c) 2004-2005, Trenkwalder Markus
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

#include "basis.h"

//! Interface class for cameras. Cameras are used to generate rays passing through the image plane.
class ICamera : public Basis {
public:
	//! 'm' is the object space to world space transformation matrix.
	ICamera(const math3d::mat4& m, unsigned long raytype_ = 0xffffffff) :
		Basis(m),
		raytype(raytype_)
	{
	}
	
	virtual ~ICamera() {}
	
	//! Generates a ray that goes through the specified point on the image plane.
	//! The point is given in normalized device coordinates (the image plane goes
	//! from (-1, -1) at the lower left corner to (1, 1) at the upper right corner).
	//! The ray's direction vector is normalized (has unit length).
	//! The generated ray has a typemask of \raytype that was specified in the
	//! constructor.
	virtual Ray generateRay(const math3d::vec2& ndc) const = 0;
	
protected:
	//! The typemask that the rays generated via the generateRay() method should have.
	unsigned long raytype;
};

#endif
