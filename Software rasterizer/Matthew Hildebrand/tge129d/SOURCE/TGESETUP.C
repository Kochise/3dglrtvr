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
#include <mem.h>
#include <stdio.h>
#include "tge.h"


/* Guess what... this file must be compiled with the large or huge model. */
#if !defined(__LARGE__) && !defined(__HUGE__) && !defined(M_I86LM)
  #error This module must be compiled with the large or huge model.
#endif

/* This copyright notice may not be removed and must remain intact */
static char copyright[] = "The Graphics Engine 1.29·D -- Copyright (c) 1993 by Matthew Hildebrand";

/* #define macros needed only for TGE.C */
#define INADDR		(TGEsys->inAddr)
#define OUTADDR		(TGEsys->outAddr)
#define SCRADDR		(TGEsys->scrAddr)
#define INSCREENWIDE	(TGEsys->inScreenWide)
#define OUTSCREENWIDE	(TGEsys->outScreenWide)
#define SCREEN		1
#define MEMORY		2



/******
******* Function prototypes and global data
******/

extern void far TGE_deInitGraphics(void);
extern void far TGE_setPaletteReg(unsigned palReg, unsigned char red, unsigned char blue, unsigned char green);
extern void far TGE_getPaletteReg(unsigned palReg, unsigned char *red, unsigned char *blue, unsigned char *green);
extern void far TGE_setBlockPalette(unsigned firstReg, unsigned numRegs, void far *data);
extern void far TGE_getBlockPalette(unsigned firstReg, unsigned numRegs, void far *data);
extern unsigned far TGE_colourCloseTo(unsigned char red, unsigned char green, unsigned	char blue);
extern unsigned far TGE_colourCloseToX(unsigned char red, unsigned char green, unsigned	char blue, unsigned colourExclude);
extern unsigned long far TGE_imageSize(int ulx, int uly, int lrx, int lry);
extern unsigned long far TGE_imageSizeDim(unsigned wide, unsigned deep);
extern void huge TGE_getImage(int ulx, int uly, int lrx, int lry, void far *image);
extern void far TGE_getLine(int lineNum, int xOff, int lineLen, void far *buf);
extern unsigned far TGE_getPixel_scr(int x, int y);
extern unsigned far TGE_getPixel_mem(int x, int y);
extern void huge TGE_putImage(int x, int y, void far *image);
extern void huge TGE_putImageInv(int x, int y, void far *image);
extern void far TGE_putLine(int lineNum, int xOff, int lineLen, void far *buf);
extern void far TGE_putLineInv(int lineNum, int xOff, int lineLen, void far *buf);
extern void far TGE_putPixel_scr_copy(int x, int y, unsigned colour);
extern void far TGE_putPixel_scr_and (int x, int y, unsigned colour);
extern void far TGE_putPixel_scr_not (int x, int y, unsigned colour);
extern void far TGE_putPixel_scr_or  (int x, int y, unsigned colour);
extern void far TGE_putPixel_scr_xor (int x, int y, unsigned colour);
extern void far TGE_putPixel_mem_copy(int x, int y, unsigned colour);
extern void far TGE_putPixel_mem_and (int x, int y, unsigned colour);
extern void far TGE_putPixel_mem_not (int x, int y, unsigned colour);
extern void far TGE_putPixel_mem_or  (int x, int y, unsigned colour);
extern void far TGE_putPixel_mem_xor (int x, int y, unsigned colour);
extern void far TGE_line(int x1, int y1, int x2, int y2, unsigned colour);
extern void far TGE_horizLine(int y, int x1, int x2, unsigned colour);
extern void far TGE_vertLine(int x, int y1, int y2, unsigned colour);
extern void far TGE_drawRect(int ulx, int uly, int lrx, int lry, unsigned colour);
extern void far TGE_filledRect(int ulx, int uly, int lrx, int lry, unsigned colour);
extern void far TGE_clearGraphics(unsigned colour);
extern void far TGE_ellipse(int xc, int yc, int wide, int deep, unsigned colour);
extern void far TGE_filledEllipse(int xc, int yc, int wide, int deep, unsigned colour);
extern void far TGE_circle(int x, int y, int radius, unsigned colour);
extern void far TGE_filledCircle(int x, int y, int radius, unsigned colour);
extern void far TGE_fillRegion(int x, int y, unsigned colour);
static void* near forceOffset(void far *p);
static void near setupInfo(struct GraphDrv far *grDrv);
extern void near setInputFunctions(int memOrScreen);
extern void near setOutputFunctions(int memOrScreen, int writeMode);

static int curOutputMode = TGE_COPY_PUT;
static int curOutputDest = SCREEN;
static void far *drvAddr = NULL;
struct GraphDrv far *TGEsys = NULL;


/*****
****** Routines to load a graphics driver from disk.
*****/

/***
**** Main loader function.
***/
int TGE_loadGraphDriver(char *filename)
{
  struct GraphDrv far *grDrv = NULL;
  FILE *inFile;
  long fileLen;
  char signature[4];


  /* Outsmart smart linkers; force copyright string to be in EXE file */
  copyright[0] = 'T';				/* dummy write */


  /* Open the input file */
  if ((inFile=fopen(filename,"rb")) == NULL)	/* open the file */
    return (TGE_OPEN_ERR);


  /* Get file size */
  fseek(inFile, 0, SEEK_END);			/* seek to end of file */
  fileLen = ftell(inFile);			/* get current position */
  rewind(inFile);				/* seek to start of file */


  /* Parse the file header */
  if (!fread(signature, 1, 4, inFile))		/* read signature string */
  {
    fclose(inFile);
    return (TGE_FORMAT_ERR);
  }

  if (memcmp(signature, "TGE3", 4))       	/* check signature string */
  {
    fclose(inFile);
    return (TGE_FORMAT_ERR);
  }


  /* Allocate memory for the driver */
  fileLen -= 4;					/* remove signature size */
  if (((void*)drvAddr=farmalloc(fileLen+15L)) == NULL)	/* grab some RAM */
  {
    fclose(inFile);
    return (TGE_ALLOC_ERR);
  }

  (void*)grDrv = forceOffset(drvAddr);		/* force to XXXX:0004 */


  /* Read in the body of the driver */
  if (fread(grDrv, 1, (unsigned)fileLen, inFile) != (unsigned)fileLen)
  {
    fclose(inFile);
    farfree(drvAddr);
    return (TGE_FILE_ERR);
  }


  /* Everything is loaded properly; now set up for program use */
  TGEsys = grDrv;				/* set global variable */
  setupInfo(grDrv);				/* setup function pointers */
  fclose(inFile);				/* close input file */

  return (TGE_SUCCESS);				/* return success code */
}


/***
**** Force a pointer to the form XXXX:0004.
***/
static void* near forceOffset(void far *p)
{
  void huge *temp = (void huge*)p;

  if (FP_OFF(temp) == 4)
    return ((void far*)temp);
  else if (FP_OFF(temp) > 4)
    return (MK_FP(FP_SEG(temp)+1, 4));
  else
    return (MK_FP(FP_SEG(temp), 4));
}


/***
**** Set up function pointers once a driver has been loaded.
***/
static void near setupInfo(struct GraphDrv far *grDrv)
{
  register unsigned seg = FP_SEG(grDrv);

  /* initGraphics must always be present */
  (void*)grDrv->_initGraphics = MK_FP(seg, FP_OFF(grDrv->_initGraphics));


  /* Set up non-I/O functions */
  if (FP_OFF(grDrv->_deInitGraphics))		/* deInitGraphics */
    (void*)grDrv->_deInitGraphics = MK_FP(seg, FP_OFF(grDrv->_deInitGraphics));
  else
    grDrv->_deInitGraphics = TGE_deInitGraphics;

  if (FP_OFF(grDrv->_setPaletteReg))		/* setPaletteReg */
    (void*)grDrv->_setPaletteReg = MK_FP(seg, FP_OFF(grDrv->_setPaletteReg));
  else
    grDrv->_setPaletteReg = TGE_setPaletteReg;

  if (FP_OFF(grDrv->_getPaletteReg))		/* getPaletteReg */
    (void*)grDrv->_getPaletteReg = MK_FP(seg, FP_OFF(grDrv->_getPaletteReg));
  else
    grDrv->_getPaletteReg = TGE_getPaletteReg;

  if (FP_OFF(grDrv->_setBlockPalette))		/* setBlockPalette */
    (void*)grDrv->_setBlockPalette = MK_FP(seg, FP_OFF(grDrv->_setBlockPalette));
  else
    grDrv->_setBlockPalette = TGE_setBlockPalette;

  if (FP_OFF(grDrv->_getBlockPalette))		/* getBlockPalette */
    (void*)grDrv->_getBlockPalette = MK_FP(seg, FP_OFF(grDrv->_getBlockPalette));
  else
    grDrv->_getBlockPalette = TGE_getBlockPalette;

  if (FP_OFF(grDrv->_colourCloseTo))		/* colourCloseTo */
    (void*)grDrv->_colourCloseTo = MK_FP(seg, FP_OFF(grDrv->_colourCloseTo));
  else
    grDrv->_colourCloseTo = TGE_colourCloseTo;

  if (FP_OFF(grDrv->_colourCloseToX))		/* colourCloseToX */
    (void*)grDrv->_colourCloseToX = MK_FP(seg, FP_OFF(grDrv->_colourCloseToX));
  else
    grDrv->_colourCloseToX = TGE_colourCloseToX;

  if (FP_OFF(grDrv->_imageSize))		/* imageSize */
    (void*)grDrv->_imageSize = MK_FP(seg, FP_OFF(grDrv->_imageSize));
  else
    grDrv->_imageSize = TGE_imageSize;

  if (FP_OFF(grDrv->_imageSizeDim))		/* imageSizeDim */
    (void*)grDrv->_imageSizeDim = MK_FP(seg, FP_OFF(grDrv->_imageSizeDim));
  else
    grDrv->_imageSizeDim = TGE_imageSizeDim;


  /* Set up input functions */
  if (FP_OFF(grDrv->_getImage_scr))		/* getImage_scr */
    (void*)grDrv->_getImage_scr = MK_FP(seg, FP_OFF(grDrv->_getImage_scr));
  else
    grDrv->_getImage_scr = TGE_getImage;

  if (FP_OFF(grDrv->_getImage_mem))		/* getImage_mem */
    (void*)grDrv->_getImage_mem = MK_FP(seg, FP_OFF(grDrv->_getImage_mem));
  else
    grDrv->_getImage_mem = TGE_getImage;

  if (FP_OFF(grDrv->_getLine_scr))		/* getLine_scr */
    (void*)grDrv->_getLine_scr = MK_FP(seg, FP_OFF(grDrv->_getLine_scr));
  else
    grDrv->_getLine_scr = TGE_getLine;

  if (FP_OFF(grDrv->_getLine_mem))		/* getLine_mem */
    (void*)grDrv->_getLine_mem = MK_FP(seg, FP_OFF(grDrv->_getLine_mem));
  else
    grDrv->_getLine_mem = TGE_getLine;

  if (FP_OFF(grDrv->_getPixel_scr))		/* getPixel_scr */
    (void*)grDrv->_getPixel_scr = MK_FP(seg, FP_OFF(grDrv->_getPixel_scr));
  else
    grDrv->_getPixel_scr = TGE_getPixel_scr;

  if (FP_OFF(grDrv->_getPixel_mem))		/* getPixel_mem */
    (void*)grDrv->_getPixel_mem = MK_FP(seg, FP_OFF(grDrv->_getPixel_mem));
  else
    grDrv->_getPixel_mem = TGE_getPixel_mem;


  /* Set up output functions */
  if (FP_OFF(grDrv->_putImage_scr_copy))	/* putImage_scr_copy */
    (void*)grDrv->_putImage_scr_copy = MK_FP(seg, FP_OFF(grDrv->_putImage_scr_copy));
  else
    grDrv->_putImage_scr_copy = TGE_putImage;
  if (FP_OFF(grDrv->_putImage_scr_and))		/* putImage_scr_and */
    (void*)grDrv->_putImage_scr_and = MK_FP(seg, FP_OFF(grDrv->_putImage_scr_and));
  else
    grDrv->_putImage_scr_and = TGE_putImage;
  if (FP_OFF(grDrv->_putImage_scr_not))		/* putImage_scr_not */
    (void*)grDrv->_putImage_scr_not = MK_FP(seg, FP_OFF(grDrv->_putImage_scr_not));
  else
    grDrv->_putImage_scr_not = TGE_putImage;
  if (FP_OFF(grDrv->_putImage_scr_or))		/* putImage_scr_or */
    (void*)grDrv->_putImage_scr_or = MK_FP(seg, FP_OFF(grDrv->_putImage_scr_or));
  else
    grDrv->_putImage_scr_or = TGE_putImage;
  if (FP_OFF(grDrv->_putImage_scr_xor))		/* putImage_scr_xor */
    (void*)grDrv->_putImage_scr_xor = MK_FP(seg, FP_OFF(grDrv->_putImage_scr_xor));
  else
    grDrv->_putImage_scr_xor = TGE_putImage;
  if (FP_OFF(grDrv->_putImage_mem_copy))	/* putImage_mem_copy */
    (void*)grDrv->_putImage_mem_copy = MK_FP(seg, FP_OFF(grDrv->_putImage_mem_copy));
  else
    grDrv->_putImage_mem_copy = TGE_putImage;
  if (FP_OFF(grDrv->_putImage_mem_and))		/* putImage_mem_and */
    (void*)grDrv->_putImage_mem_and = MK_FP(seg, FP_OFF(grDrv->_putImage_mem_and));
  else
    grDrv->_putImage_mem_and = TGE_putImage;
  if (FP_OFF(grDrv->_putImage_mem_not))		/* putImage_mem_not */
    (void*)grDrv->_putImage_mem_not = MK_FP(seg, FP_OFF(grDrv->_putImage_mem_not));
  else
    grDrv->_putImage_mem_not = TGE_putImage;
  if (FP_OFF(grDrv->_putImage_mem_or))		/* putImage_mem_or */
    (void*)grDrv->_putImage_mem_or = MK_FP(seg, FP_OFF(grDrv->_putImage_mem_or));
  else
    grDrv->_putImage_mem_or = TGE_putImage;
  if (FP_OFF(grDrv->_putImage_mem_xor))		/* putImage_mem_xor */
    (void*)grDrv->_putImage_mem_xor = MK_FP(seg, FP_OFF(grDrv->_putImage_mem_xor));
  else
    grDrv->_putImage_mem_xor = TGE_putImage;

  if (FP_OFF(grDrv->_putImageInv_scr_copy))	/* putImageInv_scr_copy */
    (void*)grDrv->_putImageInv_scr_copy = MK_FP(seg, FP_OFF(grDrv->_putImageInv_scr_copy));
  else
    grDrv->_putImageInv_scr_copy = TGE_putImageInv;
  if (FP_OFF(grDrv->_putImageInv_scr_and))	/* putImageInv_scr_and */
    (void*)grDrv->_putImageInv_scr_and = MK_FP(seg, FP_OFF(grDrv->_putImageInv_scr_and));
  else
    grDrv->_putImageInv_scr_and = TGE_putImageInv;
  if (FP_OFF(grDrv->_putImageInv_scr_not))	/* putImageInv_scr_not */
    (void*)grDrv->_putImageInv_scr_not = MK_FP(seg, FP_OFF(grDrv->_putImageInv_scr_not));
  else
    grDrv->_putImageInv_scr_not = TGE_putImageInv;
  if (FP_OFF(grDrv->_putImageInv_scr_or))	/* putImageInv_scr_or */
    (void*)grDrv->_putImageInv_scr_or = MK_FP(seg, FP_OFF(grDrv->_putImageInv_scr_or));
  else
    grDrv->_putImageInv_scr_or = TGE_putImageInv;
  if (FP_OFF(grDrv->_putImageInv_scr_xor))	/* putImageInv_scr_xor */
    (void*)grDrv->_putImageInv_scr_xor = MK_FP(seg, FP_OFF(grDrv->_putImageInv_scr_xor));
  else
    grDrv->_putImageInv_scr_xor = TGE_putImageInv;
  if (FP_OFF(grDrv->_putImageInv_mem_copy))	/* putImageInv_mem_copy */
    (void*)grDrv->_putImageInv_mem_copy = MK_FP(seg, FP_OFF(grDrv->_putImageInv_mem_copy));
  else
    grDrv->_putImageInv_mem_copy = TGE_putImageInv;
  if (FP_OFF(grDrv->_putImageInv_mem_and))	/* putImageInv_mem_and */
    (void*)grDrv->_putImageInv_mem_and = MK_FP(seg, FP_OFF(grDrv->_putImageInv_mem_and));
  else
    grDrv->_putImageInv_mem_and = TGE_putImageInv;
  if (FP_OFF(grDrv->_putImageInv_mem_not))	/* putImageInv_mem_not */
    (void*)grDrv->_putImageInv_mem_not = MK_FP(seg, FP_OFF(grDrv->_putImageInv_mem_not));
  else
    grDrv->_putImageInv_mem_not = TGE_putImageInv;
  if (FP_OFF(grDrv->_putImageInv_mem_or))	/* putImageInv_mem_or */
    (void*)grDrv->_putImageInv_mem_or = MK_FP(seg, FP_OFF(grDrv->_putImageInv_mem_or));
  else
    grDrv->_putImageInv_mem_or = TGE_putImageInv;
  if (FP_OFF(grDrv->_putImageInv_mem_xor))	/* putImageInv_mem_xor */
    (void*)grDrv->_putImageInv_mem_xor = MK_FP(seg, FP_OFF(grDrv->_putImageInv_mem_xor));
  else
    grDrv->_putImageInv_mem_xor = TGE_putImageInv;

  if (FP_OFF(grDrv->_putLine_scr_copy))		/* putLine_scr_copy */
    (void*)grDrv->_putLine_scr_copy = MK_FP(seg, FP_OFF(grDrv->_putLine_scr_copy));
  else
    grDrv->_putLine_scr_copy = TGE_putLine;
  if (FP_OFF(grDrv->_putLine_scr_and))		/* putLine_scr_and */
    (void*)grDrv->_putLine_scr_and = MK_FP(seg, FP_OFF(grDrv->_putLine_scr_and));
  else
    grDrv->_putLine_scr_and = TGE_putLine;
  if (FP_OFF(grDrv->_putLine_scr_not))		/* putLine_scr_not */
    (void*)grDrv->_putLine_scr_not = MK_FP(seg, FP_OFF(grDrv->_putLine_scr_not));
  else
    grDrv->_putLine_scr_not = TGE_putLine;
  if (FP_OFF(grDrv->_putLine_scr_or))		/* putLine_scr_or */
    (void*)grDrv->_putLine_scr_or = MK_FP(seg, FP_OFF(grDrv->_putLine_scr_or));
  else
    grDrv->_putLine_scr_or = TGE_putLine;
  if (FP_OFF(grDrv->_putLine_scr_xor))		/* putLine_scr_xor */
    (void*)grDrv->_putLine_scr_xor = MK_FP(seg, FP_OFF(grDrv->_putLine_scr_xor));
  else
    grDrv->_putLine_scr_xor = TGE_putLine;
  if (FP_OFF(grDrv->_putLine_mem_copy))		/* putLine_mem_copy */
    (void*)grDrv->_putLine_mem_copy = MK_FP(seg, FP_OFF(grDrv->_putLine_mem_copy));
  else
    grDrv->_putLine_mem_copy = TGE_putLine;
  if (FP_OFF(grDrv->_putLine_mem_and))		/* putLine_mem_and */
    (void*)grDrv->_putLine_mem_and = MK_FP(seg, FP_OFF(grDrv->_putLine_mem_and));
  else
    grDrv->_putLine_mem_and = TGE_putLine;
  if (FP_OFF(grDrv->_putLine_mem_not))		/* putLine_mem_not */
    (void*)grDrv->_putLine_mem_not = MK_FP(seg, FP_OFF(grDrv->_putLine_mem_not));
  else
    grDrv->_putLine_mem_not = TGE_putLine;
  if (FP_OFF(grDrv->_putLine_mem_or))		/* putLine_mem_or */
    (void*)grDrv->_putLine_mem_or = MK_FP(seg, FP_OFF(grDrv->_putLine_mem_or));
  else
    grDrv->_putLine_mem_or = TGE_putLine;
  if (FP_OFF(grDrv->_putLine_mem_xor))		/* putLine_mem_xor */
    (void*)grDrv->_putLine_mem_xor = MK_FP(seg, FP_OFF(grDrv->_putLine_mem_xor));
  else
    grDrv->_putLine_mem_xor = TGE_putLine;

  if (FP_OFF(grDrv->_putLineInv_scr_copy))	/* putLineInv_scr_copy */
    (void*)grDrv->_putLineInv_scr_copy = MK_FP(seg, FP_OFF(grDrv->_putLineInv_scr_copy));
  else
    grDrv->_putLineInv_scr_copy = TGE_putLineInv;
  if (FP_OFF(grDrv->_putLineInv_scr_and))	/* putLineInv_scr_and */
    (void*)grDrv->_putLineInv_scr_and = MK_FP(seg, FP_OFF(grDrv->_putLineInv_scr_and));
  else
    grDrv->_putLineInv_scr_and = TGE_putLineInv;
  if (FP_OFF(grDrv->_putLineInv_scr_not))	/* putLineInv_scr_not */
    (void*)grDrv->_putLineInv_scr_not = MK_FP(seg, FP_OFF(grDrv->_putLineInv_scr_not));
  else
    grDrv->_putLineInv_scr_not = TGE_putLineInv;
  if (FP_OFF(grDrv->_putLineInv_scr_or))	/* putLineInv_scr_or */
    (void*)grDrv->_putLineInv_scr_or = MK_FP(seg, FP_OFF(grDrv->_putLineInv_scr_or));
  else
    grDrv->_putLineInv_scr_or = TGE_putLineInv;
  if (FP_OFF(grDrv->_putLineInv_scr_xor))	/* putLineInv_scr_xor */
    (void*)grDrv->_putLineInv_scr_xor = MK_FP(seg, FP_OFF(grDrv->_putLineInv_scr_xor));
  else
    grDrv->_putLineInv_scr_xor = TGE_putLineInv;
  if (FP_OFF(grDrv->_putLineInv_mem_copy))	/* putLineInv_mem_copy */
    (void*)grDrv->_putLineInv_mem_copy = MK_FP(seg, FP_OFF(grDrv->_putLineInv_mem_copy));
  else
    grDrv->_putLineInv_mem_copy = TGE_putLineInv;
  if (FP_OFF(grDrv->_putLineInv_mem_and))	/* putLineInv_mem_and */
    (void*)grDrv->_putLineInv_mem_and = MK_FP(seg, FP_OFF(grDrv->_putLineInv_mem_and));
  else
    grDrv->_putLineInv_mem_and = TGE_putLineInv;
  if (FP_OFF(grDrv->_putLineInv_mem_not))	/* putLineInv_mem_not */
    (void*)grDrv->_putLineInv_mem_not = MK_FP(seg, FP_OFF(grDrv->_putLineInv_mem_not));
  else
    grDrv->_putLineInv_mem_not = TGE_putLineInv;
  if (FP_OFF(grDrv->_putLineInv_mem_or))	/* putLineInv_mem_or */
    (void*)grDrv->_putLineInv_mem_or = MK_FP(seg, FP_OFF(grDrv->_putLineInv_mem_or));
  else
    grDrv->_putLineInv_mem_or = TGE_putLineInv;
  if (FP_OFF(grDrv->_putLineInv_mem_xor))	/* putLineInv_mem_xor */
    (void*)grDrv->_putLineInv_mem_xor = MK_FP(seg, FP_OFF(grDrv->_putLineInv_mem_xor));
  else
    grDrv->_putLineInv_mem_xor = TGE_putLineInv;

  if (FP_OFF(grDrv->_putPixel_scr_copy))	/* putPixel_scr_copy */
    (void*)grDrv->_putPixel_scr_copy = MK_FP(seg, FP_OFF(grDrv->_putPixel_scr_copy));
  else
    grDrv->_putPixel_scr_copy = TGE_putPixel_scr_copy;
  if (FP_OFF(grDrv->_putPixel_scr_and))		/* putPixel_scr_and */
    (void*)grDrv->_putPixel_scr_and = MK_FP(seg, FP_OFF(grDrv->_putPixel_scr_and));
  else
    grDrv->_putPixel_scr_and = TGE_putPixel_scr_and;
  if (FP_OFF(grDrv->_putPixel_scr_not))		/* putPixel_scr_not */
    (void*)grDrv->_putPixel_scr_not = MK_FP(seg, FP_OFF(grDrv->_putPixel_scr_not));
  else
    grDrv->_putPixel_scr_not = TGE_putPixel_scr_not;
  if (FP_OFF(grDrv->_putPixel_scr_or))		/* putPixel_scr_or */
    (void*)grDrv->_putPixel_scr_or = MK_FP(seg, FP_OFF(grDrv->_putPixel_scr_or));
  else
    grDrv->_putPixel_scr_or = TGE_putPixel_scr_or;
  if (FP_OFF(grDrv->_putPixel_scr_xor))		/* putPixel_scr_xor */
    (void*)grDrv->_putPixel_scr_xor = MK_FP(seg, FP_OFF(grDrv->_putPixel_scr_xor));
  else
    grDrv->_putPixel_scr_xor = TGE_putPixel_scr_xor;
  if (FP_OFF(grDrv->_putPixel_mem_copy))	/* putPixel_mem_copy */
    (void*)grDrv->_putPixel_mem_copy = MK_FP(seg, FP_OFF(grDrv->_putPixel_mem_copy));
  else
    grDrv->_putPixel_mem_copy = TGE_putPixel_mem_copy;
  if (FP_OFF(grDrv->_putPixel_mem_and))		/* putPixel_mem_and */
    (void*)grDrv->_putPixel_mem_and = MK_FP(seg, FP_OFF(grDrv->_putPixel_mem_and));
  else
    grDrv->_putPixel_mem_and = TGE_putPixel_mem_and;
  if (FP_OFF(grDrv->_putPixel_mem_not))		/* putPixel_mem_not */
    (void*)grDrv->_putPixel_mem_not = MK_FP(seg, FP_OFF(grDrv->_putPixel_mem_not));
  else
    grDrv->_putPixel_mem_not = TGE_putPixel_mem_not;
  if (FP_OFF(grDrv->_putPixel_mem_or))		/* putPixel_mem_or */
    (void*)grDrv->_putPixel_mem_or = MK_FP(seg, FP_OFF(grDrv->_putPixel_mem_or));
  else
    grDrv->_putPixel_mem_or = TGE_putPixel_mem_or;
  if (FP_OFF(grDrv->_putPixel_mem_xor))		/* putPixel_mem_xor */
    (void*)grDrv->_putPixel_mem_xor = MK_FP(seg, FP_OFF(grDrv->_putPixel_mem_xor));
  else
    grDrv->_putPixel_mem_xor = TGE_putPixel_mem_xor;

  if (FP_OFF(grDrv->_line_scr_copy))		/* line_scr_copy */
    (void*)grDrv->_line_scr_copy = MK_FP(seg, FP_OFF(grDrv->_line_scr_copy));
  else
    grDrv->_line_scr_copy = TGE_line;
  if (FP_OFF(grDrv->_line_scr_and))		/* line_scr_and */
    (void*)grDrv->_line_scr_and = MK_FP(seg, FP_OFF(grDrv->_line_scr_and));
  else
    grDrv->_line_scr_and = TGE_line;
  if (FP_OFF(grDrv->_line_scr_not))		/* line_scr_not */
    (void*)grDrv->_line_scr_not = MK_FP(seg, FP_OFF(grDrv->_line_scr_not));
  else
    grDrv->_line_scr_not = TGE_line;
  if (FP_OFF(grDrv->_line_scr_or))		/* line_scr_or */
    (void*)grDrv->_line_scr_or = MK_FP(seg, FP_OFF(grDrv->_line_scr_or));
  else
    grDrv->_line_scr_or = TGE_line;
  if (FP_OFF(grDrv->_line_scr_xor))		/* line_scr_xor */
    (void*)grDrv->_line_scr_xor = MK_FP(seg, FP_OFF(grDrv->_line_scr_xor));
  else
    grDrv->_line_scr_xor = TGE_line;
  if (FP_OFF(grDrv->_line_mem_copy))		/* line_mem_copy */
    (void*)grDrv->_line_mem_copy = MK_FP(seg, FP_OFF(grDrv->_line_mem_copy));
  else
    grDrv->_line_mem_copy = TGE_line;
  if (FP_OFF(grDrv->_line_mem_and))		/* line_mem_and */
    (void*)grDrv->_line_mem_and = MK_FP(seg, FP_OFF(grDrv->_line_mem_and));
  else
    grDrv->_line_mem_and = TGE_line;
  if (FP_OFF(grDrv->_line_mem_not))		/* line_mem_not */
    (void*)grDrv->_line_mem_not = MK_FP(seg, FP_OFF(grDrv->_line_mem_not));
  else
    grDrv->_line_mem_not = TGE_line;
  if (FP_OFF(grDrv->_line_mem_or))		/* line_mem_or */
    (void*)grDrv->_line_mem_or = MK_FP(seg, FP_OFF(grDrv->_line_mem_or));
  else
    grDrv->_line_mem_or = TGE_line;
  if (FP_OFF(grDrv->_line_mem_xor))		/* line_mem_xor */
    (void*)grDrv->_line_mem_xor = MK_FP(seg, FP_OFF(grDrv->_line_mem_xor));
  else
    grDrv->_line_mem_xor = TGE_line;

  if (FP_OFF(grDrv->_horizLine_scr_copy))	/* horizLine_scr_copy */
    (void*)grDrv->_horizLine_scr_copy = MK_FP(seg, FP_OFF(grDrv->_horizLine_scr_copy));
  else
    grDrv->_horizLine_scr_copy = TGE_horizLine;
  if (FP_OFF(grDrv->_horizLine_scr_and))	/* horizLine_scr_and */
    (void*)grDrv->_horizLine_scr_and = MK_FP(seg, FP_OFF(grDrv->_horizLine_scr_and));
  else
    grDrv->_horizLine_scr_and = TGE_horizLine;
  if (FP_OFF(grDrv->_horizLine_scr_not))	/* horizLine_scr_not */
    (void*)grDrv->_horizLine_scr_not = MK_FP(seg, FP_OFF(grDrv->_horizLine_scr_not));
  else
    grDrv->_horizLine_scr_not = TGE_horizLine;
  if (FP_OFF(grDrv->_horizLine_scr_or))		/* horizLine_scr_or */
    (void*)grDrv->_horizLine_scr_or = MK_FP(seg, FP_OFF(grDrv->_horizLine_scr_or));
  else
    grDrv->_horizLine_scr_or = TGE_horizLine;
  if (FP_OFF(grDrv->_horizLine_scr_xor))	/* horizLine_scr_xor */
    (void*)grDrv->_horizLine_scr_xor = MK_FP(seg, FP_OFF(grDrv->_horizLine_scr_xor));
  else
    grDrv->_horizLine_scr_xor = TGE_horizLine;
  if (FP_OFF(grDrv->_horizLine_mem_copy))	/* horizLine_mem_copy */
    (void*)grDrv->_horizLine_mem_copy = MK_FP(seg, FP_OFF(grDrv->_horizLine_mem_copy));
  else
    grDrv->_horizLine_mem_copy = TGE_horizLine;
  if (FP_OFF(grDrv->_horizLine_mem_and))	/* horizLine_mem_and */
    (void*)grDrv->_horizLine_mem_and = MK_FP(seg, FP_OFF(grDrv->_horizLine_mem_and));
  else
    grDrv->_horizLine_mem_and = TGE_horizLine;
  if (FP_OFF(grDrv->_horizLine_mem_not))	/* horizLine_mem_not */
    (void*)grDrv->_horizLine_mem_not = MK_FP(seg, FP_OFF(grDrv->_horizLine_mem_not));
  else
    grDrv->_horizLine_mem_not = TGE_horizLine;
  if (FP_OFF(grDrv->_horizLine_mem_or))		/* horizLine_mem_or */
    (void*)grDrv->_horizLine_mem_or = MK_FP(seg, FP_OFF(grDrv->_horizLine_mem_or));
  else
    grDrv->_horizLine_mem_or = TGE_horizLine;
  if (FP_OFF(grDrv->_horizLine_mem_xor))	/* horizLine_mem_xor */
    (void*)grDrv->_horizLine_mem_xor = MK_FP(seg, FP_OFF(grDrv->_horizLine_mem_xor));
  else
    grDrv->_horizLine_mem_xor = TGE_horizLine;

  if (FP_OFF(grDrv->_vertLine_scr_copy))	/* vertLine_scr_copy */
    (void*)grDrv->_vertLine_scr_copy = MK_FP(seg, FP_OFF(grDrv->_vertLine_scr_copy));
  else
    grDrv->_vertLine_scr_copy = TGE_vertLine;
  if (FP_OFF(grDrv->_vertLine_scr_and))		/* vertLine_scr_and */
    (void*)grDrv->_vertLine_scr_and = MK_FP(seg, FP_OFF(grDrv->_vertLine_scr_and));
  else
    grDrv->_vertLine_scr_and = TGE_vertLine;
  if (FP_OFF(grDrv->_vertLine_scr_not))		/* vertLine_scr_not */
    (void*)grDrv->_vertLine_scr_not = MK_FP(seg, FP_OFF(grDrv->_vertLine_scr_not));
  else
    grDrv->_vertLine_scr_not = TGE_vertLine;
  if (FP_OFF(grDrv->_vertLine_scr_or))		/* vertLine_scr_or */
    (void*)grDrv->_vertLine_scr_or = MK_FP(seg, FP_OFF(grDrv->_vertLine_scr_or));
  else
    grDrv->_vertLine_scr_or = TGE_vertLine;
  if (FP_OFF(grDrv->_vertLine_scr_xor))		/* vertLine_scr_xor */
    (void*)grDrv->_vertLine_scr_xor = MK_FP(seg, FP_OFF(grDrv->_vertLine_scr_xor));
  else
    grDrv->_vertLine_scr_xor = TGE_vertLine;
  if (FP_OFF(grDrv->_vertLine_mem_copy))	/* vertLine_mem_copy */
    (void*)grDrv->_vertLine_mem_copy = MK_FP(seg, FP_OFF(grDrv->_vertLine_mem_copy));
  else
    grDrv->_vertLine_mem_copy = TGE_vertLine;
  if (FP_OFF(grDrv->_vertLine_mem_and))		/* vertLine_mem_and */
    (void*)grDrv->_vertLine_mem_and = MK_FP(seg, FP_OFF(grDrv->_vertLine_mem_and));
  else
    grDrv->_vertLine_mem_and = TGE_vertLine;
  if (FP_OFF(grDrv->_vertLine_mem_not))		/* vertLine_mem_not */
    (void*)grDrv->_vertLine_mem_not = MK_FP(seg, FP_OFF(grDrv->_vertLine_mem_not));
  else
    grDrv->_vertLine_mem_not = TGE_vertLine;
  if (FP_OFF(grDrv->_vertLine_mem_or))		/* vertLine_mem_or */
    (void*)grDrv->_vertLine_mem_or = MK_FP(seg, FP_OFF(grDrv->_vertLine_mem_or));
  else
    grDrv->_vertLine_mem_or = TGE_vertLine;
  if (FP_OFF(grDrv->_vertLine_mem_xor))		/* vertLine_mem_xor */
    (void*)grDrv->_vertLine_mem_xor = MK_FP(seg, FP_OFF(grDrv->_vertLine_mem_xor));
  else
    grDrv->_vertLine_mem_xor = TGE_vertLine;

  if (FP_OFF(grDrv->_drawRect_scr_copy))	/* drawRect_scr_copy */
    (void*)grDrv->_drawRect_scr_copy = MK_FP(seg, FP_OFF(grDrv->_drawRect_scr_copy));
  else
    grDrv->_drawRect_scr_copy = TGE_drawRect;
  if (FP_OFF(grDrv->_drawRect_scr_and))		/* drawRect_scr_and */
    (void*)grDrv->_drawRect_scr_and = MK_FP(seg, FP_OFF(grDrv->_drawRect_scr_and));
  else
    grDrv->_drawRect_scr_and = TGE_drawRect;
  if (FP_OFF(grDrv->_drawRect_scr_not))		/* drawRect_scr_not */
    (void*)grDrv->_drawRect_scr_not = MK_FP(seg, FP_OFF(grDrv->_drawRect_scr_not));
  else
    grDrv->_drawRect_scr_not = TGE_drawRect;
  if (FP_OFF(grDrv->_drawRect_scr_or))		/* drawRect_scr_or */
    (void*)grDrv->_drawRect_scr_or = MK_FP(seg, FP_OFF(grDrv->_drawRect_scr_or));
  else
    grDrv->_drawRect_scr_or = TGE_drawRect;
  if (FP_OFF(grDrv->_drawRect_scr_xor))		/* drawRect_scr_xor */
    (void*)grDrv->_drawRect_scr_xor = MK_FP(seg, FP_OFF(grDrv->_drawRect_scr_xor));
  else
    grDrv->_drawRect_scr_xor = TGE_drawRect;
  if (FP_OFF(grDrv->_drawRect_mem_copy))	/* drawRect_mem_copy */
    (void*)grDrv->_drawRect_mem_copy = MK_FP(seg, FP_OFF(grDrv->_drawRect_mem_copy));
  else
    grDrv->_drawRect_mem_copy = TGE_drawRect;
  if (FP_OFF(grDrv->_drawRect_mem_and))		/* drawRect_mem_and */
    (void*)grDrv->_drawRect_mem_and = MK_FP(seg, FP_OFF(grDrv->_drawRect_mem_and));
  else
    grDrv->_drawRect_mem_and = TGE_drawRect;
  if (FP_OFF(grDrv->_drawRect_mem_not))		/* drawRect_mem_not */
    (void*)grDrv->_drawRect_mem_not = MK_FP(seg, FP_OFF(grDrv->_drawRect_mem_not));
  else
    grDrv->_drawRect_mem_not = TGE_drawRect;
  if (FP_OFF(grDrv->_drawRect_mem_or))		/* drawRect_mem_or */
    (void*)grDrv->_drawRect_mem_or = MK_FP(seg, FP_OFF(grDrv->_drawRect_mem_or));
  else
    grDrv->_drawRect_mem_or = TGE_drawRect;
  if (FP_OFF(grDrv->_drawRect_mem_xor))		/* drawRect_mem_xor */
    (void*)grDrv->_drawRect_mem_xor = MK_FP(seg, FP_OFF(grDrv->_drawRect_mem_xor));
  else
    grDrv->_drawRect_mem_xor = TGE_drawRect;

  if (FP_OFF(grDrv->_filledRect_scr_copy))	/* filledRect_scr_copy */
    (void*)grDrv->_filledRect_scr_copy = MK_FP(seg, FP_OFF(grDrv->_filledRect_scr_copy));
  else
    grDrv->_filledRect_scr_copy = TGE_filledRect;
  if (FP_OFF(grDrv->_filledRect_scr_and))	/* filledRect_scr_and */
    (void*)grDrv->_filledRect_scr_and = MK_FP(seg, FP_OFF(grDrv->_filledRect_scr_and));
  else
    grDrv->_filledRect_scr_and = TGE_filledRect;
  if (FP_OFF(grDrv->_filledRect_scr_not))	/* filledRect_scr_not */
    (void*)grDrv->_filledRect_scr_not = MK_FP(seg, FP_OFF(grDrv->_filledRect_scr_not));
  else
    grDrv->_filledRect_scr_not = TGE_filledRect;
  if (FP_OFF(grDrv->_filledRect_scr_or))	/* filledRect_scr_or */
    (void*)grDrv->_filledRect_scr_or = MK_FP(seg, FP_OFF(grDrv->_filledRect_scr_or));
  else
    grDrv->_filledRect_scr_or = TGE_filledRect;
  if (FP_OFF(grDrv->_filledRect_scr_xor))	/* filledRect_scr_xor */
    (void*)grDrv->_filledRect_scr_xor = MK_FP(seg, FP_OFF(grDrv->_filledRect_scr_xor));
  else
    grDrv->_filledRect_scr_xor = TGE_filledRect;
  if (FP_OFF(grDrv->_filledRect_mem_copy))	/* filledRect_mem_copy */
    (void*)grDrv->_filledRect_mem_copy = MK_FP(seg, FP_OFF(grDrv->_filledRect_mem_copy));
  else
    grDrv->_filledRect_mem_copy = TGE_filledRect;
  if (FP_OFF(grDrv->_filledRect_mem_and))	/* filledRect_mem_and */
    (void*)grDrv->_filledRect_mem_and = MK_FP(seg, FP_OFF(grDrv->_filledRect_mem_and));
  else
    grDrv->_filledRect_mem_and = TGE_filledRect;
  if (FP_OFF(grDrv->_filledRect_mem_not))	/* filledRect_mem_not */
    (void*)grDrv->_filledRect_mem_not = MK_FP(seg, FP_OFF(grDrv->_filledRect_mem_not));
  else
    grDrv->_filledRect_mem_not = TGE_filledRect;
  if (FP_OFF(grDrv->_filledRect_mem_or))	/* filledRect_mem_or */
    (void*)grDrv->_filledRect_mem_or = MK_FP(seg, FP_OFF(grDrv->_filledRect_mem_or));
  else
    grDrv->_filledRect_mem_or = TGE_filledRect;
  if (FP_OFF(grDrv->_filledRect_mem_xor))	/* filledRect_mem_xor */
    (void*)grDrv->_filledRect_mem_xor = MK_FP(seg, FP_OFF(grDrv->_filledRect_mem_xor));
  else
    grDrv->_filledRect_mem_xor = TGE_filledRect;

  if (FP_OFF(grDrv->_clearGraphics_scr_copy))	/* clearGraphics_scr_copy */
    (void*)grDrv->_clearGraphics_scr_copy = MK_FP(seg, FP_OFF(grDrv->_clearGraphics_scr_copy));
  else
    grDrv->_clearGraphics_scr_copy = TGE_clearGraphics;
  if (FP_OFF(grDrv->_clearGraphics_scr_and))	/* clearGraphics_scr_and */
    (void*)grDrv->_clearGraphics_scr_and = MK_FP(seg, FP_OFF(grDrv->_clearGraphics_scr_and));
  else
    grDrv->_clearGraphics_scr_and = TGE_clearGraphics;
  if (FP_OFF(grDrv->_clearGraphics_scr_not))	/* clearGraphics_scr_not */
    (void*)grDrv->_clearGraphics_scr_not = MK_FP(seg, FP_OFF(grDrv->_clearGraphics_scr_not));
  else
    grDrv->_clearGraphics_scr_not = TGE_clearGraphics;
  if (FP_OFF(grDrv->_clearGraphics_scr_or))	/* clearGraphics_scr_or */
    (void*)grDrv->_clearGraphics_scr_or = MK_FP(seg, FP_OFF(grDrv->_clearGraphics_scr_or));
  else
    grDrv->_clearGraphics_scr_or = TGE_clearGraphics;
  if (FP_OFF(grDrv->_clearGraphics_scr_xor))	/* clearGraphics_scr_xor */
    (void*)grDrv->_clearGraphics_scr_xor = MK_FP(seg, FP_OFF(grDrv->_clearGraphics_scr_xor));
  else
    grDrv->_clearGraphics_scr_xor = TGE_clearGraphics;
  if (FP_OFF(grDrv->_clearGraphics_mem_copy))	/* clearGraphics_mem_copy */
    (void*)grDrv->_clearGraphics_mem_copy = MK_FP(seg, FP_OFF(grDrv->_clearGraphics_mem_copy));
  else
    grDrv->_clearGraphics_mem_copy = TGE_clearGraphics;
  if (FP_OFF(grDrv->_clearGraphics_mem_and))	/* clearGraphics_mem_and */
    (void*)grDrv->_clearGraphics_mem_and = MK_FP(seg, FP_OFF(grDrv->_clearGraphics_mem_and));
  else
    grDrv->_clearGraphics_mem_and = TGE_clearGraphics;
  if (FP_OFF(grDrv->_clearGraphics_mem_not))	/* clearGraphics_mem_not */
    (void*)grDrv->_clearGraphics_mem_not = MK_FP(seg, FP_OFF(grDrv->_clearGraphics_mem_not));
  else
    grDrv->_clearGraphics_mem_not = TGE_clearGraphics;
  if (FP_OFF(grDrv->_clearGraphics_mem_or))	/* clearGraphics_mem_or */
    (void*)grDrv->_clearGraphics_mem_or = MK_FP(seg, FP_OFF(grDrv->_clearGraphics_mem_or));
  else
    grDrv->_clearGraphics_mem_or = TGE_clearGraphics;
  if (FP_OFF(grDrv->_clearGraphics_mem_xor))	/* clearGraphics_mem_xor */
    (void*)grDrv->_clearGraphics_mem_xor = MK_FP(seg, FP_OFF(grDrv->_clearGraphics_mem_xor));
  else
    grDrv->_clearGraphics_mem_xor = TGE_clearGraphics;

  if (FP_OFF(grDrv->_ellipse_scr_copy))		/* ellipse_scr_copy */
    (void*)grDrv->_ellipse_scr_copy = MK_FP(seg, FP_OFF(grDrv->_ellipse_scr_copy));
  else
    grDrv->_ellipse_scr_copy = TGE_ellipse;
  if (FP_OFF(grDrv->_ellipse_scr_and))		/* ellipse_scr_and */
    (void*)grDrv->_ellipse_scr_and = MK_FP(seg, FP_OFF(grDrv->_ellipse_scr_and));
  else
    grDrv->_ellipse_scr_and = TGE_ellipse;
  if (FP_OFF(grDrv->_ellipse_scr_not))		/* ellipse_scr_not */
    (void*)grDrv->_ellipse_scr_not = MK_FP(seg, FP_OFF(grDrv->_ellipse_scr_not));
  else
    grDrv->_ellipse_scr_not = TGE_ellipse;
  if (FP_OFF(grDrv->_ellipse_scr_or))		/* ellipse_scr_or */
    (void*)grDrv->_ellipse_scr_or = MK_FP(seg, FP_OFF(grDrv->_ellipse_scr_or));
  else
    grDrv->_ellipse_scr_or = TGE_ellipse;
  if (FP_OFF(grDrv->_ellipse_scr_xor))		/* ellipse_scr_xor */
    (void*)grDrv->_ellipse_scr_xor = MK_FP(seg, FP_OFF(grDrv->_ellipse_scr_xor));
  else
    grDrv->_ellipse_scr_xor = TGE_ellipse;
  if (FP_OFF(grDrv->_ellipse_mem_copy))		/* ellipse_mem_copy */
    (void*)grDrv->_ellipse_mem_copy = MK_FP(seg, FP_OFF(grDrv->_ellipse_mem_copy));
  else
    grDrv->_ellipse_mem_copy = TGE_ellipse;
  if (FP_OFF(grDrv->_ellipse_mem_and))		/* ellipse_mem_and */
    (void*)grDrv->_ellipse_mem_and = MK_FP(seg, FP_OFF(grDrv->_ellipse_mem_and));
  else
    grDrv->_ellipse_mem_and = TGE_ellipse;
  if (FP_OFF(grDrv->_ellipse_mem_not))		/* ellipse_mem_not */
    (void*)grDrv->_ellipse_mem_not = MK_FP(seg, FP_OFF(grDrv->_ellipse_mem_not));
  else
    grDrv->_ellipse_mem_not = TGE_ellipse;
  if (FP_OFF(grDrv->_ellipse_mem_or))		/* ellipse_mem_or */
    (void*)grDrv->_ellipse_mem_or = MK_FP(seg, FP_OFF(grDrv->_ellipse_mem_or));
  else
    grDrv->_ellipse_mem_or = TGE_ellipse;
  if (FP_OFF(grDrv->_ellipse_mem_xor))		/* ellipse_mem_xor */
    (void*)grDrv->_ellipse_mem_xor = MK_FP(seg, FP_OFF(grDrv->_ellipse_mem_xor));
  else
    grDrv->_ellipse_mem_xor = TGE_ellipse;

  if (FP_OFF(grDrv->_filledEllipse_scr_copy))	/* filledEllipse_scr_copy */
    (void*)grDrv->_filledEllipse_scr_copy = MK_FP(seg, FP_OFF(grDrv->_filledEllipse_scr_copy));
  else
    grDrv->_filledEllipse_scr_copy = TGE_filledEllipse;
  if (FP_OFF(grDrv->_filledEllipse_scr_and))	/* filledEllipse_scr_and */
    (void*)grDrv->_filledEllipse_scr_and = MK_FP(seg, FP_OFF(grDrv->_filledEllipse_scr_and));
  else
    grDrv->_filledEllipse_scr_and = TGE_filledEllipse;
  if (FP_OFF(grDrv->_filledEllipse_scr_not))	/* filledEllipse_scr_not */
    (void*)grDrv->_filledEllipse_scr_not = MK_FP(seg, FP_OFF(grDrv->_filledEllipse_scr_not));
  else
    grDrv->_filledEllipse_scr_not = TGE_filledEllipse;
  if (FP_OFF(grDrv->_filledEllipse_scr_or))	/* filledEllipse_scr_or */
    (void*)grDrv->_filledEllipse_scr_or = MK_FP(seg, FP_OFF(grDrv->_filledEllipse_scr_or));
  else
    grDrv->_filledEllipse_scr_or = TGE_filledEllipse;
  if (FP_OFF(grDrv->_filledEllipse_scr_xor))	/* filledEllipse_scr_xor */
    (void*)grDrv->_filledEllipse_scr_xor = MK_FP(seg, FP_OFF(grDrv->_filledEllipse_scr_xor));
  else
    grDrv->_filledEllipse_scr_xor = TGE_filledEllipse;
  if (FP_OFF(grDrv->_filledEllipse_mem_copy))	/* filledEllipse_mem_copy */
    (void*)grDrv->_filledEllipse_mem_copy = MK_FP(seg, FP_OFF(grDrv->_filledEllipse_mem_copy));
  else
    grDrv->_filledEllipse_mem_copy = TGE_filledEllipse;
  if (FP_OFF(grDrv->_filledEllipse_mem_and))	/* filledEllipse_mem_and */
    (void*)grDrv->_filledEllipse_mem_and = MK_FP(seg, FP_OFF(grDrv->_filledEllipse_mem_and));
  else
    grDrv->_filledEllipse_mem_and = TGE_filledEllipse;
  if (FP_OFF(grDrv->_filledEllipse_mem_not))	/* filledEllipse_mem_not */
    (void*)grDrv->_filledEllipse_mem_not = MK_FP(seg, FP_OFF(grDrv->_filledEllipse_mem_not));
  else
    grDrv->_filledEllipse_mem_not = TGE_filledEllipse;
  if (FP_OFF(grDrv->_filledEllipse_mem_or))	/* filledEllipse_mem_or */
    (void*)grDrv->_filledEllipse_mem_or = MK_FP(seg, FP_OFF(grDrv->_filledEllipse_mem_or));
  else
    grDrv->_filledEllipse_mem_or = TGE_filledEllipse;
  if (FP_OFF(grDrv->_filledEllipse_mem_xor))	/* filledEllipse_mem_xor */
    (void*)grDrv->_filledEllipse_mem_xor = MK_FP(seg, FP_OFF(grDrv->_filledEllipse_mem_xor));
  else
    grDrv->_filledEllipse_mem_xor = TGE_filledEllipse;

  if (FP_OFF(grDrv->_circle_scr_copy))		/* circle_scr_copy */
    (void*)grDrv->_circle_scr_copy = MK_FP(seg, FP_OFF(grDrv->_circle_scr_copy));
  else
    grDrv->_circle_scr_copy = TGE_circle;
  if (FP_OFF(grDrv->_circle_scr_and))		/* circle_scr_and */
    (void*)grDrv->_circle_scr_and = MK_FP(seg, FP_OFF(grDrv->_circle_scr_and));
  else
    grDrv->_circle_scr_and = TGE_circle;
  if (FP_OFF(grDrv->_circle_scr_not))		/* circle_scr_not */
    (void*)grDrv->_circle_scr_not = MK_FP(seg, FP_OFF(grDrv->_circle_scr_not));
  else
    grDrv->_circle_scr_not = TGE_circle;
  if (FP_OFF(grDrv->_circle_scr_or))		/* circle_scr_or */
    (void*)grDrv->_circle_scr_or = MK_FP(seg, FP_OFF(grDrv->_circle_scr_or));
  else
    grDrv->_circle_scr_or = TGE_circle;
  if (FP_OFF(grDrv->_circle_scr_xor))		/* circle_scr_xor */
    (void*)grDrv->_circle_scr_xor = MK_FP(seg, FP_OFF(grDrv->_circle_scr_xor));
  else
    grDrv->_circle_scr_xor = TGE_circle;
  if (FP_OFF(grDrv->_circle_mem_copy))		/* circle_mem_copy */
    (void*)grDrv->_circle_mem_copy = MK_FP(seg, FP_OFF(grDrv->_circle_mem_copy));
  else
    grDrv->_circle_mem_copy = TGE_circle;
  if (FP_OFF(grDrv->_circle_mem_and))		/* circle_mem_and */
    (void*)grDrv->_circle_mem_and = MK_FP(seg, FP_OFF(grDrv->_circle_mem_and));
  else
    grDrv->_circle_mem_and = TGE_circle;
  if (FP_OFF(grDrv->_circle_mem_not))		/* circle_mem_not */
    (void*)grDrv->_circle_mem_not = MK_FP(seg, FP_OFF(grDrv->_circle_mem_not));
  else
    grDrv->_circle_mem_not = TGE_circle;
  if (FP_OFF(grDrv->_circle_mem_or))		/* circle_mem_or */
    (void*)grDrv->_circle_mem_or = MK_FP(seg, FP_OFF(grDrv->_circle_mem_or));
  else
    grDrv->_circle_mem_or = TGE_circle;
  if (FP_OFF(grDrv->_circle_mem_xor))		/* circle_mem_xor */
    (void*)grDrv->_circle_mem_xor = MK_FP(seg, FP_OFF(grDrv->_circle_mem_xor));
  else
    grDrv->_circle_mem_xor = TGE_circle;

  if (FP_OFF(grDrv->_filledCircle_scr_copy))	/* filledCircle_scr_copy */
    (void*)grDrv->_filledCircle_scr_copy = MK_FP(seg, FP_OFF(grDrv->_filledCircle_scr_copy));
  else
    grDrv->_filledCircle_scr_copy = TGE_filledCircle;
  if (FP_OFF(grDrv->_filledCircle_scr_and))	/* filledCircle_scr_and */
    (void*)grDrv->_filledCircle_scr_and = MK_FP(seg, FP_OFF(grDrv->_filledCircle_scr_and));
  else
    grDrv->_filledCircle_scr_and = TGE_filledCircle;
  if (FP_OFF(grDrv->_filledCircle_scr_not))	/* filledCircle_scr_not */
    (void*)grDrv->_filledCircle_scr_not = MK_FP(seg, FP_OFF(grDrv->_filledCircle_scr_not));
  else
    grDrv->_filledCircle_scr_not = TGE_filledCircle;
  if (FP_OFF(grDrv->_filledCircle_scr_or))	/* filledCircle_scr_or */
    (void*)grDrv->_filledCircle_scr_or = MK_FP(seg, FP_OFF(grDrv->_filledCircle_scr_or));
  else
    grDrv->_filledCircle_scr_or = TGE_filledCircle;
  if (FP_OFF(grDrv->_filledCircle_scr_xor))	/* filledCircle_scr_xor */
    (void*)grDrv->_filledCircle_scr_xor = MK_FP(seg, FP_OFF(grDrv->_filledCircle_scr_xor));
  else
    grDrv->_filledCircle_scr_xor = TGE_filledCircle;
  if (FP_OFF(grDrv->_filledCircle_mem_copy))	/* filledCircle_mem_copy */
    (void*)grDrv->_filledCircle_mem_copy = MK_FP(seg, FP_OFF(grDrv->_filledCircle_mem_copy));
  else
    grDrv->_filledCircle_mem_copy = TGE_filledCircle;
  if (FP_OFF(grDrv->_filledCircle_mem_and))	/* filledCircle_mem_and */
    (void*)grDrv->_filledCircle_mem_and = MK_FP(seg, FP_OFF(grDrv->_filledCircle_mem_and));
  else
    grDrv->_filledCircle_mem_and = TGE_filledCircle;
  if (FP_OFF(grDrv->_filledCircle_mem_not))	/* filledCircle_mem_not */
    (void*)grDrv->_filledCircle_mem_not = MK_FP(seg, FP_OFF(grDrv->_filledCircle_mem_not));
  else
    grDrv->_filledCircle_mem_not = TGE_filledCircle;
  if (FP_OFF(grDrv->_filledCircle_mem_or))	/* filledCircle_mem_or */
    (void*)grDrv->_filledCircle_mem_or = MK_FP(seg, FP_OFF(grDrv->_filledCircle_mem_or));
  else
    grDrv->_filledCircle_mem_or = TGE_filledCircle;
  if (FP_OFF(grDrv->_filledCircle_mem_xor))	/* filledCircle_mem_xor */
    (void*)grDrv->_filledCircle_mem_xor = MK_FP(seg, FP_OFF(grDrv->_filledCircle_mem_xor));
  else
    grDrv->_filledCircle_mem_xor = TGE_filledCircle;

  if (FP_OFF(grDrv->_fillRegion_scr_copy))	/* fillRegion_scr_copy */
    (void*)grDrv->_fillRegion_scr_copy = MK_FP(seg, FP_OFF(grDrv->_fillRegion_scr_copy));
  else
    grDrv->_fillRegion_scr_copy = TGE_fillRegion;
  if (FP_OFF(grDrv->_fillRegion_scr_and))	/* fillRegion_scr_and */
    (void*)grDrv->_fillRegion_scr_and = MK_FP(seg, FP_OFF(grDrv->_fillRegion_scr_and));
  else
    grDrv->_fillRegion_scr_and = TGE_fillRegion;
  if (FP_OFF(grDrv->_fillRegion_scr_not))	/* fillRegion_scr_not */
    (void*)grDrv->_fillRegion_scr_not = MK_FP(seg, FP_OFF(grDrv->_fillRegion_scr_not));
  else
    grDrv->_fillRegion_scr_not = TGE_fillRegion;
  if (FP_OFF(grDrv->_fillRegion_scr_or))	/* fillRegion_scr_or */
    (void*)grDrv->_fillRegion_scr_or = MK_FP(seg, FP_OFF(grDrv->_fillRegion_scr_or));
  else
    grDrv->_fillRegion_scr_or = TGE_fillRegion;
  if (FP_OFF(grDrv->_fillRegion_scr_xor))	/* fillRegion_scr_xor */
    (void*)grDrv->_fillRegion_scr_xor = MK_FP(seg, FP_OFF(grDrv->_fillRegion_scr_xor));
  else
    grDrv->_fillRegion_scr_xor = TGE_fillRegion;
  if (FP_OFF(grDrv->_fillRegion_mem_copy))	/* fillRegion_mem_copy */
    (void*)grDrv->_fillRegion_mem_copy = MK_FP(seg, FP_OFF(grDrv->_fillRegion_mem_copy));
  else
    grDrv->_fillRegion_mem_copy = TGE_fillRegion;
  if (FP_OFF(grDrv->_fillRegion_mem_and))	/* fillRegion_mem_and */
    (void*)grDrv->_fillRegion_mem_and = MK_FP(seg, FP_OFF(grDrv->_fillRegion_mem_and));
  else
    grDrv->_fillRegion_mem_and = TGE_fillRegion;
  if (FP_OFF(grDrv->_fillRegion_mem_not))	/* fillRegion_mem_not */
    (void*)grDrv->_fillRegion_mem_not = MK_FP(seg, FP_OFF(grDrv->_fillRegion_mem_not));
  else
    grDrv->_fillRegion_mem_not = TGE_fillRegion;
  if (FP_OFF(grDrv->_fillRegion_mem_or))	/* fillRegion_mem_or */
    (void*)grDrv->_fillRegion_mem_or = MK_FP(seg, FP_OFF(grDrv->_fillRegion_mem_or));
  else
    grDrv->_fillRegion_mem_or = TGE_fillRegion;
  if (FP_OFF(grDrv->_fillRegion_mem_xor))	/* fillRegion_mem_xor */
    (void*)grDrv->_fillRegion_mem_xor = MK_FP(seg, FP_OFF(grDrv->_fillRegion_mem_xor));
  else
    grDrv->_fillRegion_mem_xor = TGE_fillRegion;

  if (FP_OFF(grDrv->_fillLine_scr_copy))	/* fillLine_scr_copy */
    (void*)grDrv->_fillLine_scr_copy = MK_FP(seg, FP_OFF(grDrv->_fillLine_scr_copy));
  else
    grDrv->_fillLine_scr_copy = TGEsys->_horizLine_scr_copy;
  if (FP_OFF(grDrv->_fillLine_scr_and))		/* fillLine_scr_and */
    (void*)grDrv->_fillLine_scr_and = MK_FP(seg, FP_OFF(grDrv->_fillLine_scr_and));
  else
    grDrv->_fillLine_scr_and = TGEsys->_horizLine_scr_and;
  if (FP_OFF(grDrv->_fillLine_scr_not))		/* fillLine_scr_not */
    (void*)grDrv->_fillLine_scr_not = MK_FP(seg, FP_OFF(grDrv->_fillLine_scr_not));
  else
    grDrv->_fillLine_scr_not = TGEsys->_horizLine_scr_not;
  if (FP_OFF(grDrv->_fillLine_scr_or))		/* fillLine_scr_or */
    (void*)grDrv->_fillLine_scr_or = MK_FP(seg, FP_OFF(grDrv->_fillLine_scr_or));
  else
    grDrv->_fillLine_scr_or = TGEsys->_horizLine_scr_or;
  if (FP_OFF(grDrv->_fillLine_scr_xor))		/* fillLine_scr_xor */
    (void*)grDrv->_fillLine_scr_xor = MK_FP(seg, FP_OFF(grDrv->_fillLine_scr_xor));
  else
    grDrv->_fillLine_scr_xor = TGEsys->_horizLine_scr_xor;
  if (FP_OFF(grDrv->_fillLine_mem_copy))	/* fillLine_mem_copy */
    (void*)grDrv->_fillLine_mem_copy = MK_FP(seg, FP_OFF(grDrv->_fillLine_mem_copy));
  else
    grDrv->_fillLine_mem_copy = TGEsys->_horizLine_mem_copy;
  if (FP_OFF(grDrv->_fillLine_mem_and))		/* fillLine_mem_and */
    (void*)grDrv->_fillLine_mem_and = MK_FP(seg, FP_OFF(grDrv->_fillLine_mem_and));
  else
    grDrv->_fillLine_mem_and = TGEsys->_horizLine_mem_and;
  if (FP_OFF(grDrv->_fillLine_mem_not))		/* fillLine_mem_not */
    (void*)grDrv->_fillLine_mem_not = MK_FP(seg, FP_OFF(grDrv->_fillLine_mem_not));
  else
    grDrv->_fillLine_mem_not = TGEsys->_horizLine_mem_not;
  if (FP_OFF(grDrv->_fillLine_mem_or))		/* fillLine_mem_or */
    (void*)grDrv->_fillLine_mem_or = MK_FP(seg, FP_OFF(grDrv->_fillLine_mem_or));
  else
    grDrv->_fillLine_mem_or = TGEsys->_horizLine_mem_or;
  if (FP_OFF(grDrv->_fillLine_mem_xor))		/* fillLine_mem_xor */
    (void*)grDrv->_fillLine_mem_xor = MK_FP(seg, FP_OFF(grDrv->_fillLine_mem_xor));
  else
    grDrv->_fillLine_mem_xor = TGEsys->_horizLine_mem_xor;


  /* Set up currently active functions */
  setInputFunctions(SCREEN);
  setOutputFunctions(SCREEN, TGE_COPY_PUT);

  /* Set up the currently active viewports */
  TGE_setViewports(0, 0, OUTMAXX, OUTMAXY);
}


/*****
****** Free graphics driver memory
*****/

void TGE_unloadGraphDriver(void)
{
  if (drvAddr)
  {
    farfree(drvAddr);
    drvAddr = NULL;
  }
}


/*****
****** Virtual/real screen switching
*****/

void far TGE_setGraphicsAddr(void far *addr)
{
  TGE_setGraphicsInputAddr(addr);		/* configure input */
  TGE_setGraphicsOutputAddr(addr);		/* configure output */
}

void far TGE_setGraphicsInputAddr(void far *addr)
{
  register unsigned wide, deep;

  if (addr != NULL)	/* make a virtual screen active */
  {
    wide = ((unsigned*)addr)[0];	/* get memory block dimensions */
    deep = ((unsigned*)addr)[1];
    INADDR = (char*) addr + 4;		/* store as active address */
    INSCREENWIDE = wide;		/* needed for virtual screens */
    INMAXX = wide - 1;			/* adjust current input */
    INMAXY = deep - 1;			/* screen dimensions    */
    TGE_setInputViewport(0, 0, wide-1, deep-1);	/* viewport */
    setInputFunctions(MEMORY);		/* update TGE's function pointers */
  }
  else			/* make the screen active */
  {
    INADDR = TGEsys->scrAddr;		/* revert to screen address */
    INSCREENWIDE = wide;		/* needed for virtual screens */
    INMAXX = SCREENMAXX;		/* restore screen dimensions */
    INMAXY = SCREENMAXY;
    TGE_setInputViewport(0, 0, INMAXX, INMAXY);	/* viewport */
    setInputFunctions(SCREEN);		/* update TGE's function pointers */
  }
}

void far TGE_setGraphicsOutputAddr(void far *addr)
{
  register unsigned wide, deep;

  if (addr != NULL)	/* make a virtual screen active */
  {
    wide = ((unsigned*)addr)[0];	/* get memory block dimensions */
    deep = ((unsigned*)addr)[1];
    OUTADDR = (char*) addr + 4;		/* store as active address */
    OUTSCREENWIDE = wide;		/* needed for virtual screens */
    OUTMAXX = wide - 1;			/* adjust current screen */
    OUTMAXY = deep - 1;			/* dimensions */
    TGE_setOutputViewport(0, 0, wide-1, deep-1); /* viewport */
    setOutputFunctions(MEMORY, curOutputMode);	/* update TGE's function pointers */
    curOutputDest = MEMORY;		/* store current destination */
  }
  else			/* make the screen active */
  {
    OUTADDR = TGEsys->scrAddr;		/* revert to screen address */
    OUTSCREENWIDE = wide;		/* needed for virtual screens */
    OUTMAXX = SCREENMAXX;		/* restore screen dimensions */
    OUTMAXY = SCREENMAXY;
    TGE_setOutputViewport(0, 0, OUTMAXX, OUTMAXY); /* viewport */
    setOutputFunctions(SCREEN, curOutputMode);	/* update TGE's function pointers */
    curOutputDest = SCREEN;		/* store current destination */
  }
}

void far *TGE_getGraphicsOutputAddr(void)
{
  if (OUTADDR == SCRADDR)
    return (NULL);
  else
    return (OUTADDR);
}

void far *TGE_getGraphicsInputAddr(void)
{
  if (INADDR == SCRADDR)
    return (NULL);
  else
    return (INADDR);
}

void far *TGE_makeVirtScreen(unsigned wide, unsigned deep)
{
  void far *addr;

  if (wide && deep)			/* ensure wide>0 and deep>0 */
  {
    addr = farmalloc(TGEsys->_imageSizeDim(wide,deep));	/* allocate some RAM */
    if (addr == NULL)
      return (NULL);
    else
    {
      ((unsigned*)addr)[0] = wide;	/* store image width */
      ((unsigned*)addr)[1] = deep;	/* store image depth */
      return (addr);			/* return the address */
    }
  }
  else
    return (NULL);
}


/*****
****** Change the current output mode
*****/

void TGE_setOutputMode(int mode)
{
  curOutputMode = mode;				/* store as current mode */
  setOutputFunctions(curOutputDest, mode);	/* setup for the new mode */
}


/*****
****** Update function pointers given output mode and destination.
*****/

void near setInputFunctions(int memOrScreen)
{
  if (memOrScreen == MEMORY)	/* set virtual screen active */
  {
    TGEsys->_getImage      = TGEsys->_getImage_mem;	 /* getImage      */
    TGEsys->_getLine       = TGEsys->_getLine_mem;	 /* getLine       */
    TGEsys->_getPixel      = TGEsys->_getPixel_mem;	 /* getPixel      */
  }
  else				/* set real screen active */
  {
    TGEsys->_getImage      = TGEsys->_getImage_scr;	 /* getImage      */
    TGEsys->_getLine       = TGEsys->_getLine_scr;	 /* getLine       */
    TGEsys->_getPixel      = TGEsys->_getPixel_scr;	 /* getPixel      */
  }
}

void near setOutputFunctions(int memOrScreen, int writeMode)
{
  if (memOrScreen == MEMORY)	/* set virtual screen active */
  {
    switch (writeMode)
    {
      case TGE_COPY_PUT:	/* output using COPY */
	TGEsys->_putImage      = TGEsys->_putImage_mem_copy;	  /* putImage      */
	TGEsys->_putImageInv   = TGEsys->_putImageInv_mem_copy;	  /* putImageInv   */
	TGEsys->_putLine       = TGEsys->_putLine_mem_copy;	  /* putLine       */
	TGEsys->_putLineInv    = TGEsys->_putLineInv_mem_copy;	  /* putLineInv    */
	TGEsys->_putPixel      = TGEsys->_putPixel_mem_copy;	  /* putPixel      */
	TGEsys->_line          = TGEsys->_line_mem_copy;	  /* line          */
	TGEsys->_horizLine     = TGEsys->_horizLine_mem_copy;     /* horizLine     */
	TGEsys->_vertLine      = TGEsys->_vertLine_mem_copy;   	  /* vertLine      */
	TGEsys->_drawRect      = TGEsys->_drawRect_mem_copy;	  /* drawRect      */
	TGEsys->_filledRect    = TGEsys->_filledRect_mem_copy;    /* filledRect    */
	TGEsys->_clearGraphics = TGEsys->_clearGraphics_mem_copy; /* clearGraphics */
	TGEsys->_ellipse       = TGEsys->_ellipse_mem_copy;	  /* ellipse       */
	TGEsys->_filledEllipse = TGEsys->_filledEllipse_mem_copy; /* filledEllipse */
	TGEsys->_circle        = TGEsys->_circle_mem_copy;	  /* circle        */
	TGEsys->_filledCircle  = TGEsys->_filledCircle_mem_copy;  /* filledCircle  */
	TGEsys->_fillRegion    = TGEsys->_fillRegion_mem_copy;	  /* fillRegion    */
	TGEsys->_fillLine      = TGEsys->_fillLine_mem_copy;	  /* fillLine      */
	break;
      case TGE_AND_PUT:		/* output using AND */
	TGEsys->_putImage      = TGEsys->_putImage_mem_and;	  /* putImage      */
	TGEsys->_putImageInv   = TGEsys->_putImageInv_mem_and;	  /* putImageInv   */
	TGEsys->_putLine       = TGEsys->_putLine_mem_and;	  /* putLine       */
	TGEsys->_putLineInv    = TGEsys->_putLineInv_mem_and;	  /* putLineInv    */
	TGEsys->_putPixel      = TGEsys->_putPixel_mem_and;	  /* putPixel      */
	TGEsys->_line          = TGEsys->_line_mem_and;		  /* line          */
	TGEsys->_horizLine     = TGEsys->_horizLine_mem_and;      /* horizLine     */
	TGEsys->_vertLine      = TGEsys->_vertLine_mem_and;   	  /* vertLine      */
	TGEsys->_drawRect      = TGEsys->_drawRect_mem_and;	  /* drawRect      */
	TGEsys->_filledRect    = TGEsys->_filledRect_mem_and;  	  /* filledRect    */
	TGEsys->_clearGraphics = TGEsys->_clearGraphics_mem_and;  /* clearGraphics */
	TGEsys->_ellipse       = TGEsys->_ellipse_mem_and;	  /* ellipse       */
	TGEsys->_filledEllipse = TGEsys->_filledEllipse_mem_and;  /* filledEllipse */
	TGEsys->_circle        = TGEsys->_circle_mem_and;	  /* circle        */
	TGEsys->_filledCircle  = TGEsys->_filledCircle_mem_and;	  /* filledCircle  */
	TGEsys->_fillRegion    = TGEsys->_fillRegion_mem_and;	  /* fillRegion    */
	TGEsys->_fillLine      = TGEsys->_fillLine_mem_and;	  /* fillLine      */
	break;
      case TGE_NOT_PUT:		/* output using NOT */
	TGEsys->_putImage      = TGEsys->_putImage_mem_not;	  /* putImage      */
	TGEsys->_putImageInv   = TGEsys->_putImageInv_mem_not;	  /* putImageInv   */
	TGEsys->_putLine       = TGEsys->_putLine_mem_not;	  /* putLine       */
	TGEsys->_putLineInv    = TGEsys->_putLineInv_mem_not;	  /* putLineInv    */
	TGEsys->_putPixel      = TGEsys->_putPixel_mem_not;	  /* putPixel      */
	TGEsys->_line          = TGEsys->_line_mem_not;		  /* line          */
	TGEsys->_horizLine     = TGEsys->_horizLine_mem_not;      /* horizLine     */
	TGEsys->_vertLine      = TGEsys->_vertLine_mem_not;   	  /* vertLine      */
	TGEsys->_drawRect      = TGEsys->_drawRect_mem_not;	  /* drawRect      */
	TGEsys->_filledRect    = TGEsys->_filledRect_mem_not;  	  /* filledRect    */
	TGEsys->_clearGraphics = TGEsys->_clearGraphics_mem_not;  /* clearGraphics */
	TGEsys->_ellipse       = TGEsys->_ellipse_mem_not;	  /* ellipse       */
	TGEsys->_filledEllipse = TGEsys->_filledEllipse_mem_not;  /* filledEllipse */
	TGEsys->_circle        = TGEsys->_circle_mem_not;	  /* circle        */
	TGEsys->_filledCircle  = TGEsys->_filledCircle_mem_not;	  /* filledCircle  */
	TGEsys->_fillRegion    = TGEsys->_fillRegion_mem_not;	  /* fillRegion    */
	TGEsys->_fillLine      = TGEsys->_fillLine_mem_not;	  /* fillLine      */
	break;
      case TGE_OR_PUT:		/* output using OR */
	TGEsys->_putImage      = TGEsys->_putImage_mem_or;	  /* putImage      */
	TGEsys->_putImageInv   = TGEsys->_putImageInv_mem_or;	  /* putImageInv   */
	TGEsys->_putLine       = TGEsys->_putLine_mem_or;	  /* putLine       */
	TGEsys->_putLineInv    = TGEsys->_putLineInv_mem_or;	  /* putLineInv    */
	TGEsys->_putPixel      = TGEsys->_putPixel_mem_or;	  /* putPixel      */
	TGEsys->_line          = TGEsys->_line_mem_or;		  /* line          */
	TGEsys->_horizLine     = TGEsys->_horizLine_mem_or;       /* horizLine     */
	TGEsys->_vertLine      = TGEsys->_vertLine_mem_or;   	  /* vertLine      */
	TGEsys->_drawRect      = TGEsys->_drawRect_mem_or;	  /* drawRect      */
	TGEsys->_filledRect    = TGEsys->_filledRect_mem_or;  	  /* filledRect    */
	TGEsys->_clearGraphics = TGEsys->_clearGraphics_mem_or;	  /* clearGraphics */
	TGEsys->_ellipse       = TGEsys->_ellipse_mem_or;	  /* ellipse       */
	TGEsys->_filledEllipse = TGEsys->_filledEllipse_mem_or;	  /* filledEllipse */
	TGEsys->_circle        = TGEsys->_circle_mem_or;	  /* circle        */
	TGEsys->_filledCircle  = TGEsys->_filledCircle_mem_or;	  /* filledCircle  */
	TGEsys->_fillRegion    = TGEsys->_fillRegion_mem_or;	  /* fillRegion    */
	TGEsys->_fillLine      = TGEsys->_fillLine_mem_or;	  /* fillLine      */
	break;
      case TGE_XOR_PUT:		/* output using XOR */
	TGEsys->_putImage      = TGEsys->_putImage_mem_xor;	  /* putImage      */
	TGEsys->_putImageInv   = TGEsys->_putImageInv_mem_xor;	  /* putImageInv   */
	TGEsys->_putLine       = TGEsys->_putLine_mem_xor;	  /* putLine       */
	TGEsys->_putLineInv    = TGEsys->_putLineInv_mem_xor;	  /* putLineInv    */
	TGEsys->_putPixel      = TGEsys->_putPixel_mem_xor;	  /* putPixel      */
	TGEsys->_line          = TGEsys->_line_mem_xor;		  /* line          */
	TGEsys->_horizLine     = TGEsys->_horizLine_mem_xor;      /* horizLine     */
	TGEsys->_vertLine      = TGEsys->_vertLine_mem_xor;   	  /* vertLine      */
	TGEsys->_drawRect      = TGEsys->_drawRect_mem_xor;	  /* drawRect      */
	TGEsys->_filledRect    = TGEsys->_filledRect_mem_xor;  	  /* filledRect    */
	TGEsys->_clearGraphics = TGEsys->_clearGraphics_mem_xor;  /* clearGraphics */
	TGEsys->_ellipse       = TGEsys->_ellipse_mem_xor;	  /* ellipse       */
	TGEsys->_filledEllipse = TGEsys->_filledEllipse_mem_xor;  /* filledEllipse */
	TGEsys->_circle        = TGEsys->_circle_mem_xor;	  /* circle        */
	TGEsys->_filledCircle  = TGEsys->_filledCircle_mem_xor;	  /* filledCircle  */
	TGEsys->_fillRegion    = TGEsys->_fillRegion_mem_xor;	  /* fillRegion    */
	TGEsys->_fillLine      = TGEsys->_fillLine_mem_xor;	  /* fillLine      */
	break;
      default:
	;
    }
  }
  else				/* set real screen active */
  {
    switch (writeMode)
    {
      case TGE_COPY_PUT:	/* output using COPY */
	TGEsys->_putImage      = TGEsys->_putImage_scr_copy;	  /* putImage      */
	TGEsys->_putImageInv   = TGEsys->_putImageInv_scr_copy;	  /* putImageInv   */
	TGEsys->_putLine       = TGEsys->_putLine_scr_copy;	  /* putLine       */
	TGEsys->_putLineInv    = TGEsys->_putLineInv_scr_copy;	  /* putLineInv    */
	TGEsys->_putPixel      = TGEsys->_putPixel_scr_copy;	  /* putPixel      */
	TGEsys->_line          = TGEsys->_line_scr_copy;	  /* line          */
	TGEsys->_horizLine     = TGEsys->_horizLine_scr_copy;     /* horizLine     */
	TGEsys->_vertLine      = TGEsys->_vertLine_scr_copy;   	  /* vertLine      */
	TGEsys->_drawRect      = TGEsys->_drawRect_scr_copy;	  /* drawRect      */
	TGEsys->_filledRect    = TGEsys->_filledRect_scr_copy;    /* filledRect    */
	TGEsys->_clearGraphics = TGEsys->_clearGraphics_scr_copy; /* clearGraphics */
	TGEsys->_ellipse       = TGEsys->_ellipse_scr_copy;	  /* ellipse       */
	TGEsys->_filledEllipse = TGEsys->_filledEllipse_scr_copy; /* filledEllipse */
	TGEsys->_circle        = TGEsys->_circle_scr_copy;	  /* circle        */
	TGEsys->_filledCircle  = TGEsys->_filledCircle_scr_copy;  /* filledCircle  */
	TGEsys->_fillRegion    = TGEsys->_fillRegion_scr_copy;	  /* fillRegion    */
	TGEsys->_fillLine      = TGEsys->_fillLine_scr_copy;	  /* fillLine      */
	break;
      case TGE_AND_PUT:		/* output using AND */
	TGEsys->_putImage      = TGEsys->_putImage_scr_and;	  /* putImage      */
	TGEsys->_putImageInv   = TGEsys->_putImageInv_scr_and;	  /* putImageInv   */
	TGEsys->_putLine       = TGEsys->_putLine_scr_and;	  /* putLine       */
	TGEsys->_putLineInv    = TGEsys->_putLineInv_scr_and;	  /* putLineInv    */
	TGEsys->_putPixel      = TGEsys->_putPixel_scr_and;	  /* putPixel      */
	TGEsys->_line          = TGEsys->_line_scr_and;		  /* line          */
	TGEsys->_horizLine     = TGEsys->_horizLine_scr_and;      /* horizLine     */
	TGEsys->_vertLine      = TGEsys->_vertLine_scr_and;   	  /* vertLine      */
	TGEsys->_drawRect      = TGEsys->_drawRect_scr_and;	  /* drawRect      */
	TGEsys->_filledRect    = TGEsys->_filledRect_scr_and;  	  /* filledRect    */
	TGEsys->_clearGraphics = TGEsys->_clearGraphics_scr_and;  /* clearGraphics */
	TGEsys->_ellipse       = TGEsys->_ellipse_scr_and;	  /* ellipse       */
	TGEsys->_filledEllipse = TGEsys->_filledEllipse_scr_and;  /* filledEllipse */
	TGEsys->_circle        = TGEsys->_circle_scr_and;	  /* circle        */
	TGEsys->_filledCircle  = TGEsys->_filledCircle_scr_and;	  /* filledCircle  */
	TGEsys->_fillRegion    = TGEsys->_fillRegion_scr_and;	  /* fillRegion    */
	TGEsys->_fillLine      = TGEsys->_fillLine_scr_and;	  /* fillLine      */
	break;
      case TGE_NOT_PUT:		/* output using NOT */
	TGEsys->_putImage      = TGEsys->_putImage_scr_not;	  /* putImage      */
	TGEsys->_putImageInv   = TGEsys->_putImageInv_scr_not;	  /* putImageInv   */
	TGEsys->_putLine       = TGEsys->_putLine_scr_not;	  /* putLine       */
	TGEsys->_putLineInv    = TGEsys->_putLineInv_scr_not;	  /* putLineInv    */
	TGEsys->_putPixel      = TGEsys->_putPixel_scr_not;	  /* putPixel      */
	TGEsys->_line          = TGEsys->_line_scr_not;		  /* line          */
	TGEsys->_horizLine     = TGEsys->_horizLine_scr_not;      /* horizLine     */
	TGEsys->_vertLine      = TGEsys->_vertLine_scr_not;   	  /* vertLine      */
	TGEsys->_drawRect      = TGEsys->_drawRect_scr_not;	  /* drawRect      */
	TGEsys->_filledRect    = TGEsys->_filledRect_scr_not;  	  /* filledRect    */
	TGEsys->_clearGraphics = TGEsys->_clearGraphics_scr_not;  /* clearGraphics */
	TGEsys->_ellipse       = TGEsys->_ellipse_scr_not;	  /* ellipse       */
	TGEsys->_filledEllipse = TGEsys->_filledEllipse_scr_not;  /* TGEsys->_filledEllipse */
	TGEsys->_circle        = TGEsys->_circle_scr_not;	  /* circle        */
	TGEsys->_filledCircle  = TGEsys->_filledCircle_scr_not;	  /* filledCircle  */
	TGEsys->_fillRegion    = TGEsys->_fillRegion_scr_not;	  /* fillRegion    */
	TGEsys->_fillLine      = TGEsys->_fillLine_scr_not;	  /* fillLine      */
	break;
      case TGE_OR_PUT:		/* output using OR */
	TGEsys->_putImage      = TGEsys->_putImage_scr_or;	  /* putImage      */
	TGEsys->_putImageInv   = TGEsys->_putImageInv_scr_or;	  /* putImageInv   */
	TGEsys->_putLine       = TGEsys->_putLine_scr_or;	  /* putLine       */
	TGEsys->_putLineInv    = TGEsys->_putLineInv_scr_or;	  /* putLineInv    */
	TGEsys->_putPixel      = TGEsys->_putPixel_scr_or;	  /* putPixel      */
	TGEsys->_line          = TGEsys->_line_scr_or;		  /* line          */
	TGEsys->_horizLine     = TGEsys->_horizLine_scr_or;       /* horizLine     */
	TGEsys->_vertLine      = TGEsys->_vertLine_scr_or;   	  /* vertLine      */
	TGEsys->_drawRect      = TGEsys->_drawRect_scr_or;	  /* drawRect      */
	TGEsys->_filledRect    = TGEsys->_filledRect_scr_or;  	  /* filledRect    */
	TGEsys->_clearGraphics = TGEsys->_clearGraphics_scr_or;	  /* clearGraphics */
	TGEsys->_ellipse       = TGEsys->_ellipse_scr_or;	  /* ellipse       */
	TGEsys->_filledEllipse = TGEsys->_filledEllipse_scr_or;	  /* filledEllipse */
	TGEsys->_circle        = TGEsys->_circle_scr_or;	  /* circle        */
	TGEsys->_filledCircle  = TGEsys->_filledCircle_scr_or;	  /* filledCircle  */
	TGEsys->_fillRegion    = TGEsys->_fillRegion_scr_or;	  /* fillRegion    */
	TGEsys->_fillLine      = TGEsys->_fillLine_scr_or;	  /* fillLine      */
	break;
      case TGE_XOR_PUT:		/* output using XOR */
	TGEsys->_putImage      = TGEsys->_putImage_scr_xor;	  /* putImage      */
	TGEsys->_putImageInv   = TGEsys->_putImageInv_scr_xor;	  /* putImageInv   */
	TGEsys->_putLine       = TGEsys->_putLine_scr_xor;	  /* putLine       */
	TGEsys->_putLineInv    = TGEsys->_putLineInv_scr_xor;	  /* putLineInv    */
	TGEsys->_putPixel      = TGEsys->_putPixel_scr_xor;	  /* putPixel      */
	TGEsys->_line          = TGEsys->_line_scr_xor;		  /* line          */
	TGEsys->_horizLine     = TGEsys->_horizLine_scr_xor;      /* horizLine     */
	TGEsys->_vertLine      = TGEsys->_vertLine_scr_xor;   	  /* vertLine      */
	TGEsys->_drawRect      = TGEsys->_drawRect_scr_xor;	  /* drawRect      */
	TGEsys->_filledRect    = TGEsys->_filledRect_scr_xor;  	  /* filledRect    */
	TGEsys->_clearGraphics = TGEsys->_clearGraphics_scr_xor;  /* clearGraphics */
	TGEsys->_ellipse       = TGEsys->_ellipse_scr_xor;	  /* ellipse       */
	TGEsys->_filledEllipse = TGEsys->_filledEllipse_scr_xor;  /* filledEllipse */
	TGEsys->_circle        = TGEsys->_circle_scr_xor;	  /* circle        */
	TGEsys->_filledCircle  = TGEsys->_filledCircle_scr_xor;	  /* filledCircle  */
	TGEsys->_fillRegion    = TGEsys->_fillRegion_scr_xor;	  /* fillRegion    */
	TGEsys->_fillLine      = TGEsys->_fillLine_scr_xor;	  /* fillLine      */
	break;
      default:
	;
    }
  }
}