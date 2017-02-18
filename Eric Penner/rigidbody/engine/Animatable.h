#pragma once

class CAnimatable
{
public:
	CAnimatable(void);
	virtual ~CAnimatable(void);

	virtual void Animate(float fSeconds){fSeconds;}
};
