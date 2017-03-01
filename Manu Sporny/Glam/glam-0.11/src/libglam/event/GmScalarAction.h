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
#ifndef GMSCALARACTION_H
#define GMSCALARACTION_H

#include <glam/GmWidgetAction.h>

// forward declaration of scalar widget
class GmScalarWidget;

/**
 * This action encapsulates information about actions performed
 * by scalar widgets. When the value of a scalar widget is
 * changed by its actions, the information can be passed via
 * this class.
 */
class GmScalarAction : public GmWidgetAction
{
 protected:
   /**
    * The value of the scalar involved in this action.
    */
   float mValue;

   /**
    * The change from the old value.
    */
   float mChange;
   
   /**
    * Standard constructor.
    */
   GmScalarAction();

 public:
   /**
    * Constructs a scalar action with the given source,
    * scalar value, and scalar change.
    *
    * @param source the widget that is the source
    * @param value the current value of the scalar
    * @param change the change from the old value
    */
   GmScalarAction(GmScalarWidget* source, float value, float change);

   /**
    * Constructs a scalar action with the given
    * object id and widget id of the source and the
    * scalar value and scalar change.
    *
    * @param objectId the object id of the source
    * @param widgetId the widget id of the source
    * @param value the current value of the scalar
    * @param change the change from the old value
    */
   GmScalarAction(unsigned short srcObjectId, WidgetIDType srcWidgetId,
                  float value, float change);
   
   /**
    * Virtual destructor.
    */
   virtual ~GmScalarAction();
   
   /**
    * Gets the current value of the scalar involved in this action.
    *
    * @return the current value of the scalar
    */
   virtual float getValue();

   /**
    * Gets the change between the old value and the new value.
    *
    * @return the change from the old value
    */
   virtual float getChange();
};

#endif
