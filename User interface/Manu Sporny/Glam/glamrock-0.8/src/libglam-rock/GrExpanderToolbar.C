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
#include <values.h>
#include <glam/glam.h>
#include <glamrock/GrOptions.h>
#include <glamrock/GrShapeFactory.h>
#include <glamrock/GrExpanderToolbar.h>
#include <glamrock/GrPushButton.h>

GrExpanderToolbar::GrExpanderToolbar()
{
   GmOptions* gmOptions = GmOptions::getInstance();
   GlmColor4* baseColor = NULL;
   float borderWidth;
   
   baseColor =
      (GlmColor4*)gmOptions->getColorOption(GRO_EXPANDERTOOLBAR_BASECOLOR);
   borderWidth = gmOptions->getScalarOption(GRO_EXPANDERTOOLBAR_BORDERWIDTH);

   mWidgetState = GrTypes::WS_NORMAL;
   mDisplayListId = 0;
   mExpandWidth = 0.0;
   mBorderWidth = 0.75;

   if(baseColor != NULL)
      mBaseColor = *baseColor;
   
   if(borderWidth != MAXFLOAT)
      mBorderWidth = borderWidth;
}

GrExpanderToolbar::~GrExpanderToolbar()
{

   if(getDisplayListId() != GL_ZERO)
      glDeleteLists(getDisplayListId(), 1);
   
   setDisplayListId(GL_ZERO);
}

void GrExpanderToolbar::buttonPressed(GmButtonAction* action)
{
   GmSelectionAction* sa = new GmSelectionAction(this,
                                                 action->getSrcWidgetId());
   fireSelection(sa);
   delete action;
}

void GrExpanderToolbar::layoutInGLVolume(float minX, float minY, float minZ,
                                         float width, float height,
                                         float depth)
{
   // TEMP CODE: should probably use layout managers
   
   float x, y, z;
   float currX;
   vector<GmDrawable*>::iterator i;
   
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);

   mGLPosition.getXYZ(x, y, z);

   mFilletSize = mGLHeight * 0.25;

   mExpandWidth = mComponents.size() * mGLHeight;  // the expanded size
                     
   currX = x;
   z += GLAM_LAYER;
   for (i = mComponents.begin(); i != mComponents.end(); i++)
   {
      (*i)->layoutInGLVolume(currX, y, z, mGLHeight, mGLHeight, z);
      currX += mGLHeight;
   }
   setDisplayListValid(false);
}

void GrExpanderToolbar::fireSelection(GmSelectionAction* action)
{
   vector<GmSelectionListener*>::iterator i;
   for (i = mSelectionListeners.begin(); i != mSelectionListeners.end(); i++)
      (*i)->selectionOccurred(action);
}

void GrExpanderToolbar::fireDeselection(GmSelectionAction* action)
{
   vector<GmSelectionListener*>::iterator i;
   for (i = mSelectionListeners.begin(); i != mSelectionListeners.end(); i++)
      (*i)->deselectionOccurred(action);
}

void GrExpanderToolbar::addSelectionListener(GmSelectionListener* sl)
{
   mSelectionListeners.push_back(sl);
}

bool GrExpanderToolbar::removeSelectionListener(GmSelectionListener* sl)
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

void GrExpanderToolbar::mouseReleaseEvent(GmEvent* event)
{
   if(mWidgetState == GrTypes::WS_NORMAL)
      expand();
   else
      contract();
}

void GrExpanderToolbar::addButton(string buttonText,
                                  GmButtonListener* buttonListener,
                                  WidgetIDType token)
{
   addButton("", "", buttonText, buttonListener, token);
}

void GrExpanderToolbar::addButton(string pressedGraphic,
                                  string unpressedGraphic,
                                  GmButtonListener* buttonListener,
                                  WidgetIDType token)
{
   addButton(pressedGraphic, unpressedGraphic,
             NULL, buttonListener, token);
}

void GrExpanderToolbar::addButton(string pressedGraphic,
                                  string unpressedGraphic,
                                  string buttonText,
                                  GmButtonListener* buttonListener,
                                  WidgetIDType token)
{
   float r, g, b, a;   
   GrPushButton* newButton;

   mBaseColor.getRGBA(r, g, b, a);
   
   newButton = new GrPushButton();
   newButton->setBaseColor(r+0.2, g+0.2, b+0.2, a);
   newButton->setTextColor(r-0.2, g-0.2, b-0.2, a);
   newButton->setTextSize(12.0);
   newButton->setBorderWidth(0.1);
   newButton->setWidgetId(token);
   
   if(buttonListener != NULL)
      newButton->addButtonListener(buttonListener);
   
   if(buttonText != "")
      newButton->setText(buttonText);

   GmFileCache* fc = &GmFileCache::getInstance();

   if(pressedGraphic != "")
      newButton->setPressedIcon(fc->fetchIcon(pressedGraphic));
   
   if(unpressedGraphic != "")
      newButton->setUnpressedIcon(fc->fetchIcon(unpressedGraphic));

   if(pressedGraphic != "" || unpressedGraphic != "")
      newButton->setTextJustification(GmTypes::JTH_CENTER,
                                      GmTypes::JTV_BOTTOM);
   
   newButton->setRelativeGeometry(0.1, 0.1, 0.9, 0.9);
   addComponent(newButton);
}

void GrExpanderToolbar::contract()
{
   mWidgetState = GrTypes::WS_NORMAL;
   setDisplayListValid(false);
}

void GrExpanderToolbar::expand()
{
   mWidgetState = GrTypes::WS_EXPANDED;
   setDisplayListValid(false);
}

void GrExpanderToolbar::render2d(GmTypes::RenderOptionType renderOptions)
{
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
      
      float width;
      if(mWidgetState == GrTypes::WS_NORMAL)
         width = 0.0;
      else
         width = mExpandWidth;

      // Draw main body of toolbar
      glColor4f(r, g, b, a);
      sf->sfBegin(GrTypes::FS_SOLID);
      sf->sfFilletSize(0.0);
      sf->sfVertex2f(0.0, 0.0);
      sf->sfFilletSize(mFilletSize);
      sf->sfVertex2f(width + mFilletSize + (mBorderWidth * 2.0), 0.0);
      sf->sfVertex2f(width + mFilletSize + (mBorderWidth * 2.0), mGLHeight);
      sf->sfFilletSize(0.0);
      sf->sfVertex2f(0.0, mGLHeight);
      sf->sfEnd();

      glColor4f(r*0.5, g*0.5, b*0.5, a);
      sf->sfBegin(GrTypes::FS_OUTLINE);
      sf->sfFilletSize(0.0);
      sf->sfVertex2f(0.0, mBorderWidth);
      sf->sfFilletSize(mFilletSize);
      sf->sfVertex2f(width + mFilletSize + mBorderWidth, mBorderWidth);
      sf->sfVertex2f(width + mFilletSize + mBorderWidth,
                     mGLHeight - mBorderWidth);
      sf->sfFilletSize(0.0);
      sf->sfVertex2f(0.0, mGLHeight - mBorderWidth);
      sf->sfEnd();

      // Draw button on end of toolbar
      glColor4f(r+0.2, g+0.2, b+0.2, a);
      sf->sfBegin(GrTypes::FS_SOLID);
      sf->sfFilletSize(0.0);
      sf->sfVertex2f(width, (mBorderWidth * 2.0));
      sf->sfFilletSize(mFilletSize);
      sf->sfVertex2f(width + mFilletSize, (mBorderWidth * 2.0));
      sf->sfVertex2f(width + mFilletSize, mGLHeight - (mBorderWidth * 2.0));
      sf->sfFilletSize(0.0);
      sf->sfVertex2f(width, mGLHeight - (mBorderWidth * 2.0));
      sf->sfEnd();

      glColor4f(r*0.5, g*0.5, b*0.5, a);
      sf->sfBegin(GrTypes::FS_OUTLINE);
      sf->sfFilletSize(0.0);
      sf->sfVertex2f(width, (mBorderWidth * 2.0));
      sf->sfFilletSize(mFilletSize);
      sf->sfVertex2f(width + mFilletSize, (mBorderWidth * 2.0));
      sf->sfVertex2f(width + mFilletSize, mGLHeight - (mBorderWidth * 2.0));
      sf->sfFilletSize(0.0);
      sf->sfVertex2f(width, mGLHeight - (mBorderWidth * 2.0));   sf->sfEnd();

      // Draw arrow on button
      if(mWidgetState == GrTypes::WS_NORMAL)
      {
         glColor4f(r*1.5, g*1.5, b*1.5, a);
         sf->sfBegin(GrTypes::FS_SOLID);
         sf->sfFilletSize(mFilletSize * 0.2);
         sf->sfVertex2f(width + mBorderWidth * 2.0,
                        (mGLHeight * 0.5) - (mGLHeight * 0.2));
         sf->sfVertex2f(width + mFilletSize - (mBorderWidth * 2.0),
                        (mGLHeight * 0.5));
         sf->sfVertex2f(width + mBorderWidth * 2.0,
                        (mGLHeight * 0.5) + (mGLHeight * 0.2));
         sf->sfEnd();

         glColor4f(r*0.5, g*0.5, b*0.5, a);   
         sf->sfBegin(GrTypes::FS_OUTLINE);
         sf->sfFilletSize(mFilletSize * 0.2);
         sf->sfVertex2f(width + mBorderWidth * 2.0,
                        (mGLHeight * 0.5) - (mGLHeight * 0.2));
         sf->sfVertex2f(width + mFilletSize - (mBorderWidth * 2.0),
                        (mGLHeight * 0.5));
         sf->sfVertex2f(width + mBorderWidth * 2.0,
                        (mGLHeight * 0.5) + (mGLHeight * 0.2));
         sf->sfEnd();
      }
      else
      {
         glColor4f(r*1.5, g*1.5, b*1.5, a);
         sf->sfBegin(GrTypes::FS_SOLID);
         sf->sfFilletSize(mFilletSize * 0.2);
         sf->sfVertex2f(width + mFilletSize - mBorderWidth * 2.0,
                        mGLHeight * 0.5 - mGLHeight * 0.2);
         sf->sfVertex2f(width + mBorderWidth * 2.0, mGLHeight * 0.5);
         sf->sfVertex2f(width + mFilletSize - mBorderWidth * 2.0,
                        mGLHeight * 0.5 + mGLHeight * 0.2);
         sf->sfEnd();

         glColor4f(r*0.5, g*0.5, b*0.5, a);   
         sf->sfBegin(GrTypes::FS_OUTLINE);
         sf->sfFilletSize(mFilletSize * 0.2);
         sf->sfVertex2f(width + mFilletSize - mBorderWidth * 2.0,
                        mGLHeight * 0.5 - mGLHeight * 0.2);
         sf->sfVertex2f(width + mBorderWidth * 2.0, mGLHeight * 0.5);
         sf->sfVertex2f(width + mFilletSize - mBorderWidth * 2.0,
                        mGLHeight * 0.5 + mGLHeight * 0.2);
         sf->sfEnd();
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
  
   if(mWidgetState == GrTypes::WS_EXPANDED)
     GmContainer::render2d(renderOptions);
}

void GrExpanderToolbar::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float width;
   if(mWidgetState == GrTypes::WS_NORMAL)
      width = 0.0;
   else
      width = mExpandWidth;

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   // Center section of panel
   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);

   glColor4f(0.4, 0.4, 0.4, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(width, 0.0);
   glVertex2f(width + mFilletSize, 0.0);
   glVertex2f(width + mFilletSize, mGLHeight);
   glVertex2f(width, mGLHeight);
   glEnd();

   glPopName();
   
   glPopMatrix();

   // restore color settings
   glPopAttrib();

   if(mWidgetState == GrTypes::WS_EXPANDED)
      GmContainer::render2dSelect(renderOptions);
}
