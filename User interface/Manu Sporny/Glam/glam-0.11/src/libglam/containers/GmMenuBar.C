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
#include <glam/GmMenuBar.h>
#include <glam/Gm2dLayout.h>

GmMenuBar::GmMenuBar()
{
   GmOptions* gmOptions = GmOptions::getInstance();
   GlmColor4* color = NULL;

   color = (GlmColor4*)gmOptions->getColorOption(GMO_MENUBAR_BASECOLOR);
  
   if(color != NULL)
      mBaseColor = *color;
}

GmMenuBar::~GmMenuBar()
{
}

void GmMenuBar::fireSelection(GmSelectionAction* action)
{
   vector<GmSelectionListener*>::iterator i;
   for(i = mSelectionListeners.begin(); i != mSelectionListeners.end(); i++)
      (*i)->selectionOccurred(action);
}

void GmMenuBar::fireDeselection(GmSelectionAction* action)
{
   vector<GmSelectionListener*>::iterator i;
   for(i = mSelectionListeners.begin(); i != mSelectionListeners.end(); i++)
      (*i)->deselectionOccurred(action);
}

void GmMenuBar::addSelectionListener(GmSelectionListener* sl)
{
   mSelectionListeners.push_back(sl);
}

bool GmMenuBar::removeSelectionListener(GmSelectionListener* sl)
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

void GmMenuBar::addMenu(GmMenu* menu, WidgetIDType widgetId)
{
   menu->setRelativeGeometry(0.1, 0.1, 0.9, 0.9);

   // add the menu
   mMenus.push_back(menu);

   // set the menu's widget id and listen to it
   menu->setWidgetId(widgetId);
   menu->addSelectionListener(this);
   menu->setFocusParent(this);
}

void GmMenuBar::keyPressEvent(GmEvent* event)
{
   bool handled = false;
   char key = event->getKeyData()->key;
   switch(key)
   {
      case '\t':
      {
         GmEventDistributor* ed = GmEventDistributor::getInstance();
         GmInteractiveWidget* focus = ed->getFocus();

         if(focus != NULL)
         {
            // if the focus is the menubar, set it to the first menu
            if(focus->getObjectId() == getObjectId())
            {
               if(!mMenus.empty() && mMenus.front()->getFocusable())
               {
                  mMenus.front()->setFocused(true);
                  mMenus.front()->openMenu();
                  handled = true;
                  break;
               }
            }

            // default behavior is to switch focus to the next
            // focusable component
            vector<GmMenu*>::iterator i = mMenus.begin();
            for(; i != mMenus.end(); i++)
            {
               GmMenu* menu = (GmMenu*)(*i);
               
               // set the focus to the next menu
               if(menu->getFocusOrder() == focus->getFocusOrder() + 1)
               {
                  menu->setFocused(true);
                  menu->openMenu();
                  handled = true;
                  break;
               }
            }
         }

         // if tab didn't pick any other widgets up, take focus
         if(!handled)
            setFocused(true);
      }
         break;
      default:
         break;
   }

   if(!handled)
      GmInteractiveWidget::keyPressEvent(event);
}

void GmMenuBar::selectionOccurred(GmSelectionAction* action)
{
   // close the unselected menus in the menu bar
   vector<GmMenu*>::iterator mi;
   for(mi = mMenus.begin(); mi != mMenus.end(); mi++)
   {
      if((*mi)->getWidgetId() != action->getSrcWidgetId())
      {
         // close the menu
         (*mi)->closeMenu();
      }
   }
   
   // pass this event up with new source, same selection
   GmSelectionAction* sa = new GmSelectionAction(this,
                                                 action->getSelection());
   fireSelection(sa);
   delete sa;
}

void GmMenuBar::deselectionOccurred(GmSelectionAction* action)
{
   // keep focus on menubar after a deselect
   // i.e. do nothing here
}

void GmMenuBar::focusLost(GmFocusAction* action)
{
   vector<GmMenu*>::iterator i = mMenus.begin();
   for(; i != mMenus.end(); i++)
   {
      if((*i)->getFocusState() == GmTypes::FT_NONE)
         (*i)->closeMenu();
   }

   GmInteractiveWidget::focusLost(action);
}

void GmMenuBar::layoutInGLVolume(float minX, float minY, float minZ,
                                 float width, float height, float depth)
{
   float x, y, z;
   float menuWidth, currentX;
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);
   
   // layout all the components
   mGLPosition.getXYZ(x, y, z);

   menuWidth = (mGLWidth) / mMenus.size();
   currentX = x;

   z += GLAM_LAYER * 100;

   for(vector<GmMenu*>::iterator mi = mMenus.begin();
       mi !=  mMenus.end(); mi++)
   {
      (*mi)->layoutInGLVolume(currentX, y, z, menuWidth, mGLHeight, mGLDepth);
      currentX += menuWidth;
   }
}

void GmMenuBar::render2d(GmTypes::RenderOptionType renderOptions)
{
   GmInteractiveWidget::render2d(renderOptions);
   
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   float r, g, b, a;

   mGLPosition.getXYZ(x, y, z);
   mBaseColor.getRGBA(r, g, b, a);

   glPushMatrix();

   glColor4f(r, g, b, a);
   glTranslatef(x, y, z);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   glPopMatrix();

   // restore color settings
   glPopAttrib();

   renderMenus(renderOptions);
}

void GmMenuBar::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);
   
   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);

   glColor4f(0.0, 0.0, 0.5, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   glPopName();

   glPopMatrix();

   // restore color settings
   glPopAttrib();

   // Render the menus
   renderMenus(renderOptions);
}

void GmMenuBar::renderMenus(GmTypes::RenderOptionType renderOptions)
{
   vector<GmMenu*>::iterator mi;
   for(mi = mMenus.begin(); mi !=  mMenus.end(); mi++)
      (*mi)->render(renderOptions);
}
