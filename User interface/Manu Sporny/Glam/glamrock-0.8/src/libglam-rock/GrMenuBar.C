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
#include <glamrock/GrShapeFactory.h>
#include <glamrock/GrMenuBar.h>

#define BMR_PERCENT 0.5     // Panel rounding percentage

GrMenuBar::GrMenuBar()
{
}

GrMenuBar::~GrMenuBar()
{
}

void GrMenuBar::render2d(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   if(!getDisplayListValid())
   {
      if(getDisplayListId() != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);

      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);

      GrShapeFactory* sf = GrShapeFactory::getInstance();
      
      float r, g, b, a;
      mBaseColor.getRGBA(r, g, b, a);
      
      glColor4f(r, g, b, a);
      sf->renderRoundedRect(GrTypes::BS_ALL,
                            GrTypes::FS_SOLID,
                            mGLWidth, mGLHeight, BMR_PERCENT);
      glColor4f(r*0.5, g*0.5, b*0.5, a);
      sf->renderRoundedRect(GrTypes::BS_ALL,
                            GrTypes::FS_OUTLINE,
                            mGLWidth, mGLHeight, BMR_PERCENT);
   
      glEndList();
      
      setDisplayListValid(true);
   }

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);
   glCallList(getDisplayListId());
   glPopMatrix();

   // restore color settings
   glPopAttrib();

   // render the menus
   renderMenus(renderOptions);
}

void GrMenuBar::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // render the menus
   GmMenuBar::render2dSelect(renderOptions);
}
