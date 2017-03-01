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
#ifndef GMOBJECT_H
#define GMOBJECT_H

#include <glam/GmConfig.h>

/**
 * The base class for all GLAM object is the GmObject class. It
 * encapsulates general functionality for all GLAM objects.
 */
class GmObject
{
  protected:
   /**
    * The object ID is used to uniquely identify each GmObject.
    */
   unsigned short mObjectId;

  public:
    /**
     * Standard constructor.
     */
    GmObject();

    /**
     * Standard destructor.
     */
    virtual ~GmObject();

    /**
     * Gets the object ID of this GLAM object. The object ID is useful
     * for unique identification and is also used internally for
     * OpenGL picking.
     *
     * @return the object id of the given GLAM object
     */
    unsigned short getObjectId();

    /**
     * Overloaded equal comparison operator for GmObjects. This
     * operator compares ObjectIds for equality.
     *
     * @return true if object ids are equal, false if not
     */
    bool operator==(const GmObject& obj);
    
 private:
    /**
     * Allocates an object ID from a pool of available object IDs.
     *
     * @return an object ID in the range from 1 to 65535
     */
    unsigned short allocateObjectId();
    
    /**
     * De-allocates an object ID from a pool of available object IDs.
     *
     * @param objectId the object ID to deallocate and return to the
     *                 pool of available object IDs
     */
    void deallocateObjectId(unsigned short objectId);
};

#endif
