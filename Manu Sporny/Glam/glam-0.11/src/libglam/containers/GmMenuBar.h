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
#ifndef GMMENUBAR_H
#define GMMENUBAR_H

#include <glam/GmTypes.h>
#include <glam/GmMenu.h>
#include <glam/GmContainer.h>
#include <glam/GmSelectionListener.h>

/**
 * The menu bar contains a group of menu widgets and can be oriented
 * horizontally or vertically. 
 */
class GmMenuBar: public GmContainer,
                 public GmSelectionListener
{
 protected:
   /**
    * The vector of listeners that listen to this component.
    */
   vector<GmSelectionListener*> mSelectionListeners;
   
   /**
    * The list of menus that are part of this menu bar
    */
   vector<GmMenu*> mMenus;

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
   GmMenuBar();

   /**
    * Virtual destructor
    */
   virtual ~GmMenuBar();

   /**
    * Adds a selection listener to this menubar.
    *
    * @param sl a selection listener that will listen to this menubar.
    */
   virtual void addSelectionListener(GmSelectionListener* sl);

   /**
    * Removes a selection listener from this menubar.
    *
    * @param sl the selection listener to remove.
    * @return true if the listener was removed, false if not.
    */
   virtual bool removeSelectionListener(GmSelectionListener* sl);   
   
   /**
    * Adds a menu to the menu bar
    *
    * @param menu the menu to add to the menu bar
    * @param widgetTokenId the identifier token for the menu
    */
   virtual void addMenu(GmMenu* menu, WidgetIDType widgetId);

   /**
    * Event processing function to process a key press event.
    *
    * @param event the key press event data
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

   /* Interface for GmFocusListener */

   /**
    * Called whenever a widget loses focus.
    *
    * @param action the action associated with the focus.
    */
   virtual void focusLost(GmFocusAction* action);
   
   /* End Interface for GmFocusListener */   

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

 protected:
   /**
    * Renders this menuBar to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the menuBar.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);
   
   /**
    * Renders this menuBar to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the menuBar.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);

   /**
    * Calls each menu's render function
    *
    * @param renderOptions the render options to use when rendering
    *                      the menus.    
    */
   virtual void renderMenus(GmTypes::RenderOptionType renderOptions);   
};

#endif
