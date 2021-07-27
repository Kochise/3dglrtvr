#pragma once

#include "Engine/Vector.h"

class CPlane
{
public:
	virtual ~CPlane(void){}
	CPlane(CVector3 Position,CVector3 Normal)
	{
		m_Normal = Normal;
		m_fDistance = -CVector3::Dot(Normal,Position);
	}
	CPlane(float fDistance,CVector3 Normal)
	{
		m_Normal = Normal;
		m_fDistance = fDistance;
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