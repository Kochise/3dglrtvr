/*
  $Id: MainScreeP.h,v 1.1.2.3 2003/01/27 01:36:10 euske Exp $

  XCruiser - a filesystem visualization utility
  MainScreeP.h: the private header file for MainScreenWidget

  Copyright (C) 1999-2002  Yusuke Shinyama <yusuke at cs . nyu . edu>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
 *  MainScreenP.h
 *	Private Header File for MainScreen Widget
 */

#ifndef _ORAMainScreenP_h
#define _ORAMainScreenP_h

#include <X11/IntrinsicP.h>
#include <X11/Xaw/SimpleP.h>
#include "xcruiser.h"
#include "MainScreen.h"

/* New fields for the MainScreen widget class record */
typedef struct {
    int foo;
} MainScreenClassPart;

/* Full class record declaration */
typedef struct _MainScreenClassRec {
    CoreClassPart core_class;
    SimpleClassPart simple_class;
    MainScreenClassPart mainScreen_class;
} MainScreenClassRec;

typedef void (*XCruiserAction1)(MainScreenWidget, double);
typedef void (*XCruiserAction2)(MainScreenWidget, double, double);

/* New fields for the MainScreen widget record */
typedef struct _MainScreenPart {
    /* resources */
    Pixel background;
    XtTranslations translations;
    Pixel infoColor;
    Pixel alertColor;
    Pixel measureColor;
    Pixel cursorColor;
 
    Pixel innerRootColor;
    PixelList* innerGalaxyColors;
    PixelList* innerUnreadableGalaxyColors;
    PixelList* innerPlanetColors;
    PixelList* innerUnreadablePlanetColors;
    PixelList* innerWormholeColors;
    Pixel innerLabelColor;
 
    Pixel outerRootColor;
    PixelList* outerGalaxyColors;
    PixelList* outerUnreadableGalaxyColors;
    PixelList* outerPlanetColors;
    PixelList* outerUnreadablePlanetColors;
    PixelList* outerWormholeColors;
    Pixel outerLabelColor;
 
    XFontStruct* infoFont;
    XFontStruct* alertFont;
    XFontStruct* measureFont;
    FontStructList* labelFonts;
    Boolean show_info, show_measure, show_label;

    Boolean view_all;
    int currentGalaxyLineWidth;
    int cursorSize;
    int pitchLimit;
    
    /* private state */
    Pixmap screen_buff;
    GC gc_copy;
    GC gc_background;
    GC gc_info;
    GC gc_alert;
    GC gc_measure;
    GC gc_cursor;
    GC gc_root[2];
    GCList* gcl_galaxy[2][2];	/* [activated][permitted] */
    GCList* gcl_planet[2][2];	/* [activated][permitted] */
    GCList* gcl_wormhole[2];	/* [activated] */
    FontGCAList* gca_label[2];	/* [activated] */
    
    int centerx, centery;
    XRectangle measure_clip_h, measure_clip_v;
    
    int delayTicks;
    XCruiserAction1 action1;
    double arg1;
    XCruiserAction2 action2;
    double arg2x, arg2y;
 
    int state, flyingmode;
    Boolean freeze;
    double velocity;
    int cursorx, cursory;
    view curview;
    star* focus;
 
    route* activeRoute;
} MainScreenPart;

/* Full instance record declaration */
typedef struct _MainScreenRec {
    CorePart core;
    SimplePart simple;
    MainScreenPart mainScreen;
} MainScreenRec;

#endif /* _ORAMainScreenP_h */
