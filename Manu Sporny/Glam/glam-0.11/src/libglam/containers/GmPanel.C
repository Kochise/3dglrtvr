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
#include <glam/GmPanel.h>

GmPanel::GmPanel()
      : GmContentPane()
{
   GmOptions* gmOptions = GmOptions::getInstance();
   GlmColor4* color = NULL;

   color = (GlmColor4*)gmOptions->getColorOption(GMO_PANEL_BASECOLOR);
   if(color != NULL)
      mBaseColor = *color;
}

GmPanel::~GmPanel()
{
}

void GmPanel::render2d(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   float r, g, b, a;
   mBaseColor.getRGBA(r, g, b, a);

   glPushMatrix();
   glTranslatef(x, y, z);
   
   // draw panel
   glColor4f(r, g, b, a);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   glPopMatrix();

   // restore color settings
   glPopAttrib();

   // render components
   GmContainer::render2d(renderOptions);
}

void GmPanel::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);

   // draw panel
   glColor4f(0.4, 0.4, 0.4, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   glPopMatrix();

   // restore color settings
   glPopAttrib();
   
   // render components
   GmContainer::render2dSelect(renderOptions);
}
