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
#include <glamrock/GrExpandableListItem.h>
#include <glamrock/GrShapeFactory.h>

#define CBR_PERCENT 0.4

GrExpandableListItem::GrExpandableListItem(GmDrawable* titleComponent,
                                           GmDrawable* bodyComponent)
      : GmExpandableListItem(titleComponent, bodyComponent)
{
}

GrExpandableListItem::GrExpandableListItem()
      : GmExpandableListItem()
{
}

GrExpandableListItem::~GrExpandableListItem()
{
}

void GrExpandableListItem::render2d(GmTypes::RenderOptionType renderOptions)
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

      float th = mTitleComponent->getGLHeight();

      float r, g, b, a;
      mBaseColor.getRGBA(r, g, b, a);
      
      float baseOffset = 0.0;
      if(mExpandState != GmTypes::ES_EXPANDED)
         baseOffset = mGLHeight - th;
      
      // draw the widget border
      glColor4f(r, g, b, a);
      sf->sfBegin(GrTypes::FS_OUTLINE);
      sf->sfFilletSize(th * 0.2);
      sf->sfVertex2f(0.0, baseOffset);
      sf->sfVertex2f(0.0, mGLHeight);
      sf->sfVertex2f(mGLWidth, mGLHeight);
      sf->sfVertex2f(mGLWidth, baseOffset);
      sf->sfEnd();
      
      
      if(mExpandState == GmTypes::ES_EXPANDED)
      {
         // Draw the decorator arrow
         glColor4f(r * 0.8, g * 0.8, b * 0.8, a);
         glBegin(GL_POLYGON);
         glVertex2f(mGLWidth-(mGLWidth*0.1), mGLHeight-th);
         glVertex2f(mGLWidth-(mGLWidth*0.05), mGLHeight);
         glVertex2f(mGLWidth, mGLHeight-th);
         glEnd();
         
//          glColor4f(r*0.8, g*0.8, b*0.8, a);
//          sf->sfBegin(GrTypes::FS_SOLID);
//          sf->sfFilletSize(1.0);
//          sf->sfVertex2f(mGLWidth-(mGLWidth*0.1), mGLHeight-th);
//          sf->sfVertex2f(mGLWidth-(mGLWidth*0.055), mGLHeight);
//          sf->sfVertex2f(mGLWidth-(mGLWidth*0.01), mGLHeight-th);
//          sf->sfEnd();
         
//          glColor4f(1.0, 1.0, 1.0, a);
//          sf->sfBegin(GrTypes::FS_OUTLINE);
//          sf->sfFilletSize(1.0);
//          sf->sfVertex2f(mGLWidth-(mGLWidth*0.1), mGLHeight-th);
//          sf->sfVertex2f(mGLWidth-(mGLWidth*0.055), mGLHeight);
//          sf->sfVertex2f(mGLWidth-(mGLWidth*0.01), mGLHeight-th);
//          sf->sfEnd();
      }
      else
      {
         // Draw the decorator arrow
         glColor4f(r * 0.8, g * 0.8, b * 0.8, a);
         glBegin(GL_POLYGON);
         glVertex2f(mGLWidth-(mGLWidth*0.1), mGLHeight);
         glVertex2f(mGLWidth, mGLHeight);
         glVertex2f(mGLWidth-(mGLWidth*0.05), mGLHeight - th);
         glEnd();

//          glColor4f(r*0.8, g*0.8, b*0.8, a);
//          sf->sfBegin(GrTypes::FS_SOLID);
//          sf->sfFilletSize(1.0);
//          sf->sfVertex2f(mGLWidth-(mGLWidth*0.1), mGLHeight);
//          sf->sfVertex2f(mGLWidth-(mGLWidth*0.01), mGLHeight);
//          sf->sfVertex2f(mGLWidth-(mGLWidth*0.055), mGLHeight - th);
//          sf->sfEnd();
         
//          glColor4f(1.0, 1.0, 1.0, a);
//          sf->sfBegin(GrTypes::FS_OUTLINE);
//          sf->sfFilletSize(1.0);
//          sf->sfVertex2f(mGLWidth-(mGLWidth*0.1), mGLHeight);
//          sf->sfVertex2f(mGLWidth-(mGLWidth*0.01), mGLHeight);
//          sf->sfVertex2f(mGLWidth-(mGLWidth*0.055), mGLHeight - th);
//          sf->sfEnd();
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

   mTitleComponent->render(renderOptions);

   if(mExpandState == GmTypes::ES_EXPANDED)
      mBodyComponent->render(renderOptions);
}

void GrExpandableListItem::render2dSelect(GmTypes::RenderOptionType
                                          renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   // render the title widget
   mTitleComponent->render(renderOptions);

   float th = mTitleComponent->getGLHeight();
   
   if(mExpandState == GmTypes::ES_EXPANDED)
      mBodyComponent->render(renderOptions);
   
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);

   glColor4f(0.8, 0.5, 1.0, 1.0);   
   glBegin(GL_POLYGON);
   glVertex2f(mGLWidth-(mGLWidth*0.1), mGLHeight-th);
   glVertex2f(mGLWidth-(mGLWidth*0.1), mGLHeight);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(mGLWidth, mGLHeight-th);
   glEnd();

   glPopName();

   glPopMatrix();

   // restore color settings
   glPopAttrib();
}
