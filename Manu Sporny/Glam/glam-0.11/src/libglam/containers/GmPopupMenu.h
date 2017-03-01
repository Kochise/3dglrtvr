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
#ifndef GMPOPUPMENU_H
#define GMPOPUPMENU_H

#include <glam/GmMenu.h>

/**
 * GmPopupMenu is a menu that can be a context sensitive menu.
 * This would be useful for a right-click selection menu, etc.
 */
class GmPopupMenu : public GmMenu
{
 public:
   /**
    * Standard constructor
    */
   GmPopupMenu();

   /**
    * Virtual destructor
    */
   virtual ~GmPopupMenu();
};

#endif
