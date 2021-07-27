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
#ifndef GMWIDGETLISTENER_H
#define GMWIDGETLISTENER_H

/**
 * The GmWidgetListener class is a base class for all listeners.
 * Listeners that inherit from this class are strictly interfaces,
 * therefore their methods should provide no default implementation.
 */
class GmWidgetListener
{
 public:
   /**
    * Overloaded equals operator for comparing listeners.
    */
   bool operator==(const GmWidgetListener& rhs) { return this == &rhs; }
};

#endif
