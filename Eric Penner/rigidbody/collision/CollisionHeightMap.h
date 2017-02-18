#pragma once

#include "CollisionModel.h"
#include "../HeightMap.h"
#include "../Pointers.h"

class CCollisionHeightMap : public CCollisionModel
{
	friend class CCollisionDetector;

public:
	virtual ~CCollisionHeightMap(void){}
	CCollisionHeightMap(CPositionPtr pPosition,CHeightMapPtr pHeightMap)
	{
		SetPosition(pPosition);
		SetRestitution(0.0f);
		SetFriction(0);
		m_pHeightMap = pHeightMap;
	}

protected:

	CHeightMapPtr m_pHeightMap;
};

