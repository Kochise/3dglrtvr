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
#ifndef GMSELECTIONACTION_H
#define GMSELECTIONACTION_H

#include <glam/GmWidgetAction.h>
#include <glam/GmInteractiveWidget.h>

/**
 * The GmSelectionAction class
 */
class GmSelectionAction : public GmWidgetAction
{
 protected:
   /**
    * The widget id of the selection in this action.
    */
   WidgetIDType mSelection;
   
   /**
    * Standard constructor
    */
   GmSelectionAction();

 public:
   /**
    * Constructs a selection action with the given source
    * and selection.
    *
    * @param source the widget that is the source.
    * @param selection the widget that is the selection.
    */
   GmSelectionAction(GmInteractiveWidget* source,
                     GmInteractiveWidget* selection);

   /**
    * Constructs a selection action with the given source
    * and the widget id of the selection.
    *
    * @param source the widget that is the source.
    * @param selection the widget id of the selection.
    */
   GmSelectionAction(GmInteractiveWidget* source, WidgetIDType selection);

   /**
    * Constructs a selection action with the given
    * object id and widget id of the source and the
    * widget id of the selection.
    *
    * @param objectId the object id of the source.
    * @param widgetId the widget id of the source.
    * @param selection the widget that is the selection.
    */
   GmSelectionAction(unsigned short srcObjectId, WidgetIDType srcWidgetId,
                     GmInteractiveWidget* selection);

   /**
    * Constructs a selection action with the given
    * object id and widget id of the source and the
    * widget id of the selection.
    *
    * @param objectId the object id of the source.
    * @param widgetId the widget id of the source.
    * @param selection the widget id of the selection.
    */
   GmSelectionAction(unsigned short srcObjectId, WidgetIDType srcWidgetId,
                     WidgetIDType selection);

   /**
    * Virtual destructor
    */
   virtual ~GmSelectionAction();
   
   /**
    * Gets the widget id of the selected/deselected item.
    *
    * @return the widget id of the selected item.
    */
   WidgetIDType getSelection();
};

#endif
