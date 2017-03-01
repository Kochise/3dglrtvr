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
#ifndef GAETYPES_H
#define GAETYPES_H

/**
 * Description
 */
class GaeTypes
{
 public:
   /**
    * Empty Constructor
    */
   GaeTypes(){};

   /**
    * Empty Destructor
    */
   ~GaeTypes(){};
   
   /**
    * View types possible for a spline edit widget.
    */
   enum ViewOrientationType
      {
         VO_XY,
         VO_ZX,
         VO_ZY,
         VO_XYZ         
      };
   /**
    * Enumeration for all events concerning the animation editor.
    */
   enum EventType
      {
         E_RESTRUCTURE
      };

   enum ControlPointType
      {
         CP_UNFIXED,
         CP_X_FIXED,
         CP_Y_FIXED
      };
};

#endif
