/*
 * Scene node header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_SCENE_SCENENODE_H__
#define __SP_SCENE_SCENENODE_H__


#include "Base/spStandard.hpp"
#include "Base/spDimension.hpp"
#include "Base/spInputOutputLog.hpp"
#include "Base/spMaterialStates.hpp"
#include "Base/spGeometryStructures.hpp"
#include "Base/spAnimation.hpp"
#include "Base/spNode.hpp"
#include "Base/spMath.hpp"
#include "RenderSystem/spShaderProgram.hpp"

#include <list>


namespace sp
{
namespace scene
{


/*
 * Global members
 */

extern dim::matrix4f spProjectionMatrix;
extern dim::matrix4f spViewMatrix;
extern dim::matrix4f spViewInvMatrix;
extern dim::matrix4f spWorldMatrix;
extern dim::matrix4f spTextureMatrix[MAX_COUNT_OF_TEXTURES];
extern dim::matrix4f spColorMatrix;


/* 
 * Enumerations
 */

enum ENodeTypes
{
    NODE_BASICNODE,     // Basic scene node
    NODE_CUSTOM,        // Custom scene node
    NODE_SCENEGRAPH,    // Scene graph tree
    NODE_CAMERA,        // Camera
    NODE_LIGHT,         // Light
    NODE_MESH,          // 3D object/ mesh
    NODE_BILLBOARD,     // Billboard/ particle/ sprite
    NODE_TERRAIN,       // Terrains
};

enum EBoundingVolumes
{
    BOUNDING_NONE,
    BOUNDING_SPHERE,
    BOUNDING_BOX,
};


/**
Nodes are the root of each object. This is the parent class of Entity, Camera, Light, Sprite and Terrain objects.
Here you can find all the functions for locating the object in position, rotation and scaling. Some basic
information about visibility, user data (void*) etc. are implemented in this class, too.
*/
class SP_EXPORT SceneNode : public Node
{
    
    public:
        
        /* === Header === */
        
        SceneNode(const ENodeTypes Type);
        virtual ~SceneNode();
        
        /* === Matrix transformations === */
        
        virtual inline void setPositionMatrix(const dim::matrix4f &Position)
        {
            Position_ = Position;
        }
        virtual inline dim::matrix4f getPositionMatrix() const
        {
            return Position_;
        }
        
        virtual inline void setRotationMatrix(const dim::matrix4f &Rotation)
        {
            Rotation_ = Rotation;
        }
        virtual inline dim::matrix4f getRotationMatrix() const
        {
            return Rotation_;
        }
        
        virtual inline void setScaleMatrix(const dim::matrix4f &Scale)
        {
            Scale_ = Scale;
        }
        virtual inline dim::matrix4f getScaleMatrix() const
        {
            return Scale_;
        }
        
        /**
        Sets the object's position matrix.
        \param Position: Matrix which is to be used for the position transformation.
        \param isGlobal: Only useful when the object is a child of an other.
        If so and "isGlobal" is true the transformation will be processed global.
        */
        virtual void setPositionMatrix(const dim::matrix4f &Position, bool isGlobal);
        virtual dim::matrix4f getPositionMatrix(bool isGlobal) const;
        
        virtual void setRotationMatrix(const dim::matrix4f &Rotation, bool isGlobal);
        virtual dim::matrix4f getRotationMatrix(bool isGlobal) const;
        
        virtual void setScaleMatrix(const dim::matrix4f &Scale, bool isGlobal);
        virtual dim::matrix4f getScaleMatrix(bool isGlobal) const;
        
        
        virtual void setPosition(const dim::vector3df &Position, bool isGlobal = false);
        virtual dim::vector3df getPosition(bool isGlobal = false) const;
        
        /**
        Sets the object's rotation. A typically rotation in the engine is performed by
        the follwoing combination: Y, X, Z. This is the order in which the rotation axles are
        processed. If you want to perform an individual rotation use "setRotationMatrix"
        with your own matrix transformation.
        */
        virtual void setRotation(const dim::vector3df &Rotation, bool isGlobal = false);
        virtual dim::vector3df getRotation(bool isGlobal = false) const;
        
        virtual void setScale(const dim::vector3df &Scale, bool isGlobal = false);
        virtual dim::vector3df getScale(bool isGlobal = false) const;
        
        /* === Summarized matrix transformations === */
        
        virtual void setMatrix(const dim::matrix4f &Matrix);
        virtual dim::matrix4f getMatrix() const;
        
        virtual void lookAt(const dim::vector3df &Position, bool isGlobal = false);
        
        virtual inline dim::vector3df getDirection(const dim::vector3df &upVector = dim::vector3df(0, 0, 1)) const
        {
            return Rotation_ * upVector;
        }
        
        /* === Movement === */
        
        //! Moves the object in the specified direction. This is dependent on the current rotation transformation.
        virtual void move(const dim::vector3df &Direction);
        
        //! Turns the object with the specified rotation.
        virtual void turn(const dim::vector3df &Rotation);
        
        //! Moves the object in the specified direction. This is independent on the current rotation transformation.
        virtual void translate(const dim::vector3df &Direction);
        
        //! Transforms the object with the specified size.
        virtual void transform(const dim::vector3df &Size);
        
        /* === Animation === */
        
        /**
        Creats a new animation for this object.
        \param Type: Type of animation (node-, morph-target- or skeletal animation).
        \param Name: Name of animation (by default empty).
        \return Pointer to the new Animation object. To have the full control about the animation use
        casting to get the real class object:
        e.g. "AnimationSkeletal* BoneAnim = (AnimationSkeletal*)Obj->addAnimation(ANIMATION_SKELETAL);"
        */
        virtual Animation* addAnimation(const EAnimationTypes Type, const io::stringc &Name = "");
        virtual void removeAnimation(Animation* Anim);
        virtual void clearAnimations();
        
        virtual Animation* getAnimation(const io::stringc Name) const;
        virtual Animation* getAnimation(u32 Index = 0) const;
        
        inline std::list<Animation*> getAnimationList() const
        {
            return AnimationList_;
        }
        
        /* === Collision === */
        
        bool checkContact(Collision* CollisionHandle);
        bool getContact(SCollisionContact &NextContact, Collision* CollisionHandle);
        bool getNextContact(SCollisionContact &NextContact);
        
        inline std::list<SCollisionContact>& getContactList()
        {
            return CollisionContactList_;
        }
        
        /* === Bounding volume === */
        
        /**
        Sets the type of bounding box. This is used for frustum-culling only for Entity object.
        Use BOUNDING_NONE to disable frustum culling. Mostly used are bounding boxes.
        But bounding spheres are faster because of less calculation steps.
        \param Type: Type of the bounding volume.
        */
        virtual inline void setBoundingType(const EBoundingVolumes Type)
        {
            BoundingType_ = Type;
        }
        virtual inline EBoundingVolumes getBoundingType() const
        {
            return BoundingType_;
        }
        
        /**
        Sets the bounding box if the bounding type is BOUNDING_BOX.
        \param BoundBox: Axis-aligned-bounding-box which is to used for frustum-culling.
        */
        virtual inline void setBoundingBox(const dim::aabbox3df &BoundBox)
        {
            BoundingBox_ = BoundBox;
        }
        virtual inline dim::aabbox3df getBoundingBox() const
        {
            return BoundingBox_;
        }
        
        //! Sets the bounding-sphere radius if the bounding type is BOUNDING_SPHERE.
        virtual inline void setBoundingSphere(const f32 BoundSphere)
        {
            BoundingSphere_ = BoundSphere;
        }
        virtual inline f32 getBoundingSphere() const
        {
            return BoundingSphere_;
        }
        
        /* === Identification === */
        
        //! \return Object type. Entity (NODE_ENTITY), Light (ENTITY_LIGHT) etc.
        virtual inline ENodeTypes getType() const
        {
            return Type_;
        }
        
        /* === Parents === */
        
        /**
        \param isGlobal: If true the visibility uses the parent hierarchy.
        \return true if this scene node and all scene nodes in its parent hierarchy are visible. Otherwise false.
        */
        virtual bool getVisible(bool isGlobal = false) const;
        
        /**
        Sets the parent object. With parents object transformations become much more easier. If the parent object
        will be moved, rotated or scaled this child object will be transformed in dependency of its parent.
        e.g. a car has four children even their tires. When the car moves around the scene you don't need to handle
        the tire transformation. But don't forget to disable parents when the parent object has been deleted but the
        children further exist!
        \param ParentNode: Pointer to the new parent Node object. If you want to disable the parent set it to 0.
        \param isGlobal: If true the current object transformation looks the same. The transformation will be performed
        in global space. Otherwise in object space.
        */
        void setParent(SceneNode* Parent, bool isGlobal);
        
        //! Sets the parent object only in object space (this is a little bit faster).
        inline void setParent(SceneNode* Parent)
        {
            SceneParent_ = Parent;
        }
        inline SceneNode* getParent() const
        {
            return SceneParent_;
        }
        
        /* === Children === */
        
        //! Adds the specified child.
        virtual void addChild(SceneNode* Child);
        
        //! Adds the specified children.
        virtual void addChildren(const std::list<SceneNode*> &Children);
        
        //! Removes the specified child and returns true if the child could be removed.
        virtual bool removeChild(SceneNode* Child);
        
        //! Removes the first child and returns true if a child could be removed.
        virtual bool removeChild();
        
        //! Removes the specified children and returns the count of removed children.
        virtual u32 removeChildren(const std::list<SceneNode*> &Children);
        
        //! Removes all children.
        virtual void removeChildren();
        
        //! Returns the children list.
        inline const std::list<SceneNode*> &getSceneChildren() const
        {
            return SceneChildren_;
        }
        inline std::list<SceneNode*> &getSceneChildren()
        {
            return SceneChildren_;
        }
        
        /* === Parent system === */
        
        /**
        Updates the parent transformation if a parent object is set.
        You don't need to call this function after setting a new parent. It is used internally.
        */
        virtual void updateParent() const;
        
        //! Returns a matrix which holds the global object transformation.
        virtual dim::matrix4f getGlobalLocation() const;
        virtual dim::matrix4f getGlobalLocationInverse() const;
        
        virtual void updateGlobalLocation() const;
        virtual void updateGlobalLocationInverse() const;
        
        //! Returns a matrix which holds the local object transformation.
        virtual dim::matrix4f getLocalLocation() const;
        virtual dim::matrix4f getLocalLocationInverse() const;
        
        virtual void updateLocalLocation() const;
        virtual void updateLocalLocationInverse() const;
        
        //! Updates the current world or view matrix to the objects transformation.
        virtual void updateModelviewMatrix();
        
        //! Updates the objects transformation.
        virtual void updateTransformation();
        
        //! Loads the transformation which has been updated previously.
        virtual void loadTransformation();
        
        /* === Extra functions === */
        
        SceneNode* copy() const;
        
    protected:
        
        friend class CollisionDetector;
        
        /* === Functions === */
        
        virtual void copyRoot(SceneNode* NewNode) const;
        
        /* === Members === */
        
        ENodeTypes Type_;
        
        dim::matrix4f Position_;
        dim::matrix4f Rotation_;
        dim::matrix4f Scale_;
        dim::matrix4f Transformation_;  // Final transformation (position * rotation * scale)
        
        SceneNode* SceneParent_;
        std::list<SceneNode*> SceneChildren_;
        
        EBoundingVolumes BoundingType_;
        dim::aabbox3df BoundingBox_;
        f32 BoundingSphere_;
        
        std::list<Animation*> AnimationList_;
        std::list<SCollisionContact> CollisionContactList_;
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
