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
#ifndef GMDROPLISTENER_H
#define GMDROPLISTENER_H

#include <glam/GmConfig.h>
#include <glam/GmWidgetListener.h>
#include <glam/GmDropAction.h>
#include <vector>
#include <string>

/**
 * The drop target widget must implement this interface. It can then
 * accept dragable widgets that have an acceptable mime type.
 */
class GmDropListener : public GmWidgetListener
{
 protected:
   /**
    * The vector of acceptable mime-types for this drop target widget.
    */
   vector<string> mAcceptableMimeTypes;
   
   /**
    * Adds a drag mime-type to this drop target
    */
   virtual void acceptDragMimeType(string mimeType);
   
 public:
   /**
    * Default constructor
    */
   GmDropListener();

   /**
    * Virtual destructor
    */
   virtual ~GmDropListener();
   
   /**
    * Gets the set of acceptable mime-types for this drop target.
    *
    * @return a vector of strings, which are the acceptable mime-types
    *         for this particular view. THIS IS A BORROWED POINTER, DO
    *         NOT MODIFY THE DATA.
    */
   virtual vector<string>* getAcceptableDragMimeTypes();

   /**
    * This method is called whenever a drop happens on the particular
    * widget. The widget is guaranteed that the drop conforms to
    * atleast one in it's list of acceptable drag mime types.
    *
    * @param dropAction the action associated with the drop
    */
   virtual void receiveDrop(GmDropAction* dropAction);
};

#endif
