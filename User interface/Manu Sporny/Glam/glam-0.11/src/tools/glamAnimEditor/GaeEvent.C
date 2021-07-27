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
#include <iostream>
#include <GaeEvent.h>

using namespace std;

GaeEvent::GaeEvent() 
      : GmEvent()
{
}

GaeEvent::~GaeEvent()
{
   freePayload();
}

GaeTypes::EventType GaeEvent::getGaeType()
{
   return mGaeType;
}

GaeEvent::RestructureEventType* GaeEvent::getRestructureData()
{
   RestructureEventType* restructureEvent = NULL;

   ///// TEMP CODE: OLD USER EVENT STUFF REMOVED
//    switch(mType)
//    {
//       switch(mGaeType)
//       {
//          
//          case GaeTypes::E_RESTRUCTURE:
//             restructureEvent = (RestructureEventType*)mPayload;
//             break;
            
//          default:
//             restructureEvent = NULL;
//             break;
//       }
//       break;

//       default:
//          restructureEvent = NULL;
//          break;
//    }

   return restructureEvent;
}

void GaeEvent::setRestructureEvent(unsigned int targetId,
                                   GaeTypes::ViewOrientationType
                                   viewOrientation,
                                   vector<GaeControlPoint*>* controlPoints)
{
   RestructureEventType* restructureEvent;
   
   //   mType = GmTypes::E_USER;
   mGaeType = GaeTypes::E_RESTRUCTURE;

   restructureEvent = new RestructureEventType;
   restructureEvent->basicData.rxId = targetId;
   restructureEvent->viewOrientation = viewOrientation;
   restructureEvent->controlPoints = controlPoints;
   
   freePayload();
   mPayload = restructureEvent;
}

void GaeEvent::freePayload()
{
   if(mPayload != NULL)
   {
      switch(mType)
      {
//          case GmTypes::E_USER:     
//             switch(mGaeType)
//             {
//                case GaeTypes::E_RESTRUCTURE:
//                   delete (RestructureEventType*)mPayload;
//                   break;

//                default:
//                   cout <<
//                      "GaeEvent::freePayload() error: Cannot delete unknown "
//                        << "event payload type " << mType << endl;
//                   break;
//             }
//             break;

         default:
            GmEvent::freePayload();
            cout << "GaeEvent::freePayload() error: Cannot delete unknown "
                 << "event payload type " << mType << endl;
            break;
      }
      mPayload = NULL;
   }
}
