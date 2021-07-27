#pragma once

#include "Sphere.h"
#include "Engine/Entity.h"

#include "../Pointers.h"
#include "CollisionInfo.h"
#include "CollisionModel.h"
#include "CollisionSphere.h"
#include "CollisionPlane.h"
#include "CollisionBox.h"

class CCollisionDetector
{
public:
	virtual ~CCollisionDetector(void){}
	CCollisionDetector(void){}

	void DetectCollisions(const CEntityPtrVector& Entities,CCollisionInfoVector& Collisions);
	void DetectCollisions(const CEntityPtrVector& DynamicEntities,const CEntityPtrVector& StaticEntities,CCollisionInfoVector& Collisions);
	bool DetectCollision(CCollisionModelPtr pModel1,CCollisionModelPtr pModel2,CCollisionInfo& CollisionInfo);

	//Sphere Collisions
	bool CollisionSphereSphere(CSphere object1,CSphere object2);
	bool CollisionSphereSphere(CSphere object1,CSphere object2,CCollisionInfo& CollisionInfo);
	bool CollisionSphereBox(CCollisionSpherePtr pSphere,CCollisionBoxPtr pBox,CCollisionInfo& CollisionInfo);	
	bool CollisionSpherePlane(CCollisionSpherePtr pSphere,CCollisionPlanePtr pPlane,CCollisionInfo& CollisionInfo);

	//Box Collisions
	//todo: box-box collision
	bool CollisionBoxBox(CCollisionBoxPtr pBox1,CCollisionBoxPtr pBox2,CCollisionInfo& CollisionInfo);
	bool CollisionBoxPlane(CCollisionBoxPtr pBox,CCollisionPlanePtr pPlane,CCollisionInfo& CollisionInfo);
	/*bool CollisionBoxHeightMap(CCollisionBoxPtr pBox,CCollisionHeightMapPtr pGround,CCollisionInfo& CollisionInfo);*/


protected:

	//these are sort of a hack as we want to detect more than one collision in some cases
	void AddCollision(CCollisionInfo& CollisionInfo);

	CEntityPtr m_pCurrentEntity1;
	CEntityPtr m_pCurrentEntity2;

	CCollisionInfoVector* m_pCollisionInfoVector;
};
