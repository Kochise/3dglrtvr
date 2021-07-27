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
#include <glam/GmMenu.h>
#include <glam/Gm2dLayout.h>
#include <glam/GmEventDistributor.h>

#define SPACER_PERCENT 0.2

GmMenu::GmMenu()
      : GmButton()
{
   GmOptions* gmOptions = GmOptions::getInstance();
   GlmColor4* tColor = NULL;
   GlmColor4* bColor = NULL;
   
   bColor = (GlmColor4*)gmOptions->getColorOption(GMO_MENU_BASECOLOR);
   tColor = (GlmColor4*)gmOptions->getColorOption(GMO_MENU_TEXTCOLOR);

   if(tColor != NULL)
   {
      if(mLabel == NULL)
         mLabel = new GmLabel();
      
      mLabel->setTextColor(tColor);
   }
   
   if(bColor != NULL)
      mBaseColor = *bColor;
   
   mGLMenuWidth = 0.0;
   mGLMenuHeight = 0.0;
   mGLMenuDepth = 0.0;
   mSubMenu = false;
}

GmMenu::~GmMenu()
{
}

void GmMenu::fireSelection(GmSelectionAction* action)
{
   vector<GmSelectionListener*>::iterator i;
   for(i = mSelectionListeners.begin(); i != mSelectionListeners.end(); i++)
      (*i)->selectionOccurred(action);
}

void GmMenu::fireDeselection(GmSelectionAction* action)
{
   vector<GmSelectionListener*>::iterator i;
   for(i = mSelectionListeners.begin(); i != mSelectionListeners.end(); i++)
      (*i)->deselectionOccurred(action);
}

void GmMenu::addSelectionListener(GmSelectionListener* sl)
{
   mSelectionListeners.push_back(sl);
}

bool GmMenu::removeSelectionListener(GmSelectionListener* sl)
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

void GmMenu::addTextItem(string textMenuItem, WidgetIDType widgetTokenId)
{
   // create a new list item
   GmListItem* li = new GmListItem();
   li->setText(textMenuItem);
   li->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);

   // add the list item to the list
   mMenuItems.push_back(li);

   // set the widget's id and listen to it
   li->setWidgetId(widgetTokenId);
   li->addSelectionListener(this);
   li->setFocusParent(this);
   li->setFocusOrder(mMenuItems.size() + 1);

   float r, g, b, a;
   if(mLabel != NULL)
   {
      mLabel->getTextColor()->getRGBA(r, g, b, a);
      li->setTextColor(r, g, b, a);
   }

   mBaseColor.getRGBA(r, g, b, a);
   li->setBaseColor(r, g, b, a);

   // reset geometry of all components
   float x, y, z;
   mPosition.getXYZ(x, y, z);
   
   setRelativeGeometry(x, y, z,
                       x + getWidth(), y + getHeight(), z + getDepth());
}

void GmMenu::addSubMenu(GmMenu* subMenu, WidgetIDType widgetId)
{
   // set submenu state to true for our new submenu
   subMenu->setSubMenuState(true);
   
   // add the menu to both the list and menu items
   mMenuItems.push_back(subMenu);
   mSubMenus.push_back(subMenu);

   // set the menu's widget id and listen to it
   subMenu->setWidgetId(widgetId);
   subMenu->addSelectionListener(this);
   subMenu->setFocusParent(this);
   subMenu->setFocusOrder(mMenuItems.size() + 1);

   // reset geometry of all components
   float x, y, z;
   mPosition.getXYZ(x, y, z);

   setRelativeGeometry(x, y, z,
                       x + getWidth(), y + getHeight(), z + getDepth());
}

void GmMenu::setSubMenuState(bool isSubMenu)
{
   // sets submenu status of this menu to true/false
   mSubMenu = isSubMenu;

   // set text justification left if submenu is true, center if false
   if(mSubMenu)
   {
      mLabel->setTextJustification(GmTypes::JTH_LEFT,
                                   GmTypes::JTV_MIDDLE);
   }
   else
   {
      mLabel->setTextJustification(GmTypes::JTH_CENTER,
                                   GmTypes::JTV_MIDDLE);
   }
}

void GmMenu::openMenu()
{
   if(mMenuItems.size() != 0)
      setState(GmTypes::BS_DOWN);
}

void GmMenu::closeMenu()
{
   // if the menu is already closed, return
   if(!isOpen())
      return;

   // recursively close all child submenus
   vector<GmMenu*>::iterator mi;
   for(mi = mSubMenus.begin(); mi != mSubMenus.end(); mi++)
      (*mi)->closeMenu();

   // close self
   setState(GmTypes::BS_UP);
}

bool GmMenu::isOpen()
{
   return mState == GmTypes::BS_DOWN;
}

void GmMenu::mousePressEvent(GmEvent* event)
{
   GmInteractiveWidget::mousePressEvent(event);
   
   // create a selection action to pass to listeners
   GmSelectionAction* sa = new GmSelectionAction(this, this);

   // if the menu is already open
   if(isOpen())
   {
      // close the menu, inform listeners of deselection
      closeMenu();
      fireDeselection(sa);
   }
   else
   {
      // open the menu, inform listeners of selection
      openMenu();
      fireSelection(sa);
   }

   // delete the action
   delete sa;
}

void GmMenu::mouseReleaseEvent(GmEvent* event)
{
   GmInteractiveWidget::mouseReleaseEvent(event);
}

void GmMenu::keyPressEvent(GmEvent* event)
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
            // if the focus is this menu, set it to the first item
            if(focus->getObjectId() == getObjectId())
            {
               if(!mMenuItems.empty() &&
                  mMenuItems.front()->getFocusable())
               {
                  mMenuItems.front()->setFocused(true);
                  focus = mMenuItems.front();
                  handled = true;
               }
            }

            // switch focus to the next focusable item
            vector<GmInteractiveWidget*>::iterator i = mMenuItems.begin();
            for(; !handled && i != mMenuItems.end(); i++)
            {
               // set the focus to the next item
               if((*i)->getFocusOrder() == focus->getFocusOrder() + 1)
               {
                  (*i)->setFocused(true);
                  focus = (*i);
                  handled = true;
               }
            }

            if(focus != NULL)
            {
               // open the new focus if it is a submenu
               vector<GmMenu*>::iterator im = mSubMenus.begin();
               for(; im != mSubMenus.end(); im++)
               {
                  if(focus->getObjectId() == (*im)->getObjectId())
                     (*im)->openMenu();
               }
            }
         }

         // if tab didn't pick any other widgets up, take focus
         if(!handled)
            setFocused(true);
      }
         break;
      case ' ':
      {
         GmEventDistributor* ed = GmEventDistributor::getInstance();
         GmInteractiveWidget* focus = ed->getFocus();

         if(focus != NULL)
         {
            // create a selection action to pass to listeners
            GmSelectionAction* sa =
               new GmSelectionAction(this, focus);

            // if the focus was on the menu
            if(focus->getObjectId() == getObjectId())
            {
               // if the menu is already open
               if(isOpen())
               {
                  // close the menu, inform listeners of deselection
                  closeMenu();
                  fireDeselection(sa);
               }
               else
               {
                  // open the menu, inform listeners of selection
                  openMenu();
                  fireSelection(sa);
               }
            }
            else
               fireSelection(sa);

            // delete the action
            delete sa;
         }

         handled = true;
      }
         break;
      default:
         break;
   }

   if(!handled)
      GmInteractiveWidget::keyPressEvent(event);
}

void GmMenu::selectionOccurred(GmSelectionAction* action)
{
   GmMenu* subMenu = NULL;
   
   // close all submenus that aren't the source of selection
   vector<GmMenu*>::iterator mi;
   for(mi = mSubMenus.begin(); mi != mSubMenus.end(); mi++)
   {
      if((*mi)->getWidgetId() != action->getSrcWidgetId())
         (*mi)->closeMenu();
      else
      {
         // if it was a submenu, check to see if its closed
         // if it was selected and is closed, close all menus
         if(!(*mi)->isOpen())
         {

            closeMenu();
            break;
         }

         subMenu = *mi;
      }
   }

   // if a submenu was not picked, close menu, lose focus
   if(!subMenu)
   {
      closeMenu();
      setFocused(false);
   }

   // pass this event up with new source, same selection
   GmSelectionAction* sa = new GmSelectionAction(this,
                                                 action->getSelection());
   fireSelection(sa);
   delete sa;
}

void GmMenu::deselectionOccurred(GmSelectionAction* action)
{
}

void GmMenu::setRelativeGeometry(float minX, float minY, float minZ,
                                 float maxX, float maxY, float maxZ)
{
   GmButton::setRelativeGeometry(minX, minY, minZ, maxX, maxY, maxZ);

   // set relative geometry of menu components
   float topY = 1.0, botY = 1.0;
   int numItems = mMenuItems.size();
   int numSpacers = numItems+1;

   //the percentage each spacer is
   float spacer = SPACER_PERCENT / numSpacers;
   
   //the percentage of space each menu item should take
   float inc = 0.0;

   float menuWidth, menuHeight;
   getMenuExtents(menuWidth, menuHeight);
   //width and height of the current list item to see relative geometry
   float liWidth, liHeight;

   vector<GmInteractiveWidget*>::iterator i;
   for(i = mMenuItems.begin(); i != mMenuItems.end(); i++)
   {
      (*i)->getGLExtents(liWidth, liHeight);
      inc = (liHeight / menuHeight) * (1.0 - SPACER_PERCENT);
      
      botY -= inc + spacer;
      
      (*i)->setRelativeGeometry(0.05, botY, 0.95, topY - spacer);

      topY = botY;
   }
}

void GmMenu::setRelativeGeometry(float minX, float minY,
                                 float maxX, float maxY)
{
   setRelativeGeometry(minX, minY, 0.0, maxX, maxY, 0.0);
}

void GmMenu::layoutInGLVolume(float minX, float minY, float minZ,
                              float width, float height, float depth)
{
   GmButton::layoutInGLVolume(minX, minY, minZ, width, height, depth);

   float tempWidth, tempHeight;
   getMenuExtents(tempWidth, tempHeight);
   
   // layout all the components in the menu box
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   vector<GmInteractiveWidget*>::iterator mi;
   if(!mSubMenu)
   {
      for(mi = mMenuItems.begin(); mi != mMenuItems.end(); mi++)
         (*mi)->layoutInGLVolume(x, y - mGLMenuHeight,
                                 z + GLAM_LAYER * 100.0,
                                 mGLMenuWidth, mGLMenuHeight, mGLMenuDepth);
   }
   else if(mSubMenu)
   {
      for(mi = mMenuItems.begin(); mi != mMenuItems.end(); mi++)
         (*mi)->layoutInGLVolume(x + mGLWidth,
                                 y + mGLHeight - mGLMenuHeight,
                                 z + GLAM_LAYER * 100.0,
                                 mGLMenuWidth, mGLMenuHeight, mGLMenuDepth);
   }
}

void GmMenu::getGLExtents(float& width, float& height)
{
   GmButton::getGLExtents(width, height);
}

void GmMenu::getMenuExtents(float& width, float& height)
{
   mGLMenuWidth = 0.0;
   mGLMenuHeight = 0.0;
   
   float tempMenuHeight = 0.0;

   // calculate the menu width and menu height
   vector<GmInteractiveWidget*>::iterator mi;
   for(mi = mMenuItems.begin(); mi != mMenuItems.end(); mi++)
   {
      float tempWidth, tempHeight;
      (*mi)->getGLExtents(tempWidth, tempHeight);

      if(tempWidth > mGLMenuWidth)
         mGLMenuWidth = tempWidth;

      tempMenuHeight += tempHeight;

      if(tempMenuHeight > mGLMenuHeight)
         mGLMenuHeight = tempMenuHeight;
   }

   width = mGLMenuWidth;
   height = mGLMenuHeight;
   
   //int numItems = mMenuItems.size();

   //adjust height to allow for spacers
   mGLMenuHeight += mGLMenuHeight * 2 * SPACER_PERCENT;
   //mGLMenuHeight += (numItems + 1) * 1.0;

   //adjust width to account for relative geometry padding
   mGLMenuWidth += mGLMenuWidth * 2 * SPACER_PERCENT;

   if(mSubMenu)
      if(mGLMenuHeight < mGLHeight)
         mGLMenuHeight = mGLHeight;

   if(!mSubMenu)
      if(mGLMenuWidth < mGLWidth)
         mGLMenuWidth = mGLWidth;
}

void GmMenu::render2d(GmTypes::RenderOptionType renderOptions)
{
   render2dToolTip(renderOptions);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   float r, g, b, a;
   mBaseColor.getRGBA(r, g, b, a);

   glPushAttrib(GL_CURRENT_BIT);

   glPushMatrix();
   
   // render the button of the menubutton
   glColor4f(r, g, b, a);
   glTranslatef(x, y, z);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   // render the menu if it is open
   if(isOpen())
   {
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
   
   // pop off the matrix
   glPopMatrix();

   glPopAttrib();

   // render all of the menu items
   if(isOpen())
      renderMenuItems(renderOptions);

   // render the title text
   if(mLabel != NULL)
      mLabel->render(renderOptions);
}

void GmMenu::render2dSelect(GmTypes::RenderOptionType renderOptions)
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

   // render the title text
   if(mLabel != NULL)
      mLabel->render(renderOptions);
}

void GmMenu::renderMenuItems(GmTypes::RenderOptionType renderOptions)
{
   // render the menu items
   vector<GmInteractiveWidget*>::iterator mi;
   for(mi = mMenuItems.begin(); mi != mMenuItems.end(); mi++)
      (*mi)->render(renderOptions);
}
