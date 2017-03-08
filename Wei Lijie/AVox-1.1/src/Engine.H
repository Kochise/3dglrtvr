#ifndef __3DEngine_
#define __3DEngine_

#include "Includes.H"
#include "DXControl.H"
#include "Font.H"
#include "FPSClass.H"

#include "Landscape.H"

class VoxelEngine : public Voxel
{
protected:
	int Speed;
public:
	FPS Perfomance;
	Font Cout;

	DirectInput *DI8;

	VoxelEngine();
	~VoxelEngine();

	void Controll();
	void RefilterTexture();
	void Render();
};



#endif