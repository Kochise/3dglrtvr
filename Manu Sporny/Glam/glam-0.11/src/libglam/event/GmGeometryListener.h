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
#ifndef GMGEOMETRYLISTENER_H
#define GMGEOMETRYLISTENER_H

#include <glam/GmWidgetListener.h>
#include <glam/GmGeometryAction.h>

/**
 * The GmGeometryListener class is purely an interface that classes
 * can implement if they wish to handle geometry changes.
 */
class GmGeometryListener : public GmWidgetListener
{
 public:
   /**
    * Called whenever an associated widget is minimized.
    *
    * @param action the geometry action involved in geometry change.
    */
   virtual void minimizeOccurred(GmGeometryAction* action) { }

   /**
    * Called whenever an associated widget is maximized.
    *
    * @param action the geometry action involved in geometry change.
    */
   virtual void maximizeOccurred(GmGeometryAction* action) { }

   /**
    * Called whenever an associated widget has its geometry changed.
    *
    * @param action the geometry action involved in geometry change.
    */
   virtual void geometryResized(GmGeometryAction* action) { }
};

#endif
