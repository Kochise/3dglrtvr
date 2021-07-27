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
#ifndef GMCHECKBOX_H
#define GMCHECKBOX_H

#include <glam/GmButton.h>
#include <glam/GmSelectionListener.h>

/**
 * A checkbox class. Checkboxes represent a toggleable state.
 */
class GmCheckBox: public GmButton
{
 protected:
   /**
    * The vector of selection listeners that listen to this component.
    */
   vector<GmSelectionListener*> mSelectionListeners;
   
   /**
    * Determines if a checkbox has been checked or not.
    */
   bool mSelected;
   
   /**
    * The color of the check inside the checkbox
    */
   GlmColor4 mCheckColor;

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
   GmCheckBox();

   /**
    * Standard destructor
    */
   virtual ~GmCheckBox();

   /**
    * Adds a selection listener to this checkbox.
    *
    * @param sl a selection listener that will listen to this checkbox.
    */
   virtual void addSelectionListener(GmSelectionListener* sl);

   /**
    * Removes a selection listener from this checkbox.
    *
    * @param sl the selection listener to remove.
    * @return true if the listener was removed, false if not.
    */
   virtual bool removeSelectionListener(GmSelectionListener* sl);

   /**
    * Sets the check color of the checkbox. The check is the item that
    * is displayed inside the checkbox to denote that the user has
    * clicked on the checkbox.
    *
    * @param r the red value
    * @param g the green value
    * @param b the blue value
    * @param a the alpha value
    */
   virtual void setCheckColor(float r, float g, float b, float a);

   /**
    * Sets the text of this text widget to the given text.
    *
    * @param text the text to set this widget to display
    */
   virtual void setText(string text);

   /**
    * Event processing function to process a mouse release event.
    *
    * @param event the mouse release event for this object to process
    */
   virtual void mouseReleaseEvent(GmEvent* event);

   /**
    * Set the selected state of this checkbox.
    *
    * @param selected the new selected state
    */
   virtual void setSelected(bool selected);
   
   /**
    * Return the selected state of this checkbox.
    *
    * @return the selected state of this checkbox.
    */
   virtual bool getSelected();

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

 protected:
   /**
    * Renders this button to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this button to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
