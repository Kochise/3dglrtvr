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
#include <limits.h>
#include <GL/glut.h>
#include <glam/GmTypes.h>
#include <glam/GmInteractiveWidget.h>
#include <glam/GmEventDistributor.h>
#include <glam/GmSvgIcon.h>

GmInteractiveWidget::GmInteractiveWidget()
      : GmAnimatedWidget()
{
   mFocusState = GmTypes::FT_NONE;
   mFocusTimestamp = INT_MAX;
   mFocusParent = NULL;
   mFocusOrder = 0;
   mFocusable = true;
   mResizeable = true;
   mMouseOver = false;

   mWidgetToolTip = NULL;
   mToolTipEnabled = true;

   // all interactive widgets register with the event distributor
   GmEventDistributor* ed = GmEventDistributor::getInstance(); 
   ed->registerWidget(this);
}

GmInteractiveWidget::~GmInteractiveWidget()
{
   if(mWidgetToolTip != NULL)
      delete mWidgetToolTip;

   setFocused(false);

   // unregister this widget from the event distributor
   GmEventDistributor* ed = GmEventDistributor::getInstance(); 
   ed->unregisterWidget(this);
}

void GmInteractiveWidget::fireFocusGained(GmFocusAction* action)
{
   // notify all focus listeners of the focus gain
   vector<GmFocusListener*>::iterator i;
   for(i = mFocusListeners.begin(); i != mFocusListeners.end(); i++)
      (*i)->focusGained(action);
}

void GmInteractiveWidget::fireFocusLost(GmFocusAction* action)
{
   // notify all focus listeners of the focus loss
   vector<GmFocusListener*>::iterator i;
   for(i = mFocusListeners.begin(); i != mFocusListeners.end(); i++)
      (*i)->focusLost(action);
}

void GmInteractiveWidget::notifyFocusParent()
{
   // notify the focus parent of the focus gain
   if(mFocusParent != NULL)
   {
      // if widget has no focus or pending, add it to the focus list
      if(mFocusState == GmTypes::FT_NONE ||
         mFocusState == GmTypes::FT_PASSIVE_PENDING)
      {
         // grant this widget passive focus
         mFocusState = GmTypes::FT_PASSIVE;

         // add this widget to the focus list
         GmEventDistributor* ed = GmEventDistributor::getInstance();
         ed->addFocusedWidget(this);
      }

      // notify the focus parent's focus parent
      mFocusParent->notifyFocusParent();
   }
}

void GmInteractiveWidget::focusGained(GmFocusAction* action)
{
   // pass focus action up
   fireFocusGained(action);
}

void GmInteractiveWidget::focusLost(GmFocusAction* action)
{
   // pass focus action up
   fireFocusLost(action);
}

void GmInteractiveWidget::setFocusParent(GmInteractiveWidget* fp)
{
   // update the focus parent
   mFocusParent = fp;

   // the focus parent is automatically a focus listener
   addFocusListener(fp);
}

void GmInteractiveWidget::addFocusListener(GmFocusListener* fl)
{
   mFocusListeners.push_back(fl);
}

bool GmInteractiveWidget::removeFocusListener(GmFocusListener* fl)
{
   vector<GmFocusListener*>::iterator i;
   for(i = mFocusListeners.begin(); i != mFocusListeners.end(); i++)
   {
      // compare the listeners
      if(*(*i) == *fl)
      {
         mFocusListeners.erase(i);
         return true;
      }
   }

   return false;
}

bool GmInteractiveWidget::setFocused()
{
   // get event distributor for focus handling, get the old focus
   GmEventDistributor* ed = GmEventDistributor::getInstance();
   GmInteractiveWidget* oldFocus = ed->getFocus();

   // handle previously focused widget
   if(oldFocus != NULL)
   {
      // get the focus list
      vector<GmInteractiveWidget*>* widgets = ed->getFocusList();

      // unfocus and erase while widget and parent are not front
      while(!widgets->empty() &&
            widgets->front()->getObjectId() != getObjectId())
      {
         if(mFocusParent != NULL &&
            widgets->front()->getObjectId() == mFocusParent->getObjectId())
            break;
         
         widgets->front()->mFocusState = GmTypes::FT_NONE;
         widgets->erase(widgets->begin());
      }

      // if the old focus is still set to active, set it to passive
      // pending; this occurs if the old active focus is the parent
      if(oldFocus->mFocusState == GmTypes::FT_ACTIVE)
         oldFocus->mFocusState = GmTypes::FT_PASSIVE_PENDING;

      // set the focus state of this widget to active
      mFocusState = GmTypes::FT_ACTIVE;

      // if the list is empty or the first focused widget is
      // not this one, then prefix it to the focus list
      if(widgets->empty() || widgets->front()->getObjectId() != getObjectId())
         ed->prefixFocusedWidget(this);

      // notify old focus of its focus change
      oldFocus->setFocused(false);
   }
   else
   {
      // set focus state of widget to active, prefix it to list
      mFocusState = GmTypes::FT_ACTIVE;
      ed->prefixFocusedWidget(this);
   }

   // notify this widget of its focus change
   return setFocused(true);
}

bool GmInteractiveWidget::setUnFocused()
{
   // get event distributor for focus handling, remember old focus
   GmEventDistributor* ed = GmEventDistributor::getInstance();
   GmInteractiveWidget* oldFocus = ed->getFocus();

   // get the list of currently focused widgets
   vector<GmInteractiveWidget*>* widgets = ed->getFocusList();

   // unfocus and pop off widgets before and including this one
   bool found = false;
   while(!widgets->empty() && !found)
   {
      if(widgets->front()->getObjectId() == getObjectId())
         found = true;

      widgets->front()->mFocusState = GmTypes::FT_NONE;
      widgets->erase(widgets->begin());
   }

   // search for the next actively focusable widget
   while(!widgets->empty() && !widgets->front()->getFocusable())
   {
      // unfocus and pop off widget
      widgets->front()->mFocusState = GmTypes::FT_NONE;
      widgets->erase(widgets->begin());
   }

   // if widgets still remain, get the new focus
   GmInteractiveWidget* newFocus = NULL;
   if(!widgets->empty())
   {
      // get new focus and set its state to active
      newFocus = widgets->front();
      newFocus->mFocusState = GmTypes::FT_ACTIVE;
   }

   // notify old focus of its focus change(after setting up new focus)
   oldFocus->setFocused(false);

   // notify new focus of its focus change
   if(newFocus != NULL)
      newFocus->setFocused(true);
   
   return true;
}

bool GmInteractiveWidget::setFocused(bool focus)
{
   // focused is already set false or cannot be set
   if(!getFocusable())
      return !focus;

   if(focus)
   {
      // already set to focused, but notify listeners
      if(mFocusState == GmTypes::FT_ACTIVE)
      {
         // notify focus parent of focus gain so updates its state
         notifyFocusParent();
         
         // send focus action to listeners
         GmFocusAction* action = new GmFocusAction(getWidgetId(),
                                                   getFocusOrder());
         fireFocusGained(action);
         delete action;

         return true;
      }

      return setFocused();
   }
   else
   {
      // already set to unfocused, but notify listeners
      if(mFocusState == GmTypes::FT_NONE ||
         mFocusState == GmTypes::FT_PASSIVE_PENDING)
      {
         // grant passive status to pending widgets
         if(mFocusState == GmTypes::FT_PASSIVE_PENDING)
            mFocusState = GmTypes::FT_PASSIVE;
         
         // send the focus action to listeners
         GmFocusAction* action = new GmFocusAction(getWidgetId(),
                                                   getFocusOrder());
         fireFocusLost(action);
         delete action;

         return true;
      }

      return setUnFocused();
   }
}

GmTypes::FocusType GmInteractiveWidget::getFocusState()
{
   return mFocusState;
}

bool GmInteractiveWidget::hasFocus()
{
   return mFocusState == GmTypes::FT_ACTIVE;
}

void GmInteractiveWidget::setFocusOrder(int order)
{
   mFocusOrder = order;
}

int GmInteractiveWidget::getFocusOrder()
{
   return mFocusOrder;
}

bool GmInteractiveWidget::getFocusable()
{
   return mFocusable;
}

void GmInteractiveWidget::setFocusable(bool focusable)
{
   // if setting unfocusable, remove current focus, if any
   if(!focusable)
      setFocused(false);
   
   mFocusable = focusable;
}

void GmInteractiveWidget::fireMinimizeOccurred(GmGeometryAction* action)
{
   vector<GmGeometryListener*>::iterator i;
   for(i = mGeometryListeners.begin(); i != mGeometryListeners.end(); i++)
      (*i)->minimizeOccurred(action);
}

void GmInteractiveWidget::fireMaximizeOccurred(GmGeometryAction* action)
{
   vector<GmGeometryListener*>::iterator i;
   for(i = mGeometryListeners.begin(); i != mGeometryListeners.end(); i++)
      (*i)->maximizeOccurred(action);
}

void GmInteractiveWidget::fireGeometryResize(GmGeometryAction* action)
{
   vector<GmGeometryListener*>::iterator i;
   for(i = mGeometryListeners.begin(); i != mGeometryListeners.end(); i++)
      (*i)->geometryResized(action);
}

void GmInteractiveWidget::addGeometryListener(GmGeometryListener* gl)
{
   mGeometryListeners.push_back(gl);
}

bool GmInteractiveWidget::removeGeometryListener(GmGeometryListener* gl)
{
   vector<GmGeometryListener*>::iterator i;
   for(i = mGeometryListeners.begin(); i != mGeometryListeners.end(); i++)
   {
      // compare the listeners
      if(*(*i) == *gl)
      {
         mGeometryListeners.erase(i);
         return true;
      }
   }

   return false;
}

bool GmInteractiveWidget::getResizeable()
{
   return mResizeable;
}

void GmInteractiveWidget::setResizeable(bool resizeable)
{
   mResizeable = resizeable;
}

bool GmInteractiveWidget::getMouseOver()
{
   return mMouseOver;
}

void GmInteractiveWidget::setToolTipText(string text)
{
   if(mWidgetToolTip == NULL)
      mWidgetToolTip = new GmToolTip();
         
   mWidgetToolTip->setText(text);
}

string GmInteractiveWidget::getToolTipText()
{
   return mWidgetToolTip->getText();
}

void GmInteractiveWidget::setToolTipTextSize(float size)
{
   if(mWidgetToolTip == NULL)
      mWidgetToolTip = new GmToolTip();

   mWidgetToolTip->setTextSize(size);
}

void GmInteractiveWidget::setToolTipEnabled(bool enabled)
{
   mToolTipEnabled = enabled;
}

bool GmInteractiveWidget::getToolTipEnabled()
{
   return mToolTipEnabled;
}

void GmInteractiveWidget::mousePressEvent(GmEvent* event)
{
   setFocused(true);
}

void GmInteractiveWidget::mouseReleaseEvent(GmEvent* event)
{
}

void GmInteractiveWidget::mouseMoveEvent(GmEvent* event)
{
}

void GmInteractiveWidget::mouseEnteredEvent(GmEvent* event)
{
   // TEMP CODE: temp for tooltip display
   mFocusTimestamp = glutGet(GLUT_ELAPSED_TIME);
   mMouseOver = true;

   // set the tool tip's position to under where the mouse
   // enters the widget
   if(mWidgetToolTip != NULL)
   {
      // z position is 0.0, tooltips always on top
      mWidgetToolTip->setGLPosition(event->getMouseData()->GLx,
                                    event->getMouseData()->GLy,
                                    0.0);
   }
}

void GmInteractiveWidget::mouseExitedEvent(GmEvent* event)
{
   // TEMP CODE: temp for removing tooltip display
   mFocusTimestamp = INT_MAX;
   mMouseOver = false;
}

void GmInteractiveWidget::mouseDragEvent(GmEvent* event)
{
}

void GmInteractiveWidget::wheelMoveEvent(GmEvent* event)
{
}

void GmInteractiveWidget::keyPressEvent(GmEvent* event)
{
   // pass the key press event to the focus parent by default
   if(mFocusParent != NULL)
      mFocusParent->keyPressEvent(event);
}

void GmInteractiveWidget::keyReleaseEvent(GmEvent* event)
{
}

void GmInteractiveWidget::render2dToolTip(GmTypes::RenderOptionType
                                          renderOptions)
{
   if((mToolTipEnabled) && (mWidgetToolTip != NULL) &&
      ((glutGet(GLUT_ELAPSED_TIME) - 1000) > (int)mFocusTimestamp))
   {
      mWidgetToolTip->render(renderOptions);
   }
}

void GmInteractiveWidget::render2d(GmTypes::RenderOptionType
                                   renderOptions)
{
   GmDrawable::render2d(renderOptions);
   render2dToolTip(renderOptions);
}
