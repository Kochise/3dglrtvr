/*****************************************************************************
*       The Graphics Engine version 1.29áD                                   *
*                                                                            *
*       The Graphics Engine code and documentation are Copyright (c) 1993    *
*       by Matthew Hildebrand.                                               *
*                                                                            *
*       Unauthorised usage or modification of any or all of The Graphics     *
*       Engine is strictly prohibited.                                       *
*****************************************************************************/

#if !defined (TGEdotH)
#define TGEdotH


/* The basic structure used to store function pointers and mode data */

struct GraphDrv {
  /* Non-I/O functions */
  int far (*_initGraphics)(void);
  void far (*_deInitGraphics)(void);
  void far (*_setPaletteReg)(unsigned palReg, unsigned char red, unsigned char green, unsigned char blue);
  void far (*_getPaletteReg)(unsigned palReg, unsigned char far *red, unsigned char far *green, unsigned char far *blue);
  void far (*_setBlockPalette)(unsigned firstReg, unsigned lastReg, void far *data);
  void far (*_getBlockPalette)(unsigned firstReg, unsigned lastReg, void far *data);
  unsigned far (*_colourCloseTo)(unsigned char red, unsigned char green, unsigned char blue);
  unsigned far (*_colourCloseToX)(unsigned char red, unsigned char green, unsigned char blue, unsigned colourExclude);
  unsigned long far (*_imageSize)(int ulx, int uly, int lrx, int lry);
  unsigned long far (*_imageSizeDim)(unsigned wide, unsigned deep);
  void far (*_setPage)(unsigned pageNum);	/* not implemented yet */

  /* Currently active I/O functions */
  void huge (*_getImage)(int ulx, int uly, int lrx, int lry, void far *image);
  void far (*_getLine)(int lineNum, int xOff, int lineLen, void far *buf);
  unsigned far (*_getPixel)(int x, int y);
  void huge (*_putImage)(int x, int y, void far *image);
  void huge (*_putImageInv)(int x, int y, void far *image);
  void far (*_putLine)(int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLineInv)(int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putPixel)(int x, int y, unsigned colour);
  void far (*_line)(int x1, int y1, int x2, int y2, unsigned colour);
  void far (*_horizLine)(int y, int x1, int x2, unsigned colour);
  void far (*_vertLine)(int x, int y1, int y2, unsigned colour);
  void far (*_drawRect)(int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_filledRect)(int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_clearGraphics)(unsigned colour);
  void far (*_ellipse)(int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_filledEllipse)(int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_circle)(int x, int y, int radius, unsigned colour);
  void far (*_filledCircle)(int x, int y, int radius, unsigned colour);
  void far (*_fillRegion)(int x, int y, unsigned colour);
  void far (*_fillLine)(int y, int x1, int x2, unsigned colour);

  /* Input functions */
  void huge (*_getImage_scr)(int ulx, int uly, int lrx, int lry, void far *image);
  void huge (*_getImage_mem)(int ulx, int uly, int lrx, int lry, void far *image);
  void far (*_getLine_scr)(int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_getLine_mem)(int lineNum, int xOff, int lineLen, void far *buf);
  unsigned far (*_getPixel_scr)(int x, int y);
  unsigned far (*_getPixel_mem)(int x, int y);

  /* Output functions */
  void huge (*_putImage_scr_copy)(int x, int y, void far *image);
  void huge (*_putImage_scr_and) (int x, int y, void far *image);
  void huge (*_putImage_scr_not) (int x, int y, void far *image);
  void huge (*_putImage_scr_or)  (int x, int y, void far *image);
  void huge (*_putImage_scr_xor) (int x, int y, void far *image);
  void huge (*_putImage_mem_copy)(int x, int y, void far *image);
  void huge (*_putImage_mem_and) (int x, int y, void far *image);
  void huge (*_putImage_mem_not) (int x, int y, void far *image);
  void huge (*_putImage_mem_or)  (int x, int y, void far *image);
  void huge (*_putImage_mem_xor) (int x, int y, void far *image);
  void huge (*_putImageInv_scr_copy)(int x, int y, void far *image);
  void huge (*_putImageInv_scr_and) (int x, int y, void far *image);
  void huge (*_putImageInv_scr_not) (int x, int y, void far *image);
  void huge (*_putImageInv_scr_or)  (int x, int y, void far *image);
  void huge (*_putImageInv_scr_xor) (int x, int y, void far *image);
  void huge (*_putImageInv_mem_copy)(int x, int y, void far *image);
  void huge (*_putImageInv_mem_and) (int x, int y, void far *image);
  void huge (*_putImageInv_mem_not) (int x, int y, void far *image);
  void huge (*_putImageInv_mem_or)  (int x, int y, void far *image);
  void huge (*_putImageInv_mem_xor) (int x, int y, void far *image);
  void far (*_putLine_scr_copy)(int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLine_scr_and) (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLine_scr_not) (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLine_scr_or)  (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLine_scr_xor) (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLine_mem_copy)(int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLine_mem_and) (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLine_mem_not) (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLine_mem_or)  (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLine_mem_xor) (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLineInv_scr_copy)(int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLineInv_scr_and) (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLineInv_scr_not) (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLineInv_scr_or)  (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLineInv_scr_xor) (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLineInv_mem_copy)(int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLineInv_mem_and) (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLineInv_mem_not) (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLineInv_mem_or)  (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putLineInv_mem_xor) (int lineNum, int xOff, int lineLen, void far *buf);
  void far (*_putPixel_scr_copy)(int x, int y, unsigned colour);
  void far (*_putPixel_scr_and) (int x, int y, unsigned colour);
  void far (*_putPixel_scr_not) (int x, int y, unsigned colour);
  void far (*_putPixel_scr_or)  (int x, int y, unsigned colour);
  void far (*_putPixel_scr_xor) (int x, int y, unsigned colour);
  void far (*_putPixel_mem_copy)(int x, int y, unsigned colour);
  void far (*_putPixel_mem_and) (int x, int y, unsigned colour);
  void far (*_putPixel_mem_not) (int x, int y, unsigned colour);
  void far (*_putPixel_mem_or)  (int x, int y, unsigned colour);
  void far (*_putPixel_mem_xor) (int x, int y, unsigned colour);
  void far (*_line_scr_copy)(int x1, int y1, int x2, int y2, unsigned colour);
  void far (*_line_scr_and) (int x1, int y1, int x2, int y2, unsigned colour);
  void far (*_line_scr_not) (int x1, int y1, int x2, int y2, unsigned colour);
  void far (*_line_scr_or)  (int x1, int y1, int x2, int y2, unsigned colour);
  void far (*_line_scr_xor) (int x1, int y1, int x2, int y2, unsigned colour);
  void far (*_line_mem_copy)(int x1, int y1, int x2, int y2, unsigned colour);
  void far (*_line_mem_and) (int x1, int y1, int x2, int y2, unsigned colour);
  void far (*_line_mem_not) (int x1, int y1, int x2, int y2, unsigned colour);
  void far (*_line_mem_or)  (int x1, int y1, int x2, int y2, unsigned colour);
  void far (*_line_mem_xor) (int x1, int y1, int x2, int y2, unsigned colour);
  void far (*_horizLine_scr_copy)(int y, int x1, int x2, unsigned colour);
  void far (*_horizLine_scr_and) (int y, int x1, int x2, unsigned colour);
  void far (*_horizLine_scr_not) (int y, int x1, int x2, unsigned colour);
  void far (*_horizLine_scr_or)  (int y, int x1, int x2, unsigned colour);
  void far (*_horizLine_scr_xor) (int y, int x1, int x2, unsigned colour);
  void far (*_horizLine_mem_copy)(int y, int x1, int x2, unsigned colour);
  void far (*_horizLine_mem_and) (int y, int x1, int x2, unsigned colour);
  void far (*_horizLine_mem_not) (int y, int x1, int x2, unsigned colour);
  void far (*_horizLine_mem_or)  (int y, int x1, int x2, unsigned colour);
  void far (*_horizLine_mem_xor) (int y, int x1, int x2, unsigned colour);
  void far (*_vertLine_scr_copy)(int x, int y1, int y2, unsigned colour);
  void far (*_vertLine_scr_and) (int x, int y1, int y2, unsigned colour);
  void far (*_vertLine_scr_not) (int x, int y1, int y2, unsigned colour);
  void far (*_vertLine_scr_or)  (int x, int y1, int y2, unsigned colour);
  void far (*_vertLine_scr_xor) (int x, int y1, int y2, unsigned colour);
  void far (*_vertLine_mem_copy)(int x, int y1, int y2, unsigned colour);
  void far (*_vertLine_mem_and) (int x, int y1, int y2, unsigned colour);
  void far (*_vertLine_mem_not) (int x, int y1, int y2, unsigned colour);
  void far (*_vertLine_mem_or)  (int x, int y1, int y2, unsigned colour);
  void far (*_vertLine_mem_xor)(int x, int y1, int y2, unsigned colour);
  void far (*_drawRect_scr_copy)(int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_drawRect_scr_and) (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_drawRect_scr_not) (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_drawRect_scr_or)  (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_drawRect_scr_xor) (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_drawRect_mem_copy)(int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_drawRect_mem_and) (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_drawRect_mem_not) (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_drawRect_mem_or)  (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_drawRect_mem_xor) (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_filledRect_scr_copy)(int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_filledRect_scr_and) (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_filledRect_scr_not) (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_filledRect_scr_or)  (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_filledRect_scr_xor) (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_filledRect_mem_copy)(int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_filledRect_mem_and) (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_filledRect_mem_not) (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_filledRect_mem_or)  (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_filledRect_mem_xor) (int ulx, int uly, int lrx, int lry, unsigned colour);
  void far (*_clearGraphics_scr_copy)(unsigned colour);
  void far (*_clearGraphics_scr_and) (unsigned colour);
  void far (*_clearGraphics_scr_not) (unsigned colour);
  void far (*_clearGraphics_scr_or)  (unsigned colour);
  void far (*_clearGraphics_scr_xor) (unsigned colour);
  void far (*_clearGraphics_mem_copy)(unsigned colour);
  void far (*_clearGraphics_mem_and) (unsigned colour);
  void far (*_clearGraphics_mem_not) (unsigned colour);
  void far (*_clearGraphics_mem_or)  (unsigned colour);
  void far (*_clearGraphics_mem_xor) (unsigned colour);
  void far (*_ellipse_scr_copy)(int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_ellipse_scr_and) (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_ellipse_scr_not) (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_ellipse_scr_or)  (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_ellipse_scr_xor) (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_ellipse_mem_copy)(int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_ellipse_mem_and) (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_ellipse_mem_not) (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_ellipse_mem_or)  (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_ellipse_mem_xor) (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_filledEllipse_scr_copy)(int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_filledEllipse_scr_and) (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_filledEllipse_scr_not) (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_filledEllipse_scr_or)  (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_filledEllipse_scr_xor) (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_filledEllipse_mem_copy)(int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_filledEllipse_mem_and) (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_filledEllipse_mem_not) (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_filledEllipse_mem_or)  (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_filledEllipse_mem_xor) (int xc, int yc, int wide, int deep, unsigned colour);
  void far (*_circle_scr_copy)(int x, int y, int radius, unsigned colour);
  void far (*_circle_scr_and) (int x, int y, int radius, unsigned colour);
  void far (*_circle_scr_not) (int x, int y, int radius, unsigned colour);
  void far (*_circle_scr_or)  (int x, int y, int radius, unsigned colour);
  void far (*_circle_scr_xor) (int x, int y, int radius, unsigned colour);
  void far (*_circle_mem_copy)(int x, int y, int radius, unsigned colour);
  void far (*_circle_mem_and) (int x, int y, int radius, unsigned colour);
  void far (*_circle_mem_not) (int x, int y, int radius, unsigned colour);
  void far (*_circle_mem_or)  (int x, int y, int radius, unsigned colour);
  void far (*_circle_mem_xor) (int x, int y, int radius, unsigned colour);
  void far (*_filledCircle_scr_copy)(int x, int y, int radius, unsigned colour);
  void far (*_filledCircle_scr_and) (int x, int y, int radius, unsigned colour);
  void far (*_filledCircle_scr_not) (int x, int y, int radius, unsigned colour);
  void far (*_filledCircle_scr_or)  (int x, int y, int radius, unsigned colour);
  void far (*_filledCircle_scr_xor) (int x, int y, int radius, unsigned colour);
  void far (*_filledCircle_mem_copy)(int x, int y, int radius, unsigned colour);
  void far (*_filledCircle_mem_and) (int x, int y, int radius, unsigned colour);
  void far (*_filledCircle_mem_not) (int x, int y, int radius, unsigned colour);
  void far (*_filledCircle_mem_or)  (int x, int y, int radius, unsigned colour);
  void far (*_filledCircle_mem_xor) (int x, int y, int radius, unsigned colour);
  void far (*_fillRegion_scr_copy)(int x, int y, unsigned colour);
  void far (*_fillRegion_scr_and) (int x, int y, unsigned colour);
  void far (*_fillRegion_scr_not) (int x, int y, unsigned colour);
  void far (*_fillRegion_scr_or)  (int x, int y, unsigned colour);
  void far (*_fillRegion_scr_xor) (int x, int y, unsigned colour);
  void far (*_fillRegion_mem_copy)(int x, int y, unsigned colour);
  void far (*_fillRegion_mem_and) (int x, int y, unsigned colour);
  void far (*_fillRegion_mem_not) (int x, int y, unsigned colour);
  void far (*_fillRegion_mem_or)  (int x, int y, unsigned colour);
  void far (*_fillRegion_mem_xor) (int x, int y, unsigned colour);
  void far (*_fillLine_scr_copy)(int y, int x1, int x2, unsigned colour);
  void far (*_fillLine_scr_and) (int y, int x1, int x2, unsigned colour);
  void far (*_fillLine_scr_not) (int y, int x1, int x2, unsigned colour);
  void far (*_fillLine_scr_or)  (int y, int x1, int x2, unsigned colour);
  void far (*_fillLine_scr_xor) (int y, int x1, int x2, unsigned colour);
  void far (*_fillLine_mem_copy)(int y, int x1, int x2, unsigned colour);
  void far (*_fillLine_mem_and) (int y, int x1, int x2, unsigned colour);
  void far (*_fillLine_mem_not) (int y, int x1, int x2, unsigned colour);
  void far (*_fillLine_mem_or)  (int y, int x1, int x2, unsigned colour);
  void far (*_fillLine_mem_xor) (int y, int x1, int x2, unsigned colour);

  /* Driver data */
  int scrnMaxX, scrnMaxY;		/* physical screen dimensions */
  unsigned maxColour;			/* maximum colour number */
  unsigned xRatio, yRatio;		/* screen aspect ratio */
  unsigned bitsPerPixel;		/* bits per pixel */
  int inMaxX, inMaxY;			/* input screen dimensions */
  int outMaxX, outMaxY;			/* output screen dimensions */
  unsigned inScreenWide, outScreenWide;	/* needed for virtual screens */
  int inViewportULX, inViewportULY;	/* input viewport coords */
  int inViewportLRX, inViewportLRY;
  int outViewportULX, outViewportULY;	/* output viewport coords */
  int outViewportLRX, outViewportLRY;
  unsigned pagingSupported;		/* not implemented yet */
  unsigned curPage, maxPage;		/* not implemented yet */
  unsigned imageWideAdjust;		/* not implemented yet */
  void far *inAddr;			/* current input address */
  void far *outAddr;			/* current output address */
  void far *scrAddr;			/* screen address */
};


/* Note that this C/C++ bit might not work properly with your compiler.  If
   you are using C++ and your linker reports that these functions are not
   found, try explicitly forcing the extern "C" syntax by removing these
   preprocessor directives. */

#ifdef __cplusplus
extern "C" {
#endif

/* load/unload routines */
int TGE_loadGraphDriver(char *filename);
void TGE_unloadGraphDriver(void);

/* virtual/real screen routines */
void TGE_setGraphicsInputAddr(void far *addr);
void TGE_setGraphicsOutputAddr(void far *addr);
void TGE_setGraphicsAddr(void far *addr);
void far *TGE_getGraphicsInputAddr(void);
void far *TGE_getGraphicsOutputAddr(void);
void far *TGE_makeVirtScreen(unsigned wide, unsigned deep);

/* output mode control routine */
void TGE_setOutputMode(int mode);

/* viewport routines */
void TGE_setInputViewport(int ulx, int uly, int lrx, int lry);
void TGE_setOutputViewport(int ulx, int uly, int lrx, int lry);
void TGE_setViewports(int ulx, int uly, int lrx, int lry);
void TGE_getInputViewport(int far *ulx, int far *uly, int far *lrx, int far *lry);
void TGE_getOutputViewport(int far *ulx, int far *uly, int far *lrx, int far *lry);

/* clipping routines */
int TGE_clipLine(int *x1, int *y1, int *x2, int *y2);
int TGE_clipFilledRect(int *ulx, int *uly, int *lrx, int *lry);

/* scale bitmap */
void scaleBitmap(void *srcImage, unsigned newWide, unsigned newDeep,
	void *destImage);

#ifdef __cplusplus
};
#endif


/* Declare the structure used by the system so that programs can access it. */

extern struct GraphDrv far *TGEsys;


/* The function names of TGE's functions are defined here.  Since they
   are really just macros pretending to be functions, you can easily
   change their names here if you don't like the default ones. */

#define initGraphics		(TGEsys->_initGraphics)
#define deInitGraphics		(TGEsys->_deInitGraphics)
#define putImage		(TGEsys->_putImage)
#define putImageInv		(TGEsys->_putImageInv)
#define getImage		(TGEsys->_getImage)
#define putLine			(TGEsys->_putLine)
#define putLineInv		(TGEsys->_putLineInv)
#define getLine			(TGEsys->_getLine)
#define imageSize		(TGEsys->_imageSize)
#define imageSizeDim		(TGEsys->_imageSizeDim)
#define putPixel		(TGEsys->_putPixel)
#define getPixel		(TGEsys->_getPixel)
#define line			(TGEsys->_line)
#define horizLine		(TGEsys->_horizLine)
#define vertLine		(TGEsys->_vertLine)
#define drawRect		(TGEsys->_drawRect)
#define filledRect		(TGEsys->_filledRect)
#define setPaletteReg		(TGEsys->_setPaletteReg)
#define getPaletteReg		(TGEsys->_getPaletteReg)
#define setBlockPalette		(TGEsys->_setBlockPalette)
#define getBlockPalette		(TGEsys->_getBlockPalette)
#define clearGraphics		(TGEsys->_clearGraphics)
#define ellipse			(TGEsys->_ellipse)
#define filledEllipse		(TGEsys->_filledEllipse)
#define circle			(TGEsys->_circle)
#define filledCircle		(TGEsys->_filledCircle)
#define fillRegion		(TGEsys->_fillRegion)
#define colourCloseTo		(TGEsys->_colourCloseTo)
#define colourCloseToX		(TGEsys->_colourCloseToX)
#define fillLine		(TGEsys->_fillLine)

#define loadGraphDriver		TGE_loadGraphDriver
#define unloadGraphDriver	TGE_unloadGraphDriver

#define setGraphicsOutputAddr	TGE_setGraphicsOutputAddr /* set outputs */
#define setGraphicsInputAddr	TGE_setGraphicsInputAddr  /* set input   */
#define setGraphicsAddr		TGE_setGraphicsAddr	  /* set I/O     */
#define makeVirtScreen		TGE_makeVirtScreen	  /* make virtual screen */
#define getGraphicsOutputAddr	TGE_getGraphicsOutputAddr /* set output  */
#define getGraphicsInputAddr	TGE_getGraphicsInputAddr  /* set input   */

#define setGraphicsOutputMode   TGE_setOutputMode         /* output mode */

#define setOutputViewport	TGE_setOutputViewport	  /* viewports */
#define setInputViewport       	TGE_setInputViewport
#define setViewports            TGE_setViewports
#define setViewport		TGE_setViewports
#define getOutputViewport	TGE_getOutputViewport
#define getInputViewport      	TGE_getInputViewport
#define getViewport		TGE_getOutputViewport

#define clipLine		TGE_clipLine		  /* clipping */
#define clipFilledRect		TGE_clipFilledRect
#define clipInputPoint(x,y)     TGE_clipInputPoint(x,y)
#define clipOutputPoint(x,y)    TGE_clipOutputPoint(x,y)
#define clipPoint(x,y)          TGE_clipPoint(x,y)
#define pointOnScreen(x,y)      TGE_pointOnScreen(x,y)


/* These macros provide easy access to important mode information in the
   TGE system structure. */

#define INMAXX			(TGEsys->inMaxX)
#define INMAXY			(TGEsys->inMaxY)
#define OUTMAXX			(TGEsys->outMaxX)
#define OUTMAXY			(TGEsys->outMaxY)
#define MAXCOLOUR		(TGEsys->maxColour)
#define XRATIO			(TGEsys->xRatio)
#define YRATIO			(TGEsys->yRatio)
#define INVIEWPORTULX		(TGEsys->inViewportULX)
#define INVIEWPORTULY		(TGEsys->inViewportULY)
#define INVIEWPORTLRX		(TGEsys->inViewportLRX)
#define INVIEWPORTLRY		(TGEsys->inViewportLRY)
#define OUTVIEWPORTULX		(TGEsys->outViewportULX)
#define OUTVIEWPORTULY		(TGEsys->outViewportULY)
#define OUTVIEWPORTLRX		(TGEsys->outViewportLRX)
#define OUTVIEWPORTLRY		(TGEsys->outViewportLRY)
#define SCREENMAXX		(TGEsys->scrnMaxX)
#define SCREENMAXY		(TGEsys->scrnMaxY)
#define MAXX			(TGEsys->outMaxX)
#define MAXY			(TGEsys->outMaxY)
#define VIEWPORTULX		(TGEsys->outViewportULX)
#define VIEWPORTULY		(TGEsys->outViewportULY)
#define VIEWPORTLRX		(TGEsys->outViewportLRX)
#define VIEWPORTLRY		(TGEsys->outViewportLRY)


/* These macros are for the various output modes */

#define TGE_COPY_PUT	0
#define TGE_AND_PUT	1
#define TGE_NOT_PUT	2
#define TGE_OR_PUT	3
#define TGE_XOR_PUT	4


/* These macros are standard with Borland C/C++ compilers; this next bit
   defines them if you're using a non-Borland compiler. */

#if !defined(__TURBOC__) && !defined(__BORLANDC__)
  #define MK_FP(seg,ofs)	((void _seg *)(seg)+(void near *)(ofs))
  #define FP_SEG(fp)		((unsigned)(void _seg *)(void far *)(fp))
  #define FP_OFF(fp)		((unsigned)(fp))
#endif


/* These macros are the return codes from loadGraphDriver() */

#define TGE_SUCCESS	 0		/* driver loaded successfully */
#define TGE_OPEN_ERR	-1		/* error opening given file */
#define TGE_FORMAT_ERR	-2		/* file is not a TGE driver */
#define TGE_ALLOC_ERR	-3	    	/* error allocating memory */
#define TGE_FILE_ERR	-4		/* file I/O error */


/******
******* Point clipping routines
******/

#define TGE_pointOnScreen(x,y) \
  ((x>=0) && (x<=TGEsys->scrnMaxX) && (y>=0) && (y<=TGEsys->scrnMaxY))

#define TGE_clipInputPoint(x,y) \
  ((x>=INVIEWPORTULX) && (x<=INVIEWPORTLRX) && \
   (y>=INVIEWPORTULY) && (y<=INVIEWPORTLRY))
#define TGE_clipOutputPoint(x,y) \
  ((x>=OUTVIEWPORTULX) && (x<=OUTVIEWPORTLRX) && \
   (y>=OUTVIEWPORTULY) && (y<=OUTVIEWPORTLRY))
#define TGE_clipPoint(x,y) \
  TGE_clipOutputPoint(x,y)

#endif
