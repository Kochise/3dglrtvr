#pragma once

#include "Pointers.h"
#include "EntityFactory.h"
#include "Collision/CollisionDetector.h"
#include "Engine/PhysicsManager.h"

class CSimulation
{
public:
	CSimulation(void);
	virtual ~CSimulation(void);

	void InitPart1(void);
	void InitPart2(void);
	void InitPart3(void);
	void InitPart4(void);
	void InitPart5(void);
	void InitPart6(void);
	void Start(void);
	void Reverse(void);
	void UpdateAndRender(float fSimulationSpeed);

	void PrintState(void);
	void PrintHelp(void);

	void Advance(float fSeconds);
	void Render(void);

protected:

	CEntityPtrVector     m_DynamicEntities;  //Moving Entities
	CEntityPtrVector     m_StaticEntities;   //Non-Moving Entities

	CCollisionInfoVector m_Collisions;       //Collisions
	IForcePtrVector      m_GlobalForces;     //Forces affecting all objects

	CEntityFactory       m_EntityFactory;
	CCollisionDetector   m_CollisionDetector;
	CPhysicsManager      m_PhysicsManager;

	unsigned int m_nTickCount;
};
