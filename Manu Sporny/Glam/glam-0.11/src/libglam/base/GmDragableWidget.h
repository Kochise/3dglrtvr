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
#ifndef GMDRAGABLEWIDGET_H
#define GMDRAGABLEWIDGET_H

#include <glam/GmIcon.h>

/**
 * A dragable widget is one that can be dragged from one point to
 * another. A dragable widget can be dropped onto another "drop-target"
 * widget that is a drop-target for a particular mime type.
 */
class GmDragableWidget
{
 protected:
   /**
    * The drag icon for this particular drag widget.
    */
   GmIcon* mDragIcon;
   
 public:
   /**
    * Default constructor
    */
   GmDragableWidget();

   /**
    * Virtual destructor
    */
   virtual ~GmDragableWidget();

   /**
    * Gets whether this widget is drag-able or not. By default,
    * widgets are not drag-able.
    *
    * @return true if the widget is drag-able, false otherwise
    */
   virtual bool getDragable();

   /**
    * Gets the drag data associated with this dragable widget. The
    * drag data only makes sense to a widget that understands the
    * mime-type for this particular dragable icon
    *
    * @return the drag data associated with this dragable widget
    */
   virtual void* getDragData();

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
    * Gets the drag icon for this particular widget. The drag icon is
    * displayed when the object is being dragged from point A to point B.
    *
    * @return a "dragable" representation icon for this widget
    */
   virtual GmIcon* getDragIcon();
};

#endif
