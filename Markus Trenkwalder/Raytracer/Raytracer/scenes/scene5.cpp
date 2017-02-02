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

using namespace math3d;
using namespace std;

namespace {
	
}

void scene5main() {
	mat4 transform =	translate(1.5, 0, 0);
	
	IObject *sphere1 = new Sphere(transform, new RefractiveFresnelMaterialShader(1.52f, color3(1), 0.375f));
	
	transform = translate(-1.5, 0, 0);
	
	IObject *sphere2 = new Sphere(transform, new ReflectiveMaterialShader(color3(0.8)));
	
	CompoundObject *compound = new CompoundObject(identity4());
	compound->addObject(sphere1);
	compound->addObject(sphere2);
	
	IBackgroundShader *hdr_cubemap = new HDRCubemapBackgroundShader("data/rnl_cross.pfm");
	
	Scene scene(compound, hdr_cubemap);
	Framebuffer framebuffer(640, 480);
	
	mat4 camera_transform =
		inverse(
			lookat(
				vec3(2.5, 0, -4),
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
	cout << "Scene5: HDR Rendering" << endl;
	cout << "Resolution: " << framebuffer.width() << "x" << framebuffer.height() << endl;
	
	
	cout << "Quincunx Sampling (scene5_Q.ppm)                 ... ";
	QuincunxPixelSampler pixelsampler1(framebuffer);
	renderer.setPixelSampler(&pixelsampler1);
	t1 = clock();
	renderer.renderScene();
	t2 = clock();
	cout << (static_cast<float>(t2 - t1) / CLOCKS_PER_SEC) << " seconds"<< endl;
	saveFramebufferToPFM("output/scene5_Q.pfm", framebuffer);
	saveFramebufferToPPMExposure("output/scene5_Q.ppm", framebuffer, 1.0f);
	
	cout << endl;
}
