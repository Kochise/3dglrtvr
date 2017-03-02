#ifndef VOLUME_RENDERER_H
#define VOLUME_RENDERER_H

#include "Volume.h"

class VolumeRenderer
{
	private:
		Volume *volume;
		GLuint texnum;
		int w, h, d;
		double rx, ry;
		double sx, sy, sz;

	public:
		VolumeRenderer(Volume *vol);
		~VolumeRenderer();

		void render(int nslices);
		void rotate(double x, double y);
//		void scale(double x, double y, double z);
};

#endif

