/*
 * Collision sphere header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_COLLISION_SPHERE_H__
#define __SP_COLLISION_SPHERE_H__


#include "Base/spStandard.hpp"
#include "SceneGraph/Collision/spCollisionNode.hpp"


namespace sp
{
namespace scene
{


/**
CollisionSphere is one of the collision models and represents a perfect sphere with only a position and a radius.
Rotations and scaling does not effect the collision model.
*/
class SP_EXPORT CollisionSphere : public CollisionNode
{
    
    public:
        
        CollisionSphere(SceneNode* Node, f32 Radius);
        ~CollisionSphere();
        
        /* Functions */
        
        bool checkIntersection(const dim::line3df &Line, SCollisionNodeContact &Contact);
        bool checkCollision(CollisionSphere* Collider, SCollisionNodeContact &Contact);
        
        /* Inline functions */
        
        inline void setRadius(f32 Radius)
        {
            Radius_ = Radius;
        }
        inline f32 getRadius() const
        {
            return Radius_;
        }
        
    private:
        
        /* Members */
        
        f32 Radius_;
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
