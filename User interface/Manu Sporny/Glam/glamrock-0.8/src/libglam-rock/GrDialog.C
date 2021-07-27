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
#include <glamrock/GrDialog.h>
#include <glamrock/GrPanel.h>

#define TBH_MULTIPLIER 1.2 // horizontal title bar vs. text multiplier
#define TBV_MULTIPLIER 1.3 // vertical title bar vs. text multiplier
#define TBR_PERCENT 0.4    // title bar corner rounding percentage
#define DR_PERCENT 0.2     // dialog corner rounding percentage

GrDialog::GrDialog()
{
}

GrDialog::~GrDialog()
{
}

void GrDialog::layoutInGLArea(float minX, float minY,
                              float width, float height,
                              float layer)
{
   layoutInGLVolume(minX, minY, layer, width, height, 0.0);
}

void GrDialog::layoutInGLVolume(float minX, float minY, float minZ,
                                float width, float height, float depth)
{
   GmDialog::layoutInGLVolume(minX, minY, minZ, width, height, depth);
   
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   float tbHeight, tbWidth; // title bar width and height
   mTitleTextWidget->getGLExtents(tbWidth, tbHeight);
   tbHeight *= TBV_MULTIPLIER;
   tbWidth *= TBH_MULTIPLIER;

   mTitleTextWidget->layoutInGLVolume(x + (mGLWidth / 2.0) - (tbWidth / 2.0),
                                      y + mGLHeight - tbHeight,
                                      z, tbWidth, tbHeight, mGLDepth);
}

void GrDialog::render2d(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   if(!getDisplayListValid())
   {
      if(getDisplayListId() != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);
      
      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);

      float r, g, b, a;
      mBaseColor.getRGBA(r, g, b, a);
      
      // Draw the base part of the dialog
      glColor4f(r, g, b, a);
      
      GrShapeFactory* sf = GrShapeFactory::getInstance();

      sf->renderRoundedRect(GrTypes::BS_ALL,
                            GrTypes::FS_SOLID,
                            mGLWidth, mGLHeight, DR_PERCENT);

      // Draw the border for the dialog
      mBaseColor.getRGBA(r, g, b, a);
      glColor4f(r*0.5, g*0.5, b*0.5, a);
      
      glPushMatrix();
//       sf->renderRoundedRect(GrTypes::BS_ALL,
//                             GrTypes::FS_OUTLINE,
//                             mGLWidth, mGLHeight, DR_PERCENT);
      glTranslatef(mGLWidth*0.01, mGLHeight*0.01, 0.0);
      sf->renderRoundedRect(GrTypes::BS_ALL,
                            GrTypes::FS_OUTLINE,
                            mGLWidth*0.98, mGLHeight*0.98, DR_PERCENT);
      glPopMatrix();

      float tbHeight, tbWidth; // title bar width and height
      mTitleTextWidget->getGLExtents(tbWidth, tbHeight);
      tbHeight *= TBV_MULTIPLIER;
      tbWidth *= TBH_MULTIPLIER;
      
      // Draw the titlebar of the dialog
      glPushMatrix();
      mTitlebarColor.getRGBA(r, g, b, a);
      glColor4f(r, g, b, a);
      
      //draw the titlebar
      glTranslatef((mGLWidth - tbWidth) / 2.0,
                   mGLHeight - tbHeight, 0.0);
      sf->renderRoundedRect(GrTypes::BS_BOTTOM,
                            GrTypes::FS_SOLID,
                            tbWidth, tbHeight, TBR_PERCENT);
   
      // Draw the border for the titlebar
      mBaseColor.getRGBA(r, g, b, a);
      glColor4f(r*0.5, g*0.5, b*0.5, a);
      sf->renderRoundedRect(GrTypes::BS_BOTTOM,
                            GrTypes::FS_OUTLINE,
                            tbWidth, tbHeight, TBR_PERCENT);

      glPopMatrix();

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
   
   // Draw the titlebar text for the dialog
   mTitleTextWidget->render(renderOptions);

   // render components
   GmDialog::render2d(renderOptions);
}

void GrDialog::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   GrShapeFactory* sf = GrShapeFactory::getInstance();

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);
   
   glPushName(mObjectId);

   glColor4f(0.3, 0.3, 0.3, 1.0);
   sf->renderRoundedRect(GrTypes::BS_ALL,
                         GrTypes::FS_SOLID,
                         mGLWidth, mGLHeight, DR_PERCENT);
   glPopName();

   glPopMatrix();

   // restore color settings
   glPopAttrib();

   // render components
   GmDialog::render2dSelect(renderOptions);
}
