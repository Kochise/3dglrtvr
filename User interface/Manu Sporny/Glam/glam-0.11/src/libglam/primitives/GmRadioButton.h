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
#ifndef GMRADIOBUTTON_H
#define GMRADIOBUTTON_H

#include <glam/GmCheckBox.h>

/**
 * A radio button class. Radio buttons are like checkboxes in that
 * they represent a toggle, but they are typically are only used in
 * groups where the user must make a unique choice between a defined
 * set of options.
 */
class GmRadioButton : public GmCheckBox
{
 public:
   /**
    * Standard constructor
    */
   GmRadioButton();

   /**
    * Virtual destructor
    */
   virtual ~GmRadioButton();

   /**
    * Event processing function to process a mouse release event.
    *
    * @param event the mouse release event for this object to process
    */
   virtual void mouseReleaseEvent(GmEvent* event);

 protected:
   /**
    * Renders this radio button to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);
   
   /**
    * Renders this radio button to a 2D selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
