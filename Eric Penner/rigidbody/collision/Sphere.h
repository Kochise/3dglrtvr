#pragma once

#include "Engine/Vector.h"

class CSphere
{
public:
	virtual ~CSphere(void){}
	CSphere(CVector3 Center, float fRadius) { m_Center = Center; m_fRadius = fRadius;}

	void  SetRadius(float fRadius) {m_fRadius=fRadius;} 
	float GetRadius() {return m_fRadius;}

	void     SetCenter(CVector3 Center) {m_Center = Center;}
	CVector3 GetCenter(){return m_Center;}

	bool ContainsPoint(CVector3 Point)
	{
		return (Point.LengthSq() < m_fRadius*m_fRadius);
	}	

protected:

	float    m_fRadius;
	CVector3 m_Center;

};
