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
#include <glamrock/GrMenu.h>
 
#define BMR_PERCENT 0.25//0.5     // Panel rounding percentage

GrMenu::GrMenu() 
      : GmMenu()
{
}

GrMenu::~GrMenu()
{
}

void GrMenu::render2d(GmTypes::RenderOptionType renderOptions)
{
   // render the label's tooltip
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

      glPushMatrix();
      
      GrShapeFactory* sf = GrShapeFactory::getInstance();
   
      float r, g, b, a;
      mBaseColor.getRGBA(r, g, b, a);
      
      // Draw the background
      glColor4f(r, g, b, a);
      
      if(!mSubMenu)
      {
         if(isOpen())
         {
            // solid
            sf->sfBegin(GrTypes::FS_SOLID);
            sf->sfFilletSize(1.0);
            sf->sfVertex2f(0.0, 0.0);
            sf->sfVertex2f(0.0, mGLHeight);
            sf->sfVertex2f(mGLWidth, mGLHeight);
            sf->sfVertex2f(mGLWidth, 0.0);
            //sf->sfFilletSize(2.0);
            sf->sfVertex2f(mGLMenuWidth, 0.0);
            sf->sfVertex2f(mGLMenuWidth, -mGLMenuHeight);
            sf->sfVertex2f(0.0, -mGLMenuHeight);
            sf->sfEnd();
            
            // outline
            glColor4f(r*0.5, g*0.5, b*0.5, a);
            sf->sfBegin(GrTypes::FS_OUTLINE);
            sf->sfFilletSize(1.0);
            sf->sfVertex2f(0.0, 0.0);
            sf->sfVertex2f(0.0, mGLHeight);
            sf->sfVertex2f(mGLWidth, mGLHeight);
            sf->sfVertex2f(mGLWidth, 0.0);
            //sf->sfFilletSize(2.0);
            sf->sfVertex2f(mGLMenuWidth, 0.0);
            sf->sfVertex2f(mGLMenuWidth, -mGLMenuHeight);
            sf->sfVertex2f(0.0, -mGLMenuHeight);
            sf->sfEnd();
         }
         else
         {
            sf->renderRoundedRect(GrTypes::BS_ALL,
                                  GrTypes::FS_SOLID,
                                  mGLWidth, mGLHeight, BMR_PERCENT);
            glColor4f(r*0.5, g*0.5, b*0.5, a);
            sf->renderRoundedRect(GrTypes::BS_ALL,
                                  GrTypes::FS_OUTLINE,
                                  mGLWidth, mGLHeight, BMR_PERCENT);
         }
      }
      else //is a submenu
      {
         if(isOpen())
         {
            // solid
            sf->sfBegin(GrTypes::FS_SOLID);
            sf->sfFilletSize(1.0);
            sf->sfVertex2f(0.0, 0.0);
            sf->sfVertex2f(0.0, mGLHeight);
            sf->sfVertex2f(mGLWidth, mGLHeight);
            //sf->sfFilletSize(2.0);
            sf->sfVertex2f(mGLWidth + mGLMenuWidth, mGLHeight);
            sf->sfVertex2f(mGLWidth + mGLMenuWidth, mGLHeight - mGLMenuHeight);
            sf->sfVertex2f(mGLWidth, mGLHeight - mGLMenuHeight);
            sf->sfVertex2f(mGLWidth, 0.0);
            sf->sfEnd();
            
            // outline
            glColor4f(r*0.5, g*0.5, b*0.5, a);
            sf->sfBegin(GrTypes::FS_OUTLINE);
            sf->sfFilletSize(1.0);
            sf->sfVertex2f(0.0, 0.0);
            sf->sfVertex2f(0.0, mGLHeight);
            sf->sfVertex2f(mGLWidth, mGLHeight);
            //sf->sfFilletSize(2.0);
            sf->sfVertex2f(mGLWidth + mGLMenuWidth, mGLHeight);
            sf->sfVertex2f(mGLWidth + mGLMenuWidth, mGLHeight - mGLMenuHeight);
            sf->sfVertex2f(mGLWidth, mGLHeight - mGLMenuHeight);
            sf->sfVertex2f(mGLWidth, 0.0);
            sf->sfEnd();
         }
         else
         {
            sf->renderRoundedRect(GrTypes::BS_ALL,
                                  GrTypes::FS_SOLID,
                                  mGLWidth, mGLHeight, BMR_PERCENT);
            glColor4f(r*0.5, g*0.5, b*0.5, a);
            sf->renderRoundedRect(GrTypes::BS_ALL,
                                  GrTypes::FS_OUTLINE,
                                  mGLWidth, mGLHeight, BMR_PERCENT);
         }
      }
      
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
   
   // render menu items
   if(isOpen())
      renderMenuItems(renderOptions);

   // render title text
   if(mLabel != NULL)
      mLabel->render(renderOptions);
}

void GrMenu::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   GrShapeFactory* sf = GrShapeFactory::getInstance();

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);

   glColor4f(0.3, 0.8, 0.3, 1.0);
   sf->renderRoundedRect(GrTypes::BS_ALL,
                         GrTypes::FS_SOLID,
                         mGLWidth, mGLHeight, BMR_PERCENT);
   glPopName();

   // render the background of the menu
   if(isOpen())
   {
      glColor4f(0.7, 0.7, 1.0, 1.0);      
      
      // a non-submenu rendering
      if(!mSubMenu)
      {
         glBegin(GL_POLYGON);
         glVertex2f(0.0, 0.0);
         glVertex2f(0.0, -mGLMenuHeight);
         glVertex2f(mGLMenuWidth, -mGLMenuHeight);
         glVertex2f(mGLMenuWidth, 0.0);
         glEnd();
      }
      // submenu rendering
      else
      {
         glBegin(GL_POLYGON);
         glVertex2f(mGLWidth, mGLHeight);
         glVertex2f(mGLWidth, mGLHeight - mGLMenuHeight);
         glVertex2f(mGLWidth + mGLMenuWidth, mGLHeight - mGLMenuHeight);
         glVertex2f(mGLWidth + mGLMenuWidth, mGLHeight);
         glEnd();
      }
   }   

   glPopMatrix();

   // restore color settings
   glPopAttrib();
   
   // Render the menu items
   if(isOpen())
      renderMenuItems(renderOptions);
}
