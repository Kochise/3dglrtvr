/*
 * Collision mesh file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "SceneGraph/Collision/spCollisionMesh.hpp"
#include "SceneGraph/Collision/spCollisionSphere.hpp"

#include <list>


namespace sp
{
namespace scene
{


CollisionMesh::CollisionMesh(SceneNode* Node, scene::Mesh* Mesh, u8 MaxTreeLevel)
    : CollisionNode(Node, COLLISION_MESH), RootTreeNode_(0)
{
    std::list<scene::Mesh*> MeshList;
    MeshList.push_back(Mesh);
    createCollisionModel(MeshList, MaxTreeLevel);
}
CollisionMesh::CollisionMesh(SceneNode* Node, const std::list<Mesh*> &MeshList, u8 MaxTreeLevel)
    : CollisionNode(Node, COLLISION_MESH), RootTreeNode_(0)
{
    createCollisionModel(MeshList, MaxTreeLevel);
}
CollisionMesh::~CollisionMesh()
{
    MemoryManager::deleteMemory(RootTreeNode_);
}

bool CollisionMesh::checkIntersection(const dim::line3df &Line, SCollisionMeshContact &Contact)
{
    return false; //todo
}

bool CollisionMesh::checkCollision(CollisionSphere* Collider, SCollisionMeshContact &Contact)
{
    return false; //todo
}


/*
 * ======= Private: =======
 */

void CollisionMesh::createCollisionModel(const std::list<Mesh*> &MeshList, u8 MaxTreeLevel)
{
    //todo
}


} // /namespace scene

} // /namespace sp



// ================================================================================
