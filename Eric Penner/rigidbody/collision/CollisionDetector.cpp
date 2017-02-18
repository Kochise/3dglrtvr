#include "collisiondetector.h"
#include "CollisionSphere.h"
//#include "CollisionHeightMap.h"
#include "Engine/Engine.h"
//#include "Game.h"

void CCollisionDetector::DetectCollisions(const CEntityPtrVector& Entities,CCollisionInfoVector& Collisions)
{
	m_pCollisionInfoVector = &Collisions;
	CCollisionInfo CollisionInfo;

	for(int i = 0, n = (int)Entities.size(); i < n; i++)
	{
		for(int j = i+1; j < n; j++)
		{
			m_pCurrentEntity1 = Entities[i];
			m_pCurrentEntity2 = Entities[j];

			CCollisionModelPtr pModel1 = m_pCurrentEntity1->GetCollisionModel();
			CCollisionModelPtr pModel2 = m_pCurrentEntity2->GetCollisionModel();

			if(DetectCollision(pModel1,pModel2,CollisionInfo))
			{
				CollisionInfo.pEntity1 = m_pCurrentEntity1;
				CollisionInfo.pEntity2 = m_pCurrentEntity2;
				CollisionInfo.fFriction    = (pModel1->GetFriction()    + pModel2->GetFriction())/2;
				CollisionInfo.fRestitution = (pModel1->GetRestitution() + pModel2->GetRestitution())/2;
				Collisions.push_back(CollisionInfo);
			}
		}
	}
}


void CCollisionDetector::DetectCollisions(const CEntityPtrVector& DynamicEntities,const CEntityPtrVector& StaticEntities,CCollisionInfoVector& Collisions)
{
	m_pCollisionInfoVector = &Collisions;
	CCollisionInfo CollisionInfo;

	for(int i = 0, n = (int)DynamicEntities.size(); i < n; i++)
	{
		for(int j = 0, m = (int)StaticEntities.size(); j < m; j++)
		{
			m_pCurrentEntity1 = DynamicEntities[i];
			m_pCurrentEntity2 = StaticEntities[j];

			CCollisionModelPtr pModel1 = m_pCurrentEntity1->GetCollisionModel();
			CCollisionModelPtr pModel2 = m_pCurrentEntity2->GetCollisionModel();

			if(DetectCollision(pModel1,pModel2,CollisionInfo))
			{
				CollisionInfo.pEntity1 = m_pCurrentEntity1;
				CollisionInfo.pEntity2 = m_pCurrentEntity2;
				CollisionInfo.fFriction    = (pModel1->GetFriction()    + pModel2->GetFriction())/2;
				CollisionInfo.fRestitution = (pModel1->GetRestitution() + pModel2->GetRestitution())/2;
				Collisions.push_back(CollisionInfo);
			}	
		}
	}
}




bool CCollisionDetector::DetectCollision(CCollisionModelPtr pModel1,CCollisionModelPtr pModel2,CCollisionInfo& CollisionInfo)
{
	bool bCollisionOccured = false;
	bool bReverseNormal    = false;

	if(pModel1.Is<CCollisionSphere>() && pModel2.Is<CCollisionSphere>())
		bCollisionOccured = CollisionSphereSphere(pModel1.DynamicCast<CCollisionSphere>()->GetBoundingSphere(),pModel2.DynamicCast<CCollisionSphere>()->GetBoundingSphere(),CollisionInfo);

	else if(pModel1.Is<CCollisionSphere>() && pModel2.Is<CCollisionPlane>())
		bCollisionOccured = CollisionSpherePlane(pModel1.DynamicCast<CCollisionSphere>(),pModel2.DynamicCast<CCollisionPlane>(),CollisionInfo);

	else if(pModel2.Is<CCollisionSphere>() && pModel1.Is<CCollisionPlane>())
	{
		bCollisionOccured = CollisionSpherePlane(pModel2.DynamicCast<CCollisionSphere>(),pModel1.DynamicCast<CCollisionPlane>(),CollisionInfo);		
		bReverseNormal = true;
	}
	
	else if(pModel1.Is<CCollisionSphere>() && pModel2.Is<CCollisionBox>())
		bCollisionOccured = CollisionSphereBox(pModel1.DynamicCast<CCollisionSphere>(),pModel2.DynamicCast<CCollisionBox>(),CollisionInfo);

	else if(pModel2.Is<CCollisionSphere>() && pModel1.Is<CCollisionBox>())
	{
		bCollisionOccured = CollisionSphereBox(pModel2.DynamicCast<CCollisionSphere>(),pModel1.DynamicCast<CCollisionBox>(),CollisionInfo);		
		bReverseNormal = true;
	}
	
	else if(pModel1.Is<CCollisionBox>() && pModel2.Is<CCollisionPlane>())
		bCollisionOccured = CollisionBoxPlane(pModel1.DynamicCast<CCollisionBox>(),pModel2.DynamicCast<CCollisionPlane>(),CollisionInfo);

	else if(pModel2.Is<CCollisionBox>() && pModel1.Is<CCollisionPlane>())
	{
		bCollisionOccured = CollisionBoxPlane(pModel2.DynamicCast<CCollisionBox>(),pModel1.DynamicCast<CCollisionPlane>(),CollisionInfo);		
		bReverseNormal = true;
	}

	else if(pModel1.Is<CCollisionBox>() && pModel2.Is<CCollisionBox>())
	{
		bCollisionOccured = CollisionBoxBox(pModel1.DynamicCast<CCollisionBox>(),pModel2.DynamicCast<CCollisionBox>(),CollisionInfo);
	}

	/*
	else if(pModel1.Is<CCollisionBox>() && pModel2.Is<CCollisionHeightMap>())
		bCollisionOccured = CollisionBoxHeightMap(pModel1.DynamicCast<CCollisionBox>(),pModel2.DynamicCast<CCollisionHeightMap>(),CollisionInfo);

	else if(pModel2.Is<CCollisionBox>() && pModel1.Is<CCollisionHeightMap>())
	{
		bCollisionOccured = CollisionBoxHeightMap(pModel2.DynamicCast<CCollisionBox>(),pModel1.DynamicCast<CCollisionHeightMap>(),CollisionInfo);		
		bReverseNormal = true;
	}
	*/


	if(bCollisionOccured)
	{
		if(bReverseNormal)
			CollisionInfo.Normal = CollisionInfo.Normal * -1;
		return true;
	}

	return false;
}


bool CCollisionDetector::CollisionSphereSphere(CSphere object1,CSphere object2)
{
	float collisionDistance   = object1.GetRadius() + object2.GetRadius();
	float collisionDistanceSq = collisionDistance * collisionDistance;

	CVector3 difference       = object1.GetCenter() - object2.GetCenter();
	float   distanceSq        = difference.LengthSq();

	if( distanceSq < collisionDistanceSq)
		return true;
	else
		return false;
}

bool CCollisionDetector::CollisionSphereSphere(CSphere object1,CSphere object2,CCollisionInfo& CollisionInfo)
{
	float collisionDistance   = object1.GetRadius() + object2.GetRadius();
	float collisionDistanceSq = collisionDistance * collisionDistance;

	CVector3 difference       = object1.GetCenter() - object2.GetCenter();
	float   distanceSq        = difference.LengthSq();

	if( distanceSq < collisionDistanceSq)
	{
		difference.Normalize();
		CollisionInfo.Normal = difference;
		CollisionInfo.Point  = object2.GetCenter() + difference * object2.GetRadius();
		return true;
	}

	return false;
}


bool CCollisionDetector::CollisionSphereBox(CCollisionSpherePtr pSphere,CCollisionBoxPtr pBox,CCollisionInfo& CollisionInfo)
{
	return false;
	/*
	if(CollisionSphereSphere(pSphere->GetBoundingSphere(),pBox->GetBoundingSphere()))
	{
		//Transform sphere into box coordinates
		CSphere Sphere = pSphere->GetBoundingSphere();		
		CMatrix WorldToBox = pBox->GetPosition()->GetMatrixInverse();
		CVector3 SpherePos = CMatrix::TransformCoordinate(Sphere.GetCenter(),WorldToBox);
		
		if(fabs(pBox->m_WidthOver2 - SpherePos.GetX()) <  Sphere.GetRadius())
		{
			
		}
		if(fabs(pBox->m_HeightOver2 - SpherePos.GetY()) <  Sphere.GetRadius())
		{
			
		}
		if(fabs(pBox->m_DepthOver2 - SpherePos.GetZ()) <  Sphere.GetRadius())
		{
			
		}
	}
	return false;
	*/
}



bool CCollisionDetector::CollisionSpherePlane(CCollisionSpherePtr pSphere,CCollisionPlanePtr pPlane,CCollisionInfo& CollisionInfo)
{
	CSphere BoundingSphere = pSphere->GetBoundingSphere();

	if(pPlane->DistanceToPoint(BoundingSphere.GetCenter()) < BoundingSphere.GetRadius())
	{
		CollisionInfo.Normal = pPlane->GetNormal();
		CollisionInfo.Point  = BoundingSphere.GetCenter() +  CollisionInfo.Normal * -BoundingSphere.GetRadius();
		return true;
	}
	return false;
}


bool CCollisionDetector::CollisionBoxPlane(CCollisionBoxPtr pBox,CCollisionPlanePtr pPlane,CCollisionInfo& CollisionInfo)
{
	//Sphere Test
	CSphere BoundingSphere = pBox->GetBoundingSphere();
	if(pPlane->DistanceToPoint(BoundingSphere.GetCenter()) > BoundingSphere.GetRadius())
	{
		return false;
	}

	//Box Test
	CVector3 Corners[8];
	CMatrix	BoxToWorld = pBox->GetPosition()->GetMatrix();
	CMatrix::TransformCoordinateArray(Corners,pBox->m_Corners,BoxToWorld,8);

	CVector3 Accumulator(0,0,0);
	int      nCount = 0;

	for(int i = 0; i < 8; i++)
	{
		if(pPlane->DistanceToPoint(Corners[i])<=0)
		{
			nCount++;
			Accumulator += Corners[i];
		}
	}

	if(nCount > 0)
	{
		CollisionInfo.Normal = pPlane->GetNormal();
		CollisionInfo.Point  = Accumulator * (1.0f/nCount);
		return true;	
	}

	return false;
}

/*
bool CCollisionDetector::CollisionBoxHeightMap(CCollisionBoxPtr pBox,CCollisionHeightMapPtr pGround,CCollisionInfo& CollisionInfo)
{
	CHeightMapPtr pHeightMap = pGround->m_pHeightMap;
	CMatrix BoxToWorld = pBox->GetPosition()->GetMatrix();

	for(int i = 0; i < 8; i++)
	{
		CVector3 WorldPoint = CMatrix::TransformCoordinate(pBox->m_Corners[i],BoxToWorld);

		if(pHeightMap->GetHeight(WorldPoint.GetX(),WorldPoint.GetZ()) >= WorldPoint.GetY())
		{
			CollisionInfo.Point  = WorldPoint;
			CollisionInfo.Normal = CVector3(0,1,0);
			AddCollision(CollisionInfo);
		}
	}

	return false;
}
*/

void CCollisionDetector::AddCollision(CCollisionInfo& CollisionInfo)
{
	CollisionInfo.pEntity1 = m_pCurrentEntity1;
	CollisionInfo.pEntity2 = m_pCurrentEntity2;
	
	CollisionInfo.fFriction    = (m_pCurrentEntity1->GetCollisionModel()->GetFriction()    + m_pCurrentEntity2->GetCollisionModel()->GetFriction())/2;
	CollisionInfo.fRestitution = (m_pCurrentEntity1->GetCollisionModel()->GetRestitution() + m_pCurrentEntity2->GetCollisionModel()->GetRestitution())/2;
	m_pCollisionInfoVector->push_back(CollisionInfo);	
}


bool CCollisionDetector::CollisionBoxBox(CCollisionBoxPtr pBox1,CCollisionBoxPtr pBox2,CCollisionInfo& CollisionInfo)
{
	if(!CollisionSphereSphere(pBox1->GetBoundingSphere(),pBox2->GetBoundingSphere()))
		return false;

	float Box1Volume = pBox1->GetVolume();
	float Box2Volume = pBox2->GetVolume();

	CVector3 Box1Center = pBox1->GetPosition()->GetCenter();
	CVector3 Box2Center = pBox2->GetPosition()->GetCenter();

	{
		CMatrix Box1ToWorld = pBox1->GetPosition()->GetMatrix();
		CMatrix WorldToBox2 = pBox2->GetPosition()->GetMatrixInverse();
		CMatrix Box1ToBox2  = Box1ToWorld * WorldToBox2;

		for(int i = 0; i < (int)pBox1->m_HardPoints.size(); i++)
		{
			if(pBox2->ContainsPoint(CMatrix::TransformCoordinate(pBox1->m_HardPoints[i],Box1ToBox2)))
			{
				CollisionInfo.Point  = CMatrix::TransformCoordinate(pBox1->m_HardPoints[i],Box1ToWorld);
				/*
				if(Box1Volume > Box2Volume)
					CollisionInfo.Normal = -CVector3::Normalize(Box1Center-CollisionInfo.Point);
				else
					CollisionInfo.Normal = CVector3::Normalize(Box2Center-CollisionInfo.Point);
				*/
				CollisionInfo.Normal = -CMatrix::TransformNormal(CVector3::Normalize(pBox1->m_HardPoints[i]),Box1ToWorld);
				CollisionInfo.Normal.y = 0;
				CollisionInfo.Normal.Normalize();
				AddCollision(CollisionInfo);
			}
		}
	}

	{
		CMatrix Box2ToWorld = pBox2->GetPosition()->GetMatrix();
		CMatrix WorldToBox1 = pBox1->GetPosition()->GetMatrixInverse();
		CMatrix Box2ToBox1  = Box2ToWorld * WorldToBox1;

		for(int i = 0; i < (int)pBox2->m_HardPoints.size(); i++)
		{
			if(pBox1->ContainsPoint(CMatrix::TransformCoordinate(pBox2->m_HardPoints[i],Box2ToBox1)))
			{
				CollisionInfo.Point  = CMatrix::TransformCoordinate(pBox2->m_HardPoints[i],Box2ToWorld);
				/*
				if(Box1Volume > Box2Volume)
					CollisionInfo.Normal = -CVector3::Normalize(Box1Center-CollisionInfo.Point);
				else
					CollisionInfo.Normal = CVector3::Normalize(Box2Center-CollisionInfo.Point);
				*/
				CollisionInfo.Normal = CMatrix::TransformNormal(CVector3::Normalize(pBox2->m_HardPoints[i]),Box2ToWorld);
				CollisionInfo.Normal.y = 0;
				CollisionInfo.Normal.Normalize();
				AddCollision(CollisionInfo);
			}
		}
	}


	return false;
}


/*



		public void DetectCollisions(ArrayList DynamicEntities,ArrayList StaticEntities,ref ArrayList Collisions)
		{
			CollisionInfo collisionInfo = new CollisionInfo();

			for(int i = 0, n = DynamicEntities.Count; i < n; i++)
			{
				for(int j = 0, m = StaticEntities.Count; j < m; j++)
				{
					Entity object1 = (Entity)DynamicEntities[i];
					Entity object2 = (Entity)StaticEntities[j];

					CollisionModel model1 = object1.CollisionModel;
					CollisionModel model2 = object2.CollisionModel;

					if(DetectCollision(model1,model2,ref collisionInfo))
					{
						collisionInfo.Object1 = object1;
						collisionInfo.Object2 = object2;
						Collisions.Add(collisionInfo);
					}	
				}
			}
		}
			
		public void DetectCollisions(ArrayList Entities,ref ArrayList Collisions)
		{
			CollisionInfo collisionInfo = new CollisionInfo();

			for(int i = 0, n = Entities.Count; i < n; i++)
			{
				for(int j = i+1; j < n; j++)
				{
					Entity object1 = (Entity)Entities[i];
					Entity object2 = (Entity)Entities[j];

					CollisionModel model1 = object1.CollisionModel;
					CollisionModel model2 = object2.CollisionModel;

					if(DetectCollision(model1,model2,ref collisionInfo))
					{
						collisionInfo.Object1 = object1;
						collisionInfo.Object2 = object2;
						Collisions.Add(collisionInfo);
					}
				}
			}
		}

		public bool DetectCollision(CollisionModel model1,CollisionModel model2,ref CollisionInfo collisionInfo)
		{
			bool collisionOccured = false;
			bool reverseNormal    = false;

			if(model1 is CollisionSphere && model2 is CollisionSphere)
				collisionOccured = collisionSphereSphere(((CollisionSphere)model1).BoundingSphere,((CollisionSphere)model2).BoundingSphere,ref collisionInfo);
					
			else if(model1 is CollisionImplicit && model2 is CollisionImplicit)
				collisionOccured = collisionImplicitImplicit((CollisionImplicit)model1,(CollisionImplicit)model2,ref collisionInfo);

	
			else if(model1 is CollisionSphere && model2 is CollisionImplicit)
				collisionOccured = collisionSphereImplicit(((CollisionSphere)model1).BoundingSphere,(CollisionImplicit)model2,ref collisionInfo);
			else if(model2 is CollisionSphere && model1 is CollisionImplicit)
			{
				collisionOccured = collisionSphereImplicit(((CollisionSphere)model2).BoundingSphere,(CollisionImplicit)model1,ref collisionInfo);
				reverseNormal = true;
			}


			else if(model1 is CollisionSphere && model2 is CollisionSphereContainer) 
				collisionOccured = collisionSphereContainingSphere(((CollisionSphere)model1).BoundingSphere,((CollisionSphereContainer)model2).ContainingSphere,ref collisionInfo);	
			else if(model2 is CollisionSphere && model1 is CollisionSphereContainer) 
			{
				collisionOccured = collisionSphereContainingSphere(((CollisionSphere)model2).BoundingSphere,((CollisionSphereContainer)model1).ContainingSphere,ref collisionInfo);
				reverseNormal = true;
			}


			else if(model1 is CollisionSphereContainer && model2 is CollisionImplicit) 
				collisionOccured = collisionContainingSphereImplicit(((CollisionSphereContainer)model1).ContainingSphere,(CollisionImplicit)model2,ref collisionInfo);
			else if(model2 is CollisionSphereContainer && model1 is CollisionImplicit) 
			{
				collisionOccured = collisionContainingSphereImplicit(((CollisionSphereContainer)model2).ContainingSphere,(CollisionImplicit)model1,ref collisionInfo);
				reverseNormal = true;
			}

			if(collisionOccured)
			{
				if(reverseNormal)
					collisionInfo.Normal = collisionInfo.Normal * -1;
				
				return true;
			}

			return false;
		}


		*/