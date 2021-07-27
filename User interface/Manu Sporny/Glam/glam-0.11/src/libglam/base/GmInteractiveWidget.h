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
#ifndef GMINTERACTIVEWIDGET_H
#define GMINTERACTIVEWIDGET_H

#include <glam/GmAnimation.h>
#include <glam/GmAnimatedWidget.h>
#include <glam/GmDragableWidget.h>
#include <glam/GmEvent.h>
#include <glam/GmFocusListener.h>
#include <glam/GmGeometryListener.h>
#include <glam/GmDropListener.h>
#include <glam/GmToolTip.h>
#include <glam/GmIcon.h>

class GmWidgetAction;

/**
 * The interactive widget class is the base class for all widgets that
 * will interact with the user.
 */
class GmInteractiveWidget :
   public GmAnimatedWidget, public GmDragableWidget,
   public GmDropListener, public GmFocusListener
{
 protected:
   /**
    * Description
    */
   GmAnimation* mMouseOverAnim;
   
   /**
    * Description
    */
   GmAnimation* mMouseExitAnim;

   /**
    * The animation that is played whenever the mouse is pressed on
    * this interactive widget.
    */
   GmAnimation* mMousePressAnim;

   /**
    * Description
    */
   GmAnimation* mMouseReleaseAnim;

   /**
    * Description
    */
   GmAnimation* mKeyPressAnim;

   /**
    * Description
    */
   GmAnimation* mKeyReleaseAnim;

   /**
    * Description
    */
   GmAnimation* mFocusGainAnim;

   /**
    * Description
    */
   GmAnimation* mFocusLoseAnim;

   /**
    * The focus state of this interactive widget.
    */
   GmTypes::FocusType mFocusState;

   /**
    * True if the mouse if over this widget, false if not.
    */
   bool mMouseOver;

   /**
    * The time at which the last focus occurred. This value is used to
    * determine if a tooltip should be shown.
    */
   unsigned int mFocusTimestamp;

   /**
    * The tooltip that is associated with this interactive widget.
    */
   GmToolTip* mWidgetToolTip;

   /**
    * Determines if the tooltip will be displayed.
    */
   bool mToolTipEnabled;

   /**
    * The focus parent of this widget. Whenever this widget
    * gains active or passive focus, its parent gains passive focus.
    * Whenever this widget loses active focus, its parent
    * either gains active focus or loses passive focus.
    */
   GmInteractiveWidget* mFocusParent;

   /**
    * A vector of focus listeners. Any widgets that implements the
    * GmFocusListener interface can listen to this widget's
    * focus actions.
    */
   vector<GmFocusListener*> mFocusListeners;

   /**
    * The focus order of this widget.
    */
   int mFocusOrder;

   /**
    * Determines whether this widget can be focused or not.
    */
   bool mFocusable;

   /**
    * The vector of geometry listeners that listen to this widget.
    */
   vector<GmGeometryListener*> mGeometryListeners;

   /**
    * Determines if this widget is resizable or not.
    */
   bool mResizeable;

   /**
    * Fires a focus action to the listeners when focus is gained.
    *
    * @param action the action to pass to the listener
    */
   virtual void fireFocusGained(GmFocusAction* action);

   /**
    * Fires a focus action to the listeners when focus is lost.
    *
    * @param action the action to pass to the listener
    */
   virtual void fireFocusLost(GmFocusAction* action);

   /**
    * Notifies the focus parent of a focus gain so that
    * it will change its state to FT_PASSIVE and notify
    * its own focus parent of the focus gain.
    */
   virtual void notifyFocusParent();
   
   /**
    * Sets the widget to actively focused status and passes action.
    *
    * @return true if focused, false if not focused.
    */
   virtual bool setFocused();

   /**
    * Sets widget to have no focus.
    *
    * @return true if widget has no focus, false if it still does.
    */
   virtual bool setUnFocused();

   /**
    * Fires a geometry action to all listeners when a minimize occurs.
    *
    * @param action the action to pass to the listeners
    */
   virtual void fireMinimizeOccurred(GmGeometryAction* action);

   /**
    * Fires a geometry action to all listeners when a maximize occurs.
    *
    * @param action the action to pass to the listeners
    */
   virtual void fireMaximizeOccurred(GmGeometryAction* action);

   /**
    * Fires a geometry resize action to all listeners when size is changed.
    *
    * @param action the action to pass to the listeners
    */
   virtual void fireGeometryResize(GmGeometryAction* action);   
   
 public:
   /**
    * Standard constructor
    */
   GmInteractiveWidget();

   /**
    * Virtual destructor
    */
   virtual ~GmInteractiveWidget();

   /**
    * Sets the tool tip text for this interactive widget.  If
    * a user input device hovers over the interactive widget for more
    * than a specified amount of time, a textual help message appears
    * above the interactive widget.
    *
    * @param the tooltip text to use
    */
   virtual void setToolTipText(string text);

   /**
    * Gets the tooltip text for this widget.
    *
    * @return the tooltip text.
    */
   virtual string getToolTipText();

   /**
    * Sets the size of the tooltip text.
    *
    * @param size the size of the tooltip text.
    */
   virtual void setToolTipTextSize(float size);

   /**
    * Sets the state of the tooltip to enabled
    * or disabled. If enabled, the tooltip will
    * render, if not, it will not.
    */
   virtual void setToolTipEnabled(bool enable);

   /**
    * Returns true if the tooltip is enabled,
    * false if not.
    */
   virtual bool getToolTipEnabled();
   
   /**
    * This method is called when this widget receives
    * a mouse press event.
    */
   virtual void mousePressEvent(GmEvent* event);

   /**
    * This method is called when this widget receives
    * a mouse release event.
    */
   virtual void mouseReleaseEvent(GmEvent* event);

   /**
    * This method is called when this widget receives
    * a mouse entered event.
    */
   virtual void mouseEnteredEvent(GmEvent* event);

   /**
    * This method is called when this widget receives
    * a mouse exited event.
    */
   virtual void mouseExitedEvent(GmEvent* event);

   /**
    * This method is called when this widget receives
    * a mouse move event.
    */
   virtual void mouseMoveEvent(GmEvent* event);
   
   /**
    * This method is called when this widget receives
    * a mouse drag event.
    *
    * @param event the mouse drag event for this object to process
    */
   virtual void mouseDragEvent(GmEvent* event);

   /**
    * This method is called when this widget receives
    * a mouse wheel event.
    */
   virtual void wheelMoveEvent(GmEvent* event);

   /**
    * This method is called when this widget receives
    * a key press event.
    */
   virtual void keyPressEvent(GmEvent* event);

   /**
    * This method is called when this widget receives
    * a key release event.
    */
   virtual void keyReleaseEvent(GmEvent* event);

   /* Interface for GmFocusListener */

   /**
    * Called whenever a widget gains focus.
    *
    * @param action the action associated with the focus.
    */
   virtual void focusGained(GmFocusAction* action);
   
   /**
    * Called whenever a widget loses focus.
    *
    * @param action the action associated with the focus.
    */
   virtual void focusLost(GmFocusAction* action);
   
   /* End Interface for GmFocusListener */   

   /**
    * Sets the focus parent of this widget. This focus
    * parent is automatically a focus listener to its
    * focus children. If the focus parent is changed,
    * and the old focus parent does not want to receive
    * any focus actions any longer, it must remove itself
    * from the child's focus listener list by calling
    * removeFocusListener().
    *
    * @param fm the interactive widget to use as this
    *        widget's focus parent.
    */
   virtual void setFocusParent(GmInteractiveWidget* fm);

   /**
    * Adds a focus listener to this widget.
    *
    * @param fl the focus listener to add to this widget.
    */
   virtual void addFocusListener(GmFocusListener* fl);

   /**
    * Removes a focus listener from this widget.
    *
    * @param fl the focus listener to remove.
    * @return true if the listener was removed, false if not.
    */
   virtual bool removeFocusListener(GmFocusListener* fl);

   /**
    * Sets the focus of this widget.
    *
    * @bool focus the focus to set.
    * @bool true if set, false if not.
    */
   virtual bool setFocused(bool focus);

   /**
    * Gets the focus state of this widget.
    *
    * @return the focus state of this widget.
    */
   virtual GmTypes::FocusType getFocusState();

   /**
    * Returns true if the widget has active focus,
    * false if not.
    */
   virtual bool hasFocus();
 
   /**
    * Sets the focus order of this widget.
    *
    * @order the order to set.
    */
   virtual void setFocusOrder(int order);
  
   /**
    * Gets the focus order of this widget.
    *
    * @return the focus order of this widget.
    */
   virtual int getFocusOrder();

   /**
    * Sets the focusable state of this widget.
    *
    * @focusable true for focusable, false for not.
    */
   virtual void setFocusable(bool focusable);

   /**
    * Gets the focusable status of this widget.
    *
    * @return true if focusable, false if not.
    */
   virtual bool getFocusable();

   /**
    * Adds a geometry listener to this widget.
    *
    * @param gl a geometry listener that will listen to this menu.
    */
   virtual void addGeometryListener(GmGeometryListener* gl);

   /**
    * Removes a geometry listener from this widget.
    *
    * @param gl the geometry listener to remove.
    * @return true if the listener was removed, false if not.
    */
   virtual bool removeGeometryListener(GmGeometryListener* gl);

   /**
    * Sets the resize state of this widget.
    *
    * @focusable true for resizeable, false for not.
    */
   virtual void setResizeable(bool focusable);

   /**
    * Gets the resizeable status of this widget.
    *
    * @return true if resizeable, false if not.
    */
   virtual bool getResizeable();

   /**
    * Gets whether the mouse is currently over this widget or not.
    *
    * @return true if mouse is over the widget, false if not.
    */
   virtual bool getMouseOver();

   /**
    * This is a convenience method to render a tooltip if the given
    * tooltip pop-up time has occurred.
    */
   virtual void render2dToolTip(GmTypes::RenderOptionType renderOptions);
   
 protected:
   /**
    * Renders this drawable to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the drawable.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);   
};

#endif
