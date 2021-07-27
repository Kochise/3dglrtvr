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
#ifndef GRCOMBOBOX_H
#define GRCOMBOBOX_H

#include <glam/GmComboBox.h>

/**
 * A visual box that allows a user to make a selection from
 * a number of options.
 */
class GrComboBox : public GmComboBox
{
 public:
   /**
    * Standard constructor.
    */
   GrComboBox();
   
   /**
    * Virtual destructor.
    */
   virtual ~GrComboBox();
   
   /**
    * Gets the ideal extents of the drawable in OpenGL units. This is
    * sometimes used by parent containers to determine the optimal
    * layout strategy by taking each components extents into account.
    *
    * @param width the width of the drawable in OpenGL units
    * @param height the height of the drawable in OpenGL units
    */
   virtual void getGLExtents(float& width, float& height);
   
 protected:
   /**
    * Renders this button to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);
};

#endif
