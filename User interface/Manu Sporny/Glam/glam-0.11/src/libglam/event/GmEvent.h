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
#ifndef GMEVENT_H
#define GMEVENT_H

#include <glam/GmTypes.h>

/**
 * The GmEvent class encapsulates all user input events that can be
 * delivered to widgets in glam. An event is usually created by the
 * application and then given to the GmEventDistributor for
 * distribution to the correct widget, or set of widgets. The
 * GmEvent class supports mouse and keyboard events.
 */
class GmEvent
{
 public:
   /**
    * The BasicEventType is to store information that every event
    * must have.  Every specific event must have BasicEventType as the
    * first variable.
    */
   typedef struct
   {
      /**
       * Object that is supposed to recieve the event.
       */
      ObjectIDType rxId;

      /**
       * The time at which the event occurred (in seconds).
       */
      float timeStamp;
   } BasicEventType;
   
   /**
    * The MouseEventType is used for, *gasp* mouse events. It can
    * encapsulate which component the event is to be delivered to as
    * well as which button was pressed, the button's state
    * (pressed/released) and its x and y position in screen
    * coordinates.
    */
   typedef struct
   {
      BasicEventType basicData;
      int button;
      int state;
      float x;
      float y;
      float GLx;
      float GLy;
      float GLz;
   } MouseEventType;

   /**
    * The KeyEventType is used for keyboard events. It can encapsulate
    * which component the event is to be delivered to as well as which
    * key was pressed and the x and y position in screen coordinates.
    */
   typedef struct
   {
      BasicEventType basicData;
      unsigned char key;
      float x;
      float y;
   } KeyEventType;
   
 protected:
   /**
    * The type of event that this event encapsulates. See GmTypes for
    * a complete list of possible event types.
    */
   GmTypes::EventType mType;
   
   /**
    * The payload for the event. The payload changes based on the type
    * of event. If the GmEvent object's type is any of the mouse
    * events, then the mPayload is going to be a MouseEventType.
    */
   void* mPayload;
   
 public:
   /**
    * Standard constructor.
    */
   GmEvent();

   /**
    * Virtual destructor.
    */
   virtual ~GmEvent();
   
   /**
    * Gets the type of this event. Based on this type, you can call a
    * getXXXEvent(), where XXX is Mouse, Key, etc, to get the
    * data associated with the event. For a complete list of all event
    * types see GmTypes::EventType.
    *
    * @return the type of event that this object encapsulates
    */
   GmTypes::EventType getType();

   /**
    * Gets the data associated with any event.
    *
    * @return The data associated with any event
    */
   GmEvent::BasicEventType* GmEvent::getBasicData();
   
   /**
    * Gets the data associated with a mouse event.
    *
    * @return The data associated with a mouse event
    */
   GmEvent::MouseEventType* GmEvent::getMouseData();

   /**
    * Gets the data associated with a keyboard event.
    *
    * @return The data associated with a keyboard event
    */
   GmEvent::KeyEventType* GmEvent::getKeyData();

   /**
    * Sets this event as a mouse event and fills in all the necessary
    * information about the event.
    *
    * @param eventType the type of keyboard event to set the object to
    * @param targetId the widget ID to designate as the target for
    *                 this event
    * @param button the mouse button that is associated with this
    *               mouse event. This value should be one of
    *               GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON or
    *               GLUT_MIDDLE_BUTTON
    * @param state the state of the active button. This value should
    *              be one of GLUT_UP or GLUT_DOWN
    * @param x the x position of the pointer in screen coordinates
    * @param y the y position of the pointer in screen coordinates
    */
   virtual void setMouseEvent(GmTypes::EventType eventType,
                              unsigned short targetId,
                              int button, int state, float x, float y);

   /**
    * Sets this event as a keyboard event and fills in all the necessary
    * information about the event.
    *
    * @param eventType the type of keyboard event to set the object to
    * @param targetId the widget ID to designate as the target for
    *                 this event
    * @param key the key that was pressed
    * @param x the x position of the pointer in screen coordinates
    * @param y the y position of the pointer in screen coordinates
    */
   virtual void setKeyEvent(GmTypes::EventType eventType,
                            unsigned short targetId,
                            unsigned char key, float x, float y);
   
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
