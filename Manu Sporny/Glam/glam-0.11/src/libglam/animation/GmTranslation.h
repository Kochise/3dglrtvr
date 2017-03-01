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
#ifndef GMTRANSLATION_H
#define GMTRANSLATION_H

#include <glmath/GlmPath3.h>
#include <glam/GmTransform.h>

/**
 * The GmTranslation animation construct handles all object
 * translation and can be handled to make a particular animatable item
 * follow a pre-determined, or even real-time calculated parametric path.
 */
class GmTranslation : public GmTransform
{
 protected:
   /**
    * The translation spline evaluates what the X, Y and Z
    * translations should be at a given percentage of the entire path
    * travelled. For example: What is the X, Y and Z translation at
    * 50% of the total time spent during the animation.
    */
   GlmPath3* mTranslationSpline;
   
 public:
   /**
    * Standard constructor
    */
   GmTranslation();

   /**
    * Standard destructor
    */
   virtual ~GmTranslation();
   
   /**
    * Applies an OpenGL translation transformations in the X, Y, and Z
    * axis given the percentage of the total transformation curve.
    * For example, if applyTransformation(0.5) is called, the transformation
    * calculation is performed based on a value at 50% of the
    * transformation and velocity curves.
    *
    * @param percentageElapsed the percentage elapsed of the
    *                          total transformation path
    */
   virtual void applyTransform(float percentageElapsed);
   
   /**
    * Sets the translation spline to be used by this GmTranslation
    * object.DO NOT DELETE THE GIVEN VELOCITY CURVE OBJECT, IT IS USED
    * DIRECTLY AND WILL BE DESTROYED WHEN THIS GMTRANSFORM OBJECT IS
    * DESTROYED.
    *
    * @param translationSpline the translation spline to use to
    *                          perform the X, Y and Z translations.
    *                          DO NOT DELETE THE GIVEN VELOCITY CURVE
    *                          OBJECT, IT IS USED DIRECTLY AND WILL BE
    *                          DESTROYED WHEN THIS GMTRANSFORM OBJECT
    *                          IS DESTROYED.
    */
   void setTranslationSpline(GlmPath3* translationSpline);
   
   /**
    * Gets the translation spline that is being used by this
    * object. DO NOT DELETE/MODIFY THE RETURNED VALUE, IT IS A POINTER
    * TO THE ACTUAL DATA.
    *
    * @param translationSpline the translation spline to use to
    *                          perform the X, Y and Z translations
    */
   GlmPath3* getTranslationSpline();
};

#endif
