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
#include <glam/GmHyperlinkAction.h>

GmHyperlinkAction::GmHyperlinkAction()
      : GmWidgetAction()
{
}

GmHyperlinkAction::GmHyperlinkAction(GmHyperlink* source)
      : GmWidgetAction((GmInteractiveWidget*)source)
{
}

GmHyperlinkAction::GmHyperlinkAction(unsigned short srcObjectId,
                                     WidgetIDType srcWidgetId)
      : GmWidgetAction(srcObjectId, srcWidgetId)
{
}

GmHyperlinkAction::GmHyperlinkAction(GmHyperlink* source, GmPage* page)
      : GmWidgetAction((GmInteractiveWidget*)source)
{
   mPage = page;
}

GmHyperlinkAction::GmHyperlinkAction(unsigned short srcObjectId,
                                     WidgetIDType srcWidgetId,
                                     GmPage* page)
      : GmWidgetAction(srcObjectId, srcWidgetId)
{
   mPage = page;
}

GmHyperlinkAction::GmHyperlinkAction(GmPage* srcPage, GmPage* destPage)
      : GmWidgetAction((GmInteractiveWidget*)srcPage)
{
   mPage = destPage;
}

GmHyperlinkAction::~GmHyperlinkAction()
{
}

GmPage* GmHyperlinkAction::getPage()
{
   return mPage;
}
