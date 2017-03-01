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
#ifndef GMFOCUSACTION_H
#define GMFOCUSACTION_H

#include <glam/GmWidgetAction.h>

/**
 * This action encapsulates information about focus changes.
 */
class GmFocusAction : public GmWidgetAction
{
 protected:
   /**
    * Standard constructor.
    */
   GmFocusAction();

   /**
    * The focus order number of the widget associated
    * with this action.
    */
   int mFocusOrder;

 public:
   /**
    * Constructs a focus action with the given source.
    *
    * @param source the widget that is the source
    */
   GmFocusAction(GmInteractiveWidget* source);

   /**
    * Constructs a focus action with the given
    * object id and widget id of the source.
    *
    * @param objectId the object id of the source
    * @param widgetId the widget id of the source
    */
   GmFocusAction(unsigned short srcObjectId, WidgetIDType srcWidgetId);

   /**
    * Constructs a focus action with the given source.
    *
    * @param source the widget that is the source
    * @param focusOrder the focus order of the associated widget
    */
   GmFocusAction(GmInteractiveWidget* source, int focusOrder);

   /**
    * Constructs a focus action with the given
    * object id and widget id of the source.
    *
    * @param objectId the object id of the source
    * @param widgetId the widget id of the source
    * @param focusOrder the focus order of the associated widget
    */
   GmFocusAction(unsigned short srcObjectId, WidgetIDType srcWidgetId,
                 int focusOrder);

   /**
    * Virtual destructor.
    */
   virtual ~GmFocusAction();

   /**
    * Returns the focus order of the widget associated in this action.
    *
    * @return the focus order
    */
   virtual int getFocusOrder();
};

#endif
