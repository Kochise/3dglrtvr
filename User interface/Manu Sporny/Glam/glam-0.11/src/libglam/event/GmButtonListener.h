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
#ifndef GMBUTTONLISTENER_H
#define GMBUTTONLISTENER_H

#include <glam/GmWidgetListener.h>
#include <glam/GmButtonAction.h>

/**
 * The GmButtonListener class is purely an interface that classes
 * can implement if they wish to handle button behavior.
 */
class GmButtonListener : public GmWidgetListener
{
 public:
   /**
    * Called whenever an associated button is pressed.
    *
    * @param action the button action involved in the press.
    */
   virtual void buttonPressed(GmButtonAction* action) { }
};

#endif
