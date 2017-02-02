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

#include "shaders.h"

#include "core/ray.h"
#include "core/scene.h"
#include "core/intersectioninfo.h"
#include "core/iobject.h"

#include <cmath>
#include <algorithm>

using namespace math3d;
using namespace std;

float fresnel(const vec3& I, const vec3& N, float refractionIndex) {
	float c = dot(-I, N);
	float g = std::sqrt(refractionIndex * refractionIndex + c * c - 1.0f);
	float gpc = g + c;
	float gmc = g - c;
	float cgpcm1 = c * gpc - 1.0f;
	float cgmcp1 = c * gmc + 1.0f;
	float r = (1.0f / 2.0f) * (gmc * gmc) / (gpc * gpc) * (1.0f + ((cgpcm1 * cgpcm1) / (cgmcp1 * cgmcp1)));
	return r;
}

ReflectiveMaterialShader::ReflectiveMaterialShader(const color3& c) :
	color(c)
{
}

color3 ReflectiveMaterialShader::shadeRay(const Scene& scene, const Ray& ray, const IntersectionInfo& iinfo, int maxdepth) const {
	if ( maxdepth > 0 ) {
		const vec3 pos = ray.origin + iinfo.abscissa * ray.direction;
		const vec3 n = normalized(iinfo.object->transformNormalToWorld(iinfo.normal));
		
		Ray reflected_ray(pos + n * 0.00001f, reflect(normalized(ray.direction), n));
		color3 result = scene.trace(reflected_ray, maxdepth - 1);
		return result *= color;
	}
	
	return color3(0, 0, 0);
}

RefractiveFresnelMaterialShader::RefractiveFresnelMaterialShader(float refractionIndex, const color3& c, float density_) :
	refindex(refractionIndex),
	color(c),
	density(density_)
{
}

color3 RefractiveFresnelMaterialShader::shadeRay(const Scene& scene, const Ray& ray, const IntersectionInfo& iinfo, int maxdepth) const {
	if ( maxdepth > 0 ) {
		const vec3 pos = ray.origin + iinfo.abscissa * ray.direction;
		const vec3 normal = normalized(iinfo.object->transformNormalToWorld(iinfo.normal));
		
		
		if ( dot(ray.direction, normal) < 0 ) {
			// Ray enters sphere
			const vec3 refracted_dir = refract(normalized(ray.direction), normal, 1.0003f / refindex);
			const float fresnel_term = fresnel(normalized(ray.direction), normal, refindex);
			
			color3 refracted = scene.trace(Ray(pos - 0.00001f * normal, refracted_dir), maxdepth - 1);
			color3 reflected = scene.trace(Ray(pos + 0.00001f * normal, reflect(ray.direction, normal)), maxdepth - 1);
			
			return refracted * (1.0f - fresnel_term) + reflected * fresnel_term;
		}
		else {
			// Ray exits sphere
			const vec3 refracted_dir = refract(normalized(ray.direction), -normal, refindex / 1.0003f);
			float absorb = exp(-length(ray.origin - pos) * density);
			color3 result;
			
      // total internal reflection
			if ( refracted_dir == vec3(0, 0, 0) ) {
				result = scene.trace(Ray(pos - 0.00001f * normal, reflect(ray.direction, normal)), maxdepth - 1);
			}
			else {
				result = scene.trace(Ray(pos + 0.00001f * normal, refracted_dir), maxdepth - 1);
			}
			
			return result *= 1.0f - ((1.0f - color) * (1.0f - absorb));
		}
	}
	
	return color3(0, 0, 0);
}

/*
color3 RefractiveFresnelMaterialShader::shadeRay(const Scene& scene, const Ray& ray, const IntersectionInfo& iinfo, int maxdepth) const {
	if ( maxdepth > 0 ) {
		const vec3 pos = ray.origin + iinfo.abscissa * ray.direction;
		const vec3 normal = normalized(iinfo.object->transformNormalToWorld(iinfo.normal));
		
		
		if ( dot(ray.direction, normal) < 0 ) {
			// Ray enters sphere
			const vec3 refracted_dir = refract(normalized(ray.direction), normal, 1.0003f / refindex);
			const float fresnel_term = fresnel(normalized(ray.direction), normal, refindex);
			
			color3 refracted = scene.trace(Ray(pos - 0.00001f * normal, refracted_dir), maxdepth - 1);
			color3 reflected = scene.trace(Ray(pos + 0.00001f * normal, reflect(ray.direction, normal)), maxdepth - 1);
			
			return refracted * (1.0f - fresnel_term) + reflected * fresnel_term;
		}
		else {
			// Ray exits sphere
			const vec3 refracted_dir = refract(normalized(ray.direction), -normal, refindex / 1.0003f);
			float absorb = exp(-length(ray.origin - pos) * density);
			color3 result;
			
      // total internal reflection
			if ( refracted_dir == vec3(0, 0, 0) ) {
				result = scene.trace(Ray(pos - 0.00001f * normal, reflect(ray.direction, normal)), maxdepth - 1);
			}
			else {
				const float fresnel_term = fresnel(normalized(ray.direction), -normal, 1.0003f / refindex);
				result = scene.trace(Ray(pos + 0.00001f * normal, refracted_dir), maxdepth - 1);
				color3 reflected = scene.trace(Ray(pos - 0.00001f * normal, reflect(ray.direction, -normal)), maxdepth - 1);
				
				result = result * (1.0f - fresnel_term) + reflected * fresnel_term;
			}
			
			return result *= 1.0f - ((1.0f - color) * (1.0f - absorb));
		}
	}
	
	return color3(0, 0, 0);
}
*/

HDRCubemapBackgroundShader::HDRCubemapBackgroundShader(const char* filename) :
	image_data(0)
{
	FILE *file = fopen(filename, "rb");
	
	if ( !file ) return;
	
	float byteOrder;
	
	if ( fscanf(file, "PF%i%i%f\n", &image_width, &image_height, &byteOrder) != 3 ) {
		fclose(file);
		return;
	}
	
	tile_size = image_width / 3;
	image_data = new color3[image_width * image_height];
	fread(image_data, sizeof(color3), image_width * image_height, file);			
	
	fclose(file);
}
	
HDRCubemapBackgroundShader::~HDRCubemapBackgroundShader() {
	delete image_data;
}
	
color3 HDRCubemapBackgroundShader::shadeRay(const Ray& ray) const {
	const vec3 &r = ray.direction;
	
	int imax = 0;
	float rmax = abs(r[0]);
	if ( abs(r[1]) > rmax ) { rmax = abs(r[1]); imax = 1; }
	if ( abs(r[2]) > rmax ) { rmax = abs(r[2]); imax = 2; }
	
	int face = -1;
	float s = 0.0f, t = 0.0f;
	
	switch ( imax ) {
	case 0:
		if ( r[0] > 0 ) {
			face = 3;
			s = r[2];
			t = r[1];
		}
		else {
			face = 1;
			s = -r[2];
			t = r[1];
		}
		break;
	case 1:
		if ( r[1] > 0 ) {
			face = 0;
			s = r[0];
			t = r[2];
		}
		else {
			face = 4;
			s = r[0];
			t = -r[2];
		}
		break;
	case 2:
		if ( r[2] > 0 ) {
			face = 5;
			s = r[0];
			t = -r[1];
		}
		else {
			face = 2;
			s = r[0];
			t = r[1];
		}
		break;
	}
	
	s = 0.5f * (s / rmax + 1.0f);
	t = 0.5f * (t / rmax + 1.0f);
	
	return sampleFace(face, s, t);
}
	
color3 HDRCubemapBackgroundShader::sampleFace2(int face, int px, int py) const {
	int x_offset = 0, y_offset = 0;

	switch ( face ) {
	case 0:
		x_offset = tile_size;
		y_offset = tile_size * 3;
		break;
	case 1:
		x_offset = 0;
		y_offset = tile_size * 2;
		break;
	case 2:
		x_offset = tile_size;
		y_offset = tile_size * 2;
		break;
	case 3:
		x_offset = y_offset = tile_size * 2;
		break;
	case 4:
		x_offset = y_offset = tile_size;
		break;
	case 5:
		x_offset = tile_size;
		y_offset = 0;
		break;
	}
	
	px = x_offset + max(0, min(tile_size - 1, px));
	py = y_offset + max(0, min(tile_size - 1, py));
	
	return image_data[px + py * image_width];
}
	
color3 HDRCubemapBackgroundShader::sampleFace(int face, float s, float t) const {

	s *= tile_size;
	t *= tile_size;
	
	int pxl = static_cast<int>(s - 0.5f);
	int pxh = static_cast<int>(s + 0.5f);
	int pyl = static_cast<int>(t - 0.5f);
	int pyh = static_cast<int>(t + 0.5f);
	
	float xs = s - 0.5f - pxl;
	float ys = t - 0.5f - pyl;
	
	color3 cll = sampleFace2(face, pxl, pyl);
	color3 chl = sampleFace2(face, pxh, pyl);
	color3 clh = sampleFace2(face, pxl, pyh);
	color3 chh = sampleFace2(face, pxh, pyh);
	
	return (1.0f - xs) * (1.0f - ys) * cll + xs * (1.0f - ys) * chl + (1.0f - xs) * ys * clh + xs * ys * chh;
}

	

