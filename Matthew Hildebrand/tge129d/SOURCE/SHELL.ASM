; Skeleton graphics driver -- use as a foundation for custom drivers
; Loadable driver for The Graphics Engine
; Copyright (c) 1993 by Matthew Hildebrand
; Turbo Assembler syntax

IDEAL
MODEL LARGE TGESHELL_TEXT
P386N


SCREEN_WIDE	=	320
SCREEN_DEEP	=	200
NUM_COLOURS	=	256


		CODESEG TGESHELL_TEXT
		ORG	0

;*** Signature
			db	'TGE3'
;*** Non-I/O functions
_initGraphics		dw	initGraphics		; initGraphics
			dw	?
_deInitGraphics		dw	0			; deInitGraphics
			dw	?
_setPaletteReg		dw	0			; setPaletteReg
			dw	?
_getPaletteReg		dw	0			; getPaletteReg
			dw	?
_setBlockPalette	dw	0			; setBlockPalette
			dw	?
_getBlockPalette	dw	0			; getBlockPalette
			dw	?
_colourCloseTo		dw	0			; colourCloseTo
			dw	?
_colourCloseToX		dw	0			; colourCloseToX
			dw	?
_imageSize		dw	0			; imageSize
			dw	?
_imageSizeDim		dw	0			; imageSizeDim
			dw	?
_setPage		dw	0			; setPage (not implemented yet)
			dw	?

;*** Currently active I/O functions (filled in by loadGraphDriver())
			dd	20	DUP(?)

;*** Input functions
_getImage_scr		dw	0			; getImage
			dw	?
_getImage_mem		dw	0
			dw	?
_getLine_scr		dw      0		      	; getLine
			dw	?
_getLine_mem		dw	0
			dw	?
_getPixel_scr		dw	0			; getPixel
			dw	?
_getPixel_mem		dw	0
			dw	?

;*** Output functions
_putImage_scr_copy	dw	0			; putImage
			dw	?
_putImage_scr_and	dw	0
			dw	?
_putImage_scr_not	dw	0
			dw	?
_putImage_scr_or	dw	0
			dw	?
_putImage_scr_xor	dw	0
			dw	?
_putImage_mem_copy	dw	0
			dw	?
_putImage_mem_and	dw	0
			dw	?
_putImage_mem_not	dw	0
			dw	?
_putImage_mem_or	dw	0
			dw	?
_putImage_mem_xor	dw	0
			dw	?
_putImageInv_scr_copy	dw	0			; putImageInv
			dw	?
_putImageInv_scr_and	dw	0
			dw	?
_putImageInv_scr_not	dw      0
			dw	?
_putImageInv_scr_or	dw      0
			dw	?
_putImageInv_scr_xor	dw	0
			dw	?
_putImageInv_mem_copy	dw	0
			dw	?
_putImageInv_mem_and	dw	0
			dw	?
_putImageInv_mem_not	dw	0
			dw	?
_putImageInv_mem_or	dw	0
			dw	?
_putImageInv_mem_xor	dw	0
			dw	?
_putLine_scr_copy      	dw	0			; putLine
			dw	?
_putLine_scr_and      	dw      0
			dw	?
_putLine_scr_not      	dw      0
			dw	?
_putLine_scr_or      	dw      0
			dw	?
_putLine_scr_xor      	dw	0
			dw	?
_putLine_mem_copy	dw      0
			dw	?
_putLine_mem_and	dw      0
			dw	?
_putLine_mem_not	dw      0
			dw	?
_putLine_mem_or		dw      0
			dw	?
_putLine_mem_xor	dw	0
			dw	?
_putLineInv_scr_copy   	dw	0			; putLineInv
			dw	?
_putLineInv_scr_and   	dw	0
			dw	?
_putLineInv_scr_not   	dw	0
			dw	?
_putLineInv_scr_or   	dw	0
			dw	?
_putLineInv_scr_xor   	dw	0
			dw	?
_putLineInv_mem_copy	dw	0
			dw	?
_putLineInv_mem_and	dw	0
			dw	?
_putLineInv_mem_not	dw	0
			dw	?
_putLineInv_mem_or	dw	0
			dw	?
_putLineInv_mem_xor	dw	0
			dw	?
_putPixel_scr_copy	dw	0			; putPixel
			dw	?
_putPixel_scr_and	dw      0
			dw	?
_putPixel_scr_not	dw      0
			dw	?
_putPixel_scr_or	dw      0
			dw	?
_putPixel_scr_xor	dw	0
			dw	?
_putPixel_mem_copy	dw	0
			dw	?
_putPixel_mem_and	dw      0
			dw	?
_putPixel_mem_not	dw      0
			dw	?
_putPixel_mem_or	dw      0
			dw	?
_putPixel_mem_xor	dw	0
			dw	?
_line_scr_copy		dw	0			; line
			dw	?
_line_scr_and		dw      0
			dw	?
_line_scr_not		dw      0
			dw	?
_line_scr_or		dw      0
			dw	?
_line_scr_xor		dw	0
			dw	?
_line_mem_copy		dw	0
			dw	?
_line_mem_and		dw	0
			dw	?
_line_mem_not		dw	0
			dw	?
_line_mem_or		dw	0
			dw	?
_line_mem_xor		dw	0
			dw	?
_horizLine_scr_copy	dw	0		  	; horizLine
			dw	?
_horizLine_scr_and	dw      0
			dw	?
_horizLine_scr_not	dw      0
			dw	?
_horizLine_scr_or	dw      0
			dw	?
_horizLine_scr_xor	dw	0
			dw	?
_horizLine_mem_copy	dw	0
			dw	?
_horizLine_mem_and	dw      0
			dw	?
_horizLine_mem_not	dw      0
			dw	?
_horizLine_mem_or	dw      0
			dw	?
_horizLine_mem_xor	dw	0
			dw	?
_vertLine_scr_copy	dw	0		 	; vertLine
			dw	?
_vertLine_scr_and	dw	0
			dw	?
_vertLine_scr_not	dw	0
			dw	?
_vertLine_scr_or	dw	0
			dw	?
_vertLine_scr_xor	dw	0
			dw	?
_vertLine_mem_copy	dw	0
			dw	?
_vertLine_mem_and	dw	0
			dw	?
_vertLine_mem_not	dw	0
			dw	?
_vertLine_mem_or	dw	0
			dw	?
_vertLine_mem_xor	dw	0
			dw	?
_drawRect_scr_copy	dw	0			; drawRect
			dw	?
_drawRect_scr_and	dw	0
			dw	?
_drawRect_scr_not	dw	0
			dw	?
_drawRect_scr_or	dw	0
			dw	?
_drawRect_scr_xor	dw	0
			dw	?
_drawRect_mem_copy	dw	0
			dw	?
_drawRect_mem_and	dw	0
			dw	?
_drawRect_mem_not	dw	0
			dw	?
_drawRect_mem_or	dw	0
			dw	?
_drawRect_mem_xor	dw	0
			dw	?
_filledRect_scr_copy	dw	0			; filledRect
			dw	?
_filledRect_scr_and	dw      0
			dw	?
_filledRect_scr_not	dw      0
			dw	?
_filledRect_scr_or	dw      0
			dw	?
_filledRect_scr_xor	dw	0
			dw	?
_filledRect_mem_copy	dw	0
			dw	?
_filledRect_mem_and	dw	0
			dw	?
_filledRect_mem_not	dw	0
			dw	?
_filledRect_mem_or	dw	0
			dw	?
_filledRect_mem_xor	dw	0
			dw	?
_clearGraphics_scr_copy	dw	0			; clearGraphics
			dw	?
_clearGraphics_scr_and	dw      0
			dw	?
_clearGraphics_scr_not	dw      0
		      	dw	?
_clearGraphics_scr_or	dw      0
			dw	?
_clearGraphics_scr_xor	dw	0
			dw	?
_clearGraphics_mem_copy	dw	0
			dw	?
_clearGraphics_mem_and	dw	0
			dw	?
_clearGraphics_mem_not	dw	0
			dw	?
_clearGraphics_mem_or	dw	0
			dw	?
_clearGraphics_mem_xor	dw	0
			dw	?
_ellipse_scr_copy	dw	0			; ellipse
			dw	?
_ellipse_scr_and	dw	0
			dw	?
_ellipse_scr_not	dw	0
			dw	?
_ellipse_scr_or		dw	0
			dw	?
_ellipse_scr_xor	dw	0
			dw	?
_ellipse_mem_copy	dw	0
			dw	?
_ellipse_mem_and	dw	0
			dw	?
_ellipse_mem_not	dw	0
			dw	?
_ellipse_mem_or		dw	0
			dw	?
_ellipse_mem_xor	dw	0
			dw	?
_filledEllipse_scr_copy	dw	0			; filledEllipse
			dw	?
_filledEllipse_scr_and	dw	0
			dw	?
_filledEllipse_scr_not	dw	0
			dw	?
_filledEllipse_scr_or	dw	0
			dw	?
_filledEllipse_scr_xor	dw	0
			dw	?
_filledEllipse_mem_copy	dw	0
			dw	?
_filledEllipse_mem_and	dw	0
			dw	?
_filledEllipse_mem_not	dw	0
			dw	?
_filledEllipse_mem_or	dw	0
			dw	?
_filledEllipse_mem_xor	dw	0
			dw	?
_circle_scr_copy	dw	0			; circle
			dw	?
_circle_scr_and		dw	0
			dw	?
_circle_scr_not		dw	0
			dw	?
_circle_scr_or		dw	0
			dw	?
_circle_scr_xor		dw	0
			dw	?
_circle_mem_copy	dw	0
			dw	?
_circle_mem_and		dw	0
			dw	?
_circle_mem_not		dw	0
			dw	?
_circle_mem_or		dw	0
			dw	?
_circle_mem_xor		dw	0
			dw	?
_filledCircle_scr_copy	dw	0			; filledCircle
			dw	?
_filledCircle_scr_and	dw	0
			dw	?
_filledCircle_scr_not	dw	0
			dw	?
_filledCircle_scr_or	dw	0
			dw	?
_filledCircle_scr_xor	dw	0
			dw	?
_filledCircle_mem_copy	dw	0
			dw	?
_filledCircle_mem_and	dw	0
			dw	?
_filledCircle_mem_not	dw	0
			dw	?
_filledCircle_mem_or	dw	0
			dw	?
_filledCircle_mem_xor	dw	0
			dw	?
_fillRegion_scr_copy	dw	0			; fillRegion
			dw	?
_fillRegion_scr_and	dw	0
			dw	?
_fillRegion_scr_not	dw	0
			dw	?
_fillRegion_scr_or	dw	0
			dw	?
_fillRegion_scr_xor	dw	0
			dw	?
_fillRegion_mem_copy	dw	0
			dw	?
_fillRegion_mem_and	dw	0
			dw	?
_fillRegion_mem_not	dw	0
			dw	?
_fillRegion_mem_or	dw	0
			dw	?
_fillRegion_mem_xor	dw	0
			dw	?
_fillLine_scr_copy	dw	0			; fillLine
			dw	?
_fillLine_scr_and	dw      0
			dw	?
_fillLine_scr_not	dw      0
			dw	?
_fillLine_scr_or	dw      0
			dw	?
_fillLine_scr_xor	dw	0
			dw	?
_fillLine_mem_copy	dw	0
			dw	?
_fillLine_mem_and	dw      0
			dw	?
_fillLine_mem_not	dw      0
			dw	?
_fillLine_mem_or	dw      0
			dw	?
_fillLine_mem_xor	dw	0
			dw	?
;*** Mode information
scrnMaxX		dw	319	; physical dimensions
scrnMaxY		dw	199
maxColour		dw	255	; maximum colour number
xRatio		       	dw	8	; aspect ratio 8:5 (320:200 in
yRatio			dw	5	;   lowest terms)
bitsPerPixel		dw	8	; 8 bits per pixel
inMaxX			dw	319	; current input screen dimensions
inMaxY			dw	199
outMaxX			dw	319	; current output screen dimensions
outMaxY			dw	199
inScreenWide		dw	?	; needed only for virtual screens
outScreenWide		dw	?
;*** Viewport information
inViewportULX		dw	0
inViewportULY		dw	0
inViewportLRX		dw	319
inViewportLRY		dw	199
outViewportULX		dw	0
outViewportULY		dw	0
outViewportLRX		dw	319
outViewportLRY		dw	199
;*** Paging information
pagingSupported		dw	?	; not implemented yet
curPage			dw	?	; not implemented yet
maxPage			dw	?	; not implemented yet
;*** Force (image width MOD imageWideAdjust) = 0.
imageWideAdjust		dw	?	; not implemented yet
;*** Current and screen addresses
	LABEL	inAddr	DWORD		; current input address
inOff	dw	0
inSeg	dw	0A000h
	LABEL	outAddr	DWORD		; current output address
outOff	dw	0
outSeg	dw	0A000h
	LABEL	scrAddr	DWORD		; screen address
scrOff	dw	0
scrSeg	dw	0A000h
;*** Copyright string
	db	'The Graphics Engine -- Copyright (c) 1993 by Matthew Hildebrand'



;*****
;***** initGraphics
;*****

	PUBLIC C	initGraphics
PROC	C	initGraphics
  mov	ax,0013h			; turn on 320x200x256 mode
  int	10h
  mov	ax,1				; success code in AX for return
  retf 					; return
ENDP


	ENDS

END
