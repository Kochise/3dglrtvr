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
#ifndef GMWIDGETACTION_H
#define GMWIDGETACTION_H

#include <glam/GmTypes.h>
#include <glam/GmDrawable.h>

// forward declare interactive widget
class GmInteractiveWidget;

/**
 * The GmWidgetAction class is the base class for all actions.
 * It stores the widgetId of the source that acted.
 */
class GmWidgetAction
{
 protected:
   /**
    * The source's object id, it is its unique identifier.
    */
   unsigned short mSrcObjectId;
   
   /**
    * The source's widget id, it is its customized identifier.
    */
   WidgetIDType mSrcWidgetId;
   
   /**
    * Standard constructor.
    */
   GmWidgetAction();

 public:
   /**
    * Constructs a widget action with the given source.
    *
    * @param source the widget that is the source
    */
   GmWidgetAction(GmInteractiveWidget* source);

   /**
    * Constructs a widget action with the given
    * object id and widget id of the source.
    *
    * @param srcObjectId the object id of the source
    * @param srcWidgetId the widget id of the source
    */
   GmWidgetAction(unsigned short srcObjectId, WidgetIDType srcWidgetId);

   /**
    * Virtual destructor.
    */
   virtual ~GmWidgetAction();
   
   /**
    * Gets the widget ID of the widget involved in this action.
    *
    * @return the widget ID of the widget involved in this action
    */
   unsigned short getSrcObjectId();

   /**
    * Gets the widget ID of the widget involved in this action.
    *
    * @return the widget ID of the widget involved in this action
    */
   WidgetIDType getSrcWidgetId();
};

#endif
