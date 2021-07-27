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
#include <glam/GmObject.h>

#define MAX_IDS 65536
#define CACHE_SIZE (MAX_IDS/32)

static unsigned int gGmObjectIdCache[CACHE_SIZE];

GmObject::GmObject()
{
   mObjectId = allocateObjectId();
}

GmObject::~GmObject()
{
   deallocateObjectId(mObjectId);
}

unsigned short GmObject::getObjectId()
{
   return mObjectId;
}

bool GmObject::operator==(const GmObject& obj)
{
   return mObjectId == obj.mObjectId;
}

unsigned short GmObject::allocateObjectId()
{
   bool idFound = false;
   int i, b;
   for (i = 0; i < CACHE_SIZE && !idFound; i++)
   {
      if((gGmObjectIdCache[i] & 0xffffffff) != 0xffffffff)
      {
         for (b = 0; b < 32 && !idFound; b++)
         {
            if(((gGmObjectIdCache[i] >> b) ^ 0x1) == 0x1)
            {
               gGmObjectIdCache[i] |= (0x1 << b);
               idFound = true;
            }
         }
      }
   }

   if (i != 0)
      i--;
   if (b != 0)
      b--;

   return ((i*32) + b + 1);
}

void GmObject::deallocateObjectId(unsigned short objectId)
{
   int chunk = (objectId-1)/32;
   int bit = (objectId-1) % 32;
   unsigned int andMask = ~(0x1 << bit);
   
   gGmObjectIdCache[chunk] &= andMask;
}
