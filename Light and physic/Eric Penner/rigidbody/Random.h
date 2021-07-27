#pragma once

class CRandom
{
public:
	CRandom(void){}
	virtual ~CRandom(void){}

	static float RandomFloat(float fStart,float fEnd);
};	
