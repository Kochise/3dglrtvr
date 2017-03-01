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
#ifndef GMANIMATIONSEGMENT_H
#define GMANIMATIONSEGMENT_H

#include <glam/GmTranslation.h>
#include <glam/GmRotation.h>
#include <glam/GmScale.h>

/**
 * An animation segment defines a set of motions (translation,
 * rotation and scale) that occur over a certain period of time. The
 * animation segment is usually chained together with other animation
 * segments to provide complex movements for glam components.
 */
class GmAnimationSegment
{
 protected:
   /**
    * The rotation movement associated with this animation
    * segment. This controls how the object rotates in the X, Y, and Z
    * axes.
    */
   GmRotation* mRotation;
   
   /**
    * The scale changes associated with this animation segment. This
    * member variable controls how the object scales in the X, Y and Z
    * directions during the animation segment.
    */
   GmScale* mScale;

   /**
    * The translation movement associated with this animation segment. This
    * value describes how the object translates in the X, Y and Z
    * directions during the animation segment.
    */
   GmTranslation* mTranslation;

   /**
    * The length in seconds of this animation segment
    */
   float mLengthInSeconds;
   
  public:
   /**
    * Standard constructor
    */
   GmAnimationSegment();

   /**
    * Standard destructor
    */
   virtual ~GmAnimationSegment();

   /**
    * Sets the temporal length of the animation segment in seconds.
    *
    * @param seconds the length of the animation segment in seconds.
    */
   virtual void setLength(float seconds);

   /**
    * Gets the temporal length of the animation segment.
    *
    * @return the temporal length of the animation segment in seconds.
    */
   virtual float getLength();

   /**
    * Sets the rotation motion for this animation segment.
    *
    * @param rotation the rotation motion for this animation. DO NOT
    *                 DELETE THE GIVEN POINTER AS IT IS USED AS-IS. THE
    *                 ANIMATION SEGMENT WILL DELETE IT WHEN IT IS DONE
    *                 WITH IT.
    */
   void setRotation(GmRotation* rotation);

   /**
    * Gets the rotation motion for this animation segment.
    *
    * @returns the rotation motion for this animation segment. DO NOT
    *          DELETE THE POINTER THAT THIS METHOD RETURNS, IT IS A
    *          POINTER TO A CLASS MEMBER.
    */
   GmRotation* getRotation();

   /**
    * Sets the scaling motion for this animation segment.
    *
    * @param scale the scaling motion for this animation segment. DO NOT
    *              DELETE THE GIVEN POINTER AS IT IS USED AS-IS. THE
    *              ANIMATION SEGMENT WILL DELETE IT WHEN IT IS DONE
    *              WITH IT.
    */
   void setScale(GmScale* scale);

   
   /**
    * Gets the scaling motion for this animation segment.
    *
    * @returns the scaling motion for this animation segment. DO NOT
    *          DELETE THE POINTER THAT THIS METHOD RETURNS, IT IS A
    *          POINTER TO A CLASS MEMBER.
    */
   GmScale* getScale();
   
   /**
    * Sets the translation motion for this animation segment.
    *
    * @param translation the translation motion for this animation
    *                    segment. DO NOT DELETE THE GIVEN POINTER AS
    *                    IT IS USED AS-IS. THE ANIMATION SEGMENT WILL
    *                    DELETE IT WHEN IT IS DONE WITH IT.
    */
   void setTranslation(GmTranslation* translation);
   
   /**
    * Gets the translation motion for this animation segment.
    *
    * @returns the translation motion for this animation segment. DO NOT
    *          DELETE THE POINTER THAT THIS METHOD RETURNS, IT IS A
    *          POINTER TO A CLASS MEMBER.
    */
   GmTranslation* getTranslation();
   
   /**
    * Applies all translation, scaling and rotation associated with
    * this animation segment at the given time period.
    *
    * @param secondsElapsed the number of seconds that have elapsed in
    *                       the animation segment.
    */
   virtual void applyAnimationTransforms(float secondsElapsed);
};

#endif
