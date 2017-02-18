#pragma once

#include "Entity.h"
#include "Matrix.h"
#include "Collision/CollisionInfo.h"

class CPhysicsManager
{
public:
	CPhysicsManager(void){}
	virtual ~CPhysicsManager(void){}

	void AdvanceEntity(CEntityPtr& pEntity,float seconds);
    void AdvanceEntity(CEntityPtr& pEntity,IForcePtrVector& GlobalForces,float seconds);
	void AdvanceEntities(CEntityPtrVector& Entities,float seconds);
	void AdvanceEntities(CEntityPtrVector& Entities,IForcePtrVector& GlobalForces,float seconds);
	void CollideObjects(CCollisionInfoVector& Collisions);

	CMatrix OrthoNormalize(CMatrix matrix);
};

