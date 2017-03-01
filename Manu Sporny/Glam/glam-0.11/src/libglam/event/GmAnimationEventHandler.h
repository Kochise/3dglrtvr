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
#ifndef GMANIMATIONEVENTHANDLER_H
#define GMANIMATIONEVENTHANDLER_H

#include <glam/GmTypes.h>

/**
 * The animation event handler handles all animation events that
 * occurr throughout the lifetime of an animation.
 */
class GmAnimationEventHandler
{
 public:
   /**
    * Standard constructor
    */
   GmAnimationEventHandler();

   /**
    * Standard destructor
    */
   virtual ~GmAnimationEventHandler();

   /**
    * This method is called whenever an animation is finished. This
    * animation event handler must register itself with the animation
    * of interest if it is going to recieve an animation complete
    * event.
    *
    * @param animationType the type of animation that completed animating.
    */
   virtual void animationFinished(GmTypes::AnimationType animationType);
};

#endif
