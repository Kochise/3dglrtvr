#pragma once

#include "Sphere.h"
#include "CollisionModel.h"

class CCollisionPlane : public CCollisionModel
{
public:
	virtual ~CCollisionPlane(void){}
	CCollisionPlane(CPositionPtr pPosition,CVector3 Normal)
	{
		SetPosition(pPosition);

		m_Normal = Normal;
		m_fDistance = -CVector3::Dot(Normal,pPosition->GetCenter());
	}

	float DistanceToPoint(CVector3 Point)
	{
		return CVector3::Dot(m_Normal,Point) + m_fDistance;
	}

	CVector3 GetNormal() {return m_Normal;}

protected:

	float    m_fDistance;
	CVector3 m_Normal;
};

typedef SmartPtr<CCollisionPlane> CCollisionPlanePtr;
