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
#ifndef GMEVENTDISTRIBUTOR_H
#define GMEVENTDISTRIBUTOR_H

#include <map>
#include <glam/GmEvent.h>
#include <glam/GmInteractiveWidget.h>
#include <glam/GmIcon.h>

/**
 * This class handles all user event distribution.  There is a 1:1
 * coorespondence between user events and their target components.
 * Only the component that resides in the top most layer of the UI
 * should receive user events.
 */
class GmEventDistributor
{
 protected:
   /**
    * Description
    */
   map<ObjectIDType, GmInteractiveWidget*> mRegisteredWidgets;

   /**
    * The vector of currently focused widgets.
    */
   vector<GmInteractiveWidget*> mFocusedWidgets;
   
   /**
    * Protected constructor
    */
   GmEventDistributor();
   
   /**
    * Virtual destructor
    */
   virtual ~GmEventDistributor();

   /**
    * The object ID of the last widget that was mouse pressed.
    */
   ObjectIDType mLastWidgetPressed;

   /**
    * The widgets that are currently being dragged across the screen.
    */
   map<ObjectIDType, GmInteractiveWidget*> mDraggedWidgets;

   /**
    * Does the actual work of firing the event.
    *
    * @param event the event to fire to a widget
    */
   void fireEvent(GmEvent* event);

   /**
    * Adds a widget to the list of dragged widgets.
    *
    * @param widget the widget that is currently being dragged
    */
   void addDraggedWidget(GmInteractiveWidget* widget);

   /**
    * Removes a widget to the list of dragged widgets.
    *
    * @param widget the widget to remove from the list of dragged widgets
    */
   void removeDraggedWidget(GmInteractiveWidget* widget);
   
 public:
   /**
    * Description
    */
   static GmEventDistributor* getInstance();
   
   /**
    * Description
    */
   void registerWidget(GmInteractiveWidget* widget);
   
   /**
    * Description
    */
   void unregisterWidget(GmInteractiveWidget* widget);

   /**
    * Description
    */
   void distributeEvent(GmEvent* event);

   /**
    * Processes the drop portion of a drag and drop action. The drop
    * checks to see if the widget in question can accept the currently
    * dragged components, and it accepts the drop only for the
    * components that it understands.
    *
    * @param widget the widget that will be recieving the drop
    * @param event the event that is associated with the drop. Most
    *              likely it is a mouse release event.
    */
   void processDrop(GmInteractiveWidget* widget, GmEvent* event);

   /**
    * Clears the vector of focused widgets.
    */
   void clearFocus();

   /**
    * Returns the main focused widget.
    */
   GmInteractiveWidget* getFocus();

   /**
    * Returns a pointer to the vector of focused widgets.
    *
    * @return the vector of focused widgets.
    */
   vector<GmInteractiveWidget*>* getFocusList();

   /**
    * Adds a widget to the end of the list of focused widgets.
    */
   void addFocusedWidget(GmInteractiveWidget* widget);

   /**
    * Adds a widget to the beginning of list of focused widgets.
    */
   void prefixFocusedWidget(GmInteractiveWidget* widget);

   
   /**
    * Gets the current dragged icon.
    *
    * @return the currently dragged icon if there is one, or NULL if
    *         nothing is being dragged at the moment.
    */
   GmIcon* getDraggedIcon();
};

#endif
