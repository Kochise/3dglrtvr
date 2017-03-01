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
#ifndef GMMENU_H
#define GMMENU_H

#include <glam/GmSelectionListener.h>
#include <glam/GmInteractiveWidget.h>
#include <glam/GmButton.h>

/**
 * The menu class defines the functionality of all menu objects such
 * as pop-ups and pull-downs. 
 */
class GmMenu : public GmButton,
               public GmSelectionListener
{
 protected:
   /**
    * The vector of listeners that listen to this component.
    */
   vector<GmSelectionListener*> mSelectionListeners;
   
   /**
    * The list of menu item objects that are contained in this menu
    */
   vector<GmInteractiveWidget*> mMenuItems;

   /**
    * The list of submenu objects that are contained in this menu
    */
   vector<GmMenu*> mSubMenus;

   /**
    * The OpenGL width of the drop-down menu
    */
   float mGLMenuWidth;

   /**
    * The OpenGL height of the drop-down menu
    */
   float mGLMenuHeight;

   /**
    * The OpenGL depth of the drop-down menu
    */
   float mGLMenuDepth;

   /**
    * The submenu flag, set by a parent menu.
    * This flag means that this instantiation of the GmMenu object is
    * a submenu to another GmMenu. Set automatically by the menu system.
    */
   bool mSubMenu;

   /**
    * Fires the appropriate selection action to all listeners.
    *
    * @param action the action to pass to the listeners
    */
   virtual void fireSelection(GmSelectionAction* action);

   /**
    * Fires the appropriate selection action to all listeners.
    *
    * @param action the action to pass to the listeners
    */
   virtual void fireDeselection(GmSelectionAction* action);

 public:
   /**
    * Standard constructor
    */
   GmMenu();
   
   /**
    * Virtual destructor
    */
   virtual ~GmMenu();
   
   /**
    * Adds a selection listener to this menu.
    *
    * @param sl a selection listener that will listen to this menu.
    */
   virtual void addSelectionListener(GmSelectionListener* sl);

   /**
    * Removes a selection listener from this menu.
    *
    * @param sl the selection listener to remove.
    * @return true if the listener was removed, false if not.
    */
   virtual bool removeSelectionListener(GmSelectionListener* sl);

   /**
    * Adds a text-based menu item to the menu.
    *
    * @param textMenuItem the text to use for the menu item.
    * @param widgetTokenId the token id pneumonic to use when identifying
    *                      the widget. The widget token ID is
    *                      necessary for the widget event handler to
    *                      uniquely identify the menu item when
    *                      responding to a widget event.
    */
   virtual void addTextItem(string textMenuItem, WidgetIDType widgetId);

   /**
    * Adds a submenu to the menu.
    *
    * @param menu the submenu to add to the menu
    * @param widgetId the identifier token for the menu
    *
    */
   virtual void addSubMenu(GmMenu* subMenu, WidgetIDType widgetId);

   /**
    * Opens a menu at the top level.
    */
   virtual void openMenu();
   
   /**
    * Recursively closes all childmenus and this menu itself.
    */
   virtual void closeMenu();

   /**
    * Determines if a menu is open.
    *
    * @return true if the menu is open, false if it is closed
    */
   virtual bool isOpen();

   /**
    * Event processing function to process a mouse press event.
    *
    * @param event the mouse press event for this object to process
    */
   virtual void mousePressEvent(GmEvent* event);

   /**
    * Event processing function to process a mouse release event.
    *
    * @param event the mouse release event for this object to process
    */
   virtual void mouseReleaseEvent(GmEvent* event);
   
   /**
    * Event processing function to process a key press event.
    */
   virtual void keyPressEvent(GmEvent* event);
   
   /**
    * Called when a selection is made. Either this menu or one of
    * its items was selected.
    *
    * @param action the action associated with the selection.
    */
   virtual void selectionOccurred(GmSelectionAction* action);

   /**
    * Called when a deselection occurred.
    *
    * @param action the action associated with the deselection.
    */
   virtual void deselectionOccurred(GmSelectionAction* action);

   /**
    * Sets the relative geometry to use for this drawable object. This
    * method is provided merely for convenience.
    *
    * @param minX the relative min X value for this drawable
    * @param minY the relative min Y value for this drawable
    * @param minZ the relative min Z value for this drawable
    * @param maxX the relative max X of this drawable
    * @param maxY the relative max Y of this drawable
    * @param maxZ the relative max Z of this drawable
    */
   virtual void setRelativeGeometry(float minX, float minY, float minZ,
                                    float maxX, float maxY, float maxZ);

   /**
    * Sets the relative geometry to use for this drawable object. This
    * method is provided merely for convenience. This method should be
    * used for 2d objects that will be in a container that uses a
    * layout manager.
    *
    * @param minX the relative min X value for this drawable
    * @param minY the relative min Y value for this drawable
    * @param maxX the relative max X of this drawable
    * @param maxY the relative max Y of this drawable
    */
   virtual void setRelativeGeometry(float minX, float minY,
                                    float maxX, float maxY);

   /**
    * Repositions and resizes this component based on parameters
    * passed into the method. These parameters are usually passed in
    * by the parent class.
    *
    * @param minX the starting X position of the constraint volume
    * @param minY the starting Y position of the constraint volume
    * @param minZ the starting Z position of the constraint volume
    * @param width the width of the constraint volume
    * @param height the height of the constraint volume
    * @param depth the depth of the constraint volume
    */
   virtual void layoutInGLVolume(float minX, float minY, float minZ,
                                 float width, float height, float depth);
   
   /**
    * Gets the ideal extents of the drawable in OpenGL units. This is
    * sometimes used by parent containers to determine the optimal
    * layout strategy by taking each components extents into account.
    *
    * @param width the width of the drawable in OpenGL units
    * @param height the height of the drawable in OpenGL units
    */
   virtual void getGLExtents(float& width, float& height);

   /**
    * Gets the menu ideal extents of all list items in the menu.
    * IMPORTANT NOTE: This function returns the exact gl sizes of the
    * menu items, whereas getting the menu width and height from the
    * member variables mGLMenuWidth/Height will be different because
    * those variables also take into account spacing between menu items.
    *
    * @param width the width of the drawable in OpenGL units
    * @param height the height of the drawable in OpenGL units
    */
   virtual void getMenuExtents(float& width, float& height);
   
 protected:

   /**
    * Parent menu calls this function to set its childmenu mSubMenu
    * flag.
    * @param isSubMenu the boolean true/false to tell the menu if it
    * is a submenu. Should never be called outside of the class, since
    * using the addSubMenu function will set this state for you.
    */
   virtual void setSubMenuState(bool isSubMenu);

   /**
    * Renders this menu to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the menu.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this menu to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the menu.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this menu's menu items.
    *
    * @param renderOptions the render options to use when rendering
    *                      the items.
    */
   virtual void renderMenuItems(GmTypes::RenderOptionType renderOptions);
};

#endif
