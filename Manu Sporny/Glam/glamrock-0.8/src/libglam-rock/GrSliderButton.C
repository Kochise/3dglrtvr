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
#include <GL/gl.h>
#include <glamrock/GrShapeFactory.h>
#include <glamrock/GrSliderButton.h>

#define PR_PERCENT 0.5     // Panel rounding percentage

GrSliderButton::GrSliderButton()
      : GmSliderButton()
{
}

GrSliderButton::~GrSliderButton()
{
}

void GrSliderButton::render2d(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   GrShapeFactory* sf = GrShapeFactory::getInstance();

   float r, g, b, a;
   mBaseColor.getRGBA(r, g, b, a);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);
   
   glPushMatrix();
   glTranslatef(x, y, z);

   glColor4f(r, g, b, a);
   sf->renderRoundedRect(GrTypes::BS_ALL,
                         GrTypes::FS_SOLID,
                         mGLWidth, mGLHeight, PR_PERCENT);
   glPopMatrix();

   // restore color settings
   glPopAttrib();
}

void GrSliderButton::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   GrShapeFactory* sf = GrShapeFactory::getInstance();

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);

   glColor4f(0.8, 0.0, 0.8, 1.0);
   sf->renderRoundedRect(GrTypes::BS_ALL,
                         GrTypes::FS_SOLID,
                         mGLWidth, mGLHeight, PR_PERCENT);

   glPopName();
   
   glPopMatrix();

   // restore color settings
   glPopAttrib();
}
