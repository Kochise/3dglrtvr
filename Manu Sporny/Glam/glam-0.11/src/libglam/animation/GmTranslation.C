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
#include <glam/GmTranslation.h>

GmTranslation::GmTranslation()
{
   mTranslationSpline = NULL;
}

GmTranslation::~GmTranslation()
{
   if(mTranslationSpline != NULL)
      delete mTranslationSpline;
}

void GmTranslation::applyTransform(float percentageElapsed)
{
   GlmPoint3 translation;
   GlmPoint2 velocity;
   float x, y, z, v, dummy;

   if(mVelocityCurve != NULL)
   {
      mVelocityCurve->getPositionByPercentage(percentageElapsed);
      velocity.getXY(dummy, v);
   }
   else
      v = 1.0;

   if(mTranslationSpline != NULL)
   {
      translation =
         mTranslationSpline->getPositionByPercentage(percentageElapsed * v);
      translation.getXYZ(x, y, z);

      glTranslatef(x, y, z);
   }
}

void GmTranslation::setTranslationSpline(GlmPath3* translationSpline)
{
   if(mTranslationSpline != NULL)
      delete mTranslationSpline;
   
   mTranslationSpline = translationSpline;
}

GlmPath3* GmTranslation::getTranslationSpline()
{
   return mTranslationSpline;
}
