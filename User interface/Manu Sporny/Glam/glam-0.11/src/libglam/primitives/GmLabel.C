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
#include <glam/GmLabel.h>

GmLabel::GmLabel()
   : GmInteractiveWidget()
{
   mTextWidget = new GmTextWidget();
   mTextWidget->setMeasureMode(GmTypes::MMT_APPROXIMATE);
   mTextWidget->setTextJustification(GmTypes::JTH_CENTER,
                                     GmTypes::JTV_MIDDLE);
}

GmLabel::~GmLabel()
{
   delete mTextWidget;
}

void GmLabel::setText(string text)
{
   // set the value text
   mText = text;

   // set the tooltip text
   setToolTipText(mText);

   // set the text
   mTextWidget->setText(mText);
}

string GmLabel::getText()
{
   return mText;
}

void GmLabel::validateText()
{
   // set to no text if widget width is zero or no text
   if(mGLWidth == 0 || mText == "")
   {
      mTextWidget->setText("");
      return;
   }
   
   float textWidth, textHeight;
   mTextWidget->getGLExtents(textWidth, textHeight);

   // if the text is wider than the width, abbreviate the text
   if(textWidth > mGLWidth)
   {
      // get the approximate letter width
      float letterWidth = textWidth / mText.length();

      // determine how many letters can fit inside the width
      int numLetters = (int)(mGLWidth / letterWidth);

      // if at least 3 letters fit within the width
      if(numLetters >= 3)
         mTextWidget->setText(mText.substr(0, numLetters - 3) + "...");
      else
         mTextWidget->setText(".");
   }
   else
      mTextWidget->setText(mText);
}

void GmLabel::unAbbreviateText()
{
   mTextWidget->setText(mText);
}

bool GmLabel::isAbbreviated()
{
   return mTextWidget->getText() != mText;
}

void GmLabel::setFontType(GmTypes::FontType fontType)
{
   mTextWidget->setFontType(fontType);
}

GmTypes::FontType GmLabel::getFontType()
{
   return mTextWidget->getFontType();
}

void GmLabel::setTextColor(GlmColor4* color)
{
   mTextWidget->setTextColor(color);
}

void GmLabel::setTextColor(float r, float g, float b, float a)
{
   mTextWidget->setTextColor(r, g, b, a);
}

GlmColor4* GmLabel::getTextColor()
{
   return mTextWidget->getTextColor();
}

void GmLabel::setTextSize(float size)
{
   mTextWidget->setTextSize(size);
}

float GmLabel::getTextSize()
{
   return mTextWidget->getTextSize();
}

void GmLabel::setTextJustification(GmTypes::JustificationType horizontal,
                                   GmTypes::JustificationType vertical)
{
   mTextWidget->setTextJustification(horizontal, vertical);
}

void GmLabel::getTextJustification(GmTypes::JustificationType& horizontal,
                                   GmTypes::JustificationType& vertical)
{
   mTextWidget->getTextJustification(horizontal, vertical);
}

void GmLabel::setOffset(float xOffset, float yOffset)
{
   mTextWidget->setOffset(xOffset, yOffset);
}

void GmLabel::getOffset(float& xOffset, float& yOffset)
{
   mTextWidget->getOffset(xOffset, yOffset);
}

void GmLabel::getGLExtents(float& width, float& height)
{
   mTextWidget->getGLExtents(width, height);
}

void GmLabel::setGLPosition(float x, float y, float z)
{
   mGLPosition.setXYZ(x, y, z);
   mTextWidget->setGLPosition(x, y, z);
}

void GmLabel::setGLPosition(GlmPoint3* point)
{
   float x, y, z;
   point->getXYZ(x, y, z);
   setGLPosition(x, y, z);
}

void GmLabel::layoutInGLVolume(float minX, float minY, float minZ,
                               float width, float height, float depth)
{
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);
   mTextWidget->layoutInGLVolume(x, y, z, mGLWidth, mGLHeight, mGLDepth);

   // validate the text
   validateText();
}

void GmLabel::render2d(GmTypes::RenderOptionType renderOptions)
{
   render2dToolTip(renderOptions);
   mTextWidget->render(renderOptions);
}

void GmLabel::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);
   
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);
   
   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);
   
   glColor4f(1.0, 0.0, 0.0, 1.0);
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
