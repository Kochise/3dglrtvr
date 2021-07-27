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
#ifndef GMCOMBOBOX_H
#define GMCOMBOBOX_H

#include <glam/GmMenu.h>

/**
 * The combo box class defines a user interface widget that has a
 * button and a drop down menu attached to it. When the button is
 * pressed, the menu is dropped so the user may make a selection.
 */
class GmComboBox : public GmMenu
{
 protected:
   /**
    * The current selection
    */
   GmInteractiveWidget* mCurrentSelection;
   
   /**
    * Fires the appropriate selection action to all listeners.
    *
    * @param action the action to pass to the listeners
    */
   virtual void fireSelection(GmSelectionAction* action);
   
 public:
   /**
    * Standard constructor
    */
   GmComboBox();
   
   /**
    * Virtual destructor
    */
   virtual ~GmComboBox();

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
    * Sets the current selection to the component with the given
    * widget ID.
    *
    * @param widgetId the component to set as the active selected
    *                 component in the combo box.
    */
   virtual void setCurrentSelection(WidgetIDType widgetId);

   /**
    * Gets the current selection.
    *
    * @return the widget ID of the selected component.
    */
   virtual WidgetIDType getCurrentSelection();
   
 protected:
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
   void getMenuExtents(float& width, float& height);
      
   /**
    * Renders this combo box to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the combo box.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this combo box to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the combo box.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this combo box's menu items.
    *
    * @param renderOptions the render options to use when rendering
    *                      the items.
    */
   virtual void renderMenuItems(GmTypes::RenderOptionType renderOptions);
};

#endif
