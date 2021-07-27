/*
  $Id: MainScreen.h,v 1.1.2.3 2003/01/27 01:36:10 euske Exp $

  XCruiser - a filesystem visualization utility
  MainScreen.h: the public header file for MainScreenWidget

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

#ifndef _ORAMainScreen_h
#define _ORAMainScreen_h

/*  MainScreen widget
 */

/* Resources:
 * Name		     Class		RepType		Default Value
 * ----		     -----		-------		-------------
 * (from RectObj)
 * ancestorSensitive AncestorSensitive	Boolean		True
 * x		     Position		Position	0
 * y		     Position		Position	0
 * width	     Width		Dimension	text width
 * height	     Height		Dimension	text height
 * borderWidth	     BorderWidth	Dimension	1
 * sensitive	     Sensitive		Boolean		True
 *
 * (from WindowObj)
 * screen	     Screen		Screen		parent's Screen
 * depth	     Depth		int		parent's depth
 * colormap	     Colormap		Colormap	parent's colormap
 * background	     Background		Pixel		XtDefaultBackground
 * backgroundPixmap  Pixmap		Pixmap		XtUnspecifiedPixmap
 * borderColor	     BorderColor	Pixel		XtDefaultForeground
 * borderPixmap	     Pixmap		Pixmap		XtUnspecifiedPixmap
 * mappedWhenManaged MappedWhenManaged	Boolean		True
 * translations	     Translations	TranslationTable see doc or source
 * accelerators	     Accelerators	AcceleratorTable NULL
 *
 * (from Core)
 * none
 *
 * (from MainScreen)
 * background        Background		Pixel		XtDefaultBackground
 * translations	     Translations	TranslationTable see doc or source
 * infoColor         InfoColor		Pixel		green
 * alertColor        AlertColor		Pixel		red
 * measureColor      MeasureColor	Pixel		green
 * cursorColor       CursorColor	Pixel		white
 * innerRootColor    InnerRootColor	Pixel		blue
 * innerGalaxyColors			PixelList	cyan,gray
 * innerUnreadableGalaxyColors		PixelList	magenta
 * innerPlanetColors			PixelList	yellow,orange,violet,red
 * innerUnreadablePlanetColors		PixelList	magenta
 * innerWormholeColors			PixelList	green
 * innerLabelColor			Pixel		white
 * outerRootColor    OuterRootColor	Pixel		blue
 * outerGalaxyColors			PixelList	#008080,#606060
 * outerUnreadableGalaxyColors		PixelList	#c000c0
 * outerPlanetColors			PixelList	#c0c000,darkorange,darkviolet,brown
 * outerUnreadablePlanetColors		PixelList	#c000c0
 * outerWormholeColors			PixelList	#008000
 * outerLabelColor			Pixel		#606060
 * showInfo	     ShowInfo		Boolean		True
 * showMeasure	     ShowMeasure	Boolean		True
 * showLabel	     ShowLabel		Boolean		True
 * infoFont	     InfoFont		FontStruct	courier-bold-r
 * alertFont	     AlertFont		FontStruct	helvetica-bold-r
 * measureFont	     MeasureFont	FontStruct	helvetica-medium-r
 * labelFonts	     LabelFonts		FontStructList	adobe...
 * viewAllFiles	     ViewAllFiles	Boolean		False
 * currentGalaxyLineWidth		int		2
 * cursorSize        CursorSize		int		20
 * pitchLimit        PitchLimit		float		0.8
 */


#include <X11/Xaw/Simple.h>

#define XtRPixelList			"PixelList"
#define XtRFontStructList		"FontStructList"

typedef struct _PixelList {
    int		num_pixels;
    Pixel*	pixels;
} PixelList;

typedef struct _FontStructList {
    int		num_fss;
    XFontStruct** fss;
} FontStructList;

typedef struct _GCList {
    int		num_gcs;
    GC*		gcs;
} GCList;

typedef struct _FontGCAList {
    int		num_gcs;
    int*	sizes;
    GC*		gcs;
    XFontStruct** fss;
} FontGCAList;

/*
  actions:

  setspeed(n), addspeed(n), settarget(x,y), movetarget(x,y),
  toggle({info,measure,label}), open(), getinfo(), navigation([path])
*/

#define XtNinfoColor			"infoColor"
#define XtNalertColor			"alertColor"
#define XtNmeasureColor			"measureColor"
#define XtNcursorColor			"cursorColor"
#define XtNinnerRootColor		"innerRootColor"
#define XtNinnerGalaxyColors		"innerGalaxyColors"
#define XtNinnerUnreadableGalaxyColors	"innerUnreadableGalaxyColors"
#define XtNinnerPlanetColors		"innerPlanetColors"
#define XtNinnerUnreadablePlanetColors	"innerUnreadablePlanetColors"
#define XtNinnerWormholeColors		"innerWormholeColors"
#define XtNinnerLabelColor		"innerLabelColor"
#define XtNouterRootColor		"outerRootColor"
#define XtNouterGalaxyColors		"outerGalaxyColors"
#define XtNouterUnreadableGalaxyColors	"outerUnreadableGalaxyColors"
#define XtNouterPlanetColors		"outerPlanetColors"
#define XtNouterUnreadablePlanetColors	"outerUnreadablePlanetColors"
#define XtNouterWormholeColors		"outerWormholeColors"
#define XtNouterLabelColor		"outerLabelColor"
#define XtNshowInfo			"showInfo"
#define XtNshowMeasure			"showMeasure"
#define XtNshowLabel			"showLabel"
#define XtNinfoFont			"infoFont"
#define XtNalertFont			"alertFont"
#define XtNmeasureFont			"measureFont"
#define XtNlabelFonts			"labelFonts"
#define XtNcurrentGalaxyLineWidth	"currentGalaxyLineWidth"
#define XtNcursorSize			"cursorSize"
#define XtNpitchLimit			"pitchLimit"
#define XtNviewAllFiles			"viewAllFiles"

#define XtCInfoColor			"InfoColor"
#define XtCAlertColor			"AlertColor"
#define XtCMeasureColor			"MeasureColor"
#define XtCCursorColor			"CursorColor"
#define XtCInnerRootColor		"InnerRootColor"
#define XtCInnerGalaxyColors		"InnerGalaxyColors"
#define XtCInnerUnreadableGalaxyColors	"InnerUnreadableGalaxyColors"
#define XtCInnerPlanetColors		"InnerPlanetColors"
#define XtCInnerUnreadablePlanetColors	"InnerUnreadablePlanetColors"
#define XtCInnerWormholeColors		"InnerWormholeColors"
#define XtCInnerLabelColor		"InnerLabelColor"
#define XtCOuterRootColor		"OuterRootColor"
#define XtCOuterGalaxyColors		"OuterGalaxyColors"
#define XtCOuterUnreadableGalaxyColors	"OuterUnreadableGalaxyColors"
#define XtCOuterPlanetColors		"OuterPlanetColors"
#define XtCOuterUnreadablePlanetColors	"OuterUnreadablePlanetColors"
#define XtCOuterWormholeColors		"OuterWormholeColors"
#define XtCOuterLabelColor		"OuterLabelColor"
#define XtCShowInfo			"ShowInfo"
#define XtCShowMeasure			"ShowMeasure"
#define XtCShowLabel			"ShowLabel"
#define XtCInfoFont			"InfoFont"
#define XtCAlertFont			"AlertFont"
#define XtCMeasureFont			"MeasureFont"
#define XtCLabelFonts			"LabelFonts"
#define XtCCurrentGalaxyLineWidth	"CurrentGalaxyLineWidth"
#define XtCCursorSize			"CursorSize"
#define XtCPitchLimit			"PitchLimit"
#define XtCViewAllFiles			"ViewAllFiles"

extern WidgetClass	mainScreenWidgetClass;

typedef struct _MainScreenClassRec	*MainScreenWidgetClass;
typedef struct _MainScreenRec		*MainScreenWidget;

extern	void MainScreenIdleTask(Widget w);

#endif /* _ORAMainScreen_h */
