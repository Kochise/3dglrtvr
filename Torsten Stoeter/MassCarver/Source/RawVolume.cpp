#include <iostream>
#include <fstream>

#include "RawVolume.h"

using namespace std;


RawVolume::RawVolume(const char *filename)
{
	ifstream file(filename, ios::in | ios::binary | ios::ate);

	if (file.is_open())
	{
		size = file.tellg();
		data = new char[size];
		file.seekg(0, ios::beg);
		file.read(data, size);
		file.close();
	}
	else
		throw 20;
}

RawVolume::~RawVolume()
{
	delete data;
}

char *RawVolume::getDataPointer()
{
	return data;
}

char RawVolume::getVoxel(int i)
{
	if (i <= 0 || i > size)
		return -1;

	return data[i];
}

