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
#include <glam/GmScalarWidget.h>

GmScalarWidget::GmScalarWidget()
      : GmInteractiveWidget()
{
   mMinimumValue = 0.0;
   mMaximumValue = 0.0;
   mCurrentValue = 0.0;
   mCurrentPercent = 0.0;
   mLineStep = 0.0;
}

GmScalarWidget::~GmScalarWidget()
{
}

void GmScalarWidget::fireChangeOccurred(GmScalarAction* action)
{
   vector<GmScalarListener*>::iterator i;
   for(i = mScalarListeners.begin(); i != mScalarListeners.end(); i++)
      (*i)->changeOccurred(action);
}

void GmScalarWidget::addScalarListener(GmScalarListener* sl)
{
   mScalarListeners.push_back(sl);
}

bool GmScalarWidget::removeScalarListener(GmScalarListener* sl)
{
   vector<GmScalarListener*>::iterator i;
   for(i = mScalarListeners.begin(); i != mScalarListeners.end(); i++)
   {
      // compare the listeners
      if(*(*i) == *sl)
      {
         mScalarListeners.erase(i);
         return true;
      }
   }

   return false;
}

void GmScalarWidget::setMinimumValue(float minValue)
{
   mMinimumValue = minValue;

   if(mCurrentValue < mMinimumValue)
      setCurrentValue(mMinimumValue);
}

float GmScalarWidget::getMinimumValue()
{
   return mMinimumValue;
}

void GmScalarWidget::setMaximumValue(float maxValue)
{
   mMaximumValue = maxValue;

   if(mCurrentValue > mMaximumValue)
      setCurrentValue(mMaximumValue);
}

float GmScalarWidget::getMaximumValue()
{
   return mMaximumValue;
}

void GmScalarWidget::setCurrentValue(float currentValue)
{
   if(currentValue <= mMaximumValue && currentValue >= mMinimumValue)
   {
      mCurrentValue = currentValue;
      mCurrentPercent = mCurrentValue / (mMaximumValue - mMinimumValue);
   }
   else if(currentValue < mMinimumValue)
   {
      mCurrentValue = mMinimumValue;
      mCurrentPercent = 0.0;
   }
   else if(currentValue > mMaximumValue)
   {
      mCurrentValue = mMaximumValue;
      mCurrentPercent = 1.0;
   }
}

float GmScalarWidget::getCurrentValue()
{
   return mCurrentValue;
}

void GmScalarWidget::setRange(float minValue, float maxValue)
{
   setMinimumValue(minValue);
   setMaximumValue(maxValue);
}

void GmScalarWidget::setLineStep(float lineStep)
{
   mLineStep = lineStep;
}

float GmScalarWidget::getLineStep()
{
   return mLineStep;
}

void GmScalarWidget::setPageStep(float pageStep)
{
   mPageStep = pageStep;
}

float GmScalarWidget::getPageStep()
{
   return mPageStep;
}

void GmScalarWidget::setSteps(float lineStep, float pageStep)
{
   setLineStep(lineStep);
   setPageStep(pageStep);
}

void GmScalarWidget::addPage()
{
   float newValue = getCurrentValue()+getPageStep();
   if(newValue < getMaximumValue())
      setCurrentValue(newValue);
   else
      setCurrentValue(getMaximumValue());
}

void GmScalarWidget::subtractPage()
{
   float newValue = getCurrentValue()-getPageStep();
   if(newValue > getMinimumValue())
      setCurrentValue(newValue);
   else
      setCurrentValue(getMinimumValue());
}

void GmScalarWidget::addLine()
{
   float newValue = getCurrentValue()+getLineStep();
   if(newValue < getMaximumValue())
      setCurrentValue(newValue);
   else
      setCurrentValue(getMaximumValue());
}

void GmScalarWidget::subtractLine()
{
   float newValue = getCurrentValue()-getLineStep();
   if(newValue > getMinimumValue())
      setCurrentValue(newValue);
   else
      setCurrentValue(getMinimumValue());
}

float GmScalarWidget::getCurrentPercentage()
{
   return (mCurrentValue-mMinimumValue)/(mMaximumValue-mMinimumValue);
}

void GmScalarWidget::setCurrentPercentage(float currentPercentage)
{
   if(currentPercentage <= 1.0 && currentPercentage >= 0.0)
   {
      mCurrentPercent = currentPercentage;
      setCurrentValue(mCurrentPercent * (mMaximumValue - mMinimumValue));
   }
   else if (currentPercentage < 0.0)
   {
      mCurrentPercent = 0.0;
      setCurrentValue(mMinimumValue);
   }
   else if (currentPercentage > 1.0)
   {
      mCurrentPercent = 1.0;
      setCurrentValue(mMaximumValue);
   }
}
