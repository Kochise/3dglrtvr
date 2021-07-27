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
#include <glam/GmButton.h>

GmButton::GmButton()
      : GmInteractiveWidget()
{
   setState(GmTypes::BS_UP);
   mBorderWidth = 0.0;
   mLabel = NULL;

   mPressed = false;
}

GmButton::~GmButton()
{
   if(mLabel != NULL)
      delete mLabel;
}

void GmButton::fireButtonPressed(GmButtonAction* action)
{
   vector<GmButtonListener*>::iterator i;
   for(i = mButtonListeners.begin(); i != mButtonListeners.end(); i++)
      (*i)->buttonPressed(action);
}

void GmButton::addButtonListener(GmButtonListener* bl)
{
   mButtonListeners.push_back(bl);
}

bool GmButton::removeButtonListener(GmButtonListener* bl)
{
   vector<GmButtonListener*>::iterator i;
   for(i = mButtonListeners.begin(); i != mButtonListeners.end(); i++)
   {
      // compare the listeners
      if(*(*i) == *bl)
      {
         mButtonListeners.erase(i);
         return true;
      }
   }

   return false;
}

void GmButton::setState(GmTypes::ButtonStateType state)
{
   mState = state;
   setDisplayListValid(false);
}

GmTypes::ButtonStateType GmButton::getState()
{
   return mState;
}

void GmButton::setBorderWidth(float borderWidth)
{
   mBorderWidth = borderWidth;
   setDisplayListValid(false);
}

float GmButton::getBorderWidth()
{
   return mBorderWidth;
}

// Text widget wrappers...

void GmButton::setText(string text)
{
   if(mLabel != NULL)
      mLabel->setText(text);
   else
   {
      // create a new label and set its text
      mLabel = new GmLabel();
      mLabel->setText(text);

      // relayout the button
      relayout();
   }

   setDisplayListValid(false);
}

string GmButton::getText()
{
   if(mLabel != NULL)
      return mLabel->getText();
   else
      return NULL;
}

void GmButton::setTextColor(GlmColor4* color)
{
   if(mLabel != NULL)
      mLabel->setTextColor(color);
   
   setDisplayListValid(false);
}

void GmButton::setTextColor(float r, float g, float b, float a)
{
   if(mLabel != NULL)
      mLabel->setTextColor(r, g, b, a);
   
   setDisplayListValid(false);
}

GlmColor4* GmButton::getTextColor()
{
   if(mLabel != NULL)
      return mLabel->getTextColor();
   else
      return NULL;
}

void GmButton::setTextSize(float size)
{
   if(mLabel != NULL)
      mLabel->setTextSize(size);
   
   setDisplayListValid(false);
}

float GmButton::getTextSize()
{
   if(mLabel != NULL)
      return mLabel->getTextSize();
   else
      return 0.0;
}

void GmButton::setTextJustification(GmTypes::JustificationType horizontal,
                                        GmTypes::JustificationType vertical)
{
   if(mLabel != NULL)
      mLabel->setTextJustification(horizontal, vertical);
   
   setDisplayListValid(false);
}

void GmButton::getTextJustification(
   GmTypes::JustificationType& horizontal,
   GmTypes::JustificationType& vertical)
{
   if(mLabel != NULL)
      mLabel->getTextJustification(horizontal, vertical);
}

void GmButton::setGLPosition(float x, float y, float z)
{
   float dX, dY, dZ;
   mGLPosition.getXYZ(dX, dY, dZ);

   dX = x - dX;
   dY = y - dY;
   dZ = z - dZ;

   GmDrawable::setGLPosition(x, y, z);

   // position label
   if(mLabel != NULL)
   {
      float lx, ly, lz;
      mLabel->getGLPosition(lx, ly, lz);
      mLabel->setGLPosition(lx + dX, ly + dY, lz + dZ);
   }
}

void GmButton::setVisible(bool visible)
{
   GmDrawable::setVisible(visible);
   
   if(mLabel != NULL)
      mLabel->setVisible(visible);
}

void GmButton::setTextVisible(bool visible)
{
   if(mLabel != NULL)
      mLabel->setVisible(visible);
}

void GmButton::layoutInGLVolume(float minX, float minY, float minZ,
                                float width, float height, float depth)
{
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);

   // position the text
   if(mLabel != NULL)
   {
      // get position of the button
      float x, y, z;
      mGLPosition.getXYZ(x, y, z);

      // reset the text validation thinger
      mLabel->setText(mLabel->getText());

      float tWidth, tHeight;
      mLabel->getGLExtents(tWidth, tHeight);

      // get the maximum width for the label
      float width = mGLWidth - mBorderWidth * mGLWidth;

      // if the text width is less than the max width, use it
      if(tWidth < width)
         width = tWidth;

      // layout the label
      GmTypes::JustificationType horz, vert;
      mLabel->getTextJustification(horz, vert);

      mLabel->layoutInGLVolume(x + (mGLWidth - width) / 2.0,
                               y + (mGLHeight - tHeight) / 2.0, z,
                               width, tHeight, mGLDepth);
      }
}

void GmButton::mousePressEvent(GmEvent* event)
{
   GmInteractiveWidget::mousePressEvent(event);
   
   // set the button's state and notify listeners
   setState(GmTypes::BS_DOWN);

   // remember that the mouse has been pressed
   // in case mouse exits and reenters
   mPressed = true;
}

void GmButton::mouseReleaseEvent(GmEvent* event)
{
   GmInteractiveWidget::mouseReleaseEvent(event);
   
   // release button press
   mPressed = false;

   // if the button is down, fire a button pressed event
   if(mState == GmTypes::BS_DOWN)
   {
      GmButtonAction* ba = new GmButtonAction(this, mState);

      // set the button's state
      setState(GmTypes::BS_UP);
      
      fireButtonPressed(ba);
      delete ba;
   }
}

void GmButton::mouseEnteredEvent(GmEvent* event)
{
   GmInteractiveWidget::mouseEnteredEvent(event);
   
   // if the button has been pressed and no mouse release received
   if(mPressed)
      setState(GmTypes::BS_DOWN);
}

void GmButton::mouseExitedEvent(GmEvent* event)
{
   GmInteractiveWidget::mouseExitedEvent(event);
   
   // if the button has been pressed and no mouse release received
   if(mPressed)
      setState(GmTypes::BS_UP);
}

void GmButton::getGLExtents(float& width, float& height)
{
   // this gets the minimum button size that the text will fit into
   // with the given border size
   if(mLabel != NULL)
   {
      mLabel->getGLExtents(width, height);
      width += mBorderWidth * width;
      height += mBorderWidth * width;
   }
   else
   {
      width = 1.0;
      height = 1.0;
   }
}
