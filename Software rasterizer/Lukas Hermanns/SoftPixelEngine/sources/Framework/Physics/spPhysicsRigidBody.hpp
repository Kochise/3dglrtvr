/*
 * Physics rigid body header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_PHYSICS_RIGIDBODY_H__
#define __SP_PHYSICS_RIGIDBODY_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_NEWTONPHYSICS


#include "Base/spDimension.hpp"
#include "SceneGraph/spSceneMesh.hpp"
#include "Framework/Physics/spPhysicsBodyJoint.hpp"
#include "Framework/Physics/spPhysicsBasicObject.hpp"


namespace sp
{
namespace physics
{


class RigidBody;

typedef void (*PFNRIGIDBODYGRAVITYPROC)(RigidBody* PhysicsObject);

//! Rigid body types.
enum ERigidBodies
{
    RIGIDBODY_BOX,              //!< Normal box.
    RIGIDBODY_CONE,             //!< Normal cone.
    RIGIDBODY_SPHERE,           //!< Normal sphere.
    RIGIDBODY_CYLINDER,         //!< Normal cylinder.
    RIGIDBODY_CHAMFERCYLINDER,  //!< Cylinder with smooth edges at top and bottom.
    RIGIDBODY_CAPSULE,          //!< Cylinder with half spheres at top and bottom.
    RIGIDBODY_MESH,             //!< Universal mesh object.
};


//! Rigid body construction data.
struct SRigidBodyConstruction
{
    SRigidBodyConstruction(f32 InitRadius = 0.5f, f32 InitHeight = 0.5f)
        : Size(0.5f), Radius(InitRadius), Height(InitHeight)
    {
    }
    SRigidBodyConstruction(
        const dim::vector3df InitSize, f32 InitRadius = 0.5f, f32 InitHeight = 0.5f)
        : Size(InitSize), Radius(InitRadius), Height(InitHeight)
    {
    }
    ~SRigidBodyConstruction()
    {
    }
    
    /* Members */
    dim::vector3df Size;
    f32 Radius, Height;
};


/**
...
*/
class SP_EXPORT RigidBody : public PhysicsBasicObject
{
    
    public:
        
        RigidBody(const ERigidBodies Type, const SRigidBodyConstruction &Construct);
        RigidBody(scene::Mesh* MeshGeom);
        ~RigidBody();
        
        /* === Functions === */
        
        //! Sets the root node which is to be located and oriented by the rigid body.
        void setRootNode(scene::SceneNode* Node);
        
        //! Sets the body's mass factor (by default 25.0).
        void setMass(const f32 Mass);
        
        //! Enables or disables the auto sleep and freeze mode. By default disabled. This can be used for optimization purposes.
        void setAutoSleep(bool Enable);
        
        //! Sets the balance point (or rather center of mass) of the rigid body in object space. By default (0 | 0 | 0).
        void setBalancePoint(const dim::vector3df &LocalPoint);
        //! Returns the balance point (or rather center of mass).
        dim::vector3df getBalancePoint() const;
        
        void addVelocity(const dim::vector3df &Direction);
        void setVelocity(const dim::vector3df &Direction);
        dim::vector3df getVelocity() const;
        
        /**
        Adds the specified impulse to the body. E.g. use this when you shoot at a box and the box shall throw away.
        \param Direction: Specifies the direction in which the impulse is to be pointed.
        \param PivotPoint: Specifies the global point where the impulse is to be performed.
        */
        void addImpulse(const dim::vector3df &Direction, const dim::vector3df &PivotPoint);
        
        /**
        Sets the force for this object. This is actually only used to apply the g-force.
        \note This function can only be used internally. Otherwise it has no effect.
        */
        void setForce(const dim::vector3df &Direction);
        
        /**
        Adds a new joint connected child body to this rigid body.
        \param Child: Specifies the child rigid body which is to be added with a joint.
        If this parameter is 0 this rigid body is the child and will be appended to the physics world.
        \param Type: Specifies the type of the joint. E.g. when you want to have a physics door use the JOINT_HINGE type.
        \param PivotPoint: Specifies the global point where the joint is to be set.
        \return Pointer to the new BodyJoint object.
        */
        BodyJoint* addJoint(
            PhysicsBasicObject* Child, const EBodyJoints Type, const dim::vector3df &PivotPoint,
            const dim::vector3df &Direction = dim::vector3df(0, 1, 0), const dim::vector3df &SecondDirection = dim::vector3df(0, 1, 0)
        );
        void removeJoint(BodyJoint* Object);
        void removeJoint(PhysicsBasicObject* Child);
        
        /* === Inline functions === */
        
        //! Returns the rigid body's type.
        inline ERigidBodies getType() const
        {
            return Type_;
        }
        
        //! Returns the root node.
        inline scene::SceneNode* getRootNode() const
        {
            return RootNode_;
        }
        
        //! Sets the individual gravity for this rigid body.
        inline void setGravity(const dim::vector3df &Gravity)
        {
            Gravity_ = Gravity;
        }
        //! Returns the individual gravity for this rigid body.
        inline dim::vector3df getGravity() const
        {
            return Gravity_;
        }
        
        //! Returns the body's mass factor.
        inline f32 getMass() const
        {
            return Mass_;
        }
        
        //! Returns true if the auto sleep and freeze mode is enabled.
        inline bool getAutoSleep() const
        {
            return AutoSleep_;
        }
        
        //! Returns the list of all joints.
        inline std::list<BodyJoint*> getJointList() const
        {
            return JointList_;
        }
        
        //! Sets the gravity callback. By default 0.
        inline void setGravityCallback(PFNRIGIDBODYGRAVITYPROC Callback)
        {
            GravityCallback_ = Callback;
        }
        //! Returns the gravity callback.
        inline PFNRIGIDBODYGRAVITYPROC getGravityCallback() const
        {
            return GravityCallback_;
        }
        
    private:
        
        friend class BodyJoint;
        
        /* === Functions === */
        
        void createBox              (const SRigidBodyConstruction &Construct);
        void createCone             (const SRigidBodyConstruction &Construct);
        void createSphere           (const SRigidBodyConstruction &Construct);
        void createCylinder         (const SRigidBodyConstruction &Construct);
        void createChamferCylinder  (const SRigidBodyConstruction &Construct);
        void createCapsule          (const SRigidBodyConstruction &Construct);
        
        void createMesh(scene::Mesh* MeshGeom);
        
        void createDefaultBody();
        
        /* === Members === */
        
        ERigidBodies Type_;
        
        SRigidBodyConstruction Construct_;
        
        dim::vector3df Gravity_;
        f32 Mass_;
        bool AutoSleep_;
        
        scene::SceneNode* RootNode_;
        std::list<BodyJoint*> JointList_;
        
        PFNRIGIDBODYGRAVITYPROC GravityCallback_;
        
};


} // /namespace physics

} // /namespace sp


#endif

#endif



// ================================================================================
