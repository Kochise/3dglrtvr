/*
 * Physics basic object file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Framework/Physics/spPhysicsBasicObject.hpp"

#ifdef SP_COMPILE_WITH_NEWTONPHYSICS


#include "Framework/Physics/spPhysicsSystem.hpp"

#include <newton.h>


namespace sp
{
namespace physics
{


PhysicsBasicObject::PhysicsBasicObject() : NCollision_(0), NBody_(0)
{
}
PhysicsBasicObject::~PhysicsBasicObject()
{
    if (NCollision_)
        NewtonReleaseCollision(PhysicsSystem::CurWorld_, NCollision_);
    if (NBody_)
        NewtonDestroyBody(PhysicsSystem::CurWorld_, NBody_);
}

void PhysicsBasicObject::setTransformation(const dim::matrix4f &Transformation)
{
    NewtonBodySetMatrix(NBody_, Transformation.getPositionRotationMatrix().getArray());
}
dim::matrix4f PhysicsBasicObject::getTransformation() const
{
    dim::matrix4f Transformation;
    NewtonBodyGetMatrix(NBody_, Transformation.getArray());
    return Transformation;
}

void PhysicsBasicObject::setPosition(const dim::vector3df &Position)
{
    dim::matrix4f Transformation(getTransformation());
    Transformation.setPosition(Position);
    setTransformation(Transformation);
}
dim::vector3df PhysicsBasicObject::getPosition() const
{
    return getTransformation().getPosition();
}

void PhysicsBasicObject::setRotation(const dim::matrix4f &Rotation)
{
    const dim::vector3df Position(getPosition());
    dim::matrix4f Transformation(Rotation);
    Transformation.setPosition(Position);
    setTransformation(Transformation);
}
dim::matrix4f PhysicsBasicObject::getRotation() const
{
    return getTransformation().getRotationMatrix();
}


} // /namespace physics

} // /namespace sp


#endif



// ================================================================================
