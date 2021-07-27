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
	
	vec3 lightpos(9, 9, 9);
	color3 ambient(0.0f, 0.0f, 0.0f);
	
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
	
	class PlaneCheckerboardBumpedReflective : public IMaterialShader {
	public:
		virtual color3 shadeRay(const Scene& scene, const Ray& ray, const IntersectionInfo& iinfo, int maxdepth) const {
			vec3 pos = ray.origin + iinfo.abscissa * ray.direction;
			vec3 normal = normalized(iinfo.object->transformNormalToWorld(iinfo.normal));
			
			vec3 lpos = iinfo.object->transformPointToLocal(pos) / 1.0f;
			
			int u = static_cast<int>(lpos[0] > 0 ? lpos[0] : lpos[0] - 1);
			int v = static_cast<int>(lpos[2] > 0 ? lpos[2] : lpos[2] - 1);
			
			int tmp = abs(u + v) % 2;
			color3 color = color3(tmp);
			
			float fu = lpos[0] - floor(lpos[0]);
			float fv = lpos[2] - floor(lpos[2]);
			
			vec3 shadingnormal = normal; 
			
			if ( fv > 0.975f && (fv > fu && fv > (1.0f - fu)) )
				shadingnormal = normalized(iinfo.object->transformNormalToWorld(vec3(0, 1, 1)));
			else if ( fv < 0.025f && (fv < fu && fv < (1.0f - fu)) )
				shadingnormal = normalized(iinfo.object->transformNormalToWorld(vec3(0, 1, -1)));
			else if ( fu > 0.975f && (fu > fv && fu > (1.0f - fv)) )
				shadingnormal = normalized(iinfo.object->transformNormalToWorld(vec3(1, 1, 0)));
			else if ( fu < 0.025f && (fu < fv && fu < (1.0f - fv)) )
				shadingnormal = normalized(iinfo.object->transformNormalToWorld(vec3(-1, 1, 0)));
			
			vec3 lightvec = normalized(lightpos - pos);
			
			color3 lightcolor = ambient;
			if ( !scene.intersect(Ray(pos + 0.0001f * normal, lightvec)) )
				lightcolor += max(0.0f, dot(lightvec, shadingnormal) / (0.007f * length2(lightpos - pos)));
			
			color3 reflectivecolor;
			if ( maxdepth > 0 && color[0] != 0.0f ) {
				reflectivecolor = scene.trace(Ray(pos + 0.0001f * normal, reflect(ray.direction, shadingnormal)), maxdepth - 1);
			}
			
			return clamp((lightcolor * color) * 0.85f + reflectivecolor * 0.15f);
		}
	};
}

void scene3main() {
	mat4 transform;

	transform =
		translate(0, 0, 0) *
		scale(2, 2, 2) *
		translate(0, 1, 0);
	IObject *sphere1 = new Sphere(transform, new ReflectiveMaterialShader(color3(1.0f, 1.0f, 0.2f)));
	
	transform =
		translate(0, 0, -4) *
		scale(1.5, 1.5, 1.5) * 
		translate(0, 1, 0);
	IObject *sphere2 = new Sphere(transform, new ReflectiveMaterialShader(color3(1.0f, 0.2f, 0.2f)));
	
	transform =
		rotate(120, 0, 1, 0)*
		translate(0, 0, -4) *
		scale(1.5, 1.5, 1.5) *
		translate(0, 1, 0);
	IObject *sphere3 = new Sphere(transform, new ReflectiveMaterialShader(color3(0.2f, 1.0f, 0.2f)));
	
	transform = 
		rotate(-120, 0, 1, 0) *
		translate(0, 0, -4) *
		scale(1.5, 1.5, 1.5) *
		translate(0, 1, 0);
	IObject *sphere4 = new Sphere(transform, new ReflectiveMaterialShader(color3(0.2f, 0.2f, 1.0f)));
	
	IObject *plane = new Plane(identity4(), new PlaneCheckerboardBumpedReflective);
	
	
	CompoundObject *compoundObject = new CompoundObject(identity4());
	compoundObject->addObject(sphere1);
	compoundObject->addObject(sphere2);
	compoundObject->addObject(sphere3);
	compoundObject->addObject(sphere4);
	compoundObject->addObject(plane);
	
	Scene scene(compoundObject, new ConstantColorBackground(color3(1.0f, 1.0f, 0.97f)));
	
	Framebuffer framebuffer(640, 480);
	
	mat4 camera_transform =
		inverse(
			lookat(
				vec3(5, 6.5, 8),
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
	cout << "Scene3: Colored reflections, bumpmapping and shadows" << endl;
	cout << "Resolution: " << framebuffer.width() << "x" << framebuffer.height() << endl;
	
	cout << "1 Sample per Pixel (scene3_B.ppm)                ... ";
	BasicPixelSampler pixelsampler1;
	renderer.setPixelSampler(&pixelsampler1);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene3_B.ppm", framebuffer);
	
	cout << "Adaptive Supersampling d = 1 (scene3_ASS.ppm)    ... ";
	AdaptiveSSPixelSampler pixelsampler2(framebuffer, 1.0f / 255.0f, 1);
	renderer.setPixelSampler(&pixelsampler2);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene3_ASS.ppm", framebuffer);
	
	cout << endl;
}
