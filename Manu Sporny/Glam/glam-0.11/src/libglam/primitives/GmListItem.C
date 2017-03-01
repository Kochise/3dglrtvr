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
#include <glam/GmListItem.h>

GmListItem::GmListItem()
{
   mLabel = new GmLabel();
}

GmListItem::~GmListItem()
{
   delete mLabel;
}

void GmListItem::fireSelection(GmSelectionAction* action)
{
   vector<GmSelectionListener*>::iterator i;
   for(i = mSelectionListeners.begin(); i != mSelectionListeners.end(); i++)
      (*i)->selectionOccurred(action);
}

void GmListItem::fireDeselection(GmSelectionAction* action)
{
   vector<GmSelectionListener*>::iterator i;
   for(i = mSelectionListeners.begin(); i != mSelectionListeners.end(); i++)
      (*i)->deselectionOccurred(action);
}

void GmListItem::addSelectionListener(GmSelectionListener* sl)
{
   mSelectionListeners.push_back(sl);
}

bool GmListItem::removeSelectionListener(GmSelectionListener* sl)
{
   vector<GmSelectionListener*>::iterator i;
   for(i = mSelectionListeners.begin(); i != mSelectionListeners.end(); i++)
   {
      // compare the listeners
      if(*(*i) == *sl)
      {
         mSelectionListeners.erase(i);
         return true;
      }
   }

   return false;
}

void GmListItem::mousePressEvent(GmEvent* event)
{
   GmInteractiveWidget::mousePressEvent(event);
   
   GmSelectionAction* action = new GmSelectionAction(this, this);
   fireSelection(action);
   delete action;
}

void GmListItem::setText(string text)
{
   mLabel->setText(text);
}

void GmListItem::setTextColor(GlmColor4* color)
{
   mLabel->setTextColor(color);
}

void GmListItem::setTextColor(float r, float g, float b, float a)
{
   mLabel->setTextColor(r, g, b, a);
}

void GmListItem::setTextSize(float size)
{
   mLabel->setTextSize(size);
}

void GmListItem::setTextJustification(GmTypes::JustificationType horizontal,
                                      GmTypes::JustificationType vertical)
{
   mLabel->setTextJustification(horizontal, vertical);
}

void GmListItem::setRelativeGeometry(float minX, float minY, float minZ,
                                     float maxX, float maxY, float maxZ)
{
   GmInteractiveWidget::setRelativeGeometry(minX, minY, minZ,
                                            maxX, maxY, maxZ);
   mLabel->setRelativeGeometry(minX, minY, minZ, maxX, maxY, maxZ);
}

void GmListItem::setRelativeGeometry(float minX, float minY,
                                     float maxX, float maxY)
{
   setRelativeGeometry(minX, minY, 0.0, maxX, maxY, 0.0);
}

void GmListItem::layoutInGLVolume(float minX, float minY, float minZ,
                                  float width, float height, float depth)
{
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);
   mLabel->layoutInGLVolume(minX, minY, minZ, width, height, depth);
}

void GmListItem::getGLExtents(float& width, float& height)
{
   mLabel->getGLExtents(width, height);
}

void GmListItem::render2d(GmTypes::RenderOptionType renderOptions)
{
   // We might want to make this fancier in the future
   mLabel->render(renderOptions);
}

void GmListItem::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);
   
   glPushName(mObjectId);
   
   glColor4f(0.8, 0.5, 1.0, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   glPopName();
   
   glPopMatrix();

   // restore color settings
   glPopAttrib();

   //   mLabel->render(renderOptions);
}
