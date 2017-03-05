/*
 * Collision box file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "SceneGraph/Collision/spCollisionBox.hpp"
#include "SceneGraph/Collision/spCollisionSphere.hpp"


namespace sp
{
namespace scene
{


CollisionBox::CollisionBox(SceneNode* Node, const dim::aabbox3df &Box)
    : CollisionNode(Node, COLLISION_BOX), Box_(Box)
{
}
CollisionBox::~CollisionBox()
{
}

bool CollisionBox::checkIntersection(const dim::line3df &Line, SCollisionNodeContact &Contact)
{
    /* Store transformations */
    const dim::vector3df Pos(getPosition());
    const dim::vector3df Scl(getScale());
    
    const dim::matrix4f Mat(getTransformation());
    const dim::matrix4f InvMat(Mat.getInverse());
    
    const dim::line3df InvLine(InvMat * Line.Start, InvMat * Line.End);
    
    /* Make an intersection test with the inverse-transformed line and the aab-box */
    if (math::CollisionLibrary::checkLineBoxIntersection(InvLine, Box_, Contact.Point))
    {
        Contact.Object = this;
        
        /* Calculate normal and final contact point */
             if (Contact.Point.X > Box_.Max.X) Contact.Normal = dim::vector3df( 1,  0,  0);
        else if (Contact.Point.X < Box_.Min.X) Contact.Normal = dim::vector3df(-1,  0,  0);
        else if (Contact.Point.Y > Box_.Max.Y) Contact.Normal = dim::vector3df( 0,  1,  0);
        else if (Contact.Point.Y < Box_.Min.Y) Contact.Normal = dim::vector3df( 0, -1,  0);
        else if (Contact.Point.Z > Box_.Max.Z) Contact.Normal = dim::vector3df( 0,  0,  1);
        else                                   Contact.Normal = dim::vector3df( 0,  0, -1);
        
        Contact.Point   = Mat * Contact.Point;
        Contact.Normal  = (Mat.getRotationMatrix() * Contact.Normal).normalize();
        
        return true;
    }
    
    return false;
}

bool CollisionBox::checkCollision(CollisionSphere* Collider, SCollisionNodeContact &Contact)
{
    if (!Collider)
        return false;
    
    /* Store transformation */
    const dim::matrix4f Mat(getTransformation().getPositionRotationMatrix());
    const dim::matrix4f InvMat(Mat.getInverse());
    
    const dim::aabbox3df Box(Box_.getScaled(getScale()));
    const dim::vector3df OtherPos(Collider->getPosition());
    const dim::vector3df OtherInvPos(InvMat * OtherPos);
    
    /* Get the closest point from this box and the collider */
    const dim::vector3df Point = math::CollisionLibrary::getClosestPoint(Box, OtherInvPos);
    
    /* Check if this object and the other collide with each other */
    if (math::getDistanceSq(Point, OtherInvPos) < math::Pow2(Collider->getRadius()))
    {
        Contact.Object  = this;
        Contact.Normal  = (OtherPos - Point).normalize();
        Contact.Point   = Mat * Point;
        return true;
    }
    
    return false;
}


} // /namespace scene

} // /namespace sp



// ================================================================================
