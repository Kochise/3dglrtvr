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
#include <GL/gl.h>
#include <glam/GmRotation.h>

GmRotation::GmRotation()
{
   mRotationSpline = NULL;
}

GmRotation::~GmRotation()
{
   if(mRotationSpline != NULL)
      delete mRotationSpline;
}

void GmRotation::applyTransform(float percentageElapsed)
{
   GlmPoint3 rotation;
   GlmPoint2 velocity;
   float x, y, z, v, dummy;
   
   if(mVelocityCurve != NULL)
   {
      mVelocityCurve->getPositionByPercentage(percentageElapsed);
      velocity.getXY(dummy, v);
   }
   else
      v = 1.0;

   if(mRotationSpline != NULL)
   {
      rotation =
         mRotationSpline->getPositionByPercentage(percentageElapsed * v);
      rotation *= 360.0;
      rotation.getXYZ(x, y, z);
   
      glRotatef(x, 1.0, 0.0, 0.0);
      glRotatef(y, 0.0, 1.0, 0.0);
      glRotatef(z, 0.0, 0.0, 1.0);
   }
}

void GmRotation::setRotationSpline(GlmPath3* rotationSpline)
{
   if(mRotationSpline != NULL)
      delete mRotationSpline;
   
   mRotationSpline = rotationSpline;
}
