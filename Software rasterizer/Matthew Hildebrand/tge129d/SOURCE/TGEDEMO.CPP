/*****************************************************************************
*       The Graphics Engine version 1.29แD                                   *
*                                                                            *
*       The Graphics Engine code and documentation are Copyright (c) 1993    *
*       by Matthew Hildebrand.                                               *
*                                                                            *
*       Unauthorised usage or modification of any or all of The Graphics     *
*       Engine is strictly prohibited.                                       *
*****************************************************************************/

#include <alloc.h>
#include <conio.h>
#include <dir.h>
#include <dos.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tgemouse.h"
#include "tgefont.h"
#include "tge.h"


#ifndef __TURBOC__
  #define random(num)		(rand() % num)
  #define randomize()      	srand((unsigned)time(NULL))
#endif

#define ESC	27


void setup(int argc, char *argv[]);
void lineDemo(void);
void putPixelDemo(void);
void drawRectDemo(void);
void filledRectDemo(void);
void ellipseDemo(void);
void filledEllipseDemo(void);
void circleDemo(void);
void filledCircleDemo(void);
void viewportDemo(void);
void setupViewport(void);
void viewportPutImageDemo(int delayTime);
void viewportRandomDemo(void);
void scaleBitmapDemo(void);
void paletteDemo(void);
void mouseDemo(void);
void cycleColour(int *colour, int *increment);
void forceExtension(char *filename, char *extension);
void signOff(void);
//void showScreen(void);
void quitIfEsc(void);


unsigned char tgeLogo[] = {
  24,0,9,0,
  0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,
  0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,
  0,0,1,1,0,0,0,0,1,1,0,0,1,1,1,0,1,1,1,1,1,0,0,0,
  0,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,0,0,0,
  0,1,1,0,0,0,0,1,1,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,
  0,1,1,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,0,
  1,1,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,0,0
};
#define LOGOWIDE	((unsigned*)tgeLogo)[0]
#define LOGODEEP	((unsigned*)tgeLogo)[1]


Font font("8x16.fnt");
int count, colour;
void far *bground, *screen;


//*****
//***** Program entry
//*****

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
  viewportDemo();
  scaleBitmapDemo();
  paletteDemo();
  mouseDemo();

  deInitGraphics();			// restore text and quit
//  farfree(screen);
}


//*****
//***** Set up at program start
//*****

void setup(int argc, char *argv[])
{
  char filename[80];

  //*** Parse command line
  if (argc < 2)
  {
    printf("TGEDEMO 1.29แD  Copyright (c) 1993 by Matthew Hildebrand\n\n"
	   "          Usage:  TGEDEMO driver\n\n"
	   "       Examples:  tgedemo 640x480\n"
	   "                  tgedemo ..\\320x200.drv\n"
	   "                  tgedemo e:\\tge\\360x480\n\n");
    exit(1);
  }

  //*** Ensure font loaded correctly
  if (!font.status())
  {
    printf("Error loading 8X16.FNT\n\n");
    exit(1);
  }

  //*** Load the graphics driver
  strcpy(filename, argv[1]);
  forceExtension(filename, ".DRV");
  if (loadGraphDriver(filename) != TGE_SUCCESS)
  {
    printf("Error loading \"%s\".\n\n", filename);
    exit(1);
  }
  else
    atexit(unloadGraphDriver);

  //*** Allocate some temporary memory
  bground = farmalloc(imageSizeDim(24,9));

  //*** Initialize graphics mode
  if (!initGraphics())
  {
    printf("Unable to initialize graphics hardware.\n\n");
    exit(1);
  }

  //*** Set font foreground colour
  font.foreground(colourCloseToX(255,255,255,0));

  //*** Create a virtual screen
//  screen = makeVirtScreen(OUTMAXX+1, OUTMAXY+1);
//  if (screen == NULL)
//  {
//    deInitGraphics();
//    printf("Not enough memory to hold virtual screen.\n\n");
//    exit(1);
//  }
//  setGraphicsAddr(screen);
//  clearGraphics(0);

  //*** Seed the random number generator
  randomize();

  //*** Set up atexit() queue
  atexit(signOff);				// sign-off screen
}


//*****
//***** line() demo
//*****

void lineDemo(void)
{
  static char str[] = "TGE supports many functions.";
  int depth = font.deep(str);

  font.put((OUTMAXX+1)/2-font.wide(str)/2, OUTMAXY-depth, str);

  for (; !kbhit();)
    line(random(OUTMAXX+1), random(OUTMAXY-depth), random(OUTMAXX+1),
	random(OUTMAXY-depth), random(MAXCOLOUR));

  quitIfEsc();

  colour = random(MAXCOLOUR);
  for (count=OUTMAXX; count>=0&&!kbhit(); count-=2)
  {
    line(0, 0, OUTMAXX-count, OUTMAXY, colour);
    line(OUTMAXX, OUTMAXY, count, 0, colour);
  }

  if (kbhit())
    quitIfEsc();
  clearGraphics(random(MAXCOLOUR));
}


//*****
//***** putPixel() demo
//*****

void putPixelDemo(void)
{
  for (; !kbhit();)
  {
    for (count=0; count<2000; count++)
      putPixel(random(OUTMAXX+1), random(OUTMAXY+1), random(MAXCOLOUR));
  }

  quitIfEsc();
  clearGraphics(random(MAXCOLOUR));
}


//*****
//***** drawRect() demo
//*****

void drawRectDemo(void)
{
  while (!kbhit())
    drawRect(random(OUTMAXX+1), random(OUTMAXY+1), random(OUTMAXX+1),
		random(OUTMAXY+1), random(MAXCOLOUR));

  quitIfEsc();
  clearGraphics(random(MAXCOLOUR));
}


//*****
//***** filledRect() demo
//*****

void filledRectDemo(void)
{
  while (!kbhit())
    filledRect(random(OUTMAXX+1), random(OUTMAXY+1), random(OUTMAXX+1),
		random(OUTMAXY+1), random(MAXCOLOUR));

  quitIfEsc();
  clearGraphics(random(MAXCOLOUR));
}


//*****
//***** ellipse() demo
//*****

void ellipseDemo(void)
{
  while (!kbhit())
    ellipse(random(OUTMAXX+1), random(OUTMAXY+1), random(OUTMAXX/8)*2+10,
		random(OUTMAXY/8)*2+10, random(MAXCOLOUR));

  quitIfEsc();
  clearGraphics(random(MAXCOLOUR));
}


//*****
//***** filledEllipse() demo
//*****

void filledEllipseDemo(void)
{
  while (!kbhit())
    filledEllipse(random(OUTMAXX+1), random(OUTMAXY+1), random(OUTMAXX/8)*2+10,
	random(OUTMAXY/8)*2+10, random(MAXCOLOUR));

  quitIfEsc();
  clearGraphics(random(MAXCOLOUR));
}


//*****
//***** circle() demo
//*****

void circleDemo(void)
{
  while (!kbhit())
    circle(random(OUTMAXX+1), random(OUTMAXY+1), random(OUTMAXX/8)+10,
		random(MAXCOLOUR));

  quitIfEsc();
  clearGraphics(random(MAXCOLOUR));
}


//*****
//***** filledCircle() demo
//*****

void filledCircleDemo(void)
{
  while (!kbhit())
    filledCircle(random(OUTMAXX+1), random(OUTMAXY+1), random(OUTMAXX/8)+10,
		random(MAXCOLOUR));

  quitIfEsc();
  clearGraphics(0);
}


//*****
//***** Viewport demo
//*****

void viewportDemo(void)
{
  int x1, y1, x2, y2;
  static char str1[] = "Viewports can be very useful.";
  static char str2[] = "(Now that it's slower, the";
  static char str3[] = "image's features can be seen.)";
  static char str4[] = "Many functions work with viewports,";
  static char str5[] = "and at considerable speeds.";

  x1 = 2 * (OUTMAXX+1)/5;
  y1 = 2 * (OUTMAXY+1)/5;
  x2 = OUTMAXX - x1;
  y2 = OUTMAXY - y1;

  setupViewport();
  font.put((OUTMAXX+1)/2-font.wide(str1)/2, (OUTMAXY+1)/10, str1);
  setViewports(x1, y1, x2, y2);
  viewportPutImageDemo(0);

  setupViewport();
  font.put((OUTMAXX+1)/2-font.wide(str2)/2, (OUTMAXY+1)/10, str2);
  font.put((OUTMAXX+1)/2-font.wide(str3)/2, (OUTMAXY+1)/10+font.deep(str1),
	str3);
  setViewports(x1, y1, x2, y2);
  viewportPutImageDemo(12);
  setViewports(0, 0, OUTMAXX, OUTMAXY);

  x1 = (OUTMAXX+1)/5;			// make a larger viewport
  y1 = (OUTMAXY+1)/5;
  x2 = OUTMAXX - x1;
  y2 = OUTMAXY - y1;
  clearGraphics(colourCloseTo(90,90,90));
  filledRect(x1, y1, x2, y2, colourCloseTo(0,0,0));
  font.put((OUTMAXX+1)/2-font.wide(str4)/2, (OUTMAXY+1)/30, str4);
  font.put((OUTMAXX+1)/2-font.wide(str5)/2, (OUTMAXY+1)/30+font.deep(str4), str5);
  setViewports(x1, y1, x2, y2);
  viewportRandomDemo();

  setViewports(0, 0, OUTMAXX, OUTMAXY);		// restore the viewport
  clearGraphics(colourCloseTo(0,0,0));
}


//*****
//***** Set up the viewport
//*****

void setupViewport(void)
{
  int x1, y1, x2, y2;

  x1 = 2 * (OUTMAXX+1)/5;
  y1 = 2 * (OUTMAXY+1)/5;
  x2 = OUTMAXX - x1;
  y2 = OUTMAXY - y1;

  clearGraphics(colourCloseTo(90,90,90));
  filledRect(x1, y1, x2, y2, colourCloseTo(0,0,0));
  setViewports(x1, y1, x2, y2);                  // set viewport

  // center
  putImage((x2+x1+1)/2-LOGOWIDE/2, (y2+y1+1)/2-LOGODEEP/2, tgeLogo);
  // upper left
  putImage(x1-LOGOWIDE/2, y1-LOGODEEP/2, tgeLogo);
  // upper middle
  putImage(((x2+x1+1)/2)-LOGOWIDE/2, y1-LOGODEEP/2, tgeLogo);
  // upper right
  putImage(x2-LOGOWIDE/2, y1-LOGODEEP/2, tgeLogo);
  // middle right
  putImage(x2-LOGOWIDE/2, (y2+y1+1)/2-LOGODEEP/2, tgeLogo);
  // lower right
  putImage(x2-LOGOWIDE/2, y2-LOGODEEP/2, tgeLogo);
  // lower middle
  putImage((x2+x1+1)/2-LOGOWIDE/2, y2-LOGODEEP/2, tgeLogo);
  // lower left
  putImage(x1-LOGOWIDE/2, y2-LOGODEEP/2, tgeLogo);
  // middle left
  putImage(x1-LOGOWIDE/2, (y2+y1+1)/2-LOGODEEP/2, tgeLogo);

  setViewports(0, 0, OUTMAXX, OUTMAXY);
}


//*****
//***** Viewport and putImage() demo
//*****

void viewportPutImageDemo(int delayTime)
{
  int x, y;

  // moving putImageInv() demo (clipped)
  x = (OUTMAXX+1)/2 - LOGOWIDE/2;
  y = (OUTMAXY+1)/2 - LOGODEEP/2;

  while (!kbhit())
  {
    getImage(x, y, x+LOGOWIDE-1, y+LOGODEEP-1, bground);
    putImageInv(x, y, tgeLogo);
    if (delayTime)
      delay(delayTime);
    putImage(x>=VIEWPORTULX?x:VIEWPORTULX, y>=VIEWPORTULY?y:VIEWPORTULY,
	bground);

    if (random(201) > 100)
      x = x<VIEWPORTLRX+LOGOWIDE/2 ? x+1 : x-1;
    else
      x = x>VIEWPORTULX-LOGOWIDE/2 ? x-1 : x+1;

    if (random(201) > 100)
      y = y<VIEWPORTLRY+LOGODEEP/2 ? y+1 : y-1;
    else
      y = y>VIEWPORTULY-LOGODEEP/2 ? y-1 : y+1;
  }

  quitIfEsc();
}


//*****
//***** A bit of everything in a viewport
//*****

void viewportRandomDemo(void)
{
  int type;
  int x1, y1, x2, y2;

  while (!kbhit())
  {
    type = random(8);
    switch (type)
    {
      case 0:				// line()
	x1 = random(OUTMAXX+1);
	y1 = random(OUTMAXY+1);
	x2 = random(OUTMAXX+1);
	y2 = random(OUTMAXY+1);
	if (clipLine(&x1, &y1, &x2, &y2))
	  line(x1, y1, x2, y2, random(MAXCOLOUR));
	break;
      case 1:				// ellipse()
	ellipse(random(OUTMAXX+1), random(OUTMAXY+1), random(OUTMAXX/3),
		random(OUTMAXY/3),	random(MAXCOLOUR));
	break;
      case 2:				// filledEllipse();
	filledEllipse(random(OUTMAXX+1), random(OUTMAXY+1), random(OUTMAXX/3),
		random(OUTMAXY/3), random(MAXCOLOUR));
	break;
      case 3:				// circle()
	circle(random(OUTMAXX+1), random(OUTMAXY+1), random(OUTMAXX/3),
		random(MAXCOLOUR));
	break;
      case 4:				// filledCircle()
	filledCircle(random(OUTMAXX+1), random(OUTMAXY+1), random(OUTMAXX/3),
		random(MAXCOLOUR));
	break;
      case 5:				// putImageInv()
	putImageInv(random(OUTMAXX+1), random(OUTMAXY+1), tgeLogo);
	break;
      case 6:				// filledRect()
	x1 = random(OUTMAXX+1);
	y1 = random(OUTMAXY+1);
	x2 = random(OUTMAXX+1);
	y2 = random(OUTMAXY+1);
	if (clipFilledRect(&x1, &y1, &x2, &y2))
	  filledRect(x1, y1, x2, y2, random(MAXCOLOUR));
	break;
      case 7:
	drawRect(random(OUTMAXX+1), random(OUTMAXY+1), random(OUTMAXX+1),
		random(OUTMAXY+1), random(MAXCOLOUR));
	break;
      default:
	;
    }
  };

  quitIfEsc();
}


//*****
//***** scaleBitmap() demo
//*****

void scaleBitmapDemo(void)
{
  void far *image;
  register unsigned count;

  //*** Grab a block of memory
  image = farmalloc(imageSizeDim(200, 200));
  if (image == NULL)
    return;

  //*** Make image grow from center of screen
  for (count=1; count<=200&&!kbhit(); count++)
  {
    scaleBitmap(tgeLogo, count, count, image);  	 // scale it
    putImage((MAXX+1-count)/2, (MAXY+1-count)/2, image); // draw it
  }

  //*** Release memory
  farfree(image);

  quitIfEsc();
  setGraphicsAddr(NULL);
  clearGraphics(0);
}


//*****
//***** Palette demo
//*****

#define MAXINC	7

void paletteDemo(void)
{
  int redInc, greenInc, blueInc;
  int red=0, green=0, blue=0;
  colour = 1;

  redInc = random(MAXINC);
  redInc++;
  greenInc = random(MAXINC);
  greenInc++;
  blueInc = random(MAXINC);
  blueInc++;

  for (count=OUTMAXX; count>=0&&!kbhit(); count-=2) {
    line(0, 0, OUTMAXX-count, OUTMAXY, colour);
    line(OUTMAXX, OUTMAXY, count, 0, colour);
    cycleColour(&red, &redInc);
    cycleColour(&green, &greenInc);
    cycleColour(&blue, &blueInc);
    setPaletteReg(0, red, green, blue);
  }

  if (kbhit() && getch()==ESC)
  {
    deInitGraphics();
    exit(0);
  }

  setPaletteReg(0, 0, 0, 0);
  clearGraphics(1);
}

void cycleColour(int *colour, int *increment)
{
  *colour += *increment;

  if (*colour < 0)
  {
    *colour = 0;
    *increment = random(MAXINC) + 1;
  }
  else if (*colour > 255)
  {
    *colour = 255;
    *increment *= -1;
  }
}


//*****
//***** Mouse demo
//*****

void mouseDemo(void)
{
  int pointerNum;

  if (!resetMouse())			// check if mouse is installed
  {
    deInitGraphics();
    unloadGraphDriver();
    printf("Microsoft or compatible mouse not detected.\n"
	   "The demo for definable, device-independent pointers will not be run.\n\n"
	   "Press a key to continue...");
    getch();
    clrscr();
    exit(0);
  }

  initNewMouse();			// initialize new mouse handler
  setHorizLimitsMouse(0, OUTMAXX);
  setVertLimitsMouse(0, OUTMAXY);
  setPosMouse(OUTMAXX/2, OUTMAXY/2);
  pointerNum = BIG_ARROW_POINTER;
  setupMousePointer(pointerNum);
  showMouse();

  for (; !kbhit();)			// main loop
  {
    if (buttonMouse() && pointerNum!=BIG_TARGET_POINTER)
    {
      pointerNum = BIG_TARGET_POINTER;
      hideMouse();
      setupMousePointer(pointerNum);
      showMouse();
    }
    else if (!buttonMouse() && pointerNum==BIG_TARGET_POINTER)
    {
      pointerNum = BIG_ARROW_POINTER;
      hideMouse();
      setupMousePointer(pointerNum);
      showMouse();
    }
  }
  getch();				// flush keyboard buffer

  deInitNewMouse();			// shut off new mouse handler
}


//*****
//***** Miscellaneous routines
//*****

void forceExtension(char *filename, char *extention)
{
  char fileDrive[MAXDRIVE];
  char fileDir[MAXDIR];
  char fileFile[MAXFILE];
  char fileExt[MAXEXT];

  fnsplit(filename, fileDrive, fileDir, fileFile, fileExt);

  if (strcmpi(fileExt, extention))
    fnmerge(filename, fileDrive, fileDir, fileFile, extention);
}


void signOff(void)
{
  static char notice[] = {
    "ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ"
    "ณ The Graphics Engine 1.29แD Demo     Copyright (c) 1993 by Matthew Hildebrand ณ"
    "ฦออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออต"
    "ณ Although this demo is not particularly gripping, the power of TGE can be     ณ"
    "ณ seen when it uses different drivers:  the demo uses the same code to access  ณ"
    "ณ different graphics modes.  Even if you don't want device-independence, but   ณ"
    "ณ only one graphics mode, TGE's optimized assembler routines may be used.      ณ"
    "ณ                                                                              ณ"
    "ณ TGE has many features, including:                                            ณ"
    "ณ      þ Use of loadable graphics drivers and loadable fonts                   ณ"
    "ณ      þ A powerful set of graphical functions                                 ณ"
    "ณ      þ Support for viewports, or clipping regions                            ณ"
    "ณ      þ Support for virtual screens of varying sizes                          ณ"
    "ณ      þ Extensive mouse support, including definable pointers                 ณ"
    "ณ      þ Graphics output using COPY, AND, NOT, OR, and XOR                     ณ"
    "ณ      þ A utility to convert PCX files to TGE-format images                   ณ"
    "ณ                                                                              ณ"
    "ณ TGE costs non-commercial users a mere $30; paid users are entitled to free   ณ"
    "ณ upgrades, technical support, royalty-free distribution rights, and eternal   ณ"
    "ณ happiness.  Refer to TGE.DOC for details.                                    ณ"
    "ฦออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออต"
    "ณ Matthew Hildebrand, 4 College St., St. Catharines, Ontario, Canada, L2R 2W7  ณ"
    "ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู"
    "\nPress a key to continue..."
  };

  printf(notice);

  getch();
  clrscr();
  printf("*** ATTENTION ***\n\n"
	 "This version of TGE (1.29แD) is a PUBLIC BETA version.  It contains a\n"
	 "few known bugs, and is not yet entirely completed; for instance, TGEDEMO\n"
	 "currently does not reflect all of TGE's features.  TGE 1.30 will be\n"
	 "released when the problems in 1.29แD have been rectified.  Until then,\n"
	 "TGE 1.29แD should be reliable enough for most purposes.\n\n");
}


//void showScreen(void)
//{
//  setGraphicsAddr(NULL);
//  putImage(0, 0, screen);
//  setGraphicsAddr(screen);
//}

void quitIfEsc(void)
{
//  showScreen();
  if (getch() == ESC)
  {
    deInitGraphics();
    exit(0);
  }
}
