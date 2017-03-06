/*
 * Physics static body file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Framework/Physics/spPhysicsStaticBody.hpp"

#ifdef SP_COMPILE_WITH_NEWTONPHYSICS


#include "Framework/Physics/spPhysicsSystem.hpp"
#include "Base/spMemoryManagement.hpp"

#include <newton.h>


namespace sp
{
namespace physics
{


StaticBody::StaticBody(scene::Mesh* MeshGeom)
    : PhysicsBasicObject(), MeshGeom_(MeshGeom)
{
    createMesh();
}
StaticBody::~StaticBody()
{
}


/*
 * ======= Private: =======
 */

void StaticBody::createMesh()
{
    /* Create the collision tree */
    NCollision_ = NewtonCreateTreeCollision(PhysicsSystem::CurWorld_, 0);
    
    /* Temporary storage */
    u32 Indices[3], s, i;
    dim::vector3df VertexCoords[3];
    
    const dim::matrix4f ScaleMatrix(MeshGeom_->getScaleMatrix());
    
    /* Build the collision tree */
    NewtonTreeCollisionBeginBuild(NCollision_);
    
    for (s = 0; s < MeshGeom_->getMeshBufferCount(); ++s)
    {
        video::MeshBuffer* Surface = MeshGeom_->getMeshBuffer(s);
        
        for (i = 0; i < Surface->getTriangleCount(); ++i)
        {
            Surface->getTriangleIndices(i, Indices);
            
            VertexCoords[0] = ScaleMatrix * Surface->getVertexCoord(Indices[0]);
            VertexCoords[1] = ScaleMatrix * Surface->getVertexCoord(Indices[1]);
            VertexCoords[2] = ScaleMatrix * Surface->getVertexCoord(Indices[2]);
            
            NewtonTreeCollisionAddFace(NCollision_, 3, &VertexCoords[0].X, sizeof(dim::vector3df), 0);
        }
    }
    
    NewtonTreeCollisionEndBuild(NCollision_, 1);
    
    /* Create the body */
    NBody_ = NewtonCreateBody(PhysicsSystem::CurWorld_, NCollision_);
    
    /* Update first transformation */
    const dim::matrix4f FirstTransformation(MeshGeom_->getPositionMatrix() * MeshGeom_->getRotationMatrix());
    NewtonBodySetMatrix(NBody_, FirstTransformation.getArray());
    
    /* Compute the collision axis-alined-bounding-box (AABB) */
    f32 BoxP0[3], BoxP1[3], Matrix[16];
    
    NewtonBodyGetMatrix(NBody_, Matrix);
    NewtonCollisionCalculateAABB(NCollision_, Matrix, BoxP0, BoxP1);
}


} // /namespace physics

} // /namespace sp


#endif



// ================================================================================
