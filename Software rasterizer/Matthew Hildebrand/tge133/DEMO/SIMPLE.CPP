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
#include "..\include\tge.h"
#include "..\include\tgemouse.h"
#include "..\include\varfont.h"
#include "..\include\vcoord.h"

//*** #defines
#define DRIVERNAME	"320X200.DRV"
#define FONTNAME        "BIGTEXT.FNT"

//*** Function prototypes
void setup(void);
void demo(void);

//*** Global data
VariableFont font;
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
  //*** Load the driver
  if (loadGraphDriver(DRIVERNAME) != TGE_SUCCESS)
  {
    printf("Error loading driver %s; aborting.\n\n", DRIVERNAME);
    exit(EXIT_FAILURE);
  }

  //*** Initialize graphics mode
  if (!initGraphics())
  {
    printf("Unable to initialize graphics hardware; aborting.\n\n");
    exit(EXIT_FAILURE);
  }

  //*** Load the font
  if (!font.load(FONTNAME))
  {
    deInitGraphics();
    printf("Error loading font file %s; aborting.\n\n", FONTNAME);
    exit(EXIT_FAILURE);
  }

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
  ellipse(OUTMAXX/2, OUTMAXY/2, virtScreen.realX(340), virtScreen.realY(170), blue);

  //*** Use the VariableFont class
  font.put(0, 0, "Press a key to quit...");

  //*** Use the mouse
  if (mouseExists)
    showMouse();

  //*** Get a key, then return
  getch();
}
