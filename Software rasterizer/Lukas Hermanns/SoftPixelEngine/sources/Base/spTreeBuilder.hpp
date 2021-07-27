/*
 * Tree builder header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_TREEBUILDER_H__
#define __SP_TREEBUILDER_H__


#include "Base/spStandard.hpp"
#include "Base/spTreeNode.hpp"
#include "Base/spTreeNodeBSP.hpp"
#include "Base/spTreeNodeQuad.hpp"
#include "Base/spTreeNodeOct.hpp"
#include "Base/spTreeNodeKD.hpp"
#include "Base/spMathTriangleCutter.hpp"
#include "SceneGraph/spSceneMesh.hpp"
#include "SceneGraph/Collision/spCollisionConfigTypes.hpp"


namespace sp
{
namespace scene
{


/**
This tree builder class builds or rather constructs all hierarchical trees.
It builds particular trees for collision detection purposes.
\note Building a tree for a mesh can modify your geometry object. Sometimes triangles
will be cut and divided into two parts.
*/
class SP_EXPORT TreeBuilder
{
    
    public:
        
        TreeBuilder();
        ~TreeBuilder();
        
        /* === Functions === */
        
        //static KDTreeNode* buildKdTree(CollisionStaticMesh* Object, s32 MaxLevel = 12); // <- use "struct SCollisionTriangle"
        //static QuadTreeNode* buildQuadTree(CollisionTerrain* Object, s32 MaxLevel = 6);
        
        static KDTreeNode* buildKdTree(Mesh* Object, u8 MaxLevel = 12);
        static OcTreeNode* buildOcTree(Mesh* Object, u8 MaxLevel = 4);
        static QuadTreeNode* buildQuadTree(Mesh* Object, u8 MaxLevel = 6);
        static BSPTreeNode* buildBSPTree(Mesh* Object, u8 MaxLevel = 12);
        
    private:
        
        /* === Functions === */
        
        static void buildKdTreeNode(
            KDTreeNode* Node, Mesh* Object, const std::list<SCollisionTriangle*> &Triangles,
            EKDTreeAxles Axis, f32 Distance, u8 MaxLevel
        );
        
        static void createKdTreeNodeChildren(
            KDTreeNode* Node, Mesh* Object, const std::list<SCollisionTriangle*> &Triangles,
            EKDTreeAxles Axis, f32 Distance, u8 MaxLevel
        );
        static void fillKdTreeNode(
            KDTreeNode* Node, Mesh* Object, const std::list<SCollisionTriangle*> &Triangles
        );
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
