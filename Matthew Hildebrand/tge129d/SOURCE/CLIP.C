/*****************************************************************************
*       The Graphics Engine version 1.29·D                                   *
*                                                                            *
*       The Graphics Engine code and documentation are Copyright (c) 1993    *
*       by Matthew Hildebrand.                                               *
*                                                                            *
*       Unauthorised usage or modification of any or all of The Graphics     *
*       Engine is strictly prohibited.                                       *
*****************************************************************************/

#include "tge.h"


/****
***** Initial data declarations
****/

#define X1	(*x1)
#define Y1	(*y1)
#define X2	(*x2)
#define Y2	(*y2)

struct endpoints
{
  int x1, y1, x2, y2;
};

union outCodes
{
  struct
  {
    unsigned code0 : 1;         /* x<OUTVIEWPORTULX */
    unsigned code1 : 1;         /* y<OUTVIEWPORTULY */
    unsigned code2 : 1;         /* x>OUTVIEWPORTLRX */
    unsigned code3 : 1;         /* y>OUTVIEWPORTLRY */
  } ocs;
  int outcodes;
};

static void setOutCodes(union outCodes *ocu, int x, int y);


/****
***** Clip a line
****/

int TGE_clipLine(int *x1, int *y1, int *x2, int *y2)
{
  union outCodes ocu1, ocu2;
  int inside, outside;
  int temp;

  /* initialize 4-bit codes */
  setOutCodes(&ocu1, X1, Y1);
  setOutCodes(&ocu2, X2, Y2);

  inside = ((ocu1.outcodes | ocu2.outcodes) == 0);
  outside = ((ocu1.outcodes & ocu2.outcodes) != 0);

  while (!outside && !inside)
  {
    if (ocu1.outcodes == 0)		// swap endpoints if necessary so
    {					// that (x1,y1) needs to be clipped
      temp = X1;		/* swap X1 and X2 */
      X1 = X2;
      X2 = temp;
      temp = Y1;		/* swap Y1 and Y2 */
      Y1 = Y2;
      Y2 = temp;
      temp = ocu1.outcodes;	/* swap ocu1 and ocu2 */
      ocu1 = ocu2;
      ocu2.outcodes = temp;
    }

    if (ocu1.ocs.code0)			/* clip left */
    {
      Y1 += (int)((long)(Y2-Y1)*(OUTVIEWPORTULX-X1)/(X2-X1));
      X1 = OUTVIEWPORTULX;
    }
    else if (ocu1.ocs.code1)		/* clip above */
    {
      X1 += (int)((long)(X2-X1)*(OUTVIEWPORTULY-Y1)/(Y2-Y1));
      Y1 = OUTVIEWPORTULY;
    }
    else if (ocu1.ocs.code2)		/* clip right */
    {
      Y1 += (int)((long)(Y2-Y1)*(OUTVIEWPORTLRX-X1)/(X2-X1));
      X1 = OUTVIEWPORTLRX;
    }
    else if (ocu1.ocs.code3)		/* clip below */
    {
      X1 += (int)((long)(X2-X1)*(OUTVIEWPORTLRY-Y1)/(Y2-Y1));
      Y1 = OUTVIEWPORTLRY;
    }

    setOutCodes(&ocu1, X1, Y1);

    inside = ((ocu1.outcodes | ocu2.outcodes) == 0);	/* update codes */
    outside = ((ocu1.outcodes & ocu2.outcodes) != 0);
  }

  return (inside);
}

void setOutCodes(union outCodes *u, int x, int y)
{
  u->outcodes = 0;
  u->ocs.code0 = (x < OUTVIEWPORTULX);
  u->ocs.code1 = (y < OUTVIEWPORTULY);
  u->ocs.code2 = (x > OUTVIEWPORTLRX);
  u->ocs.code3 = (y > OUTVIEWPORTLRY);
}


/****
***** Clip a filled rectangle
****/

int TGE_clipFilledRect(int *ulx, int *uly, int *lrx, int *lry)
{
  int temp;

  if (*ulx > *lrx)			/* swap coords if necessary */
  {
    temp = *ulx;
    *ulx = *lrx;
    *lrx = temp;
  }
  if (*uly > *lry)
  {
    temp = *uly;
    *uly = *lry;
    *lry = temp;
  }

  if (*ulx<=OUTVIEWPORTLRX && *uly<=OUTVIEWPORTLRY && *lrx>=OUTVIEWPORTULX &&
        *lry>=OUTVIEWPORTULY)
  {
    if (*ulx < OUTVIEWPORTULX)     /* clip to viewport boundaries */
      *ulx = OUTVIEWPORTULX;
    if (*uly < OUTVIEWPORTULY)
      *uly = OUTVIEWPORTULY;
    if (*lrx > OUTVIEWPORTLRX)
      *lrx = OUTVIEWPORTLRX;
    if (*lry > OUTVIEWPORTLRY)
      *lry = OUTVIEWPORTLRY;
    return (1);			/* at least partially within viewport */
  }
  else
    return (0);			/* entirely outside viewport */
}
