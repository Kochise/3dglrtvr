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
#include <glam/GmOptions.h>
#include <glam/GmCheckBox.h>

GmCheckBox::GmCheckBox()
      : GmButton()
{
   setSelected(false);
   
   if(mLabel != NULL)
      mLabel->setTextJustification(GmTypes::JTH_LEFT,
                                   GmTypes::JTV_MIDDLE);
   
   GmOptions* gmOptions = GmOptions::getInstance();

   mBaseColor = *(gmOptions->getColorOption(GMO_CHECKBOX_BASECOLOR));
   mCheckColor = *(gmOptions->getColorOption(GMO_CHECKBOX_CHECKCOLOR));
}

GmCheckBox::~GmCheckBox()
{
}

void GmCheckBox::fireSelection(GmSelectionAction* action)
{
   vector<GmSelectionListener*>::iterator i;
   for(i = mSelectionListeners.begin(); i != mSelectionListeners.end(); i++)
      (*i)->selectionOccurred(action);
}

void GmCheckBox::fireDeselection(GmSelectionAction* action)
{
   vector<GmSelectionListener*>::iterator i;
   for(i = mSelectionListeners.begin(); i != mSelectionListeners.end(); i++)
      (*i)->deselectionOccurred(action);
}

void GmCheckBox::addSelectionListener(GmSelectionListener* sl)
{
   mSelectionListeners.push_back(sl);
}

bool GmCheckBox::removeSelectionListener(GmSelectionListener* sl)
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

void GmCheckBox::setCheckColor(float r, float g, float b, float a)
{
   mCheckColor.setRGBA(r, g, b, a);
}

void GmCheckBox::setText(string text)
{
   if(mLabel != NULL)
      mLabel->setText(text);
   else
   {
      mLabel = new GmLabel();
      mLabel->setText(text);
      mLabel->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   }

   setDisplayListValid(false);
}

void GmCheckBox::mouseReleaseEvent(GmEvent* event)
{
   // release button press
   mPressed = false;

   // if the button is down, fire a button pressed event
   if(mState == GmTypes::BS_DOWN)
   {
      // regenerate the display list to display the check (or not)
      setDisplayListValid(false);

      // toggle the selection
      mSelected = !mSelected;

      // fire off selection event
      GmSelectionAction* sa = new GmSelectionAction(this, this);

      if (mSelected)
         fireSelection(sa);
      else
         fireDeselection(sa);

      delete sa;

      GmButtonAction* ba = new GmButtonAction(this, mState);

      // set the button's state
      setState(GmTypes::BS_UP);

      fireButtonPressed(ba);
      delete ba;
   }
}

void GmCheckBox::setSelected(bool selected)
{
   mSelected = selected;
   setDisplayListValid(false);
}

bool GmCheckBox::getSelected()
{
   return mSelected;
}

void GmCheckBox::setRelativeGeometry(float minX, float minY, float minZ,
                                     float maxX, float maxY, float maxZ)
{
   GmDrawable::setRelativeGeometry(minX, minY, minZ,
                                   maxX, maxY, maxZ);

   if(mLabel != NULL)
      mLabel->setRelativeGeometry(minX, minY, minZ, maxX, maxY, maxZ);
   
   setDisplayListValid(false);
}

void GmCheckBox::setRelativeGeometry(float minX, float minY,
                                     float maxX, float maxY)
{
   setRelativeGeometry(minX, minY, 0.0, maxX, maxY, 0.0);
}

void GmCheckBox::layoutInGLVolume(float minX, float minY, float minZ,
                                  float width, float height, float depth)
{
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);

   if(mLabel != NULL)
   {
      float x, y, z;
      mGLPosition.getXYZ(x, y, z);
      
      float textWidth, textHeight;
      mLabel->getGLExtents(textWidth, textHeight);
      mLabel->layoutInGLVolume(x, y, z, textWidth, mGLHeight, mGLDepth);
   }
}

void GmCheckBox::render2d(GmTypes::RenderOptionType renderOptions)
{
   render2dToolTip(renderOptions);
   
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;

   if(!getDisplayListValid())
   {
      if(getDisplayListId() != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);

      float r, g, b, a;
      mBaseColor.getRGBA(r, g, b, a);
      
      float bWidth = mGLHeight * (mBorderWidth / 2.0);
      float bHeight = mGLHeight * (mBorderWidth / 2.0);
   
      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);
      
      // Left border
      glColor4f(0, 0, 0, a);
      glBegin(GL_POLYGON);
      glVertex2f(0.0, 0.0);
      glVertex2f(bWidth, bWidth);
      glVertex2f(bWidth, mGLHeight - bHeight);
      glVertex2f(0.0, mGLHeight);
      glEnd();
   
      // Top border
      glColor4f(0, 0, 0, a);
      glBegin(GL_POLYGON);
      glVertex2f(0.0, mGLHeight);
      glVertex2f(mGLHeight, mGLHeight);
      glVertex2f(mGLHeight - bWidth, mGLHeight - bHeight);
      glVertex2f(bWidth, mGLHeight - bHeight);
      glEnd();
      
      // Bottom border
      glColor4f(1.0, 1.0, 1.0, a);
      glBegin(GL_POLYGON);
      glVertex2f(0.0, 0.0);
      glVertex2f(mGLHeight, 0.0);
      glVertex2f(mGLHeight - bWidth, bHeight);
      glVertex2f(bWidth, bHeight);
      glEnd();
      
      // Right border
      glColor4f(1.0, 1.0, 1.0, a);
      glBegin(GL_POLYGON);
      glVertex2f(mGLHeight, 0.0);
      glVertex2f(mGLHeight, mGLHeight);
      glVertex2f(mGLHeight - bWidth, mGLHeight - bHeight);
      glVertex2f(mGLHeight - bWidth, bHeight);
      glEnd();
      
      // the "check"
      if(mSelected)
      {
         glColor4f(r, g, b, a);
         glLineWidth(bWidth);
         glBegin(GL_LINES);
         glVertex2f(bWidth + .1, bHeight + .1);
         glVertex2f(mGLHeight - bWidth - .1, mGLHeight - bHeight - .1);
         glVertex2f(bWidth + .1, mGLHeight - bHeight - .1);
         glVertex2f(mGLHeight - bWidth - .1, bHeight + .1);
         glEnd();
         glLineWidth(1.0);
      }
      
      glEndList();
      setDisplayListValid(true);

      // set up text widget
      if(mLabel != NULL)
      {
         mGLPosition.getXYZ(x, y, z);
         mLabel->setGLPosition(x + mGLHeight + mLabel->getTextSize() / 4, y, z);
      }
   }

   // Draw the checkbox
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);
   glCallList(getDisplayListId());
   glPopMatrix();

   // restore color settings
   glPopAttrib();

   // The text label (if any)
   if(mLabel != NULL)
      mLabel->render(renderOptions);
}

void GmCheckBox::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);
   
   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);
   
   glColor4f(1.0, 1.0, 0.0, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLHeight, 0.0);
   glVertex2f(mGLHeight, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();
   
   glPopName();

   glPopMatrix();

   // restore color settings
   glPopAttrib();

   // render the label
   if(mLabel != NULL)
      mLabel->render(renderOptions);
}
