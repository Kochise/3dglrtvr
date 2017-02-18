
#include "EntityFactory.h"
#include "Collision\CollisionSphere.h"
#include "Collision\CollisionPlane.h"
#include "Collision\CollisionBox.h"
#include "3DObject.h"
#include "Engine\GlutShapes.h"

CEntityFactory::CEntityFactory(void)
	:m_pSkull(new Geometry::C3DObject())
{
	m_pSkull->LoadFromObjFile("skull.obj");	
	m_pSkull->CenterOnOrigin();
	m_pSkull->Normalize(0.2f);
}

CEntityFactory::~CEntityFactory(void)
{
}



CEntityPtr CEntityFactory::CreateCamera(CVector3 position)
{
	CEntityPtr pEntity(new CEntity());
	pEntity->GetPosition()->SetCenter(position);
	//pEntity->SetCollisionModel(new CCollisionSphere(pEntity->GetPosition(),1,0.1f));
	//pEntity->GetForces().push_back(new CDragForce(0,10));
	pEntity->GetCollisionModel()->SetMass(1);
	return pEntity;
}

CEntityPtr CEntityFactory::CreateSkull(CVector3 position,CVector3 velocity)
{
	CEntityPtr pEntity(new CEntity());
	pEntity->GetPosition()->SetCenter(position);
	pEntity->GetMotion()->SetVelocity(velocity);
	pEntity->SetCollisionModel(new CCollisionSphere(pEntity->GetPosition(),0.3f,0.2f));
	pEntity->SetRenderable(new CMaterialRenderable(CMaterial::RandomMaterial(),m_pSkull));
	pEntity->GetCollisionModel()->SetRestitution(0.9);
	pEntity->GetCollisionModel()->SetFriction(0.05);
	return pEntity;
}

CEntityPtr CEntityFactory::CreateCube(CVector3 position,CVector3 velocity,float fSize)
{
	CEntityPtr pEntity(new CEntity());
	pEntity->GetPosition()->SetCenter(position);
	pEntity->GetMotion()->SetVelocity(velocity);
	pEntity->SetCollisionModel(new CCollisionBox(pEntity->GetPosition(),0.3f,fSize,fSize,fSize));
	pEntity->SetRenderable(new CMaterialRenderable(CMaterial::RandomMaterial(),new CGlutCube(fSize)));
	pEntity->GetCollisionModel()->SetRestitution(0.9);
	pEntity->GetCollisionModel()->SetFriction(0.05);
	return pEntity;
}

CEntityPtr CEntityFactory::CreateSphere(CVector3 position,CVector3 velocity,float fRadius,float fMass)
{
	CEntityPtr pEntity(new CEntity());
	pEntity->GetPosition()->SetCenter(position);
	pEntity->GetMotion()->SetVelocity(velocity);
	pEntity->SetCollisionModel(new CCollisionSphere(pEntity->GetPosition(),fMass,fRadius));
	pEntity->SetRenderable(new CMaterialRenderable(CMaterial::RandomMaterial(),new CGlutSphere(fRadius)));
	return pEntity;	
}


CEntityPtr CEntityFactory::CreateSpring(CVector3 GroundPosition,CVector3 AttachmentPosition, CEntityPtr pAttachEntity)
{
	CEntityPtr pEntity(new CEntity());
	pEntity->SetRenderable(new CSpringRenderable(GroundPosition,AttachmentPosition,pAttachEntity));
	return pEntity;		
}

CEntityPtr CEntityFactory::CreateWall(CVector3 position, CVector3 normal)
{
	CEntityPtr pEntity(new CEntity());
	pEntity->GetPosition()->SetCenter(position);
	pEntity->SetCollisionModel(new CCollisionPlane(pEntity->GetPosition(),normal));
	return pEntity;
}
