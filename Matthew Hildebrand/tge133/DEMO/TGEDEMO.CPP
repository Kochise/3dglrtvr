/**************************************************************************
*  File:        TGEDEMO.CPP   Copyright (c) 1993-1994 by Matthew Hildebrand
*
*  Purpose:     Demonstrate TGE 1.33's features.
**************************************************************************/

#include <alloc.h>
#include <conio.h>
#include <dos.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>
#include "..\include\tge.h"
#include "..\include\tgemouse.h"
#include "..\include\varfont.h"
#include "..\include\vcoord.h"

#define TGELOGO_FILENAME    "TGELOGO.RAW"
#define FONT_FILENAME       "BIGTEXT.FNT"
#define QUIT_IF_ESCAPE      if (getKey() == 27)  \
                              exit(EXIT_SUCCESS)

enum
{
  Center, BottomCenter
};

void setup(int argc, char *argv[]);
void printMessage(char *string, int where=BottomCenter);
void lineDemo(void);
void putPixelDemo(void);
void drawRectDemo(void);
void filledRectDemo(void);
void ellipseDemo(void);
void filledEllipseDemo(void);
void circleDemo(void);
void filledCircleDemo(void);
void putImageDemo(void);
void fontDemo(void);
void viewportDemo(void);
void paletteDemo(void);
void scaleBitmapDemo(void);
void mouseDemo(void);
void signOff(void);
int getKey(void);
void waitVerticalRetrace(void);

VariableFont font;
void far *tgeLogo;
unsigned char palette[] =
{
    0,   0,   0,   0,   0, 168,   0, 168,   0,   0, 168, 168,
  168,   0,   0, 168,   0, 168, 168,  84,   0, 168, 168, 168,
   84,  84,  84,  84,  84, 252,  84, 252,  84,  84, 252, 252,
  252,  84,  84, 252,  84, 252, 252, 252,  84, 252, 252, 252,
    0,   0,   0,  20,  20,  20,  32,  32,  32,  44,  44,  44,
   56,  56,  56,  68,  68,  68,  80,  80,  80,  96,  96,  96,
  112, 112, 112, 128, 128, 128, 144, 144, 144, 160, 160, 160,
  180, 180, 180, 200, 200, 200, 224, 224, 224, 252, 252, 252,
    0,   0, 252,  64,   0, 252, 124,   0, 252, 188,   0, 252,
  252,   0, 252, 252,   0, 188, 252,   0, 124, 252,   0,  64,
  252,   0,   0, 252,  64,   0, 252, 124,   0, 252, 188,   0,
  252, 252,   0, 188, 252,   0, 124, 252,   0,  64, 252,   0,
    0, 252,   0,   0, 252,  64,   0, 252, 124,   0, 252, 188,
    0, 252, 252,   0, 188, 252,   0, 124, 252,   0,  64, 252,
  124, 124, 252, 156, 124, 252, 188, 124, 252, 220, 124, 252,
  252, 124, 252, 252, 124, 220, 252, 124, 188, 252, 124, 156,
  252, 124, 124, 252, 156, 124, 252, 188, 124, 252, 220, 124,
  252, 252, 124, 220, 252, 124, 188, 252, 124, 156, 252, 124,
  124, 252, 124, 124, 252, 156, 124, 252, 188, 124, 252, 220,
  124, 252, 252, 124, 220, 252, 124, 188, 252, 124, 156, 252,
  180, 180, 252, 196, 180, 252, 216, 180, 252, 232, 180, 252,
  252, 180, 252, 252, 180, 232, 252, 180, 216, 252, 180, 196,
  252, 180, 180, 252, 196, 180, 252, 216, 180, 252, 232, 180,
  252, 252, 180, 232, 252, 180, 216, 252, 180, 196, 252, 180,
  180, 252, 180, 180, 252, 196, 180, 252, 216, 180, 252, 232,
  180, 252, 252, 180, 232, 252, 180, 216, 252, 180, 196, 252,
    0,   0, 112,  28,   0, 112,  56,   0, 112,  84,   0, 112,
  112,   0, 112, 112,   0,  84, 112,   0,  56, 112,   0,  28,
  112,   0,   0, 112,  28,   0, 112,  56,   0, 112,  84,   0,
  112, 112,   0,  84, 112,   0,  56, 112,   0,  28, 112,   0,
    0, 112,   0,   0, 112,  28,   0, 112,  56,   0, 112,  84,
    0, 112, 112,   0,  84, 112,   0,  56, 112,   0,  28, 112,
   56,  56, 112,  68,  56, 112,  84,  56, 112,  96,  56, 112,
  112,  56, 112, 112,  56,  96, 112,  56,  84, 112,  56,  68,
  112,  56,  56, 112,  68,  56, 112,  84,  56, 112,  96,  56,
  112, 112,  56,  96, 112,  56,  84, 112,  56,  68, 112,  56,
   56, 112,  56,  56, 112,  68,  56, 112,  84,  56, 112,  96,
   56, 112, 112,  56,  96, 112,  56,  84, 112,  56,  68, 112,
   80,  80, 112,  88,  80, 112,  96,  80, 112, 104,  80, 112,
  112,  80, 112, 112,  80, 104, 112,  80,  96, 112,  80,  88,
  112,  80,  80, 112,  88,  80, 112,  96,  80, 112, 104,  80,
  112, 112,  80, 104, 112,  80,  96, 112,  80,  88, 112,  80,
   80, 112,  80,  80, 112,  88,  80, 112,  96,  80, 112, 104,
   80, 112, 112,  80, 104, 112,  80,  96, 112,  80,  88, 112,
    0,   0,  64,  16,   0,  64,  32,   0,  64,  48,   0,  64,
   64,   0,  64,  64,   0,  48,  64,   0,  32,  64,   0,  16,
   64,   0,   0,  64,  16,   0,  64,  32,   0,  64,  48,   0,
   64,  64,   0,  48,  64,   0,  32,  64,   0,  16,  64,   0,
    0,  64,   0,   0,  64,  16,   0,  64,  32,   0,  64,  48,
    0,  64,  64,   0,  48,  64,   0,  32,  64,   0,  16,  64,
   32,  32,  64,  40,  32,  64,  48,  32,  64,  56,  32,  64,
   64,  32,  64,  64,  32,  56,  64,  32,  48,  64,  32,  40,
   64,  32,  32,  64,  40,  32,  64,  48,  32,  64,  56,  32,
   64,  64,  32,  56,  64,  32,  48,  64,  32,  40,  64,  32,
   32,  64,  32,  32,  64,  40,  32,  64,  48,  32,  64,  56,
   32,  64,  64,  32,  56,  64,  32,  48,  64,  32,  40,  64,
   44,  44,  64,  48,  44,  64,  52,  44,  64,  60,  44,  64,
   64,  44,  64,  64,  44,  60,  64,  44,  52,  64,  44,  48,
   64,  44,  44,  64,  48,  44,  64,  52,  44,  64,  60,  44,
   64,  64,  44,  60,  64,  44,  52,  64,  44,  48,  64,  44,
   44,  64,  44,  44,  64,  48,  44,  64,  52,  44,  64,  60,
   44,  64,  64,  44,  60,  64,  44,  52,  64,  44,  48,  64,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};



/**************************************************************************
*  Function:    main
*
*  Purpose:     Program entry point.
*
*  Entry:       argc = Number of parameters - 1.
*               argv = Array containing the actual parameters.
*
*  Exit:        N/A
**************************************************************************/

void main(int argc, char *argv[])
{
  //*** Set things up
  setup(argc, argv);

  //*** Run the demos
  lineDemo();
  putPixelDemo();
  drawRectDemo();
  filledRectDemo();
  ellipseDemo();
  filledEllipseDemo();
  circleDemo();
  filledCircleDemo();
  putImageDemo();
  fontDemo();
  viewportDemo();
  paletteDemo();
  scaleBitmapDemo();
  mouseDemo();

  //*** Get a key before quitting
  getKey();
}



/**************************************************************************
*  Function:    setup
*
*  Purpose:     Load drivers and fonts, and do other setup things.
*
*  Entry:       argc = Number of parameters - 1.
*               argv = Array containing the actual parameters.
*
*  Exit:        If an error occurs, the program will exit from within this
*               function.
**************************************************************************/

void setup(int argc, char *argv[])
{
  //*** Parse command line
  if (argc != 2)
  {
    printf("TGEDEMO 1.33  Copyright (c) 1993-1994 by Matthew Hildebrand\n\n"
           "        Usage:  TGEDEMO driver[.drv]\n\n"
           "     Examples:  tgedemo 320x200\n"
           "                tgedemo ..\\drivers\\640x480.drv\n"
           "                tgedemo d:\\tge\\drivers\\360x480\n\n"
           "         Note:  If you are running TGEDEMO for the first time,\n"
           "                you will need to initialize the TGEDRIVERS and\n"
           "                TGEFONTS environment variables.  Refer to\n"
           "                TGEDEMO.DOC for details.\n\n");
    exit(EXIT_FAILURE);
  }
  clrscr();                                 // clear the screen

  //*** Load the specified graphics driver
  switch (loadGraphDriver(argv[1]))
  {
    case TGE_SUCCESS:                       // success
      break;
    case TGE_OPEN_ERR:                      // file not found
      printf("Error opening driver file %s; aborting.\n\n"
             "Please ensure that the file specified is spelled correctly, and that the\n"
             "TGEDRIVERS environment variable has been set to point to the TGE drivers\n"
             "directory; refer to TGEDEMO.DOC for more information.\n\n",
             argv[1]);
      exit(EXIT_FAILURE);
    case TGE_FORMAT_ERR:                    // not a TGE driver
      printf("Format error processing file %s; aborting.\n\n", argv[1]);
      exit(EXIT_FAILURE);
    case TGE_ALLOC_ERR:                     // out of memory
      printf("Insufficient memory; aborting.\n\n");
      exit(EXIT_FAILURE);
    case TGE_FILE_ERR:                      // file read error
      printf("Error reading file %s; aborting.\n\n", argv[1]);
      exit(EXIT_FAILURE);
    default:                                // who knows?
      printf("Unknown error loading driver %s; aborting.\n\n", argv[1]);
      exit(EXIT_FAILURE);
  }
  atexit(unloadGraphDriver);                // unload driver on exit

  //*** Load the TGE logo bitmap
  tgeLogo = loadRawFile(TGELOGO_FILENAME);  // load it
  if (tgeLogo == NULL)                      // check for errors
  {
    printf("Error loading bitmap file %s; aborting.\n\n", TGELOGO_FILENAME);
    exit(EXIT_FAILURE);
  }

  //*** Switch to graphics mode
  if (!initGraphics())                      // initialize graphics mode
  {
    printf("Unable to initialize graphics hardware; aborting.\n\n"
           "Try installing the Universal VESA TSR before running this\n"
           "program again; refer to UNIVESA.DOC for more information.\n\n");
    exit(EXIT_FAILURE);
  }
  setBlockPalette(0, 255, palette);         // set the palette

  //*** Load the font
  if (font.load(FONT_FILENAME) == 0)
  {
    deInitGraphics();
    printf("Error loading font file %s; aborting.\n\n"
           "Please ensure that the file specified is spelled correctly, and that the\n"
           "TGEFONTS environment variable has been set to point to the TGE fonts\n"
           "directory; refer to TGEDEMO.DOC for more information.\n\n",
           FONT_FILENAME);
    exit(EXIT_FAILURE);
  }

  //*** Seed the random number generator
  randomize();

  //*** Sign off before exit
  atexit(signOff);                          // display sign-off message
  atexit(deInitGraphics);                   // restore text mode
}



/**************************************************************************
*  Function:    printMessage
*
*  Purpose:     Print a one-line message centered on the screen.
*
*  Entry:       string = Message to display
*               where = Flag indicating where to display message.  Can be
*                       Center or BottomCenter.
*
*  Exit:        N/A
**************************************************************************/

void printMessage(char *string, int where)
{
  switch (where)
  {
    case Center:
      font.put(((OUTVIEWPORTLRX-OUTVIEWPORTULX+1-font.width(string)) / 2),
              ((OUTVIEWPORTLRY-OUTVIEWPORTULY+1-font.maxHeight()) / 2),
               string);
      break;
    case BottomCenter:
      font.put(((OUTVIEWPORTLRX-OUTVIEWPORTULX+1-font.width(string)) / 2),
               OUTVIEWPORTLRY-font.maxHeight(),
               string);
      break;
    default:
      ;
  }
}



/**************************************************************************
*  Function:    lineDemo
*
*  Purpose:     Demonstrate line().
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void lineDemo(void)
{
  static char message[] = "TGE can draw lines...";
  int maxY, count;

  //*** Set up the message
  printMessage(message);                    // print it
  maxY = OUTMAXY - font.maxHeight() - 4;    // get usable limits

  //*** Draw lines until a key is struck
  while (!kbhit())
  {
    for (count=0; count<100; count++)
      line(random(OUTMAXX+1), random(maxY),
           random(OUTMAXX+1), random(maxY),
           random(MAXCOLOUR+1));
  }
  QUIT_IF_ESCAPE;                           // quit if ESC pressed
}



/**************************************************************************
*  Function:    putPixelDemo
*
*  Purpose:     Demonstrate putPixel().
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void putPixelDemo(void)
{
  static char message[] = "pixels...";
  int maxY, count;

  //*** Set up the message
  clearGraphics(0);                         // clear screen to black
  printMessage(message);                    // print it
  maxY = OUTMAXY - font.maxHeight() - 4;    // get usable limits

  //*** Draw pixels until a key is struck
  while (!kbhit())
  {
    for (count=0; count<2000; count++)
      putPixel(random(OUTMAXX+1), random(maxY), random(MAXCOLOUR+1));
  }
  QUIT_IF_ESCAPE;                           // quit if ESC pressed
}



/**************************************************************************
*  Function:    drawRectDemo
*
*  Purpose:     Demonstrate drawRect().
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void drawRectDemo(void)
{
  static char message[] = "rectangles...";
  int maxY, count;

  //*** Set up the message
  clearGraphics(0);                         // clear screen to black
  printMessage(message);                    // print it
  maxY = OUTMAXY - font.maxHeight() - 4;    // get usable limits

  //*** Draw rectangles until a key is struck
  while (!kbhit())
  {
    for (count=0; count<100; count++)
      drawRect(random(OUTMAXX+1), random(maxY),
               random(OUTMAXX+1), random(maxY),
               random(MAXCOLOUR+1));
  }
  QUIT_IF_ESCAPE;                           // quit if ESC pressed
}



/**************************************************************************
*  Function:    filledRectDemo
*
*  Purpose:     Demonstrate filledRectDemo().
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void filledRectDemo(void)
{
  static char message[] = "filled rectangles...";
  int maxY, count;

  //*** Set up the message
  clearGraphics(0);                         // clear screen to black
  printMessage(message);                    // print it
  maxY = OUTMAXY - font.maxHeight() - 4;    // get usable limits

  //*** Draw filled rectangles until a key is struck
  while (!kbhit())
  {
    for (count=0; count<15; count++)
      filledRect(random(OUTMAXX+1), random(maxY),
                 random(OUTMAXX+1), random(maxY),
                 random(MAXCOLOUR+1));
  }
  QUIT_IF_ESCAPE;                           // quit if ESC pressed
}



/**************************************************************************
*  Function:    ellipseDemo
*
*  Purpose:     Demonstrate ellipse().
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void ellipseDemo(void)
{
  static char message[] = "ellipses...";
  int maxY, count;

  //*** Set up the message
  clearGraphics(0);                         // clear screen to black
  printMessage(message);                    // print it
  maxY = OUTMAXY - font.maxHeight() - 4;    // get usable limits

  //*** Draw ellipses until a key is struck
  setOutputViewport(0, 0, SCREENMAXX, maxY);  // set the viewport
  while (!kbhit())
  {
    for (count=0; count<100; count++)
      ellipse(random(OUTMAXX+1), random(OUTMAXY+1),
              random((OUTMAXX+1)/8)*2+10, random((OUTMAXY+1)/8)*2+10,
              random(MAXCOLOUR+1));
  }
  QUIT_IF_ESCAPE;                           // quit if ESC pressed
  setOutputViewport(0, 0, SCREENMAXX, SCREENMAXY);  // reset viewport
}



/**************************************************************************
*  Function:    filledEllipseDemo
*
*  Purpose:     Demonstrate filledEllipse().
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void filledEllipseDemo(void)
{
  static char message[] = "filled ellipses...";
  int maxY, count;

  //*** Set up the message
  clearGraphics(0);                         // clear screen to black
  printMessage(message);                    // print it
  maxY = OUTMAXY - font.maxHeight() - 4;    // get usable limits

  //*** Draw filled ellipses until a key is struck
  setOutputViewport(0, 0, SCREENMAXX, maxY);  // set the viewport
  while (!kbhit())
  {
    for (count=0; count<30; count++)
      filledEllipse(random(OUTMAXX+1), random(OUTMAXY+1),
                    random((OUTMAXX+1)/8)*2+10, random((OUTMAXY+1)/8)*2+10,
                    random(MAXCOLOUR+1));
  }
  QUIT_IF_ESCAPE;                           // quit if ESC pressed
  setOutputViewport(0, 0, SCREENMAXX, SCREENMAXY);  // reset viewport
}



/**************************************************************************
*  Function:    circleDemo
*
*  Purpose:     Demonstrate circle().
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void circleDemo(void)
{
  static char message[] = "circles...";
  int maxY, count;

  //*** Set up the message
  clearGraphics(0);                         // clear screen to black
  printMessage(message);                    // print it
  maxY = OUTMAXY - font.maxHeight() - 4;    // get usable limits

  //*** Draw circles until a key is struck
  setOutputViewport(0, 0, SCREENMAXX, maxY);  // set the viewport
  while (!kbhit())
  {
    for (count=0; count<100; count++)
      circle(random(OUTMAXX+1), random(OUTMAXY+1),
             random((OUTMAXX+1)/8)+10,
             random(MAXCOLOUR+1));
  }
  QUIT_IF_ESCAPE;                           // quit if ESC pressed
  setOutputViewport(0, 0, SCREENMAXX, SCREENMAXY);  // reset viewport
}



/**************************************************************************
*  Function:    filledCircleDemo
*
*  Purpose:     Demonstrate filledCircle().
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void filledCircleDemo(void)
{
  static char message[] = "filled circles...";
  int maxY, count;

  //*** Set up the message
  clearGraphics(0);                         // clear screen to black
  printMessage(message);                    // print it
  maxY = OUTMAXY - font.maxHeight() - 4;    // get usable limits

  //*** Draw filled circles until a key is struck
  setOutputViewport(0, 0, SCREENMAXX, maxY);  // set the viewport
  while (!kbhit())
  {
    for (count=0; count<30; count++)
      filledCircle(random(OUTMAXX+1), random(OUTMAXY+1),
                   random((OUTMAXX+1)/8)+10,
                   random(MAXCOLOUR+1));
  }
  QUIT_IF_ESCAPE;                           // quit if ESC pressed
  setOutputViewport(0, 0, SCREENMAXX, SCREENMAXY);  // reset viewport
}



/**************************************************************************
*  Function:    putImageDemo
*
*  Purpose:     Demonstrate putImage().
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void putImageDemo(void)
{
  static char message[] = "images...";
  int maxY, curX, curY;

  //*** Set up the message
  clearGraphics(0);                         // clear screen to black
  printMessage(message);                    // print it
  maxY = OUTMAXY - font.maxHeight() - 4;    // get usable limits

  //*** Set the viewport to exclude the message area
  setOutputViewport(0, 0, SCREENMAXX, maxY);

  //*** Draw the TGE logo all over the background
  curY = -(((maxY+1)%imageHeight(tgeLogo)) / 2);    // starting y-coordinate
  curX = -(((OUTMAXX+1)%imageWidth(tgeLogo)) / 2);  // starting x-coordinate
  do
  {
    do
    {
      putImage(curX, curY, tgeLogo);        // draw the logo
      curX += imageWidth(tgeLogo);          // update x-coordinate
    }
    while (curX <= OUTMAXX);
    curY += imageHeight(tgeLogo);           // update y-coordinate
    curX = -(((OUTMAXX+1)%imageWidth(tgeLogo)) / 2);  // starting x-coordinate
  }
  while (curY <= maxY);
  QUIT_IF_ESCAPE;                           // quit if ESC pressed

  //*** Reset the viewport to cover the whole screen
  setOutputViewport(0, 0, SCREENMAXX, SCREENMAXY);
}



/**************************************************************************
*  Function:    fontDemo
*
*  Purpose:     Demonstrate fonts.
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void fontDemo(void)
{
  static char message[] = "and, of course, text.";

  //*** Set up the message
  clearGraphics(0);                         // clear screen to black
  printMessage(message, Center);            // print it
  QUIT_IF_ESCAPE;                           // quit if ESC pressed
}



/**************************************************************************
*  Function:    viewportDemo
*
*  Purpose:     Demonstrate viewports.
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void viewportDemo(void)
{
  static char message[] = "Output can be clipped.";
  VirtualCoord virtScreen;
  int x1, y1, x2, y2;

  //*** Set up the message
  font.palette(1, 255, 255, 255);           // update font colour
  clearGraphics(colourCloseTo(90,90,90));   // clear screen to black
  printMessage(message);                    // print it

  //*** Configure the virtual coordinate system
  virtScreen.virtParams(4, 4);
  virtScreen.realParams(OUTMAXX, OUTMAXY);

  //*** Set the viewport to exclude the message area
  x1 = virtScreen.realX(1);
  y1 = virtScreen.realY(1);
  x2 = virtScreen.realX(3);
  y2 = virtScreen.realY(3);
  setOutputViewport(x1, y1, x2, y2);
  filledRect(x1, y1, x2, y2, colourCloseTo(0,0,0));

  //*** Draw random things at random places.
  while (!kbhit())
  {
    switch (random(8))                      // choose a thing to draw
    {
      case 0:                               // line()
	x1 = random(OUTMAXX+1);
	y1 = random(OUTMAXY+1);
	x2 = random(OUTMAXX+1);
	y2 = random(OUTMAXY+1);
	if (clipLine(&x1, &y1, &x2, &y2))
          line(x1, y1, x2, y2, random(MAXCOLOUR+1));
	break;
      case 1:                               // ellipse()
        ellipse(random(OUTMAXX+1), random(OUTMAXY+1),
                random(OUTMAXX/3), random(OUTMAXY/3),
                random(MAXCOLOUR+1));
	break;
      case 2:                               // filledEllipse();
        filledEllipse(random(OUTMAXX+1), random(OUTMAXY+1),
                      random(OUTMAXX/3), random(OUTMAXY/3),
                      random(MAXCOLOUR+1));
	break;
      case 3:                               // circle()
	circle(random(OUTMAXX+1), random(OUTMAXY+1), random(OUTMAXX/3),
                random(MAXCOLOUR+1));
	break;
      case 4:                               // filledCircle()
	filledCircle(random(OUTMAXX+1), random(OUTMAXY+1), random(OUTMAXX/3),
                random(MAXCOLOUR+1));
	break;
      case 5:                               // putImageInv()
	putImageInv(random(OUTMAXX+1), random(OUTMAXY+1), tgeLogo);
	break;
      case 6:                               // filledRect()
	x1 = random(OUTMAXX+1);
	y1 = random(OUTMAXY+1);
	x2 = random(OUTMAXX+1);
	y2 = random(OUTMAXY+1);
	if (clipFilledRect(&x1, &y1, &x2, &y2))
          filledRect(x1, y1, x2, y2, random(MAXCOLOUR+1));
	break;
      case 7:
	drawRect(random(OUTMAXX+1), random(OUTMAXY+1), random(OUTMAXX+1),
                random(OUTMAXY+1), random(MAXCOLOUR+1));
	break;
      default:
	;
    }
  }
  QUIT_IF_ESCAPE;                           // quit if ESC pressed

  //*** Reset the viewport to cover the whole screen
  setOutputViewport(0, 0, SCREENMAXX, SCREENMAXY);
}



/**************************************************************************
*  Function:    paletteDemo
*
*  Purpose:     Demonstrate palette fading and rotation.
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void paletteDemo(void)
{
  static char message[] = "TGE has other powerful features...";
  unsigned char inPal[768], outPal[768], targetPalA[768], targetPalB[768];
  void *in, *out, *temp;
  int count, stillFadingFlag=1;

  //*** Introduce the next two demos
  clearGraphics(0);                         // clear screen to black
  printMessage(message, Center);            // print it
  QUIT_IF_ESCAPE;                           // quit if ESC pressed

  //*** Initialize the palettes
  memcpy(targetPalA, palette, 768);         // copy target palette into buffer
  memset(inPal, 0x00, 768);                 // black starting palette
  setBlockPalette(0, 255, inPal);           // set palette to black

  //*** Fill the screen with horizontal lines
  for (count=0; count<=OUTMAXY; count++)
    horizLine(count, 0, OUTMAXX, count&0xFF);

  //*** Rotate the palette until a key is pressed
  in = (void*) inPal;                       // point to input buffer
  out = (void*) outPal;                     // point to output buffer
  while (!kbhit())
  {
    if (stillFadingFlag)                    // fade the palette in
    {
      stillFadingFlag = fadePalette(1, in, out, (void*)targetPalA);
      memcpy(in, out, 768);
    }
    delay(10);                              // delay for a bit
    rotatePalette(1, in, out);              // rotate the palette
    rotatePalette(1, targetPalA, targetPalB); // rotate target palette
    memcpy(targetPalA, targetPalB, 768);    // update target palette
    waitVerticalRetrace();                  // avoid flicker
    setBlockPalette(0, 255, out);           // update the palette
    temp = out;                             // swap input and output pointers
    out = in;
    in = temp;
  }

  QUIT_IF_ESCAPE;                           // quit if ESC pressed
  setBlockPalette(0, 255, palette);         // restore palette
}



/**************************************************************************
*  Function:    scaleBitmapDemo
*
*  Purpose:     Demonstrate scaleBitmap.
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void scaleBitmapDemo(void)
{
  void far *image;
  unsigned count;

  //*** Grab a block of memory
  if ((image=farmalloc(imageSizeDim(200,200))) == NULL)
  {
    deInitGraphics();
    printf("Insufficient memory; aborting.\n\n"
           "Press a key to continue...");
    getKey();
    clrscr();
    exit(EXIT_FAILURE);
  }

  //*** Make image grow from center of screen
  clearGraphics(0);                         // clear screen to black
  for (count=1; count<=200&&!kbhit(); count++)
  {
    scaleBitmap(tgeLogo, count, count, image);  	 // scale it
    putImage((MAXX+1-count)/2, (MAXY+1-count)/2, image); // draw it
  }

  //*** Clean up and go home
  farfree(image);                           // release memory
  if (kbhit())                              // if a key was pressed, then
    QUIT_IF_ESCAPE;                         // quit if ESC pressed
}



/**************************************************************************
*  Function:    mouseDemo
*
*  Purpose:     Demonstrate the use of the mouse.
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void mouseDemo(void)
{
  int pointerNum;

  //*** Ensure that a mouse is installed
  if (!resetMouse())
  {
    deInitGraphics();
    printf("Microsoft or compatible mouse not detected.\n"
	   "The demo for definable, device-independent pointers will not be run.\n\n"
	   "Press a key to continue...");
    getKey();
    clrscr();
    exit(EXIT_SUCCESS);
  }

  //*** Initialize the mouse handler
  initNewMouse();                           // initialize handler
  setHorizLimitsMouse(0, OUTMAXX);          // set horizontal limits
  setVertLimitsMouse(0, OUTMAXY);           // set vertical limits
  setPosMouse(OUTMAXX/2, OUTMAXY/2);        // center pointer on-screen
  pointerNum = BIG_ARROW_POINTER;           // signal using BIG_ARROW_POINTER
  setupMousePointer(pointerNum);            // make BIG_ARROW_POINTER active
  showMouse();                              // show the pointer

  //*** Main loop
  while (!kbhit())
  {
    //*** Button pressed; change to BIG_TARGET_POINTER
    if (buttonMouse() && pointerNum!=BIG_TARGET_POINTER)
    {
      pointerNum = BIG_TARGET_POINTER;      // signal using BIG_TARGET_POINTER
      setupMousePointer(pointerNum);        // make BIG_TARGET_POINTER active
    }
    //*** Button released; change to BIG_ARROW_POINTER
    else if (!buttonMouse() && pointerNum==BIG_TARGET_POINTER)
    {
      pointerNum = BIG_ARROW_POINTER;       // signal using BIG_ARROW_POINTER
      setupMousePointer(pointerNum);        // make BIG_ARROW_POINTER active
    }
  }

  //*** Shut off mouse handler
  deInitNewMouse();
}



/**************************************************************************
*  Function:    signOff
*
*  Purpose:     Print a sign-off message.
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void signOff(void)
{
  static char notice[] =
  {
    "ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿"
    "³ The Graphics Engine 1.33 Demo  Copyright (c) 1993-1994 by Matthew Hildebrand ³"
    "ÆÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍµ"
    "³ Although this program is not particularly gripping, it demonstrates TGE's    ³"
    "³ speed and most of its many features.  TGE is especially suited to device-    ³"
    "³ independence, as can be seen by running this demo with different drivers,    ³"
    "³ though it may of course be used in single-mode programs as well.             ³"
    "³                                                                              ³"
    "³ TGE has many features, including:                                            ³"
    "³      þ Use of loadable graphics drivers and loadable fonts                   ³"
    "³      þ A powerful set of graphical functions                                 ³"
    "³      þ Support for viewports, or clipping regions                            ³"
    "³      þ Support for virtual screens of varying sizes                          ³"
    "³      þ Extensive mouse support, including definable pointers                 ³"
    "³      þ Graphics output using COPY, AND, NOT, OR, and XOR                     ³"
    "³      þ Native support for both PCX files and TGE-format RAW files            ³"
    "³                                                                              ³"
    "³ TGE costs only $30 US funds or $40 Canadian funds; paid users are entitled   ³"
    "³ to free upgrades, technical support, royalty-free distribution rights,       ³"
    "³ complete source code, and eternal happiness.  Refer to TGE.DOC for details.  ³"
    "ÆÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍµ"
    "³ Matthew Hildebrand, 4 College St., St. Catharines, Ontario, Canada, L2R 2W7  ³"
    "ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ"
  };

  printf(notice);                           // print the message
}



/**************************************************************************
*  Function:    getKey
*
*  Purpose:     Get a character from the keyboard.  This function differs
*               from getch() in that it can correctly handle extended keys
*               such as the function keys.
*
*  Entry:       N/A
*
*  Exit:        Returns the keycode of the key pressed.
**************************************************************************/

int getKey(void)
{
  int ch;

  ch = getch();                             // get a byte
  if (!(ch & 0xFF))                         // is it zero?
    ch = getch() << 8;                      // yes, get another byte
  return (ch);                              // return the keycode
}



/**************************************************************************
*  Function:    waitVerticalRetrace
*
*  Purpose:     Wait for the start of a new vertical retrace.
*
*  Entry:       N/A
*
*  Exit:        N/A
**************************************************************************/

void waitVerticalRetrace(void)
{
  //*** Wait for any current vertical retrace to end.
  asm mov   dx,3DAh
          WaitVerticalRetrace1:
  asm in    al,dx                           // get status
  asm and   al,08h
  asm jnz   WaitVerticalRetrace1            // loop if necessary

  //*** Wait for the start of the next vertical retrace.
  asm mov   dx,3DAh
          WaitVerticalRetrace2:
  asm in    al,dx                           // get status
  asm and   al,08h
  asm jz    WaitVerticalRetrace2            // loop if necessary
}
