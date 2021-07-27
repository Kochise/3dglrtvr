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
#ifndef GMBUTTONACTION_H
#define GMBUTTONACTION_H

#include <glam/GmWidgetAction.h>

// forward declaration of GmButton
class GmButton;

/**
 * This action encapsulates information about actions performed
 * by buttons.
 */
class GmButtonAction : public GmWidgetAction
{
 protected:
   /**
    * The state of the button in this action.
    */
   GmTypes::ButtonStateType mState;
   
   /**
    * Standard constructor
    */
   GmButtonAction();

 public:
   /**
    * Constructs a button action with the given source
    * and button state.
    *
    * @param source the widget that is the source.
    * @param state the state of the button involved in this action.
    */
   GmButtonAction(GmButton* source, GmTypes::ButtonStateType state);

   /**
    * Constructs a button action with the given
    * object id and widget id of the source and
    * button state.
    *
    * @param objectId the object id of the source.
    * @param widgetId the widget id of the source.
    * @param state the state of the button involved in this action.
    */
   GmButtonAction(unsigned short srcObjectId, WidgetIDType srcWidgetId,
                  GmTypes::ButtonStateType state);
   
   /**
    * Virtual destructor
    */
   virtual ~GmButtonAction();
   
   /**
    * Gets the state of the button involved in this action.
    *
    * @return the state of the button involved in this action
    */
   GmTypes::ButtonStateType getButtonState();
};

#endif
