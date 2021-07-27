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
#ifndef GRSCROLLABLEPANE_H
#define GRSCROLLABLEPANE_H

#include <glam/GmScrollablePane.h>
#include <glamrock/GrScrollbar.h>

/**
 * This is glamRock's scrollable viewport area that allows for content
 * larger than the viewable area.
 */
class GrScrollablePane : public GmScrollablePane
{
 public:
   /**
    * standard constructor
    */
   GrScrollablePane();
   
   /**
    * virtual destructor
    */
   virtual ~GrScrollablePane();
};

#endif
