#include ".\simulation.h"

#include "Engine/Engine.h"
#include "EntityFactory.h"
#include "Lights.h"
#include "Random.h"

CSimulation::CSimulation(void)
{
	m_nTickCount = 0;
}

CSimulation::~CSimulation(void)
{
}


void CSimulation::InitPart1(void)
{
	m_StaticEntities.clear();
	m_DynamicEntities.clear();
	m_GlobalForces.clear();

	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3(-1, 0, 0),CVector3( 1, 0, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 1, 0, 0),CVector3(-1, 0, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0,-1, 0),CVector3( 0, 1, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 1, 0),CVector3( 0,-1, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 0,-1),CVector3( 0, 0, 1)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 0, 1),CVector3( 0, 0,-1)));
	
	for(int i = 0 ; i < 4; i++)
	{
		CVector3 Position(CRandom::RandomFloat(-0.9,0.9),CRandom::RandomFloat(-0.9,0.9),0);
		CVector3 Velocity(CRandom::RandomFloat(-1.0,1.0),CRandom::RandomFloat(-1.0,1.0),0);

		CEntityPtr Entity = m_EntityFactory.CreateSphere(Position,Velocity,0.1f);
		m_GlobalForces.push_back(new CGravitationalForce(Entity,1));
		m_DynamicEntities.push_back(Entity);
	}
}

void CSimulation::InitPart2(void)
{
	m_StaticEntities.clear();
	m_DynamicEntities.clear();
	m_GlobalForces.clear();

	m_StaticEntities.clear();
	m_DynamicEntities.clear();
	m_GlobalForces.clear();

	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3(-1, 0, 0),CVector3( 1, 0, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 1, 0, 0),CVector3(-1, 0, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0,-1, 0),CVector3( 0, 1, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 1, 0),CVector3( 0,-1, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 0,-1),CVector3( 0, 0, 1)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 0, 1),CVector3( 0, 0,-1)));
	
	for(int i = 0 ; i < 8; i++)
	{
		CVector3 Position(CRandom::RandomFloat(-0.9,0.9),CRandom::RandomFloat(-0.9,0.9),0);
		CVector3 Velocity(CRandom::RandomFloat(-1.0,1.0),CRandom::RandomFloat(-1.0,1.0),0);

		CEntityPtr Entity = m_EntityFactory.CreateSphere(Position,Velocity,0.1f);
		m_GlobalForces.push_back(new CGravitationalForce(Entity,1));
		m_DynamicEntities.push_back(Entity);
	}
}

void CSimulation::InitPart3(void)
{
	m_StaticEntities.clear();
	m_DynamicEntities.clear();
	m_GlobalForces.clear();

	//m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3(-1, 0, 0),CVector3( 1, 0, 0)));
	//m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 1, 0, 0),CVector3(-1, 0, 0)));
	//m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0,-1, 0),CVector3( 0, 1, 0)));
	//m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 1, 0),CVector3( 0,-1, 0)));
	//m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 0,-1),CVector3( 0, 0, 1)));
	//m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 0, 1),CVector3( 0, 0,-1)));
	
	{
		CVector3 Position(-1.0, 0.0, 0.0);
		CVector3 Velocity( 0.0, 1.0, 0.0);

		CEntityPtr Entity = m_EntityFactory.CreateSphere(Position,Velocity,0.1f,4.0);
		m_GlobalForces.push_back(new CGravitationalForce(Entity,1));
		m_DynamicEntities.push_back(Entity);
	}

	{
		CVector3 Position( 1.0, 0.0, 0.0);
		CVector3 Velocity( 0.0,-1.0, 0.0);

		CEntityPtr Entity = m_EntityFactory.CreateSphere(Position,Velocity,0.1f,4.0);
		m_GlobalForces.push_back(new CGravitationalForce(Entity,1));
		m_DynamicEntities.push_back(Entity);
	}

}


void CSimulation::InitPart4(void)
{
	m_StaticEntities.clear();
	m_DynamicEntities.clear();
	m_GlobalForces.clear();

	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3(-1, 0, 0),CVector3( 1, 0, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 1, 0, 0),CVector3(-1, 0, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0,-1, 0),CVector3( 0, 1, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 1, 0),CVector3( 0,-1, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 0,-1),CVector3( 0, 0, 1)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 0, 1),CVector3( 0, 0,-1)));
	
	for(int i = 0 ; i < 8; i++)
	{
		CVector3 Position(CRandom::RandomFloat(-0.9,0.9),CRandom::RandomFloat(-0.9,0.9),CRandom::RandomFloat(-0.9,0.9));
		CVector3 Velocity(CRandom::RandomFloat(-1.0,1.0),CRandom::RandomFloat(-1.0,1.0),CRandom::RandomFloat(-1.0,1.0));

		CEntityPtr Entity = m_EntityFactory.CreateSkull(Position,Velocity);
		m_GlobalForces.push_back(new CGravitationalForce(Entity,1));
		m_DynamicEntities.push_back(Entity);
	}
}

void CSimulation::InitPart5(void)
{
	m_StaticEntities.clear();
	m_DynamicEntities.clear();
	m_GlobalForces.clear();

	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3(-1, 0, 0),CVector3( 1, 0, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 1, 0, 0),CVector3(-1, 0, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0,-1, 0),CVector3( 0, 1, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 1, 0),CVector3( 0,-1, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 0,-1),CVector3( 0, 0, 1)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 0, 1),CVector3( 0, 0,-1)));

	for(int i = 0; i < 3; i++)
	{
		{
			CVector3 SpringGround = CVector3(-0.7,1, -0.7 + i * 0.7);
			CVector3 SpringEntity = CVector3(0.0f,0.2f,0.0f);
			CVector3 Velocity     = CVector3(CRandom::RandomFloat(0,1),CRandom::RandomFloat(0,1),CRandom::RandomFloat(0,1));

			CEntityPtr Entity1 = m_EntityFactory.CreateSkull(CVector3( -0.7, 0.7, -0.7 + i * 0.7),Velocity);
			Entity1->GetForces().push_back(new CGroundedSpringForce(SpringGround,SpringEntity,0.5f,10.0f));
			Entity1->GetForces().push_back(new CDirectionalForce(CVector3(0,-1,0),9.81f));
			Entity1->GetForces().push_back(new CDragForce(0.01f,0.005f));
			m_DynamicEntities.push_back(Entity1);
			m_StaticEntities.push_back(m_EntityFactory.CreateSpring(SpringGround,SpringEntity,Entity1));
		}
		{
			CVector3 SpringGround = CVector3(0,1, -0.7 + i * 0.7);
			CVector3 SpringEntity = CVector3(0.10f,0.10f,0.10f);
			CVector3 Velocity     = CVector3(CRandom::RandomFloat(0,1),CRandom::RandomFloat(0,1),CRandom::RandomFloat(0,1));

			CEntityPtr Entity1 = m_EntityFactory.CreateSkull(CVector3( 0.0, 0.7, -0.7 + i * 0.7),Velocity);
			Entity1->GetForces().push_back(new CGroundedSpringForce(SpringGround,SpringEntity,0.5f,10.0f));
			Entity1->GetForces().push_back(new CDirectionalForce(CVector3(0,-1,0),9.81f));
			Entity1->GetForces().push_back(new CDragForce(0.01f,0.005f));
			m_DynamicEntities.push_back(Entity1);
			m_StaticEntities.push_back(m_EntityFactory.CreateSpring(SpringGround,SpringEntity,Entity1));
		}
		{
			CVector3 SpringGround = CVector3(0.7,1, -0.7 + i * 0.7);
			CVector3 SpringEntity = CVector3(0.125f,0.125f,0.0f);
			CVector3 Velocity     = CVector3(CRandom::RandomFloat(0,1),CRandom::RandomFloat(0,1),CRandom::RandomFloat(0,1));

			CEntityPtr Entity1 = m_EntityFactory.CreateSkull(CVector3( 0.7, 0.7, -0.7 + i * 0.7),Velocity);
			Entity1->GetForces().push_back(new CGroundedSpringForce(SpringGround,SpringEntity,0.5f,10.0f));
			Entity1->GetForces().push_back(new CDirectionalForce(CVector3(0,-1,0),9.81f));
			Entity1->GetForces().push_back(new CDragForce(0.01f,0.005f));
			m_DynamicEntities.push_back(Entity1);

			m_StaticEntities.push_back(m_EntityFactory.CreateSpring(SpringGround,SpringEntity,Entity1));
		}
	}

}

void CSimulation::InitPart6(void)
{
	m_StaticEntities.clear();
	m_DynamicEntities.clear();
	m_GlobalForces.clear();

	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3(-1, 0, 0),CVector3( 1, 0, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 1, 0, 0),CVector3(-1, 0, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0,-1, 0),CVector3( 0, 1, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 1, 0),CVector3( 0,-1, 0)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 0,-1),CVector3( 0, 0, 1)));
	m_StaticEntities.push_back(m_EntityFactory.CreateWall(CVector3( 0, 0, 1),CVector3( 0, 0,-1)));

	for(int i = 0; i < 3; i++)
	{
		{
			CVector3 SpringGround = CVector3(-0.7,1, -0.7 + i * 0.7);
			CVector3 SpringEntity = CVector3(0.0f,0.15f,0.0f);
			CVector3 Velocity     = CVector3(CRandom::RandomFloat(0,1),CRandom::RandomFloat(0,1),CRandom::RandomFloat(0,1));

			CEntityPtr Entity1 = m_EntityFactory.CreateCube(CVector3( -0.7, 0.7, -0.7 + i * 0.7),Velocity,0.3f);
			Entity1->GetForces().push_back(new CGroundedSpringForce(SpringGround,SpringEntity,0.5f,10.0f));
			Entity1->GetForces().push_back(new CDirectionalForce(CVector3(0,-1,0),9.81f));
			Entity1->GetForces().push_back(new CDragForce(0.01f,0.005f));
			m_DynamicEntities.push_back(Entity1);
			m_StaticEntities.push_back(m_EntityFactory.CreateSpring(SpringGround,SpringEntity,Entity1));
		}
		{
			CVector3 SpringGround = CVector3(0,1, -0.7 + i * 0.7);
			CVector3 SpringEntity = CVector3(0.15f,0.15f,0.15f);
			CVector3 Velocity     = CVector3(CRandom::RandomFloat(0,1),CRandom::RandomFloat(0,1),CRandom::RandomFloat(0,1));

			CEntityPtr Entity1 = m_EntityFactory.CreateCube(CVector3( 0.0, 0.7, -0.7 + i * 0.7),Velocity,0.3f);
			Entity1->GetForces().push_back(new CGroundedSpringForce(SpringGround,SpringEntity,0.5f,10.0f));
			Entity1->GetForces().push_back(new CDirectionalForce(CVector3(0,-1,0),9.81f));
			Entity1->GetForces().push_back(new CDragForce(0.01f,0.005f));
			m_DynamicEntities.push_back(Entity1);
			m_StaticEntities.push_back(m_EntityFactory.CreateSpring(SpringGround,SpringEntity,Entity1));
		}
		{
			CVector3 SpringGround = CVector3(0.7,1, -0.7 + i * 0.7);
			CVector3 SpringEntity = CVector3(0.15f,0.15f,0.0f);
			CVector3 Velocity     = CVector3(CRandom::RandomFloat(0,1),CRandom::RandomFloat(0,1),CRandom::RandomFloat(0,1));

			CEntityPtr Entity1 = m_EntityFactory.CreateCube(CVector3( 0.7, 0.7, -0.7 + i * 0.7),Velocity,0.3f);
			Entity1->GetForces().push_back(new CGroundedSpringForce(SpringGround,SpringEntity,0.5f,10.0f));
			Entity1->GetForces().push_back(new CDirectionalForce(CVector3(0,-1,0),9.81f));
			Entity1->GetForces().push_back(new CDragForce(0.01f,0.005f));
			m_DynamicEntities.push_back(Entity1);

			m_StaticEntities.push_back(m_EntityFactory.CreateSpring(SpringGround,SpringEntity,Entity1));
		}
	}

}




void CSimulation::Start(void)
{
	m_nTickCount = GetTickCount();
}

void CSimulation::Reverse(void)
{
	for(int i = 0, n = (int) m_DynamicEntities.size(); i < n; i++)
	{
		CVector3 NewVelocity        = m_DynamicEntities[i]->GetMotion()->GetVelocity() * -1;
		CVector3 NewAngularVelocity = m_DynamicEntities[i]->GetMotion()->GetAngularVelocity() * -1;
		m_DynamicEntities[i]->GetMotion()->SetVelocity(NewVelocity);
		m_DynamicEntities[i]->GetMotion()->SetAngularVelocity(NewAngularVelocity);
	}
}

void CSimulation::UpdateAndRender(float fSimulationSpeed)
{
	unsigned int nNewTickCount = GetTickCount();
	float fSeconds = (nNewTickCount - m_nTickCount) * 0.001f * fSimulationSpeed;
	m_nTickCount = nNewTickCount;

	if(fSeconds > 0.1f)  //Minimum of 1/10th of a second update (for stability)
		fSeconds = 0.1f;

	Advance(fSeconds);
	Render();
}

void CSimulation::Advance(float fSeconds)
{
	m_Collisions.clear();

	//Need to run the physics system a few times to keep things smooth
	float fSecondsOver5 = fSeconds/5;   
	for(int i = 0; i < 5; i++)
	{
		m_PhysicsManager.AdvanceEntities(m_DynamicEntities,m_GlobalForces,fSecondsOver5);
		m_CollisionDetector.DetectCollisions(m_DynamicEntities,m_Collisions);
		m_CollisionDetector.DetectCollisions(m_DynamicEntities,m_StaticEntities,m_Collisions);
		m_PhysicsManager.CollideObjects(m_Collisions);	
	}
}

void CSimulation::Render(void)
{
	CScopeLighting lighting;
	CScopeLight0 light;
	CScopeBlueMaterial material;

	for(int i = 0, n = (int)m_StaticEntities.size(); i < n; i++)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			CMatrix m = m_StaticEntities[i]->GetPosition()->GetMatrix();
			glMultMatrixf((float*)m.m);
			m_StaticEntities[i]->GetRenderable()->Render();
		glPopMatrix();
	}

	for(int i = 0, n = (int)m_DynamicEntities.size(); i < n; i++)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			CMatrix m = m_DynamicEntities[i]->GetPosition()->GetMatrix();
			glMultMatrixf((float*)m.m);
			m_DynamicEntities[i]->GetRenderable()->Render();
		glPopMatrix();
	}
}

void CSimulation::PrintHelp()
{
		printf ("l-6\t View Different Simulations\n");
		printf ("p\t Pause/Resume (at normal speed)\n");
		printf ("w\t Speed Up\n");
		printf ("s\t Slow Down\n");
		printf ("a\t Step back 1/20th of a second\n");
		printf ("d\t Step ahead 1/20th of a second\n");
		printf ("r\t Reverse\n");
		printf ("q\t Query State\n");
		printf ("h\t Print Help\n");
		printf ("ESC\t Exit\n\n");
}

void CSimulation::PrintState()
{
	for(int i = 0, n = (int) m_DynamicEntities.size(); i < n; i++)
	{
		CVector3 p = m_DynamicEntities[i]->GetPosition()->GetCenter();
		CVector3 v = m_DynamicEntities[i]->GetMotion()->GetVelocity();
		printf("Object %d: Position(%5.2f,%5.2f,%5.2f) Velocity(%5.2f,%5.2f,%5.2f)\n",i,p.x,p.y,p.z,v.x,v.y,v.z);
	}

	printf("\n");

	PrintHelp();
}
