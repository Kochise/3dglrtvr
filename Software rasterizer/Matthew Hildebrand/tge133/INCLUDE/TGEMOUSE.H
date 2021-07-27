/*****************************************************************************
*       The Graphics Engine version 1.33                                     *
*                                                                            *
*       The Graphics Engine code and documentation are                       *
*       Copyright (c) 1993-1994 by Matthew Hildebrand; all rights reserved.  *
*                                                                            *
*       Unauthorised usage or modification of any or all of The Graphics     *
*       Engine is strictly prohibited.                                       *
*****************************************************************************/

#if !defined(TGEMOUSEdotH)
#define TGEMOUSEdotH


/* Note that this C/C++ bit might not work properly with your compiler.  If
   you are using C++ and your linker reports that these functions are not
   found, try explicitly forcing the extern "C" syntax by removing these
   preprocessor directives. */

#ifdef __cplusplus
extern "C" {
#endif

void far TGE_initNewMouse(void huge (*drawPointer)(int x, int y, void far *image),
		  void huge (*drawBitmap)(int x, int y, void far *image),
		  void huge (*getBitmap)(int ulx, int uly, int lrx, int lry,
		  	void far *image));
void TGE_deInitNewMouse(void);
void TGE_disableNewMouse(void);
void TGE_enableNewMouse(void);

void TGE_setupMousePointer(int pointerNum);
void TGE_setPointerColours(void);

int far TGE_resetMouse(void);
int far TGE_getButtonsMouse(void);
void far TGE_showMouse(void);
void far TGE_hideMouse(void);
void far TGE_getPosMouse(int far *x, int far *y);
void far TGE_setPosMouse(unsigned x, unsigned y);
int far TGE_buttonMouse(void);
int far TGE_leftButtonMouse(void);
int far TGE_rightButtonMouse(void);
int far TGE_centerButtonMouse(void);
unsigned far TGE_buttonPressMouse(unsigned button, int far *x, int far *y);
unsigned far TGE_buttonReleaseMouse(unsigned button, int far *x, int far *y);
void far TGE_setHorizLimitsMouse(unsigned min, unsigned max);
void far TGE_setVertLimitsMouse(unsigned min, unsigned max);
void far TGE_setPointerMouse(int xoff, int yoff, void far *p);
unsigned far TGE_getSaveSizeMouse(void);
void far TGE_saveStateMouse(void far *buf);
void far TGE_restoreStateMouse(void far *buf);
void far TGE_softResetMouse(void);
void far TGE_waitReleaseMouse(int button);
void far TGE_setRatioMouse(int horiz, int vert);
void far TGE_getSensitivityMouse(int *horiz, int *vert, int *doubleSpeed);

#ifdef __cplusplus
};
#endif


/* The function names of TGE's mouse functions are defined here.  Since they
   are really just macros pretending to be functions, you can easily
   change their names here if you don't like the default ones. */

#define initNewMouse()			TGE_initNewMouse(putImageInv, \
						putImage, getImage)
#define deInitNewMouse		     	TGE_deInitNewMouse
#define disableNewMouse			TGE_disableNewMouse
#define enableNewMouse			TGE_enableNewMouse

#define setupMousePointer(num)	 	TGE_setupMousePointer(num)
#define setPointerColours		TGE_setPointerColours

#define resetMouse			TGE_resetMouse
#define getButtonsMouse			TGE_getButtonsMouse
#define showMouse			TGE_showMouse
#define hideMouse			TGE_hideMouse
#define getPosMouse(x,y)		TGE_getPosMouse(x,y)
#define setPosMouse(x,y)		TGE_setPosMouse(x,y)
#define buttonMouse			TGE_buttonMouse
#define leftButtonMouse			TGE_leftButtonMouse
#define rightButtonMouse	       	TGE_rightButtonMouse
#define centerButtonMouse		TGE_centerButtonMouse
#define buttonPressMouse(button,x,y)	TGE_buttonPressMouse(button,x,y)
#define buttonReleaseMouse(button,x,y)	TGE_buttonReleaseMouse(button,x,y)
#define setHorizLimitsMouse(min,max)	TGE_setHorizLimitsMouse(min,max)
#define setVertLimitsMouse(min,max)	TGE_setVertLimitsMouse(min,max)
#define setPointerMouse(xOff,yOff,p)	TGE_setPointerMouse(xOff,yOff,p)
#define getSaveSizeMouse		TGE_getSaveSizeMouse
#define saveStateMouse(buf)		TGE_saveStateMouse(buf)
#define restoreStateMouse(buf)		TGE_restoreStateMouse(buf)
#define softResetMouse			TGE_softResetMouse
#define waitReleaseMouse(button)	TGE_waitReleaseMouse(button)
#define setRatioMouse(horiz,vert)	TGE_setRatioMouse(horiz,vert)
#define getSensitivityMouse(h,v,d)	TGE_getSensitivityMouse(h,v,d)


/* These macros are used by the mouse routines */

#define	LEFTBUTTON	1
#define RIGHTBUTTON	2
#define	CENTERBUTTON	3

#define	ARROW_POINTER		1
#define TARGET_POINTER		2
#define BIG_ARROW_POINTER	3
#define BIG_TARGET_POINTER	4



#endif
