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

#include "scenecommon.h"

#include <cmath>
#include <ctime>
#include <iostream>

using namespace std;
using namespace math3d;

namespace {
	
	vec3 lightpos(20, 60, 40);
	color3 light_color(0.8f, 0.8f, 0.8f);
	color3 ambient(0.15f, 0.15f, 0.15f);
	
	inline color3 clamp(const color3& value) {
		return color3(
			max(0.0f, min(1.0f, value[0])),
			max(0.0f, min(1.0f, value[1])),
			max(0.0f, min(1.0f, value[2]))
		);
	}
	
	inline float clamp(float value) {
		return max(0.0f, min(1.0f, value));
	}
	
	class DiffuseMaterial : public IMaterialShader {
		color3 material_color;
	public:
		DiffuseMaterial(const color3& c) : material_color(c) {}
		
		virtual color3 shadeRay(const Scene& scene, const Ray& ray, const IntersectionInfo& iinfo, int) const {
			vec3 pos = ray.origin + iinfo.abscissa * ray.direction;
			vec3 normal = normalized(iinfo.object->transformNormalToWorld(iinfo.normal));
			
			vec3 lightvec = normalized(lightpos - pos);
			
			color3 color = ambient;
			const float dnl = dot(normal, lightvec);
			if ( dnl > 0 && !scene.intersect(Ray(pos + 0.0001f * normal, lightvec)) )
				color += dnl * light_color;
			
			return clamp(color * material_color);
		}
	};
	
	class PlaneCheckerboard : public IMaterialShader {
	public:
		virtual color3 shadeRay(const Scene& scene, const Ray& ray, const IntersectionInfo& iinfo, int /*maxdepth*/) const {
			vec3 pos = ray.origin + iinfo.abscissa * ray.direction;
			vec3 normal = normalized(iinfo.object->transformNormalToWorld(iinfo.normal));
			
			vec3 lpos = iinfo.object->transformPointToLocal(pos) / 0.75f;
			
			int u = static_cast<int>(lpos[0] > 0 ? lpos[0] : lpos[0] - 1);
			int v = static_cast<int>(lpos[2] > 0 ? lpos[2] : lpos[2] - 1);
			
			int tmp = abs(u + v) % 2;
			color3 color = color3(1) * tmp + (1 - tmp) * color3(0.2, 0.2, 0.7);
			
			vec3 lightvec = normalized(lightpos - pos);
			
			color3 lc = ambient;
			if ( !scene.intersect(Ray(pos + 0.0001f * normal, lightvec)) )
				lc += light_color * max(0.0f, dot(lightvec, normal) / (0.00013f * length2(lightpos - pos)) );
			
			return clamp(lc * color);
		}
	};
}

void scene8main() {
	Mesh3D *mesh = new Mesh3D(
		scale(0.1,0.1,0.1),
		new DiffuseMaterial(color3(1.0f, 1.0f, 1.0f)),
		"data/dragon.mesh"
	);
	
	IObject *plane = new Plane(translate(0.0, 0.5273, 0.0), new PlaneCheckerboard);
	
	CompoundObject *compoundObject = new CompoundObject(identity4());
	compoundObject->addObject(mesh);
	compoundObject->addObject(plane);
	
	Scene scene(compoundObject, new ConstantColorBackground(color3(1.0f, 1.0f, 1.0f)));
	Framebuffer framebuffer(640, 480);
	
	mat4 camera_transform =
		inverse(
	 		lookat(
				vec3(-1.2, 2.7, 2.3),
				vec3(0, 1, 0),
				vec3(0, 1, 0)
			)
		);
		
	float fovy = 53.0f / 180.0f * M_PI;
	float aspect = static_cast<float>(framebuffer.width()) / framebuffer.height();
	
	PinholeCamera camera(camera_transform, fovy, aspect);
	
	Renderer renderer;
	renderer.setFramebuffer(&framebuffer);
	renderer.setCamera(&camera);
	renderer.setScene(&scene);
	
	clock_t t1, t2;
	cout << "Scene8: Dragon 3D Mesh" << endl;
	cout << "Resolution: " << framebuffer.width() << "x" << framebuffer.height() << endl;
	
	cout << "1 Sample per Pixel (scene8_B.ppm)                ... ";
	BasicPixelSampler pixelsampler1;
	renderer.setPixelSampler(&pixelsampler1);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene8_B.ppm", framebuffer);
	
	cout << "Quincunx Sampling (scene8_Q.ppm)                 ... ";
	QuincunxPixelSampler pixelsampler2(framebuffer);
	renderer.setPixelSampler(&pixelsampler2);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene8_Q.ppm", framebuffer);
	
	cout << "Adaptive Supersampling d = 1 (scene8_ASS.ppm)    ... ";
	AdaptiveSSPixelSampler pixelsampler3(framebuffer, 0.1f, 1);
	renderer.setPixelSampler(&pixelsampler3);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene8_ASS.ppm", framebuffer);
	
	cout << endl;
}
