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
#include <GL/glut.h>
#include <glam/GmAnimation.h>

#define TIME_CODES 0
#define MS_LENGTHS 1

GmAnimation::GmAnimation(GmTypes::AnimationType animationType)
{
   mAnimationType = animationType;
   mStartTime = 0;
   mAnimTimecodes[TIME_CODES] = (unsigned int*)malloc(4);
   mAnimTimecodes[MS_LENGTHS] = (unsigned int*)malloc(4);
}

GmAnimation::~GmAnimation()
{
   mSegments.clear();
   
   if(mAnimTimecodes[TIME_CODES] != NULL)
      free(mAnimTimecodes[TIME_CODES]);
   if(mAnimTimecodes[MS_LENGTHS] != NULL)
      free(mAnimTimecodes[MS_LENGTHS]);
}

void GmAnimation::recalculateAnimLength()
{
   int i = 0;
   unsigned int numSegments;
   mAnimLength = 0;

   // reallocate memory for the timecodes
   numSegments = mSegments.size();
   realloc(mAnimTimecodes[TIME_CODES], numSegments*sizeof(unsigned int));
   realloc(mAnimTimecodes[MS_LENGTHS], numSegments*sizeof(unsigned int));

   for(vector<GmAnimationSegment*>::iterator as = mSegments.begin();
       as != mSegments.end(); as++, i++)
   {
      unsigned int segmentLength;
      
      segmentLength = (unsigned int)((*as)->getLength() * 1000.0);
      mAnimLength += segmentLength;
      mAnimTimecodes[TIME_CODES][i] = mAnimLength;
      mAnimTimecodes[MS_LENGTHS][i] = segmentLength;
   }
}

void GmAnimation::startAnimation()
{
   mStartTime = glutGet(GLUT_ELAPSED_TIME);
   mTimecodeOffset = 0;
}

bool GmAnimation::isFinished()
{
   bool rval = false;

   if((glutGet(GLUT_ELAPSED_TIME) - mStartTime) > mAnimLength)
      rval = true;

   return rval;
}

float GmAnimation::getLength()
{
   return (float(mAnimLength) / 1000.0);
}

void GmAnimation::applyAnimationTransforms()
{
   unsigned int elapsedTime;

   elapsedTime = (glutGet(GLUT_ELAPSED_TIME) - mStartTime);

   if(elapsedTime > mAnimLength)
   {
      // We are at the end of the animation, keep playing the last frame
      GmAnimationSegment* as = mSegments[mSegments.size()-1];
      as->applyAnimationTransforms(as->getLength());

      // Distribute the animation finished event
      for(vector<GmAnimationEventHandler*>::iterator aeh =
             mAnimHandlers.begin(); aeh != mAnimHandlers.end(); aeh++)
         (*aeh)->animationFinished(mAnimationType);
   }
   else
   {
      // we are somewhere in the middle of the animation, keep playing.
      float secondsElapsed;
      
      if(elapsedTime > mAnimTimecodes[TIME_CODES][mTimecodeOffset])
         mTimecodeOffset++;

      if(mTimecodeOffset > 0)
         secondsElapsed = (float(elapsedTime)/1000.0) -
            (float(mAnimTimecodes[TIME_CODES][mTimecodeOffset])/1000.0);
      else
         secondsElapsed = float(elapsedTime)/1000.0;
      
      mSegments[mTimecodeOffset]->applyAnimationTransforms(secondsElapsed);
   }
}

void GmAnimation::addSegment(GmAnimationSegment* animationSegment)
{
   mSegments.push_back(animationSegment);
   recalculateAnimLength();   
}

void GmAnimation::registerAnimHandler(GmAnimationEventHandler* eventHandler)
{
   mAnimHandlers.push_back(eventHandler);
}

void GmAnimation::unregisterAnimHandler(GmAnimationEventHandler* eventHandler)
{
   for(vector<GmAnimationEventHandler*>::iterator aeh = mAnimHandlers.begin();
       aeh != mAnimHandlers.end(); aeh++)
   {
      // This could be incredibly stupid and wrong, but it works for
      // the time being. Pointer comparisons should be safe as long as
      // we're operating in the same memory space.
      if((*aeh) == eventHandler)
         mAnimHandlers.erase(aeh);
   }
}
