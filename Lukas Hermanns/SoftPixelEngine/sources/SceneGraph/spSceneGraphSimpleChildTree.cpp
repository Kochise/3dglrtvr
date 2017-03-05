/*
 * Simple child tree scene graph file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "SceneGraph/spSceneGraphSimple.hpp"

#ifdef SP_COMPILE_WITH_SCENEGRAPH_SIMPLE_CHILDTREE


#include "Platform/spSoftPixelDeviceOS.hpp"


namespace sp
{

extern video::RenderSystem* __spVideoDriver;

namespace scene
{


/*
 * Internal declerations
 */

bool cmpObjectSceneNodes(SceneNode* &obj1, SceneNode* &obj2);


/*
 * SceneGraphSimpleChildTree class
 */

SceneGraphSimpleChildTree::SceneGraphSimpleChildTree() : SceneGraph(SCENEGRAPH_SIMPLE_CHILDTREE)
{
    hasChildTree_ = true;
}
SceneGraphSimpleChildTree::~SceneGraphSimpleChildTree()
{
}

void SceneGraphSimpleChildTree::addSceneNode(SceneNode* Object)
{
    if (Object)
    {
        NodeList_.push_back(Object);
        RootNodeList_.push_back(Object);
    }
}
void SceneGraphSimpleChildTree::removeSceneNode(SceneNode* Object)
{
    if (Object)
    {
        removeObjectFromList<SceneNode, SceneNode>(Object, NodeList_);
        removeObjectFromList<SceneNode, SceneNode>(Object, RootNodeList_);
    }
}

void SceneGraphSimpleChildTree::addSceneNode(Camera* Object)
{
    if (Object)
    {
        CameraList_.push_back(Object);
        RootNodeList_.push_back(Object);
    }
}
void SceneGraphSimpleChildTree::removeSceneNode(Camera* Object)
{
    if (Object)
    {
        removeObjectFromList<Camera, Camera>(Object, CameraList_);
        removeObjectFromList<Camera, SceneNode>(Object, RootNodeList_);
    }
}

void SceneGraphSimpleChildTree::addSceneNode(Light* Object)
{
    if (Object)
    {
        LightList_.push_back(Object);
        RootNodeList_.push_back(Object);
    }
}
void SceneGraphSimpleChildTree::removeSceneNode(Light* Object)
{
    if (Object)
    {
        removeObjectFromList<Light, Light>(Object, LightList_);
        removeObjectFromList<Light, SceneNode>(Object, RootNodeList_);
    }
}

void SceneGraphSimpleChildTree::addSceneNode(RenderNode* Object)
{
    if (Object)
    {
        RenderList_.push_back(Object);
        RootNodeList_.push_back(Object);
    }
}
void SceneGraphSimpleChildTree::removeSceneNode(RenderNode* Object)
{
    if (Object)
    {
        removeObjectFromList<RenderNode, RenderNode>(Object, RenderList_);
        removeObjectFromList<RenderNode, SceneNode>(Object, RootNodeList_);
    }
}

void SceneGraphSimpleChildTree::addRootNode(SceneNode* Object)
{
    if (Object)
        RootNodeList_.push_back(Object);
}
void SceneGraphSimpleChildTree::removeRootNode(SceneNode* Object)
{
    removeObjectFromList<Node>(Object, RootNodeList_);
}

void SceneGraphSimpleChildTree::render()
{
    /* Update scene graph transformation */
    dim::matrix4f BaseMatrix = spWorldMatrix;
    updateModelviewMatrix();
    BaseMatrix *= spWorldMatrix;
    
    /* Update object transformation */
    for (std::list<SceneNode*>::iterator it = RootNodeList_.begin(); it != RootNodeList_.end(); ++it)
    {
        spWorldMatrix = BaseMatrix;
        updateRootNode(*it);
    }
    
    /* Render objects */
    RootNodeList_.sort(cmpObjectSceneNodes);
    
    for (std::list<SceneNode*>::iterator it = RootNodeList_.begin(); it != RootNodeList_.end(); ++it)
    {
        spWorldMatrix.reset();
        renderRootNode(*it);
    }
}


/*
 * ======= Protected: =======
 */

void SceneGraphSimpleChildTree::updateRootNode(SceneNode* Object)
{
    if (!Object->getVisible())
        return;
    
    Object->updateTransformation();
    
    for (std::list<SceneNode*>::const_iterator it = Object->getSceneChildren().begin(); it != Object->getSceneChildren().end(); ++it)
        updateRootNode(*it);
}

void SceneGraphSimpleChildTree::renderRootNode(SceneNode* Object)
{
    if (!Object->getVisible())
        return;
    
    /* Update individual object type */
    switch (Object->getType())
    {
        case NODE_MESH:
            setActiveMesh(static_cast<Mesh*>(Object));
        case NODE_BILLBOARD:
        case NODE_TERRAIN:
            static_cast<RenderNode*>(Object)->render();
            break;
            
        case NODE_LIGHT:
            static_cast<Light*>(Object)->render();
            break;
            
        default:
            Object->loadTransformation();
            break;
    }
    
    /* Render children */
    Object->getSceneChildren().sort(cmpObjectSceneNodes);
    
    for (std::list<SceneNode*>::const_iterator it = Object->getSceneChildren().begin(); it != Object->getSceneChildren().end(); ++it)
        renderRootNode(*it);
}


} // /namespace scene

} // /namespace sp


#endif








// ================================================================================
