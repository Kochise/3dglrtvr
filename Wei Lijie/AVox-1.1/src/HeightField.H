#ifndef __HEIGHTFIELD_H
#define __HEIGHTFIELD_H

#include <Windows.H>
#include "Log.H"
#include "Utils.H"

const unsigned int MapSize = 1024;
const unsigned char MAP_SHIFT= 10;

class HeightField
{
private:
	
protected:
	unsigned char *HMap;
	
public:

	HeightField();
	~HeightField();

	bool LoadMap(const char*, unsigned char*);
};

#endif

