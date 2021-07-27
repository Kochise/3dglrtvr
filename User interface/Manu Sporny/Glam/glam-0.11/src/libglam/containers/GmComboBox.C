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
#include <glam/GmListItem.h>
#include <glam/GmComboBox.h>

GmComboBox::GmComboBox()
      : GmMenu()
{
   mCurrentSelection = NULL;
   setText("No Selection");
}

GmComboBox::~GmComboBox()
{
}

void GmComboBox::fireSelection(GmSelectionAction* action)
{
   GmMenu::fireSelection(action);

   // Set the displayed item for the combo box
   vector<GmInteractiveWidget*>::iterator li = mMenuItems.begin();
   while(li != mMenuItems.end())
   {
      if(((*li)->getWidgetId()) == action->getSelection())
      {
         mCurrentSelection = (*li);
         li = mMenuItems.end();
      }
      else
         li++;
   }
   
   setDisplayListValid(false);
}

void GmComboBox::getMenuExtents(float& width, float& height)
{
   mGLMenuWidth = mGLWidth;
   mGLMenuHeight = 0.0;
   
   float tempMenuHeight = 0.0;

   // calculate the menu width and menu height
   vector<GmInteractiveWidget*>::iterator mi;
   for(mi = mMenuItems.begin(); mi != mMenuItems.end(); mi++)
   {
      float tempWidth, tempHeight;
      (*mi)->getGLExtents(tempWidth, tempHeight);

      tempMenuHeight += tempHeight;
   }
   mGLMenuHeight = tempMenuHeight;

   width = mGLMenuWidth;
   height = mGLMenuHeight;

   //adjust height to allow for spacers
   mGLMenuHeight += mGLMenuHeight * 0.l;
}

void GmComboBox::renderMenuItems(GmTypes::RenderOptionType renderOptions)
{
   // render the menu items
   vector<GmInteractiveWidget*>::iterator li;
   for(li = mMenuItems.begin(); li != mMenuItems.end(); li++)
      (*li)->render(renderOptions);
}

void GmComboBox::getGLExtents(float& width, float& height)
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
   
   width = (maxWidth * 2.0);
   height = (maxHeight * 2.0);
}

void GmComboBox::setCurrentSelection(WidgetIDType widgetId)
{   
   // Calculate the widest component in the combo box
   for(vector<GmInteractiveWidget*>::iterator li = mMenuItems.begin();
       li != mMenuItems.end(); li++)
   {
      if((*li)->getWidgetId() == widgetId)
      {
         cout << "Checking widget " << (*li)->getWidgetId() << endl;
         mCurrentSelection = (*li);
      }
   }
}

WidgetIDType GmComboBox::getCurrentSelection()
{
   if(mCurrentSelection == NULL)
      return GLAM_DYNAMIC_WIDGETID_MAX;

   return mCurrentSelection->getWidgetId();
}

void GmComboBox::render2d(GmTypes::RenderOptionType renderOptions)
{
   render2dToolTip(renderOptions);
   
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   float r, g, b, a;
   mBaseColor.getRGBA(r, g, b, a);

   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   glPushMatrix();
   glTranslatef(x, y, z);
   
   // render the button of the menubutton
   glColor4f(r, g, b, a);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();
   
   // render the border around the button
   glColor4f(0, 0, 0, a);
   glBegin(GL_LINE_LOOP);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();
   
   // now render the menu if it is open
   if(isOpen())
   {
      // render the pull-down decorator
      glBegin(GL_LINES);
      glVertex2f(mGLWidth * 0.9, mGLHeight);
      glVertex2f(mGLWidth * 0.9, 0.0);
      glEnd();
      glBegin(GL_POLYGON);
      glVertex2f(mGLWidth * 0.92, mGLHeight * 0.3);
      glVertex2f(mGLWidth * 0.98, mGLHeight * 0.3);
      glVertex2f(mGLWidth * 0.95, mGLHeight * 0.7);
      glEnd();

      // a non-submenu rendering
      if(!mSubMenu)
      {
         glColor4f(r, g, b, a);
         glBegin(GL_POLYGON);
         glVertex2f(0.0, 0.0);
         glVertex2f(0.0, -mGLMenuHeight);
         glVertex2f(mGLMenuWidth, -mGLMenuHeight);
         glVertex2f(mGLMenuWidth, 0.0);
         glEnd();
         glColor4f(0.0, 0.0, 0.0, a);
         glBegin(GL_LINE_LOOP);
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
   else
   {
      // render the pull-down decorator
      glBegin(GL_LINES);
      glVertex2f(mGLWidth * 0.9, mGLHeight);
      glVertex2f(mGLWidth * 0.9, 0.0);
      glEnd();
      glBegin(GL_POLYGON);
      glVertex2f(mGLWidth * 0.92, mGLHeight * 0.7);
      glVertex2f(mGLWidth * 0.98, mGLHeight * 0.7);
      glVertex2f(mGLWidth * 0.95, mGLHeight * 0.3);
      glEnd();
   }

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
      glTranslatef(0.0, (y - cy) + (mGLHeight / 4.0), 0.0);
      mCurrentSelection->render(renderOptions);
      glPopMatrix();
   }
   else if(mLabel != NULL)
      mLabel->render(renderOptions);
}

void GmComboBox::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);
   
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);

   // render the menubutton rectangle
   glColor4f(0.5, 0.5, 1.0, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();
   
   glPopName();
   
   if(isOpen())
   {
      //background square
      glColor4f(1.0, 1.0, 1.0, 1.0);
      glBegin(GL_POLYGON);
      glVertex2f(0.0, 0.0);
      glVertex2f(0.0, -mGLMenuHeight);
      glVertex2f(mGLMenuWidth, -mGLMenuHeight);
      glVertex2f(mGLMenuWidth, 0.0);
      glEnd();
   }
 
   glPopMatrix();

   // restore color settings
   glPopAttrib();

   // Render the menu items
   if(isOpen())
      renderMenuItems(renderOptions);
}
