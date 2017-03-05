/*
 * Animation node header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_ANIMATION_NODE_H__
#define __SP_ANIMATION_NODE_H__


#include "Base/spStandard.hpp"
#include "Base/spAnimation.hpp"
#include "Base/spMathSpline.hpp"


namespace sp
{
namespace scene
{


/*
 * Animation node class
 */

class SP_EXPORT AnimationNode : public Animation
{
    
    public:
        
        AnimationNode(SceneNode* ObjNode);
        ~AnimationNode();
        
        /* Functions */
        
        /**
        Adds a new keyframe sequence.
        \param Position: Local position.
        \param Rotation: Local quaternion rotation (for spherical-linear-interpolation).
        \param Scale: Local scaling.
        \param Speed: Animation's speed.
        */
        void addSequence(
            const dim::vector3df &Position,
            const dim::quaternion &Rotation = dim::quaternion(),
            const dim::vector3df &Scale = 1.0f,
            const f32 Speed = 1.0f
        );
        void removeSequence(u32 SeqIndex);
        
        void setSequence(u32 SeqIndex, const SAnimKeyframe &Sequence);
        SAnimKeyframe getSequence(u32 SeqIndex) const;
        
        s32 getLength() const;
        
        /**
        Enables or disables the spline translation. If enabled the position's translation
        is processed using splines for smooth movements.
        */
        void setSplineTranslation(const bool Enable);
        bool getSplineTranslation() const;
        
        /**
        Sets the spline's expansion.
        \param Expansion: Spline's expansion. By default 1.0.
        If 0.0 the animation looks the same when spline translations are disabled.
        */
        void setSplineExpansion(const f32 Expansion);
        f32 getSplineExpansion() const;
        
        void updateAnimation(const bool DirectUpdate = false);
        
        /* Inline functions */
        
        inline SceneNode* getNode() const
        {
            return Node_;
        }
        
        inline math::Spline3D* getSpline() const
        {
            return Spline_;
        }
        
    private:
        
        friend class SceneNode;
        
        /* Functions */
        
        void updateNextSequence();
        
        void updateSpline();
        
        Animation* copy(SceneNode* NewNode) const;
        
        /* Members */
        
        SceneNode* Node_;
        
        math::Spline3D* Spline_;
        f32 SplineExpansion_;
        
        // Sequences
        SAnimKeyframe * PrevSeq_, * NextSeq_;
        std::vector<SAnimKeyframe> SequenceList_;
        
};


} // /namespace scene

} // /namespace sp


#endif







// ================================================================================
