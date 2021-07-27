#pragma once

#include "../smartptr.h"
#include "Vector.h"

class CMotion
{
public:
	CMotion(void);
	virtual ~CMotion(void);

	CVector3& GetVelocity()                               {return m_Velocity;}
	void      SetVelocity(CVector3 Velocity)              {m_Velocity = Velocity;}

	//We can't return a reference for angular velocity as setting it needs to go through
	//and set up the axis/rotation rate
	CVector3  GetAngularVelocity()                        {return m_AngularVelocity;}
	void      SetAngularVelocity(CVector3 Axis,float fRate)
	{
		m_RotationAxis = Axis;
		m_RotationRate = fRate;
		m_AngularVelocity = Axis * fRate;
	}
	void     SetAngularVelocity(CVector3 AngularVelocity)
	{
		m_AngularVelocity = AngularVelocity;
		m_RotationRate = AngularVelocity.Length();
		if(m_RotationRate == 0)
			m_RotationAxis = CVector3(0,0,0);
		else
			m_RotationAxis = m_AngularVelocity * (1/m_RotationRate);
	}

	float     GetRotationRate() {return m_RotationRate;}
	CVector3  GetRotationAxis() {return m_RotationAxis;}


protected:

	CVector3 m_Velocity;
	CVector3 m_AngularVelocity;

	CVector3 m_RotationAxis;
	float    m_RotationRate;
};

typedef SmartPtr<CMotion> CMotionPtr;