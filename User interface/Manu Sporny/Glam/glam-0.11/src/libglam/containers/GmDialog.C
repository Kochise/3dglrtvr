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
#include <glam/GmDialog.h>

GmDialog::GmDialog()
      : GmContentPane()
{
   GmOptions* gmOptions = GmOptions::getInstance();
   GlmColor4* baseColor = NULL;
   GlmColor4* tBarColor = NULL;
   GlmColor4* tTextColor = NULL;

   baseColor = (GlmColor4*)gmOptions->getColorOption(GMO_DIALOG_BASECOLOR);
   tBarColor = (GlmColor4*)gmOptions->getColorOption(GMO_DIALOG_TITLEBARCOLOR);
   tTextColor = \
      (GlmColor4*)gmOptions->getColorOption(GMO_DIALOG_TITLETEXTCOLOR);

   mTitleTextWidget = new GmLabel();
   mTitleTextWidget->setTextJustification(GmTypes::JTH_CENTER,
                                          GmTypes::JTV_MIDDLE);

   if(baseColor != NULL)
      mBaseColor = *baseColor;
   if(tBarColor != NULL)
      mTitlebarColor = *tBarColor;
   else
      mTitlebarColor.setRGBA(0.8, 0.8, 0.8, 1.0);
   if(tTextColor != NULL)
   {
      float r, g, b, a;

      tTextColor->getRGBA(r, g, b, a);
      mTitleTextWidget->setTextColor(r, g, b, a);
   }
}

GmDialog::~GmDialog()
{
   if(mTitleTextWidget)
      delete mTitleTextWidget;
}

void GmDialog::setTitlebarColor(float r, float g, float b, float a)
{
   mTitlebarColor.setRGBA(r, g, b, a);
   setDisplayListValid(false);
}

GlmColor4* GmDialog::getTitlebarColor()
{
   return &mTitlebarColor;
}

void GmDialog::setTitleText(string text)
{
   if(mTitleTextWidget)
      mTitleTextWidget->setText(text);
   
   setDisplayListValid(false);
}

string GmDialog::getTitleText()
{
   if(mTitleTextWidget)
      return mTitleTextWidget->getText();
   else
      return NULL;
}

void GmDialog::setTitleTextColor(GlmColor4* color)
{
   if(mTitleTextWidget)
      mTitleTextWidget->setTextColor(color);
}

void GmDialog::setTitleTextColor(float r, float g, float b, float a)
{
   if(mTitleTextWidget)
      mTitleTextWidget->setTextColor(r, g, b, a);
}

GlmColor4* GmDialog::getTitleTextColor()
{
   if(mTitleTextWidget)
      return mTitleTextWidget->getTextColor();
   else
      return NULL;
}

void GmDialog::setTitleTextSize(float size)
{
   if(mTitleTextWidget)
      mTitleTextWidget->setTextSize(size);
   
   setDisplayListValid(false);
}

float GmDialog::getTitleTextSize()
{
   if(mTitleTextWidget)
      return mTitleTextWidget->getTextSize();
   else
      return 0.0;
}

void GmDialog::setTitleTextJustification(GmTypes::JustificationType horizontal,
                                         GmTypes::JustificationType vertical)
{
   if(mTitleTextWidget)
      mTitleTextWidget->setTextJustification(horizontal, vertical);
   setDisplayListValid(false);
}

void GmDialog::getTitleTextJustification(
   GmTypes::JustificationType& horizontal,
   GmTypes::JustificationType& vertical)
{
   if(mTitleTextWidget)
      mTitleTextWidget->getTextJustification(horizontal,vertical);
}

void GmDialog::setGLPosition(float x, float y, float z)
{
   float dX, dY, dZ;
   mGLPosition.getXYZ(dX, dY, dZ);

   dX = x - dX;
   dY = y - dY;
   dZ = z - dZ;

   GmContainer::setGLPosition(x, y, z);

   // position components
   float cx, cy, cz;
   if(mTitleTextWidget)
   {
      mTitleTextWidget->getGLPosition()->getXYZ(cx, cy, cz);
      mTitleTextWidget->setGLPosition(cx + dX, cy + dY, cz + dZ);
   }
}
