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
#ifndef GMTOOLTIP_H
#define GMTOOLTIP_H

//#include <glam/GmTextArea.h>
#include <glam/GmTextWidget.h>

/**
 * The tool tip class is used by most interactive widgets to display
 * more information on what the interactive widget does.
 */
class GmToolTip : public GmTextWidget // this needs to be changed to
                                      // text area
{
 public:
   /**
    * Standard constructor
    */
   GmToolTip();

   /**
    * Virtual destructor
    */
   virtual ~GmToolTip();
   
 protected:
   /**
    * Renders this label to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the label.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);
};

#endif
