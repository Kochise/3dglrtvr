/*
 * Collision configuration types header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_COLLISION_CONFIG_TYPES_H__
#define __SP_COLLISION_CONFIG_TYPES_H__


#include "Base/spStandard.hpp"
#include "Base/spMeshBuffer.hpp"
#include "SceneGraph/spSceneMesh.hpp"


namespace sp
{
namespace scene
{


/*
 * Pre-declerations
 */

class Collision;
class CollisionDetector;
class CollisionNode;
class CollisionSphere;
class CollisionBox;
class CollisionMesh;


/*
 * Enumerations
 */

//! Collision models.
enum ECollisionModels
{
    COLLISION_SPHERE,   //!< Collision sphere with position and radius.
    COLLISION_BOX,      //!< Collision box with position, rotation and axis-alined-bounding-box.
    COLLISION_MESH,     //!< Collision mesh using kd-Tree.
    COLLISION_TERRAIN,  //!< Collision terrain using QuadTree.
};


/*
 * Structures
 */

struct SCollisionNodeContact
{
    CollisionNode* Object;          //!< Collision object.
    dim::vector3df Point;           //!< Contact point.
    dim::vector3df Normal;          //!< Contact normal.
};

struct SCollisionMeshContact
{
    CollisionMesh* Object;          //!< Collision object.
    dim::vector3df Point;           //!< Contact point.
    dim::vector3df Normal;          //!< Contact normal.
    scene::Mesh* Mesh;              //!< Mesh object.
    video::MeshBuffer* Surface;     //!< Original surface (or rather MeshBuffer).
    u32 TriangleIndex;              //!< Original triangle index.
};

struct SCollisionTriangle
{
    scene::Mesh* Mesh;              //!< Mesh object.
    video::MeshBuffer* Surface;     //!< Original surface index.
    u32 TriangleIndex;              //!< Original triangle index.
    dim::triangle3df Triangle;      //!< Collision geometry triangle.
};

#if 1 // !!! todo -> remove these structures

struct SCollisionSystemObject
{
    SCollisionSystemObject();
    virtual ~SCollisionSystemObject();
    
    /* Structures */
    struct STriangleData
    {
        u32 Surface;
        u32 Index;
        dim::ptriangle3df Triangle;
    };
    
    /* Functions */
    void createTriangleData();
    void deleteTriangleData();
    
    /* Members */
    scene::SceneNode* Object;
    scene::Mesh* Mesh;
    
    f32 Radius; // dim::vector3df Radius;
    dim::aabbox3df BoundingBox;
    video::EFaceTypes FaceType;
    
    STriangleData* TriangleList;
    u32 TriangleCount;
    
    bool IgnoreVisibility;
};

struct SCollisionObject : public SCollisionSystemObject
{
    SCollisionObject()
        : SCollisionSystemObject(), CollisionHandle(0),
        isAutoFreeze(false), isCollidable(true)
    {
    }
    ~SCollisionObject()
    {
    }
    
    /* Members */
    Collision* CollisionHandle;
    dim::vector3df LastPosition;
    bool isAutoFreeze;
    bool isCollidable;
};

struct SPickingObject : public SCollisionSystemObject
{
    SPickingObject()
        : SCollisionSystemObject()
    {
    }
    ~SPickingObject()
    {
    }
    
    /* Members */
    EPickingTypes Type;
};

#endif


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
