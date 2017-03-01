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
#ifndef GMHYPERLINKACTION_H
#define GMHYPERLINKACTION_H

#include <glam/GmWidgetAction.h>

// forward declaration of hyperlink and page
class GmHyperlink;
class GmPage;

/**
 * This action encapsulates information about actions performed
 * by hyperlinks.
 */
class GmHyperlinkAction : public GmWidgetAction
{
 protected:
   /**
    * A page that this hyperlink action links to.
    */
   GmPage* mPage;

   /**
    * Standard constructor.
    */
   GmHyperlinkAction();

 public:
   /**
    * Constructs a hyperlink action with the given source.
    *
    * @param source the widget that is the source
    */
   GmHyperlinkAction(GmHyperlink* source);

   /**
    * Constructs a hyperlink action with the given
    * object id and widget id of the source.
    *
    * @param objectId the object id of the source
    * @param widgetId the widget id of the source
    */
   GmHyperlinkAction(unsigned short srcObjectId, WidgetIDType srcWidgetId);

   /**
    * Constructs a hyperlink action with the given source
    * and page.
    *
    * @param source the widget that is the source
    * @param page the page that this action links to
    */
   GmHyperlinkAction(GmHyperlink* source, GmPage* page);

   /**
    * Constructs a hyperlink action with the given
    * object id and widget id of the source and page.
    *
    * @param objectId the object id of the source
    * @param widgetId the widget id of the source
    * @param page the page that this action links to
    */
   GmHyperlinkAction(unsigned short srcObjectId, WidgetIDType srcWidgetId,
                     GmPage* page);

   /**
    * Constructs a hyperlink action with the given source
    * page and destination page.
    *
    * @param srcPage the page that is the source
    * @param destPage the page that this action links to
    */
   GmHyperlinkAction(GmPage* srcPage, GmPage* destPage);

   /**
    * Virtual destructor.
    */
   virtual ~GmHyperlinkAction();

   /**
    * Returns the page this action linked to.
    *    
    * @return the page this action linked to
    */
   GmPage* getPage();   
};

#endif
