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
#include <glam/GmToolTip.h>
#include <glam/GmOptions.h>

GmToolTip::GmToolTip()
      : GmTextWidget() // this needs to be replaced with GmTextArea
{
   GmOptions* gmOptions = GmOptions::getInstance();
   
   GlmColor4* tColor = NULL;
   GlmColor4* bColor = NULL;

   bColor = (GlmColor4*)gmOptions->getColorOption(GMO_TOOLTIP_BASECOLOR);
   tColor = (GlmColor4*)gmOptions->getColorOption(GMO_TOOLTIP_TEXTCOLOR);

   if(tColor != NULL)
      setTextColor(tColor);
   if(bColor != NULL)
      setBaseColor(bColor);

   setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
}

GmToolTip::~GmToolTip()
{
}

void GmToolTip::render2d(GmTypes::RenderOptionType renderOptions)
{
   if(!getDisplayListValid())
   {
      float textHeight, textWidth;
      getGLExtents(textWidth, textHeight);

      textWidth *= 1.05;
      textHeight *= 1.2;

      setGLWidth(textWidth);
      setGLHeight(textHeight);
   }
   
   float r, g, b, a;
   mBaseColor.getRGBA(r, g, b, a);

   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);
   
   glPushMatrix();
   glTranslatef(x, y, z);
   
   glColor4f(r, g, b, a);
   glBegin(GL_QUADS);
   glVertex2f(0.0, 0.0);
   glVertex2f(0.0, mGLHeight);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(mGLWidth, 0.0);
   glEnd();

   // Draw the widget text outline
   glColor4f(0.0, 0.0, 0.0, a);
   glBegin(GL_LINE_LOOP);
   glVertex2f(0.0, 0.0);
   glVertex2f(0.0, mGLHeight);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(mGLWidth, 0.0);
   glEnd();

   glPopMatrix();

   // restore color settings
   glPopAttrib();

   // render the text
   GmTextWidget::render2d(renderOptions);
}
