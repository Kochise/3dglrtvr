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
#ifndef GRPUSHBUTTON_H
#define GRPUSHBUTTON_H

#include <glam/GmPushButton.h>

/**
 * The panel class is a basic container for grouping similar user
 * interface components on a single distinct UI pane.
 */
class GrPushButton : public GmPushButton
{
 public:
   /**
    * Standard constructor
    */
   GrPushButton();

   /**
    * Virtual destructor
    */
   virtual ~GrPushButton();
   
 protected:
   /**
    * Renders this button to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the panel.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);
   
   /**
    * Renders this button to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the dialog.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
