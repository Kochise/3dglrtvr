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
#include <glam/GmContentPane.h>
#include <glam/Gm2dLayout.h>

GmContentPane::GmContentPane()
      : GmContainer()
{
   // defaults to a 2d layout manager
   Gm2dLayout* lo = new Gm2dLayout();
   setLayout(lo);
}

GmContentPane::~GmContentPane()
{
}

void GmContentPane::render2d(GmTypes::RenderOptionType renderOptions)
{
   GmContainer::render2d(renderOptions);
}

void GmContentPane::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);

   // render a background pane to prevent "pick-throughs"
   glColor4f(0.4, 0.4, 0.8, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   glPopMatrix();

   // restore color settings
   glPopAttrib();

   GmContainer::render2dSelect(renderOptions);
}
