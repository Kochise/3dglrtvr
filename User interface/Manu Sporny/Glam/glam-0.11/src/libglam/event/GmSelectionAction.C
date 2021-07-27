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
#include <glam/GmSelectionAction.h>

GmSelectionAction::GmSelectionAction()
      : GmWidgetAction()
{
}

GmSelectionAction::GmSelectionAction(GmInteractiveWidget* source,
                                     GmInteractiveWidget* selection)
      : GmWidgetAction(source)
{
   mSelection = selection->getWidgetId();
}

GmSelectionAction::GmSelectionAction(GmInteractiveWidget* source,
                                     WidgetIDType selection)
      : GmWidgetAction(source)
{
   mSelection = selection;
}

GmSelectionAction::GmSelectionAction(unsigned short srcObjectId,
                                     WidgetIDType srcWidgetId,
                                     GmInteractiveWidget* selection)
      : GmWidgetAction(srcObjectId, srcWidgetId)   
{
   mSelection = selection->getWidgetId();
}

GmSelectionAction::GmSelectionAction(unsigned short srcObjectId,
                                     WidgetIDType srcWidgetId,
                                     WidgetIDType selection)
      : GmWidgetAction(srcObjectId, srcWidgetId)
{
   mSelection = selection;
}

GmSelectionAction::~GmSelectionAction()
{
}

WidgetIDType GmSelectionAction::getSelection()
{
   return mSelection;
}
