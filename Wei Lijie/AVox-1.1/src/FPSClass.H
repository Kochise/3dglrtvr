#ifndef __Fps_
#define __Fps_

#include "Log.H"
#include <Windows.H>
#include <StdIo.H>
#include "GL\GL.H"


class FPS
{
private:
	float FrameRate, RefreshTime;
	DWORD FrameCount;
	float Elapsed;								// 

public:
	unsigned int TotalTime;
	long double TotalFps;


	FPS();										// 
	FPS(float Interval);

	~FPS();
 
	GLfloat DoTimerStuff();						// 
};

#endif