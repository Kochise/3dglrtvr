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
#ifndef GAEEVENT_H
#define GAEEVENT_H

#include <vector>
#include <glam/GmEvent.h>
#include <GaeControlPoint.h>
#include <GaeSplineEditWidget.h>
#include <GaeTypes.h>

/**
 * The GmEvent class encapsulates all possible events that can be
 * delivered to components in glam. An event is usually created by the
 * application and then given to the GmEventDistributor for
 * distribution to the correct component, or set of components. The
 * GmEvent class supports mouse, keyboard and ui events (such as
 * focus, defocus, etc).
 */
class GaeEvent : public GmEvent
{
 public:

   /**
    * The RestructureEventType is used when the user changes the
    * position of a control point when editing a curve. It informs the
    * other viewports that a change has occured.
    */
   typedef struct
   {
      BasicEventType basicData;
      GaeTypes::ViewOrientationType viewOrientation;
      vector<GaeControlPoint*>* controlPoints;
   } RestructureEventType;

 protected:
   /**
    * The type of event that this event encapsulates. See GaeEventType for
    * a complete list of possible event types.
    */
   GaeTypes::EventType mGaeType;
   
 public:
   /**
    * Standard constructor
    */
   GaeEvent();

   /**
    * Virtual destructor
    */
   virtual ~GaeEvent();
   
   /**
    * Gets the type of this event.
    *
    * @return the type of gae event that this object encapsulates
    */
   GaeTypes::EventType getGaeType();

   /**
    * Gets the data associated with a restructure event.
    *
    * @return The data associated with a restructure event
    */
   RestructureEventType* getRestructureData();

   /**
    * Sets this event as a restructure event and fills in all the necessary
    * information about the event.
    */
   void setRestructureEvent(unsigned int targetId,
                            GaeTypes::ViewOrientationType viewOrientation,
                            vector<GaeControlPoint*>* controlPoints);
   
 protected:
   /**
    * This method frees the memory associated with the event
    * payload. The payload size can vary so the freePayload will
    * detect the size of the payload and delete the appropriately
    * sized memory block.
    */
   virtual void freePayload();   
};

#endif
