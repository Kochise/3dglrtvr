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
#include <glam/GmSlider.h>

GmSlider::GmSlider()
      : GmScalarWidget()
{
   mMinimumValue = 0.0;
   mMaximumValue = 1.0;
   mCurrentValue = 0.0;
   mCurrentPercent = 0.0;
   mLineStep = 0.1;
   mPageStep = 0.1;
   
   // create the slider button and listen to it
   mSliderButton = new GmSliderButton();
   mSliderButton->setWidgetId(SLIDER_BUTTON_ID);
   mSliderButton->addScalarListener(this);
   mSliderButton->setFocusParent(this);

   // default orientation is vertical
   setOrientation(GmTypes::OT_VERTICAL);
}

GmSlider::~GmSlider()
{
   if(mSliderButton)
      delete mSliderButton;
}

void GmSlider::setPageStep(float pageStep)
{
   GmScalarWidget::setPageStep(pageStep);
   resizeSliderButton();
   
   // layout the resized slider button
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   // get the dimensions for the slider button
   mSliderButton->layoutInGLVolume(x, y, z + GLAM_LAYER,
                                   mGLWidth, mGLHeight,
                                   mGLDepth);

   // reset the slider button's range
   setSliderButtonRange();

   // position the slider button
   setCurrentValue(mCurrentValue);
}

void GmSlider::setOrientation(GmTypes::OrientationType orientation)
{
   mOrientation = orientation;
   mSliderButton->setOrientation(orientation);
}

void GmSlider::setCurrentValue(float currentValue)
{
   GmScalarWidget::setCurrentValue(currentValue);
   
   float range = mSliderButton->getMaximumValue() -
      mSliderButton->getMinimumValue();

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   // get slider button's new value
   float newValue;
   if(mOrientation == GmTypes::OT_HORIZONTAL)
   {
      newValue = x + getCurrentPercentage() * range +
         mSliderButton->getGLWidth() / 2;
   }
   else if(mOrientation == GmTypes::OT_VERTICAL)
   {
      // if vertical, swap percentage, top is min, bottom is max
      newValue = y + (1.0 - getCurrentPercentage()) * range +
         mSliderButton->getGLHeight() / 2;
   }   

   // set the slider button's current value
   mSliderButton->setCurrentValue(newValue);
}

void GmSlider::setMinimumValue(float minValue)
{
   GmScalarWidget::setMinimumValue(minValue);
   resizeSliderButton();   
   setCurrentValue(getCurrentValue());
}

void GmSlider::setMaximumValue(float maxValue)
{
   GmScalarWidget::setMaximumValue(maxValue);
   resizeSliderButton();   
   setCurrentValue(getCurrentValue());
}

void GmSlider::setColor(float r, float g, float b, float a)
{
   mBaseColor.setRGBA(r, g, b, a);
   mSliderButton->setColor(r * 1.1, g * 1.1, b * 1.1, a);
}

float GmSlider::convertSliderButtonValue()
{
   float range = getMaximumValue() - getMinimumValue();
   float percentage = mSliderButton->getCurrentPercentage();

   // if vertical, swap percentage, top is min, bottom is max
   if(mOrientation == GmTypes::OT_VERTICAL)
      return (1.0 - percentage) * range;
   
   return percentage * range;
}

void GmSlider::setSliderButtonRange()
{
   // recalculate and resize the sliderbutton range
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);
   
   if(mOrientation == GmTypes::OT_HORIZONTAL)
   {
      float width = mSliderButton->getGLWidth() / 2;
      
      // set range of the button according to slider's coords
      mSliderButton->setRange(x + width,
                              x + mGLWidth - width);
   }
   else if(mOrientation == GmTypes::OT_VERTICAL)
   {
      float height = mSliderButton->getGLHeight() / 2;

      // set range of the button according to slider's coords
      mSliderButton->setRange(y + height,
                              y + mGLHeight - height);
   }
}

void GmSlider::mousePressEvent(GmEvent* event)
{
   GmInteractiveWidget::mousePressEvent(event);
   
   float x = event->getMouseData()->GLx;
   float y = event->getMouseData()->GLy;

   // set new coordinate according to orientation
   float newCoord = (mOrientation == GmTypes::OT_HORIZONTAL) ? x : y;

   // store the slider button's change
   float change = newCoord - mSliderButton->getCurrentValue();

   // return if no change
   if(change == 0)
      return;

   // set the slider button's new value
   mSliderButton->setCurrentValue(newCoord);

   // convert the slider button's value to slider's value
   float newValue = convertSliderButtonValue();

   // get the slider's change
   change = getCurrentValue() - newValue;

   // set the current value of the slider   
   GmScalarWidget::setCurrentValue(newValue);

   // send the action to listeners
   GmScalarAction* scalarAction = new GmScalarAction(this, getCurrentValue(),
                                                     change);
   fireChangeOccurred(scalarAction);
   delete scalarAction;
}

void GmSlider::changeOccurred(GmScalarAction* action)
{
   // get the new value for the slider
   float newValue = convertSliderButtonValue();

   // get the change in the slider's value
   float change = getCurrentValue() - newValue;

   // set the new value
   GmScalarWidget::setCurrentValue(newValue);

   // send the action to listeners
   GmScalarAction* scalarAction = new GmScalarAction(this, getCurrentValue(),
                                                     change);
   fireChangeOccurred(scalarAction);
   delete scalarAction;
}

void GmSlider::resizeSliderButton()
{
   // resize slider button
   float range = getMaximumValue() - getMinimumValue();

   float percent;
   if(range != 0.0)
      percent = getPageStep() / (range + getPageStep());
   else
      percent = 1.0;

   // set the slider and sliderbutton relative geometry
   if(mOrientation == GmTypes::OT_HORIZONTAL)
      mSliderButton->setRelativeGeometry(0.0, 0.0, percent, 1.0);
   else if(mOrientation == GmTypes::OT_VERTICAL)
      mSliderButton->setRelativeGeometry(0.0, 1.0 - percent, 1.0, 1.0);
}

void GmSlider::setGLPosition(float x, float y, float z)
{
   float dX, dY, dZ;
   mGLPosition.getXYZ(dX, dY, dZ);

   dX = x - dX;
   dY = y - dY;
   dZ = z - dZ;

   // set slider position
   GmDrawable::setGLPosition(x, y, z);

   // translate the slider button accordingly
   mSliderButton->getGLPosition(x, y, z);
   mSliderButton->setGLPosition(x + dX, y + dY, z + dZ);
}

void GmSlider::setRelativeGeometry(float minX, float minY, float minZ,
                                   float maxX, float maxY, float maxZ)
{
   GmDrawable::setRelativeGeometry(minX, minY, minZ, maxX, maxY, maxZ);
   resizeSliderButton();
}

void GmSlider::setRelativeGeometry(float minX, float minY,
                                   float maxX, float maxY)
{
   setRelativeGeometry(minX, minY, 0.0, maxX, maxY, 0.0);
}

void GmSlider::layoutInGLVolume(float minX, float minY, float minZ,
                                 float width, float height, float depth)
{
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);

   // recalculate and resize the sliderbutton range
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   // get the dimensions for the slider button
   mSliderButton->layoutInGLVolume(x, y, z + GLAM_LAYER,
                                   mGLWidth, mGLHeight, mGLDepth);

   // reset the slider button's range
   setSliderButtonRange();

   // position the slider button
   setCurrentValue(getCurrentValue());
}

void GmSlider::render2d(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   if(!getDisplayListValid())
   {
      if(mDisplayListId != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);

      float r, g, b, a;
      mBaseColor.getRGBA(r, g, b, a);
      
      // create display list
      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);   

      // draw background rectangle
      glColor4f(r, g, b, a);
      glBegin(GL_POLYGON);
      glVertex2f(0.0, 0.0);
      glVertex2f(mGLWidth, 0.0);
      glVertex2f(mGLWidth, mGLHeight);
      glVertex2f(0.0, mGLHeight);
      glEnd();

      glEndList();

      setDisplayListValid(true);      
   }
   
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);
   glCallList(getDisplayListId());
   glPopMatrix();

   // restore color settings
   glPopAttrib();
   
   // now draw sliderbutton
   if(mSliderButton != NULL)
      mSliderButton->render(renderOptions);
}

void GmSlider::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);
   
   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);
   // draw background rectangle
   glColor4f(1.0, 0.0, 1.0, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   glPopName();
   
   glPopMatrix();

   // restore color settings
   glPopAttrib();
   
   // draw sliderbutton
   if(mSliderButton != NULL)
      mSliderButton->render(renderOptions);
}
