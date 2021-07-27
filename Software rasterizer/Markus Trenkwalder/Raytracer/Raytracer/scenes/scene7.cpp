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
			//if ( dnl > 0 && !scene.intersect(Ray(pos + 0.0001f * normal, lightvec)) )
				color += dnl * light_color;
			
			return clamp(color * material_color);
		}
	};
}

void scene7main() {
	Mesh3D *mesh = new Mesh3D(
		scale(0.1,0.1,0.1),
		new DiffuseMaterial(color3(1.0f, 1.0f, 1.0f)),
		"data/dragon.mesh"
	);
	
	Scene scene(mesh, NULL);
	Framebuffer framebuffer(640, 480);
	
	mat4 camera_transform1 =
		inverse(
	 		lookat(
				vec3(-1.2, 2.7, 2.3),
				vec3(0, 1, 0),
				vec3(0, 1, 0)
			)
		);
		
	mat4 camera_transform2 =
		inverse(
	 		lookat(
				vec3(0, 1.5, 2.3),
				vec3(0, 1, 0),
				vec3(0, 1, 0)
			)
		);
		
		mat4 camera_transform3 =
		inverse(
	 		lookat(
				vec3(-1.2, 2.7, -2),
				vec3(0, 1, 0),
				vec3(0, 1, 0)
			)
		);
		
		mat4 camera_transform4 =
		inverse(
	 		lookat(
				vec3(1.7, 1.0, 1.7),
				vec3(0, 1, 0),
				vec3(0, 1, 0)
			)
		);
		
	float fovy = 53.0f / 180.0f * M_PI;
	float aspect = static_cast<float>(framebuffer.width()) / framebuffer.height();
	
	PinholeCamera camera1(camera_transform1, fovy, aspect);
	PinholeCamera camera2(camera_transform2, fovy, aspect);
	PinholeCamera camera3(camera_transform3, fovy, aspect);
	PinholeCamera camera4(camera_transform4, fovy, aspect);
	
	//BasicPixelSampler pixelsampler1;
	Rooks8PixelSampler pixelsampler1;
	
	Renderer renderer;
	renderer.setFramebuffer(&framebuffer);
	renderer.setScene(&scene);
	renderer.setPixelSampler(&pixelsampler1);
	
	clock_t t1, t2;
	cout << "Scene7: Dragon 3D Mesh" << endl;
	cout << "Resolution: " << framebuffer.width() << "x" << framebuffer.height() << endl;
	
	cout << "1 Sample per Pixel (scene7A_B.ppm)                ... ";
	renderer.setCamera(&camera1);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene7A_B.ppm", framebuffer);
	
	cout << "1 Sample per Pixel (scene7B_B.ppm)                ... ";
	renderer.setCamera(&camera2);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene7B_B.ppm", framebuffer);
	
	cout << "1 Sample per Pixel (scene7C_B.ppm)                ... ";
	renderer.setCamera(&camera3);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene7C_B.ppm", framebuffer);
	
	cout << "1 Sample per Pixel (scene7D_B.ppm)                ... ";
	renderer.setCamera(&camera4);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene7D_B.ppm", framebuffer);
	
	cout << endl;
}
