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
#include <glam/GmFocusAction.h>

GmFocusAction::GmFocusAction()
      : GmWidgetAction()
{
}

GmFocusAction::GmFocusAction(GmInteractiveWidget* source)
      : GmWidgetAction(source)
{
   mFocusOrder = 0;
}

GmFocusAction::GmFocusAction(unsigned short srcObjectId,
                             WidgetIDType srcWidgetId)
      : GmWidgetAction(srcObjectId, srcWidgetId)
{
   mFocusOrder = 0;
}

GmFocusAction::GmFocusAction(GmInteractiveWidget* source, int focusOrder)
      : GmWidgetAction(source)
{
   mFocusOrder = focusOrder;
}

GmFocusAction::GmFocusAction(unsigned short srcObjectId,
                             WidgetIDType srcWidgetId,
                             int focusOrder)
      : GmWidgetAction(srcObjectId, srcWidgetId)
{
   mFocusOrder = focusOrder;
}

GmFocusAction::~GmFocusAction()
{
}

int GmFocusAction::getFocusOrder()
{
   return mFocusOrder;
}
