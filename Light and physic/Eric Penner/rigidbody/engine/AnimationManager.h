#pragma once

#include "../Pointers.h"

class CAnimationManager
{
public:
	CAnimationManager(void);
	virtual ~CAnimationManager(void);

	void AnimateEntities(CEntityPtrVector& m_Entities,float fSeconds);
};
