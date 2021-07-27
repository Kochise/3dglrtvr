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
#ifndef GMCONTAINER_H
#define GMCONTAINER_H

#include <glam/GmTypes.h>
#include <glam/GmInteractiveWidget.h>
#include <glam/GmLayoutManager.h>
#include <glam/GmEventDistributor.h>

/**
 * A container is a widget that encapsulates other widgets.
 */
class GmContainer : public GmInteractiveWidget
{
 protected:
   /**
    * The components that this container encapsulates.
    */
   vector<GmDrawable*> mComponents;

   /**
    * The associated layout manager for this container.
    */
   GmLayoutManager* mLayoutManager;
   
   /**
    * Applies this container's layout to the components that it
    * encapsulates using default boundaries.
    */
   virtual void applyLayout();

   /**
    * Applies this container's layout to the components that it
    * encapsulates using default boundaries plus the passed
    * relative insets (relative to width and height 0.0 to 1.0).
    *
    * @param li the relative left inset.
    * @param ri the relative right inset.
    * @param ti the relative top inset.
    * @param bi the relative bottom inset.
    */
   virtual void applyLayout(float li, float ri, float ti, float bi);

 public:
   /**
    * Standard constructor.
    */
   GmContainer();

   /**
    * Virtual destructor.
    */
   virtual ~GmContainer();

   /**
    * Adds a drawable component to the container.
    *
    * @param component the component to add to the container. DO NOT
    *                  DELETE THIS ARGUMENT AFTER THE METHOD CALL. The
    *                  container doesn't make a copy, rather it uses
    *                  the object that is passed in directly. This
    *                  class will handle the components finalization/deletion.
    */
   virtual void addComponent(GmDrawable* component);

   /**
    * Adds an interactive component. This ensures that this container
    * will be the focus parent for the interactive component.
    */
   virtual void addComponent(GmInteractiveWidget* component);

   /**
    * Adds a drawable component at index. If the passed index is
    * greater than the number of components currently in the
    * container, the component will NOT be added to the container.
    *
    * @return bool true or false if the insertion is successful.
    * @param component the component to add.
    * @param index the index at which to add the component.
    */
   virtual bool insertComponent(GmDrawable* component, int index);

   /**
    * Adds a drawable component at index. If the passed index is
    * greater than the number of components currently in the
    * container, the component will NOT be added to the container.
    *
    * @param component the component to add.
    * @param index the index at which to add the component.
    *
    * @return true if the insertion is successful, false if not.
    */
   virtual bool insertComponent(GmInteractiveWidget* component, int index);

   /**
    * Removes a drawable component from the container.
    *
    * @param component the component to remove from the container.
    * @param destroy true to destroy the component, false to leave alone.
    *
    * @return true if the component was removed, false if not.
    */
   virtual bool removeComponent(GmDrawable* component, bool destroy);

   /**
    * Removes a drawable component from the container and deletes it.
    *
    * @param component the component to remove from the container.
    *
    * @return true if the component was removed, false if not.
    */
   virtual bool removeComponent(GmDrawable* component);

   /**
    * Empties the container by removing all components and deleting
    * them if the destroy flag is set to true.
    *
    * @param destroy true to destroy the components, false to leave alone.
    */
   virtual void removeAllComponents(bool destroy);

   /**
    * Empties the container by removing and deleting all of its components.
    */
   virtual void removeAllComponents();

   /**
    * Event processing function to process a key press event.
    */
   virtual void keyPressEvent(GmEvent* event);
   
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
    *              of the drawable.
    */
   virtual void setGLPosition(GlmPoint3* point);

   /**
    * Translate the GL positions of all of the components by
    * x, y, z.
    *
    * @param x the amount to translate in the x direction
    * @param y the amount to translate in the y direction
    * @param z the amount to translate in the z direction
    */
   virtual void translateComponentsGLPositions(float x, float y, float z);

   /**
    * Translate the GL positions of all of the components by
    * the given point.
    *
    * @param point the amount to translate
    */
   virtual void translateComponentsGLPositions(GlmPoint3* point);

   /**
    * Repositions and resizes this component based on parameters
    * passed into the method.
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
    * Sets this container's associated layout manager.
    *
    * @param layoutManager the new layout manager for this container
    */
   void setLayout(GmLayoutManager *layoutManager);

   /**
    * Gets this container's associated layout manager.
    *
    * @return this container's layout manager    
    */
   GmLayoutManager *getLayout();

   /**
    * Gets the number of components in this container.
    *
    * @return the number of components in this container.
    */
   int getNumComponents();

   /**
    * Given an (x,y) world coordinate, this attempts to find
    * a contained component at that location. If one is found,
    * its index is returned. If not, -1 is returned.
    *
    * @param x the to-be-matched x coordinate
    * @param y the to-be-matched y coordinate
    * @return if found, the index of the target component, if not, -1
    */
   virtual int findComponent(float x, float y);
   
   /**
    * Gets the ideal extents of the drawable in OpenGL units. This is
    * sometimes used by parent containers to determine the optimal
    * layout strategy by taking each components extents into account.
    *
    * @param width the width of the drawable in OpenGL units
    * @param height the height of the drawable in OpenGL units
    */
   virtual void getGLExtents(float& width, float& height);

 protected:
   /**
    * Renders this container and all of its sub-components to a 2D
    * image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the container.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this container and all of its sub-components to a
    * selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the container.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
