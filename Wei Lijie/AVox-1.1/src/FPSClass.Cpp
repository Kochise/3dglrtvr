#include "FPSClass.H"

FPS::FPS()
{
	TotalTime = 0;
	TotalFps = 0;
	RefreshTime = 1.0f;							//
	FrameRate = 0;
	FrameCount = 0;
	Log.Write("Black","Default Timer initialization: Complete");
}

FPS::FPS(float Interval)
{
	TotalTime = 0;
	TotalFps = 0;
	RefreshTime = Interval;
	FrameRate = 0;
	FrameCount = 0;
	Log.Write("Black","Custom Timer initialization: Complete");
}

FPS::~FPS()
{
	Log.Write("Black","Remove Timer: Successfully");
}

GLfloat FPS::DoTimerStuff(void)
{
	static float Timer = 0.0f;
	static DWORD PrevTime = timeGetTime();

	FrameCount++;										// 
	DWORD NowTime = timeGetTime();						// 
	Elapsed = (float)(NowTime - PrevTime)/1000.0f;		// 
	Timer+=Elapsed;										// 

	if (Timer>=RefreshTime)								// 
	{
		FrameRate = ((float)FrameCount)/Timer;
		FrameCount = 0;		
		Timer = 0.0f;
	}
	
	PrevTime = NowTime;
	return FrameRate;
}