/*****************************************************************************
*       The Graphics Engine version 1.33                                     *
*                                                                            *
*       The Graphics Engine code and documentation are                       *
*       Copyright (c) 1993-1994 by Matthew Hildebrand; all rights reserved.  *
*                                                                            *
*       Unauthorised usage or modification of any or all of The Graphics     *
*       Engine is strictly prohibited.                                       *
*****************************************************************************/

#if !defined(TGEdotH)
#define TGEdotH



/* Comment out this line to disable output modes */
#define TGE_USE_OUTPUT_MODES
/* Comment out this line to disable virtual screens */
#define TGE_USE_VIRTUAL_SCREENS
/* Comment out this line to disable fillRegion() */
#define TGE_USE_REGION_FILLS
/* Comment out this line to disable colourCloseTo() and colourCloseToX() */
#define TGE_USE_COLOUR_APPROXIMATION
/* Comment out this line to disable ellipses and circles */
#define TGE_USE_ELLIPSES
/* Comment out this line to disable images (ie. bitmaps) */
#define TGE_USE_IMAGES
/* Comment out this line to disable putVertLine() and friends */
#define TGE_USE_PUTVERTLINE



/*
 * The basic structure used to store function pointers and mode data.
 */

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

  /* Currently active I/O functions */
  void huge (*_getImage)(int ulx, int uly, int lrx, int lry, void far *image);
  void far (*_getHorizLine)(int y, int xOff, int lineLen, void far *buf);
  void far (*_getVertLine)(int y, int xOff, int lineLen, void far *buf);
  unsigned far (*_getPixel)(int x, int y);
  void huge (*_putImage)(int x, int y, void far *image);
  void huge (*_putImageInv)(int x, int y, void far *image);
  void far (*_putHorizLine)(int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLineInv)(int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLine)(int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLineInv)(int y, int xOff, int lineLen, void far *buf);
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
  void far (*_getHorizLine_scr)(int y, int xOff, int lineLen, void far *buf);
  void far (*_getHorizLine_mem)(int y, int xOff, int lineLen, void far *buf);
  void far (*_getVertLine_scr)(int y, int xOff, int lineLen, void far *buf);
  void far (*_getVertLine_mem)(int y, int xOff, int lineLen, void far *buf);
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
  void far (*_putHorizLine_scr_copy)(int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLine_scr_and) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLine_scr_not) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLine_scr_or)  (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLine_scr_xor) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLine_mem_copy)(int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLine_mem_and) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLine_mem_not) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLine_mem_or)  (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLine_mem_xor) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLineInv_scr_copy)(int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLineInv_scr_and) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLineInv_scr_not) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLineInv_scr_or)  (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLineInv_scr_xor) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLineInv_mem_copy)(int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLineInv_mem_and) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLineInv_mem_not) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLineInv_mem_or)  (int y, int xOff, int lineLen, void far *buf);
  void far (*_putHorizLineInv_mem_xor) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLine_scr_copy)(int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLine_scr_and) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLine_scr_not) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLine_scr_or)  (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLine_scr_xor) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLine_mem_copy)(int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLine_mem_and) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLine_mem_not) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLine_mem_or)  (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLine_mem_xor) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLineInv_scr_copy)(int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLineInv_scr_and) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLineInv_scr_not) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLineInv_scr_or)  (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLineInv_scr_xor) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLineInv_mem_copy)(int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLineInv_mem_and) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLineInv_mem_not) (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLineInv_mem_or)  (int y, int xOff, int lineLen, void far *buf);
  void far (*_putVertLineInv_mem_xor) (int y, int xOff, int lineLen, void far *buf);
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
  short scrnMaxX, scrnMaxY;                 /* physical screen dimensions */
  unsigned short maxColour;                 /* maximum colour number */
  unsigned short xRatio, yRatio;            /* screen aspect ratio */
  unsigned short bitsPerPixel;              /* bits per pixel */
  short inMaxX, inMaxY;                     /* input screen dimensions */
  short outMaxX, outMaxY;                   /* output screen dimensions */
  unsigned short inScreenWide, outScreenWide; /* needed for virtual screens */
  short inViewportULX, inViewportULY;       /* input viewport coords */
  short inViewportLRX, inViewportLRY;
  short outViewportULX, outViewportULY;     /* output viewport coords */
  short outViewportLRX, outViewportLRY;
  void far *inAddr;                         /* current input address */
  void far *outAddr;                        /* current output address */
  void far *scrAddr;                        /* screen address */
};



/*
 * Note that this C/C++ bit might not work properly with your compiler.  If
 * you are using C++ and your linker reports that these functions are not
 * found, try explicitly forcing the extern "C" syntax by removing these
 * preprocessor directives.
 */

#if defined(__cplusplus)
extern "C" {
#endif

/* Load/unload routines */
int TGE_loadGraphDriver(char *filename);
void TGE_unloadGraphDriver(void);

/* Virtual/real screen routines */
#if defined(TGE_USE_VIRTUAL_SCREENS)
  void TGE_setGraphicsInputAddr(void far *addr);
  void TGE_setGraphicsOutputAddr(void far *addr);
  void TGE_setGraphicsAddr(void far *addr);
  void far *TGE_getGraphicsInputAddr(void);
  void far *TGE_getGraphicsOutputAddr(void);
  void far *TGE_makeVirtScreen(unsigned wide, unsigned deep);
#endif

/* Output mode control routine */
#if defined(TGE_USE_OUTPUT_MODES)
  void TGE_setOutputMode(int mode);
#endif

/* Viewport routines */
void TGE_setInputViewport(int ulx, int uly, int lrx, int lry);
void TGE_setOutputViewport(int ulx, int uly, int lrx, int lry);
void TGE_setViewports(int ulx, int uly, int lrx, int lry);
void TGE_getInputViewport(int *ulx, int *uly, int *lrx, int *lry);
void TGE_getOutputViewport(int *ulx, int *uly, int *lrx, int *lry);

/* Clipping routines */
int TGE_clipLine(int *x1, int *y1, int *x2, int *y2);
int TGE_clipRect(int *ulx, int *uly, int *lrx, int *lry);

/* Scale bitmap */
void TGE_scaleBitmap(void *srcImage, unsigned newWide, unsigned newDeep,
	void *destImage);

/* Palette routines */
int TGE_fadePalette(unsigned step, void *inPal, void *outPal, void *targetPal);
void TGE_greyPalette(void *inPal, void *outPal);
void TGE_rotatePalette(int howMuch, void *inPal, void *outPal);

/* RAW/PAL file routines */
void *TGE_loadRawFile(char *filename);
int TGE_displayRawFile(int x, int y, char *filename);
void *TGE_loadPalFile(char *filename, void *addr);
int TGE_saveRawFile(char *filename, void *image);
int TGE_savePalFile(char *filename, void *pal);

/* PCX file routines */
void *TGE_loadPcxFile(char *filename, void *palette);
int TGE_displayPcxFile(int x, int y, char *filename);
void *TGE_loadPcxFilePal(char *filename, void *addr);

/* Colour replacement routine */
void TGE_replaceColour(int ulx, int uly, int lrx, int lry, unsigned oldColour, unsigned newColour);

#if defined(__cplusplus)
};
#endif



/*
 * Declare the structure used by the system so that programs can access it.
 */

extern struct GraphDrv far *TGEsys;



/*
 * The function names of TGE's functions are defined here.  Since they are
 * really just macros pretending to be functions, you can easily change
 * their names here if you don't like the default ones.
 */

#define initGraphics		(TGEsys->_initGraphics)
#define deInitGraphics		(TGEsys->_deInitGraphics)
#if defined(TGE_USE_IMAGES)
  #define putImage              (TGEsys->_putImage)
  #define putImageInv           (TGEsys->_putImageInv)
  #define getImage              (TGEsys->_getImage)
#endif
#define getHorizLine            (TGEsys->_getHorizLine)
#define getLine                 getHorizLine
#define putHorizLine            (TGEsys->_putHorizLine)
#define putLine                 putHorizLine
#define putHorizLineInv         (TGEsys->_putHorizLineInv)
#define putLineInv              putHorizLineInv
#if defined(TGE_USE_PUTVERTLINE)
  #define getVertLine           (TGEsys->_getVertLine)
  #define putVertLine           (TGEsys->_putVertLine)
  #define putVertLineInv        (TGEsys->_putVertLineInv)
#endif
#if defined(TGE_USE_IMAGES)
  #define imageSize             (TGEsys->_imageSize)
  #define imageSizeDim          (TGEsys->_imageSizeDim)
#endif
#define putPixel                (TGEsys->_putPixel)
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
#if defined(TGE_USE_ELLIPSES)
  #define ellipse               (TGEsys->_ellipse)
  #define filledEllipse         (TGEsys->_filledEllipse)
  #define circle                (TGEsys->_circle)
  #define filledCircle          (TGEsys->_filledCircle)
#endif
#if defined(TGE_USE_REGION_FILLS)
  #define fillRegion		(TGEsys->_fillRegion)
#endif
#if defined(TGE_USE_COLOUR_APPROXIMATION)
  #define colourCloseTo		(TGEsys->_colourCloseTo)
  #define colourCloseToX        (TGEsys->_colourCloseToX)
  #define colorCloseTo          (TGEsys->_colourCloseTo)
  #define colorCloseToX         (TGEsys->_colourCloseToX)
#endif
#define fillLine                (TGEsys->_fillLine)

#define loadGraphDriver       TGE_loadGraphDriver
#define unloadGraphDriver     TGE_unloadGraphDriver

#if defined(TGE_USE_VIRTUAL_SCREENS)
  #define setGraphicsOutputAddr TGE_setGraphicsOutputAddr /* set outputs */
  #define setGraphicsInputAddr  TGE_setGraphicsInputAddr  /* set input   */
  #define setGraphicsAddr       TGE_setGraphicsAddr       /* set I/O     */
  #define makeVirtScreen        TGE_makeVirtScreen        /* make virtual screen */
  #define getGraphicsOutputAddr TGE_getGraphicsOutputAddr /* set output  */
  #define getGraphicsInputAddr  TGE_getGraphicsInputAddr  /* set input   */
#endif

#if defined(TGE_USE_OUTPUT_MODES)
  #define setGraphicsOutputMode   TGE_setOutputMode         /* output mode */
#endif

#define setOutputViewport	TGE_setOutputViewport	  /* viewports */
#define setInputViewport       	TGE_setInputViewport
#define setViewports            TGE_setViewports
#define setViewport		TGE_setViewports
#define getOutputViewport	TGE_getOutputViewport
#define getInputViewport      	TGE_getInputViewport
#define getViewport		TGE_getOutputViewport

#define clipLine		TGE_clipLine		  /* clipping */
#define clipRect                TGE_clipRect
#define clipFilledRect          TGE_clipRect
#define clipInputPoint          TGE_clipInputPoint
#define clipOutputPoint         TGE_clipOutputPoint
#define clipPoint               TGE_clipPoint
#define pointOnScreen           TGE_pointOnScreen

#define scaleBitmap             TGE_scaleBitmap           /* scaling */

#define fadePalette             TGE_fadePalette           /* palette */
#define greyPalette             TGE_greyPalette
#define rotatePalette           TGE_rotatePalette
#define promotePalette          TGE_promotePalette
#define demotePalette           TGE_demotePalette

#define loadRawFile             TGE_loadRawFile           /* image files */
#define displayRawFile          TGE_displayRawFile
#define loadPalFile             TGE_loadPalFile
#define saveRawFile             TGE_saveRawFile
#define savePalFile             TGE_savePalFile
#define loadPcxFile             TGE_loadPcxFile
#define displayPcxFile          TGE_displayPcxFile
#define loadPcxFilePal          TGE_loadPcxFilePal

#define replaceColour           TGE_replaceColour         /* colour replacing */
#define replaceColor            TGE_replaceColour



/*
 * These macros provide easy access to important mode information in the
 * TGE system structure.
 */

#define INMAXX			(TGEsys->inMaxX)
#define INMAXY			(TGEsys->inMaxY)
#define OUTMAXX			(TGEsys->outMaxX)
#define OUTMAXY			(TGEsys->outMaxY)
#define MAXCOLOUR		(TGEsys->maxColour)
#define MAXCOLOR                (TGEsys->maxColour)
#define XRATIO                  (TGEsys->xRatio)
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



/*
 * These macros are for the various output modes.
 */

#if defined(TGE_USE_OUTPUT_MODES)
  #define TGE_COPY_PUT          0
  #define TGE_AND_PUT           1
  #define TGE_NOT_PUT           2
  #define TGE_OR_PUT            3
  #define TGE_XOR_PUT           4
#endif



/*
 * These macros are the return codes from loadGraphDriver().
 */

#define TGE_SUCCESS	 0		/* driver loaded successfully */
#define TGE_OPEN_ERR	-1		/* error opening given file */
#define TGE_FORMAT_ERR	-2		/* file is not a TGE driver */
#define TGE_ALLOC_ERR	-3	    	/* error allocating memory */
#define TGE_FILE_ERR	-4		/* file I/O error */



/*
 * Point clipping routines.
 */

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



/*
 * Image dimension macros.
 */

#if defined(TGE_USE_IMAGES)
  #define imageWidth(image)     ((unsigned*)image)[0]
  #define imageHeight(image)    ((unsigned*)image)[1]
#endif



/*
 * Ease the use of TGE with different compilers by standardizing #defines.
 */

#if defined(__TURBOC__)
  #define ENABLE_INTERRUPTS     enable()
  #define DISABLE_INTERRUPTS    disable()
  #define GENERATE_INTERRUPT(i) geninterrupt(i)
#else
  #define ENABLE_INTERRUPTS     asm cli
  #define DISABLE_INTERRUPTS    asm sti
  #define GENERATE_INTERRUPT(i) asm int i
#endif

#if !defined(__TURBOC__)
  #define MK_FP(seg,ofs)        (((unsigned long)seg<<16) | ofs)
  #define FP_SEG(fp)            ((unsigned short)(seg>>16))
  #define FP_OFF(fp)            ((unsigned long)fp & 0xFFFF)
#endif



#endif
