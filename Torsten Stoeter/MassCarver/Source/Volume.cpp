#include <iostream>
#include <fstream>

#ifdef WIN32
	#include <direct.h>
	char dirsep = '\\';
	int (*chdir_fptr)(const char *) = &_chdir;
#else
	#include <unistd.h>
	char dirsep = '/';
	int (*chdir_fptr)(const char *) = &chdir;
#endif

#include "Volume.h"

using namespace std;


Volume::Volume(const char *filename)
{
	ifstream file(filename, ios::in);

	if (file.is_open())
	{
		file.getline(descr, 256);
		file.getline(rawfile, 256);

		char dir[256];
		strcpy(dir, filename);

		for (int i=strlen(dir); i>=0; i--)
		{
			if (dir[i] == dirsep)
				break;

			dir[i] = 0;
		}

		chdir_fptr(dir);

		rawvol = new RawVolume(rawfile);

		file >> skipws;
		file >> width;
		file >> height;
		file >> depth;

		file >> scale_x;
		file >> scale_y;
		file >> scale_z;

		scale_x = 1/scale_x;
		scale_y = 1/scale_y;
		scale_z = 1/scale_z;

		otf = ctf = NULL;
		int n = 0;
		double v, r, g, b, a;

		file >> n;

		if (n > 0)
		{
			otf = new ColorScale(n);

			for (int i=0; i<n; i++)
			{
				file >> v;
				file >> a;

				otf->AddPoint(v, 0, 0, 0, a);
			}
		}

		n = 0;
		file >> n;

		if (n > 0)
		{
			ctf = new ColorScale(n);

			for (int i=0; i<n; i++)
			{
				file >> v;
				file >> r;
				file >> g;
				file >> b;

				ctf->AddPoint(v, r, g, b, 0);
			}
		}
		
		file.close();
	}
	else
		throw 10;
}

Volume::~Volume()
{
	delete rawvol;
}

void Volume::getDimensions(int &w, int &h, int &d)
{
	w = width;
	h = height;
	d = depth;
}

void Volume::getScale(double &sx, double &sy, double &sz)
{
	sx = scale_x;
	sy = scale_y;
	sz = scale_z;
}

char Volume::getVoxel(int i)
{
	return rawvol->getVoxel(i);
}

ColorScale *Volume::getOTF()
{
	return otf;
}

ColorScale *Volume::getCTF()
{
	return ctf;
}

char *Volume::getDescription()
{
	return descr;
}

