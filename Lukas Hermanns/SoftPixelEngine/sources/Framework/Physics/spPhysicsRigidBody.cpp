/*
 * Physics rigid body file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Framework/Physics/spPhysicsRigidBody.hpp"

#ifdef SP_COMPILE_WITH_NEWTONPHYSICS


#include "Framework/Physics/spPhysicsSystem.hpp"
#include "Base/spMemoryManagement.hpp"

#include <newton.h>


namespace sp
{
namespace physics
{


void PhysicsTransformCallback(const NewtonBody* NBody, const f32* Transformation, s32 ThreadIndex);
void PhysicsForceAndTorqueCallback(const NewtonBody* NBody, f32 TimeStep, s32 ThreadIndex);


RigidBody::RigidBody(const ERigidBodies Type, const SRigidBodyConstruction &Construct)
    : PhysicsBasicObject(), Type_(Type), Construct_(Construct), Mass_(25.0f), RootNode_(0), GravityCallback_(0)
{
    /* Create the specified rigid body type */
    switch (Type_)
    {
        case RIGIDBODY_BOX:             createBox               (Construct); break;
        case RIGIDBODY_CONE:            createCone              (Construct); break;
        case RIGIDBODY_SPHERE:          createSphere            (Construct); break;
        case RIGIDBODY_CYLINDER:        createCylinder          (Construct); break;
        case RIGIDBODY_CHAMFERCYLINDER: createChamferCylinder   (Construct); break;
        case RIGIDBODY_CAPSULE:         createCapsule           (Construct); break;
    }
}
RigidBody::RigidBody(scene::Mesh* MeshGeom)
    : PhysicsBasicObject(), Type_(RIGIDBODY_MESH), Mass_(25.0f), RootNode_(0), GravityCallback_(0)
{
    createMesh(MeshGeom);
    setRootNode(MeshGeom);
}
RigidBody::~RigidBody()
{
    MemoryManager::deleteList(JointList_);
}

void RigidBody::setRootNode(scene::SceneNode* Node)
{
    RootNode_ = Node;
}

void RigidBody::setMass(const f32 Mass)
{
    Mass_ = Mass;
    
    /* Compute the moment of inertia for all three axles */
    f32 Inertia[3], Origin[3];
    NewtonConvexCollisionCalculateInertialMatrix(NCollision_, Inertia, Origin);
    
    NewtonBodySetMassMatrix(NBody_, Mass_, Inertia[0] * Mass_, Inertia[1] * Mass_, Inertia[2] * Mass_);
}

void RigidBody::setAutoSleep(bool Enable)
{
    if (AutoSleep_ != Enable)
    {
        AutoSleep_ = Enable;
        NewtonBodySetFreezeState(NBody_, 0);
        NewtonBodySetAutoSleep(NBody_, 0);
    }
}

void RigidBody::setBalancePoint(const dim::vector3df &LocalPoint)
{
    NewtonBodySetCentreOfMass(NBody_, &LocalPoint.X);
}
dim::vector3df RigidBody::getBalancePoint() const
{
    dim::vector3df Center;
    NewtonBodyGetCentreOfMass(NBody_, &Center.X);
    return Center;
}

void RigidBody::addVelocity(const dim::vector3df &Direction)
{
    setVelocity(getVelocity() + Direction);
}
void RigidBody::setVelocity(const dim::vector3df &Direction)
{
    NewtonBodySetVelocity(NBody_, &Direction.X);
}
dim::vector3df RigidBody::getVelocity() const
{
    dim::vector3df Velocity;
    NewtonBodyGetVelocity(NBody_, &Velocity.X);
    return Velocity;
}

void RigidBody::addImpulse(const dim::vector3df &Direction, const dim::vector3df &PivotPoint)
{
    NewtonBodyAddImpulse(NBody_, &Direction.X, &PivotPoint.X);
}

void RigidBody::setForce(const dim::vector3df &Direction)
{
    NewtonBodySetForce(NBody_, &Direction.X);
}

BodyJoint* RigidBody::addJoint(
    PhysicsBasicObject* Child, const EBodyJoints Type, const dim::vector3df &PivotPoint,
    const dim::vector3df &Direction, const dim::vector3df &SecondDirection)
{
    BodyJoint* NewJoint = 0;
    
    if (Child)
        NewJoint = new BodyJoint(Type, this, Child, PivotPoint, Direction, SecondDirection);
    else
        NewJoint = new BodyJoint(Type, 0, this, PivotPoint, Direction, SecondDirection);
    
    JointList_.push_back(NewJoint);
    return NewJoint;
}
void RigidBody::removeJoint(BodyJoint* Object)
{
    MemoryManager::removeElement(JointList_, Object, true);
}
void RigidBody::removeJoint(PhysicsBasicObject* Child)
{
    for (std::list<BodyJoint*>::iterator it = JointList_.begin(); it != JointList_.end(); ++it)
    {
        if ((*it)->getChild() == Child)
        {
            MemoryManager::deleteMemory(*it);
            JointList_.erase(it);
            break;
        }
    }
}


/*
 * ======= Private: =======
 */

void RigidBody::createBox(const SRigidBodyConstruction &Construct)
{
    NCollision_ = NewtonCreateBox(
        PhysicsSystem::CurWorld_, Construct.Size.X*2, Construct.Size.Y*2, Construct.Size.Z*2, 0, 0
    );
    createDefaultBody();
}
void RigidBody::createCone(const SRigidBodyConstruction &Construct)
{
    NCollision_ = NewtonCreateCone(
        PhysicsSystem::CurWorld_, Construct.Radius, Construct.Height, 0, 0
    );
    createDefaultBody();
}
void RigidBody::createSphere(const SRigidBodyConstruction &Construct)
{
    NCollision_ = NewtonCreateSphere(
        PhysicsSystem::CurWorld_, Construct.Size.X, Construct.Size.Y, Construct.Size.Z, 0, 0
    );
    createDefaultBody();
}
void RigidBody::createCylinder(const SRigidBodyConstruction &Construct)
{
    NCollision_ = NewtonCreateCylinder(
        PhysicsSystem::CurWorld_, Construct.Radius, Construct.Height, 0, 0
    );
    createDefaultBody();
}
void RigidBody::createChamferCylinder(const SRigidBodyConstruction &Construct)
{
    NCollision_ = NewtonCreateChamferCylinder(
        PhysicsSystem::CurWorld_, Construct.Radius, Construct.Height, 0, 0
    );
    createDefaultBody();
}
void RigidBody::createCapsule(const SRigidBodyConstruction &Construct)
{
    NCollision_ = NewtonCreateCapsule(
        PhysicsSystem::CurWorld_, Construct.Radius, Construct.Height, 0, 0
    );
    createDefaultBody();
}

void RigidBody::createMesh(scene::Mesh* MeshGeom)
{
    static const f32 OptimizeTolerance = 0.002f;
    
    /* Temporary storage */
    u32 s, i, j = 0;
    
    const dim::matrix4f ScaleMatrix(MeshGeom->getScaleMatrix());
    
    /* Create the vertex coordinates buffer */
    const s32 VertexCount = MeshGeom->getVertexCount();
    f32* VertexBuffer = new f32[VertexCount*3];
    
    for (u32 s = 0; s < MeshGeom->getMeshBufferCount(); ++s)
    {
        video::MeshBuffer* Surface = MeshGeom->getMeshBuffer(s);
        
        for (i = 0; i < Surface->getVertexCount(); ++i, j += 3)
            *((dim::vector3df*)(VertexBuffer + j)) = ScaleMatrix * Surface->getVertexCoord(i);
    }
    
    /* Create the convex hull collision and delete the buffer */
    NCollision_ = NewtonCreateConvexHull(
        PhysicsSystem::CurWorld_, VertexCount, VertexBuffer, sizeof(f32)*3, OptimizeTolerance, 0, 0
    );
    
    delete [] VertexBuffer;
    
    /* Create the default physics body */
    createDefaultBody();
}

void RigidBody::createDefaultBody()
{
    if (!NCollision_)
    {
        io::Log::error("Could not create physics collision");
        return;
    }
    
    /* Create the physics body */
    NBody_ = NewtonCreateBody(PhysicsSystem::CurWorld_, NCollision_);
    
    if (!NBody_)
    {
        io::Log::error("Could not create physics body");
        return;
    }
    
    /* Disable by default auto sleep and freeze */
    NewtonBodySetFreezeState(NBody_, 0);
    NewtonBodySetAutoSleep(NBody_, 0);
    
    /* Setup the callback procedures for transformation, force and torque */
    NewtonBodySetTransformCallback(NBody_, PhysicsTransformCallback);
    NewtonBodySetForceAndTorqueCallback(NBody_, PhysicsForceAndTorqueCallback);
    
    /* Setup user data */
    NewtonBodySetUserData(NBody_, this);
    
    setGravity(dim::vector3df(0, -9.81f, 0));
}


} // /namespace physics

} // /namespace sp


#endif



// ================================================================================
