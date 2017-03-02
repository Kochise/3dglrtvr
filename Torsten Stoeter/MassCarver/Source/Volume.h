#ifndef VOLUME_H
#define VOLUME_H

#include "RawVolume.h"
#include "ColorScale.h"

class Volume
{
	private:
		RawVolume *rawvol;
		char descr[256], rawfile[256];
		int width, height, depth;
		double scale_x, scale_y, scale_z;

		ColorScale *otf, *ctf;

	public:
		Volume(const char *filename);
		~Volume();

		void getDimensions(int &w, int &h, int &d);
		void getScale(double &sx, double &sy, double &sz);
		//char getVoxel(int x, int y, int z);
		char getVoxel(int i);
		ColorScale *getCTF();
		ColorScale *getOTF();
		char *getDescription();
};

#endif

