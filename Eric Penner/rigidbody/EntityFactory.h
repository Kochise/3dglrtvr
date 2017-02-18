#pragma once


#include "Pointers.h"
#include "engine/Engine.h"
#include "3DObject.h"

class CEntityFactory
{
public:
	CEntityFactory(void);
	virtual ~CEntityFactory(void);

	CEntityPtr CreateCamera(CVector3 position);
	CEntityPtr CreateSkull (CVector3 position,CVector3 velocity);
	CEntityPtr CreateCube  (CVector3 position,CVector3 velocity,float fSize);
	CEntityPtr CreateSphere(CVector3 position,CVector3 velocity,float fRadius,float fMass = 0.3f);
	CEntityPtr CreateSpring(CVector3 GroundPosition,CVector3 AttachmentPosition, CEntityPtr pEntity);
	CEntityPtr CreateWall(CVector3 position, CVector3 normal);

protected:

	Geometry::C3DObjectPtr m_pSkull; 
};
