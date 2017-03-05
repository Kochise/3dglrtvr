/*
 * Animation node file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Base/spAnimationNode.hpp"
#include "SceneGraph/spSceneNode.hpp"


namespace sp
{
namespace scene
{


AnimationNode::AnimationNode(SceneNode* ObjNode)
    : Animation(ANIMATION_NODE), Node_(ObjNode),
    Spline_(0), SplineExpansion_(1.0f), PrevSeq_(0), NextSeq_(0)
{
}
AnimationNode::~AnimationNode()
{
    setSplineTranslation(false);
}


void AnimationNode::addSequence(
    const dim::vector3df &Position, const dim::quaternion &Rotation, const dim::vector3df &Scale, const f32 Speed)
{
    SAnimKeyframe Sequence;
    {
        Sequence.Translation    = Position;
        Sequence.Rotation       = Rotation;
        Sequence.Scale          = Scale;
        Sequence.Speed          = Speed;
    }
    SequenceList_.push_back(Sequence);
    
    updateSpline();
}

void AnimationNode::removeSequence(u32 SeqIndex)
{
    if (SeqIndex < SequenceList_.size())
    {
        SequenceList_.erase(SequenceList_.begin() + SeqIndex);
        updateSpline();
    }
}

void AnimationNode::setSequence(u32 SeqIndex, const SAnimKeyframe &Sequence)
{
    if (SeqIndex < SequenceList_.size())
    {
        SequenceList_[SeqIndex] = Sequence;
        updateSpline();
    }
}
SAnimKeyframe AnimationNode::getSequence(u32 SeqIndex) const
{
    return (SeqIndex < SequenceList_.size() ? SequenceList_[SeqIndex] : SAnimKeyframe());
}

s32 AnimationNode::getLength() const
{
    return SequenceList_.size();
}

void AnimationNode::setSplineTranslation(const bool Enable)
{
    if (getSplineTranslation() != Enable)
    {
        if (Enable)
        {
            Spline_ = new math::Spline3D();
            Spline_->setExpansion(SplineExpansion_);
            updateSpline();
        }
        else
        {
            Spline_->clear();
            MemoryManager::deleteMemory(Spline_);
        }
    }
}
bool AnimationNode::getSplineTranslation() const
{
    return Spline_ != 0;
}

void AnimationNode::setSplineExpansion(const f32 Expansion)
{
    if (!math::Equal(SplineExpansion_, Expansion))
    {
        SplineExpansion_ = Expansion;
        
        if (Spline_)
        {
            delete Spline_;
            Spline_ = new math::Spline3D();
            Spline_->setExpansion(SplineExpansion_);
            updateSpline();
        }
    }
}
f32 AnimationNode::getSplineExpansion() const
{
    return SplineExpansion_;
}


void AnimationNode::updateAnimation(const bool DirectUpdate)
{
    if (!DirectUpdate && AnimMode_ == ANIM_STOP)
        return;
    
    updateSequence(NextSeq_->Speed);
    
    dim::quaternion Rotation;
    
    /* Matrix transformation */
    Rotation.slerp(PrevSeq_->Rotation, NextSeq_->Rotation, Interpolation_);
    
    if (Spline_)
        Node_->setPosition(Spline_->getPolynom((u32)Sequence_).calc(Interpolation_));
    else
        Node_->setPosition(PrevSeq_->Translation + ( NextSeq_->Translation - PrevSeq_->Translation ) * Interpolation_);
    
    Node_->setRotationMatrix(Rotation.getMatrix());
    Node_->setScale(PrevSeq_->Scale + ( NextSeq_->Scale - PrevSeq_->Scale ) * Interpolation_);
}


/*
 * ======= Private: =======
 */

void AnimationNode::updateNextSequence()
{
    s32 NextSeqIndex = Sequence_ + AnimMode_;
    
    if (Sequence_ + AnimMode_ > End_)
        NextSeqIndex = Start_;
    else if (Sequence_ + AnimMode_ < 0)
        NextSeqIndex = End_;
    
    PrevSeq_ = &(SequenceList_[Sequence_]);
    NextSeq_ = &(SequenceList_[NextSeqIndex]);
}

void AnimationNode::updateSpline()
{
    if (Spline_ && !SequenceList_.empty())
    {
        dim::vector3df* Points = new dim::vector3df[SequenceList_.size() + 1];
        
        u32 i = 0;
        for (std::vector<SAnimKeyframe>::const_iterator it = SequenceList_.begin(); it != SequenceList_.end(); ++it, ++i)
            Points[i] = it->Translation;
        Points[i] = SequenceList_[0].Translation;
        
        Spline_->create(Points, SequenceList_.size() + 1);
        
        delete [] Points;
    }
}

Animation* AnimationNode::copy(SceneNode* NewNode) const
{
    AnimationNode* NewAnim = new AnimationNode(NewNode);
    
    copyRoot(NewAnim);
    
    NewAnim->SplineExpansion_   = SplineExpansion_;
    NewAnim->SequenceList_      = SequenceList_;
    NewAnim->setSplineTranslation(getSplineTranslation());
    
    return NewAnim;
}


} // /namespace scene

} // /namespace sp







// ================================================================================
