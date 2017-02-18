#pragma once

#include "Engine/Vector.h"
#include "Engine/Matrix.h"
#include "Engine/Position.h"

class CCollisionModel
{
public:
	const static float INFINITE_MASS;

	virtual ~CCollisionModel(void);
	CCollisionModel(void)
	{
		m_fRestitution = 1.0f;
		m_fFriction    = 0.0f;

		m_fMass                = 0;    
		m_fOneOverMass         = 0;
		m_InertiaTensor        = CMatrix::Zero();
		m_InertiaTensorInverse = CMatrix::Zero();
		m_pPosition            = new CPosition();
	}

protected:

	float        m_fRestitution;
	float        m_fFriction;

	float        m_fMass;
	float        m_fOneOverMass;
	CMatrix      m_InertiaTensor;
	CMatrix      m_InertiaTensorInverse;
	CPositionPtr m_pPosition;

public:

	void  SetRestitution(float fRestutution) { m_fRestitution = fRestutution;}
	float GetRestitution()                   { return m_fRestitution; }

	void  SetFriction(float fFriction) { m_fFriction = fFriction;}
	float GetFriction()                { return m_fFriction; }

	void          SetPosition(CPositionPtr& pPosition) { m_pPosition = pPosition;}
	CPositionPtr& GetPosition()                        { return m_pPosition;}

	void  SetMass(float fMass) { m_fMass = fMass; m_fOneOverMass = 1/fMass;}
	float GetMass()            { return m_fMass; }
	float GetOneOverMass()     { return m_fOneOverMass; }

	void    SetInertiaTensor(CMatrix InertiaTensor) { m_InertiaTensor = InertiaTensor; m_InertiaTensorInverse = CMatrix::Inverse(InertiaTensor);}
	CMatrix GetInertiaTensor()                      { return m_InertiaTensor; }
	CMatrix GetInertiaTensorInverse()               { return m_InertiaTensorInverse; }
	CMatrix GetRelativeInertiaTensorInverse()       { return m_pPosition->GetOrientation() *  m_InertiaTensorInverse * m_pPosition->GetOrientationInverse();}
};

typedef SmartPtr<CCollisionModel> CCollisionModelPtr;