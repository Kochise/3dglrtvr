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
#include <glam/GmOptions.h>
#include <glam/GmScrollbar.h>

GmScrollbar::GmScrollbar()
{
   GmOptions* gmOptions = GmOptions::getInstance();

   mIncrementButton = new GmPushButton();
   mDecrementButton = new GmPushButton();
   mSlider = new GmSlider();

   setOrientation(GmTypes::OT_VERTICAL);
   setRange(0.0, 1.0);
   setSteps(0.1, 0.2);
   
   mIncrementButton->setWidgetId(SCROLL_INCREMENT);
   mDecrementButton->setWidgetId(SCROLL_DECREMENT);
   mSlider->setWidgetId(SCROLL_SLIDER);

   mIncrementButton->addButtonListener(this);
   mDecrementButton->addButtonListener(this);
   mSlider->addScalarListener(this);

   mIncrementButton->setFocusParent(this);
   mDecrementButton->setFocusParent(this);
   mSlider->setFocusParent(this);

   setBaseColor((GlmColor4*)gmOptions->getColorOption(GMO_SCROLLBAR_BASECOLOR));
   setBorderWidth(gmOptions->getScalarOption(GMO_SCROLLBAR_BORDERWIDTH));
}

GmScrollbar::~GmScrollbar()
{
   delete mIncrementButton;
   delete mDecrementButton;
   delete mSlider;
}

void GmScrollbar::setOrientation(GmTypes::OrientationType orientation)
{
   mOrientation = orientation;
   mSlider->setOrientation(orientation);
}

GmTypes::OrientationType GmScrollbar::getOrientation()
{
   return mOrientation;
}

void GmScrollbar::setCurrentValue(float currentValue)
{
   if(mCurrentValue == currentValue)
      return;
   
   GmScalarWidget::setCurrentValue(currentValue);
   mSlider->setCurrentValue(mCurrentValue);
}

void GmScrollbar::setMinimumValue(float minValue)
{
   GmScalarWidget::setMinimumValue(minValue);
   mSlider->setMinimumValue(minValue);
}

void GmScrollbar::setMaximumValue(float maxValue)
{
   GmScalarWidget::setMaximumValue(maxValue);
   mSlider->setMaximumValue(maxValue);
}

void GmScrollbar::setCurrentPercentage(float currentPercentage)
{
   GmScalarWidget::setCurrentPercentage(currentPercentage);
   mSlider->setCurrentPercentage(currentPercentage);
}

void GmScrollbar::setRange(float minValue, float maxValue)
{
   GmScalarWidget::setRange(minValue, maxValue);
   mSlider->setRange(minValue, maxValue);
}

void GmScrollbar::setLineStep(float lineStep)
{
   GmScalarWidget::setLineStep(lineStep);
   mSlider->setLineStep(lineStep);
}

void GmScrollbar::setPageStep(float pageStep)
{
   GmScalarWidget::setPageStep(pageStep);
   mSlider->setPageStep(pageStep);
}

void GmScrollbar::setBorderWidth(float borderWidth)
{
   mBorderWidth = borderWidth;
   switch(mOrientation)
   {
      case GmTypes::OT_HORIZONTAL:
         mIncrementButton->setBorderWidth(borderWidth);
         mDecrementButton->setBorderWidth(borderWidth);
         break;
      case GmTypes::OT_VERTICAL:
         mIncrementButton->setBorderWidth(borderWidth);
         mDecrementButton->setBorderWidth(borderWidth);
         break;
   }
}

float GmScrollbar::getBorderWidth()
{
   return mBorderWidth;
}

void GmScrollbar::buttonPressed(GmButtonAction* action)
{
   float change = getCurrentValue();
   
   switch(action->getSrcWidgetId())
   {
      case SCROLL_INCREMENT:
         addLine();
         break;
      case SCROLL_DECREMENT:
         subtractLine();
         break;
   }

   change -= getCurrentValue();

   GmScalarAction* sa = new GmScalarAction(this, getCurrentValue(), change);
   fireChangeOccurred(sa);
   delete sa;
}

void GmScrollbar::changeOccurred(GmScalarAction* action)
{
   if(action->getSrcWidgetId() != SCROLL_SLIDER)
      return;

   // set the new value
   setCurrentValue(action->getValue());

   // send the action to listeners
   GmScalarAction* scalarAction = new GmScalarAction(this, getCurrentValue(),
                                                     action->getChange());
   fireChangeOccurred(scalarAction);
   delete scalarAction;
}

void GmScrollbar::setGLPosition(float x, float y, float z)
{
   float dX, dY, dZ;
   mGLPosition.getXYZ(dX, dY, dZ);

   dX = x - dX;
   dY = y - dY;
   dZ = z - dZ;

   // set position
   GmDrawable::setGLPosition(x, y, z);

   // translate the scrollbar's components accordingly
   mIncrementButton->getGLPosition(x, y, z);
   mIncrementButton->setGLPosition(x + dX, y + dY, z + dZ);

   mDecrementButton->getGLPosition(x, y, z);
   mDecrementButton->setGLPosition(x + dX, y + dY, z + dZ);
   
   mSlider->getGLPosition(x, y, z);
   mSlider->setGLPosition(x + dX, y + dY, z + dZ);
}

void GmScrollbar::setRelativeGeometry(float minX, float minY, float minZ,
                                      float maxX, float maxY, float maxZ)
{
   GmDrawable::setRelativeGeometry(minX, minY, minZ,
                                   maxX, maxY, maxZ);

   if(mOrientation == GmTypes::OT_HORIZONTAL)
   {
      mIncrementButton->setRelativeGeometry(0.9, 0.0, 1.0, 1.0);
      mDecrementButton->setRelativeGeometry(0.0, 0.0, 0.1, 1.0);
      mSlider->setRelativeGeometry(0.1, 0.0, 0.9, 1.0);
   }
   else if(mOrientation == GmTypes::OT_VERTICAL)
   {
      mIncrementButton->setRelativeGeometry(0.0, 0.0, 1.0, 0.1);
      mDecrementButton->setRelativeGeometry(0.0, 0.9, 1.0, 1.0);
      mSlider->setRelativeGeometry(0.0, 0.1, 1.0, 0.9);
   }
}

void GmScrollbar::setRelativeGeometry(float minX, float minY,
                                      float maxX, float maxY)
{
   setRelativeGeometry(minX, minY, 0.0, maxX, maxY, 0.0);
}

void GmScrollbar::layoutInGLVolume(float minX, float minY, float minZ,
                                   float width, float height, float depth)
{
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);
   
   mIncrementButton->layoutInGLVolume(x, y, z + GLAM_LAYER,
                                      mGLWidth, mGLHeight, mGLDepth);

   mDecrementButton->layoutInGLVolume(x, y, z + GLAM_LAYER,
                                      mGLWidth, mGLHeight, mGLDepth);

   mSlider->layoutInGLVolume(x, y, z + GLAM_LAYER,
                             mGLWidth, mGLHeight, mGLDepth);
}

void GmScrollbar::setBaseColor(GlmColor4* color)
{
   float r, g, b, a;
   color->getRGBA(r, g, b, a);
   
   GmDrawable::setBaseColor(r, g, b, a);
   mIncrementButton->setBaseColor(r + 0.15, g + 0.15, b + 0.15, a);
   mDecrementButton->setBaseColor(r + 0.15, g + 0.15, b + 0.15, a);
   mSlider->setColor(r + 0.2, g + 0.2, b + 0.2, a);
}

void GmScrollbar::setBaseColor(float r, float g, float b, float a)
{
   GmDrawable::setBaseColor(r, g, b, a);
   mIncrementButton->setBaseColor(r + 0.15, g + 0.15, b + 0.15, a);
   mDecrementButton->setBaseColor(r + 0.15, g + 0.15, b + 0.15, a);
   mSlider->setColor(r + 0.2, g + 0.2, b + 0.2, a);
}

void GmScrollbar::render2d(GmTypes::RenderOptionType renderOptions)
{
   mIncrementButton->render(renderOptions);
   mDecrementButton->render(renderOptions);
   mSlider->render(renderOptions);
}

void GmScrollbar::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   mIncrementButton->render(renderOptions);
   mDecrementButton->render(renderOptions);
   mSlider->render(renderOptions);
}
