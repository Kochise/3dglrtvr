#include "Timer.hpp"

#include <windows.h>

namespace swShader
{
	Timer::Timer()
	{
	}

	Timer::~Timer()
	{
	}

	double Timer::seconds()
	{
		__int64 currentTime;
		__int64 frequency;

		QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

		return (double)currentTime / (double)frequency;
	}
}