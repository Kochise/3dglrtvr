#ifndef ADAPTIVESSPIXELSAMPLER_H
#define ADAPTIVESSPIXELSAMPLER_H

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

#include "../core/ipixelsampler.h"

#include <vector>

class Framebuffer;

//! \brief This pixelsampler implements an adaptive supersampling sceme
//! which uses more samples if necessary.
//!
//! This pixelsampler is based on the QuincunxPixelSampler and if the recursion
//! depth equals 0 it produces exactly the same results. If the recursion depth
//! is greater than zero the algorithm uses the virtual method similarEnough() to
//! determine if more samples are needed.
//! If one wants to define its own algorithm for determining if two samples are
//! similar enough one can derive from this class and reimplement the
//! similarEnough() method.
class AdaptiveSSPixelSampler : public IPixelSampler {
public:
	//! This creates a new adaptive supersampling pixelsampler. The Framebuffer is
	//! used to determine the size of the internal cache of samplesa and must be
	//! the same size as the one used for rendering. The \a threshold parameter is
	//! used by the similarEnough() method to determine if two samples are similar
	//! enough and no recursion is needed.
	//! 
	//! \param framebuffer The Framebuffer object which will be rendered to
	//! \param _threshold  The threshold parameter used to determine if samples are similar enough
	//! \param depth       Specifies the maximum recursion depth
	explicit AdaptiveSSPixelSampler(const Framebuffer& framebuffer, float _threshold, unsigned depth = 1);
	
	virtual color3 samplePixel(const Renderer* renderer, int x, int y);	

protected:
	//! \brief Determines if two the colors of two samples are similar enough and therefore no
	//! recursion is needed.
	//!
	//! The default implementation compares the maximum difference of the colors to 
	//! the threshold value to determine the return value. If you need a different way
	//! to compare colors you can redefine this method in derived class.
	virtual bool similarEnough(const color3& c1, const color3& c2);	
	
private:
	color3 adaptiveSampling(const color3& corner, const color3& center, int cornerToSample, const math3d::vec2& min, const math3d::vec2& max, unsigned maxdepth);	
	color3 getCachedSample(int x, int y);
	bool isCached(int x, int y);
	void cacheSample(int x, int y, color3 c);

protected:
	//! \brief The threshold value used to compare two samples together.
	//!
	//! It is protected so that a derived class can use it in a overriden
	//! similarEnough() method.
	float threshold;
	
private:
	std::vector<color3> sampleCache;
	int width;
	unsigned maxdepth;
	
	const Renderer *renderer;
};

#endif
