/*
 * Physics static body header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_PHYSICS_STATICBODY_H__
#define __SP_PHYSICS_STATICBODY_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_NEWTONPHYSICS


#include "Base/spDimension.hpp"
#include "SceneGraph/spSceneMesh.hpp"
#include "Framework/Physics/spPhysicsBasicObject.hpp"


namespace sp
{
namespace physics
{


/**
...
*/
class SP_EXPORT StaticBody : public PhysicsBasicObject
{
    
    public:
        
        StaticBody(scene::Mesh* MeshGeom);
        ~StaticBody();
        
        /* === Functions === */
        
        /* === Inline functions === */
        
    private:
        
        /* === Functions === */
        
        void createMesh();
		
        /* === Members === */
        
        scene::Mesh* MeshGeom_;
        
};


} // /namespace physics

} // /namespace sp


#endif

#endif



// ================================================================================
