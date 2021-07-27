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
#include <DragText.h>
#include <glam/GmSvgIcon.h>

DragText::DragText()
{
}

DragText::~DragText()
{
}

bool DragText::getDragable()
{
   return true;
}

string DragText::getDragMimeType()
{
   return "text/plain";
}

void* DragText::getDragData()
{
   return (void*)(mLabel->getText().c_str());
}

GmIcon* DragText::getDragIcon()
{
   if(mDragIcon == NULL)
      mDragIcon = new GmSvgIcon(GLAM_DATA_DIR "/dragText.svg");
   
   return mDragIcon;
}
