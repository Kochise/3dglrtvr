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
#include <glam/GmSliderButton.h>

GmSliderButton::GmSliderButton()
      : GmScalarWidget()
{
   mBorderSize = 0.4;
   mButtonIcon = NULL;
   mIconUsed = false;
   mMouseInRange = false;
}

GmSliderButton::~GmSliderButton()
{
}

void GmSliderButton::setOrientation(GmTypes::OrientationType orientation)
{
   mOrientation = orientation;
}

void GmSliderButton::setCurrentValue(float currentValue)
{
   if(mCurrentValue == currentValue)
      return;
   
   // sets current value which is at middle of button
   GmScalarWidget::setCurrentValue(currentValue);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   // convert from middle of button to position
   if(mOrientation == GmTypes::OT_HORIZONTAL)
      GmDrawable::setGLPosition(getCurrentValue() - mGLWidth / 2, y, z);
   else if(mOrientation == GmTypes::OT_VERTICAL)
      GmDrawable::setGLPosition(x, getCurrentValue() - mGLHeight / 2, z);
}

void GmSliderButton::mousePressEvent(GmEvent* event)
{
   GmInteractiveWidget::mousePressEvent(event);

   // store last mouse position
   if(mOrientation == GmTypes::OT_HORIZONTAL)
      mLastMousePos = event->getMouseData()->GLx;
   else if(mOrientation == GmTypes::OT_VERTICAL)
      mLastMousePos = event->getMouseData()->GLy;

   mMouseInRange = true;
}

void GmSliderButton::mouseDragEvent(GmEvent* event)
{
   GmInteractiveWidget::mouseDragEvent(event);

   // get the current position of the slider button
   float bX, bY, bZ;
   mGLPosition.getXYZ(bX, bY, bZ);

   // set new mouse coordinate and oldPosition according to orientation
   float newCoord, oldPosition, halfButton;
   if(mOrientation == GmTypes::OT_HORIZONTAL)
   {
      newCoord = event->getMouseData()->GLx;
      halfButton = mGLWidth / 2;
      oldPosition = bX + halfButton;
   }
   else if(mOrientation == GmTypes::OT_VERTICAL)
   {
      newCoord = event->getMouseData()->GLy;
      halfButton = mGLHeight / 2;
      oldPosition = bY + halfButton;
   }

   // if mouse is outside slider button's range
   if(newCoord < getMinimumValue())
   {
      // if mouse is outside drag range
      if(newCoord < getMinimumValue() - halfButton)
         mMouseInRange = false;

      // make sure that if the mouse has been dragged out of
      // the range and it is now coming back in that the
      // that the button doesn't move until the mouse reaches its center
      // if already at the minimum then return
      if(!mMouseInRange)
      {
         // keep the last mouse position at the minimum
         mLastMousePos = getMinimumValue();

         if(getCurrentValue() == getMinimumValue())
            return;
      }
   }
   else if(newCoord > getMaximumValue())
   {
      // if mouse is outside drag range
      if(newCoord > getMaximumValue() + halfButton)
         mMouseInRange = false;

      // make sure that if the mouse has been dragged out of
      // the range and it is now coming back in that the
      // that the button doesn't move until the mouse reaches its center
      // if already at the maximum then return
      if(!mMouseInRange)
      {
         // keep the last mouse position at the maximum
         mLastMousePos = getMaximumValue();

         if(getCurrentValue() == getMaximumValue())
            return;
      }
   }

   // calculate change in value/position
   float change = newCoord - mLastMousePos;

   // if there is no change, return
   if(change == 0)
      return;
   
   // set the new current value of the button
   setCurrentValue(oldPosition + change);

   // send an action to listeners with new value and change
   GmScalarAction* sa = new GmScalarAction(this, getCurrentValue(), change);
   fireChangeOccurred(sa);
   delete sa;

   // don't update last mouse position if min or max reached when
   // dragging; use members and not member functions in case
   // action firing resulted in destruction of this slider button
   if(mCurrentValue == mMinimumValue || mCurrentValue == mMaximumValue)
      return;
   
   mLastMousePos = newCoord;
}

void GmSliderButton::setColor(float r, float g, float b, float a)
{
   mBaseColor.setRGBA(r, g, b, a);
}

void GmSliderButton::setIcons(GmIcon* pressedIcon, GmIcon* unpressedIcon)
{
   // if both icons are passed in, set them
   if(pressedIcon && unpressedIcon)
   {
      if(!mButtonIcon)
         mButtonIcon = new GmGraphicButton();

      mButtonIcon->setTextJustification(GmTypes::JTH_CENTER,
                                        GmTypes::JTV_MIDDLE);
      
      mButtonIcon->setPressedIcon(pressedIcon);
      mButtonIcon->setUnpressedIcon(unpressedIcon);
      mIconUsed = true;
   }
   else   // if both icons are NULL, reset the button to not use icons
   {
      if(mButtonIcon)
      {
         delete mButtonIcon;
         mButtonIcon = NULL;
      }
      
      mIconUsed = false;
   }
}

void GmSliderButton::setGLPosition(float x, float y, float z)
{
   float dX, dY, dZ;
   mGLPosition.getXYZ(dX, dY, dZ);

   dX = x - dX;
   dY = y - dY;
   dZ = z - dZ;

   // update position
   GmDrawable::setGLPosition(x, y, z);

   // update range and current value
   if(mOrientation == GmTypes::OT_HORIZONTAL)
   {
      setRange(getMinimumValue() + dX, getMaximumValue() + dX);
      setCurrentValue(getCurrentValue() + dX);
   }
   else if(mOrientation == GmTypes::OT_VERTICAL)
   {
      setRange(getMinimumValue() + dY, getMaximumValue() + dY);
      setCurrentValue(getCurrentValue() + dY);
   }
}

void GmSliderButton::layoutInGLVolume(float minX, float minY, float minZ,
                                      float width, float height, float depth)
{
   GmScalarWidget::layoutInGLVolume(minX, minY, minZ,
                                    width, height, depth);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   if(mOrientation == GmTypes::OT_HORIZONTAL)
      setCurrentValue(x);
   else if(mOrientation == GmTypes::OT_VERTICAL)
      setCurrentValue(y);
}

void GmSliderButton::render2d(GmTypes::RenderOptionType renderOptions)
{
   if(!mIconUsed)
   {
      // save color settings
      glPushAttrib(GL_CURRENT_BIT);
      
      if(!getDisplayListValid())
      {
         if (mDisplayListId != GL_ZERO)
            glDeleteLists(getDisplayListId(), 1);
        
         float bSize;
         if(mGLHeight > mGLWidth)
            bSize = mGLWidth * (mBorderSize/2.0); // border width
         else
            bSize = mGLHeight * (mBorderSize/2.0);
         
         float r, g, b, a; // red, green, blue and alpha values
         mBaseColor.getRGBA(r, g, b, a);
         
         setDisplayListId(glGenLists(1));
         glNewList(getDisplayListId(), GL_COMPILE);        
         
         // Left border of button      
         glColor4f(r*.8, g*.8, b*.8, a);
         glBegin(GL_POLYGON);
         glVertex2f(0.0, 0.0);
         glVertex2f(bSize, bSize);
         glVertex2f(bSize, mGLHeight - bSize);
         glVertex2f(0.0, mGLHeight);
         glEnd();
         
         // Top border of button
         glColor4f(r*.85, g*.85, b*.85, a);
         glBegin(GL_POLYGON);
         glVertex2f(0.0, mGLHeight);
         glVertex2f(mGLWidth, mGLHeight);
         glVertex2f(mGLWidth - bSize, mGLHeight - bSize);
         glVertex2f(bSize, mGLHeight - bSize);
         glEnd();
         
         // Bottom border of button
         glColor4f(r*.6, g*.6, b*.6, a);
         glBegin(GL_POLYGON);
         glVertex2f(0.0, 0.0);
         glVertex2f(mGLWidth, 0.0);
         glVertex2f(mGLWidth - bSize, bSize);
         glVertex2f(bSize, bSize);
         glEnd();
         
         // Right border of button
         glColor4f(r*.7, g*.7, b*.7, a);
         glBegin(GL_POLYGON);
         glVertex2f(mGLWidth, 0.0);
         glVertex2f(mGLWidth, mGLHeight);
         glVertex2f(mGLWidth - bSize, mGLHeight - bSize);
         glVertex2f(mGLWidth - bSize, bSize);
         glEnd();
         
         // Center section of button
         glColor4f(r, g, b, a);
         glBegin(GL_POLYGON);
         glVertex2f(bSize, bSize);
         glVertex2f(mGLWidth - bSize, bSize);
         glVertex2f(mGLWidth - bSize, mGLHeight - bSize);
         glVertex2f(bSize, mGLHeight - bSize);
         glEnd();

         glEndList();

         setDisplayListValid(true);
      }
 
      float x, y, z; // x,y,z coordinates
      mGLPosition.getXYZ(x, y, z);
     
      glPushMatrix();
      glTranslatef(x, y, z);
      glCallList(getDisplayListId());
      glPopMatrix();

      // restore color settings
      glPopAttrib();
   }
   else
   {
      if(mButtonIcon != NULL)
         mButtonIcon->render(renderOptions);
   }
}

void GmSliderButton::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);
   
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);
   
   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);
   
   glColor4f(1.0, 1.0, 0, 1.0);
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
}
