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
#include <glam/GmGeometryAction.h>

GmGeometryAction::GmGeometryAction()
      : GmWidgetAction()
{
}

GmGeometryAction::GmGeometryAction(GmInteractiveWidget* source,
                                   float widthChange, float heightChange,
                                   float newWidth, float newHeight)
      : GmWidgetAction(source)
{
   mWidthChange = widthChange;
   mHeightChange = heightChange;
   mNewWidth = newWidth;
   mNewHeight = newHeight;
}

GmGeometryAction::GmGeometryAction(unsigned short srcObjectId,
                                   WidgetIDType srcWidgetId,
                                   float widthChange, float heightChange,
                                   float newWidth, float newHeight)
      : GmWidgetAction(srcObjectId, srcWidgetId)
{
   mWidthChange = widthChange;
   mHeightChange = heightChange;
   mNewWidth = newWidth;
   mNewHeight = newHeight;
}

GmGeometryAction::~GmGeometryAction()
{
}

void GmGeometryAction::getChange(float& width, float& height)
{
   width = mWidthChange;
   height = mHeightChange;
}

void GmGeometryAction::getNewGeometry(float& width, float& height)
{
   width = mNewWidth;
   height = mNewHeight;
}

void GmGeometryAction::getWidthChange(float& width)
{
   width = mWidthChange;
}

void GmGeometryAction::getHeightChange(float& height)
{
   height = mHeightChange;
}

void GmGeometryAction::getNewWidth(float& width)
{
   width = mNewWidth;
}

void GmGeometryAction::getNewHeight(float& height)
{
   height = mNewHeight;
}
