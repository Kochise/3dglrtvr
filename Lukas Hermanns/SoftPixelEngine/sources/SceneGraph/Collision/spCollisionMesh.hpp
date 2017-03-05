/*
 * Collision mesh header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_COLLISION_MESH_H__
#define __SP_COLLISION_MESH_H__


#include "Base/spStandard.hpp"
#include "Base/spTreeBuilder.hpp"
#include "SceneGraph/spSceneMesh.hpp"
#include "SceneGraph/Collision/spCollisionNode.hpp"


namespace sp
{
namespace scene
{


/**
CollisionMesh is one of the collision models and represents a complete mesh and has its own kd-Tree for fast collision detection.
Each kd-Tree node leaf stores a list of SCollisionTriangle instances. Thus modifying your mesh does not effect the collision model
after it has been already created.
*/
class SP_EXPORT CollisionMesh : public CollisionNode
{
    
    public:
        
        CollisionMesh(SceneNode* Node, scene::Mesh* Mesh, u8 MaxTreeLevel = 12);
        CollisionMesh(SceneNode* Node, const std::list<Mesh*> &MeshList, u8 MaxTreeLevel = 12);
        ~CollisionMesh();
        
        /* Functions */
        
        bool checkIntersection(const dim::line3df &Line, SCollisionMeshContact &Contact);
        bool checkCollision(CollisionSphere* Collider, SCollisionMeshContact &Contact);
        
    private:
        
        /* Functions */
        
        void createCollisionModel(const std::list<Mesh*> &MeshList, u8 MaxTreeLevel);
        
        /* Members */
        
        KDTreeNode* RootTreeNode_;
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
