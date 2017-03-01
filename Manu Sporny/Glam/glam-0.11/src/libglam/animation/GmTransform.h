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
#ifndef GMTRANSFORM_H
#define GMTRANSFORM_H

#include <glmath/GlmPath2.h>

/**
 * The base class for all animation primitives in GLAM. All animation
 * primitives must be able to perform a specific set of OpenGL
 * transformations given a certain percentage of elapsed time. All
 * animations also have a velocity curve to determine how fast they
 * transition from one part of the path to the next.
 */
class GmTransform
{
  protected:
   /**
    * The velocity curve affects the speed at which an object
    * accelerates and decelerates during the course of a
    * transform. For example, how quickly an object translates from
    * point A to point B can be affected by the velocity curve and is
    * completely separate from the route the object takes from point A
    * to point B. If the velocity curve is not set, a constant
    * transformation velocity is assumed.
    */
   GlmPath2* mVelocityCurve;

  public:
   /**
    * Standard constructor
    */
   GmTransform();

   /**
    * Standard virtual destructor
    */
   virtual ~GmTransform();
   
   /**
    * Applies an OpenGL transformation given the percentage of the
    * total transformation curve. For example, if
    * applyTransformation(0.5) is called, the transformation
    * calculation is performed based on a value at 50% of the
    * transformation and velocity curves.
    *
    * @param percentageElapsed the percentage elapsed of the
    *                          total transformation path
    */
   virtual void applyTransform(float percentageElapsed) = 0;

   /**
    * Sets the velocity curve to the given GlmPath2 object, which
    * affects the final position of the object. For example, how
    * quickly an object translates from point A to point B can be
    * affected by the velocity curve and is completely separate from
    * the route the object takes from point A to point B.
    *
    * @param velocityCurve the curve defining acceleration and
    *                      deceleration points in the
    *                      transformation. DO NOT DELETE THE GIVEN
    *                      VELOCITY CURVE OBJECT, IT IS USED DIRECTLY
    *                      AND WILL BE DESTROYED WHEN THIS GMTRANSFORM
    *                      OBJECT IS DESTROYED.
    */
   virtual void setVelocityCurve(GlmPath2* velocityCurve);
   
   /**
    * Gets the velocity curve for this transformation object.
    *
    * @return the curve defining acceleration and deceleration points in the
    *         transformation. DO NOT DELETE THE RETURNED VELOCITY
    *         CURVE OBJECT. It is a pointer to the classes data.
    */
   virtual GlmPath2* getVelocityCurve();
};

#endif
