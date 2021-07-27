/*
 * Collision sphere file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "SceneGraph/Collision/spCollisionSphere.hpp"


namespace sp
{
namespace scene
{


CollisionSphere::CollisionSphere(SceneNode* Node, f32 Radius)
    : CollisionNode(Node, COLLISION_SPHERE), Radius_(Radius)
{
}
CollisionSphere::~CollisionSphere()
{
}

bool CollisionSphere::checkIntersection(const dim::line3df &Line, SCollisionNodeContact &Contact)
{
    const dim::vector3df Pos(getPosition());
    
    /* Make an intersection test with the line and this sphere */
    if (math::CollisionLibrary::checkLineSphereIntersection(Line, Pos, Radius_, Contact.Point))
    {
        Contact.Object = this;
        Contact.Normal = (Contact.Point - Pos).normalize();
        return true;
    }
    
    return false;
}

bool CollisionSphere::checkCollision(CollisionSphere* Collider, SCollisionNodeContact &Contact)
{
    if (!Collider)
        return false;
    
    /* Store transformation */
    const dim::vector3df Pos(getPosition());
    const dim::vector3df OtherPos(Collider->getPosition());
    
    /* Check if this object and the other collide with each other */
    if (math::getDistanceSq(Pos, OtherPos) < math::Pow2(Radius_ + Collider->getRadius()))
    {
        Contact.Object  = this;
        Contact.Normal  = (OtherPos - Pos).normalize();
        Contact.Point   = Pos + Contact.Normal;
        return true;
    }
    
    return false;
}


} // /namespace scene

} // /namespace sp



// ================================================================================
