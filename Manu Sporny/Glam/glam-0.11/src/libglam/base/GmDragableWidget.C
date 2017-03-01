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
#include <glam/GmDragableWidget.h>
#include <glam/GmSvgIcon.h>

GmDragableWidget::GmDragableWidget()
{
   mDragIcon = NULL;
}

GmDragableWidget::~GmDragableWidget()
{
   if(mDragIcon != NULL)
      delete mDragIcon;
}

bool GmDragableWidget::getDragable()
{
   return false;
}

void* GmDragableWidget::getDragData()
{
   return (void*)"No data";
}

string GmDragableWidget::getDragMimeType()
{
   return "application/null";
}

GmIcon* GmDragableWidget::getDragIcon()
{
   if(mDragIcon == NULL)
      mDragIcon = new GmSvgIcon(GLAM_DATA_DIR "/notdragable.svg");
   
   return mDragIcon;
}
