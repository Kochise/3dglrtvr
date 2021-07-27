#pragma once

#include "../Pointers.h"

#include "Vector.h"

#include <vector>
using namespace std;

class CEntity
{
public:
	CEntity(void);
	virtual ~CEntity(void);

	CPositionPtr&   GetPosition()                             {return m_pPosition;}
	void            SetPosition(CPositionPtr pPosition)       {m_pPosition = pPosition;}   

	CMotionPtr&     GetMotion()                               {return m_pMotion;}
	void            SetMotion(CMotionPtr pMotion)             {m_pMotion = pMotion;}   

	CRenderablePtr& GetRenderable()                           {return m_pRenderable;}
	void            SetRenderable(CRenderablePtr pRenderable) {m_pRenderable = pRenderable;} 

	CCollisionModelPtr& GetCollisionModel()                                   {return m_pCollisionModel;}
	void                SetCollisionModel(CCollisionModelPtr pCollisionModel) {m_pCollisionModel = pCollisionModel;} 

	CAnimatablePtr& GetAnimatable()                           {return m_pAnimatable;}
	void            SetAnimatable(CAnimatablePtr pAnimatable) {m_pAnimatable = pAnimatable;} 

	IForcePtrVector& GetForces()                             {return m_Forces;}

protected:

	CPositionPtr       m_pPosition;
	CMotionPtr         m_pMotion;
	CRenderablePtr     m_pRenderable;
	CAnimatablePtr     m_pAnimatable;
	CCollisionModelPtr m_pCollisionModel;
	IForcePtrVector    m_Forces;
};





