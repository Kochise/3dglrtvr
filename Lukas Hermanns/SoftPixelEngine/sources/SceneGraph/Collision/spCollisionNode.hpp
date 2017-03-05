/*
 * Collision node header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_COLLISION_NODE_H__
#define __SP_COLLISION_NODE_H__


#include "Base/spStandard.hpp"
#include "Base/spDimension.hpp"
#include "Base/spMathCollisionLibrary.hpp"
#include "SceneGraph/spSceneNode.hpp"
#include "SceneGraph/Collision/spCollisionConfigTypes.hpp"


namespace sp
{
namespace scene
{


/**
CollisionNode is the root class for all collision models which are sphere, box, mesh and terrain.
Its particular an interface for derived collision models.
*/
class SP_EXPORT CollisionNode
{
    
    public:
        
        virtual ~CollisionNode();
        
        /* Functions */
        
        //! Checks for an intersection between this collision object and the given line and stores the result in the specified contact structure.
        virtual bool checkIntersection(const dim::line3df &Line, SCollisionNodeContact &Contact);
        
        /**
        Checks for an intersection between this collision object and the given line and stores the result in the specified contact structure.
        \note This is used for collision meshes.
        */
        virtual bool checkIntersection(const dim::line3df &Line, SCollisionMeshContact &Contact);
        
        /**
        Checks for a collision between this collision object and the collider. When you want to perform collision resolving
        you have to set the new position by your self.
        */
        virtual bool checkCollision(CollisionSphere* Collider, SCollisionNodeContact &Contact);
        
        /**
        Checks for a collision between this collision object and the collider. When you want to perform collision resolving
        you have to set the new position by your self.
        \note This is used for collision meshes.
        */
        virtual bool checkCollision(CollisionSphere* Collider, SCollisionMeshContact &Contact);
        
        /* Inline functions */
        
        inline ECollisionModels getType() const
        {
            return Type_;
        }
        inline SceneNode* getNode() const
        {
            return Node_;
        }
        
        //! Returns the global position of the scene node.
        inline dim::vector3df getPosition() const
        {
            return Node_->getPosition(true);
        }
        //! Returns the global rotation of the scene node.
        inline dim::matrix4f getRotation() const
        {
            return Node_->getRotationMatrix(true);
        }
        //! Returns the global scaling of the scene node.
        inline dim::vector3df getScale() const
        {
            return Node_->getScale(true);
        }
        
        inline dim::matrix4f getTransformation() const
        {
            return Node_->getGlobalLocation();
        }
        inline dim::matrix4f getInverseTransformation() const
        {
            return Node_->getGlobalLocationInverse();
        }
        
    protected:
        
        /* Functions */
        
        CollisionNode(SceneNode* Node, const ECollisionModels Type);
        
        /* Members */
        
        ECollisionModels Type_; //!< Collision type (or rather model).
        SceneNode* Node_;  //!< Scene node object.
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
