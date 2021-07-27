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
#include <GL/glut.h>
#include <glam/GmEventDistributor.h>
#include <glam/GmPushButton.h>

static GmEventDistributor* gEventDistributor = NULL;
   
GmEventDistributor::GmEventDistributor()
{
   mLastWidgetPressed = GLAM_INVALID_OBJECTID;
}
   
GmEventDistributor::~GmEventDistributor()
{
}

GmEventDistributor* GmEventDistributor::getInstance()
{
   if(gEventDistributor == NULL)
      gEventDistributor = new GmEventDistributor();

   return gEventDistributor;
}

void GmEventDistributor::registerWidget(GmInteractiveWidget* widget)
{
   mRegisteredWidgets[widget->getObjectId()] = widget;
}

void GmEventDistributor::unregisterWidget(GmInteractiveWidget* widget)
{
   mRegisteredWidgets.erase(widget->getObjectId());
}

void GmEventDistributor::fireEvent(GmEvent* event)
{
   GmTypes::EventType eventType = event->getType();
   GmInteractiveWidget* widget =
      mRegisteredWidgets[event->getBasicData()->rxId];

   if(widget != NULL)
   {
      switch(eventType)
      {
         case GmTypes::E_MOUSE_PRESS:
            widget->mousePressEvent(event);
            break;
         case GmTypes::E_MOUSE_RELEASE:
            widget->mouseReleaseEvent(event);
            processDrop(widget, event);
            mDraggedWidgets.clear();
            //removeDraggedWidget(widget);
            break;
         case GmTypes::E_MOUSE_MOVEMENT:
            widget->mouseMoveEvent(event);
            break;
         case GmTypes::E_MOUSE_ENTERED:
            widget->mouseEnteredEvent(event);
            break;
         case GmTypes::E_MOUSE_EXITED:
            widget->mouseExitedEvent(event);
            break;
         case GmTypes::E_MOUSE_DRAG:
            addDraggedWidget(widget);
            widget->mouseDragEvent(event);
            break;
         case GmTypes::E_MOUSE_WHEEL:
            widget->wheelMoveEvent(event);
            break;
         case GmTypes::E_KEY_PRESS:
            widget->keyPressEvent(event);
         case GmTypes::E_KEY_RELEASE:
            widget->keyReleaseEvent(event);
            break;
         default:
            cout << "GmEventDistributor::distributeEvent() error: "
                 << "Cannot distribute event " << eventType << endl;
      }
   }
}

void GmEventDistributor::distributeEvent(GmEvent* event)
{
   GmTypes::EventType eventType = event->getType();
   event->getBasicData()->timeStamp = glutGet(GLUT_ELAPSED_TIME)/1000.0;

   switch(eventType)
   {
      case GmTypes::E_MOUSE_PRESS:
         mLastWidgetPressed = event->getBasicData()->rxId;
         break;
      case GmTypes::E_MOUSE_RELEASE:
         if(mDraggedWidgets.begin() != mDraggedWidgets.end())
            fireEvent(event);
         event->getBasicData()->rxId = mLastWidgetPressed;
         break;
      default:
         break;
   }
   
   fireEvent(event);
}

void GmEventDistributor::processDrop(GmInteractiveWidget* widget,
                                     GmEvent* event)
{
   // Check to see if this widget accepts widget drops
   if(mDraggedWidgets.begin() != mDraggedWidgets.end())
   {
      vector<string>* amt = widget->getAcceptableDragMimeTypes();

      // Go through all of our dragged widgets
      for(map<ObjectIDType, GmInteractiveWidget*>::iterator wi =
             mDraggedWidgets.begin(); wi != mDraggedWidgets.end();
          wi++)
      {
         GmInteractiveWidget* w = (*wi).second;

         // Go through all the mime types that the widget accepts
         for(vector<string>::iterator mti = amt->begin();
             mti != amt->end(); mti++)
         {
            string mt = (*mti);

            // if the widget accepts the dragabble's mime type and the
            // widget ID's are not the same, drop the widget
            if(mt == w->getDragMimeType())
            {
               GmDropAction* dropAction =
                  new GmDropAction(w, mt, w->getDragData());
               
               widget->receiveDrop(dropAction);

               delete dropAction;
               removeDraggedWidget(w);
               //wi--;
            }
         }
      }
   }
}

void GmEventDistributor::clearFocus()
{
   // unfocus the upper most widget (others will follow)
   if(!mFocusedWidgets.empty())
      mFocusedWidgets.back()->setFocused(false);
   
   // clear the vector
   mFocusedWidgets.clear();
}

GmInteractiveWidget* GmEventDistributor::getFocus()
{
   if(mFocusedWidgets.empty())
      return NULL;

   return mFocusedWidgets.front();
}

vector<GmInteractiveWidget*>* GmEventDistributor::getFocusList()
{
   return &mFocusedWidgets;
}

void GmEventDistributor::addFocusedWidget(GmInteractiveWidget* widget)
{
   mFocusedWidgets.push_back(widget);
}

void GmEventDistributor::prefixFocusedWidget(GmInteractiveWidget* widget)
{
   if(mFocusedWidgets.empty())
      addFocusedWidget(widget);
   else
      mFocusedWidgets.insert(mFocusedWidgets.begin(), widget);
}

GmIcon* GmEventDistributor::getDraggedIcon()
{
   GmIcon* icon = NULL;
   
   if(mDraggedWidgets.size() > 0)
   {
      GmInteractiveWidget* w;
      map<ObjectIDType, GmInteractiveWidget*>::iterator wi =
         mDraggedWidgets.begin();

      w = (*wi).second;
      icon = w->getDragIcon();
   }
   
   return icon;
}

void GmEventDistributor::addDraggedWidget(GmInteractiveWidget* widget)
{
   if(widget->getDragable())
   {
      mDraggedWidgets[widget->getObjectId()] = widget;
   }
}

void GmEventDistributor::removeDraggedWidget(GmInteractiveWidget* widget)
{
   mDraggedWidgets.erase(widget->getObjectId());
}
