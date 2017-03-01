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
#include <glam/GmGraphicButton.h>
#include <glam/GmPixmapIcon.h>
#include <glam/GmSvgIcon.h>

GmGraphicButton::GmGraphicButton()
      : GmButton()
{
   mPressedIcon = NULL;
   mUnpressedIcon = NULL;
   mFade = false;
}

GmGraphicButton::~GmGraphicButton()
{
   if(mPressedIcon != NULL)
      delete mPressedIcon;

   if(mUnpressedIcon != NULL)
      delete mUnpressedIcon;
}

void GmGraphicButton::setPressedIcon(GmIcon* icon)
{
   mPressedIcon = icon;
   setIconGeometry();
}

void GmGraphicButton::setUnpressedIcon(GmIcon* icon)
{
   mUnpressedIcon = icon;

   if(mFade && mUnpressedIcon)
   {
      float r, g, b, a;
      mUnpressedIcon->getBaseColor(r, g, b, a);
      mUnpressedIcon->setBaseColor(r, g, b, a * 0.5);
   }
   
   setIconGeometry();
}

void GmGraphicButton::setText(string text)
{
   GmButton::setText(text);
   setIconGeometry();
}

void GmGraphicButton::setTextJustification(
   GmTypes::JustificationType horizontal,
   GmTypes::JustificationType vertical)
{
   GmButton::setTextJustification(horizontal, vertical);
   setIconGeometry();
}

bool GmGraphicButton::getFade()
{
   return mFade;
}

void GmGraphicButton::setFade(bool fade)
{
   if(mFade == fade)
      return;
   
   if(mUnpressedIcon == NULL)
   {
      mFade = fade;
      return;
   }
   
   float r, g, b, a;
   mUnpressedIcon->getBaseColor(r, g, b, a);

   // if fade was previously set, unfade the icon
   if(mFade)
      mUnpressedIcon->setBaseColor(r, g, b, a / 0.5);
   else // fade was previously not set, fade the icon
      mUnpressedIcon->setBaseColor(r, g, b, a * 0.5);

   mFade = fade;
}

void GmGraphicButton::setGLPosition(float x, float y, float z)
{
   float dX, dY, dZ;
   mGLPosition.getXYZ(dX, dY, dZ);

   dX = x - dX;
   dY = y - dY;
   dZ = z - dZ;

   GmButton::setGLPosition(x, y, z);

   // position components
   float cx, cy, cz;
   if(mPressedIcon != NULL)
   {
      mPressedIcon->getGLPosition(cx, cy, cz);
      mPressedIcon->setGLPosition(cx + dX, cy + dY, cz + dZ);
   }
   
   if(mUnpressedIcon != NULL)
   {
      mUnpressedIcon->getGLPosition(cx, cy, cz);
      mUnpressedIcon->setGLPosition(cx + dX, cy + dY, cz + dZ);
   }
}

void GmGraphicButton::setGLPosition(GlmPoint3* point)
{
   float x, y, z;
   point->getXYZ(x, y, z);

   setGLPosition(x, y, z);
}

void GmGraphicButton::layoutInGLVolume(float minX, float minY, float minZ,
                                       float width, float height,
                                       float depth)
{
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);

   // position the text
   if(mLabel != NULL)
   {
      // get position of the button
      float x, y, z;
      mGLPosition.getXYZ(x, y, z);

      // unabbreviate the text to get the maximum text size
      mLabel->unAbbreviateText();

      float tWidth, tHeight;
      mLabel->getGLExtents(tWidth, tHeight);

      // get the maximum width for the label
      float width = mGLWidth - mBorderWidth * mGLWidth;

      // if the text width is less than the max width, use it
      if(tWidth < width)
         width = tWidth;

      if(mPressedIcon != NULL || mUnpressedIcon != NULL)
         y += mBorderWidth / 2.0 * mGLHeight;
      else
         y += (mGLHeight - tHeight) / 2.0;

      // layout the label
      mLabel->layoutInGLVolume(x + (mGLWidth - width) / 2.0, y, z,
                               width, tHeight, mGLDepth);
   }

   setIconGeometry();
}

void GmGraphicButton::mouseEnteredEvent(GmEvent* event)
{
   GmButton::mouseEnteredEvent(event);

   if(mFade && mUnpressedIcon)
   {
      float r, g, b, a;
      mUnpressedIcon->getBaseColor(r, g, b, a);
      mUnpressedIcon->setBaseColor(r, g, b, a / 0.5);
   }
}

void GmGraphicButton::mouseExitedEvent(GmEvent* event)
{
   GmButton::mouseExitedEvent(event);

   if(mFade && mUnpressedIcon)
   {
      float r, g, b, a;
      mUnpressedIcon->getBaseColor(r, g, b, a);
      mUnpressedIcon->setBaseColor(r, g, b, a * 0.5);
   }
}

void GmGraphicButton::render2d(GmTypes::RenderOptionType renderOptions)
{
   render2dToolTip(renderOptions);

   if(mState == GmTypes::BS_DOWN)
   {
      if(mPressedIcon != NULL)
         mPressedIcon->render(renderOptions);
   }
   else if(mUnpressedIcon != NULL)
      mUnpressedIcon->render(renderOptions);

   if(mLabel != NULL)
      mLabel->render(renderOptions);
}

void GmGraphicButton::render2dSelect(GmTypes::RenderOptionType renderOptions)
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

   if(mLabel != NULL)
      mLabel->render(renderOptions);
}

void GmGraphicButton::setIconGeometry()
{
   float x = mBorderWidth / 2.0;
   float y = mBorderWidth / 2.0;

   float w = 1.0 - mBorderWidth;
   float h = 1.0 - mBorderWidth;

   if(mLabel != NULL)
   {
      // get the height of the label
      float height = mLabel->getGLHeight();

      // get height percentage
      if(mGLHeight != 0.0)
         height /= mGLHeight;

      // make height adjustments (width changed to retain aspect)
      h -= height;
      w -= height;
      y += height;
      x += height / 2.0;
   }

   if(mPressedIcon != NULL)
      mPressedIcon->setRelativeGeometry(x, y, x + w, y + h);
   
   if(mUnpressedIcon != NULL)
      mUnpressedIcon->setRelativeGeometry(x, y, x + w, y + h);

   // layout the icons
   float z;
   mGLPosition.getXYZ(x, y, z);
   
   if(mPressedIcon != NULL)
   {
      mPressedIcon->layoutInGLVolume(x, y, z,
                                     mGLWidth, mGLHeight, mGLDepth);
   }

   if(mUnpressedIcon != NULL)
   {
      mUnpressedIcon->layoutInGLVolume(x, y, z,
                                       mGLWidth, mGLHeight, mGLDepth);
   }
}

void GmGraphicButton::getGLExtents(float& width, float& height)
{
   float tWidth = 0, tHeight = 0, uWidth = 0,
      uHeight = 0, pWidth = 0, pHeight = 0;
   
   if(mLabel != NULL)
   {
      mLabel->getGLExtents(tWidth, tHeight);
      tWidth = tWidth / (1 - mBorderWidth);
      tHeight += mBorderWidth * tWidth;
   }
   if(mUnpressedIcon != NULL)
   {
      uWidth = mUnpressedIcon->getGLWidth();
      uHeight = mUnpressedIcon->getGLHeight();
   }
   if(mPressedIcon != NULL)
   {
      pWidth = mPressedIcon->getGLWidth();
      pHeight = mPressedIcon->getGLHeight();
   }

   if(tHeight >= uHeight && tHeight >= pHeight)
      height = tHeight;
   else if(uHeight >= tHeight && uHeight >= pHeight)
      height = uHeight;
   else if(pHeight >= tHeight && pHeight >= uHeight)
      height = pHeight;
   else
      height = 1.0;

   if(tWidth >= uWidth && tWidth >= pWidth)
      width = tWidth;
   else if(uWidth >= tWidth && uWidth >= pWidth)
      width = uWidth;
   else if(pWidth >= tWidth && pWidth >= uWidth)
      width = pWidth;
   else
      width = 1.0;
}
