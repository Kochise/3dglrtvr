/*
 * Copyright (c) 2002-2004, xRhino, Inc.
 *
 * This library is free software; you can redistribute and/or modify
 * it under the terms of the dual license distributed with this source
 * package (a combined Trolltech QPL/GPL-compatible license)
 *
 * Under this dual license scheme, a separate license is available for use of 
 * this code with a proprietary (closed) system. For more information on the 
 * license, please see the LICENSE file distributed with this source package.
 */
#ifndef GMANIMATION_H
#define GMANIMATION_H

#include <glam/GmTypes.h>
#include <glam/GmAnimationSegment.h>
#include <glam/GmAnimationEventHandler.h>

/**
 * The animation class encapsulates a set of animation segments and
 * provides complex translation, rotation and scaling movements for
 * visual glam components
 */
class GmAnimation
{
 protected:
   /**
    * The type of animation this animation object encompasses.
    */
   GmTypes::AnimationType mAnimationType;
   
   /**
    * A set of animation segments that constitute a complete animation.
    */
   vector<GmAnimationSegment*> mSegments;
   
   /**
    * A set of animation event handlers that have registered
    * themselves with this animation. The animation event handlers are
    * notified whenever a particular animation event occurs (such as
    * when an animation finishes playing).
    */
   vector<GmAnimationEventHandler*> mAnimHandlers;

   /**
    * The time that this animation started playback, the value is in
    * milliseconds since application started.
    */
   unsigned int mStartTime;

   /**
    * Total animation length in milliseconds
    */
   unsigned int mAnimLength;

   /**
    * Animation segment time codes for the end of each animation
    * segment. These values allow us to determine the correct
    * animation segment to evaluate for the current time.
    */
   unsigned int* mAnimTimecodes[2];

   /**
    * The current timecode offset, which is the current animation
    * segment being played back. This member variable is used to keep
    * track of which animation segment we are currently evaluating.
    */
   unsigned int mTimecodeOffset;
   
   /**
    * Recalculates the length of the animation as well as several
    * internal variables that are used for housekeeping.
    */
   virtual void recalculateAnimLength();
   
 public:
   /**
    * Standard constructor.
    *
    * @param animationType the type of animation that this is (mouse
    *                      press, idle, keypress, focus, etc)
    */
   GmAnimation(GmTypes::AnimationType animationType);

   /**
    * Standard destructor
    */
   virtual ~GmAnimation();

   /**
    * Starts running this animation using the current time as the
    * starting time. The animation will run until the total length of
    * the animation has elapsed, at which point isFinished() will
    * return true. If the animation has already started running, it
    * will restart from the beginning.
    */
   virtual void startAnimation();

   /**
    * The current state of animation playback can be determined by
    * calling this method.
    *
    * @return true if the animation has finished playback, false if it
    *         has not.
    */
   virtual bool isFinished();

   /**
    * Gets the total temporal length of this animation in
    * seconds. This value is a sum of all the length of the animation
    * segments that are part of this animation.
    *
    * @return the total temporal length of this animation in seconds.
    */
   float getLength();

   /**
    * Applies all animation motions associated with this animation.
    */
   virtual void applyAnimationTransforms();

   /**
    * Adds an animation segment to the end of the animation.
    *
    * @param animationSegment the animation segment to add to the end
    *                         of this animation.
    */
   virtual void addSegment(GmAnimationSegment* animationSegment);

   /**
    * Registers an animation event handler with this animation. An
    * animation event handler processes animation-based events such as
    * animation start, completion, etc.
    *
    * @param eventHandler the handler to register with this animation.
    */
   virtual void registerAnimHandler(GmAnimationEventHandler* eventHandler);

   /**
    * Unregisters an animation event handler from this animation. An
    * animation event handler processes animation-based events such as
    * animation start, completion, etc.
    *
    * @param eventHandler the animation event handler to unregister
    */
   virtual void unregisterAnimHandler(GmAnimationEventHandler* eventHandler);
};

#endif
