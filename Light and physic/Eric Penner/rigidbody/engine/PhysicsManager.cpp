#include "physicsmanager.h"

#include "Engine.h"

void CPhysicsManager::AdvanceEntity(CEntityPtr& pEntity,float seconds)
{
	IForcePtrVector EmtpyForces;
	AdvanceEntity(pEntity,EmtpyForces,seconds);
}

void CPhysicsManager::AdvanceEntity(CEntityPtr& pEntity,IForcePtrVector& GlobalForces,float seconds)
{
	CForce netForce(CVector3(0,0,0),CVector3(0,0,0));	
	if(pEntity->GetCollisionModel()->GetMass() != CCollisionModel::INFINITE_MASS)
	{
		IForcePtrVector& Forces = pEntity->GetForces();
		for(unsigned int i = 0; i < Forces.size(); i++)
		{
			CForce force = Forces[i]->GetForce(pEntity);
			netForce.m_Linear += force.m_Linear;
			netForce.m_Torque += force.m_Torque;
		}
		for(unsigned int i = 0; i < GlobalForces.size(); i++)
		{
			CForce force = GlobalForces[i]->GetForce(pEntity);
			netForce.m_Linear += force.m_Linear;
			netForce.m_Torque += force.m_Torque;
		}

		//Advance velocity
		pEntity->GetMotion()->SetVelocity       (pEntity->GetMotion()->GetVelocity()        + netForce.m_Linear * pEntity->GetCollisionModel()->GetOneOverMass() * seconds);
		
		//Advance Angular Velocity 
		CVector3 correctedTorque = CMatrix::TransformNormal(netForce.m_Torque,pEntity->GetCollisionModel()->GetRelativeInertiaTensorInverse());
		pEntity->GetMotion()->SetAngularVelocity(pEntity->GetMotion()->GetAngularVelocity() + correctedTorque * pEntity->GetCollisionModel()->GetOneOverMass() * seconds);
	}

	//Advance position
	pEntity->GetPosition()->SetCenter(pEntity->GetPosition()->GetCenter() + pEntity->GetMotion()->GetVelocity() * seconds);
	pEntity->GetPosition()->SetOrientation(pEntity->GetPosition()->GetOrientation() * CMatrix::RotationAxis(pEntity->GetMotion()->GetRotationAxis(),pEntity->GetMotion()->GetRotationRate()*seconds));

	pEntity->GetPosition()->SetOrientation(OrthoNormalize(pEntity->GetPosition()->GetOrientation()));
}

void CPhysicsManager::AdvanceEntities(CEntityPtrVector& Entities,IForcePtrVector& GlobalForces,float seconds)
{
	for(unsigned int i = 0; i < Entities.size(); i++)
	{
		AdvanceEntity(Entities[i],GlobalForces,seconds);
	}
}


void CPhysicsManager::AdvanceEntities(CEntityPtrVector& Entities,float seconds)
{
	IForcePtrVector EmtpyForces;
	for(unsigned int i = 0; i < Entities.size(); i++)
	{
		AdvanceEntity(Entities[i],EmtpyForces,seconds);
	}
}

CMatrix CPhysicsManager::OrthoNormalize(CMatrix matrix)
{
	return matrix;  //TODO:  implement this
}

void CPhysicsManager::CollideObjects(CCollisionInfoVector& Collisions)
{
	for(unsigned int i = 0; i < Collisions.size(); i++)
	{
		CCollisionInfo CollisionInfo = Collisions[i];
		CEntityPtr pEntityA = CollisionInfo.pEntity1;
		CEntityPtr pEntityB = CollisionInfo.pEntity2;

		CVector3 Normal = CollisionInfo.Normal;

		CVector3 VectorAP = CollisionInfo.Point - pEntityA->GetPosition()->GetCenter();
		CVector3 VectorBP = CollisionInfo.Point - pEntityB->GetPosition()->GetCenter();

		CVector3 VelocityA = pEntityA->GetMotion()->GetVelocity() + CVector3::Cross(pEntityA->GetMotion()->GetAngularVelocity(),VectorAP);
		CVector3 VelocityB = pEntityB->GetMotion()->GetVelocity() + CVector3::Cross(pEntityB->GetMotion()->GetAngularVelocity(),VectorBP);

		CVector3 VelocityAB = VelocityA - VelocityB;

		float dirDiff = CVector3::Dot(VelocityAB,Normal);
		if(dirDiff < 0)
		{
			float fRestitution = CollisionInfo.fRestitution;
			float fFriction    = CollisionInfo.fFriction;

			CVector3 Tangent = CVector3::Normalize(CVector3::Cross(CVector3::Cross(Normal,VelocityAB),Normal));
			
			float fNumerator = -(1+ fRestitution ) * dirDiff;

			CVector3 APTemp = CVector3::Cross(CMatrix::TransformNormal(CVector3::Cross(VectorAP,Normal),pEntityA->GetCollisionModel()->GetInertiaTensorInverse()),VectorAP);
			CVector3 BPTemp = CVector3::Cross(CMatrix::TransformNormal(CVector3::Cross(VectorBP,Normal),pEntityB->GetCollisionModel()->GetInertiaTensorInverse()),VectorBP);		
						
			float fMassA = pEntityA->GetCollisionModel()->GetOneOverMass();
			float fMassB = pEntityB->GetCollisionModel()->GetOneOverMass();

			if(fMassA == CCollisionModel::INFINITE_MASS) APTemp = CVector3(0,0,0);
			if(fMassB == CCollisionModel::INFINITE_MASS) BPTemp = CVector3(0,0,0);
			
			float fDenominator = fMassA + fMassB + CVector3::Dot((APTemp + BPTemp), Normal);

			float    fImpulse = fNumerator / fDenominator;
			CVector3 ImpulseV = fImpulse * Normal - ((fFriction * fImpulse) * Tangent);

			{
				pEntityA->GetMotion()->SetVelocity(pEntityA->GetMotion()->GetVelocity() + ImpulseV * pEntityA->GetCollisionModel()->GetOneOverMass());
			
				CVector3 AngularMomentumChange = CVector3::Cross( VectorAP, ImpulseV);
				CVector3 AngularVelocityChange = CMatrix::TransformNormal(AngularMomentumChange,pEntityA->GetCollisionModel()->GetRelativeInertiaTensorInverse());
				CVector3 CurrentAngularVelocity = pEntityA->GetMotion()->GetAngularVelocity();

				pEntityA->GetMotion()->SetAngularVelocity(CurrentAngularVelocity + AngularVelocityChange);
			}

			{
				pEntityB->GetMotion()->SetVelocity(pEntityB->GetMotion()->GetVelocity() - ImpulseV * pEntityB->GetCollisionModel()->GetOneOverMass());
		
				CVector3 AngularMomentumChange = CVector3::Cross( VectorBP, ImpulseV);
				CVector3 AngularVelocityChange = CMatrix::TransformNormal(AngularMomentumChange,pEntityB->GetCollisionModel()->GetRelativeInertiaTensorInverse());
				CVector3 CurrentAngularVelocity = pEntityB->GetMotion()->GetAngularVelocity();

				pEntityB->GetMotion()->SetAngularVelocity(CurrentAngularVelocity - AngularVelocityChange);
			}
		}
	}
}

