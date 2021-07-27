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
#include <glamrock/GrFrame.h>
#include <glamrock/GrShapeFactory.h>

#define TBV_MULTIPLIER 1.3 // vertical title bar vs. text multiplier
#define TBR_PERCENT 0.4    // title bar corner rounding percentage
#define DR_PERCENT 0.2     // dialog corner rounding percentage

#define CBH_MULTIPLIER 0.15  // close button horizontal multiplier
//#define CBV_MULTIPLIER 0.3  // close button vertical multiplier
#define CBR_PERCENT 0.2     // close button rounding percentage

GrFrame::GrFrame()
      : GmFrame()
{
}

GrFrame::~GrFrame()
{
}

void GrFrame::layoutInGLVolume(float minX, float minY, float minZ,
                               float width, float height, float depth)
{
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);
   
   // calculate the titlebar height
   if(mTitleTextWidget)
   {
      float tbHeight = mTitleTextWidget->getTextSize();

      if(mGLHeight != 0)
         mTitlebarHeight = (tbHeight * TBV_MULTIPLIER) / mGLHeight;

      // gl height is updated so reposition the components
      positionCloseButton();
      positionContentPanel();
      positionTitleText();

      float x, y, z;
      mGLPosition.getXYZ(x, y, z);

      mTitleTextWidget->layoutInGLVolume(x, y, z,
                                         mGLWidth, mGLHeight, mGLDepth);      
   }

   // layout components
   GmContainer::applyLayout();
}

void GrFrame::render2d(GmTypes::RenderOptionType renderOptions)
{
   // save color and line settings
   glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);
   
   if(!getDisplayListValid())
   {
      if(getDisplayListId() != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);
      
      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);

      float r, g, b, a;
      mBaseColor.getRGBA(r, g, b, a);

      GrShapeFactory* sf = GrShapeFactory::getInstance();

      float contentHeight = mGLHeight - mTitlebarHeight * mGLHeight;
      
      // Draw the content part of the frame
      glColor4f(r, g, b, a);
      sf->renderRoundedRect(GrTypes::BS_BOTTOM,
                            GrTypes::FS_SOLID,
                            mGLWidth, contentHeight, DR_PERCENT);
     
      // Draw the border for the content part of the frame
      glLineWidth(2.0);
      glColor4f(r * 0.5, g * 0.5, b * 0.5, a);
      
      sf->renderRoundedRect(GrTypes::BS_BOTTOM,
                            GrTypes::FS_OUTLINE,
                            mGLWidth, contentHeight, DR_PERCENT);
   
      // Draw the titlebar of the frame
      glPushMatrix();
   
      mTitlebarColor.getRGBA(r, g, b, a);
      glColor4f(r, g, b, a);
      
      glTranslatef(0.0, contentHeight, 0.0);
      sf->renderRoundedRect(GrTypes::BS_TOP,
                            GrTypes::FS_SOLID,
                            mGLWidth, mTitlebarHeight * mGLHeight,
                            TBR_PERCENT);
   
      // Draw the border for the titlebar
      glLineWidth(2.0);
      glColor4f(r * 0.5, g * 0.5, b * 0.5, a);
   
      sf->renderRoundedRect(GrTypes::BS_TOP,
                            GrTypes::FS_OUTLINE,
                            mGLWidth, mTitlebarHeight * mGLHeight,
                            TBR_PERCENT);
   
      // pop off title matrix
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

   // restore color and line settings
   glPopAttrib();
   
   // Draw the text widget
   if(mTitleTextWidget != NULL)
      mTitleTextWidget->render(renderOptions);

   // render components of frame
   GmContainer::render2d(renderOptions);
}
