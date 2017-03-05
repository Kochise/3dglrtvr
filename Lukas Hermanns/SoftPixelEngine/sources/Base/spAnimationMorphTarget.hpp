/*
 * Morph target animation header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_ANIMATION_MORPHTARGET_H__
#define __SP_ANIMATION_MORPHTARGET_H__


#include "Base/spStandard.hpp"
#include "Base/spAnimation.hpp"


namespace sp
{
namespace scene
{


/*
 * Structures
 */

struct SAnimMorphTargetVertexWeight
{
    u32 Index;
    u32 Surface;
};

struct SAnimMorphTargetVertex
{
    SAnimMorphTargetVertex()
    {
    }
    SAnimMorphTargetVertex(const dim::vector3df &VertCoord, const dim::vector3df &VertNormal)
        : Coord(VertCoord), Normal(VertNormal)
    {
    }
    ~SAnimMorphTargetVertex()
    {
    }
    
    /* Members */
    dim::vector3df Coord, Normal;
};

struct SAnimMorphTargetSequence
{
    SAnimMorphTargetSequence(const f32 FrameSpeed = 1.0f)
        : Speed(FrameSpeed)
    {
    }
    SAnimMorphTargetSequence(
        const std::vector<dim::vector3df> &CoordList, f32 FrameSpeed = 1.0f)
        : Speed(FrameSpeed)
    {
        Vertices.resize(CoordList.size());
        for (u32 i = 0; i < CoordList.size(); ++i)
        {
            Vertices[i].Coord   = CoordList[i];
            Vertices[i].Normal  = dim::vector3df(0, 0, 1);
        }
    }
    ~SAnimMorphTargetSequence()
    {
    }
    
    /* Members */
    f32 Speed;
    std::vector<SAnimMorphTargetVertex> Vertices;
};


/*
 * Animation morph-target class
 */

class SP_EXPORT AnimationMorphTarget : public Animation
{
    
    public:
        
        AnimationMorphTarget(Mesh* ObjMesh);
        ~AnimationMorphTarget();
        
        /* Functions */
        
        /**
        Adds a new keyframe sequence to the animation. If the vertex-weights list
        is already set "Vertices" in the SAnimMorphTargetSequence structure must have the same count of elements.
        \param Sequence: Specifies the new keyframe sequence.
        \param Speed: Specifies the speed for this sequence.
        */
        void addSequence(const SAnimMorphTargetSequence &Sequence);
        
        //! Removes the specified keyframe sequence.
        void removeSequence(u32 SeqIndex);
        
        void setSequence(u32 SeqIndex, const SAnimMorphTargetSequence &Sequence);
        SAnimMorphTargetSequence getSequence(u32 SeqIndex) const;
        
        void setVertexWeightsDefault();
        void setVertexWeights(const std::vector<SAnimMorphTargetVertexWeight> &VertexWeights);
        std::vector<SAnimMorphTargetVertexWeight> getVertexWeights() const;
        
        s32 getLength() const;
        
        void updateAnimation(const bool DirectUpdate = false);
        
        /* Inline functions */
        
        inline Mesh* getMesh() const
        {
            return Mesh_;
        }
        
    private:
        
        /* Functions */
        
        void updateNextSequence();
        
        bool checkDataSize(s32 Size);
        
        Animation* copy(SceneNode* NewNode) const;
        
        /* Members */
        
        Mesh* Mesh_;
        
        s32 VertexWeightsSize_;
        
        SAnimMorphTargetSequence * PrevSeq_, * NextSeq_;
        std::vector<SAnimMorphTargetSequence> SequenceList_;
        std::vector<SAnimMorphTargetVertexWeight> VertexWeights_;
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
