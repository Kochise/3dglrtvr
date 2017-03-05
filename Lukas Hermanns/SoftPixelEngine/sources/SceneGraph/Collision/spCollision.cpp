/*
 * Collision file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "SceneGraph/Collision/spCollision.hpp"


namespace sp
{
namespace scene
{


/*
 * SCollisionSystemObject structure
 */

SCollisionSystemObject::SCollisionSystemObject()
    : Object(0), Mesh(0), Radius(0.5f), FaceType(video::FACE_FRONT),
    TriangleList(0), TriangleCount(0), IgnoreVisibility(false)
{
}
SCollisionSystemObject::~SCollisionSystemObject()
{
    MemoryManager::deleteBuffer(TriangleList);
}

void SCollisionSystemObject::createTriangleData()
{
    MemoryManager::deleteBuffer(TriangleList);
    
    if (Mesh)
    {
        u32 Indices[3];
        
        TriangleCount   = Mesh->getTriangleCount();
        TriangleList    = new STriangleData[TriangleCount];
        
        for (s32 s = 0, i, j = 0; s < Mesh->getMeshBufferCount(); ++s)
        {
            video::MeshBuffer* Surface = Mesh->getMeshBuffer(s);
            
            for (i = 0; i < Surface->getTriangleCount(); ++i, ++j)
            {
                TriangleList[j].Triangle    = Surface->getTriangleReference(i);
                TriangleList[j].Surface     = s;
                TriangleList[j].Index       = i;
            } // next triangle
        } // next surface
    } // fi
}

void SCollisionSystemObject::deleteTriangleData()
{
    MemoryManager::deleteBuffer(TriangleList);
    TriangleCount = 0;
}


/*
 * Collision class
 */

Collision::Collision()
{
}
Collision::~Collision()
{
}

void Collision::addCollisionMaterial(Collision* DestCollision, const ECollisionTypes Type)
{
    SCollisionMaterial DestData;
    {
        DestData.DestCollision  = DestCollision;
        DestData.Type           = Type;
    }
    CollMaterialList_.push_back(DestData);
}

void Collision::removeCollisionMaterial(Collision* DestCollision)
{
    for (std::list<SCollisionMaterial>::iterator it = CollMaterialList_.begin(); it != CollMaterialList_.end(); ++it)
    {
        if (it->DestCollision == DestCollision)
        {
            CollMaterialList_.erase(it);
            return;
        }
    }
}


} // /namespace scene

} // /namespace sp



// ================================================================================
