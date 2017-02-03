/*****************************************************************************
*       The Graphics Engine version 1.29·C                                   *
*                                                                            *
*       The Graphics Engine code and documentation are Copyright (c) 1993    *
*       by Matthew Hildebrand.                                               *
*                                                                            *
*       Unauthorised usage or modification of any or all of The Graphics     *
*       Engine is strictly prohibited.                                       *
*****************************************************************************/

#include <alloc.h>
#include <stdlib.h>
#include "tge.h"
#include "tgemouse.h"


/* More colours may, of course, be added. */

#define INVISIBLE	0
#define	BLACK		1
#define	WHITE		2


/****
***** Pointer definitions.  0=invisible, 1=black, 2=white
****/

unsigned char arrowPointer[] = {		/* ARROW_POINTER */
  6,0,8,0,			/* dimensions */
  1,1,0,0,0,0,			/* pointer data */
  1,2,1,0,0,0,
  1,2,2,1,0,0,
  1,2,2,2,1,0,
  1,2,2,2,2,1,
  1,2,1,2,1,1,
  1,1,1,1,2,1,
  0,0,0,1,1,1
};

unsigned char targetPointer[] = {		/* TARGET_POINTER */
  9,0,9,0,			/* dimensions */
  0,0,0,1,1,1,0,0,0,
  0,0,1,2,2,2,1,0,0,		/* pointer data */
  0,1,2,1,1,1,2,1,0,
  1,2,1,0,0,0,1,2,1,
  1,2,1,0,2,0,1,2,1,
  1,2,1,0,0,0,1,2,1,
  0,1,2,1,1,1,2,1,0,
  0,0,1,2,2,2,1,0,0,
  0,0,0,1,1,1,0,0,0
};

unsigned char bigArrowPointer[] = {		/* BIG_ARROW_POINTER */
  11,0,18,0,			/* dimensions */
  1,1,1,0,0,0,0,0,0,0,0,	/* pointer data */
  1,2,1,1,0,0,0,0,0,0,0,
  1,2,2,1,1,0,0,0,0,0,0,
  1,2,2,2,1,1,0,0,0,0,0,
  1,2,2,2,2,1,1,0,0,0,0,
  1,2,2,2,2,2,1,1,0,0,0,
  1,2,2,2,2,2,2,1,1,0,0,
  1,2,2,2,2,2,2,2,1,1,0,
  1,2,2,2,2,2,2,2,2,1,1,
  1,2,2,2,2,2,2,2,2,2,1,
  1,2,2,2,2,2,2,2,1,1,1,
  1,2,2,2,2,2,2,1,1,0,0,
  1,2,2,1,2,2,2,1,1,0,0,
  1,2,1,1,1,2,2,2,1,0,0,
  1,1,1,0,1,2,2,2,1,1,0,
  0,0,0,0,1,1,2,2,2,1,0,
  0,0,0,0,0,1,2,2,2,1,0,
  0,0,0,0,0,1,1,1,1,1,0
};

unsigned char bigTargetPointer[] = {		/* BIG_TARGET_POINTER */
  15,0,15,0,			/* dimensions */
  0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
  0,0,0,1,1,1,2,2,2,1,1,1,0,0,0,
  0,0,1,1,2,2,2,2,2,2,2,1,1,0,0,
  0,1,1,2,2,1,1,1,1,1,2,2,1,1,0,
  0,1,2,2,1,1,0,0,0,1,1,2,2,1,0,
  1,1,2,1,1,0,0,1,0,0,1,1,2,1,1,
  1,2,2,1,0,0,1,2,1,0,0,1,2,2,1,
  1,2,2,1,0,1,2,2,2,1,0,1,2,2,1,
  1,2,2,1,0,0,1,2,1,0,0,1,2,2,1,
  1,1,2,1,1,0,0,1,0,0,1,1,2,1,1,
  0,1,2,2,1,1,0,0,0,1,1,2,2,1,0,
  0,1,1,2,2,1,1,1,1,1,2,2,1,1,0,
  0,0,1,1,2,2,2,2,2,2,2,1,1,0,0,
  0,0,0,1,1,1,2,2,2,1,1,1,0,0,0,
  0,0,0,0,0,1,1,1,1,1,0,0,0,0,0
};

static unsigned char mousePointer[512];  /* default buffer size: 512b */
static int curPointerNum = -1;		  /* the current pointer */


/****
***** Code
****/

void TGE_setupMousePointer(int pointerNum)
{
  unsigned char *src, *dest=mousePointer;
  register unsigned count, max;
  unsigned black, white;
  unsigned wide, deep;
  int xHotSpot, yHotSpot;

  curPointerNum = pointerNum;	/* update current pointer number */
  switch (pointerNum)		/* set up proper source for pointer */
  {
    case ARROW_POINTER:		/* is it the arrow pointer? */
      src = arrowPointer;
      xHotSpot = 0;
      yHotSpot = 0;
      break;
    case TARGET_POINTER:	/* is it the target pointer? */
      src = targetPointer;
      xHotSpot = 4;
      yHotSpot = 4;
      break;
    case BIG_ARROW_POINTER:	/* is it the big arrow pointer? */
      src = bigArrowPointer;
      xHotSpot = 0;
      yHotSpot = 0;
      break;
    case BIG_TARGET_POINTER:	/* is it the big target pointer? */
      src = bigTargetPointer;
      xHotSpot = 7;
      yHotSpot = 7;
      break;
    default:			/* undefined */
      return;
  }


  /* Find the colours closest to the ones in the pointer */

  black = colourCloseToX(0, 0, 0, 0);
  white = colourCloseToX(255, 255, 255, 0);


  /* Setup pointer image dimensions */

  wide = ((unsigned*)src)[0];
  deep = ((unsigned*)src)[1];
  src += 4;
  ((unsigned*)dest)[0] = wide;
  ((unsigned*)dest)[1] = deep;
  dest += 4;


  /* Create the pointer bitmap using newly-found colour numbers */

  max = wide * deep;
  for (count=0; count<max; count++)
  {
    switch (*src)			/* decide what to put in dest */
    {
      case BLACK:
        *dest = black;
        break;
      case WHITE:
        *dest = white;
        break;
      default:
        *dest = *src;
    }
    src++;
    dest++;
  }


  /* Pass the bitmap to the driver */

  setPointerMouse(xHotSpot, yHotSpot, mousePointer);
}


/* Set up the same pointer again, thus possibly rechoosing the colours
   composing the pointer.  This function is useful after the palette has
   been changed, but you want the pointer to remain the same. */

void TGE_setPointerColours(void)
{
  setupMousePointer(curPointerNum);
}