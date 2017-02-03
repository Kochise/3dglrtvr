/*****************************************************************************
*       The Graphics Engine version 1.29·D                                   *
*                                                                            *
*       The Graphics Engine code and documentation are Copyright (c) 1993    *
*       by Matthew Hildebrand.                                               *
*                                                                            *
*       Unauthorised usage or modification of any or all of The Graphics     *
*       Engine is strictly prohibited.                                       *
*****************************************************************************/

/* Thanks are due to Thad Smith for helping to optimize this routine. */

#include <string.h>


#ifdef __BORLANDC__
  #pragma option -h-
#endif


void scaleBitmap(void *src, unsigned newWide, unsigned newDeep, void *dest)
{
  int countX, countY, oldWide, oldDeep;
  int ry;       /* residual for y aspect ratio */
  /* Source and destination pointers:
  ** far pointers are used for each line, huge pointers traverse the
  ** entire area, since it can be larger than 64k.
  */
  unsigned char huge *ph = (unsigned char huge*)dest + 4;
  unsigned char huge *qh = (unsigned char huge*)src  + 4;
  unsigned char far  *pf = (unsigned char far*) ph;
  unsigned char far  *qf = (unsigned char far*) qh;
  unsigned char far  *prev_pf = 0;      /* ptr to previous row */

  /* Get dimension information from source image */
  oldWide = ((unsigned*)src)[0];
  oldDeep = ((unsigned*)src)[1];

  /* Store dimension information in destination image */
  ((unsigned*)dest)[0] = newWide;
  ((unsigned*)dest)[1] = newDeep;

  /* The initial value of the y residual depends on whether the image
  ** is expanded or contracted.  This feels awkward to me, but it works.
  */
  if (oldDeep < newDeep)
    ry = oldDeep;
  else
    ry = newDeep - 1;

  /* Main scaling loops */
  for (countY=newDeep; --countY>=0;)
  {
    if ((ry -= oldDeep) > 0)
    {
      /* This new row is the same as the previous one.  Copy it. */
      memcpy(pf, prev_pf, newWide);
    }
    else
    {
      /* Get pixel at a time from old row with scaling algorithm */
      register int rx = newWide + oldWide - 1;

      prev_pf = pf;
      for (countX=newWide; --countX>=0;)
      {
	if ((rx-=oldWide) < 0)
	{
	  do
	    qf++;
	  while ((rx+=newWide) < 0);
	}
	*pf++ = *qf;
      }
      do
	qf = (unsigned char far*) (qh += oldWide);
      while ((ry+=newDeep) < 0);
    }
    pf = (unsigned char far*) (ph += newWide);
  }
}