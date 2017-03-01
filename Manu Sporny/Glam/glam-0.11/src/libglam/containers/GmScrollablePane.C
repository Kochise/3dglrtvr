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
#include <glam/GmScrollablePane.h>
#include <glam/GmListLayout.h>

GmScrollablePane::GmScrollablePane()
{
   mHorzScrollbar = new GmScrollbar();
   mHorzScrollbar->setOrientation(GmTypes::OT_HORIZONTAL);
   mHorzScrollbar->setWidgetId(SCROLLBAR_H);
   mHorzScrollbar->addScalarListener(this);
   //mHorzScrollbar->setRelativeGeometry(0.0, 0.0, 0.9, 0.1);
   //mHorzScrollbar->setBaseColor(0.6, 0.6, 0.6, 1.0);
   mHorzScrollbar->setBorderWidth(0.1);
   mHorzScrollbar->setVisible(false);
   mHorzScrollbar->setMinimumValue(0.0);
   mHorzScrollbar->setMaximumValue(0.0);
   mHorzScrollbar->setCurrentValue(0.0);
   
   mVertScrollbar = new GmScrollbar();
   mVertScrollbar->setOrientation(GmTypes::OT_VERTICAL);
   mVertScrollbar->setWidgetId(SCROLLBAR_V);
   mVertScrollbar->addScalarListener(this);
   //mVertScrollbar->setRelativeGeometry(0.9, 0.1, 1.0, 1.0);
   //mVertScrollbar->setBaseColor(0.6, 0.6, 0.6, 1.0);
   mVertScrollbar->setBorderWidth(0.1);
   mVertScrollbar->setVisible(false);
   mVertScrollbar->setMinimumValue(0.0);
   mVertScrollbar->setMaximumValue(0.0);
   mVertScrollbar->setCurrentValue(0.0);

   //set the default layout (a list layout)
   GmListLayout* ll = new GmListLayout();
   setLayout(ll);
}

GmScrollablePane::~GmScrollablePane()
{
   delete mHorzScrollbar;
   delete mVertScrollbar;
}

void GmScrollablePane::addComponent(GmDrawable* component)
{
   mComponents.push_back(component);
}

void GmScrollablePane::addComponent(GmInteractiveWidget* component)
{
   component->addGeometryListener(this);
   GmContainer::addComponent(component);
}

void GmScrollablePane::changeOccurred(GmScalarAction* action)
{
   float xChange = 0.0, yChange = 0.0;
   if(action->getSrcWidgetId() == SCROLLBAR_H)
      xChange = action->getChange();
   if(action->getSrcWidgetId() == SCROLLBAR_V)
      yChange = action->getChange();

   translateComponentsGLPositions(xChange, -yChange, 0.0);
}

void GmScrollablePane::geometryResized(GmGeometryAction* action)
{
   // layout the pane again
   relayout();
}

void GmScrollablePane::setGLPosition(float x, float y, float z)
{
   float dX, dY, dZ;
   mGLPosition.getXYZ(dX, dY, dZ);

   dX = x - dX;
   dY = y - dY;
   dZ = z - dZ;

   // set container's position
   GmDrawable::setGLPosition(x, y, z);

   // set the scrollbars positions
   mHorzScrollbar->getGLPosition(x, y, z);
   mHorzScrollbar->setGLPosition(x + dX, y + dY, z + dZ);
   mVertScrollbar->getGLPosition(x, y, z);
   mVertScrollbar->setGLPosition(x + dX, y + dY, z + dZ);

   // set all of the components' positions
   translateComponentsGLPositions(dX, dY, dZ);
}

void GmScrollablePane::layoutInGLVolume(float minX, float minY, float minZ,
                                        float width, float height,
                                        float depth)
{
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);

   float horzChange = mHorzScrollbar->getCurrentValue();
   float vertChange = mVertScrollbar->getCurrentValue();

   // reapply layout to components because of new geometry
   applyLayout();

   // set the new scrollbar values
   setScrollbarValues();

   if(horzChange > mHorzScrollbar->getMaximumValue())
      horzChange = mHorzScrollbar->getMaximumValue();
   if(!mHorzScrollbar->getVisible())
      horzChange = 0.0;

   if(vertChange > mVertScrollbar->getMaximumValue())
      vertChange = mVertScrollbar->getMaximumValue();
   if(!mVertScrollbar->getVisible())
      vertChange = 0.0;

   translateComponentsGLPositions(-horzChange, vertChange, 0.0);
}

void GmScrollablePane::setScrollbarValues()
{
   if(mComponents.size() == 0)
   {
      mHorzScrollbar->setVisible(false);
      mVertScrollbar->setVisible(false);
      return;
   }

   float width = 0.0, height = 0.0;
   getGLExtents(width, height);
   
   float excessWidth = width - mGLWidth;
   float excessHeight = height - mGLHeight;

   mHorzScrollbar->setVisible( (excessWidth > 0.0) );
   mVertScrollbar->setVisible( (excessHeight > 0.0) );

   if(mHorzScrollbar->getVisible() && height > mGLHeight * 0.9)
      mVertScrollbar->setVisible(true);
   if(mVertScrollbar->getVisible() && width > mGLWidth * 0.9)
      mHorzScrollbar->setVisible(true);

   if(mHorzScrollbar->getVisible())
   {
      mHorzScrollbar->setMinimumValue(0.0);
      mHorzScrollbar->setPageStep(mGLWidth);
      mHorzScrollbar->setLineStep(mGLWidth / 10.0);
      
      if(mVertScrollbar->getVisible())
      {
         mHorzScrollbar->setRelativeGeometry(0.0, 0.0, 0.9, 0.1);
         // add extra amount one must scroll if there is a scroll bar
         mHorzScrollbar->setMaximumValue(excessWidth + 0.1 * mGLWidth);
      }
      else
      {
         mHorzScrollbar->setRelativeGeometry(0.0, 0.0, 1.0, 0.1);
         mHorzScrollbar->setMaximumValue(excessWidth);
      }
   }
   
   if(mVertScrollbar->getVisible())
   {
      mVertScrollbar->setMinimumValue(0.0);
      mVertScrollbar->setPageStep(mGLHeight);
      mVertScrollbar->setLineStep(mGLHeight / 10.0);

      if(mHorzScrollbar->getVisible())
      {
         mVertScrollbar->setRelativeGeometry(0.9, 0.1, 1.0, 1.0);
         // add extra amount one must scroll if there is a scroll bar
         mVertScrollbar->setMaximumValue(excessHeight + 0.1 * mGLHeight);
      }
      else
      {
         mVertScrollbar->setRelativeGeometry(0.9, 0.0, 1.0, 1.0);
         mVertScrollbar->setMaximumValue(excessHeight);
      }
   }
   
   //layout scrollbars
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   mHorzScrollbar->layoutInGLVolume(x, y, z, mGLWidth, mGLHeight, mGLDepth);
   mVertScrollbar->layoutInGLVolume(x, y, z, mGLWidth, mGLHeight, mGLDepth);
}

void GmScrollablePane::render2d(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float minY = 0.0, maxX = mGLWidth;

   if(mVertScrollbar->getVisible())
      maxX = mGLWidth * 0.9;
   if(mHorzScrollbar->getVisible())
      minY = mGLHeight - (mGLHeight * 0.9);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   float r, g, b, a;
   mBaseColor.getRGBA(r, g, b, a);

   glPushMatrix();
   glTranslatef(x, y, z);
   
   glColor4f(r, g, b, a);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();
   
   glColor4f(r * 0.5, g * 0.5, b * 0.5, a);
   glBegin(GL_LINE_LOOP);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   //corner block (if both scrollbars are visible)
   if(mVertScrollbar->getVisible() && mHorzScrollbar->getVisible())
   {
      mVertScrollbar->getBaseColor(r, g, b, a);
      glColor4f(r * 0.5, g * 0.5, b * 0.5, a);
      glBegin(GL_POLYGON);
      glVertex2f(mGLWidth * 0.9, 0.0);
      glVertex2f(mGLWidth, 0.0);
      glVertex2f(mGLWidth, mGLHeight * 0.1);
      glVertex2f(mGLWidth * 0.9, mGLHeight * 0.1);
      glEnd();
   }
   
   glPopMatrix();

   // restore color settings
   glPopAttrib();
   
   beginClip(0.0, -minY, maxX, mGLHeight);
   GmContainer::render2d(renderOptions);
   endClip();
   
   mVertScrollbar->render(renderOptions);
   mHorzScrollbar->render(renderOptions);
}

void GmScrollablePane::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   float minY = 0.0, maxX = mGLWidth;
   if(mVertScrollbar->getVisible())
      maxX = mGLWidth * 0.9;
   if(mHorzScrollbar->getVisible())
      minY = mGLHeight - (mGLHeight * 0.9);
   
   beginClip(0.0, -minY, maxX, mGLHeight);
   GmContainer::render2d(renderOptions);
   endClip();
   
   mHorzScrollbar->render(renderOptions);
   mVertScrollbar->render(renderOptions);
}
