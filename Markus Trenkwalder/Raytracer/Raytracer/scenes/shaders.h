#ifndef SHADERS_H
#define SHADERS_H

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

#include "math3d++.h"

#include "core/imaterialshader.h"
#include "core/ibackgroundshader.h"

//! I is the incomming ray, N is the surface normal or half vector between the
//! viewer and the lightsource.
float fresnel(const math3d::vec3& I, const math3d::vec3& N, float refractionIndex);

//! Perfect reflective material
class ReflectiveMaterialShader : public IMaterialShader {
public:
	ReflectiveMaterialShader(const color3& c);
	virtual color3 shadeRay(const Scene& scene, const Ray& ray, const IntersectionInfo& iinfo, int maxdepth) const;
	
private:
	color3 color;
};

//! Refractive material which uses the fresnel term which makes the material
//! reflective at grazing angles
class RefractiveFresnelMaterialShader : public IMaterialShader {
public:
	RefractiveFresnelMaterialShader(float refractionIndex, const color3& c, float density_);	
	virtual color3 shadeRay(const Scene& scene, const Ray& ray, const IntersectionInfo& iinfo, int maxdepth) const;

private:
	float refindex;
	color3 color;
	float density;
};

//! This backgroundshader fills the background with a constant color
class ConstantColorBackground : public IBackgroundShader {
public:
	ConstantColorBackground(const color3& c) :
		color(c)
	{
	}
	
	virtual color3 shadeRay(const Ray&) const {
		return color;
	}

private:
	color3 color;
};

class HDRCubemapBackgroundShader : public IBackgroundShader {
public:
	explicit HDRCubemapBackgroundShader(const char* filename);
	~HDRCubemapBackgroundShader();
	
	virtual color3 shadeRay(const Ray& ray) const;
	
private:
	color3 sampleFace2(int face, int px, int py) const;
	color3 sampleFace(int face, float s, float t) const;

private:
	int image_width, image_height;
	int tile_size;
	color3 *image_data;
};

#endif
