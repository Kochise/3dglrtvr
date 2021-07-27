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
#ifndef GMPUSHBUTTON_H
#define GMPUSHBUTTON_H

#include <glmath/GlmColor4.h>
#include <glam/GmIcon.h>
#include <glam/GmGraphicButton.h>

/**
 * This class defines a standard push button, a button that
 * has a background that tells the user that it affords pushing,
 * and that can have icons displayed on it.
 */
class GmPushButton : public GmGraphicButton
{  
 public:
   /**
    * Standard constructor
    */
   GmPushButton();
   
   /**
    * Virtual destructor
    */
   virtual ~GmPushButton();   

 protected:
   /**
    * Renders this button to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);
};

#endif
