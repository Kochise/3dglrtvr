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
#ifndef GMFOCUSLISTENER_H
#define GMFOCUSLISTENER_H

#include <glam/GmWidgetListener.h>
#include <glam/GmFocusAction.h>

/**
 * The GmFocusListener class allows widgets to inform listeners
 * of focus actions.
 */
class GmFocusListener : public GmWidgetListener
{
 public:
   /**
    * Called whenever a widget gains focus.
    *
    * @param action the action associated with the focus.
    */
   virtual void focusGained(GmFocusAction* action) { }

   /**
    * Called whenever a widget loses focus.
    *
    * @param action the action associated with the focus.
    */
   virtual void focusLost(GmFocusAction* action) { }
};

#endif
