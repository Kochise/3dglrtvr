#include "animationmanager.h"
#include "Engine.h"

CAnimationManager::CAnimationManager(void)
{
}

CAnimationManager::~CAnimationManager(void)
{
}

void CAnimationManager::AnimateEntities(CEntityPtrVector& m_Entities,float fSeconds)
{
	for(unsigned int i=0; i<m_Entities.size(); i++)
	{
		m_Entities[i]->GetAnimatable()->Animate(fSeconds);
	}
}
