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
#ifndef GMDECORATOR_H
#define GMDECORATOR_H

#include <glam/GmAnimatedWidget.h>

/**
 * A decorator is the base class for decorative, non-interactive
 * visual components, i.e. icons.
 */
class GmDecorator : public GmAnimatedWidget
{
 public:
   /**
    * Default constructor.
    */
   GmDecorator();

   /**
    * Virtual destructor.
    */
   virtual ~GmDecorator();
};

#endif
