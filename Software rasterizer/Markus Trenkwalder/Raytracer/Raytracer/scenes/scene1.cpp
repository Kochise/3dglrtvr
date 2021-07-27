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
#include <algorithm>
#include <ctime>
#include <iostream>

using namespace math3d;
using namespace std;

namespace {
	class PlaneCheckerboardMaterialShader : public IMaterialShader {
	public:
		virtual color3 shadeRay(const Scene& scene, const Ray& ray, const IntersectionInfo& iinfo, int maxdepth) const {
			vec3 pos = ray.origin + iinfo.abscissa * ray.direction;
			
			int u = static_cast<int>(pos[0] > 0 ? pos[0] : pos[0] - 1);
			int v = static_cast<int>(pos[2] > 0 ? pos[2] : pos[2] - 1);
			
			color3 color(abs(u + v) % 2);
			
			vec3 light = normalized(vec3(1, 1, 2) - pos);
			color *= max(0.0f, dot(vec3(0, 1, 0), light)) * 0.7f;
			if ( maxdepth > 0 && color[0] != 0.0f) {
				static const vec3 n = vec3(0, 1, 0);
				Ray reflected_ray(pos + n * 0.00001f, ::reflect(normalized(ray.direction), n));
				color += scene.trace(reflected_ray, maxdepth - 1) * 0.3f;
			}
			
			return color;
		}
	};
}

void scene1main() {
	
	IMaterialShader *mtrl1 = new RefractiveFresnelMaterialShader(1.52f, color3(1, 0, 0), 0.375f);
	IMaterialShader *mtrl2 = new ReflectiveMaterialShader(color3(0.975f));
	IMaterialShader *mtrl3 = new ReflectiveMaterialShader(color3(0.975f));
	IMaterialShader *mtrl4 = new PlaneCheckerboardMaterialShader();	
	
	IObject *sphere1 = new Sphere(translate(-2.0, 1.2, 0) * scale(1.2, 1.2, 1.2) , mtrl1);
	IObject *sphere2 = new Sphere(translate(1.0, 1.0, 2.5) , mtrl2);
	IObject *sphere3  = new Sphere(translate(0.2, 1.2, -0.5), mtrl3);
	IObject *plane = new Plane(identity4(), mtrl4);
	
	CompoundObject *compoundObject = new CompoundObject(rotate(95, 0, 1, 0));
	compoundObject->addObject(sphere1);
	compoundObject->addObject(sphere2);
	compoundObject->addObject(sphere3);
	compoundObject->addObject(plane);
	
	Scene scene(compoundObject, new ConstantColorBackground(color3(1.0f, 1.0f, 1.0f)));
	
	Framebuffer framebuffer(640, 480);
	BasicPixelSampler pixelsampler1;
	
	mat4 camera_transform =
		inverse(
			lookat(
				vec3(2, 1.5, 5),
				vec3(0, 1, 0),
				vec3(0, 1, 0)
			)
		);
		
	float fovy = 60.0f / 180.0f * M_PI;
	float aspect = static_cast<float>(framebuffer.width()) / framebuffer.height();
	
	PinholeCamera camera(camera_transform, fovy, aspect);
	
	Renderer renderer;
	
	renderer.setFramebuffer(&framebuffer);
	renderer.setCamera(&camera);
	renderer.setPixelSampler(&pixelsampler1);
	renderer.setScene(&scene);
	
	
	clock_t t1, t2;
	cout << "Scene1: Reflections and Refractions" << endl;
	cout << "Resolution: " << framebuffer.width() << "x" << framebuffer.height() << endl;
	
	cout << "1 Sample per Pixel (scene1_B.ppm)                ... ";
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene1_B.ppm", framebuffer);
	
	cout << "Quincunx Sampling (scene1_Q.ppm)                 ... ";
	QuincunxPixelSampler pixelsampler2(framebuffer);
	renderer.setPixelSampler(&pixelsampler2);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene1_Q.ppm", framebuffer);

	cout << "2x2 Rotated Grid Supersampling (scene1_RGSS.ppm) ... ";
	RGSS2x2PixelSampler pixelsampler3;
	renderer.setPixelSampler(&pixelsampler3);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene1_RGSS.ppm", framebuffer);

	cout << "Adaptive Supersampling d = 1 (scene1_ASS.ppm)    ... ";
	AdaptiveSSPixelSampler pixelsampler4(framebuffer, 0.1f, 1);
	renderer.setPixelSampler(&pixelsampler4);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene1_ASS.ppm", framebuffer);

	cout << "8 Rooks Pattern (scene1_R8.ppm)                  ... ";
	Rooks8PixelSampler pixelsampler5;
	renderer.setPixelSampler(&pixelsampler5);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene1_R8.ppm", framebuffer);

	cout << "4x4 grid (scene1_G4x4.ppm)                       ... ";
	Grid4x4PixelSampler pixelsampler6;
	renderer.setPixelSampler(&pixelsampler6);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene1_G4x4.ppm", framebuffer);

	cout << "4x4 jittered grid (scene1_JG4x4.ppm)             ... ";
	JitteredGridPixelSampler pixelsampler7(4);
	renderer.setPixelSampler(&pixelsampler7);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene1_JG4x4.ppm", framebuffer);
	
	cout << endl;
}
