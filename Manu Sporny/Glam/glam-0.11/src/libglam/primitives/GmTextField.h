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
#ifndef GMTEXTFIELD_H
#define GMTEXTFIELD_H

#include <glam/GmInteractiveWidget.h>
#include <glam/GmTextWidget.h>

/**
 * A text field is an enclosed area containing text that is editable.
 */
class GmTextField : public GmInteractiveWidget
{
 protected:
   /**
    * The text widget that displays the text in the field.
    */
   GmTextWidget* mText;

   /**
    * The flag to set this text widget to either editable or
    * uneditable (static) state.
    */
   bool mEditable;

   /**
    * Renders this text field to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the text field.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this text field to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the text field.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);

 public:
   /**
    * Standard constructor
    */
   GmTextField();

   /**
    * Virtual destructor
    */
   virtual ~GmTextField();

   /**
    * Sets the editable state of the text widget to either true or
    * false.
    *
    * @param state the state to set the editability of the text to.
    *              If true, the text is editable, if false it is
    *              static.
    */
   virtual void setEditable(bool state);

   /**
    * Sets the text contained in the text field
    *
    * @param text the text to set the text field to contain
    */
   virtual void setText(string text);

   /**
    * Gets the text contained in the text field.
    *
    * @return the text contained in the text field.
    */
   virtual string getText();
   
   /**
    * Called whenever a key press event has ocurred and is meant for
    * this component.
    *
    * @param event the key press event the ocurred
    */
   virtual void keyPressEvent(GmEvent* event);

   /**
    * This method is called whenever a drop happens on the particular
    * test field.
    *
    * @param dropAction the action associated with the drop
    */
   virtual void receiveDrop(GmDropAction* dropAction);
   
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
};

#endif
