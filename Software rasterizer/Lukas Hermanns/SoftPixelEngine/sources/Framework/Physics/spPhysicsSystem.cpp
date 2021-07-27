/*
 * Physics system file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Framework/Physics/spPhysicsSystem.hpp"

#ifdef SP_COMPILE_WITH_NEWTONPHYSICS


#include "Base/spMemoryManagement.hpp"

#include <newton.h>


namespace sp
{
namespace physics
{


/*
 * Static callbacks
 */

void DefCallbackGenericContactProcess(const NewtonJoint* Contact, f32 TimeStep, s32 ThreadIndex)
{
    const NewtonMaterial* Material = NewtonContactGetMaterial(NewtonContactJointGetFirstContact(Contact));
    
    const f32 Velocity = NewtonMaterialGetContactNormalSpeed(Material);
    
    dim::vector3df Point, Normal;
    NewtonMaterialGetContactPositionAndNormal(Material, &Point.X, &Normal.X);
    
    PhysicsSystem::ContactCallback_(Point, Normal, Velocity);
}


/*
 * PhysicsSystem class
 */

NewtonWorld* PhysicsSystem::CurWorld_ = 0;
PFNPHYSICSCONTACTCALLBACKPROC PhysicsSystem::ContactCallback_ = 0;

PhysicsSystem::PhysicsSystem() : NWorld_(0)
{
    /* Print newton library information */
    io::Log::message(getVersion(), 0);
    io::Log::message("Copyright (c) <2003-2011> - Julio Jerez and Alain Suero", 0);
    io::Log::message("", 0);
    
    /* Create the newton world */
    CurWorld_ = NWorld_ = NewtonCreate(0, 0);
    
    s32 MaterialGroupID = NewtonMaterialGetDefaultGroupID(NWorld_);
    NewtonMaterialSetDefaultFriction    (NWorld_, MaterialGroupID, MaterialGroupID, 0.8f, 0.4f);
    NewtonMaterialSetDefaultElasticity  (NWorld_, MaterialGroupID, MaterialGroupID, 0.3f);
    NewtonMaterialSetDefaultSoftness    (NWorld_, MaterialGroupID, MaterialGroupID, 0.05f);
    
    /* General settings */
    Gravity_ = dim::vector3df(0, -9.81f, 0);
}
PhysicsSystem::~PhysicsSystem()
{
    NewtonDestroy(NWorld_);
}

io::stringc PhysicsSystem::getVersion() const
{
    io::stringc Version(NewtonWorldGetVersion(NWorld_));
    Version = Version.left(1) + "." + Version.right(2);
    return "NewtonGameDynamics - v." + Version;
}

void PhysicsSystem::updateSimulation(const f32 StepTime)
{
    NewtonUpdate(NWorld_, StepTime);
}

void PhysicsSystem::setGravity(const dim::vector3df &Gravity)
{
    Gravity_ = Gravity;
    for (std::list<RigidBody*>::iterator it = RigidBodyList_.begin(); it != RigidBodyList_.end(); ++it)
        (*it)->setGravity(Gravity);
}

StaticBody* PhysicsSystem::createStaticBody(scene::Mesh* MeshGeom)
{
    if (!MeshGeom)
    {
        io::Log::error("Invalid root node for static body");
        return 0;
    }
    
    CurWorld_ = NWorld_;
    StaticBody* NewBody = new StaticBody(MeshGeom);
    StaticBodyList_.push_back(NewBody);
    
    return NewBody;
}
void PhysicsSystem::deleteStaticBody(StaticBody* Object)
{
    CurWorld_ = NWorld_;
    MemoryManager::removeElement(StaticBodyList_, Object, true);
}

RigidBody* PhysicsSystem::createRigidBody(
    scene::SceneNode* RootNode, const ERigidBodies Type, const SRigidBodyConstruction &Construct, const f32 Mass)
{
    if (!RootNode)
    {
        io::Log::error("Invalid root node for rigid body");
        return 0;
    }
    
    CurWorld_ = NWorld_;
    RigidBody* NewBody = 0;
    
    if (Type == RIGIDBODY_MESH)
    {
        if (RootNode->getType() != scene::NODE_MESH)
        {
            io::Log::error("Create mesh rigid body with no valid mesh object");
            return 0;
        }
        else
            NewBody = new RigidBody((scene::Mesh*)RootNode);
    }
    else
    {
        NewBody = new RigidBody(Type, Construct);
        NewBody->setRootNode(RootNode);
    }
    
    RootNode->setParent(0);
    NewBody->setMass(Mass);
    NewBody->setGravity(Gravity_);
    NewBody->setTransformation(RootNode->getPositionMatrix() * RootNode->getRotationMatrix());
    
    RigidBodyList_.push_back(NewBody);
    
    return NewBody;
}

RigidBody* PhysicsSystem::createRigidBody(scene::Mesh* MeshGeom, const f32 Mass)
{
    if (!MeshGeom)
    {
        io::Log::error("Invalid root node for rigid body");
        return 0;
    }
    
    CurWorld_ = NWorld_;
    RigidBody* NewBody = new RigidBody(MeshGeom);
    
    MeshGeom->setParent(0);
    NewBody->setMass(Mass);
    NewBody->setGravity(Gravity_);
    NewBody->setTransformation(MeshGeom->getPositionMatrix() * MeshGeom->getRotationMatrix());
    
    RigidBodyList_.push_back(NewBody);
    
    return NewBody;
}
void PhysicsSystem::deleteRigidBody(RigidBody* Object)
{
    CurWorld_ = NWorld_;
    MemoryManager::removeElement(RigidBodyList_, Object, true);
}

/*Ragdoll* PhysicsSystem::createRagdoll()
{
    return 0;
}
void PhysicsSystem::deleteRagdoll(Ragdoll* Object)
{
}

Vehicle* PhysicsSystem::createVehicle()
{
    return 0;
}
PhysicsSystem::deleteVehicle(Vehicle* Object)
{
}

MaterialGroup* PhysicsSystem::createMaterialGroup()
{
}
void PhysicsSystem::deleteMaterialGroup(MaterialGroup* Material)
{
}*/

void PhysicsSystem::setThreadCount(s32 Count)
{
    NewtonSetThreadsCount(NWorld_, Count);
}
s32 PhysicsSystem::getThreadCount() const
{
    return NewtonGetThreadsCount(NWorld_);
}

void PhysicsSystem::setSolverModel(s32 Model)
{
    NewtonSetSolverModel(NWorld_, Model);
}

void PhysicsSystem::setContactCallback(PFNPHYSICSCONTACTCALLBACKPROC Callback)
{
    ContactCallback_ = Callback;
    
    s32 MaterialGroupID = NewtonMaterialGetDefaultGroupID(CurWorld_);
    
    if (ContactCallback_)
        NewtonMaterialSetCollisionCallback(CurWorld_, MaterialGroupID, MaterialGroupID, 0, 0, DefCallbackGenericContactProcess);
    else
        NewtonMaterialSetCollisionCallback(CurWorld_, MaterialGroupID, MaterialGroupID, 0, 0, 0);
}

PFNPHYSICSCONTACTCALLBACKPROC PhysicsSystem::getContactCallback()
{
    return ContactCallback_;
}


/*
 * ======= Physics callbacks =======
 */

void PhysicsTransformCallback(const NewtonBody* NBody, const f32* Transformation, s32 ThreadIndex)
{
    /* Get the rigid body object */
    RigidBody* PhysicsObject = (RigidBody*)NewtonBodyGetUserData(NBody);
    
    if (!PhysicsObject)
        return;
    
    scene::SceneNode* RootNode = PhysicsObject->getRootNode();
    
    if (!RootNode)
        return;
    
    /* Apply the transformation */
    const dim::matrix4f* Matrix = (const dim::matrix4f*)Transformation;
    
    RootNode->setPosition(Matrix->getPosition());
    RootNode->setRotationMatrix(Matrix->getRotationMatrix());
}

void PhysicsForceAndTorqueCallback(const NewtonBody* NBody, f32 TimeStep, s32 ThreadIndex)
{
    /* Get the rigid body object */
    RigidBody* PhysicsObject = (RigidBody*)NewtonBodyGetUserData(NBody);
    
    if (!PhysicsObject)
        return;
    
    /* Apply the forces */
    if (PhysicsObject->getGravityCallback())
        PhysicsObject->getGravityCallback()(PhysicsObject);
    else
        PhysicsObject->setForce(PhysicsObject->getGravity() * PhysicsObject->getMass());
}


} // /namespace physics

} // /namespace sp


#endif



// ================================================================================
