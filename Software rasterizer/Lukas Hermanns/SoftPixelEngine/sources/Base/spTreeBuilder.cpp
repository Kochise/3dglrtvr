/*
 * Tree builder file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Base/spTreeBuilder.hpp"


namespace sp
{
namespace scene
{


/*
 * Internal callback procedures
 */

static void spDestructCollisionTriangle(TreeNode* Node)
{
    if (Node->getUserData())
        delete static_cast<std::list<SCollisionTriangle>*>(Node->getUserData());
}


/*
 * TreeBuilder class
 */

TreeBuilder::TreeBuilder()
{
}
TreeBuilder::~TreeBuilder()
{
}

KDTreeNode* TreeBuilder::buildKdTree(Mesh* Object, u8 MaxLevel)
{
    if (!Object || !Object->getTriangleCount() || !MaxLevel)
        return 0;
    
    /* Get triangle data */
    std::list<SCollisionTriangle*> Triangles;
    SCollisionTriangle* NewTriangle = 0;
    video::MeshBuffer* Surface = 0;
    
    for (u32 s = 0; s < Object->getMeshBufferCount(); ++s)
    {
        Surface = Object->getMeshBuffer(s);
        
        for (u32 i = 0; i < Surface->getTriangleCount(); ++i)
        {
            NewTriangle = new SCollisionTriangle;
            {
                NewTriangle->Mesh           = Object;
                NewTriangle->Surface        = Surface;
                NewTriangle->TriangleIndex  = i;
                NewTriangle->Triangle       = Surface->getTriangleCoords(i);
            }
            Triangles.push_back(NewTriangle);
        }
    }
    
    /* Create tree root node */
    KDTreeNode* RootNode = new KDTreeNode();
    
    RootNode->setBox(Object->getMeshBoundingBox());
    buildKdTreeNode(RootNode, Object, Triangles, KDTREE_XAXIS, 0.0f, MaxLevel);
    
    /* Delete triangle data */
    MemoryManager::deleteList(Triangles);
    
    return RootNode;
}

OcTreeNode* TreeBuilder::buildOcTree(Mesh* Object, u8 MaxLevel)
{
    return 0; // todo
}

QuadTreeNode* TreeBuilder::buildQuadTree(Mesh* Object, u8 MaxLevel)
{
    return 0; // todo
}

BSPTreeNode* TreeBuilder::buildBSPTree(Mesh* Object, u8 MaxLevel)
{
    return 0; // todo
}


/*
 * ======= Private: =======
 */

void TreeBuilder::buildKdTreeNode(
    KDTreeNode* Node, Mesh* Object, const std::list<SCollisionTriangle*> &Triangles, EKDTreeAxles Axis, f32 Distance, u8 MaxLevel)
{
    Node->setDestructorCallback(spDestructCollisionTriangle);
    
    if (MaxLevel > 0)
        createKdTreeNodeChildren(Node, Object, Triangles, Axis, Distance, MaxLevel);
    else
        fillKdTreeNode(Node, Object, Triangles);
}

void TreeBuilder::createKdTreeNodeChildren(
    KDTreeNode* Node, Mesh* Object, const std::list<SCollisionTriangle*> &Triangles, EKDTreeAxles Axis, f32 Distance, u8 MaxLevel)
{
    std::list<SCollisionTriangle*> TrianglesNear, TrianglesFar;
    dim::vector3df AverageTrianglesCenter;
    
    /* Compute the distance by a uniform distribution of the triangles */
    for (std::list<SCollisionTriangle*>::const_iterator it = Triangles.begin(); it != Triangles.end(); ++it)
        AverageTrianglesCenter += (*it)->Triangle.getCenter();
    
    AverageTrianglesCenter /= Triangles.size();
    
    /* Setup axis and distance */
    switch (Axis)
    {
        case KDTREE_XAXIS: Axis = KDTREE_YAXIS; break;
        case KDTREE_YAXIS: Axis = KDTREE_ZAXIS; break;
        case KDTREE_ZAXIS: Axis = KDTREE_XAXIS; break;
    }
    
    Distance = AverageTrianglesCenter[Axis];
    
    Node->setAxis(Axis);
    Node->setDistance(Distance);
    
    /* Fill triangle data */
    if (Triangles.size() > 1)
    {
        for (std::list<SCollisionTriangle*>::const_iterator it = Triangles.begin(); it != Triangles.end(); ++it)
        {
            if ((*it)->Triangle.PointA[Axis] <= Distance || (*it)->Triangle.PointB[Axis] <= Distance || (*it)->Triangle.PointC[Axis] <= Distance)
                TrianglesNear.push_back(*it);
            if ((*it)->Triangle.PointA[Axis] >= Distance || (*it)->Triangle.PointB[Axis] >= Distance || (*it)->Triangle.PointC[Axis] >= Distance)
                TrianglesFar.push_back(*it);
        }
    }
    else
        TrianglesNear.push_back(*Triangles.begin());
    
    /* Create children tree nodes */
    Node->addChildren();
    
    if (!TrianglesNear.empty())
        buildKdTreeNode(Node->getChildNear(), Object, TrianglesNear, Axis, Distance, MaxLevel - 1);
    if (!TrianglesFar.empty())
        buildKdTreeNode(Node->getChildFar(), Object, TrianglesFar, Axis, Distance, MaxLevel - 1);
}

void TreeBuilder::fillKdTreeNode(
    KDTreeNode* Node, Mesh* Object, const std::list<SCollisionTriangle*> &Triangles)
{
    /* Fill in geometry data */
    std::list<dim::triangle3df> CutList;
    
    SCollisionTriangle* NewTriangle = 0;
    std::list<SCollisionTriangle>* TriangleList = new std::list<SCollisionTriangle>;
    Node->setUserData(TriangleList);
    
    for (std::list<SCollisionTriangle*>::const_iterator it = Triangles.begin(); it != Triangles.end(); ++it)
    {
        /* Check if the triangle lies complete inside the tree-node box */
        if (Node->getBox().isPointInside((*it)->Triangle.PointA) &&
            Node->getBox().isPointInside((*it)->Triangle.PointB) &&
            Node->getBox().isPointInside((*it)->Triangle.PointC))
        {
            /* Add current triangle unmodified */
            TriangleList->resize(TriangleList->size() + 1);
            TriangleList->back() = **it;
            
            continue;
        }
        
        /* Check for the cut with the current triangle and the tree-node box */
        math::TriangleCutter::cutTriangle((*it)->Triangle, Node->getBox(), CutList);
        
        for (std::list<dim::triangle3df>::iterator itTri = CutList.begin(); itTri != CutList.end(); ++itTri)
        {
            TriangleList->resize(TriangleList->size() + 1);
            NewTriangle = &TriangleList->back();
            
            NewTriangle->Mesh           = Object;
            NewTriangle->Surface        = (*it)->Surface;
            NewTriangle->TriangleIndex  = (*it)->TriangleIndex;
            NewTriangle->Triangle       = *itTri;
        }
        
        CutList.clear();
    }
    
    /* ReCreate mesh geometry */
    //for (std::list<SCollisionTriangle*>::const_iterator it = Triangles.begin(); it != Triangles.end(); ++it)
    //    (*it)->Surface->cutTriangle((*it)->Index, ClipPlane);
}


} // /namespace scene

} // /namespace sp



// ================================================================================
