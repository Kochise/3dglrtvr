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
#ifndef GMDRAWABLE_H
#define GMDRAWABLE_H

#include <glmath/GlmPoint3.h>
#include <glmath/GlmColor4.h>
#include <glam/GmTypes.h>
#include <glam/GmObject.h>

/**
 * The GmDrawable class is the base class for all render-able
 * objects. It contains basic information such as the height and width
 * of the drawable object as well as whether it is visible or not. 
 */
class GmDrawable: public virtual GmObject
{
 protected:
   /**
    * The base color of the drawable object.
    */
   GlmColor4 mBaseColor;

   /**
    * The position of the drawable object. The value
    * should be from 1-100% (0.0 to 1.0) and will be relative to the
    * draw-able's parent container.
    */
   GlmPoint3 mPosition;

   /**
    * The physical width of the drawable object in units. The value
    * should be from 1-100% (0.0 to 1.0) and will be relative to the
    * drawable's parent container.
    */
   float mWidth;

   /**
    * The physical height of the drawable object in units. The value
    * should be from 1-100% (0.0 to 1.0) and will be relative to the
    * drawable's parent container.
    */
   float mHeight;
   
   /**
    * The physical depth of the drawable object in units. The value
    * should be from 1-100% (0.0 to 1.0) and will be relative to the
    * drawable's parent container.
    */
   float mDepth;

   /**
    * The OpenGL position of this object. These are the OpenGL
    * coordinates. They differ from the mPosition coordinates because
    * this information is used by the rendering method. The mPosition
    * object can be in either an absolute or relative coordinate
    * referencing scheme, where mGLPosition is always absolute world
    * coordinates.
    */
   GlmPoint3 mGLPosition;

   /**
    * The width of this drawable in OpenGL units. This value differs
    * from the mWidth variable in that it is used by the rendering
    * method to place the drawable in OpenGL world coordinates. The
    * mWidth variable can be in absolute or relative coordinates
    * whereas this value is always in absolute world coordinates.
    */
   float mGLWidth;

   /**
    * The height of this drawable in OpenGL units. This value differs
    * from the mHeight variable in that it is used by the rendering
    * method to place the drawable in OpenGL world coordinates. The
    * mHeight variable can be in absolute or relative coordinates
    * whereas this value is always in absolute world coordinates.
    */   
   float mGLHeight;

   /**
    * The depth of this drawable in OpenGL units. This value differs
    * from the mDepth variable in that it is used by the rendering
    * method to place the drawable in OpenGL world coordinates. The
    * mDepth variable can be in absolute or relative coordinates
    * whereas this value is always in absolute world coordinates.
    */   
   float mGLDepth;
   
   /**
    * The visible status of the object, if true the drawable is
    * visible and will be rendered. If the status if false, the
    * drawable is invisible and will not be rendered to the screen.
    */
   bool mVisible;

   /**
    * The widget ID to use as this widget's custom
    * identifier. This widget ID is used by the various event
    * handlers for the widget to figure out which action to execute.
    *
    * Note: This must be defined in GmDrawable rather than
    * GmInteractiveWidget to resolve the circular definition
    * between GmWidgetAction and GmInteractiveWidget.
    */
   WidgetIDType mWidgetId;

   /**
    * An OpenGL-assigned displaylist id for this object. Defaults
    * to GL_ZERO (i.e. no displaylist id).
    */
   GLuint mDisplayListId;
   
   /**
    * A flag to inform render methods of their displaylist's state.
    * false = the diplaylist does not need to be generated
    * true = the displaylist needs to be generated
    */
   bool mDisplayListValid;

   /**
    * This data structure allows drawables to lay themselves out
    * using the parameters that were passed in their last
    * layout call.
    */
   struct LastLayout
   {
      /**
       * The minX parameter from the last layout call.
       */
      float minX;

      /**
       * The minY parameter from the last layout call.
       */
      float minY;

      /**
       * The minZ parameter from the last layout call.
       */
      float minZ;

      /**
       * The width parameter from the last layout call.
       */
      float width;

      /**
       * The height parameter from the last layout call.
       */
      float height;

      /**
       * The depth parameter from the last layout call.
       */
      float depth;
   } mLastLayout;

  public:
   /**
    * Standard constructor.
    */
   GmDrawable();

   /**
    * Standard destructor.
    */
   virtual ~GmDrawable();

   /**
    * Renders the current drawable given the rendering options. Some
    * rendering options that can be implemented are 2D/3D rendering,
    * selection mode rendering as well as many other types of
    * rendering styles and methods.
    *
    * @param renderOptions the rendering options to use when rendering
    *                      the drawable to the screen or image buffer
    */
   virtual void render(GmTypes::RenderOptionType renderOptions);

   /**
    * Sets the GL position of the drawable.
    *
    * @param x the X position of the drawable object
    * @param y the Y position of the drawable object
    * @param z the Z position of the drawable object
    */
   virtual void setGLPosition(float x, float y, float z);

   /**
    * Sets the GL position of the drawable.
    *
    * @param point the point to use as the rendering location
    *              of the drawable
    */
   virtual void setGLPosition(GlmPoint3* point);
   
   /**
    * Sets the position of the drawable. The value should be from 0%-100%
    * (0.0 to 1.0) and will be a relative to the drawable's parent container.
    *
    * @param x the X position of the drawable object
    * @param y the Y position of the drawable object
    * @param z the Z position of the drawable object
    */
   virtual void setPosition(float x, float y, float z);

   /**
    * Sets the position of the drawable. The value should be from 0%-100%
    * (0.0 to 1.0) and will be a relative to the drawable's parent container.
    *
    * @param point the point to use as the top-left rendering location
    *              of the drawable.
    */
   virtual void setPosition(GlmPoint3* point);

   /**
    * Gets the position of the drawable object.
    *
    * @return the position of the drawable object
    */
   virtual GlmPoint3* getPosition();

   /**
    * Gets the OpenGL position of the drawable object.
    *
    * @return the position of the drawable object
    */
   virtual GlmPoint3* getGLPosition();

   /**
    * Gets the OpenGL position of the drawable object.
    *
    * @param x the float to store the x coordinate in
    * @param y the float to store the y coordinate in
    * @param z the float to store the z coordinate in
    */
   virtual void getGLPosition(float &x, float &y, float &z);

   /**
    * Sets the base color of the object. 
    *
    * @param r the red value 
    * @param g the green value 
    * @param b the blue value
    * @param a the alpha value 
    */
   virtual void setBaseColor(float r, float g, float b, float a);
   
   /**
    * Sets the base color of the object.
    *
    * @param color the base color
    */
   virtual void setBaseColor(GlmColor4* color);

   /**
    * Gets the base color of the object.
    *
    * @return the base color of the object, THIS IS A BORROWED
    *         POINTER, DO NOT DELETE
    */
   virtual GlmColor4* getBaseColor();

   /**
    * Gets the base color of the object.
    *
    * @param r float to store red value
    * @param g float to store green value
    * @param b float to store blue value
    * @param a float to store alpha value
    */
   virtual void getBaseColor(float &r, float &g, float &b, float &a);

   /**
    * Sets the width of the drawable object in units. The value
    * should be from 0%-100% (0.0 to 1.0) and will be a relative to
    * the drawable's parent container.
    *
    * @param width the width to set the drawable object's width to.
    */
   virtual void setWidth(float width);

   /**
    * Gets the width of the drawable object.
    *
    * @return the width of the drawable object.
    */
   virtual float getWidth();

   /**
    * Sets the OpenGL width of the drawable object.
    *
    * @param width the width to set
    */
   virtual void setGLWidth(float width);
   
   /**
    * Gets the OpenGL width of the drawable object.
    *
    * @return the width of the drawable object.
    */
   virtual float getGLWidth();

   /**
    * Sets the height of the drawable object in units. The value
    * should be from 0%-100% (0.0 to 1.0) and will be a relative to
    * the drawable's parent container.
    *
    * @param height the height to set the drawable object's height to.
    */
   virtual void setHeight(float height);

   /**
    * Gets the height of the drawable object.
    *
    * @return the height of the drawable object.
    */
   virtual float getHeight();

   /**
    * Sets the OpenGL height of the drawable object.
    *
    * @param height the height to set.
    */
   virtual void setGLHeight(float height);

   /**
    * Gets the OpenGL height of the drawable object.
    *
    * @return the height of the drawable object.
    */
   virtual float getGLHeight();

   /**
    * Gets the ideal extents of the drawable in OpenGL units. This is
    * sometimes used by parent containers to determine the optimal
    * layout strategy by taking each components extents into account.
    *
    * @param width the width of the drawable in OpenGL units
    * @param height the height of the drawable in OpenGL units
    */
   virtual void getGLExtents(float& width, float& height);
   
   /**
    * Sets the depth of the drawable object in units. The value
    * should be from 0%-100% (0.0 to 1.0) and will be a relative to
    * the drawable's parent container.
    *
    * @param depth the depth to set the drawable object's depth to.
    */
   virtual void setDepth(float depth);
   
   /**
    * Gets the depth of the drawable object.
    *
    * @return the depth of the drawable object.
    */
   virtual float getDepth();

   /**
    * Sets the OpenGL depth of the drawable object.
    *
    * @param depth the depth to set
    */
   virtual void setGLDepth(float depth);
   
   /**
    * Gets the OpenGL depth of the drawable object.
    *
    * @return the depth of the drawable object.
    */
   virtual float getGLDepth();

   /**
    * Sets the visibility of the drawable object. If the visibility is
    * set to false, the object is not drawn. If the visibility is set
    * to true, the object will be drawn.
    *
    * @param visible the visibility status to set this drawable to
    */
   virtual void setVisible(bool visible);

   /**
    * Gets the visibility status of this drawable object. If true, the
    * object should be drawn, if false, the object should not be
    * drawn.
    *
    * @return the visibility of this drawable object
    */
   virtual bool getVisible();

   /**
    * Gets the focusable status of this drawable object. This will
    * always be false if the object is strictly a GmDrawable.
    */
   virtual bool getFocusable();

   /**
    * Sets the widget ID associated with this interactive widget.
    *
    * @param name the widget id of this interactive widget.
    */
   virtual void setWidgetId(WidgetIDType name);
   
   /**
    * Gets the widget ID associated with this interactive widget.
    *
    * @return the widget ID that is associated with this widget
    */
   virtual WidgetIDType getWidgetId();

   /**
    * Sets the relative geometry to use for this drawable object. This
    * method is provided merely for convenience.
    *
    * @param minX the relative min X value for this drawable
    * @param minY the relative min Y value for this drawable
    * @param minZ the relative min Z value for this drawable
    * @param maxX the relative max X of this drawable
    * @param maxY the relative max Y of this drawable
    * @param maxZ the relative max Z of this drawable
    */
   virtual void setRelativeGeometry(float minX, float minY, float minZ,
                                    float maxX, float maxY, float maxZ);

   /**
    * Sets the relative geometry to use for this drawable object. This
    * method is provided merely for convenience. This method should be
    * used for 2d objects that will be in a container that uses a
    * layout manager.
    *
    * @param minX the relative min X value for this drawable
    * @param minY the relative min Y value for this drawable
    * @param maxX the relative max X of this drawable
    * @param maxY the relative max Y of this drawable
    */
   virtual void setRelativeGeometry(float minX, float minY,
                                    float maxX, float maxY);
   
   /**
    * Repositions and resizes this component based on parameters
    * passed into the method. These parameters are usually passed in
    * by the parent class.
    *
    * @param minX the starting X position of the constraint volume
    * @param minY the starting Y position of the constraint volume
    * @param width the width of the constraint volume
    * @param height the height of the constraint volume
    * @param layer the layer the component should be placed in
    */
   void layoutInGLArea(float minX, float minY,
                       float width, float height,
                       float layer);

   /**
    * Repositions and resizes this component based on parameters
    * passed into the method. These parameters are usually passed in
    * by the parent class.
    *
    * @param minX the starting X position of the constraint volume
    * @param minY the starting Y position of the constraint volume
    * @param minZ the starting Z position of the constraint volume
    * @param width the width of the constraint volume
    * @param height the height of the constraint volume
    * @param depth the depth of the constraint volume
    */
   virtual void layoutInGLVolume(float minX, float minY, float minZ,
                                 float width, float height, float depth);

   /**
    * Lays this component out using the parameters passed to its
    * last call to layoutInGL*().
    */
   virtual void relayout();
   
 protected:
   /**
    * Renders this drawable to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the drawable.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this drawable to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the drawable.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);

   /**
    * Sets the displaylist id for this component. Child objects
    * then have the choice of creating/using their own displaylist.
    *
    * @param id the OpenGL assigned displaylist id
    */
   virtual void setDisplayListId(int id);

   /**
    * Gets the current displaylist id for the object. If one exists,
    * it is returned and the child object use it for rendering. If
    * not, -1 is returned.
    *
    * @return the displaylist id if it exists, -1 if not
    */
   virtual int getDisplayListId();

   /**
    * Sets the flag which informs the render functions that
    * their display list needs to be generated.
    *
    * @param valid true to generate, false to not
    */
   virtual void setDisplayListValid(bool valid);

   /**
    * Gets the flag which informs the render functions that
    * their display list needs to be generated.
    *
    * @return true if generation needed, false if not
    */
   virtual bool getDisplayListValid();
};

#endif
