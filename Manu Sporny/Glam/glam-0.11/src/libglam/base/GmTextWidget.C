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
#include <glam/GmTextWidget.h>
#include <glam/GmFontManager.h>

GmTextWidget::GmTextWidget() 
      : GmDrawable()
{
   mText = "";
   mFontFace = NULL;
   mTextDisplayListId = GL_ZERO;

   GmOptions* gmOptions = GmOptions::getInstance();

   string fontType = gmOptions->getStringOption(GMO_TEXT_FONTTYPE);
   if(fontType == "Texture")
      mFontType = GmTypes::FT_T_TRANSLUCENT;
   else if(fontType == "Solid")
      mFontType = GmTypes::FT_P_SOLID;
   else
      mFontType = GmTypes::FT_T_TRANSLUCENT;

   mTextColor = *(gmOptions->getColorOption(GMO_TEXT_COLOR));
   mTextSize = gmOptions->getScalarOption(GMO_TEXT_SIZE);
   mTextResolution = gmOptions->getScalarOption(GMO_TEXT_RESOLUTION);

   mFontHeight = 0.0;
   mTextWidth = 0.0;
   mTextWidthValid = false;
   mMeasureMode = GmTypes::MMT_EXACT;
   
   mTextHJustification = GmTypes::JTH_LEFT;
   mTextVJustification = GmTypes::JTV_MIDDLE;
   setOffset(0.0, 0.0);
}

GmTextWidget::~GmTextWidget()
{
}

bool GmTextWidget::loadFontFace()
{
   // load the font face
   GmFontManager& fm = GmFontManager::getInstance();
   GmOptions* gmOptions = GmOptions::getInstance();

   string fontFile = gmOptions->getStringOption(GMO_TEXT_FONT);

   mFontFace = fm.getFontFace(fontFile, mTextSize, mTextColor, mFontType);
   
   if(mFontFace == NULL)
   {
      cout << "GLAM Error: Failed to load font!" << fontFile << endl;
      return false;
   }

   mFontHeight = fm.getFontHeight(mFontFace, mTextResolution, mTextSize,
                                  mFontType);
   
   mDescender = fm.getDescender(mFontFace, mTextResolution, mTextSize,
                                mFontType);
   
   mTextWidthValid = false;
   setDisplayListValid(false);
   
   return true;
}

void GmTextWidget::setText(string text)
{
   if(mText == text)
      return;
   
   mText = text;

   mTextWidthValid = false;
   setDisplayListValid(false);
}

string GmTextWidget::getText()
{
   return mText;
}

void GmTextWidget::appendText(string text)
{
   mText += text;

   mTextWidthValid = false;
   setDisplayListValid(false);
}

void GmTextWidget::prefixText(string text)
{
   mText = text + mText;

   mTextWidthValid = false;
   setDisplayListValid(false);
}

void GmTextWidget::setFontType(GmTypes::FontType fontType)
{
   mFontType = fontType;
   mFontFace = NULL;

   mTextWidthValid = false;
   setDisplayListValid(false);
}

GmTypes::FontType GmTextWidget::getFontType()
{
   return mFontType;
}

void GmTextWidget::setTextColor(GlmColor4* color)
{
   float r, g, b, a;
   color->getRGBA(r, g, b, a);
   setTextColor(r, g, b, a);
}

void GmTextWidget::setTextColor(float r, float g, float b, float a)
{
   mTextColor.setRGBA(r, g, b, a);
   mFontFace = NULL;

   setDisplayListValid(false);
}

GlmColor4* GmTextWidget::getTextColor()
{
   return &mTextColor;
}

void GmTextWidget::setTextSize(float size)
{  
   if(size > 0.0)
   {
      mTextSize = size;
      mFontFace = NULL;

      mTextWidthValid = false;
      setDisplayListValid(false);
   }
}

float GmTextWidget::getTextSize()
{
   return mTextSize;
}

void GmTextWidget::setTextJustification(GmTypes::JustificationType horizontal,
                                        GmTypes::JustificationType vertical)
{
   mTextHJustification = horizontal;
   mTextVJustification = vertical;

   setDisplayListValid(false);
}

void GmTextWidget::getTextJustification(GmTypes::JustificationType& horizontal,
                                        GmTypes::JustificationType& vertical)
{
   horizontal = mTextHJustification;
   vertical = mTextVJustification;
}

void GmTextWidget::setOffset(float xOffset, float yOffset)
{
   mXOffset = xOffset;
   mYOffset = yOffset;
}

void GmTextWidget::getOffset(float& xOffset, float& yOffset)
{
   xOffset = mXOffset;
   yOffset = mYOffset;
}

void GmTextWidget::setMeasureMode(GmTypes::MeasureModeType mode)
{
   mMeasureMode = mode;

   mTextWidthValid = false;
   setDisplayListValid(false);
}

GmTypes::MeasureModeType GmTextWidget::getMeasureMode()
{
   return mMeasureMode;
}

void GmTextWidget::getGLExtents(float& width, float& height)
{
   width = 0.0;
   height = mTextSize;

   if(mText == "")
      return;

   // load the font face if necessary
   if(mFontFace == NULL)
   {
      // if it couldn't be loaded, return
      if(!loadFontFace())
         return;
   }

   // if the stored text width is valid, use it to save time
   if(mTextWidthValid)
   {
      width = mTextWidth;
      return;
   }

   if(getMeasureMode() == GmTypes::MMT_EXACT)
   {
      // allows for spaces to be measured
      string text = mText;
      int space = text.find_first_of(' ');
      while(space != -1)
      {
         text.replace(space, 1, "0");
         space = text.find_first_of(' ');
      }

      // get font manager for measuring text
      GmFontManager& fm = GmFontManager::getInstance();

      // get text width
      width = fm.getTextWidth(text, mFontFace, mTextResolution, mTextSize,
                              mFontType);
   }
   else if(getMeasureMode() == GmTypes::MMT_APPROXIMATE)
   {
      // approximate width of 1 character to be ratio of its height
      width = mText.length() * 0.58 * mTextSize;
   }
   
   // store text width for speed
   mTextWidth = width;
   mTextWidthValid = true;
}

void GmTextWidget::justifyText()
{
   // determine justification
   float textHeight, textWidth;
   getGLExtents(textWidth, textHeight);

   float xo, yo;
   switch(mTextHJustification)
   {
      case GmTypes::JTH_LEFT:
         xo = 0.0;
         break;
      case GmTypes::JTH_ORIGIN:
         xo = 0.0;
         break;
      case GmTypes::JTH_CENTER:
         if(mGLWidth > textWidth)
            xo = (mGLWidth - textWidth) / 2.0;
         else
            xo = 0.0;
         break;
      case GmTypes::JTH_RIGHT:
         if(mGLWidth > textWidth)
            xo = mGLWidth - textWidth;
         else
            xo = 0.0;
         break;
      case GmTypes::JTH_FULL:
         xo = 0.0;
         break;
      default:
         xo = 0.0;
         break;
   }
   switch(mTextVJustification)
   {
      case GmTypes::JTV_BOTTOM:
         yo = 0.0;
         break;
      case GmTypes::JTV_BASELINE:
         yo = 0.0;
         break;
      case GmTypes::JTV_MIDDLE:
         if(mTextSize < mGLHeight)
            yo = (mGLHeight - mTextSize) / 2.0;
         else
            yo = 0.0;
         break;
      case GmTypes::JTV_TOP:
         yo = mGLHeight - mTextSize;
         break;
      default:
         yo = 0.0;
         break;
   }

   setOffset(xo, yo);
}

void GmTextWidget::render2d(GmTypes::RenderOptionType renderOptions)
{
   if(mText == "")
      return;

   // save color and texture settings
   glPushAttrib(GL_CURRENT_BIT | GL_TEXTURE_BIT);

   // if the old display list is invalid
   if(!getDisplayListValid())
   {
      // load the font face
      if(mFontFace == NULL)
      {
         if(!loadFontFace())
            return;
      }

      // delete the old lists
      if(mTextDisplayListId != GL_ZERO)
         glDeleteLists(mTextDisplayListId, 1);

      if(getDisplayListId() != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);

      // compile the text list
      mTextDisplayListId = mFontFace->compile(mText.c_str());

      // create the display list that includes the scaling
      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);

      // scales the text to the proper size, 1:1 font height to GL unit height
      glPushMatrix();
      float ratio = mTextSize / mFontHeight;
      glScalef(ratio, ratio, 1.0);

      // translate by the baseline descender
      glTranslatef(0.0, mDescender, 0.0);

      // call the text display list
      glCallList(mTextDisplayListId);

      // pop off translation matrices
      glPopMatrix();

      // end the list
      glEndList();

      // justify the text
      justifyText();

      setDisplayListValid(true);
   }

   // position text
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   float xo, yo;
   getOffset(xo, yo);

   glPushMatrix();
   glTranslatef(x + xo, y + yo, z);
   glCallList(getDisplayListId());
   glPopMatrix();

   // restore color and texture settings
   glPopAttrib();
}
