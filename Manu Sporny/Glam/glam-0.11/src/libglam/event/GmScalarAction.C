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
#include <glam/GmScalarAction.h>

GmScalarAction::GmScalarAction()
      : GmWidgetAction()
{
}

GmScalarAction::GmScalarAction(GmScalarWidget* source,
                               float value, float change)
      : GmWidgetAction((GmInteractiveWidget*)source)
{
   mValue = value;
   mChange = change;
}

GmScalarAction::GmScalarAction(unsigned short srcObjectId,
                               WidgetIDType srcWidgetId,
                               float value, float change)
      : GmWidgetAction(srcObjectId, srcWidgetId)
{
   mValue = value;
   mChange = change;
}

GmScalarAction::~GmScalarAction()
{
}

float GmScalarAction::getValue()
{
   return mValue;
}

float GmScalarAction::getChange()
{
   return mChange;
}
