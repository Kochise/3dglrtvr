/*
 * Physics basic object header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_PHYSICS_BASICOBJECT_H__
#define __SP_PHYSICS_BASICOBJECT_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_NEWTONPHYSICS


#include "Base/spDimension.hpp"
#include "Base/spBasicObject.hpp"


class NewtonCollision;
class NewtonBody;

namespace sp
{
namespace physics
{


class SP_EXPORT PhysicsBasicObject : public BasicObject
{
    
    public:
        
        virtual ~PhysicsBasicObject();
        
        /* Functions */
        
        void setTransformation(const dim::matrix4f &Transformation);
        dim::matrix4f getTransformation() const;
        
        void setPosition(const dim::vector3df &Position);
        dim::vector3df getPosition() const;
        
        void setRotation(const dim::matrix4f &Rotation);
        dim::matrix4f getRotation() const;
        
    protected:
        
        friend class BodyJoint;
        
        /* Functions */
        
        PhysicsBasicObject();
        
        /* Members */
        
        NewtonCollision* NCollision_;
        NewtonBody* NBody_;
        
};


} // /namespace physics

} // /namespace sp


#endif

#endif



// ================================================================================
