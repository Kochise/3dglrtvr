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
#include <glam/GmExpandableListItem.h>

GmExpandableListItem::GmExpandableListItem()
{
   mTitleComponent = NULL;
   mBodyComponent = NULL;

   mExpandState = GmTypes::ES_UNEXPANDED;
}

GmExpandableListItem::GmExpandableListItem(GmDrawable* titleComponent,
                                           GmDrawable* bodyComponent)
{
   mTitleComponent = titleComponent;
   mBodyComponent = bodyComponent;

   mExpandState = GmTypes::ES_UNEXPANDED;
}

GmExpandableListItem::~GmExpandableListItem()
{
}

GmDrawable* GmExpandableListItem::getTitleWidget()
{
   return mTitleComponent;
}

GmDrawable* GmExpandableListItem::getBodyWidget()
{
   return mBodyComponent;
}

void GmExpandableListItem::mousePressEvent(GmEvent* event)
{
   GmInteractiveWidget::mousePressEvent(event);
   
   mExpandState = (mExpandState == GmTypes::ES_EXPANDED) ?
      GmTypes::ES_UNEXPANDED : GmTypes::ES_EXPANDED;

   setDisplayListValid(false);
}

void GmExpandableListItem::layoutInGLVolume(float minX, float minY,
                                            float minZ,
                                            float width, float height,
                                            float depth)
{
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   float tw, th;
   mTitleComponent->getGLExtents(tw, th);

   mTitleComponent->layoutInGLVolume(x, y + mGLHeight - th, z + GLAM_LAYER,
                                     mGLWidth - mGLWidth*0.1, th, mGLDepth);
   
   mBodyComponent->layoutInGLVolume(x, y, z + GLAM_LAYER,
                                    mGLWidth, mGLHeight - th, mGLDepth);
}

void GmExpandableListItem::getGLExtents(float& width, float& height)
{
   float w, h;

   /// TEMP CODE:
   /// TEMPORARILY CAUSED GL EXTENTS TO ALWAYS REPORT MAXIMUM
      /// WHICH IS THE SIZE FROM THE EXPANDED STATE
   
   //if(mExpandState == GmTypes::ES_UNEXPANDED)
   //{
   //   if(getTitleWidget() != NULL)
   //      mTitleComponent->getGLExtents(width, height);
   //   else
   //      width = height = 0.0;
   // }
   // else
   //{
      if(getTitleWidget() != NULL)
      {
         mTitleComponent->getGLExtents(w, h);
         width = w;
         height = h;
      }
      
      if(getBodyWidget() != NULL)
      {
         mBodyComponent->getGLExtents(w, h);
         height += h;
      }
      // }
}

void GmExpandableListItem::render2d(GmTypes::RenderOptionType renderOptions)
{
   render2dToolTip(renderOptions);
   
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   if(!getDisplayListValid())
   {
      if(getDisplayListId() != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);

      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);
      
      float th = mTitleComponent->getGLHeight();
   
      float baseOffset = 0.0;
      if(mExpandState != GmTypes::ES_EXPANDED)
         baseOffset = mGLHeight - th;
      
      // draw the widget border
      glColor4f(0.0, 0.0, 0.0, 1.0);
      glBegin(GL_LINE_LOOP);
      glVertex2f(0.0, baseOffset);
      glVertex2f(0.0, mGLHeight);
      glVertex2f(mGLWidth, mGLHeight);
      glVertex2f(mGLWidth, baseOffset);
      glEnd();
      
      glColor4f(1.0, 1.0, 1.0, 1.0);
      if(mExpandState == GmTypes::ES_EXPANDED)
      {
         // draw the expander arrow
         glBegin(GL_POLYGON);
         glVertex2f(mGLWidth-(mGLWidth*0.1), mGLHeight-th);
         glVertex2f(mGLWidth-(mGLWidth*0.05), mGLHeight);
         glVertex2f(mGLWidth, mGLHeight-th);
         glEnd();
      }
      else
      {
         // draw the expander arrow
         glBegin(GL_POLYGON);
         glVertex2f(mGLWidth-(mGLWidth*0.1), mGLHeight);
         glVertex2f(mGLWidth, mGLHeight);
         glVertex2f(mGLWidth-(mGLWidth*0.05), mGLHeight - th);
         glEnd();
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

void GmExpandableListItem::render2dSelect(GmTypes::RenderOptionType
                                          renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

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

   mTitleComponent->render(renderOptions);
}
