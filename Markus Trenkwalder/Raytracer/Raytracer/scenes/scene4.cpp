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
	
	class ConstantColorMaterialShader : public IMaterialShader {
		color3 color;
	public:
		ConstantColorMaterialShader(const color3& c) :
			color(c)
		{
		}
		
		virtual color3 shadeRay(const Scene&, const Ray&, const IntersectionInfo&, int) const {
			return color;
		}
	};
}

void scene4main() {
	IObject *triangle = new Triangle(
		vec3(-1, 1, 0),
		vec3(-3, 0, 0),
		vec3(3, -0.25, 0),
		new ConstantColorMaterialShader(color3(0.0f))
	);
	
	Scene scene(triangle, new ConstantColorBackground(color3(1.0f)));
	
	Framebuffer framebuffer(32, 24);
	
	
	mat4 camera_transform =
		inverse(
			lookat(
				vec3(0, 0, 5),
				vec3(0, 0, 0),
				vec3(0, 1, 0)
			)
		);
		
	float fovy = 50.0f / 180.0f * M_PI;
	float aspect = static_cast<float>(framebuffer.width()) / framebuffer.height();
	
	PinholeCamera camera(camera_transform, fovy, aspect);
	
	Renderer renderer;
	
	renderer.setFramebuffer(&framebuffer);
	renderer.setCamera(&camera);
	renderer.setScene(&scene);
	
	clock_t t1, t2;
	cout << "Scene4: Antialiasing a triangle" << endl;
	cout << "Resolution: " << framebuffer.width() << "x" << framebuffer.height() << endl;
	
	cout << "1 Sample per Pixel (scene4_B.ppm)                ... ";
	BasicPixelSampler pixelsampler1;
	renderer.setPixelSampler(&pixelsampler1);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene4_B.ppm", framebuffer);
	
	cout << "Quincunx Sampling (scene4_Q.ppm)                 ... ";
	QuincunxPixelSampler pixelsampler2(framebuffer);
	renderer.setPixelSampler(&pixelsampler2);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene4_Q.ppm", framebuffer);
	
	cout << "2x2 Rotated Grid Supersampling (scene4_RGSS.ppm) ... ";
	RGSS2x2PixelSampler pixelsampler3;
	renderer.setPixelSampler(&pixelsampler2);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene4_RGSS.ppm", framebuffer);
	
	cout << "8 Rooks Pattern (scene4_R8.ppm)                  ... ";
	Rooks8PixelSampler pixelsampler4;
	renderer.setPixelSampler(&pixelsampler4);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene4_R8.ppm", framebuffer);
	
	cout << "4x4 grid (scene4_G4x4.ppm)                       ... ";
	Grid4x4PixelSampler pixelsampler5;
	renderer.setPixelSampler(&pixelsampler5);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene4_G4x4.ppm", framebuffer);
	
	cout << "4x4 jittered grid (scene4_JG4x4.ppm)             ... ";
	JitteredGridPixelSampler pixelsampler6(4);
	renderer.setPixelSampler(&pixelsampler6);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPPM("output/scene4_JG4x4.ppm", framebuffer);
	
	cout << endl;
}
