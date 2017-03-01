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
#ifndef GMDROPACTION_H
#define GMDROPACTION_H

#include <glam/GmConfig.h>
#include <glam/GmTypes.h>
#include <glam/GmWidgetAction.h>
#include <string>

/**
 * This action encapsulates information about "drops" that occur
 * when using drag and drop in GLAM.
 */
class GmDropAction : public GmWidgetAction
{
 protected:
   /**
    * The mime type associated with this drop action.
    */
   string mMimeType;

   /**
    * The data associated with this drop action.
    */
   void* mData;
   
   /**
    * Standard constructor.
    */
   GmDropAction();

 public:
   /**
    * Constructs a drop action with the given source,
    * mime type, and drop data.
    *
    * @param source the widget that is the source
    * @param mimeType the mime type for the drop action. This variable
    *                 is used to figure out how to decode the drop data.
    * @param data the data associated with the drop action and
    *             mimeType for this drop action
    */
   GmDropAction(GmInteractiveWidget* source, string mimeType, void* data);

   /**
    * Constructs a drop action with the given
    * object id and widget id of the source,
    * mime type, and drop data.
    *
    * @param objectId the object id of the source
    * @param widgetId the widget id of the source
    * @param mimeType the mime type for the drop action. This variable
    *                 is used to figure out how to decode the drop data
    * @param data the data associated with the drop action and
    *             mimeType for this drop action
    */
   GmDropAction(unsigned short srcObjectId, WidgetIDType srcWidgetId,
                string mimeType, void* data);

   /**
    * Virtual destructor.
    */
   virtual ~GmDropAction();

   /**
    * Gets the mime type associated with this action.
    *
    * @return the mime type for this drop action
    */
   virtual string getDropMimeType();

   /**
    * Gets the data associated with this drop action.
    *
    * @return the data associated with this drop action
    */
   virtual void* getDropData();
};

#endif
