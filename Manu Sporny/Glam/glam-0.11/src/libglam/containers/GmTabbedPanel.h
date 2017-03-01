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
#ifndef GMTABBEDPANEL_H
#define GMTABBEDPANEL_H

#include <glam/GmPanel.h>
#include <glam/GmContainer.h>

/**
 * The tabbed panel is a container which can have a series of tabs at
 * the top, for selecting multiple panels for display in the same
 * area, one on top of another.
 * This is not yet implemented correctly.
 */
class GmTabbedPanel: public GmContainer
{
 protected:
   /**
    * The vector of panels that make up the tabbed panel.
    */
   vector<GmPanel*> mPanels;

 public:
   /**
    * Default constructor
    */
   GmTabbedPanel();

   /**
    * Virtual destructor
    */
   virtual ~GmTabbedPanel();
};

#endif
