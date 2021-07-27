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
#include <glam/GmConfig.h>
#include <unistd.h>
#include <glam/GmEvent.h>

GmEvent::GmEvent()
{
   mPayload = NULL;
}

GmEvent::~GmEvent()
{
   freePayload();
}

GmTypes::EventType GmEvent::getType()
{
   return mType;
}

GmEvent::BasicEventType* GmEvent::getBasicData()
{
   GmEvent::BasicEventType* basicEvent = NULL;
   
   basicEvent = (GmEvent::BasicEventType*)mPayload;
   return basicEvent;
}

GmEvent::MouseEventType* GmEvent::getMouseData()
{
   GmEvent::MouseEventType* mouseEvent = NULL;
   
   switch(mType)
   {
      case GmTypes::E_MOUSE_PRESS:
      case GmTypes::E_MOUSE_RELEASE:
      case GmTypes::E_MOUSE_ENTERED:
      case GmTypes::E_MOUSE_EXITED:
      case GmTypes::E_MOUSE_DRAG:
      case GmTypes::E_MOUSE_MOVEMENT:
      case GmTypes::E_MOUSE_WHEEL:
         mouseEvent = (GmEvent::MouseEventType*)mPayload;
         break;
      default:
         mouseEvent = NULL;
   }
   
   return mouseEvent;
}

GmEvent::KeyEventType* GmEvent::getKeyData()
{
   GmEvent::KeyEventType* keyEvent = NULL;
    
   switch(mType)
   {
      case GmTypes::E_KEY_PRESS:
      case GmTypes::E_KEY_RELEASE:
         keyEvent = (GmEvent::KeyEventType*)mPayload;
         break;
      default:
         keyEvent = NULL;
   }

   return keyEvent;
}

void GmEvent::setMouseEvent(GmTypes::EventType eventType,
                            unsigned short targetId, int button, int state,
                            float x, float y)
{
   GmEvent::MouseEventType* mouseEvent;
   
   switch(eventType)
   {
      case GmTypes::E_MOUSE_PRESS:
      case GmTypes::E_MOUSE_RELEASE:
      case GmTypes::E_MOUSE_ENTERED:
      case GmTypes::E_MOUSE_EXITED:
      case GmTypes::E_MOUSE_DRAG:
      case GmTypes::E_MOUSE_MOVEMENT:
      case GmTypes::E_MOUSE_WHEEL:
         mType = eventType;
         break;
      default:
         mType = GmTypes::E_MOUSE_PRESS;
   }

   mouseEvent = new GmEvent::MouseEventType;
   mouseEvent->basicData.rxId = targetId;
   mouseEvent->button = button;
   mouseEvent->state = state;
   mouseEvent->x = x;
   mouseEvent->y = y;

   // get the mouse GL coordinates
   GLdouble ModelMatrix[16];
   GLdouble ProjMatrix[16];
   GLint Viewport[4];
   
   glGetDoublev(GL_MODELVIEW_MATRIX, ModelMatrix);
   glGetDoublev(GL_PROJECTION_MATRIX, ProjMatrix);
   glGetIntegerv(GL_VIEWPORT, Viewport);
   
   // transform mouse y coord into real y coord
   float realY = Viewport[3] - y - 1;
   
   // unproject mouse coords into GL coords
   GLdouble GLx, GLy, GLz;
   gluUnProject(x, realY, 0.0,
                ModelMatrix, ProjMatrix, Viewport,
                &GLx, &GLy, &GLz);

   mouseEvent->GLx = GLx;
   mouseEvent->GLy = GLy;
   mouseEvent->GLz = GLz;

   // set the pay load
   freePayload();
   mPayload = mouseEvent;
}

void GmEvent::setKeyEvent(GmTypes::EventType eventType,
                          unsigned short targetId, unsigned char key,
                          float x, float y)
{
   GmEvent::KeyEventType* keyEvent;
   
   switch(eventType)
   {
      case GmTypes::E_KEY_PRESS:
      case GmTypes::E_KEY_RELEASE:
         mType = eventType;
         break;
      default:
         mType = GmTypes::E_KEY_PRESS;
   }

   keyEvent = new GmEvent::KeyEventType;

   keyEvent->basicData.rxId = targetId;
   keyEvent->key = key;
   keyEvent->x = x;
   keyEvent->y = y;

   freePayload();
   mPayload = keyEvent;
}

void GmEvent::freePayload()
{
   if(mPayload != NULL)
   {
      switch(mType)
      {
         case GmTypes::E_MOUSE_PRESS:
         case GmTypes::E_MOUSE_RELEASE:
         case GmTypes::E_MOUSE_ENTERED:
         case GmTypes::E_MOUSE_EXITED:
         case GmTypes::E_MOUSE_DRAG:
         case GmTypes::E_MOUSE_MOVEMENT:
         case GmTypes::E_MOUSE_WHEEL:
            delete (GmEvent::MouseEventType*)mPayload;
            break;
         case GmTypes::E_KEY_PRESS:
         case GmTypes::E_KEY_RELEASE:
            delete (GmEvent::KeyEventType*)mPayload;
            break;
         default:
            cout << "GmEvent::freePayload() error: Cannot delete unknown "
                 << "event payload type " << mType << endl;
      }
      
      mPayload = NULL;
   }
}
