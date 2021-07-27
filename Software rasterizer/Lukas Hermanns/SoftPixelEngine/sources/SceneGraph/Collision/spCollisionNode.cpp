/*
 * Collision node file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "SceneGraph/Collision/spCollisionNode.hpp"
#include "SceneGraph/Collision/spCollisionSphere.hpp"


namespace sp
{
namespace scene
{


CollisionNode::CollisionNode(SceneNode* Node, const ECollisionModels Type)
    : Node_(Node), Type_(Type)
{
    if (!Node_)
        io::Log::error("Collision node must be linked to a valid scene node");
}
CollisionNode::~CollisionNode()
{
}

bool CollisionNode::checkIntersection(const dim::line3df &Line, SCollisionNodeContact &Contact)
{
    return false; // do nothing
}
bool CollisionNode::checkIntersection(const dim::line3df &Line, SCollisionMeshContact &Contact)
{
    return false; // do nothing
}
bool CollisionNode::checkCollision(CollisionSphere* Collider, SCollisionNodeContact &Contact)
{
    return false; // do nothing
}
bool CollisionNode::checkCollision(CollisionSphere* Collider, SCollisionMeshContact &Contact)
{
    return false; // do nothing
}


} // /namespace scene

} // /namespace sp



// ================================================================================
