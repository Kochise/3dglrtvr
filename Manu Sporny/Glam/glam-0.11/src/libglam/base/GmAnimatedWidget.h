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
#ifndef GMANIMATEDWIDGET_H
#define GMANIMATEDWIDGET_H

#include <glam/GmTypes.h>
#include <glam/GmAnimation.h>
#include <glam/GmAnimationEventHandler.h>
#include <glam/GmDrawable.h>

/**
 * The animated widget class encapsulates all animation behavior for
 * glam components.
 */
class GmAnimatedWidget : public GmDrawable, public GmAnimationEventHandler
{
 protected:
   /**
    * The current animation type that is being played back.
    */
   GmTypes::AnimationType mCurrentAnimation;
   
   /**
    * The appear animation is performed when the setVisible(true)
    * method is called upon the GLAM component.
    */
   GmAnimation* mAppearAnimation;
   
   /**
    * The vanish animation is performed when the setVisible(false)
    * method is called upon the GLAM component.
    */
   GmAnimation* mVanishAnimation;

   /**
    * The idle animation is played whenever this GLAM component is
    * just sitting around doing nothing.
    */
   GmAnimation* mIdleAnimation;

 public:
   /**
    * Standard constructor
    */
   GmAnimatedWidget();

   /**
    * Standard destructor
    */
   virtual ~GmAnimatedWidget();

   /**
    * Sets the animation to use for a particular animation type.
    * Animation types can include mouse button presses,
    * keyboard presses, idle loops, and other various times in which
    * an animation would play.
    *
    * @param animType the animation type to associate with the given
    *                 animation.
    * @param animation the animation to associate with the given
    *                  animation type.
    */
   virtual void setEventAnimation(GmTypes::AnimationType animType,
                                  GmAnimation* animation);

   /**
    * Applies all animation transforms for the current component based
    * on the current animation that is active.
    */
   virtual void applyAnimationTransforms();

   /**
    * This is a callback that is called whenever an animation
    * completes running. The animation type is returned to denote
    * which animation has completed running.
    */
   virtual void animationFinished(GmTypes::AnimationType animationType);
};

#endif
