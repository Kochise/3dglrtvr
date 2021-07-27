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
#ifndef GRTYPES_H
#define GRTYPES_H

/**
 * The shape factory singleton can draw a variety of curved objects
 * and surfaces to an OpenGL canvas.
 */
class GrTypes
{
 protected:
   /**
    * Standard constructor
    */
   GrTypes(){};

   /**
    * Virtual destructor
    */
   virtual ~GrTypes(){};
   
 public:
   /**
    * The border style to use when drawing a component.
    */
   enum BorderStyle
   {
      BS_TOP_LEFT = 1,
      BS_TOP_RIGHT = 2,
      BS_TOP = 3,
      BS_BOTTOM_LEFT = 4,
      BS_BOTTOM_RIGHT = 8,
      BS_BOTTOM = 12,
      BS_ALL = 15
   };
   
   enum FillStyle
   {
       /**
        * Fill the graphic element with a solid color
        */
       FS_SOLID,
       
       /**
        * Do not fill the element, rather make an outline of the
        * element
        */
       FS_OUTLINE
    };

   enum OrientationStyle
   {
      /**
       * Orientation is up.
       */
       OS_UP,
       
      /**
       * Orientation is down.
       */
       OS_DOWN,
       
      /**
       * Orientation is left.
       */
       OS_LEFT,
       
      /**
       * Orientation is right.
       */
       OS_RIGHT
   };

   enum WidgetState
   {
      /**
       * Widget is in a normal state.
       */
      WS_NORMAL,
         
      /**
       * Widget is in an expanded state.
       */
      WS_EXPANDED
   };
   enum ExpandDirection{
      /**
       *  Widget expands up
       */
      EX_UP,
      /**
       *  Widget expands down
       */
      EX_DOWN,
      /**
       *  Widget expands left
       */
      EX_LEFT,
      /**
       *  Widget expands right
       */
      EX_RIGHT
   };
};

#endif
