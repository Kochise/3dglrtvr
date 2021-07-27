/*
 * Tree node file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Base/spTreeNode.hpp"


namespace sp
{
namespace scene
{


const f32 TreeNode::EXT_BOUNDBOX_SIZE = 0.01f;

TreeNode::TreeNode(TreeNode* Parent, const ETreeNodeTypes Type)
    : Type_(Type), Parent_(Parent), pDestructorCallbackProc_(0)
{
}
TreeNode::~TreeNode()
{
    if (pDestructorCallbackProc_)
        pDestructorCallbackProc_(this);
    removeChildren();
}

TreeNode* TreeNode::getRoot()
{
    if (Parent_)
        return Parent_->getRoot();
    return this;
}

u32 TreeNode::getLevel() const
{
    if (Parent_)
        return 1 + Parent_->getLevel();
    return 0;
}

void TreeNode::removeChildren()
{
}


} // /namespace scene

} // /namespace sp



// ================================================================================
