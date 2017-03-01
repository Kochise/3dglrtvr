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
#ifndef GMSCALE_H
#define GMSCALE_H

#include <glmath/GlmPath3.h>
#include <glam/GmTransform.h>

/**
 * An scale animation primitive.
 */
class GmScale : public GmTransform
{
 protected:
   //// The X, Y, and Z scale functions for this scale transformation
   GlmPath3* mScaleSpline;
   
 public:
   /**
    * Standard constructor
    */
   GmScale();

   /**
    * Standard destructor
    */
   virtual ~GmScale();
   
   /**
    * Applies OpenGL scaling transformations in the X, Y, and Z
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
    * Description
    */
   virtual void setScaleSpline(GlmPath3* scaleSpline); 
};

#endif
