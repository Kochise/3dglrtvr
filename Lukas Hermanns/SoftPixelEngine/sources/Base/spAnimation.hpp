/*
 * Animation header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_ANIMATION_H__
#define __SP_ANIMATION_H__


#include "Base/spStandard.hpp"
#include "Base/spInputOutputString.hpp"
#include "Base/spDimension.hpp"


namespace sp
{
namespace scene
{


/*
 * Pre-declerations
 */

class SceneNode;
class Mesh;


/*
 * Enumerations
 */

enum EAnimationTypes
{
    ANIMATION_NODE,
    ANIMATION_MORPHTARGET,
    ANIMATION_SKELETAL,
    ANIMATION_FACIAL,
};

enum EAnimMethods
{
    ANIM_ONESHOT,
    ANIM_ONELOOP,
    ANIM_LOOP,
    ANIM_PINGPONG,
    ANIM_PINGPONG_LOOP,
};

enum EAnimModes
{
    ANIM_FORWARDS   = 1,
    ANIM_STOP       = 0,
    ANIM_BACKWARDS  = -1
};


/*
 * Structures
 */

struct SAnimKeyframe
{
    SAnimKeyframe(
        const dim::vector3df Pos = 0.0f, const dim::quaternion Rot = dim::quaternion(),
        const dim::vector3df Scl = 1.0f, const f32 FrameSpeed = 0.01f)
        : Speed(FrameSpeed), Translation(Pos), Rotation(Rot), Scale(Scl)
    {
    }
    ~SAnimKeyframe()
    {
    }
    
    /* Members */
    f32 Speed;
    dim::vector3df Translation;
    dim::quaternion Rotation;
    dim::vector3df Scale;
};


/*
 * Animation class
 * -> Parent class of the other animation types (node-/ morph-target-/ skeletal- and facial animations)
 */

class SP_EXPORT Animation
{
    
    public:
        
        virtual ~Animation();
        
        /* === Functions === */
        
        /**
        Starts the animation.
        \param AnimMethod: Method which specifies how the animation is to be processed.
        There are multiple methods available:
        ANIM_ONESHOT (animates from Start to End),
        ANIM_ONELOOP (animates from Start to End and back to the origin),
        ANIM_LOOP (animates from Start to End and repeats this process until the animation has been stoped),
        ANIM_PINGPONG (animates from Start to End and from End to Start),
        ANIM_PINGPONG_LOOP (animates from Start to End and from End to Start and
        repeats this process endless until the animation has been stoped),
        \param AnimMode: Animation's mode which specifies if the animation shall run forwards or backwards.
        The feature backwards (ANIM_BACKWARDS) is not only available for skeletal animation.
        \param Start: First frame of the animation keyframes. By default -1 which means also the first frame (Normally 0).
        \param End: Last frame of the animation keyframes. By default -1 which means the last keyframe in the whole list.
        \param UseIndividualRange: If true each bone in a skeletal animation use its own animation range.
        */
        virtual void play(
            const EAnimMethods AnimMethod = ANIM_ONESHOT, const EAnimModes AnimMode = ANIM_FORWARDS,
            s32 Start = -1, s32 End = -1, bool UseIndividualRange = false
        );
        
        //! Pauses the animation.
        virtual void pause(bool Paused = true);
        
        //! Stops the animation.
        virtual void stop();
        
        virtual void setSeek(s32 Sequence, f32 Interpolation);
        virtual void getSeek(s32 &Sequence, f32 &Interpolation) const;
        virtual void setSeek(f32 State);
        virtual f32 getSeek() const;
        
        //! \return Count of keyframes.
        virtual s32 getLength() const;
        
        /**
        Updates the animation process. This function is called in the "updateAnimations" function of the
        SceneManager class.
        \param DirectUpdate: If true the interpolation of each bone in a skeletal animation is not processed.
        This is useful when updating a skeletal animated 3D model where the bones are transformed manual and not
        by their keyframes.
        */
        virtual void updateAnimation(const bool DirectUpdate = false) = 0;
        
        /* === Inline functions === */
        
        inline EAnimationTypes getType() const
        {
            return Type_;
        }
        
        //! Sets the animation's name.
        inline void setName(const io::stringc &Name)
        {
            Name_ = Name;
        }
        inline io::stringc getName() const
        {
            return Name_;
        }
        
        /* Animation status */
        
        //! \return True if the animation is running.
        inline bool animating() const
        {
            return AnimMode_ != ANIM_STOP;
        }
        
        //! Sets the animation's speed. By default 1.0.
        inline void setSpeed(f32 Speed)
        {
            Speed_ = Speed;
        }
        inline f32 getSpeed() const
        {
            return Speed_;
        }
        
        inline void setAnimMethod(const EAnimMethods AnimMethod)
        {
            AnimMethod_ = AnimMethod;
        }
        inline EAnimMethods getAnimMethod() const
        {
            return AnimMethod_;
        }
        
        inline void setAnimMode(const EAnimModes AnimMode)
        {
            AnimMode_ = AnimMode;
        }
        inline EAnimModes getAnimMode() const
        {
            return AnimMode_;
        }
        
    protected:
        
        friend class SceneNode;
        
        /* Functions */
        
        Animation(EAnimationTypes Type);
        
        virtual void updateSequence(f32 Speed);
        virtual void updateSeekStatus(s32 &Sequence, f32 &Interpolation);
        
        virtual void updateNextSequence();
        
        virtual Animation* copy(SceneNode* NewNode) const = 0;
        virtual void copyRoot(Animation* NewAnim) const;
        
        /* Members */
        
        EAnimationTypes Type_;
        
        io::stringc Name_;
        
        EAnimMethods AnimMethod_;   // Oneshot/ loop etc.
        EAnimModes AnimMode_;       // Forewards/ backwards/ stop
        EAnimModes AnimPausedMode_;
        
        f32 Speed_, Interpolation_; // Speed & interpolation status
        s32 Start_, End_;           // Start- & end sequence
        s32 Sequence_;              // Current sequence
        
    private:
        
        /* Functions */
        
        void init();
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
