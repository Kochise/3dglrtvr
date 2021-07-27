#pragma once

#include "Sphere.h"
#include "CollisionModel.h"

class CCollisionSphere : public CCollisionModel
{
public:
	virtual ~CCollisionSphere(void){}
	CCollisionSphere(CPositionPtr pPosition,float fMass,float fRadius)
	{
		SetMass(fMass);
		SetRadius(fRadius);
		SetPosition(pPosition);
		
		CMatrix tensor = CMatrix::Identity();
		tensor._11 = (float)(2.0/5.0 * fMass * fRadius * fRadius);
		tensor._22 = (float)(2.0/5.0 * fMass * fRadius * fRadius);
		tensor._33 = (float)(2.0/5.0 * fMass * fRadius * fRadius);
		SetInertiaTensor(tensor);
	}
	
	CSphere GetBoundingSphere()
	{
		return CSphere(m_pPosition->GetCenter(),m_fRadius);
	}

	void  SetRadius(float fRadius) { m_fRadius = fRadius;}
	float GetRadius()              { return m_fRadius; }
	

protected:

	float m_fRadius;

};

typedef SmartPtr<CCollisionSphere> CCollisionSpherePtr;
