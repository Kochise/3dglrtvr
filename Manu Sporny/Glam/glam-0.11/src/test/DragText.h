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
#ifndef DRAGTEXT_H
#define DRAGTEXT_H

#include <glam/GmListItem.h>

class DragText : public GmListItem
{
 public:
   /**
    * Default constructor
    */
   DragText();

   /**
    * Virtual destructor
    */
   virtual ~DragText();

   /**
    * Gets whether this widget is drag-able or not. By default,
    * widgets are not drag-able.
    *
    * @return true if the widget is drag-able, false otherwise
    */
   virtual bool getDragable();
   
   /**
    * Gets the mime-type for this dragable widget. The mime-type is
    * used by the widget drop target when casting the drag data to its
    * native type.
    *
    * @return the mime type string for the drag data, such as
    *         "application/myapp-text"
    */
   virtual string getDragMimeType();

   /**
    * Gets the drag data associated with this dragable widget. The
    * drag data only makes sense to a widget that understands the
    * mime-type for this particular dragable icon.
    *
    * The drag data for a drag text icon is a NULL terminated char*
    * BORROWED POINTER. Do not modify the data once you have cast it
    * into a char*. Make a copy if you need to use the data after you
    * have cast it.
    *
    * @return the drag data associated with this dragable widget
    */
   virtual void* getDragData();
   
   /**
    * Gets the drag icon for this particular widget. The drag icon is
    * displayed when the object is being dragged from point A to point B.
    *
    * @return a "dragable" representation icon for this widget
    */
   virtual GmIcon* getDragIcon();
};

#endif
