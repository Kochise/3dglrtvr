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
#include <glam/GmAnimatedWidget.h>

GmAnimatedWidget::GmAnimatedWidget() 
      : GmDrawable()
{
   mAppearAnimation = NULL;
   mVanishAnimation = NULL;
   mIdleAnimation = NULL;
   mCurrentAnimation = GmTypes::AT_IDLE;
}

GmAnimatedWidget::~GmAnimatedWidget()
{
   if(mAppearAnimation != NULL)
      delete mAppearAnimation;

   if(mVanishAnimation != NULL)
      delete mVanishAnimation;

   if(mIdleAnimation != NULL)
      delete mIdleAnimation;
}

void GmAnimatedWidget::setEventAnimation(GmTypes::AnimationType animType,
                                         GmAnimation* animation)
{
   switch(animType)
   {
      case GmTypes::AT_APPEAR:
         if(mAppearAnimation != NULL)
            delete mAppearAnimation;
         mAppearAnimation = animation;
         break;
      case GmTypes::AT_VANISH:
         if(mVanishAnimation != NULL)
            delete mVanishAnimation;
         mVanishAnimation = animation;
         break;
      case GmTypes::AT_IDLE:
         if(mIdleAnimation != NULL)
            delete mIdleAnimation;
         mIdleAnimation = animation;
         break;
      default:
         cout << "GmAnimatedWidget::setEventAnimation(): "
              << "Unknown animation type" << endl;
         break;
   }
   
   animation->registerAnimHandler(this);
}

void GmAnimatedWidget::applyAnimationTransforms()
{
   switch(mCurrentAnimation)
   {
      case GmTypes::AT_APPEAR:
         if(mAppearAnimation != NULL)
            mAppearAnimation->applyAnimationTransforms();
         break;
      case GmTypes::AT_VANISH:
         if(mVanishAnimation != NULL)
            mVanishAnimation->applyAnimationTransforms();
         break;
      case GmTypes::AT_IDLE:
         if(mIdleAnimation != NULL)
            mIdleAnimation->applyAnimationTransforms();
         break;
      default:
         break;
   }
}

void GmAnimatedWidget::animationFinished(GmTypes::AnimationType animationType)
{
   switch(animationType)
   {
      case GmTypes::AT_IDLE:
         if(mIdleAnimation != NULL)
            mIdleAnimation->startAnimation();
         break;
      default:
         break;
   }
}
