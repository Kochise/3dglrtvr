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
#include <glamrock/GrScrollablePane.h>
#include <glam/GmListLayout.h>

GrScrollablePane::GrScrollablePane()
{
   mHorzScrollbar = new GmScrollbar();
   mHorzScrollbar->setOrientation(GmTypes::OT_HORIZONTAL);
   mHorzScrollbar->setWidgetId(SCROLLBAR_H);
   mHorzScrollbar->addScalarListener(this);
   //mHorzScrollbar->setRelativeGeometry(0.0, 0.0, 0.9, 0.1);
   //mHorzScrollbar->setBaseColor(0.6, 0.6, 0.6, 1.0);
   mHorzScrollbar->setBorderWidth(0.1);
   mHorzScrollbar->setVisible(false);
   mHorzScrollbar->setMinimumValue(0.0);
   mHorzScrollbar->setMaximumValue(0.0);
   mHorzScrollbar->setCurrentValue(0.0);
   
   mVertScrollbar = new GmScrollbar();
   mVertScrollbar->setOrientation(GmTypes::OT_VERTICAL);
   mVertScrollbar->setWidgetId(SCROLLBAR_V);
   mVertScrollbar->addScalarListener(this);
   //mVertScrollbar->setRelativeGeometry(0.9, 0.1, 1.0, 1.0);
   //mVertScrollbar->setBaseColor(0.6, 0.6, 0.6, 1.0);
   mVertScrollbar->setBorderWidth(0.1);
   mVertScrollbar->setVisible(false);
   mVertScrollbar->setMinimumValue(0.0);
   mVertScrollbar->setMaximumValue(0.0);
   mVertScrollbar->setCurrentValue(0.0);

   //set the default layout (a list layout)
   GmListLayout* ll = new GmListLayout();
   setLayout(ll);
}

GrScrollablePane::~GrScrollablePane()
{
}
