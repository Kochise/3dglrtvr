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
#ifndef GMRADIOBUTTONGROUP_H
#define GMRADIOBUTTONGROUP_H

#include <glam/GmRadioButton.h>
#include <glam/GmContainer.h>
#include <glam/GmSelectionListener.h>
#include <glam/GmButtonListener.h>

/**
 * Radio button group is a collection of radio buttons, arranged and
 * assigned values to correspond to choices.
 */
class GmRadioButtonGroup : public GmContainer, public GmButtonListener
{
 protected:
   /**
    * The vector of listeners that listen to this component.
    */
   vector<GmSelectionListener*> mSelectionListeners;
   
   /**
    * The index of the selected button.
    */
   int mSelected;

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
   GmRadioButtonGroup();

   /**
    * Virtual destructor
    */
   virtual ~GmRadioButtonGroup();

   /**
    * Adds a selection listener to this group.
    *
    * @param sl a selection listener that will listen to this group.
    */
   virtual void addSelectionListener(GmSelectionListener* sl);

   /**
    * Removes a selection listener from this group.
    *
    * @param sl the selection listener to remove.
    * @return true if the listener was removed, false if not.
    */
   virtual bool removeSelectionListener(GmSelectionListener* sl);

   /**
    * Adds a radio button component to this radio button group.
    *
    * @param component the radio button to add to this radio button
    *                  group.
    * @param widgetId the widget ID to use when referring to the added
    *                 component.
    */
   virtual void addComponent(GmRadioButton* component,
                             WidgetIDType widgetId);
  
   /**
    * Repositions and resizes this component based on parameters
    * passed into the method. These parameters are usually passed in
    * by the parent class.
    *
    * @param minX the starting X position of the constraint volume
    * @param minY the starting Y position of the constraint volume
    * @param width the width of the constraint volume
    * @param height the height of the constraint volume
    * @param layer the layer the component should be placed in
    */
   virtual void layoutInGLArea(float minX, float minY,
                               float width, float height,
                               float layer);

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
    * Return a pointer to the currently selected radio button.
    *
    * @return a pointer to the currently selected radio button.
    */
   virtual GmRadioButton* getSelected();

   /**
    * Called whenever an button in this group is pressed.
    *
    * @param action the button action involved in the press.
    */
   virtual void buttonPressed(GmButtonAction* action);

 protected:
   /**
    * Renders this button to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
