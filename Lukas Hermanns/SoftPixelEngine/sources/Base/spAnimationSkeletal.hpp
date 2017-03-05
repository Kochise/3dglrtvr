/*
 * Animation skeletal header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_ANIMATION_SKELETAL_H__
#define __SP_ANIMATION_SKELETAL_H__


#include "Base/spStandard.hpp"
#include "Base/spAnimation.hpp"


namespace sp
{
namespace scene
{



class AnimationBone;

/*
 * Animation skeletal class
 */

class SP_EXPORT AnimationSkeletal : public Animation
{
    
    public:
        
        AnimationSkeletal(Mesh* ObjMesh);
        ~AnimationSkeletal();
        
        /* === Functions === */
        
        /**
        Creates a new bone.
        \param ParentBone: Bone's parent. If this is a root bone use 0.
        \param Translation: Origin position. This builds the construction or the origin
        transformation of the skeleton without a keyframe.
        \param Rotation: Origin rotation.
        \param Scale: Origin scale. Mostly unused for skeletal animation.
        \param Name: Bone's name.
        \param isGlobal: Specifies whether the given location is global (but still in object space) or not.
        \return Pointer to an AnimationBone object.
        */
        AnimationBone* addBone(
            AnimationBone* ParentBone,
            const dim::vector3df &Translation = 0.0f,
            const dim::quaternion &Rotation = dim::quaternion(),
            const dim::vector3df &Scale = 1.0f,
            const io::stringc &Name = "",
            bool isGlobal = false
        );
        void removeBone(AnimationBone* &Bone, bool DeleteChildren = false);
        
        /* Animation status */
        void play(
            const EAnimMethods AnimMethod = ANIM_ONESHOT, const EAnimModes AnimMode = ANIM_FORWARDS,
            s32 Start = -1, s32 End = -1, bool UseIndividualRange = false
        );
        
        void record(const f32 Speed = 1.0f);
        
        void setSeek(s32 Sequence, f32 Interpolation);
        void getSeek(s32 &Sequence, f32 &Interpolation) const;
        void setSeek(f32 State);
        f32 getSeek() const;
        
        s32 getLength() const;
        
        /* Skeleton */
        AnimationBone* getBone(u32 BoneID) const;
        void setBonesEnable(bool Enable);
        
        //! Updates the skeleton. This function needs to used after building the skeleton.
        void updateSkeleton();
        
        void renderSkeleton(const f32 BoneRadius = 0.5f, bool isSolid = false) const;
        
        void updateAnimation(const bool DirectUpdate = false);
        
        /* === Inline functions === */
        
        inline Mesh* getMesh() const
        {
            return Mesh_;
        }
        
        inline std::vector<AnimationBone*> getBoneList() const
        {
            return BoneList_;
        }
        
    private:
        
        /* Structures */
        
        struct SBasicVertexData
        {
            dim::vector3df Position, Normal;
        };
        
        /* Functions */
        
        void updateBone(AnimationBone* CurBone);
        void renderBoneConnection(const dim::matrix4f &Matrix, bool isSolid) const;
        
        bool checkFrustumCulling() const;
        
        Animation* copy(SceneNode* NewNode) const;
        
        /* Members */
        
        Mesh* Mesh_;
        
        u32 SkeletonBoneID_;
        
        dim::matrix4f Matrix_, NormalMatrix_;
        std::vector<AnimationBone*> BoneList_, BoneRootList_;
        
        bool isFrustumCulling_;
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
