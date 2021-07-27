/*
 * Tree node KD file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Base/spTreeNodeKD.hpp"


namespace sp
{
namespace scene
{


KDTreeNode::KDTreeNode(TreeNode* Parent)
    : TreeNode(Parent, TREENODE_KDTREE), ChildNear_(0), ChildFar_(0)
{
    Axis_       = KDTREE_XAXIS;
    Distance_   = 0.0f;
}
KDTreeNode::~KDTreeNode()
{
}

bool KDTreeNode::isLeafNode() const
{
    return !ChildNear_;
}

void KDTreeNode::addChildren()
{
    if (!ChildNear_)
    {
        ChildNear_  = new KDTreeNode(this);
        ChildFar_   = new KDTreeNode(this);
        
        setupBox();
    }
}
void KDTreeNode::addChildren(const EKDTreeAxles Axis, f32 Distance)
{
    addChildren();
    
    Axis_       = Axis;
    Distance_   = Distance;
}

void KDTreeNode::removeChildren()
{
    MemoryManager::deleteMemory(ChildNear_);
    MemoryManager::deleteMemory(ChildFar_);
}

const TreeNode* KDTreeNode::findTreeNode(const dim::vector3df &Point) const
{
    if (Box_.isPointInside(Point))
    {
        if (ChildNear_)
            return (Point[Axis_] < Distance_) ? ChildNear_->findTreeNode(Point) : ChildFar_->findTreeNode(Point);
        return this;
    }
    return 0;
}


/*
 * ======= Private: =======
 */

void KDTreeNode::setupBox()
{
    dim::vector3df NearMax, FarMin;
    
    switch (Axis_)
    {
        case KDTREE_XAXIS:
            NearMax = dim::vector3df(Distance_, Box_.Max.Y, Box_.Max.Z);
            FarMin  = dim::vector3df(Distance_, Box_.Min.Y, Box_.Min.Z);
            break;
        case KDTREE_YAXIS:
            NearMax = dim::vector3df(Box_.Max.X, Distance_, Box_.Max.Z);
            FarMin  = dim::vector3df(Box_.Min.X, Distance_, Box_.Min.Z);
            break;
        case KDTREE_ZAXIS:
            NearMax = dim::vector3df(Box_.Max.X, Box_.Max.Y, Distance_);
            FarMin  = dim::vector3df(Box_.Min.X, Box_.Min.Y, Distance_);
            break;
    }
    
    ChildNear_->Box_ = dim::aabbox3df(Box_.Min, NearMax);
    ChildFar_->Box_ = dim::aabbox3df(FarMin, Box_.Max);
}


} // /namespace scene

} // /namespace sp



// ================================================================================
