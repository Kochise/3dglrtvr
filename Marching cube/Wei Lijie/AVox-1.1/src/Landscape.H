#ifndef __LANDSCAPE_H
#define __LANDSCAPE_H

#include <Windows.H>
#include <Math.H>
#include "Font.H"
#include "FPSClass.H"
#include "HeightField.H"

#define PI 3.141592654 
#define PI_2K 6283

const GLuint MaxVoxels = 1024;


class Voxel: public HeightField
{
protected:

	int Width, Height;

protected:
	// Number of pixel to skip
    float sin_t[6284];
    float cos_t[6284];

	int hf_s, hf_a, t_zo;
	long d_s , h_h;

	GLubyte PixelSkip;
	// Max view distance 
	GLuint EyeShot;	
	// Observer position
	float Xo,Yo,Zo;
	// Horizontal angle and field of view angle
	float HorizAngle, FieldView;

	// Store previous voxel heights
	int Horizon[MaxVoxels];
	// Previous X, Y values (begin and end of each scanned line)
	int LastXo[MapSize], LastYo[MapSize];

	int zsVox;
	
	float SLOPE;	
// Variables for debug purposes
	long PolygonsInUse;
	bool Wireframe;
	long ViewScale, HeightPrjScale;
	bool FreeFly;
public:
	Tga Image;
	GLuint mTexture;

	Font Cout;
	FPS Speed;
	
	Voxel();
	~Voxel();

	void Init(int width , int height);
	void RayCasting();
	void vLine(int, int, int, int, int, int, int);
};



#endif