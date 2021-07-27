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
#ifndef GMLISTITEM_H
#define GMLISTITEM_H

#include <glam/GmInteractiveWidget.h>
#include <glam/GmSelectionListener.h>
#include <glam/GmLabel.h>

/**
 * Encapsulates the functionality of a list item in a
 * viewable/selectable list. A list item could be in a scrollable list
 * box, pull-down menu, pop-up menu, or any other UI component that
 * contains a list of widgets.
 */
class GmListItem : public GmInteractiveWidget
{
 public:
   /**
    * Standard constructor
    */
   GmListItem();

   /**
    * Virtual destructor
    */
   virtual ~GmListItem();

   /**
    * Adds a selection listener to this menu item.
    *
    * @param sl a selection listener that will listen to this menu item.
    */
   virtual void addSelectionListener(GmSelectionListener* sl);

   /**
    * Removes a selection listener from this menu item.
    *
    * @param sl the selection listener to remove.
    * @return true if the listener was removed, false if not.
    */
   virtual bool removeSelectionListener(GmSelectionListener* sl);

   /**
    * Event processing function to process a mouse press event.
    *
    * @param event the mouse press event for this object to process
    */
   virtual void mousePressEvent(GmEvent* event);

   /**
    * Sets the text of this text widget to the given text.
    *
    * @param text the text to set this widget to display
    */
   virtual void setText(string text);

   /**
    * Sets the display color of the text.
    *
    * @param color the color to set the text color to
    */
   virtual void setTextColor(GlmColor4* color);

   /**
    * Sets the display color of the text to the given red, green, blue
    * and alpha values.
    *
    * @param r the red component of the color
    * @param g the green component of the color
    * @param b the blue component of the color
    * @param a the alpha component of the color
    */
   virtual void setTextColor(float r, float g, float b, float a);

   /**
    * Sets the GL size of the text, this will affect the size at
    * which the font is rendered for the text string.
    *
    * @param size the size of the text in GL units.
    */
   virtual void setTextSize(float size);

   /**
    * Sets the horizontal and vertical text justification for the
    * widget text.
    *
    * @param horizontal the horizontal justification to use for the
    *                   text widget.
    * @param vertical the vertical justification to use for the text widget.
    */
   virtual void setTextJustification(GmTypes::JustificationType horizontal,
                                     GmTypes::JustificationType vertical);

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
    * Gets the ideal extents of the drawable in OpenGL units. In the
    * case of the GmListItem, it returns the minimum extents the text
    * will fit into for the label. (This solves some major problems
    * with the menu code for the time being, but may be modified later.)
    *
    * @param width the width of the drawable in OpenGL units
    * @param height the height of the drawable in OpenGL units
    */
   virtual void getGLExtents(float& width, float& height);
   
 protected:
   /**
    * The vector of listeners that listen to this component.
    */
   vector<GmSelectionListener*> mSelectionListeners;

   /**
    * The text representing this object.
    */
   GmLabel* mLabel;   
   
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
   
   /**
    * Renders this menu item to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this menu item to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
