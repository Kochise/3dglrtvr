/*
 * Physics system header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_PHYSICS_SYSTEM_H__
#define __SP_PHYSICS_SYSTEM_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_NEWTONPHYSICS


#include "Base/spDimension.hpp"
#include "Framework/Physics/spPhysicsRigidBody.hpp"
#include "Framework/Physics/spPhysicsStaticBody.hpp"


class NewtonWorld;

namespace sp
{
namespace physics
{


typedef void (*PFNPHYSICSCONTACTCALLBACKPROC)(const dim::vector3df &Point, const dim::vector3df &Normal, f32 Velocity);

/**
Third party library extension: "Newton Game Dynamics" (v.2.0) physics simulation engine.
*/
class SP_EXPORT PhysicsSystem
{
    
    public:
        
        PhysicsSystem();
        ~PhysicsSystem();
        
        //! Returns the version of the physics system.
        io::stringc getVersion() const;
        
        /**
        Updates the whole simulation. Call this in each frame of your game loop.
        \param StepTime: Specifies the speed of simulation for the current frame.
        */
        void updateSimulation(const f32 StepTime = 1.0f / 60.0f);
        
        /**
        Sets the gravity for each object. You can also set the gravity individually for each rigid body.
        \param Gravity: Specifies the gravity vector which is to be set. By default (0 | -9.81 | 0).
        */
        void setGravity(const dim::vector3df &Gravity);
        
        /**
        Creates a new physics static body. This is a none interactive environment object.
        Those objects represents the physics world.
        */
        StaticBody* createStaticBody(scene::Mesh* MeshGeom);
        
        //! Deletes the specified static body.
        void deleteStaticBody(StaticBody* Object);
        
        /**
        Creates a new physics rigid body. This is an interactive physics object.
        \param RootNode: Specifies the root scene node which is to be manipulated in its location and orientation.
        \param Type: Specifies the type of the body.
        \param Construct: Specifies the construction of the body. E.g. size, radius and/or height.
        \param Mass: Specifies the body's mass factor (by default 25.0).
        */
        RigidBody* createRigidBody(
            scene::SceneNode* RootNode, const ERigidBodies Type,
            const SRigidBodyConstruction &Construct = SRigidBodyConstruction(), const f32 Mass = 25.0f
        );
        
        /**
        Creates a new mesh physics rigid body. If you only want to use the mesh as the physics geometry you can
        change the root node by calling RigidBody::setRootNode".
        \param MeshGeom: Specifies the root mesh node.
        \param Mass: Specifies the body's mass factor (by default 25.0).
        \note Don't use to large mesh geometries for rigid bodies. This kind of physics simulation is very time consuming!
        */
        RigidBody* createRigidBody(scene::Mesh* MeshGeom, const f32 Mass = 25.0f);
        
        //! Delete the specified rigid body but not its root node.
        void deleteRigidBody(RigidBody* Object);
        
        /*Ragdoll* createRagdoll();
        void deleteRagdoll(Ragdoll* Object);
        
        Vehicle* createVehicle();
        deleteVehicle(Vehicle* Object);
        
        MaterialGroup* createMaterialGroup();
        void deleteMaterialGroup(MaterialGroup* Material);*/
        
        //! Sets the count of threads used for physics computations.
        void setThreadCount(s32 Count);
        //! Returns the count of threads.
        s32 getThreadCount() const;
        
        /**
        Sets the solver model. Use higher values if speed is more important than accuracy.
        \param Model: If 0 the accuracy is exact. If greater than 0 you can configure the accuracy if speed is more important.
        */
        void setSolverModel(s32 Model);
        
        //! Sets the contact callback. This callback procedure will be called when a collision contact occured.
        static void setContactCallback(PFNPHYSICSCONTACTCALLBACKPROC Callback);
        
        static PFNPHYSICSCONTACTCALLBACKPROC getContactCallback();
        
        /* === Inline functions === */
        
        inline dim::vector3df getGravity() const
        {
            return Gravity_;
        }
        
        inline std::list<RigidBody*> getRigidBodyList() const
        {
            return RigidBodyList_;
        }
        inline std::list<StaticBody*> getStaticBodyList() const
        {
            return StaticBodyList_;
        }
        
    private:
        
        friend class BodyJoint;
        friend class RigidBody;
        friend class StaticBody;
        friend class PhysicsBasicObject;
        
        friend void DefCallbackGenericContactProcess(const NewtonJoint* Contact, f32 TimeStep, s32 ThreadIndex);
        
        /* === Members === */
        
        static NewtonWorld* CurWorld_;
        
        NewtonWorld* NWorld_;
        
        std::list<RigidBody*> RigidBodyList_;
        std::list<StaticBody*> StaticBodyList_;
        //std::list<MaterialGroup*> MaterialGroupList_;
        
        dim::vector3df Gravity_;
        
        static PFNPHYSICSCONTACTCALLBACKPROC ContactCallback_;
        
};


} // /namespace physics

} // /namespace sp


#endif

#endif



// ================================================================================
