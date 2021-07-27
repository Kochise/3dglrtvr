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
#ifndef GMHYPERLINK_H
#define GMHYPERLINK_H

#include <glam/GmLabel.h>
#include <glam/GmHyperlinkListener.h>

/**
 * This class defines a visual hyperlink. A hyperlink is
 * text that links one "page" to another "page" when clicked on.
 */
class GmHyperlink : public GmLabel
{
 protected:
   /**
    * The vector of listeners for this button.
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
   GmHyperlink();
   
   /**
    * Virtual destructor
    */
   virtual ~GmHyperlink();

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
    * Event processing function to process a mouse press event.
    *
    * @param event the mouse press event for this object to process
    */
   virtual void mousePressEvent(GmEvent* event);

   /**
    * Event processing function to process a mouse release event.
    *
    * @param event the mouse release event for this object to process
    */
   virtual void mouseReleaseEvent(GmEvent* event);

 protected:
   /**
    * Renders this button to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this button to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
