//*************************************************************************
//* Program:  SIMPLE.CPP						  *
//*									  *
//* Purpose:  This program does almost nothing when it is run; it is not  *
//*	      intended to be run, but as an example in source code form   *
//*	      of how to interface to TGE's various features.  The setup() *
//*	      function illustrates how to initialize these features, and  *
//*	      demo() illustrates briefly how to use them.		  *
//*************************************************************************

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "tge.h"
#include "tgefont.h"
#include "tgemouse.h"
#include "vcoord.h"

//*** #defines
#define DRIVERNAME	"320X240.DRV"
#define FONTNAME      	"8X16.FNT"

//*** Function prototypes
void setup(void);
void demo(void);

//*** Global data
Font font(FONTNAME);
VirtualCoord virtScreen;
int mouseExists;


//*****
//***** Program start
//*****

void main(void)
{
  //*** Set things up
  setup();

  //*** Run the demo
  demo();

  //*** Restore text mode and quit
  deInitGraphics();
}


//*****
//***** General setup
//*****

void setup(void)
{
  //*** Ensure font loaded correctly
  if (!font.status())
  {
    printf("Error loading %s -- aborting.\n\n", FONTNAME);
    exit(1);
  }

  //*** Load the driver
  if (loadGraphDriver(DRIVERNAME) != TGE_SUCCESS)
  {
    printf("Error loading %s -- aborting.\n\n", DRIVERNAME);
    exit(1);
  }

  //*** Initialize graphics mode
  if (!initGraphics())
  {
    printf("Unable to initialize graphics hardware -- aborting.\n\n");
    exit(1);
  }

  //*** Initialize the font colours
  font.foreground(colourCloseToX(255,255,255,0));	// white foreground
  font.background(colourCloseToX(  0,  0,  0,0));	// black background

  //*** Initialize virtual coordinate system
  virtScreen.virtParams(1023, 767);
  virtScreen.realParams(OUTMAXX, OUTMAXY);

  //*** Set up the mouse
  if ((mouseExists=resetMouse()) != 0)		// is mouse available?
  {
    initNewMouse();				// yes, initialize it
    setHorizLimitsMouse(0, OUTMAXX);
    setVertLimitsMouse(0, OUTMAXY);
    setPosMouse(OUTMAXX/2, OUTMAXY/2);
    setupMousePointer(BIG_ARROW_POINTER);
    atexit(deInitNewMouse);
  }
}


//*****
//***** Silly little demo function
//*****

void demo(void)
{
  unsigned blue;

  //*** Use colourCloseTo()
  blue = colourCloseTo(0, 0, 200);

  //*** Use the virtual coordinate system
  ellipse(MAXX/2, MAXY/2, virtScreen.realX(340), virtScreen.realY(170), blue);

  //*** Use the Font class
  font.put(0, 0, "Press a key to quit...");

  //*** Use the mouse
  if (mouseExists)
    showMouse();

  //*** Get a key, then return
  getch();
}