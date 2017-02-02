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

#include "scenecommon.h"

#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>

using namespace math3d;
using namespace std;

namespace {
	
	class InnerSphereMaterialShader : public IMaterialShader {
	private:
		float refindexOuter;
		color3 colorOuter;
		float densityOuter;
	public:
		InnerSphereMaterialShader(float refindex, const color3& c, float d) :
			refindexOuter(refindex),
			colorOuter(c),
			densityOuter(d)
		{}
		
		virtual color3 shadeRay(const Scene& scene, const Ray& ray, const IntersectionInfo& iinfo, int maxdepth) const {
			
			if ( maxdepth > 0 ) {
				const vec3 pos = ray.origin + iinfo.abscissa * ray.direction;
				const vec3 normal = normalized(iinfo.object->transformNormalToWorld(iinfo.normal));
				
				if ( dot(ray.direction, normal) < 0 ) {
					// Ray exits outer sphere
					const vec3 refracted_dir = refract(normalized(ray.direction), normal, refindexOuter / 1.0003f);
					float absorb = exp(-length(ray.origin - pos) * densityOuter);
					color3 result;
					
					// total internal reflection
					if ( refracted_dir == vec3(0, 0, 0) ) {
						result = scene.trace(Ray(pos + 0.0001f * normal, reflect(ray.direction, normal)), maxdepth - 1);
					}
					else {
						result = scene.trace(Ray(pos - 0.0001f * normal, refracted_dir), maxdepth - 1);
					}
					
					return result *= 1.0f - ((1.0f - colorOuter) * (1.0f - absorb));
				}
				else {
					// Ray enters outer sphere
					const vec3 refracted_dir = refract(normalized(ray.direction), -normal, 1.0003f / refindexOuter);
					const float fresnel_term = fresnel(normalized(ray.direction), -normal, refindexOuter);
					
					color3 refracted = scene.trace(Ray(pos + 0.0001f * normal, refracted_dir), maxdepth - 1);
					color3 reflected = scene.trace(Ray(pos - 0.0001f * normal, reflect(ray.direction, normal)), maxdepth - 1);
					
					return refracted * (1.0f - fresnel_term) + reflected * fresnel_term;
				}
			}
	
			return color3(0);
		}
	};
	
	struct SphereData {
		vec3 pos;
		float r;
		
		SphereData(const vec3 p, float _r) :
			pos(p),
			r(_r)
		{}
	};
	
	vector<SphereData> spheres;
	
	float frand() {
		return static_cast<float>(rand()) / RAND_MAX;
	}
	
	bool intersects(const vec3& pos, float r) {
		for ( unsigned i = 0; i < spheres.size(); ++i ) {
			if ( length2(spheres[i].pos - pos) <= (spheres[i].r + r) * (spheres[i].r + r) )
				return true;
		}
		
		return false;
	}
	
	void createRandomSpheres(float outerRadius) {
		vec3 pos;
		float r;
		
		for ( int i = 0; i < 250; ++i ) {
			do {
				pos[0] = outerRadius * (2.0f * frand() - 1.0f);
				pos[1] = outerRadius * (2.0f * frand() - 1.0f);
				pos[2] = outerRadius * (2.0f * frand() - 1.0f);
				r = frand() * 0.02f + 0.03f ;
			} while ( length(pos) + r >= outerRadius || intersects(pos, r));
			
			spheres.push_back(SphereData(pos, r));
		}
	
		for ( int i = 0; i < 40; ++i ) {
			do {
				pos[0] = outerRadius * (2.0f * frand() - 1.0f);
				pos[1] = outerRadius * (2.0f * frand() - 1.0f);
				pos[2] = outerRadius * (2.0f * frand() - 1.0f);
				r = frand() * 0.05f + 0.05f ;
			} while ( length(pos) + r >= outerRadius || intersects(pos, r));
			
			spheres.push_back(SphereData(pos, r));
		}
	}
}

void scene6main() {
	IObject *sphereOuter = new Sphere(scale(2.5, 2.5, 2.5),new RefractiveFresnelMaterialShader(1.52f, color3(0, 0.75, 0), 2.0f));
	
	CompoundObject *compound = new CompoundObject(identity4());
	compound->addObject(sphereOuter);
	
	createRandomSpheres(2.5);
	
	for ( unsigned i = 0; i < spheres.size(); ++i ) {
		compound->addObject(new Sphere(translate(spheres[i].pos) * scale(spheres[i].r), new InnerSphereMaterialShader(1.5f, color3(0, 0.75, 0), 2.0f)));
	}
	
	IBackgroundShader *hdr_cubemap = new HDRCubemapBackgroundShader("data/rnl_cross.pfm");
	
	Scene scene(compound, hdr_cubemap);
	Framebuffer framebuffer(640, 480);
	
	mat4 camera_transform =
		inverse(
			lookat(
				vec3(0, 0, -7.5),
				vec3(0, 0, 0),
				vec3(0, 1, 0)
			)
		);
		
	float fovy = 60.0f / 180.0f * M_PI;
	float aspect = static_cast<float>(framebuffer.width()) / framebuffer.height();
	
	PinholeCamera camera(camera_transform, fovy, aspect);
	
	Renderer renderer;
	renderer.setFramebuffer(&framebuffer);
	renderer.setCamera(&camera);
	renderer.setScene(&scene);
	
	
	clock_t t1, t2;
	cout << "Scene6: HDR Sphere with bubbles" << endl;
	cout << "Resolution: " << framebuffer.width() << "x" << framebuffer.height() << endl;
	
	
	cout << "Quincunx Sampling (scene6_Q.ppm)                 ... ";
	QuincunxPixelSampler pixelsampler1(framebuffer);
	renderer.setPixelSampler(&pixelsampler1);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPFM("output/scene6_Q.pfm", framebuffer);
	saveFramebufferToPPMExposure("output/scene6_Q.ppm", framebuffer, 1.0f);
	
	cout << endl;
}
