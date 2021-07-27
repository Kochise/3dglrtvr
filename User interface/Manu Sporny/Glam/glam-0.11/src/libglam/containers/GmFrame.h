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
#ifndef GMFRAME_H
#define GMFRAME_H

#include <glam/GmDialog.h>
#include <glam/GmContentPane.h>
#include <glam/GmPanel.h>
#include <glam/GmGraphicButton.h>
#include <glam/GmIcon.h>
#include <glam/GmButtonListener.h>

/**
 * The GmFrame is a container that represents a window frame with
 * a main panel.
 */
class GmFrame : public GmDialog, public GmButtonListener
{
 protected:
   /**
    * The close button for this frame.
    */
   GmGraphicButton* mCloseButton;

   /**
    * The main container for this frame.
    */
   GmContentPane* mContentPanel;

   /**
    * The height of the title bar.
    */
   float mTitlebarHeight;

   /**
    * The width of the border of the frame.
    */
   float mFrameBorderWidth;

   /**
    * The relative size of the close button to the titlebar's height.
    */
   float mCloseButtonSize;

   /**
    * The offset from the edge of the frame for the close
    * button. Relative to the button size.
    */
   float mCloseButtonOffset;

   /**
    * Calculates the title bar height.
    */
   virtual void calculateTitlebarHeight();

   /**
    * Positions the close button on the title bar.
    */
   virtual void positionCloseButton();

   /**
    * Positions the content panel below the title bar.
    */
   virtual void positionContentPanel();
   
   /**
    * Positions the title text in the title bar.
    */
   virtual void positionTitleText();

   /**
    * Renders this dialog to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the dialog.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this dialog to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the dialog.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);

 public:
    /**
     * Standard constructor
     */
    GmFrame();

    /**
     * Virtual destructor
     */
    virtual ~GmFrame();

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
    * Removes a drawable component from the container.
    *
    * @param component the component to remove from the container.
    *
    * @return true if the component was removed, false if not
    */
   virtual bool removeComponent(GmDrawable* component);
   
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
    * Sets the content panel for this frame.
    *
    * @param panel the content panel to set
    */
   virtual void setContentPanel(GmContentPane* panel);

   /**
    * Gets the content panel for this frame.
    *
    * @return this frame's content panel
    */
   virtual GmContentPane* getContentPanel();

   /**
    * Sets the pressed icon for the close button.
    *
    * @param icon the pressed icon
    */
   virtual void setCloseButtonPressedIcon(GmIcon* icon);

   /**
    * Sets the unpressed icon for the close button.
    *
    * @param icon the unpressed icon
    */
   virtual void setCloseButtonUnpressedIcon(GmIcon* icon);

    /**
     * Sets the title text of this dialog to the given text.
     *
     * @param text the title text to set as this dialog's title bar
     */
   virtual void setTitleText(string titleText);

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
    * @param minZ the starting Z position of the constraint volume
    * @param width the width of the constraint volume
    * @param height the height of the constraint volume
    * @param depth the depth of the constraint volume
    */
   virtual void layoutInGLVolume(float minX, float minY, float minZ,
                                 float width, float height, float depth);   

   /**
    * Called whenever an button in this group is pressed.
    *
    * @param action the button action involved in the press.
    */
   virtual void buttonPressed(GmButtonAction* action);
};

#endif
