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
#ifndef GMSELECTIONLISTENER_H
#define GMSELECTIONLISTENER_H

#include <glam/GmWidgetListener.h>
#include <glam/GmSelectionAction.h>

/**
 * The GmSelectionListener class is purely an interface that classes
 * can implement if they wish to handle selection behavior.
 */
class GmSelectionListener : public GmWidgetListener
{
 public:
   /**
    * Called whenever a selection occurs.
    *
    * @param action the select action.
    */
   virtual void selectionOccurred(GmSelectionAction* action) { }

   /**
    * Called whenever a deselection occurs.
    *
    * @param action the deselect action.
    */
   virtual void deselectionOccurred(GmSelectionAction* action) { }
};

#endif
