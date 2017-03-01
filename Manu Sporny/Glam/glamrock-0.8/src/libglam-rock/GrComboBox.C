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

#include <glam/GmTypes.h>
#include <glamrock/GrComboBox.h>
#include <glamrock/GrShapeFactory.h>

#define CBR_PERCENT 0.4

GrComboBox::GrComboBox()
{
}

GrComboBox::~GrComboBox()
{
}

void GrComboBox::render2d(GmTypes::RenderOptionType renderOptions)
{
   // render label's tooltip
   if(getMouseOver() && mLabel != NULL)
      mLabel->render2dToolTip(renderOptions);
   
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

      // Draw the combo box decorators
      if(isOpen())
      {
         glColor4f(0.0, 0.0, 0.0, 1.0);
         glBegin(GL_LINES);
         glVertex2f(0.0, 0.0);
         glVertex2f(mGLWidth, 0.0);
         glEnd();
         
         // Draw the decorator arrow
         glColor4f(r*0.2, g*0.2, b*0.2, a);
         sf->sfBegin(GrTypes::FS_SOLID);
         sf->sfFilletSize(mGLHeight*0.1);
         sf->sfVertex2f(mGLWidth*0.82,
                        mGLHeight*0.2);
         sf->sfVertex2f(mGLWidth*0.9,
                        mGLHeight*0.9);
         sf->sfVertex2f(mGLWidth*0.98,
                        mGLHeight*0.2);
         sf->sfEnd();
         glColor4f(0.0, 0.0, 0.0, 1.0);
         sf->sfBegin(GrTypes::FS_OUTLINE);
         sf->sfFilletSize(mGLHeight*0.1);
         sf->sfVertex2f(mGLWidth*0.82,
                        mGLHeight*0.2);
         sf->sfVertex2f(mGLWidth*0.9,
                        mGLHeight*0.9);
         sf->sfVertex2f(mGLWidth*0.98,
                        mGLHeight*0.2);
         sf->sfEnd();
      }
      else
      {
         // Draw the decorator arrow
         glColor4f(r*0.2, g*0.2, b*0.2, a);
         sf->sfBegin(GrTypes::FS_SOLID);
         sf->sfFilletSize(mGLHeight*0.1);
         sf->sfVertex2f(mGLWidth*0.82,
                        mGLHeight*0.8);
         sf->sfVertex2f(mGLWidth*0.9,
                        mGLHeight*0.1);
         sf->sfVertex2f(mGLWidth*0.98,
                        mGLHeight*0.8);
         sf->sfEnd();
         glColor4f(0.0, 0.0, 0.0, 1.0);
         sf->sfBegin(GrTypes::FS_OUTLINE);
         sf->sfFilletSize(mGLHeight*0.1);
         sf->sfVertex2f(mGLWidth*0.82,
                        mGLHeight*0.8);
         sf->sfVertex2f(mGLWidth*0.9,
                        mGLHeight*0.1);
         sf->sfVertex2f(mGLWidth*0.98,
                        mGLHeight*0.8);
         sf->sfEnd();
      }
      
      glColor4f(0.0, 0.0, 0.0, 1.0);
      glBegin(GL_LINES);
      glVertex2f(mGLWidth*0.8, 0.0);
      glVertex2f(mGLWidth*0.8, mGLHeight);
      glEnd();
      
      // Draw the main part of the menu
      if(isOpen())
      {
         glColor4f(r, g, b, a);
         glPushMatrix();
         glTranslatef(0.0, -mGLMenuHeight, 0.0);
         sf->renderRoundedRect(GrTypes::BS_ALL,
                               GrTypes::FS_SOLID,
                               mGLMenuWidth, mGLMenuHeight+mGLHeight,
                               CBR_PERCENT/3.0);

         glColor4f(r*0.4, g*0.4, b*0.4, a);
         sf->renderRoundedRect(GrTypes::BS_ALL,
                               GrTypes::FS_OUTLINE,
                               mGLMenuWidth, mGLMenuHeight+mGLHeight,
                               CBR_PERCENT/3.0);
         glPopMatrix();
      }
      else
      {
         glColor4f(r, g, b, a);
         sf->renderRoundedRect(GrTypes::BS_ALL,
                               GrTypes::FS_SOLID,
                               mGLWidth, mGLHeight, CBR_PERCENT);

         glColor4f(r*0.4, g*0.4, b*0.4, a);
         sf->renderRoundedRect(GrTypes::BS_ALL,
                               GrTypes::FS_OUTLINE,
                               mGLWidth, mGLHeight, CBR_PERCENT);
      }
      
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

   // render all of the menu items
   if(isOpen())
      renderMenuItems(renderOptions);
   
   // Draw the selection
   if(mCurrentSelection != NULL)
   {
      GlmPoint3* cp;
      float cx, cy, cz;

      glPushMatrix();
      cp = mCurrentSelection->getGLPosition();
      cp->getXYZ(cx, cy, cz);
      glTranslatef(0.0, (y - cy) + (mGLHeight/4.0), 0.0);
      mCurrentSelection->render(renderOptions);
      glPopMatrix();
   }
   
   else if(mLabel != NULL)
      mLabel->render(renderOptions);
}

void GrComboBox::getGLExtents(float& width, float& height)
{
   float maxWidth = 1.0;
   float maxHeight = 1.0;
   width = 1.0;
   height = 1.0;

   if(mLabel != NULL)
      mLabel->getGLExtents(maxWidth, maxHeight);

   // Calculate the widest component in the combo box
   for(vector<GmInteractiveWidget*>::iterator li = mMenuItems.begin();
       li != mMenuItems.end(); li++)
   {
      (*li)->getGLExtents(width, height);
      
      if(width > maxWidth)
         maxWidth = width;
      if(height > maxHeight)
         maxHeight = height;
   }
   
   width = (maxWidth * 1.5);
   height = (maxHeight * 2.0);
}
