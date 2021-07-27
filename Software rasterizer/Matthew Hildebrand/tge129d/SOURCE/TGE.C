/*****************************************************************************
*       The Graphics Engine version 1.29·D                                   *
*                                                                            *
*       The Graphics Engine code and documentation are Copyright (c) 1993    *
*       by Matthew Hildebrand.                                               *
*                                                                            *
*       Unauthorised usage or modification of any or all of The Graphics     *
*       Engine is strictly prohibited.                                       *
*****************************************************************************/

#include <alloc.h>
#include <dos.h>
#include <stdlib.h>
#include "tge.h"

static unsigned char far* near inPixelAddr(int x, int y);
static unsigned char far* near outPixelAddr(int x, int y);
static void near set4Pixels(int x, int y, int xc, int yc, unsigned colour);
static void near fill4Pixels(int x, int y, int xc, int yc, unsigned colour);


/******
******* High-level routines to be used if they aren't in a driver.
******/


/***
**** deInitGraphics
***/
void TGE_deInitGraphics(void)
{
  _AX = 0x0003;
  geninterrupt(0x10);
}


/***
**** setPaletteReg
***/
void TGE_setPaletteReg(unsigned palReg, unsigned char red, unsigned char
	blue, unsigned char green)
{
  unsigned char r, g, b;

  r = red >> 2;				/* translate 8-bit to 6-bit */
  if ((red&3) >= 2)			/* round up if necessary */
    r++;
  if (r > 63)				/* ensure in range 0..63 */
    r = 63;

  g = green >> 2;
  if ((green&3) >= 2)
    g++;
  if (g > 63)
    g = 63;

  b = blue >> 2;
  if ((blue&3) >= 2)
    b++;
  if (b > 63)
    b = 63;

  _AX = 0x1010;				/* set up registers */
  _BX = palReg;
  _DH = r;
  _CH = g;
  _CL = b;
  geninterrupt(0x10);			/* call the video BIOS */
}


/***
**** getPaletteReg
***/
void TGE_getPaletteReg(unsigned palReg, unsigned char *red, unsigned char
	*blue, unsigned char *green)
{
  unsigned char r, g, b;

  _AX = 0x1015;				/* set up registers */
  _BX = palReg;
  geninterrupt(0x10);			/* call the video BIOS */
  r = _DH;
  g = _CH;
  b = _CL;

  *red = r << 2;			/* translate 6-bit to 8-bit */
  *green = g << 2;			/* and store new values */
  *blue = b << 2;
}


#ifdef __BORLANDC__
  #pragma option -h
#endif

/***
**** setBlockPalette
***/
void TGE_setBlockPalette(unsigned firstReg, unsigned lastReg, void far *data)
{
  register unsigned count;
  unsigned char r, g, b;
  unsigned char huge *p;

  p = (unsigned char*) data;		/* initialize pointer */

  for (count=firstReg; count<=lastReg; count++)	/* main loop */
  {
    r = *p >> 2;			/* translate 8-bit to 6-bit */
    if ((*p&3) >= 2)			/* round up if necessary */
      r++;
    if (r > 63)				/* ensure in range 0..63 */
      r = 63;
    p++;			     	/* update pointer */

    g = *p >> 2;
    if ((*p&3) >= 2)
      g++;
    if (g > 63)
      g = 63;
    p++;

    b = *p >> 2;
    if ((*p&3) >= 2)
      b++;
    if (b > 63)
      b = 63;
    p++;

    _AX = 0x1010;			/* set up registers */
    _BX = count;
    _DH = r;
    _CH = g;
    _CL = b;
    geninterrupt(0x10);			/* call the video BIOS */
  }
}


/***
**** getBlockPalette
***/
void TGE_getBlockPalette(unsigned firstReg, unsigned lastReg, void far *data)
{
  register unsigned count;
  unsigned char r, g, b;
  unsigned char huge *p;

  p = (unsigned char*) data;		/* initialize pointer */

  for (count=firstReg; count<=lastReg; count++)	/* main loop */
  {
    _AX = 0x1015;			/* set up registers */
    _BX = count;
    geninterrupt(0x10);			/* call the video BIOS */
    r = _DH;
    g = _CH;
    b = _CL;

    *p = r << 2;			/* translate 6-bit to 8-bit */
    p++;				/* and store values */
    *p = g << 2;
    p++;
    *p = b << 2;
    p++;
  }
}


/***
**** colourCloseTo
***/
static unsigned char palette[768];

unsigned TGE_colourCloseTo(unsigned char red, unsigned char green, unsigned
	char blue)
{
  register int count;
  int redDif, greenDif, blueDif, curDif, bestDif=1000, curNet, bestNet=1000;
  unsigned bestColour;

  TGEsys->_getBlockPalette(0, 255, palette);	/* get the palette */

  for (count=255; count>=0; count--)
  {
    redDif = red - palette[3*count];	   /* calculate greatest difference */
    greenDif = green - palette[3*count+1];
    blueDif = blue - palette[3*count+2];
    curNet = redDif + greenDif + blueDif;
    if (redDif < 0)
      redDif = -redDif;
    if (greenDif < 0)
      greenDif = -greenDif;
    if (blueDif < 0)
      blueDif = -blueDif;
    curDif = max(max(redDif,greenDif), blueDif);

    if (!curDif)			/* if same colour, return */
    {
      bestColour = count;
      break;
    }
    if (curDif < bestDif)		/* if better colour, set bestColour */
    {
      if (curNet <= bestNet)
      {
        bestDif = curDif;
        bestColour = count;
      }
    }
  }

  return (bestColour);
}


/***
**** colourCloseToX
***/
unsigned TGE_colourCloseToX(unsigned char red, unsigned char green, unsigned
	char blue, unsigned colourExclude)
{
  register int count;
  int redDif, greenDif, blueDif, curDif, bestDif=1000, curNet, bestNet=1000;
  unsigned bestColour;

  TGEsys->_getBlockPalette(0, 255, palette);	/* get the palette */

  for (count=255; count>=0; count--)
  {
    if (count != colourExclude)
    {
      redDif = red - palette[3*count];  /* calculate greatest difference */
      greenDif = green - palette[3*count+1];
      blueDif = blue - palette[3*count+2];
      curNet = redDif + greenDif + blueDif;
      if (redDif < 0)
	redDif = -redDif;
      if (greenDif < 0)
	greenDif = -greenDif;
      if (blueDif < 0)
	blueDif = -blueDif;
      curDif = max(max(redDif,greenDif), blueDif);

      if (!curDif)			/* if same colour, return */
      {
	bestColour = count;
	break;
      }
      if (curDif < bestDif)		/* if better colour, set bestColour */
      {
	if (curNet <= bestNet)
	{
	  bestDif = curDif;
	  bestColour = count;
	}
      }
    }
  }

  return (bestColour);
}


/***
**** imageSize
***/
unsigned long TGE_imageSize(int ulx, int uly, int lrx, int lry)
{
  unsigned long size;
  int temp;

  if (lrx < ulx)			/* swap coordinates if necessary */
  {
    temp = ulx;
    lrx = ulx;
    lrx = temp;
  }
  if (lry < uly)
  {
    temp = uly;
    lry = uly;
    lry = temp;
  }

  if (ulx < VIEWPORTULX)		/* ensure coords are in bounds */
    ulx = VIEWPORTULX;
  else if (ulx > VIEWPORTLRX)
    ulx = VIEWPORTLRX;

  if (uly < VIEWPORTULY)
    uly = VIEWPORTULY;
  else if (uly > VIEWPORTLRY)
    uly = VIEWPORTLRY;

  if (lrx < VIEWPORTULX)
    lrx = VIEWPORTULX;
  else if (lrx > VIEWPORTLRX)
    lrx = VIEWPORTLRX;

  if (lry < VIEWPORTULY)
    lry = VIEWPORTULY;
  else if (lry > VIEWPORTLRY)
    lry = VIEWPORTLRY;

  size = (unsigned long)(lrx-ulx+1);
  size *= (unsigned long)(lry-uly+1);

  return (size+4);			/* +4 for dimension information */
}


/***
**** imageSizeDim
***/
unsigned long TGE_imageSizeDim(unsigned wide, unsigned deep)
{
  unsigned long size;

  size = wide;
  size *= deep;

  return (size+4);			/* +4 for dimension information */
}


#ifdef __BORLANDC__
  #pragma option -h-
#endif

/***
**** putImage
***/
void huge TGE_putImage(int x, int y, void far *image)
{
  register unsigned ycount;
  int wide, deep;
  register unsigned srcAdd = 0;
  unsigned char huge *p = (unsigned char *)image;

  wide = ((unsigned*)image)[0];		/* get image dimensions */
  deep = ((unsigned*)image)[1];
  p += 4;				/* point to start of image data */

  if (y < VIEWPORTULY)			/* clip y coordinate */
  {
    if (y+deep >= VIEWPORTULY)
    {
      p += wide * abs(VIEWPORTULY-y);	/* skip any offscreen rows */
      deep -= abs(VIEWPORTULY-y);	/* update image depth */
      y = VIEWPORTULY;			/* new y coordinate */
    }
    else
      return;
  }
  else if (y > VIEWPORTLRY)
    return;
  if (y+deep > VIEWPORTLRY)
    deep = VIEWPORTLRY-y + 1;

  if (x < VIEWPORTULX)			/* clip x coordinate */
  {
    if (x+wide >= VIEWPORTULX)
    {
      srcAdd += abs(VIEWPORTULX-x);
      p += abs(VIEWPORTULX-x);
      wide -= abs(VIEWPORTULX-x);
      x = VIEWPORTULX;
    }
    else
      return;
  }
  else if (x > VIEWPORTLRX)
    return;
  if (x+wide > VIEWPORTLRX)
  {
    srcAdd += wide-1 - (VIEWPORTLRX-x);
    wide = VIEWPORTLRX-x + 1;
  }

  if (wide && deep)			/* quit if image is 0x0 pixels */
  {
    srcAdd += wide;
    for (ycount=0; ycount<deep; ycount++)
    {
      TGEsys->_putLine(y+ycount, x, wide, (void*)p);
      p += srcAdd;
    }
  }
}


/***
**** putImageInv
***/
void huge TGE_putImageInv(int x, int y, void far *image)
{
  register unsigned ycount;
  int wide, deep;
  register unsigned srcAdd = 0;
  unsigned char huge *p = (unsigned char *)image;

  wide = ((unsigned*)image)[0];		/* get image dimensions */
  deep = ((unsigned*)image)[1];
  p += 4;				/* point to start of image data */

  if (y < OUTVIEWPORTULY)		/* clip y coordinate */
  {
    if (y+deep > OUTVIEWPORTULY)
    {
      p += wide * abs(OUTVIEWPORTULY-y);/* skip any offscreen rows */
      deep -= abs(OUTVIEWPORTULY-y);	/* update image depth */
      y = OUTVIEWPORTULY;		/* new y coordinate */
    }
    else
      return;
  }
  else if (y > OUTVIEWPORTLRY)
    return;
  if (y+deep > OUTVIEWPORTLRY)
    deep = OUTVIEWPORTLRY-y + 1;

  if (x < OUTVIEWPORTULX)		/* clip x coordinate */
  {
    if (x+wide > OUTVIEWPORTULX)
    {
      srcAdd += abs(OUTVIEWPORTULX-x);
      p += abs(OUTVIEWPORTULX-x);
      wide -= abs(OUTVIEWPORTULX-x);
      x = OUTVIEWPORTULX;
    }
    else
      return;
  }
  else if (x > OUTVIEWPORTLRX)
    return;
  if (x+wide > OUTVIEWPORTLRX)
  {
    srcAdd += wide-1 - (OUTVIEWPORTLRX-x);
    wide = OUTVIEWPORTLRX-x + 1;
  }

  if (wide && deep)	   		/* quit if offscreen */
  {
    srcAdd += wide;
    for (ycount=0; ycount<deep; ycount++)
    {
      TGEsys->_putLineInv(y+ycount, x, wide, (void*)p);
      p += srcAdd;
    }
  }
}


/***
**** getImage
***/
void huge TGE_getImage(int ulx, int uly, int lrx, int lry, void far *image)
{
  register int ycount, wide, deep;
  int temp;
  unsigned char huge *p = (unsigned char*)image;

  if (lrx < ulx)			/* swap coordinates if necessary */
  {
    temp = ulx;
    lrx = ulx;
    lrx = temp;
  }
  if (lry < uly)
  {
    temp = uly;
    lry = uly;
    lry = temp;
  }

  if (ulx < INVIEWPORTULX)		/* ensure coords are in bounds */
    ulx = INVIEWPORTULX;
  else if (ulx > INVIEWPORTLRX)
    ulx = INVIEWPORTLRX;

  if (uly < INVIEWPORTULY)
    uly = INVIEWPORTULY;
  else if (uly > INVIEWPORTLRY)
    uly = INVIEWPORTLRY;

  if (lrx < INVIEWPORTULX)
    lrx = INVIEWPORTULX;
  else if (lrx > INVIEWPORTLRX)
    lrx = INVIEWPORTLRX;

  if (lry < INVIEWPORTULY)
    lry = INVIEWPORTULY;
  else if (lry > INVIEWPORTLRY)
    lry = INVIEWPORTLRY;

  ((unsigned *)image)[0] = wide = lrx-ulx+1;
  ((unsigned *)image)[1] = deep = lry-uly+1;
  p += 4;

  if (wide>0 && deep>0)
  {
    for (ycount=uly; ycount<=lry; ycount++)
    {
      TGEsys->_getLine(ycount, ulx, wide, (void*)p);
      p += wide;
    }
  }
}


/***
**** putLine
***/
void TGE_putLine(int lineNum, int xOff, int lineLen, void far *buf)
{
  register unsigned count, y;

  for (count=0,y=lineNum; count<lineLen; count++)	/* draw the line */
    TGEsys->_putPixel(xOff+count, y, *((unsigned char huge*)buf+count));
}


/***
**** putLineInv
***/
void TGE_putLineInv(int lineNum, int xOff, int lineLen, void far *buf)
{
  register unsigned count, y;
  unsigned char huge *p = (unsigned char huge*) buf;

  for (count=0,y=lineNum; count<lineLen; count++)	/* draw the line */
  {
    if (*p)
      TGEsys->_putPixel(xOff+count, y, *p);
    p++;
  }
}


/***
**** getLine
***/
void TGE_getLine(int lineNum, int xOff, int lineLen, void far *buf)
{
  register unsigned count, y;

  for (count=0,y=lineNum; count<lineLen; count++)      	/* draw the line */
    *((unsigned char huge*)buf+count) = TGEsys->_getPixel(xOff+count, y);
}


/***
**** putPixel
***/
void TGE_putPixel_scr_copy(int x, int y, unsigned colour)
{
  _AH = 0x0C;
  _AL = (unsigned char)colour;
  _BH = 0;
  _CX = x;
  _DX = y;
  geninterrupt(0x10);
}

void TGE_putPixel_scr_and(int x, int y, unsigned colour)
{
  _AH = 0x0D;				/* get the pixel */
  _BH = 0;
  _CX = x;
  _DX = y;
  geninterrupt(0x10);

  _AH = 0x0C;				/* set the ANDed pixel */
  _AL &= (unsigned char)colour;
  _BH = 0;
  _CX = x;
  _DX = y;
  geninterrupt(0x10);
}

void TGE_putPixel_scr_not(int x, int y, unsigned colour)
{
  _AH = 0x0C;				/* set the NOTed pixel */
  _AL = ~(unsigned char)colour;
  _BH = 0;
  _CX = x;
  _DX = y;
  geninterrupt(0x10);
}

void TGE_putPixel_scr_or(int x, int y, unsigned colour)
{
  _AH = 0x0D;				/* get the pixel */
  _BH = 0;
  _CX = x;
  _DX = y;
  geninterrupt(0x10);

  _AH = 0x0C;				/* set the ORed pixel */
  _AL |= (unsigned char)colour;
  _BH = 0;
  _CX = x;
  _DX = y;
  geninterrupt(0x10);
}

void TGE_putPixel_scr_xor(int x, int y, unsigned colour)
{
  _AH = 0x0D;				/* get the pixel */
  _BH = 0;
  _CX = x;
  _DX = y;
  geninterrupt(0x10);

  _AH = 0x0C;				/* set the XORed pixel */
  _AL ^= (unsigned char)colour;
  _BH = 0;
  _CX = x;
  _DX = y;
  geninterrupt(0x10);
}

void TGE_putPixel_mem_copy(int x, int y, unsigned colour)
{
  *(outPixelAddr(x,y)) = (unsigned char)colour;	 /* write the pixel */
}

void TGE_putPixel_mem_and(int x, int y, unsigned colour)
{
  *(outPixelAddr(x,y)) &= (unsigned char)colour; /* write the pixel */
}

void TGE_putPixel_mem_not(int x, int y, unsigned colour)
{
  *(outPixelAddr(x,y)) = ~(unsigned char)colour; /* write the pixel */
}

void TGE_putPixel_mem_or(int x, int y, unsigned colour)
{
  *(outPixelAddr(x,y)) |= (unsigned char)colour; /* write the pixel */
}

void TGE_putPixel_mem_xor(int x, int y, unsigned colour)
{
  *(outPixelAddr(x,y)) ^= (unsigned char)colour; /* write the pixel */
}


/***
**** getPixel
***/
unsigned TGE_getPixel_scr(int x, int y)
{
  _AH = 0x0D;
  _BH = 0;
  _CX = x;
  _DX = y;
  geninterrupt(0x10);

  _AH = 0;
  return (_AX);
}

unsigned TGE_getPixel_mem(int x, int y)
{
  return ((unsigned) *(inPixelAddr(x,y)));
}


/***
**** line
***/
void TGE_line(int x1, int y1, int x2, int y2, unsigned colour)
{
  register int t, distance;
  int xerr=0, yerr=0, deltax, deltay;
  int incx, incy;

  deltax = x2 - x1;			/* compute both distances */
  deltay = y2 - y1;

  if (deltax > 0)			/* compute increments */
    incx = 1;
  else if (deltax == 0)
    incx = 0;
  else
    incx = -1;

  if (deltay > 0)
    incy = 1;
  else if (deltay == 0)
    incy = 0;
  else
    incy = -1;

  deltax = abs(deltax);			/* determine greater distance */
  deltay = abs(deltay);
  if (deltax > deltay)
    distance = deltax;
  else
    distance = deltay;

  for (t=0; t<=distance+1; t++)		/* draw the line */
  {
    TGEsys->_putPixel(x1, y1, colour);
    xerr += deltax;
    yerr += deltay;
    if (xerr > distance)
    {
      xerr -= distance;
      x1 += incx;
    }
    if (yerr > distance)
    {
      yerr -= distance;
      y1 += incy;
    }
  }
}


/***
**** horizLine
***/
void TGE_horizLine(int y, int x1, int x2, unsigned colour)
{
  register unsigned count, max;

  for (count=x1,max=x2; count<=max; count++)	/* draw the line */
    TGEsys->_putPixel(count, y, colour);
}


/***
**** vertLine
***/
void TGE_vertLine(int x, int y1, int y2, unsigned colour)
{
  register unsigned count, max;

  for (count=y1,max=y2; count<=max; count++)	/* draw the line */
    TGEsys->_putPixel(x, count, colour);
}


/***
**** drawRect
***/
void TGE_drawRect(int ulx, int uly, int lrx, int lry, unsigned colour)
{
  register int ulxTemp, lrxTemp, temp;

  if (ulx > lrx)			/* swap coords if necessary */
  {
    temp = ulx;
    ulx = lrx;
    lrx = temp;
  }
  if (uly > lry)
  {
    temp = uly;
    uly = lry;
    lry = temp;
  }

  if (ulx<=VIEWPORTLRX && uly<=VIEWPORTLRY && lrx>=VIEWPORTULX &&
	lry>=VIEWPORTULY)
  {
    ulxTemp = ulx+1>=VIEWPORTULX ? ulx+1 : VIEWPORTULX;
    lrxTemp = lrx-1<=VIEWPORTLRX ? lrx-1 : VIEWPORTLRX;

    /* Draw horizontal lines if necessary */
    if (lrxTemp-ulxTemp > 1)
    {
      if (uly >= VIEWPORTULY)
	TGEsys->_horizLine(uly, ulxTemp, lrxTemp, colour);	/* top */
      if (lry <= VIEWPORTLRY)
	TGEsys->_horizLine(lry, ulxTemp, lrxTemp, colour);	/* bottom */
    }

    /* Clip y coordinates */
    if (uly < VIEWPORTULY)
      uly = VIEWPORTULY;
    if (lry > VIEWPORTLRY)
      lry = VIEWPORTLRY;

    /* Draw vertical lines if necessary */
    if (ulx >= VIEWPORTULX)
      TGEsys->_vertLine(ulx, uly, lry, colour);			/* left */
    if (lrx <= VIEWPORTLRX)
      TGEsys->_vertLine(lrx, uly, lry, colour);			/* right */
  }
}


/***
**** filledRect
***/
void TGE_filledRect(int ulx, int uly, int lrx, int lry, unsigned
	colour)
{
  register unsigned count, max;
  int temp;

  if (ulx > lrx)			/* swap coords if necessary */
  {
    temp = ulx;
    ulx = lrx;
    lrx = temp;
  }
  if (uly > lry)
  {
    temp = uly;
    uly = lry;
    lry = temp;
  }

  max = lry;
  for (count=uly; count<=max; count++)
    TGEsys->_fillLine(count, ulx, lrx, colour);
}


/***
**** clearGraphics
***/
void TGE_clearGraphics(unsigned colour)
{
  TGEsys->_filledRect(0, 0, MAXX, MAXY, colour);
}


/***
**** ellipse
***/
void TGE_ellipse(int xc, int yc, int wide, int deep, unsigned colour)
{
  int x, y;
  long a, b;
  long Asquared, TwoAsquared;
  long Bsquared, TwoBsquared;
  long d, dx, dy;

  wide /= 2;
  deep /= 2;

  x = 0;				/* initialize variables */
  y = deep;
  a = wide;
  b = deep;
  Asquared = a * a;
  TwoAsquared = 2 * Asquared;
  Bsquared = b * b;
  TwoBsquared = 2 * Bsquared;

  d = Bsquared - Asquared*b + Asquared/4L;
  dx = 0;
  dy = TwoAsquared * b;

  while (dx < dy)
  {
    set4Pixels(x, y, xc, yc, colour);

    if (d > 0L)
    {
      y--;
      dy -= TwoAsquared;
      d -= dy;
    }

    x++;
    dx += TwoBsquared;
    d += Bsquared + dx;
  }

  d += (3L*(Asquared-Bsquared)/2L - (dx+dy)) / 2L;

  while (y >= 0)
  {
    set4Pixels(x, y, xc, yc, colour);

    if (d < 0L)
    {
      x++;
      dx += TwoBsquared;
      d += dx;
    }

    y--;
    dy -= TwoAsquared;
    d += Asquared - dy;
  }
}

void near set4Pixels(int x, int y, int xc, int yc, unsigned colour)
{
  register int xCoord, yCoord;

  xCoord = xc + x;
  yCoord = yc + y;
  if (TGE_clipPoint(xCoord, yCoord))
    TGEsys->_putPixel(xCoord, yCoord, colour);

  xCoord = xc - x;
  if (TGE_clipPoint(xCoord, yCoord))
    TGEsys->_putPixel(xCoord, yCoord, colour);

  xCoord = xc + x;
  yCoord = yc - y;
  if (TGE_clipPoint(xCoord, yCoord))
    TGEsys->_putPixel(xCoord, yCoord, colour);

  xCoord = xc - x;
  if (TGE_clipPoint(xCoord, yCoord))
    TGEsys->_putPixel(xCoord, yCoord, colour);
}


/***
**** filledEllipse
***/
void TGE_filledEllipse(int xc, int yc, int wide, int deep, unsigned colour)
{
  int x, y;
  long a, b;
  long Asquared, TwoAsquared;
  long Bsquared, TwoBsquared;
  long d, dx, dy;

  wide /= 2;
  deep /= 2;

  x = 0;				/* initialize variables */
  y = deep;
  a = wide;
  b = deep;
  Asquared = a * a;
  TwoAsquared = 2 * Asquared;
  Bsquared = b * b;
  TwoBsquared = 2 * Bsquared;

  d = Bsquared - Asquared*b + Asquared/4L;
  dx = 0;
  dy = TwoAsquared * b;

  while (dx < dy)
  {
    fill4Pixels(x, y, xc, yc, colour);

    if (d > 0L)
    {
      y--;
      dy -= TwoAsquared;
      d -= dy;
    }

    x++;
    dx += TwoBsquared;
    d += Bsquared + dx;
  }

  d += (3L*(Asquared-Bsquared)/2L - (dx+dy)) / 2L;

  while (y >= 0)
  {
    fill4Pixels(x, y, xc, yc, colour);

    if (d < 0L)
    {
      x++;
      dx += TwoBsquared;
      d += dx;
    }

    y--;
    dy -= TwoAsquared;
    d += Asquared - dy;
  }
}

void near fill4Pixels(int x, int y, int xc, int yc, unsigned colour)
{
  register int xCoord1, xCoord2, yCoord;
  int temp;

  xCoord1 = xc - x;
  xCoord2 = xc + x;

  if (xCoord1>VIEWPORTLRX || xCoord2<VIEWPORTULX)
    return;

  if (xCoord1 > xCoord2)		/* ensure xCoord1 <= xCoord2 */
  {
    temp = xCoord1;
    xCoord1 = xCoord2;
    xCoord2 = temp;
  }

  yCoord = yc - y;
  if (yCoord>=VIEWPORTULY && yCoord<=VIEWPORTLRY)
  {
    if (xCoord1 < VIEWPORTULX)		/* clip xCoord1 horizontally */
      xCoord1 = VIEWPORTULX;
    else if (xCoord1 > VIEWPORTLRX)
      xCoord1 = VIEWPORTLRX;
    if (xCoord2 < VIEWPORTULX)		/* clip xCoord2 horizontally */
      xCoord2 = VIEWPORTULX;
    else if (xCoord2 > VIEWPORTLRX)
      xCoord2 = VIEWPORTLRX;
    TGEsys->_fillLine(yCoord, xCoord1, xCoord2, colour);
  }

  yCoord = yc + y;
  if (yCoord>=VIEWPORTULY && yCoord<=VIEWPORTLRY)
  {
    if (xCoord1 < VIEWPORTULX)		/* clip xCoord1 horizontally */
      xCoord1 = VIEWPORTULX;
    else if (xCoord1 > VIEWPORTLRX)
      xCoord1 = VIEWPORTLRX;
    if (xCoord2 < VIEWPORTULX)		/* clip xCoord2 horizontally */
      xCoord2 = VIEWPORTULX;
    else if (xCoord2 > VIEWPORTLRX)
      xCoord2 = VIEWPORTLRX;
    TGEsys->_fillLine(yCoord, xCoord1, xCoord2, colour);
  }
}


/***
**** circle
***/
void TGE_circle(int x, int y, int radius, unsigned colour)
{
  TGEsys->_ellipse(x, y, 2*radius, (2*radius*(4*YRATIO))/(3*XRATIO), colour);
}


/***
**** filledCircle
***/
void TGE_filledCircle(int x, int y, int radius, unsigned colour)
{
  TGEsys->_filledEllipse(x, y, 2*radius, (2*radius*(4*YRATIO))/(3*XRATIO), colour);
}


/***
**** fillRegion
***/
static struct params {
  int x, y;
};
static unsigned regionColour, fillColour;
static int okToPush;
static struct params huge *stack;
static unsigned long stackSize, stackPtr;

void TGE_fillRegion(int seedX, int seedY, unsigned colour)
{
  struct params holder;
  register int x, v;
  int xl, xr, y;

  if (!TGE_clipPoint(seedX, seedY))		/* abort if seed point */
    return;					/* is outside viewport */

  regionColour = TGEsys->_getPixel(seedX, seedY); /* initialize variables */
  if (regionColour == colour)
    return;
  fillColour = colour;

  stackSize = farcoreleft();			/* grab a very large */
  (void*)stack = farmalloc(stackSize);		/*  stack buffer     */
  stackSize /= sizeof(struct params);
  stackPtr = 0;

  okToPush = 1;
  holder.x = seedX;			/* push initial parameters */
  holder.y = seedY;
  stack[stackPtr] = holder;
  stackPtr++;

  while (stackPtr && stackPtr<=stackSize)	/* main loop */
  {
    stackPtr--;
    holder = stack[stackPtr];

    xl = xr = holder.x;
    y = holder.y;

    x = xl;					/* scan left */
    if (x > VIEWPORTULX)
    {
      do
      {
	x--;
	v = TGEsys->_getPixel(x, y);
      }
      while ((v==regionColour) && (v!=fillColour) && (x>=VIEWPORTULX));
      x++;
      xl = x;
    }

    x = xr;					/* scan right */
    if (x <= VIEWPORTLRX)
    {
      do
      {
	x++;
	v = TGEsys->_getPixel(x, y);
      }
      while ((v==regionColour) && (v!=fillColour) && (x<=VIEWPORTLRX));
      x--;
      xr = x;
    }

    TGEsys->_fillLine(y, xl, xr, fillColour);	/* fill the line */

    /* scan upwards */
    if (y > VIEWPORTULY)
    {
      for (x=xl; x<=xr; x++)
      {
	v = TGEsys->_getPixel(x, y-1);
	if (v==regionColour && okToPush)
	{
	  holder.x = x;
	  holder.y = y-1;
	  stack[stackPtr] = holder;
	  stackPtr++;
	  if (stackPtr != stackSize)
	  {
	    do
	    {
	      x++;
	      v = TGEsys->_getPixel(x, y-1);
	    }
	    while (v==regionColour && x<=xr);
	    x--;
	  }
	  else
	    okToPush = 0;
	}
      }
    }

    /* scan downwards */
    if (y < VIEWPORTLRY)
    {
      for (x=xl; x<=xr; x++)
      {
	v = TGEsys->_getPixel(x, y+1);
	if (v==regionColour && okToPush)
	{
	  holder.x = x;
	  holder.y = y+1;
	  stack[stackPtr] = holder;
	  stackPtr++;
	  if (stackPtr != stackSize)
	  {
	    do
	    {
	      x++;
	      v = TGEsys->_getPixel(x, y+1);
	    }
	    while (v==regionColour && x<=xr);
	    x--;
	  }
	  else
	    okToPush = 0;
	}
      }
    }
  };

  farfree((void far*)stack);			/* release stack memory */
}


/*****
****** Viewport routines
*****/

void TGE_setViewports(int ulx, int uly, int lrx, int lry)
{
  disable();
  OUTVIEWPORTULX = ulx;				/* set values */
  INVIEWPORTULX  = ulx;
  OUTVIEWPORTULY = uly;
  INVIEWPORTULY  = uly;
  OUTVIEWPORTLRX = lrx;
  INVIEWPORTLRX  = lrx;
  OUTVIEWPORTLRY = lry;
  INVIEWPORTLRY  = lry;
  enable();
}

void TGE_setOutputViewport(int ulx, int uly, int lrx, int lry)
{
  disable();
  OUTVIEWPORTULX = ulx;				/* set values */
  OUTVIEWPORTULY = uly;
  OUTVIEWPORTLRX = lrx;
  OUTVIEWPORTLRY = lry;
  enable();
}

void TGE_setInputViewport(int ulx, int uly, int lrx, int lry)
{
  disable();
  INVIEWPORTULX = ulx;				/* set values */
  INVIEWPORTULY = uly;
  INVIEWPORTLRX = lrx;
  INVIEWPORTLRY = lry;
  enable();
}

void TGE_getOutputViewport(int far *ulx, int far *uly, int far *lrx, int far *lry)
{
  disable();
  *ulx = OUTVIEWPORTULX;			/* get values */
  *uly = OUTVIEWPORTULY;
  *lrx = OUTVIEWPORTLRX;
  *lry = OUTVIEWPORTLRY;
  enable();
}

void TGE_getInputViewport(int far *ulx, int far *uly, int far *lrx, int far *lry)
{
  disable();
  *ulx = INVIEWPORTULX;				/* get values */
  *uly = INVIEWPORTULY;
  *lrx = INVIEWPORTLRX;
  *lry = INVIEWPORTLRY;
  enable();
}


/*****
****** Calculate the address of a pixel in a virtual screen.
*****/

unsigned char far* near outPixelAddr(int x, int y)
{
  unsigned char huge *p;
  unsigned long offset;

  (void far*) p = TGEsys->outAddr;	/* calculate pixel address */
  offset = y;
  offset *= TGEsys->outScreenWide;
  offset += x;

  while (offset > 0xFFFF)		/* add 'offset' to address */
  {
    p += 0xFFFF;
    offset -= 0xFFFF;
  }
  p += (unsigned)offset;

  return ((unsigned char far*) p);	/* return the address */
}

unsigned char far* near inPixelAddr(int x, int y)
{
  unsigned char huge *p;
  unsigned long offset;

  (void far*) p = TGEsys->inAddr;	/* calculate pixel address */
  offset = y;
  offset *= TGEsys->inScreenWide;
  offset += x;

  while (offset > 0xFFFF)		/* add 'offset' to address */
  {
    p += 0xFFFF;
    offset -= 0xFFFF;
  }
  p += (unsigned)offset;

  return ((unsigned char far*) p);	/* return the address */
}
