/*
  $Id: xcruiser.c,v 1.1.2.2 2003/01/26 18:29:18 euske Exp $

  XCruiser - a filesystem visualization utility
  xcruiser.c: main routine
  
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

#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xmu/Editres.h>
#include <X11/Xlocale.h> 
#include "xcruiser.h"
#include "MainScreen.h"

star Universe;


/*  Main
 */
/* Exit with message describing command line format */
static void usage()
{
  printf("xcruiser - $Revision: 1.1.2.2 $ $Date: 2003/01/26 18:29:18 $\n");
  printf("usage: xcruiser [-options ...]\n");
  printf("    -display dpy         X server to connect\n");
  printf("    -background color    background color\n");
  printf("    -geometry geom       window geometry\n");
  printf("    -titlemode           informative title bar\n");
  printf("    -viewall             view all files\n");
  printf("    -delayticks n        adjust delays (milliseconds)\n");
  exit(1);
}

/* Command line options table */
static XrmOptionDescRec options[] = {
  {"-bg",	  "*Background",	XrmoptionSepArg,	NULL},
  {"-background", "*Background",	XrmoptionSepArg,	NULL},
  {"-g",	  "*Geometry",		XrmoptionSepArg,	NULL},
  {"-geom",	  "*Geometry",		XrmoptionSepArg,	NULL},
  {"-geometry",	  "*Geometry",		XrmoptionSepArg,	NULL},
  {"-t",          "*TitleMode",		XrmoptionNoArg,		"True"},
  {"-titlemode",  "*TitleMode",		XrmoptionNoArg,		"True"},
  {"-v",          "*ViewAllFiles",	XrmoptionNoArg,		"True"},
  {"-viewall",    "*ViewAllFiles",	XrmoptionNoArg,		"True"},
  {"-delay",	  "*DelayTicks",	XrmoptionSepArg,	NULL},
  {"-delayticks", "*DelayTicks",	XrmoptionSepArg,	NULL},
};

typedef struct _ResourceData {
  Boolean titlemode;
  int delayticks;
} ResourceData, *ResourceDataPtr;

static ResourceData userOptions;

#define offset(field) XtOffsetOf(ResourceData, field)
static XtResource resources[] = {
  {
    "titleMode", 
    "TitleMode", 
    XtRBoolean, 
    sizeof(Boolean),
    offset(titlemode),
    XtRImmediate, 
    (XtPointer) False
  },
  {
    "delayTicks",
    "DelayTicks", 
    XtRInt, 
    sizeof(int),
    offset(delayticks), 
    XtRString, 
    (XtPointer) "50"
  },
};
#undef Offset

static char *fallback_resources[] = {
  /* Geometry */
  "*geometry: 640x480",
  
  /* Colors */
  "*background: rgb:0/0/2",
  "*infoColor: green",
  "*alertColor: red",
  "*measureColor: green", 
  "*cursorColor: white",

  /* Colors for inner stars */
  "*innerRootColor: blue",
  "*innerGalaxyColors: cyan,gray",
  "*innerUnreadableGalaxyColors: magenta",
  "*innerPlanetColors: yellow,orange,violet,red",
  "*innerUnreadablePlanetColors: magenta",
  "*innerWormholeColors: green",
  "*innerLabelColor: white",
  
  /* Colors for outer stars */
  "*outerRootColor: blue",
  "*outerGalaxyColors: rgb:0/8/8,rgb:6/6/6",
  "*outerUnreadableGalaxyColors: rgb:c/0/c",
  "*outerPlanetColors: rgb:c/c/0,darkorange,darkviolet,brown",
  "*outerUnreadablePlanetColors: rgb:c/0/c",
  "*outerWormholeColors: rgb:0/8/0",
  "*outerLabelColor: gray",

  /* Fonts */
  "*infoFont: *adobe-courier-bold-r*180*",
  "*alertFont: *adobe-helvetica-bold-o*240*",
  "*measureFont: *adobe-helvetica-medium-r*120*",
  "*labelFonts: *adobe-courier-bold*10*,*adobe-courier-bold*12*,*adobe-courier-bold*14*,*adobe-courier-bold*18*,*adobe-courier-bold*24*",
  
  /* Other resources */
  "*currentGalaxyLineWidth: 2",
  "*cursorSize: 20",
  "*pitchLimit: 70",
  "*viewAllFiles: False",
  
  NULL
};


static void set_timer(Widget w);
static void idle_task(XtPointer data, XtIntervalId* id);

static void set_timer(Widget w)
{
    XtAppAddTimeOut(XtWidgetToApplicationContext(w), 
		    userOptions.delayticks,
		    idle_task, (XtPointer)w);
}

static void idle_task(XtPointer data, XtIntervalId* id)
{
    MainScreenIdleTask((Widget)data);
    set_timer((Widget)data);
}

int main(int argc, char* argv[])
{
  XtAppContext app_context;
  Widget toplevel, screen1;
  
  /* locale setting */
  XtSetLanguageProc(NULL, NULL, NULL);
  
  init_draw();
  init_universe();
  toplevel = XtVaAppInitialize(&app_context, "XCruiser", 
			       options, XtNumber(options),
			       &argc, argv, fallback_resources,
			       0, 0);
  if (argc != 1) usage();

  screen1 = XtVaCreateManagedWidget("screen1",
				    mainScreenWidgetClass,
				    toplevel,
				    0, 0);
  XtGetApplicationResources(toplevel, 
			    (XtPointer)&userOptions, resources, 
			    XtNumber(resources), NULL, 0);
  XtAddEventHandler(toplevel, (EventMask)0, True,
		    _XEditResCheckMessages, NULL);
  XtRealizeWidget(toplevel);
  set_timer(screen1);
  XtAppMainLoop(app_context);
  
  /*NOTREACHED*/
  return(0);
}
