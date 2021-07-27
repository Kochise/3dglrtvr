#include "entity.h"

#include "Engine.h"

CEntity::CEntity(void)
	:m_pPosition(new CPosition())
	,m_pMotion(new CMotion())
	,m_pRenderable(new CRenderable())
	,m_pAnimatable(new CAnimatable())
	,m_pCollisionModel(new CCollisionModel())
	,m_Forces()
{
}

CEntity::~CEntity(void)
{
}
