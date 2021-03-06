/*
 * Collision sphere header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_COLLISION_BOX_H__
#define __SP_COLLISION_BOX_H__


#include "Base/spStandard.hpp"
#include "SceneGraph/Collision/spCollisionNode.hpp"


namespace sp
{
namespace scene
{


/**
CollisionBox is one of the collision models and represents an axis-aligned-bounding box which is - although it is axis-aligned - rotatable.
Rotation is anyway possible because of inverse rotations inside the collision functions.
*/
class SP_EXPORT CollisionBox : public CollisionNode
{
    
    public:
        
        CollisionBox(SceneNode* Node, const dim::aabbox3df &Box);
        ~CollisionBox();
        
        /* Functions */
        
        bool checkIntersection(const dim::line3df &Line, SCollisionNodeContact &Contact);
        bool checkCollision(CollisionSphere* Collider, SCollisionNodeContact &Contact);
        
        /* Inline functions */
        
        inline void setBox(const dim::aabbox3df &Box)
        {
            Box_ = Box;
        }
        inline dim::aabbox3df getBox() const
        {
            return Box_;
        }
        
    private:
        
        /* Members */
        
        dim::aabbox3df Box_;
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
