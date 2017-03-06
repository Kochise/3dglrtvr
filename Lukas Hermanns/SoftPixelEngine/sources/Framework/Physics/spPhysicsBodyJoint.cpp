/*
 * Physics body joint file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Framework/Physics/spPhysicsBodyJoint.hpp"

#ifdef SP_COMPILE_WITH_NEWTONPHYSICS


#include "Framework/Physics/spPhysicsSystem.hpp"

#include <newton.h>


namespace sp
{
namespace physics
{


BodyJoint::BodyJoint(
    const EBodyJoints Type, RigidBody* Parent, PhysicsBasicObject* Child, const dim::vector3df &PivotPoint,
    const dim::vector3df &Direction, const dim::vector3df &SecondDirection)
    : Type_(Type), NJoint_(0), Parent_(Parent), Child_(Child)
{
    NewtonBody* ParentBody = (Parent ? Parent->NBody_ : 0);
    
    switch (Type_)
    {
        case JOINT_BALL:
            NJoint_ = NewtonConstraintCreateBall(PhysicsSystem::CurWorld_, &PivotPoint.X, Child->NBody_, ParentBody); break;
        case JOINT_HINGE:
            NJoint_ = NewtonConstraintCreateHinge(PhysicsSystem::CurWorld_, &PivotPoint.X, &Direction.X, Child->NBody_, ParentBody); break;
        case JOINT_SLIDER:
            NJoint_ = NewtonConstraintCreateSlider(PhysicsSystem::CurWorld_, &PivotPoint.X, &Direction.X, Child->NBody_, ParentBody); break;
        case JOINT_CORKSCREW:
            NJoint_ = NewtonConstraintCreateCorkscrew(PhysicsSystem::CurWorld_, &PivotPoint.X, &Direction.X, Child->NBody_, ParentBody); break;
        case JOINT_UNIVERSAL:
            NJoint_ = NewtonConstraintCreateUniversal(PhysicsSystem::CurWorld_, &PivotPoint.X, &Direction.X, &SecondDirection.X, Child->NBody_, ParentBody); break;
    }
}
BodyJoint::~BodyJoint()
{
    if (NJoint_)
        NewtonDestroyJoint(PhysicsSystem::CurWorld_, NJoint_);
}


} // /namespace physics

} // /namespace sp


#endif



// ================================================================================
