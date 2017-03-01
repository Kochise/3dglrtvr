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
#ifndef GMICONBAR_H
#define GMICONBAR_H

#include <glam/GmPushButton.h>
#include <glam/GmPanel.h>

/**
 * This icon bar can hold any number of push buttons.
 * It has the capability of displaying the buttons in any number of
 * rows.
 */
class GmIconBar: public GmPanel
{
 protected:
   /**
    * The vector of buttons
    */
   vector<GmButton*> mButtons;

   /**
    * The number of rows to put the buttons in.
    */
   unsigned int mNumRows;

 public:
   /**
    * Standard constructor
    */
   GmIconBar();

   /**
    * Virtual destructor
    */
   virtual ~GmIconBar();

   /**
    * Adds a button with the text passed as parameter
    *
    * @param buttonText the text to put on the new button
    */
   void addButton(string buttonText);
};

#endif
