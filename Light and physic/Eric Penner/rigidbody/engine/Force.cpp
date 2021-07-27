#include "Engine.h"


CForce CDirectionalForce::GetForce(CEntityPtr& pEntity)
{
	return CForce(m_Direction * m_fAcceleration * pEntity->GetCollisionModel()->GetMass(),CVector3(0,0,0));
}


CForce CGravitationalForce::GetForce(CEntityPtr& pEntity)
{
	if(pEntity.m_pObject == m_pEntity.m_pObject)   //It doesn't attract itself
		return CForce(CVector3(0,0,0),CVector3(0,0,0));

	//F = G m1 m2 / r2

	float fMass1    = m_pEntity->GetCollisionModel()->GetMass();
	float fMass2    =   pEntity->GetCollisionModel()->GetMass();

	CVector3 ForceDirection = m_pEntity->GetPosition()->GetCenter() - pEntity->GetPosition()->GetCenter();
	float fDistanceSq = ForceDirection.LengthSq();

	if(fDistanceSq < 0.01f) fDistanceSq = 0.01f;  //If objects get too close this would get unstable

	ForceDirection = CVector3::Normalize(ForceDirection);

	return CForce(ForceDirection * (m_fGravitationalConstant * fMass1 * fMass2 / fDistanceSq),CVector3(0,0,0));
}


CForce CGroundedSpringForce::GetForce(CEntityPtr& pEntity)
{
	CVector3 RelativeAttachmentPosition = CMatrix::TransformCoordinate(m_AttachmentPosition,pEntity->GetPosition()->GetOrientation());
	CVector3 AbsoluteAttachmentPosition = pEntity->GetPosition()->GetCenter() + RelativeAttachmentPosition;
	
	CVector3 ForceDirection = m_GroundPosition - AbsoluteAttachmentPosition;
	float    fSpringLength = ForceDirection.Length();
	
	ForceDirection.Normalize();

	float fCompressionLength = (fSpringLength-m_fRestingLength);
	float fForce = (fCompressionLength) * m_fForce;
	
	if(fForce < 0) fForce = 0;       //only pull towards the grounded position, don't push away.

	CVector3 linear = ForceDirection * fForce;
	CVector3 torque = CVector3::Cross(RelativeAttachmentPosition,linear);

	return CForce(linear,torque);
}


CForce CThrustForce::GetForce(CEntityPtr& pEntity)
{
	CVector3 RelativePosition  = CMatrix::TransformNormal(m_Position,pEntity->GetPosition()->GetOrientation());
	CVector3 RelativeDirection = CMatrix::TransformNormal(m_Direction,pEntity->GetPosition()->GetOrientation());

	CVector3 linear = RelativeDirection * m_fForce;
	CVector3 torque = CVector3::Cross(RelativePosition,linear);

	return CForce(linear,torque);
}

/*
CForce CSuspensionForce::GetForce(CEntityPtr& pEntity)
{
	CVector3 RelativePosition = CMatrix::TransformCoordinate(m_Position,pEntity->GetPosition()->GetOrientation());
	CVector3 AbsolutePosition = pEntity->GetPosition()->GetCenter() + RelativePosition;
	float fSpringLength = AbsolutePosition.GetY() - GetGame()->GetResources()->GetHeightMap()->GetHeight(AbsolutePosition.GetX(),AbsolutePosition.GetZ());

	float fCompressionLength = (m_fRestingLength-fSpringLength);
	float fForce = (fCompressionLength) * m_fForce;
	
	if(fForce < 0) fForce = 0;       //only push up. Gravity will pull the car down.
	
	CVector3 LinearVelocity  = pEntity->GetMotion()->GetVelocity();
	CVector3 AngularVelocity = pEntity->GetMotion()->GetAngularVelocity();
	CVector3 Velocity = LinearVelocity + CVector3::Cross(AngularVelocity,RelativePosition);

	fForce = fForce * fForce * 1/10.0f; 

	if(Velocity.GetY() >= 0) fForce = fForce * 0.4f;

	CVector3 Linear = m_Direction * fForce;
	CVector3 Torque = CVector3::Cross(RelativePosition,Linear);

	return CForce(Linear,Torque);
}
*/


CForce CDragForce::GetForce(CEntityPtr& pEntity)
{
	CVector3 LinearVelocity  = pEntity->GetMotion()->GetVelocity();
	CVector3 AngularVelocity = pEntity->GetMotion()->GetAngularVelocity();

	return CForce(-1 * CVector3::Normalize(LinearVelocity)  * m_fLinear  * LinearVelocity.Length() ,
	   			  -1 * CVector3::Normalize(AngularVelocity) * m_fAngular * AngularVelocity.Length() );
}

/*
CForce CFollowForce::GetForce(CEntityPtr& pEntity)
{
	//Spring that equalizes velocity
	CVector3 RelativeVelocity = m_pFollow->GetMotion()->GetVelocity() - pEntity->GetMotion()->GetVelocity(); 
	CVector3 velocityForce = RelativeVelocity * m_fForce * 0.5f;

	//Spring that equalizes position
	CVector3 relativePosition = CMatrix::TransformNormal(m_Position,m_pFollow->GetPosition()->GetOrientation());
	CVector3 desiredPosition  = m_pFollow->GetPosition()->GetCenter() + relativePosition;
	desiredPosition.SetY(m_Position.GetY() + m_pFollow->GetPosition()->GetCenter().GetY());

	float fGroundHeight = GetGame()->GetResources()->GetHeightMap()->GetHeight(desiredPosition.GetX(),desiredPosition.GetZ());
	if(desiredPosition.GetY() < fGroundHeight + 0.25f)
	{
		desiredPosition.SetY(fGroundHeight + 0.25f);
	}
	
	CVector3 posDifference = desiredPosition - pEntity->GetPosition()->GetCenter();
	CVector3 springForce = posDifference*posDifference.Length()*m_fForce;

	return CForce(velocityForce+springForce,CVector3(0,0,0));
}
*/

