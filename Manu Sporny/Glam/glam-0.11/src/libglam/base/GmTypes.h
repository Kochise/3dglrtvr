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
#ifndef GMTYPES_H
#define GMTYPES_H

#define OGLFT_NO_QT
#include <OGLFT/OGLFT.h>
#include <limits.h>

#define GLAM_LAYER                0.000001f
#define GLAM_STATIC_WIDGETID_MAX  SHRT_MAX
#define GLAM_DYNAMIC_WIDGETID_MIN SHRT_MAX + 1
#define GLAM_DYNAMIC_WIDGETID_MAX INT_MAX
#define GLAM_INVALID_OBJECTID     0

// namespace for dynamic widget id's > SHORT_MAX
typedef unsigned int WidgetIDType;
typedef unsigned short ObjectIDType;

/**
 * The GmTypes class is an empty class that is used to encapsulate all
 * the various types that GLAM uses internally and externally. All
 * GLAM types should be defined in this class.
 */
class GmTypes
{
 protected:
   /**
    * Empty, protected constructor. You should never construct a
    * GmTypes object.
    */
   GmTypes(){};

   /**
    * Empty, protected destructor. There is no need for a destructor
    * since the object can never be constructed.
    */
   ~GmTypes(){};
   
 public:
   
   enum FillType
   {
      /**
       * Used to denote a solid, color fill of a graphical object.
       */
      FT_SOLID,
      /**
       * Used to denote a hashed, colored fill of a graphical object.
       */
      FT_HASHED
   };

   enum LineType
   {
      /**
       * Denotes a solid line type.
       */
      LT_SOLID,
      /**
       * Denotes a dashed line type
       */
      LT_DASHED
   };

   enum HighlightType
   {
      /**
       * Denotes a solid highlight method.
       */
      HT_SOLID,

      /**
       * Denotes a alpha-valued (see-through) highlight method.
       */
      HT_ALPHAED
   };
   
   enum OrientationType
   {
      /**
       * Specifies a horizontal orientation.
       */
      OT_HORIZONTAL,
      
      /**
       * Specifies a a vertical orientation.
       */
      OT_VERTICAL
   };
   
   enum ButtonStateType
   {
      /**
       * A button state where the button is up and not depressed.
       */
      BS_UP,

      /**
       * A button state where the button is down and in the depressed
       * state.
       */
      BS_DOWN
   };

   enum ExpandStateType
   {
      /**
       * A widget state where the widget only shows the things it
       * should show in an unexpanded state.
       */
      ES_UNEXPANDED,

      /**
       * A widget state where the widget is fully expanded to show all
       * it contains.
       */
      ES_EXPANDED
   };
   
   enum RenderOptionType
   {
      /**
       * A render option stating that all rendering should be done in
       * 2D and blitted to the image buffer.
       */
      RO_2D = 1,
      /**
       * A render option stating that all rendering should be done in
       * 2D and blitted to the selection buffer so OpenGL picking code
       * may post-process the rendering step and see which objects
       * were selected.
       */
      RO_2D_SELECT = 2,
      /**
       * A render option stating that all rendering should be done in
       * 3D and blitted to the image buffer.
       */
      RO_3D = 4,

      /**
       * A render option stating that all rendering should be done in
       * 3D and blitted to the selection buffer so OpenGL picking code
       * may post-process the rendering step and see which objects
       * were selected.
       */
      RO_3D_SELECT = 8
   };

   enum EventType
   {
      /**
       * An event noting that the user has pressed a mouse/pointer button.
       */
      E_MOUSE_PRESS,

      /**
       * An event noting that the user has released a mouse/pointer
       * button.
       */
      E_MOUSE_RELEASE,

      /**
       * An event noting that the user's mouse pointer has entered
       */
      E_MOUSE_ENTERED,

      /**
       * An event noting that the user's mouse pointer has exited
       */
      E_MOUSE_EXITED,

      /**
       * An event that denotes that the user has already pressed a
       * button, and is now dragging the pointer across the screen
       * with the mouse depressed
       */
      E_MOUSE_DRAG,

      /**
       * An event that specifies that the user does not have any mouse
       * button depressed, but is moving the pointer around the
       * screen.
       */
      E_MOUSE_MOVEMENT,

      /**
       * An event that specifies that the user is scrolling the mouse
       * wheel up or down.
       */
      E_MOUSE_WHEEL,

      /**
       * An event denoting that the user has pressed a key on the
       * keyboard.
       */
      E_KEY_PRESS,

      /**
       * An event noting that the user has released a previously
       * pressed key on the keyboard.
       */
      E_KEY_RELEASE,
   };

   enum FocusType
   {
      /**
       * The widget with active (primary) focus is in this state.
       */
      FT_ACTIVE,

      /**
       * Any widget that is focused but doesn't have primary focus
       * is in this state.
       */
      FT_PASSIVE,

      /**
       * Any widget that is trying to gain passive focus is in
       * this state until passive focus is granted.
       */
      FT_PASSIVE_PENDING,

      /**
       * All widgets that do not have any focus are in this state.
       */
      FT_NONE
   };
   
   enum JustificationType
   {
      /**
       * Left horizontal justification
       */
      JTH_LEFT = OGLFT::Face::LEFT,
      JTH_ORIGIN = OGLFT::Face::ORIGIN,
      JTH_CENTER = OGLFT::Face::CENTER,
      JTH_RIGHT = OGLFT::Face::RIGHT,
      JTH_FULL,
      JTV_BOTTOM = OGLFT::Face::BOTTOM,
      JTV_BASELINE = OGLFT::Face::BASELINE,
      JTV_MIDDLE = OGLFT::Face::MIDDLE,
      JTV_TOP = OGLFT::Face::TOP
   };

   enum MeasureModeType
   {
      MMT_EXACT,
      MMT_APPROXIMATE
   };

   enum GraphicButtonType
   {
      GB_DECAL,
      GB_GRAPHIC_ONLY
   };

   enum AnimationType
   {
      /**
       * The animation that plays when the visual object's
       * setVisible(true) method is called.
       */
      AT_APPEAR,

      /**
       * The animation that plays when the visual object's
       * setVisible(false) method is called.
       */
      AT_VANISH,

      /**
       * A looping animation that plays when the object is not being
       * edited/manipulated, etc.
       */
      AT_IDLE,

      /**
       * An animation that plays when the mouse moves over the visual object.
       */
      AT_MOUSE_OVER,

      /**
       * An animation that plays when the mouse leaves the visual
       * object, for example, on a defocus event.
       */
      AT_MOUSE_EXIT,

      /**
       * An animation that plays when the mouse is pressed on a visual
       * component.
       */
      AT_MOUSE_PRESS,

      /**
       * An animation that plays when the mouse is released over a
       * visual component.
       */
      AT_MOUSE_RELEASE,

      /**
       * An animation that plays when a keyboard key is depressed
       * while the visual component is focused.
       */
      AT_KEY_PRESS,

      /**
       * An animation that plays when a previously depressed keyboard
       * key is released while focused on a visual component.
       */
      AT_KEY_RELEASE,

      /**
       * An animation that plays when a visual component gains the
       * users focus. For example, when a mouse is pressed on a text entry
       * field.
       */
      AT_FOCUS_GAIN,

      /**
       * An animation that plays when a visual component loses the
       * users attention. For example, when a text entry field is
       * selected, and then the user pressed a button beside the text
       * entry field.
       */
      AT_FOCUS_LOOSE
   };

   enum FontType
   {
      /**
       * OGLFT filled polygonal font rendering type.
       */
      FT_P_FILLED,

      /**
       * OGLFT solid polygonal font rendering type.
       */
      FT_P_SOLID,

      /**
       * OGLFT outline polygonal font rendering type.
       */
      FT_P_OUTLINE,

      /**
       * OGLFT grayscale raster font rendering type.
       */
      FT_R_GRAYSCALE,

      /**
       * OGLFT monochrome raster font rendering type.
       */
      FT_R_MONOCHROME,

      /**
       * OGLFT translucent raster font rendering type.
       */
      FT_R_TRANSLUCENT,

      /**
       * OGLFT grayscale texture font rendering type.
       */
      FT_T_GRAYSCALE,

      /**
       * OGLFT monochrome texture font rendering type.
       */
      FT_T_MONOCHROME,

      /**
       * OGLFT translucent texture font rendering type.
       */
      FT_T_TRANSLUCENT,
   };
   
};

#endif
