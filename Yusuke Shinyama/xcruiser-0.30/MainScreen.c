/*
  $Id: MainScreen.c,v 1.1.2.3 2003/01/27 01:36:10 euske Exp $

  XCruiser - a filesystem visualization utility
  MainScreen.c: Implementation of MainScreenWidgetClass

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

#include <ctype.h>
#include <X11/StringDefs.h>
#include "xcruiser.h"
#include "MainScreeP.h"

#define DEBUG_EVENT 0


static char defaultTranslations[] = "\
  	<Motion>: moveCursor()\n\
	<Btn1Up>: brake(0.1)\n\
	<Btn3Up>: brake(0.1)\n\
	~Shift<Btn1Down>: stop() addVelocity(0.05)\n\
	Shift<Btn1Down>: stop() addVelocity(1.0)\n\
	~Shift<Btn3Down>: stop() addVelocity(-0.05)\n\
	Shift<Btn3Down>: stop() addVelocity(-1.0)\n\
	<Btn2Up>: open()\n\
	<KeyUp>: moveCursorTo(0, 0) brake(0.1)\n\
	<KeyDown>Left: moveCursor(-0.2, 0)\n\
	<KeyDown>Right: moveCursor( 0.2, 0)\n\
	<KeyDown>Up: moveCursor(0, -0.2)\n\
	<KeyDown>Down: moveCursor(0,  0.2)\n\
	~Shift<KeyDown>space: stop() addVelocity(0.05)\n\
	Shift<KeyDown>b: stop() addVelocity(-1.0)\n\
	~Shift<KeyDown>space: stop() addVelocity(0.05)\n\
	Shift<KeyDown>b: stop() addVelocity(-1.0)\n\
	<KeyDown>z: reposition()\n\
	<KeyDown>x: changeFlyingMode()\n\
	<KeyDown>v: toggleShowing(info) toggleShowing(measure) toggleShowing(label)\n\
	<KeyDown>m: toggleShowing(measure)\n\
	<KeyDown>n: autopilot()\n\
	<KeyDown>h: autopilot(/home/euske)\n\
	<KeyDown>i: getInfo()\n\
	<KeyDown>Return : open()\n\
	<KeyDown>o: open(\"open %s\")\n\
	<KeyDown>f: freeze()\n\
	<KeyDown>q: quit()\n"
#if DEBUG_DRAW
"	<KeyDown>d: debug()\n\
"
#endif
;

/*  Resources
 */
#define offset(f) XtOffsetOf(MainScreenRec, f)
static XtResource resources[] = {
    {
	XtNbackground,
	XtCBackground,
	XtRPixel,
	sizeof(Pixel),
	offset(mainScreen.background),
	XtRString,
	XtDefaultBackground
    },
    {
	XtNtranslations,
	XtCTranslations,
	XtRTranslationTable,
	sizeof(XtTranslations),
	offset(mainScreen.translations),
	XtRString,
	(XtPointer)defaultTranslations
    },
    {
	XtNinfoColor,
	XtCInfoColor,
	XtRPixel,
	sizeof(Pixel),
	offset(mainScreen.infoColor),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNalertColor,
	XtCAlertColor,
	XtRPixel,
	sizeof(Pixel),
	offset(mainScreen.alertColor),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNmeasureColor,
	XtCMeasureColor,
	XtRPixel,
	sizeof(Pixel),
	offset(mainScreen.measureColor),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNcursorColor,
	XtCCursorColor,
	XtRPixel,
	sizeof(Pixel),
	offset(mainScreen.cursorColor),
	XtRString,
	XtDefaultForeground
    },
  
    {
	XtNinnerRootColor,
	XtCInnerRootColor,
	XtRPixel,
	sizeof(Pixel),
	offset(mainScreen.innerRootColor),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNinnerGalaxyColors,
	XtCInnerGalaxyColors,
	XtRPixelList,
	sizeof(PixelList*),
	offset(mainScreen.innerGalaxyColors),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNinnerUnreadableGalaxyColors,
	XtCInnerUnreadableGalaxyColors,
	XtRPixelList,
	sizeof(PixelList*),
	offset(mainScreen.innerUnreadableGalaxyColors),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNinnerPlanetColors,
	XtCInnerPlanetColors,
	XtRPixelList,
	sizeof(PixelList*),
	offset(mainScreen.innerPlanetColors),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNinnerUnreadablePlanetColors,
	XtCInnerUnreadablePlanetColors,
	XtRPixelList,
	sizeof(PixelList*),
	offset(mainScreen.innerUnreadablePlanetColors),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNinnerWormholeColors,
	XtCInnerWormholeColors,
	XtRPixelList,
	sizeof(PixelList*),
	offset(mainScreen.innerWormholeColors),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNinnerLabelColor,
	XtCInnerLabelColor,
	XtRPixel,
	sizeof(Pixel),
	offset(mainScreen.innerLabelColor),
	XtRString,
	XtDefaultForeground
    },
  
    {
	XtNouterRootColor,
	XtCOuterRootColor,
	XtRPixel,
	sizeof(Pixel),
	offset(mainScreen.outerRootColor),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNouterGalaxyColors,
	XtCOuterGalaxyColors,
	XtRPixelList,
	sizeof(PixelList*),
	offset(mainScreen.outerGalaxyColors),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNouterUnreadableGalaxyColors,
	XtCOuterUnreadableGalaxyColors,
	XtRPixelList,
	sizeof(PixelList*),
	offset(mainScreen.outerUnreadableGalaxyColors),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNouterPlanetColors,
	XtCOuterPlanetColors,
	XtRPixelList,
	sizeof(PixelList*),
	offset(mainScreen.outerPlanetColors),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNouterUnreadablePlanetColors,
	XtCOuterUnreadablePlanetColors,
	XtRPixelList,
	sizeof(PixelList*),
	offset(mainScreen.outerUnreadablePlanetColors),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNouterWormholeColors,
	XtCOuterWormholeColors,
	XtRPixelList,
	sizeof(PixelList*),
	offset(mainScreen.outerWormholeColors),
	XtRString,
	XtDefaultForeground
    },
    {
	XtNouterLabelColor,
	XtCOuterLabelColor,
	XtRPixel,
	sizeof(Pixel),
	offset(mainScreen.outerLabelColor),
	XtRString,
	XtDefaultForeground
    },
  
    {
	XtNshowInfo,
	XtCShowInfo,
	XtRBoolean,
	sizeof(Boolean),
	offset(mainScreen.show_info),
	XtRImmediate,
	(XtPointer)True		/* default: show information */
    },
    {
	XtNshowMeasure,
	XtCShowMeasure,
	XtRBoolean,
	sizeof(Boolean),
	offset(mainScreen.show_measure),
	XtRImmediate,
	(XtPointer)True		/* default: show a measure */
    },
    {
	XtNshowLabel,
	XtCShowLabel,
	XtRBoolean,
	sizeof(Boolean),
	offset(mainScreen.show_label),
	XtRImmediate,
	(XtPointer)True		/* default: show labels */
    },
    {
	XtNinfoFont,
	XtCInfoFont,
	XtRFontStruct,
	sizeof(XFontStruct*),
	offset(mainScreen.infoFont),
	XtRString,
	NULL
    },
    {
	XtNalertFont,
	XtCAlertFont,
	XtRFontStruct,
	sizeof(XFontStruct*),
	offset(mainScreen.alertFont),
	XtRString,
	NULL
    },
    {
	XtNmeasureFont,
	XtCMeasureFont,
	XtRFontStruct,
	sizeof(XFontStruct*),
	offset(mainScreen.measureFont),
	XtRString,
	NULL
    },
    {
	XtNlabelFonts,
	XtCLabelFonts,
	XtRFontStructList,
	sizeof(FontStructList*),
	offset(mainScreen.labelFonts),
	XtRString,
	NULL
    },
    
    {
	XtNcurrentGalaxyLineWidth,
	XtCCurrentGalaxyLineWidth,
	XtRInt,
	sizeof(int),
	offset(mainScreen.currentGalaxyLineWidth),
	XtRString,
	NULL
    },
    {
	XtNcursorSize,
	XtCCursorSize,
	XtRInt,
	sizeof(int),
	offset(mainScreen.cursorSize),
	XtRString,
	NULL
    },
    {
	XtNpitchLimit,
	XtCPitchLimit,
	XtRInt,
	sizeof(int),
	offset(mainScreen.pitchLimit),
	XtRString,
	NULL
    },
    {
	XtNviewAllFiles,
	XtCViewAllFiles,
	XtRBoolean,
	sizeof(Boolean),
	offset(mainScreen.view_all),
	XtRImmediate,
	(XtPointer)False	/* default: hide dot-files */
    },
};
#undef offset


/*  Methods
 */
static void ClassInitialize(void);
static void Initialize(Widget request, Widget new,
		       ArgList args, Cardinal *num_args);
static void Realize(Widget w, XtValueMask* valueMask, 
		    XSetWindowAttributes* attributes);
static void Redisplay(Widget w, XEvent *event, Region region);
static void Resize(Widget w);


/*  Actions
 */
static void act_setVelocity(Widget w, XEvent *event,
			    String *params, Cardinal *num_params);
static void act_addVelocity(Widget w, XEvent *event,
			    String *params, Cardinal *num_params);
static void act_brake(Widget w, XEvent *event,
		      String *params, Cardinal *num_params);
static void act_moveCursorTo(Widget w, XEvent *event,
			     String *params, Cardinal *num_params);
static void act_moveCursor(Widget w, XEvent *event,
			   String *params, Cardinal *num_params);
static void act_toggleShowing(Widget w, XEvent *event,
			      String *params, Cardinal *num_params);
static void act_open(Widget w, XEvent *event,
		     String *params, Cardinal *num_params);
static void act_getInfo(Widget w, XEvent *event,
			String *params, Cardinal *num_params);
static void act_autopilot(Widget w, XEvent *event,
			  String *params, Cardinal *num_params);
static void act_stop(Widget w, XEvent *event,
		     String *params, Cardinal *num_params);
static void act_reposition(Widget w, XEvent *event,
		       String *params, Cardinal *num_params);
static void act_flyingmode(Widget w, XEvent *event,
			   String *params, Cardinal *num_params);
static void act_freeze(Widget w, XEvent *event,
		       String *params, Cardinal *num_params);
static void act_alert(Widget w, XEvent *event,
		      String *params, Cardinal *num_params);
static void act_quit(Widget w, XEvent *event,
		     String *params, Cardinal *num_params);
static void do_setVelocity(MainScreenWidget scr,
			   double arg1);
static void do_addVelocity(MainScreenWidget scr,
			   double arg1);
static void do_brake(MainScreenWidget scr,
		     double arg1);
static void do_moveCursorTo(MainScreenWidget scr,
			    double arg1, double arg2);
static void do_moveCursor(MainScreenWidget scr,
			  double arg1, double arg2);
#if DEBUG_DRAW
static void act_debug(Widget w, XEvent *event,
		      String *params, Cardinal *num_params);
#endif

static XtActionsRec actionsList[] = {
    { "setVelocity",	act_setVelocity },
    { "addVelocity",	act_addVelocity },
    { "brake",		act_brake },
    { "moveCursorTo",	act_moveCursorTo },
    { "moveCursor",	act_moveCursor },
    { "reposition",	act_reposition },
    { "changeFlyingMode", act_flyingmode },
    { "toggleShowing",	act_toggleShowing },
    { "open",		act_open },
    { "getInfo",	act_getInfo },
    { "autopilot",	act_autopilot },
    { "stop",		act_stop },
    { "freeze",		act_freeze },
    { "alert",		act_alert },
    { "quit",		act_quit },    
#if DEBUG_DRAW
    { "debug",		act_debug },    
#endif
};


/*  Class Definition
 */
#define SuperClass ((SimpleWidgetClass)&simpleClassRec)
MainScreenClassRec mainScreenClassRec = {
    /* core */
    {
	(WidgetClass) SuperClass,           /* superclass             */    
	"MainScreen",                       /* class_name             */
	sizeof(MainScreenRec),              /* size                   */
	ClassInitialize,                    /* class_initialize       */
	NULL,                               /* class_part_initialize  */
	FALSE,                              /* class_inited           */
	Initialize,                         /* initialize             */
	NULL,                               /* initialize_hook        */
	Realize,                            /* realize                */
	actionsList,                        /* actions                */
	XtNumber(actionsList),              /* num_actions            */
	resources,                          /* resources              */
	XtNumber(resources),                /* resource_count         */
	NULLQUARK,                          /* xrm_class              */
	FALSE,                              /* compress_motion        */
	TRUE,                               /* compress_exposure      */
	TRUE,                               /* compress_enterleave    */
	FALSE,                              /* visible_interest       */
	NULL,                               /* destroy                */
	Resize,                             /* resize                 */
	Redisplay,                          /* expose                 */
	NULL,                               /* set_values             */
	NULL,                               /* set_values_hook        */
	XtInheritSetValuesAlmost,           /* set_values_almost      */
	NULL,                               /* get_values_hook        */
	XtInheritAcceptFocus,               /* accept_focus           */
	XtVersion,                          /* version                */
	NULL,                               /* callback_private       */
	defaultTranslations,                /* tm_table               */
	XtInheritQueryGeometry,             /* query_geometry         */
	XtInheritDisplayAccelerator,        /* display_accelerator    */
	NULL                                /* extension              */
    },
    /* simple */
    {
	XtInheritChangeSensitive,         /* change_sensitive       */
    },
    /* mainScreen */
    {
	0,
    },
};

WidgetClass mainScreenWidgetClass = (WidgetClass)&mainScreenClassRec;


/*  Converter from XtRString to XtRPixelList
 */
#define done(type, value) \
 {       \
     if (toVal->addr != NULL) {    \
  if (toVal->size < sizeof(type)) {  \
      toVal->size = sizeof(type);   \
      return False;    \
  }      \
  *(type*)(toVal->addr) = (value);  \
     }       \
     else {      \
  static type static_val;    \
  static_val = (value);    \
  toVal->addr = (XPointer)&static_val;  \
     }       \
     toVal->size = sizeof(type);    \
     return True;     \
 }

static Boolean convStringToPixelList(
    Display* dpy, XrmValuePtr args, Cardinal* num_args,
    XrmValuePtr fromVal, XrmValuePtr toVal, 
    XtPointer *destructor_data)
{
    Pixel pixels[16];
    int n = 0;
    char str1buff[256];
    char c;
    char* src = fromVal->addr;
    char* dest;
  
    do {
	dest = str1buff;
	while((c = *(src++)) && c != ',') {
	    if (!isspace(c)) *(dest++) = c;
	}
	*dest = '\0';
	if (dest != str1buff) {
	    XrmValue pix1;
	    XrmValue str1;
	    str1.addr = (XtPointer)str1buff;
	    str1.size = strlen(str1buff)+1;
	    pix1.addr = (XtPointer)&pixels[n];
	    pix1.size = sizeof(Pixel);
	    XtCallConverter(dpy, XtCvtStringToPixel, args, *num_args,
			    &str1, &pix1, NULL);
	    if (pix1.addr)
		n++;
	}
    } while(c);
  
    if (n) {
	int i;
	static PixelList* p;
	p = (PixelList*)malloc(sizeof(PixelList));
	p->num_pixels = n;
	p->pixels = (Pixel*)malloc(sizeof(Pixel) * n);
	for(i = 0; i < n; i++)
	    p->pixels[i] = pixels[i];
	done(PixelList*, p);
    } else {
	toVal->addr = NULL;
	toVal->size = 0;
	XtDisplayStringConversionWarning(dpy, fromVal->addr, 
					 "XtRPixelList");
    }
    return True;
}

/*  Converter from XtRString to XtRFontStructList
 */
static Boolean convStringToFontStructList(
    Display* dpy, XrmValuePtr args, Cardinal* num_args,
    XrmValuePtr fromVal, XrmValuePtr toVal, 
    XtPointer *destructor_data)
{
    XFontStruct* fss[16];
    int n = 0;
    char c;
    char str1buff[256];
    char* src = fromVal->addr;
    char* dest;
  
    do {
	dest = str1buff;
	while((c = *(src++)) && c != ',') {
	    if (!isspace(c)) *(dest++) = c;
	}
	*dest = '\0';
	if (dest != str1buff) {
	    XrmValue str1;
	    XrmValue fs1;
	    XrmValue args1;
	    args1.addr = (XtPointer)&dpy;
	    args1.size = sizeof(Display**);
	    str1.addr = (XtPointer)str1buff;
	    str1.size = strlen(str1buff)+1;
	    fs1.addr = (XtPointer)&fss[n];
	    fs1.size = sizeof(XFontStruct*);
	    XtCallConverter(dpy, XtCvtStringToFontStruct, &args1, 1,
			    &str1, &fs1, NULL);
	    if (fs1.addr)
		n++;
	}
    } while(c);
    
    if (n) {
	int i;
	static FontStructList* s;
	s = (FontStructList*)malloc(sizeof(FontStructList));
	s->num_fss = n;
	s->fss = (XFontStruct**)malloc(sizeof(XFontStruct*) * n);
	for(i = 0; i < n; i++)
	    s->fss[i] = fss[i];
	done(FontStructList*, s);
    } else {
	toVal->addr = NULL;
	toVal->size = 0;
	XtDisplayStringConversionWarning(dpy, fromVal->addr, 
					 "XtRFontStructList");
    }
    return True;
}

/*  Register the converters
 */
static void ClassInitialize(void)
{
    XtSetTypeConverter(XtRString,
		       XtRPixelList,
		       convStringToPixelList,
		       (XtConvertArgList)colorConvertArgs, 2,
		       XtCacheAll,
		       NULL);
  
    XtSetTypeConverter(XtRString,
		       XtRFontStructList,
		       convStringToFontStructList,
		       (XtConvertArgList)colorConvertArgs, 2,
		       XtCacheAll,
		       NULL);
}


/*  Initialize widget
 */
static GC getGC(Display* dpy,
		Drawable d, 
		Pixel p,
		XFontStruct* fs)
{
    XtGCMask mask = fs? (GCForeground | GCFont) : GCForeground;
    XGCValues gcv;
    if (fs) {
	gcv.font = fs->fid;
    }
    gcv.foreground = p;
    return(XCreateGC(dpy, d, mask, &gcv));
}

static GCList* getGCList(Display* dpy,
			 Drawable d,
			 PixelList* pl)
{
    GCList* gcl = (GCList*)malloc(sizeof(GCList));
    int i;
    gcl->num_gcs = pl->num_pixels;
    gcl->gcs = (GC*)malloc(sizeof(GC) * gcl->num_gcs);
    for(i = 0; i < pl->num_pixels; i++) {
	XGCValues gcv;
	gcv.foreground = pl->pixels[i];
	gcl->gcs[i] = XCreateGC(dpy, d, GCForeground, &gcv);
    }
    return(gcl);
}

static FontGCAList* getFontGCAList(Display* dpy,
				   Drawable d,
				   Pixel p,
				   FontStructList* fl)
{
    FontGCAList* fgca = (FontGCAList*)malloc(sizeof(FontGCAList));
    int i;
    fgca->num_gcs = fl->num_fss;
    fgca->sizes = (int*)malloc(sizeof(int) * fgca->num_gcs);
    fgca->fss = (XFontStruct**)malloc(sizeof(XFontStruct*) * fgca->num_gcs);
    fgca->gcs = (GC*)malloc(sizeof(GC) * fgca->num_gcs);
    for(i = 0; i < fl->num_fss; i++) {
	XGCValues gcv;
	gcv.foreground = p;
	gcv.font = fl->fss[i]->fid;
	fgca->sizes[i] = fl->fss[i]->ascent + fl->fss[i]->descent;
	fgca->fss[i] = fl->fss[i];
	fgca->gcs[i] = XCreateGC(dpy, d, GCForeground | GCFont, &gcv);
    }
    return(fgca);
}

static void freeGCL(Display* dpy, GCList* gcl)
{
    int i;
    for(i = 0; i < gcl->num_gcs; i++) {
	XFreeGC(dpy, gcl->gcs[i]);
    }
    free((char*)gcl->gcs);
    free((char*)gcl);
}

static void freeFGCA(Display* dpy, FontGCAList* fgca)
{
    int i;
    for(i = 0; i < fgca->num_gcs; i++) {
	XFreeGC(dpy, fgca->gcs[i]);
    }
    free((char*)fgca->sizes);
    free((char*)fgca->fss);
    free((char*)fgca->gcs);
    free((char*)fgca);
}

static void Initialize(Widget request,
		       Widget new,
		       ArgList args,
		       Cardinal *num_args)
{
    MainScreenWidget me = (MainScreenWidget)new;
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: initialize\n");
#endif
  
    me->mainScreen.state = STATE_NORMAL;
    me->mainScreen.flyingmode = FLY_POLAR;
    me->mainScreen.freeze = 0;
    me->mainScreen.screen_buff = None;
    me->mainScreen.action1 = NULL;
    me->mainScreen.action2 = NULL;
    me->mainScreen.velocity = 0.0;
    me->mainScreen.cursorx = me->mainScreen.cursory = 0;
    init_curview(&me->mainScreen.curview);
    me->mainScreen.activeRoute = NULL;
}

static void InitScreenBuff(Widget w)
{
    MainScreenWidget me = (MainScreenWidget)w;
    MainScreenPart* scr = &((MainScreenWidget)w)->mainScreen;
    Display* dpy = XtDisplay(w);
    Pixmap p;
  
    scr->centerx = me->core.width / 2;
    scr->centery = me->core.height / 2;
    setup_measure(me);
    
    p = XCreatePixmap(dpy, XtWindow(w),
		      me->core.width, me->core.height, me->core.depth);
    scr->screen_buff = p;
  
    scr->gc_copy = getGC(dpy, XtWindow(w), 0, NULL);
    scr->gc_background =
	getGC(dpy, p, scr->background, NULL);
    scr->gc_info =
	getGC(dpy, p, scr->infoColor, scr->infoFont);
    scr->gc_alert =
	getGC(dpy, p, scr->alertColor, scr->alertFont);
    scr->gc_measure =
	getGC(dpy, p, scr->measureColor, scr->measureFont);
    scr->gc_cursor =
	getGC(dpy, p, scr->cursorColor, NULL);
    scr->gc_root[0] =
	getGC(dpy, p, scr->outerRootColor, NULL);
    scr->gc_root[1] =
	getGC(dpy, p, scr->innerRootColor, NULL);
    scr->gcl_galaxy[1][1] =
	getGCList(dpy, p, scr->innerGalaxyColors);
    scr->gcl_galaxy[0][1] =
	getGCList(dpy, p, scr->outerGalaxyColors);
    scr->gcl_galaxy[1][0] =
	getGCList(dpy, p, scr->innerUnreadableGalaxyColors);
    scr->gcl_galaxy[0][0] =
	getGCList(dpy, p, scr->outerUnreadableGalaxyColors);
    scr->gcl_planet[1][1] =
	getGCList(dpy, p, scr->innerPlanetColors);
    scr->gcl_planet[0][1] =
	getGCList(dpy, p, scr->outerPlanetColors);
    scr->gcl_planet[1][0] =
	getGCList(dpy, p, scr->innerUnreadablePlanetColors);
    scr->gcl_planet[0][0] =
	getGCList(dpy, p, scr->outerUnreadablePlanetColors);
    scr->gcl_wormhole[1] =
	getGCList(dpy, p, scr->innerWormholeColors);
    scr->gcl_wormhole[0] =
	getGCList(dpy, p, scr->outerWormholeColors);
    scr->gca_label[1] =
	getFontGCAList(dpy, p, scr->innerLabelColor, scr->labelFonts);
    scr->gca_label[0] =
	getFontGCAList(dpy, p, scr->outerLabelColor, scr->labelFonts);
}

static void CleanupScreenBuff(Widget w)
{
    MainScreenPart* scr = &((MainScreenWidget)w)->mainScreen;
    Display* dpy = XtDisplay(w);
    int i, j;
    
    XFreePixmap(dpy, scr->screen_buff);
    scr->screen_buff = None;
    
    XFreeGC(dpy, scr->gc_copy);
    XFreeGC(dpy, scr->gc_background);
    XFreeGC(dpy, scr->gc_info);
    XFreeGC(dpy, scr->gc_alert);
    XFreeGC(dpy, scr->gc_measure);
    XFreeGC(dpy, scr->gc_cursor);
    for(i = 0; i < 2; i++) {
	XFreeGC(dpy, scr->gc_root[i]);
	freeGCL(dpy, scr->gcl_wormhole[i]);
	freeFGCA(dpy, scr->gca_label[i]);
	for(j = 0; j < 2; j++) {
	    freeGCL(dpy, scr->gcl_galaxy[i][j]);
	    freeGCL(dpy, scr->gcl_planet[i][j]);
	}
    }
}

static void Realize(Widget w,
		    XtValueMask* valueMask,
		    XSetWindowAttributes* attributes)
{
    (*SuperClass->core_class.realize)(w, valueMask, attributes);
  
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: realize\n");
#endif
}

static void Resize(Widget w)
{
    /*(*SuperClass->core_class.resize)(w);*/
    MainScreenWidget me = (MainScreenWidget) w;
    if (me->mainScreen.screen_buff != None) {
	CleanupScreenBuff(w);
    }
  
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: resize\n");
#endif
    
}

static void Redisplay(Widget w,
		      XEvent *event, 
		      Region region)
{
    MainScreenWidget me = (MainScreenWidget) w;
    if (me->mainScreen.screen_buff != None) {
	XCopyArea(XtDisplay(w), me->mainScreen.screen_buff, XtWindow(w),
		  me->mainScreen.gc_copy, 0, 0,
		  me->core.width, me->core.height, 0, 0);
    }
  
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: redisplay\n");
#endif
}


static void act_setVelocity(Widget w,
			    XEvent* event,
			    String* params,
			    Cardinal* num_params)
{
    MainScreenWidget me = (MainScreenWidget) w;
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: act_setVelocity");
    if (1 <= *num_params) {
	fprintf(stderr, "(%s)", params[0]);
    }
    fprintf(stderr, "\n");
#endif
    me->mainScreen.action1 = do_setVelocity;
    me->mainScreen.arg1 = (*num_params)? atof(params[0]) : 0.0;
}

static void act_addVelocity(Widget w,
			    XEvent* event,
			    String* params,
			    Cardinal* num_params)
{
    MainScreenWidget me = (MainScreenWidget) w;
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: act_addVelocity");
    if (1 <= *num_params) {
	fprintf(stderr, "(%s)", params[0]);
    }
    fprintf(stderr, "\n");
#endif
    me->mainScreen.action1 = do_addVelocity;
    me->mainScreen.arg1 = (*num_params)? atof(params[0]) : 0.0;
}

static void act_brake(Widget w,
			 XEvent* event,
			 String* params,
			 Cardinal* num_params)
{
    MainScreenWidget me = (MainScreenWidget) w;
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: act_brake");
    if (1 <= *num_params) {
	fprintf(stderr, "(%s)", params[0]);
    }
    fprintf(stderr, "\n");
#endif
    me->mainScreen.action1 = do_brake;
    me->mainScreen.arg1 = (*num_params)? atof(params[0]) : 0.0;
}

static void act_moveCursorTo(Widget w,
			     XEvent* event,
			     String* params,
			     Cardinal* num_params)
{
    MainScreenWidget me = (MainScreenWidget) w;
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: act_moveCursorTo");
    if (2 <= *num_params) {
	fprintf(stderr, "(%s, %s)", params[0], params[1]);
    }
    fprintf(stderr, "\n");
#endif
    me->mainScreen.action2 = do_moveCursorTo;
    if (2 <= *num_params) {
	me->mainScreen.arg2x = atof(params[0]);
	me->mainScreen.arg2y = atof(params[1]);
    } else {
	me->mainScreen.arg2x = 0.0;
	me->mainScreen.arg2y = 0.0;
    }
}

static void act_moveCursor(Widget w,
			   XEvent* event,
			   String* params,
			   Cardinal* num_params)
{
    MainScreenWidget me = (MainScreenWidget) w;
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: act_moveCursor");
    if (2 <= *num_params) {
	fprintf(stderr, "(%s, %s)", params[0], params[1]);
    }
    fprintf(stderr, "\n");
#endif
    me->mainScreen.action2 = do_moveCursor;
    if (2 <= *num_params) {
	me->mainScreen.arg2x = atof(params[0]);
	me->mainScreen.arg2y = atof(params[1]);
    } else {
	me->mainScreen.arg2x = 0.0;
	me->mainScreen.arg2y = 0.0;
    }
}

static void act_toggleShowing(Widget w,
			      XEvent* event,
			      String* params,
			      Cardinal* num_params)
{
    MainScreenWidget me = (MainScreenWidget) w;
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: act_toggleShowing");
    if (1 <= *num_params) {
	fprintf(stderr, "(%s)", params[0]);
    }
    fprintf(stderr, "\n");
#endif
    if (1 <= *num_params) {
	if (!strcasecmp("info", params[0])) {
	    me->mainScreen.show_info = !me->mainScreen.show_info;
	} else if (!strcasecmp("measure", params[0])) {
	    me->mainScreen.show_measure = !me->mainScreen.show_measure;
	} else if (!strcasecmp("label", params[0])) {
	    me->mainScreen.show_label = !me->mainScreen.show_label;
	}
    }
}

static route* set_route(wormholeinfo* w)
{
    route* r = (route*) malloc(sizeof(route));
    if (r) {
	r->w = w;
	r->p = 0.0;
    }
    return(r);
}

static void act_open(Widget w,
		     XEvent* event,
		     String* params,
		     Cardinal* num_params)
{
    MainScreenWidget me = (MainScreenWidget) w;
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: act_open");
    if (1 <= *num_params) {
	fprintf(stderr, "(%s)", params[0]);
    }
    fprintf(stderr, "\n");
#endif
    if (1 <= *num_params) {
	star* focus = me->mainScreen.focus;
	if (focus && focus != &Universe) {
	    char fname[MAX_CHARS], buff[MAX_CHARS];
	    snprintf(fname, sizeof(fname), "%s%s", 
		     focus->i.parent->pathname, focus->i.basename);
	    snprintf(buff, sizeof(buff), params[0], fname);
	    fprintf(stderr, "exec: \"%s\"\n", buff);
	}
    }
}

static void act_getInfo(Widget w,
			XEvent* event,
			String* params,
			Cardinal* num_params)
{
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: act_getInfo\n");
#endif
    ;
}

static void act_autopilot(Widget w,
			  XEvent* event,
			  String* params,
			  Cardinal* num_params)
{
    /*    MainScreenWidget me = (MainScreenWidget) w;*/
#if DEBUG_EVENT
#endif
    fprintf(stderr, "MainScreen: act_autopilot");
    if (1 <= *num_params) {
	fprintf(stderr, "(%s)", params[0]);
    }
    fprintf(stderr, "\n");
    ;
}

static void act_stop(Widget w,
		     XEvent* event,
		     String* params,
		     Cardinal* num_params)
{
    /*MainScreenWidget me = (MainScreenWidget) w;*/
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: act_stop\n");
#endif
    ;
}

static void act_reposition(Widget w,
		       XEvent* event,
		       String* params,
		       Cardinal* num_params)
{
    MainScreenWidget me = (MainScreenWidget) w;
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: act_reposition\n");
#endif
    me->mainScreen.state = STATE_REPOSITION;
}

static void act_flyingmode(Widget w,
			   XEvent* event,
			   String* params,
			   Cardinal* num_params)
{
    MainScreenWidget me = (MainScreenWidget) w;
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: act_flyingmode");
    if (1 <= *num_params) {
	fprintf(stderr, "(%s)", params[0]);
    }
    fprintf(stderr, "\n");
#endif
    if (1 <= *num_params) {
	if (!strcasecmp("polar", params[0])) {
	    me->mainScreen.flyingmode = FLY_POLAR;
	} else if (!strcasecmp("cartesian", params[0])) {
	    me->mainScreen.flyingmode = FLY_CARTESIAN;
	}
    } else {
	me->mainScreen.flyingmode = (me->mainScreen.flyingmode == FLY_POLAR)? FLY_CARTESIAN : FLY_POLAR;
    }
}

static void act_freeze(Widget w,
		       XEvent* event,
		       String* params,
		       Cardinal* num_params)
{
    MainScreenWidget me = (MainScreenWidget) w;
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: act_freeze\n");
#endif
    me->mainScreen.freeze = !me->mainScreen.freeze;
}

static void act_alert(Widget w,
		      XEvent* event,
		      String* params,
		      Cardinal* num_params)
{
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: act_alert\n");
#endif
    if (1 <= *num_params)
	draw_alert((MainScreenWidget)w, params[0]);
}

static void act_quit(Widget w,
		     XEvent* event,
		     String* params,
		     Cardinal* num_params)
{
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: act_quit\n");
#endif
    exit(0);
}

#if DEBUG_DRAW
static void act_debug(Widget w,
		      XEvent* event,
		      String* params,
		      Cardinal* num_params)
{
    MainScreenWidget me = (MainScreenWidget)w;
    fprintf(stderr, "debug -------------------------------\n");
    dump_frame(&me->mainScreen.curview);
    if (me->mainScreen.focus && me->mainScreen.focus != &Universe) {
	debug_star(me->mainScreen.focus);
    } else {
	debug_star((star*)me->mainScreen.curview.frms[me->mainScreen.curview.curidx].g);
    }
}
#endif

static void do_setVelocity(MainScreenWidget me, double arg1)
{
    me->mainScreen.velocity = arg1;
}

static void do_addVelocity(MainScreenWidget me, double arg1)
{
    me->mainScreen.velocity += arg1 / (fabs(me->mainScreen.velocity)+1.0);
}

static void do_brake(MainScreenWidget me, double arg1)
{
    double v = me->mainScreen.velocity;
    if (0 < v) {
	v -= arg1 * (fabs(me->mainScreen.velocity)+1.0);
	if (v < 0) v = 0;
    } else {
	v += arg1 * (fabs(me->mainScreen.velocity)+1.0);
	if (0 < v) v = 0;
    }
    me->mainScreen.velocity = v;
}

static void do_moveCursorTo(MainScreenWidget me, double arg1, double arg2)
{
    me->mainScreen.cursorx = arg1 * me->mainScreen.centerx;
    me->mainScreen.cursory = arg2 * me->mainScreen.centery;
}

static void do_moveCursor(MainScreenWidget me, double arg1, double arg2)
{
    if (!arg1 && !arg2) {
	int x0, y0, x, y;
	Window w1, w2;
	unsigned int btn;
	XQueryPointer(XtDisplay(me), XtWindow(me),
		      &w1, &w2, &x0, &y0, &x, &y, &btn);
	me->mainScreen.cursorx = x - me->mainScreen.centerx;
	me->mainScreen.cursory = y - me->mainScreen.centery;
    } else {
	me->mainScreen.cursorx += arg1 * me->mainScreen.centerx;
	me->mainScreen.cursory += arg2 * me->mainScreen.centery;
    }
}


/* idle */
#define BOUND 0.8
#define ROT 0.3
void MainScreenIdleTask(Widget w)
{
    MainScreenWidget me = (MainScreenWidget)w;
    MainScreenPart* scr = &((MainScreenWidget)w)->mainScreen;
    int mx = scr->centerx * BOUND, my = scr->centery * BOUND;
  
    if (scr->screen_buff == None)
	InitScreenBuff(w);

    if (scr->action1)
	(*scr->action1)(me, scr->arg1);
    if (scr->action2)
	(*scr->action2)(me, scr->arg2x, scr->arg2y);
    if (scr->cursorx < -mx)
	scr->cursorx = -mx;
    else if (mx < scr->cursorx)
	scr->cursorx = mx;
    if (scr->cursory < -my)
	scr->cursory = -my;
    else if (my < scr->cursory)
	scr->cursory = my;
  
    if (!scr->freeze) {
	double dl, dx, dy;
	switch(scr->state) {
	case STATE_AUTOPILOT:
	    do_route(scr->activeRoute, &dx, &dy, &scr->velocity);
	    break;
	case STATE_REPOSITION:
	    if (0.99 < reposition_ship(&scr->curview)) {
		scr->state = STATE_NORMAL;
	    }
	    break;
	case STATE_NORMAL:
	    dx = (double)scr->cursorx / (double)scr->centerx;
	    dy = (double)scr->cursory / (double)scr->centery;
	    dl = sqrt(dx*dx + dy*dy) * ROT;
	    dx = dx * dl * dl;
	    dy = -dy * dl * dl;
	    if (scr->flyingmode == FLY_POLAR) {
		move_ship_polar(&scr->curview, scr->velocity, dx, dy, scr->pitchLimit);
	    } else {
		move_ship_cartesian(&scr->curview, scr->velocity, dx, dy);
	    }
	    break;
	}
    }
    XFillRectangle(XtDisplay(w), scr->screen_buff, scr->gc_background, 
		   0, 0, me->core.width, me->core.height);
    draw_universe(me, &scr->curview);
    draw_information(me, &scr->curview, scr->state, scr->flyingmode);
    Redisplay((Widget)me, NULL, None);
    process_marked_stars(me);
    XFlush(XtDisplay(w));
    
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: idle: (%d, %d)\n",
	    scr->cursorx, scr->cursory);
#endif
}


/* for test */
#if 0
int main(int argc, char* argv[])
{
    XtAppContext app_context;
    Widget toplevel, mainScreen;
    toplevel = XtVaOpenApplication(&app_context, "XCruiser", NULL, 0,
				   &argc, argv, NULL, mainScreenWidgetClass, 0, 0);
    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
#endif
