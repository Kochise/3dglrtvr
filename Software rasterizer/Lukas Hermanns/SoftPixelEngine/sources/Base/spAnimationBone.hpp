/*
 * Animation bone header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_ANIMATION_BONE_H__
#define __SP_ANIMATION_BONE_H__


#include "Base/spStandard.hpp"
#include "Base/spMemoryManagement.hpp"
#include "Base/spInputOutputString.hpp"
#include "Base/spDimension.hpp"
#include "Base/spMath.hpp"
#include "Base/spMeshBuffer.hpp"
#include "SceneGraph/spSceneMesh.hpp"

#include <list>
#include <vector>


namespace sp
{
namespace scene
{


class AnimationSkeletal;


/*
 * Structures
 */

struct SVertexWeight
{
    SVertexWeight()
        : Surface(0), Index(0), Weight(1.0f)
    {
    }
    SVertexWeight(video::MeshBuffer* VertexSurface, u32 VertexIndex, f32 VertexWeight = 1.0f)
        : Surface(VertexSurface), Index(VertexIndex), Weight(VertexWeight)
    {
    }
    ~SVertexWeight()
    {
    }
    
    /* Members */
    
    video::MeshBuffer* Surface; //!< Mesh buffer.
    u32 Index;                  //!< Vertex index.
    
    f32 Weight;                 //!< Percentaged weight (0.0 - 1.0).
    
    dim::vector3df Position;    //!< Original vertex position.
    dim::vector3df Normal;      //!< Original vertex normal.
};


/*
 * AnimationBone class
 */

class SP_EXPORT AnimationBone
{
    
    public:
        
        AnimationBone(
            u32 &BoneIDCounter, AnimationBone* Parent,
            const dim::vector3df &Translation, const dim::quaternion &Rotation, const dim::vector3df &Scale,
            const io::stringc &Name, bool isGlobal
        );
        ~AnimationBone();
        
        /* === Functions === */
        
        /**
        Adds a new keyframe to the list. The transformations here are never global!
        \param Translation: Position of the bone.
        \param Rotation: Quaternion rotation (4 components: X, Y, Z, W) for sphereical-linear-interpolation.
        \param Scale: Scaling of the bone.
        \param Speed: Speed of the interpolation to the next keyframe.
        \return Index of the new keyframe (beginning with 0).
        */
        u32 addKeyframe(
            const dim::vector3df &Translation = dim::vector3df(0.0f),
            const dim::quaternion &Rotation = dim::quaternion(),
            const dim::vector3df &Scale = dim::vector3df(1.0f),
            const f32 Speed = 1.0f,
            bool isGlobal = false
        );
        void removeKeyframe(u32 FrameIndex);
        
        //! Sets the specified keyframe.
        void setKeyframe(u32 Index, const SAnimKeyframe &Keyframe);
        
        //! Returns the specified keyframe.
        SAnimKeyframe getKeyframe(u32 Index) const;
        
        /**
        Adds a new vertex weight.
        \param Surface: Specifies the mesh buffer which holds the vertex.
        \param Index: Specifies the vertex index in the given surface.
        \param Weight: Specifies the vertex/bone weight. This value should be in the range [0.0, 1.0].
        */
        void addVertexWeight(video::MeshBuffer* Surface, u32 Index, f32 Weight = 1.0f);
        
        //! Sets the vertex weights. The vertex weights are the indices of the vertices which are to be transformed by that bone.
        void setupVertexWeights(const std::vector<SVertexWeight> &VertexWeights);
        
        /**
        Updates the origin transformations of the vertex weights.
        \param UpdateVertices: If true the vertices' coordinates will be updated.
        \param UpdateNormals: If true the vertices' normals will be updated.
        */
        void updateVertexWeights(bool UpdateVertices = true, bool UpdateNormals = true);
        
        /**
        Updates the next sequence so that the new interpolation will be computed.
        \param IndexFrom: Keyframe index from which the interpolation is to be started.
        \param IndexTo: Keyframe index to which the interpolation is to be ended.
        */
        void updateSequence(s32 IndexFrom, s32 IndexTo);
        
        /**
        Updates the interpolation. Translation, scaling and rotation (using spherical-linear-interpolation: look at
        the "slerp" function in the dim::quaternion class) will be processed.
        */
        void updateInterpolation();
        
        /**
        Updates the bone's transformation.
        \param Transformation: Matrix where the transformation is to be stored.
        \param isGlobal: If true the transformation is global to the parent bones (not to the whole object).
        */
        void updateTransformation(dim::matrix4f &Transformation, bool isGlobal = true) const;
        
        //! Returns global bone transformation to the object.
        dim::matrix4f getGlobalLocation() const;
        
        void setSeek(f32 Seek); // (0.0 - 1.0)
        f32 getSeek() const;
        
        bool nextFrame(const f32 Speed);
        
        static void renderBone(const AnimationBone* Bone, dim::matrix4f Matrix, const f32 Radius);
        
        /* === Inline functions === */
        
        /* Generall states & settings */
        
        inline u32 getID() const
        {
            return ID_;
        }
        
        inline void setName(const io::stringc &Name)
        {
            Name_ = Name;
        }
        inline io::stringc getName() const
        {
            return Name_;
        }
        
        inline void setEnable(bool Enable)
        {
            Enabled_ = Enable;
        }
        inline bool getEnable() const
        {
            return Enabled_;
        }
        
        /* Transformation */
        
        inline void setTranslation(const dim::vector3df &Translation)
        {
            Translation_ = Translation;
        }
        inline dim::vector3df getTranslation() const
        {
            return Translation_;
        }
        
        inline void setRotation(const dim::quaternion &Rotation)
        {
            Rotation_ = Rotation;
        }
        inline dim::quaternion getRotation() const
        {
            return Rotation_;
        }
        
        inline void setScale(const dim::vector3df &Scale)
        {
            Scale_ = Scale;
        }
        inline dim::vector3df getScale() const
        {
            return Scale_;
        }
        
        inline dim::matrix4f getRotationMatrix() const
        {
            return RotationMatrix_;
        }
        
        /* Sequence status */
        
        inline void setInterpolation(const f32 Interpolation)
        {
            Interpolation_ = Interpolation;
            math::Clamp(Interpolation_, 0.0f, 1.0f);
        }
        inline f32 getInterpolation() const
        {
            return Interpolation_;
        }
        
        inline void setSequence(const s32 Sequence)
        {
            Sequence_ = Sequence;
            math::Clamp(Sequence_, 0, (s32)(KeyframeList_.size() - 1));
        }
        inline s32 getSequence() const
        {
            return Sequence_;
        }
        
        inline void setSequenceRange(s32 Start, s32 End)
        {
            Start_  = Start;
            End_    = End;
        }
        inline void getSequenceRange(s32 &Start, s32 &End) const
        {
            Start   = Start_;
            End     = End_;
        }
        
        inline void setVertexWeights(const std::vector<SVertexWeight> &VertexWeights)
        {
            VertexWeights_ = VertexWeights;
        }
        inline std::vector<SVertexWeight> getVertexWeights() const
        {
            return VertexWeights_;
        }
        inline std::vector<SVertexWeight>& getVertexWeights()
        {
            return VertexWeights_;
        }
        
        inline void setKeyframeList(const std::vector<SAnimKeyframe> &KeyframeList)
        {
            KeyframeList_ = KeyframeList;
        }
        inline std::vector<SAnimKeyframe> getKeyframeList() const
        {
            return KeyframeList_;
        }
        inline std::vector<SAnimKeyframe>& getKeyframeList()
        {
            return KeyframeList_;
        }
        
        /* Joint connection */
        
        inline void setParent(AnimationBone* Parent)
        {
            Parent_ = Parent;
        }
        inline AnimationBone* getParent() const
        {
            return Parent_;
        }
        
        inline void setChildren(const std::vector<AnimationBone*> &Children)
        {
            Children_ = Children;
        }
        inline std::vector<AnimationBone*> getChildren() const
        {
            return Children_;
        }
        
    private:
        
        friend class AnimationSkeletal;
        
        /* === Functions === */
        
        void setupFrameLocation(
            dim::vector3df &DestTranslation, dim::quaternion &DestRotation, dim::vector3df &DestScale,
            const dim::vector3df &SrcTranslation, const dim::quaternion &SrcRotation, const dim::vector3df &SrcScale,
            bool isGlobal
        );
        
        /* === Members === */
        
        // Identity
        u32 ID_;
        io::stringc Name_;
        
        // Status
        bool Enabled_;
        
        f32 Interpolation_;
        s32 Sequence_;
        
        s32 Start_, End_;
        
        // Current states
        dim::vector3df Translation_, Scale_;
        dim::quaternion Rotation_;
        
        // Matrices
        dim::matrix4f MatrixOrigin_, RotationMatrix_;
        
        // Vertex weights
        std::vector<SVertexWeight> VertexWeights_;
        
        // Joint connections
        std::vector<AnimationBone*> Children_;
        AnimationBone* Parent_;
        
        // Interpolation data
        dim::vector3df NextTranslation_;
        dim::quaternion NextRotation_;
        dim::vector3df NextScale_;
        
        // Frames
        std::vector<SAnimKeyframe> KeyframeList_;
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
