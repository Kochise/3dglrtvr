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
#include <glam/GmAnimationSegment.h>

GmAnimationSegment::GmAnimationSegment()
{
   mRotation = NULL;
   mScale = NULL;
   mTranslation = NULL;
}

GmAnimationSegment::~GmAnimationSegment()
{
   if(mRotation != NULL)
      delete mRotation;
   if(mScale != NULL)
      delete mScale;
   if(mTranslation != NULL)
      delete mTranslation;
}

void GmAnimationSegment::setLength(float seconds)
{
   mLengthInSeconds = seconds;
}

float GmAnimationSegment::getLength()
{
   return mLengthInSeconds;
}

void GmAnimationSegment::setRotation(GmRotation* rotation)
{
   if(mRotation != NULL)
      delete mRotation;

   mRotation = rotation;
}

GmRotation* GmAnimationSegment::getRotation()
{
   return mRotation;
}

void GmAnimationSegment::setScale(GmScale* scale)
{
   if(mScale != NULL)
      delete mScale;

   mScale = scale;
}

GmScale* GmAnimationSegment::getScale()
{
   return mScale;
}

void GmAnimationSegment::setTranslation(GmTranslation* translation)
{
   if(mTranslation != NULL)
      delete mTranslation;

   mTranslation = translation;
}

GmTranslation* GmAnimationSegment::getTranslation()
{
   return mTranslation;
}

void GmAnimationSegment::applyAnimationTransforms(float secondsElapsed)
{
   float t;

   t = secondsElapsed/mLengthInSeconds;
   
   if(mTranslation != NULL)
      mTranslation->applyTransform(t);
   if(mRotation != NULL)
      mRotation->applyTransform(t);
   if(mScale != NULL)
      mScale->applyTransform(t);
}
