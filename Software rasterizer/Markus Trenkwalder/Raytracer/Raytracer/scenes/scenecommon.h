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

#include "core/basis.h"
#include "core/color3.h"
#include "core/framebuffer.h"
#include "core/ibackgroundshader.h"
#include "core/icamera.h"
#include "core/imaterialshader.h"
#include "core/intersectioninfo.h"
#include "core/iobject.h"
#include "core/ipixelsampler.h"
#include "core/ray.h"
#include "core/renderer.h"
#include "core/scene.h"

#include "objects/box.h"
#include "objects/compoundobject.h"
#include "objects/cylinder.h"
#include "objects/plane.h"
#include "objects/sphere.h"
#include "objects/triangle.h"
#include "objects/mesh3d.h"

#include "pixelsampler/adaptivesspixelsampler.h"
#include "pixelsampler/basicpixelsampler.h"
#include "pixelsampler/grid4x4pixelsampler.h"
#include "pixelsampler/quincunxpixelsampler.h"
#include "pixelsampler/rgss2x2pixelsampler.h"
#include "pixelsampler/rooks8pixelsampler.h"
#include "pixelsampler/jitteredgridpixelsampler.h"

#include "pixelselectors/everynthpixelselector.h"
#include "pixelselectors/stripepixelselector.h"

#include "cameras/pinholecamera.h"

#include "shaders.h"
#include "imageio.h"
