/*
 * Material node file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "SceneGraph/spMaterialNode.hpp"
#include "Platform/spSoftPixelDeviceOS.hpp"


namespace sp
{

extern video::RenderSystem* __spVideoDriver;

namespace scene
{


MaterialNode::MaterialNode(const ENodeTypes Type)
    : RenderNode(Type), Material_(0), ShaderObject_(0), EnableMaterial_(true)
{
    Material_ = new video::MaterialStates();
}
MaterialNode::~MaterialNode()
{
    MemoryManager::deleteMemory(Material_);
}

void MaterialNode::setMaterial(const video::MaterialStates* Material)
{
    Material_->copy(Material);
    /*if (Material)
    {
        MemoryManager::deleteMemory(Material_);
        Material_ = Material;
    }
    else
        Material_ = new video::MaterialStates();*/
}

bool MaterialNode::compare(MaterialNode* other)
{
    /* Compare order */
    if (Order_ != other->Order_)
        return Order_ > other->Order_;
    
    /* Compare material alpha channel */
    if (Material_->getDiffuseColor().Alpha != other->Material_->getDiffuseColor().Alpha)
        return Material_->getDiffuseColor().Alpha > other->Material_->getDiffuseColor().Alpha;
    
    /* Compare blending */
    if (Material_->getBlendTarget() != other->Material_->getBlendTarget())
        return Material_->getBlendTarget() > other->Material_->getBlendTarget();
    
    /* Compare depth distance */
    return DepthDistance_ > other->DepthDistance_;
}


} // /namespace scene

} // /namespace sp



// ================================================================================
