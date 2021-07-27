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
#ifndef GRGRAPHICBUTTON_H
#define GRGRAPHICBUTTON_H

#include <glam/GmGraphicButton.h>

/**
 * A glamrock style graphical button that can display an icon
 * when it is pressed and another one for when it is unpressed.
 */
class GrGraphicButton : public GmGraphicButton
{
 public:
   /**
    * Standard constructor.
    */
   GrGraphicButton();
   
   /**
    * Virtual destructor.
    */
   virtual ~GrGraphicButton();
};

#endif
