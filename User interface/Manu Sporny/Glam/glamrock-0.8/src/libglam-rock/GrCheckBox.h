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
#ifndef GRCHECKBOX_H
#define GRCHECKBOX_H

#include <glam/GmCheckBox.h>

/**
 * A standard glamrock style checkbox.
 */
class GrCheckBox : public GmCheckBox
{
 public:
   /**
    * Standard constructor.
    */
   GrCheckBox();
   
   /**
    * Virtual destructor.
    */
   virtual ~GrCheckBox();
   
 protected:
   /**
    * Renders this button to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this button to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
