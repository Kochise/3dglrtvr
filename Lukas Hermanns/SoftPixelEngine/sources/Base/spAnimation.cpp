/*
 * Animation file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Base/spAnimation.hpp"


namespace sp
{
namespace scene
{


/*
 * Animation class
 */

Animation::Animation(EAnimationTypes Type) : Type_(Type)
{
    init();
}
Animation::~Animation()
{
}

void Animation::play(
    const EAnimMethods AnimMethod, const EAnimModes AnimMode, s32 Start, s32 End, bool UseIndividualRange)
{
    if (getLength() < 2)
        return;
    
    AnimMethod_ = AnimMethod;
    AnimMode_   = AnimMode;
    
    Start_      = (Start == -1 ? 0 : Start);
    End_        = (End == -1 ? getLength() - 1 : End);
    
    math::Clamp(Start_, 0, getLength() - 1);
    math::Clamp(End_, 0, getLength() - 1);
    
    updateNextSequence();
}

void Animation::pause(bool Paused)
{
    if (Paused)
    {
        AnimPausedMode_ = AnimMode_;
        AnimMode_ = ANIM_STOP;
    }
    else
        AnimMode_ = AnimPausedMode_;
}

void Animation::stop()
{
    AnimPausedMode_ = AnimMode_ = ANIM_STOP;
    Sequence_       = 0;
    Interpolation_  = 0.0f;
}

void Animation::setSeek(s32 Sequence, f32 Interpolation)
{
    if (Sequence >= 0 && Sequence < getLength())
        Sequence_ = Sequence;
    Interpolation_  = Interpolation;
}
void Animation::getSeek(s32 &Sequence, f32 &Interpolation) const
{
    Sequence        = Sequence_;
    Interpolation   = Interpolation_;
}

void Animation::setSeek(f32 State)
{
    State *= getLength();
    
    Sequence_       = (s32)State;
    Interpolation_  = State - floor(State);
}
f32 Animation::getSeek() const
{
    return getLength() ? (Interpolation_ + (f32)Sequence_) / getLength() : 0.0f;
}

s32 Animation::getLength() const
{
    return 0;
}


/*
 * ========== Private: ==========
 */

void Animation::init()
{
    /* General settings */
    AnimMethod_     = ANIM_ONESHOT;
    AnimMode_       = ANIM_STOP;
    AnimPausedMode_ = AnimMode_;
    Speed_          = 1.0f;
    Interpolation_  = 0.0f;
    Start_          = 0;
    End_            = 0;
    Sequence_       = 0;
}

void Animation::updateSequence(f32 Speed)
{
    
    /* Update the animation sequence and its interpolation */
    Interpolation_ += ( Speed_ * Speed ) * (f32)AnimMode_;
    
    if (AnimMode_ == ANIM_FORWARDS && Interpolation_ >= 1.0f)
    {
        updateSeekStatus(Sequence_, Interpolation_);
        
        if (Sequence_ > End_)
        {
            switch (AnimMethod_)
            {
                case ANIM_ONESHOT:
                    stop();
                    break;
                case ANIM_PINGPONG:
                case ANIM_PINGPONG_LOOP:
                    Sequence_ = End_ - 1;
                    AnimMode_ = ANIM_BACKWARDS;
                    break;
                case ANIM_LOOP:
                    Sequence_ = Start_;
            }
        }
        else if (Sequence_ == Start_ && AnimMethod_ == ANIM_ONELOOP)
            stop();
        
        updateNextSequence();
    }
    else if (AnimMode_ == ANIM_BACKWARDS && Interpolation_ <= 0.0f)
    {
        updateSeekStatus(Sequence_, Interpolation_);
        
        if (Sequence_ < Start_)
        {
            switch (AnimMethod_)
            {
                case ANIM_PINGPONG:
                    stop();
                    break;
                case ANIM_PINGPONG_LOOP:
                    Sequence_ = Start_ + 1;
                    AnimMode_ = ANIM_FORWARDS;
                    break;
            }
        }
        else if (Sequence_ == End_ && AnimMethod_ == ANIM_ONELOOP)
            stop();
        
        updateNextSequence();
    }
    
}

void Animation::updateSeekStatus(s32 &Sequence, f32 &Interpolation)
{
    s32 OutOfRange = (s32)floor(Interpolation + (AnimMode_ == ANIM_FORWARDS ? 0.0f : -1.0f));
    
    Sequence        += OutOfRange;
    Interpolation   -= OutOfRange;
}

void Animation::updateNextSequence() { }

void Animation::copyRoot(Animation* NewAnim) const
{
    NewAnim->Type_              = Type_;
    NewAnim->Name_              = Name_;
    
    NewAnim->AnimMethod_        = AnimMethod_;
    NewAnim->AnimMode_          = AnimMode_;
    NewAnim->AnimPausedMode_    = AnimPausedMode_;
    
    NewAnim->Speed_             = Speed_;
    NewAnim->Interpolation_     = Interpolation_;
    NewAnim->Start_             = Start_;
    NewAnim->End_               = End_;
    NewAnim->Sequence_          = Sequence_;
}


} // /namespace scene

} // /namespace sp



// ================================================================================
