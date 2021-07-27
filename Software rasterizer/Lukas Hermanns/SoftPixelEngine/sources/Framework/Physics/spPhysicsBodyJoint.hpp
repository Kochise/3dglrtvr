/*
 * Physics body joint header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_PHYSICS_BODYJOINT_H__
#define __SP_PHYSICS_BODYJOINT_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_NEWTONPHYSICS


#include "Base/spDimension.hpp"


class NewtonJoint;

namespace sp
{
namespace physics
{


class RigidBody;
class PhysicsBasicObject;

//! Rigid body joint types.
enum EBodyJoints
{
    JOINT_BALL,         //! Can be used for a rope.
    JOINT_HINGE,        //! Can be used for a door.
    JOINT_SLIDER,       //! Can be used for a sliding object.
    JOINT_CORKSCREW,    //! Can be used for a sliding and rotating object.
    JOINT_UNIVERSAL,    //! Can be used for a free rotating object.
};


/**
...
*/
class SP_EXPORT BodyJoint
{
    
    public:
        
        BodyJoint(
            const EBodyJoints Type, RigidBody* Parent, PhysicsBasicObject* Child,
            const dim::vector3df &PivotPoint, const dim::vector3df &Direction, const dim::vector3df &SecondDirection
        );
        ~BodyJoint();
        
        /* === Functions === */
        
        
        /* === Inline functions === */
        
        inline EBodyJoints getType() const
        {
            return Type_;
        }
        
        inline RigidBody* getParent() const
        {
            return Parent_;
        }
        inline PhysicsBasicObject* getChild() const
        {
            return Child_;
        }
        
    private:
        
        /* === Functions === */
        
        
        /* === Members === */
        
        EBodyJoints Type_;
        
        NewtonJoint* NJoint_;
        
        RigidBody* Parent_;
        PhysicsBasicObject* Child_;
        
};


} // /namespace physics

} // /namespace sp


#endif

#endif



// ================================================================================
