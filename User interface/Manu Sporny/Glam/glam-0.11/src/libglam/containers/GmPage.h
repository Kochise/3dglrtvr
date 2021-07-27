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
#ifndef GMPAGE_H
#define GMPAGE_H

#include <glam/GmContentPane.h>
#include <glam/GmHyperlinkListener.h>

/**
 * A simple content pane for use with hyperlinks.
 *
 * Inheriting classes should provide a system so that hyperlinks
 * can be added with associated pages in pairs. Then when a hyperlink
 * is activated, the pair data structure can be queried for the
 * appropriate page so it can be passed off to listeners. This might
 * be implemented here in the base class in the future.
 */
class GmPage : public GmContentPane, public GmHyperlinkListener
{
 protected:
   /**
    * The vector of hyperlink listeners for this page.
    */
   vector<GmHyperlinkListener*> mHyperlinkListeners;
   
   /**
    * Fires the appropriate hyperlink action to all listeners.
    *
    * @param action the action to pass to the listeners
    */
   virtual void fireHyperlinkPressed(GmHyperlinkAction* action);
   
 public:
   /**
    * Standard constructor
    */
   GmPage();

   /**
    * Virtual destructor
    */
   virtual ~GmPage();

   /**
    * Adds a hyperlink listener to this hyperlink.
    *
    * @param hl a hyperlink listener that will listen to this hyperlink
    */
   virtual void addHyperlinkListener(GmHyperlinkListener* hl);

   /**
    * Removes a hyperlink listener from this hyperlink.
    *
    * @param hl the hyperlink listener to remove
    * @return true if the listener was removed, false if not
    */
   virtual bool removeHyperlinkListener(GmHyperlinkListener* hl);

   /**
    * Called whenever an associated hyperlink is pressed.
    *
    * @param action the hyperlink action involved in the press.
    */
   virtual void hyperlinkPressed(GmHyperlinkAction* action);
};

#endif
