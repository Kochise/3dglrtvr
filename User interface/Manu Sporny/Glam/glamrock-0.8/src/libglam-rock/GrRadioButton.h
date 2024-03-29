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
#ifndef GRRADIOBUTTON_H
#define GRRADIOBUTTON_H

#include <glam/GmRadioButton.h>

/**
 * The radio button class encapsulates standard functionality found in
 * a radio button.
 */
class GrRadioButton : public GmRadioButton
{
 public:
   /**
    * Standard constructor
    */
   GrRadioButton();

   /**
    * Virtual destructor
    */
   virtual ~GrRadioButton();
   
 protected:
   /**
    * Renders this radio button to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the panel.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);
   
   /**
    * Renders this radio button to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the dialog.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
