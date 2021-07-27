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
#ifndef GMSCALARLISTENER_H
#define GMSCALARLISTENER_H

#include <glam/GmWidgetListener.h>
#include <glam/GmScalarAction.h>

/**
 * The GmScalarListener class is purely an interface that classes
 * can implement if they wish to handle scalar widget behavior.
 */
class GmScalarListener : public GmWidgetListener
{
 public:
   /**
    * Called whenever the associated scalar widget is changed.
    *
    * @param action the scalar action involved in the change.
    */
   virtual void changeOccurred(GmScalarAction* action) { }
};

#endif
