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
#ifndef GMROTATION_H
#define GMROTATION_H

#include <glmath/GlmPath3.h>
#include <glam/GmTransform.h>

/**
 * An rotation animation primitive.
 */
class GmRotation: public GmTransform
{
 protected:
   //// The amount of rotation to perform over a given time interval
   GlmPath3* mRotationSpline;
   
 public:
   /**
    * Standard constructor
    */
   GmRotation();

   /**
    * Standard destructor
    */
   virtual ~GmRotation();
   
   /**
    * Applies an OpenGL rotation transformations in the X, Y, and Z
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
   void setRotationSpline(GlmPath3* rotationSpline);
};

#endif
