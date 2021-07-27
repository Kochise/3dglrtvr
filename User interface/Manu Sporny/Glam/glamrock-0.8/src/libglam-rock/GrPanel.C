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
#include <glamrock/GrPanel.h>

#define PR_PERCENT 0.2     // Panel rounding percentage

GrPanel::GrPanel()
{
}

GrPanel::~GrPanel()
{
}

void GrPanel::layoutInGLArea(float minX, float minY,
                             float width, float height,
                             float layer)
{
   layoutInGLVolume(minX, minY, layer, width, height, 0.0);
}

void GrPanel::layoutInGLVolume(float minX, float minY, float minZ,
                               float width, float height, float depth)
{
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);

   float bWidth = PR_PERCENT / 8.0;

   // apply the layout with border
   applyLayout(bWidth, bWidth, bWidth, bWidth);
}

void GrPanel::render2d(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);
   
   if(!getDisplayListValid())
   {
      GrShapeFactory* sf = GrShapeFactory::getInstance();

      if(getDisplayListId() != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);
      
      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);

      float r, g, b, a;
      mBaseColor.getRGBA(r, g, b, a);

      // Draw the base part of the panel
      glColor4f(r, g, b, a);
      sf->renderRoundedRect(GrTypes::BS_ALL,
                            GrTypes::FS_SOLID,
                            mGLWidth, mGLHeight, PR_PERCENT);

      // Draw the border for the panel
      glColor4f(r*0.5, g*0.5, b*0.5, a);
      sf->renderRoundedRect(GrTypes::BS_ALL,
                            GrTypes::FS_OUTLINE,
                            mGLWidth, mGLHeight, PR_PERCENT);

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
   
   // render the components in the panel
   GmContainer::render2d(renderOptions);
}

void GrPanel::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);
   
   GrShapeFactory* sf = GrShapeFactory::getInstance();

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);

   glColor4f(0.4, 0.4, 0.4, 1.0);
   sf->renderRoundedRect(GrTypes::BS_ALL,
                         GrTypes::FS_SOLID,
                         mGLWidth, mGLHeight, PR_PERCENT);

   glPopName();

   glPopMatrix();

   // restore color settings
   glPopAttrib();

   // render select for components in the panel
   GmContainer::render2dSelect(renderOptions);
}
