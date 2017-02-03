; 320x200x256
; Loadable driver for The Graphics Engine
; Copyright (c) 1993 by Matthew Hildebrand
; Turbo Assembler syntax

IDEAL
MODEL LARGE TGE320X200_TEXT
P386N


SCREEN_WIDE	=	320
SCREEN_DEEP	=	200
NUM_COLOURS	=	256


                CODESEG TGE320X200_TEXT
		ORG	0

;*** Signature
			db	'TGE3'
;*** Non-I/O functions
_initGraphics		dw	initGraphics		; initGraphics
			dw	?
_deInitGraphics		dw	0			; deInitGraphics
			dw	?
_setPaletteReg		dw	setPaletteReg		; setPaletteReg
			dw	?
_getPaletteReg		dw	getPaletteReg		; getPaletteReg
			dw	?
_setBlockPalette	dw	setBlockPalette		; setBlockPalette
			dw	?
_getBlockPalette	dw	getBlockPalette		; getBlockPalette
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
_getImage_scr		dw	getImage_scr		; getImage
			dw	?
_getImage_mem		dw	0
			dw	?
_getLine_scr		dw	getLine_scr	      	; getLine
			dw	?
_getLine_mem		dw	getLine_mem
			dw	?
_getPixel_scr		dw	getPixel_scr		; getPixel
			dw	?
_getPixel_mem		dw	getPixel_mem
			dw	?

;*** Output functions
_putImage_scr_copy	dw	putImage		; putImage
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
_putImageInv_scr_copy	dw	putImageInv		; putImageInv
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
_putLine_scr_copy       dw      putLine_scr_copy        ; putLine
			dw	?
_putLine_scr_and      	dw      0
			dw	?
_putLine_scr_not      	dw      0
			dw	?
_putLine_scr_or      	dw      0
			dw	?
_putLine_scr_xor      	dw	0
			dw	?
_putLine_mem_copy	dw	putLine_mem_copy
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
_putLineInv_mem_copy	dw	putLineInv_mem_copy
			dw	?
_putLineInv_mem_and	dw	0
			dw	?
_putLineInv_mem_not	dw	0
			dw	?
_putLineInv_mem_or	dw	0
			dw	?
_putLineInv_mem_xor	dw	0
			dw	?
_putPixel_scr_copy	dw	putPixel_scr_copy	; putPixel
			dw	?
_putPixel_scr_and	dw      putPixel_scr_and
			dw	?
_putPixel_scr_not	dw      putPixel_scr_not
			dw	?
_putPixel_scr_or	dw      putPixel_scr_or
			dw	?
_putPixel_scr_xor	dw	putPixel_scr_xor
			dw	?
_putPixel_mem_copy	dw	putPixel_mem_copy
			dw	?
_putPixel_mem_and	dw      putPixel_mem_and
			dw	?
_putPixel_mem_not	dw	putPixel_mem_not
			dw	?
_putPixel_mem_or	dw	putPixel_mem_or
			dw	?
_putPixel_mem_xor	dw	putPixel_mem_xor
			dw	?
_line_scr_copy		dw	line			; line
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
_horizLine_scr_copy	dw	horizLine_scr_copy	; horizLine
			dw	?
_horizLine_scr_and	dw      0
			dw	?
_horizLine_scr_not	dw      0
			dw	?
_horizLine_scr_or	dw      0
			dw	?
_horizLine_scr_xor	dw	0
			dw	?
_horizLine_mem_copy	dw	horizLine_mem_copy
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
_filledRect_scr_copy	dw	filledRect		; filledRect
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
_clearGraphics_scr_copy	dw	clearGraphics		; clearGraphics
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
_fillLine_scr_copy	dw	horizLine_scr_copy	; fillLine
			dw	?
_fillLine_scr_and	dw      0
			dw	?
_fillLine_scr_not	dw      0
			dw	?
_fillLine_scr_or	dw      0
			dw	?
_fillLine_scr_xor	dw	0
			dw	?
_fillLine_mem_copy	dw	horizLine_mem_copy
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


inited		db	0               ; already initialized flag
tempPalette	db      768	DUP(?)	; for palette data
lineOffs	dw	200	DUP(?)	; line start offset table


;*****
;***** initGraphics
;*****

PROC	C	initGraphics
  cmp	[inited],0
  je	@@L1				; skip if already done

  mov	ax,0013h			; short initialization:
  int	10h				; turn on graphics and leave
  mov	ax,1
  retf

	@@L1:
  push	di				; first initialization
  mov	[inited],1

  mov	ax,1200h			; enable default palette loading
  mov	bl,31h
  int	10h
  mov	ax,0013h			; turn on 320x200x256
  int	10h

  mov	di,OFFSET lineOffs		; set up line start offset table
  mov	ax,cs
  mov	es,ax
  mov	cx,200
  xor	bx,bx
  cld
	@@LLoop:
  mov	ax,320
  mul	bx
  inc	bx
  stosw
  loop	@@LLoop

  mov	ax,1				; return success flag
  pop	di
  retf
ENDP


;*****
;***** putImage
;*****

PROC	C	putImage
	ARG	x:WORD,y:WORD,image:DWORD
	LOCAL	@@destAdd:WORD,@@srcAdd:WORD,@@xSize:WORD
  push	ds si di

  mov	[@@destAdd],320
  mov	[@@srcAdd],0

  lds	si,[image]
  les	di,[outAddr]
  cld

  lodsw					; xsize
  or	ax,ax
  jz	@@Exit
  mov	cx,ax
  mov	[@@xSize],cx
  lodsw					; ysize
  or	ax,ax
  jz	@@Exit
  mov	dx,ax

  mov	ax,[x]
  cmp	ax,[outViewportLRX]		; check x <= OUTVIEWPORTLRX
  jg	@@Exit
  cmp	ax,[outViewportULX]		; check x >= OUTVIEWPORTULX
  jl	@@xLessThanMin

	@@checkxMax:
  mov	ax,[x]
  add	ax,cx			; right edge of image
  cmp	ax,[outViewportLRX]
  jg	@@truncateRightImage	; truncate if necessary

	@@checkyMin:
  mov	ax,[y]
  cmp	ax,[outViewportLRY]		; check y <= OUTVIEWPORTLRY
  jg	@@Exit
  cmp	ax,[outViewportULY]		; check y >= OUTVIEWPORTULY
  jl	@@yLessThanMin

	@@checkyMax:
  mov	ax,[y]
  add	ax,dx			; bottom of image
  cmp	ax,[outViewportLRY]
  jg	@@truncateBottomImage	; truncate if necessary

  jmp	short	@@drawImage	; jump to draw logic

	@@xLessThanMin:			; clip x to 0
  mov	bx,[outViewportULX]
  mov	[x],bx
  sub	bx,ax
  add	si,bx
  sub	cx,bx
  cmp	cx,0
  jle	@@Exit			; quit if offscreen
  add	[@@srcAdd],bx
  jmp	short	@@checkxMax

	@@Exit:				; quit putImage()
  pop	di si ds
  leave
  retf

	@@truncateRightImage:		; truncate the excess of right edge
  sub	ax,[outViewportLRX]	; ax=right edge from before
  dec	ax
  sub	cx,ax
  add	[@@srcAdd],ax
  jmp	short	@@checkyMin

	@@yLessThanMin:			; clip y to OUTVIEWPORTULY
  mov	bx,[outViewportULY]
  mov	[y],bx
  sub	bx,ax
  sub	dx,bx
  cmp	dx,0
  jle	@@Exit			; quit if offscreen
  push	dx cx
  mov	ax,bx
  mov	cx,[@@xSize]
  mul	cx
  pop	cx dx
  add	si,ax
  jmp	short	@@checkyMax

	@@truncateBottomImage:		; clip y to OUTVIEWPORTLRY
  sub	ax,[outViewportLRY]
  dec	ax
  sub	dx,ax
  cmp	dx,0			; quit if no lines left
  jle	@@Exit			; fall through to @@drawImage

	@@drawImage:			; label for @@yGreaterThanMax jmp
  push	dx				; calculate pixel address
  mov	dx,[x]
  mov	ax,[y]
  xchg	ah,al			; ax=256*y
  add	dx,ax			; dx=256*y+x
  shr	ax,2			; ax=64*y
  add	dx,ax			; dx=320*y+x
  mov	di,dx
  pop	dx

  mov	ax,[@@destAdd]
  sub	ax,cx				; update destAdd

  mov	bx,cx				; BX = CX
  jmp	short	@@LoopB			; jump to loop entry point
	@@LoopA:
  mov	cx,bx				; CX = line length in bytes
  add	di,ax				; update DI
  add	si,[@@srcAdd]			; update SI
	@@LoopB:
  push	cx				; store CX
  shr	cx,2				; CX = line length in dwords
  rep	movsd				; move the dwords
  pop	cx				; restore CX
  and	cx,0000000000000011b		; CX = number of residual bytes
  rep	movsb				; move the residual bytes, if any
  dec	dx				; decrement loop counter
  jnz	@@LoopA				; restart loop if necesary
  jmp	short	@@Exit			; done loop, quit
ENDP


;*****
;***** putImageInv
;*****

PROC	C	putImageInv
	ARG	x:WORD,y:WORD,image:DWORD
	LOCAL	@@destAdd:WORD,@@srcAdd:WORD,@@xSize:WORD
  push	ds si di

  mov	[@@destAdd],320
  mov	[@@srcAdd],0

  lds	si,[image]
  les	di,[outAddr]
  cld

  lodsw					; xsize
  or	ax,ax
  jz	@@Exit
  mov	cx,ax
  mov	[@@xSize],cx
  lodsw					; ysize
  or	ax,ax
  jz	@@Exit
  mov	dx,ax

  mov	ax,[x]
  cmp	ax,[outViewportLRX]		; check x <= OUTVIEWPORTLRX
  jg	@@Exit
  cmp	ax,[outViewportULX]		; check x >= OUTVIEWPORTULX
  jl	@@xLessThanMin

	@@checkxMax:
  mov	ax,[x]
  add	ax,cx			; right edge of image
  cmp	ax,[outViewportLRX]
  jg	@@truncateRightImage	; truncate if necessary

	@@checkyMin:
  mov	ax,[y]
  cmp	ax,[outViewportLRY]		; check y <= OUTVIEWPORTLRY
  jg	@@Exit
  cmp	ax,[outViewportULY]		; check y >= OUTVIEWPORTULY
  jl	@@yLessThanMin

	@@checkyMax:
  mov	ax,[y]
  add	ax,dx			; bottom of image
  cmp	ax,[outViewportLRY]
  jg	@@truncateBottomImage	; truncate if necessary

  jmp	short	@@drawImage	; jump to draw logic

	@@xLessThanMin:			; clip x to 0
  mov	bx,[outViewportULX]
  mov	[x],bx
  sub	bx,ax
  add	si,bx
  sub	cx,bx
  cmp	cx,0
  jle	@@Exit			; quit if offscreen
  add	[@@srcAdd],bx
  jmp	short	@@checkxMax

	@@Exit:				; quit putImage()
  pop	di si ds
  leave
  retf

	@@truncateRightImage:		; truncate the excess of right edge
  sub	ax,[outViewportLRX]	; ax=right edge from before
  dec	ax
  sub	cx,ax
  add	[@@srcAdd],ax
  jmp	short	@@checkyMin

	@@yLessThanMin:			; clip y to OUTVIEWPORTULY
  mov	bx,[outViewportULY]
  mov	[y],bx
  sub	bx,ax
  sub	dx,bx
  cmp	dx,0
  jle	@@Exit			; quit if offscreen
  push	dx cx
  mov	ax,bx
  mov	cx,[@@xSize]
  mul	cx
  pop	cx dx
  add	si,ax
  jmp	short	@@checkyMax

	@@truncateBottomImage:		; clip y to OUTVIEWPORTLRY
  sub	ax,[outViewportLRY]
  dec	ax
  sub	dx,ax
  cmp	dx,0			; quit if no lines left
  jle	@@Exit			; fall through to @@drawImage

	@@drawImage:			; label for @@yGreaterThanMax jmp
; calculate the pixel address
  push	dx
  mov	dx,[x]
  mov	ax,[y]
  xchg	ah,al			; ax=256*y
  add	dx,ax			; dx=256*y+x
  shr	ax,2			; ax=64*y
  add	dx,ax			; dx=320*y+x
  mov	di,dx
  pop	dx

  sub	[@@destAdd],cx			; update destAdd
  mov	bx,cx
  jmp	short	@@xLoop
	@@yLoop:
  add	di,[@@destAdd]
  add	si,[@@srcAdd]
  mov	cx,bx

	@@xLoop:
  lodsb
  or	al,al
  jz	@@Invisible
  stosb
  loop	@@xLoop
  dec	dx
  jnz	@@yLoop
  jmp	short	@@Exit

	@@Invisible:
  inc	di
  loop	@@xLoop

  dec	dx
  jnz	@@yLoop
  jmp	short	@@Exit
ENDP


;*****
;***** getImage
;*****

PROC	C	getImage_scr
	ARG	x1:WORD,y1:WORD,x2:WORD,y2:WORD,image:DWORD
  push	ds si di

  mov	bx,[inViewportULX]		; preload for speed
  mov	cx,[inViewportLRX]

  mov	ax,[x1]				; clip x1
  cmp	ax,bx
  jge	@@L1b
  mov	[x1],bx
  jmp	short	@@L2a
	@@L1b:
  cmp	ax,cx
  jle	@@L2a
  mov	[x1],cx

	@@L2a:
  mov	ax,[x2]				; clip x2
  cmp	ax,bx
  jge	@@L2b
  mov	[x2],bx
  jmp	short  @@L3a
	@@L2b:
  cmp	ax,cx
  jle	@@L3a
  mov	[x2],cx

	@@L3a:
  mov	bx,[inViewportULY]		; preload for speed
  mov	cx,[inViewportLRY]

  mov	ax,[y1]				; clip y1
  cmp   ax,bx
  jge	@@L3b
  mov	[y1],bx
  jmp	short	@@L4a
	@@L3b:
  cmp	ax,cx
  jle	@@L4a
  mov	[y1],cx

	@@L4a:
  mov	ax,[y2]				; clip y2
  cmp	ax,bx
  jge	@@L4b
  mov	[y2],bx
  jmp	short	@@L5
	@@L4b:
  cmp	ax,cx
  jle	@@L5
  mov	[y2],cx

	@@L5:
  mov	ax,[x2]				; make sure x2>=x1
  cmp	ax,[x1]
  jge	@@L6
  mov	dx,[x1]
  mov	[x2],dx
  mov	[x1],ax
	@@L6:
  mov	ax,[y2]				; make sure y2>=y1
  cmp	ax,[y1]
  jge	@@L7
  mov	dx,[y1]
  mov	[y2],dx
  mov	[y1],ax

	@@L7:
  cld
  lds	si,[inAddr]			; set up DS:SI
  mov	bx,[y1]
  shl	bx,1
  add	si,[lineOffs+bx]
  add	si,[x1]

  les	di,[image]			; set up ES:DI
  mov	ax,[x2]
  sub	ax,[x1]
  inc	ax
  push	ax
  stosw					; store image width
  mov	ax,[y2]
  sub	ax,[y1]
  inc	ax
  stosw					; store image depth

  mov	ax,[x2]				; calculate srcAdd
  sub	ax,[x1]
  mov	dx,319
  sub	dx,ax
  push	dx				; save srcAdd for later

  mov	ax,[y2]				; y loop counter
  sub	ax,[y1]
  mov	dx,ax
  inc	dx

  pop	ax				; pop srcAdd
  pop	cx				; pop image width

  mov	bx,cx				; BX = CX
  jmp	short	@@LoopB			; jump to loop entry point
	@@LoopA:
  mov	cx,bx				; CX = line length in bytes
  add	si,ax				; update SI
	@@LoopB:
  push	cx				; store CX
  shr	cx,2				; CX = line length in dwords
  rep	movsd				; move the dwords
  pop	cx				; restore CX
  and	cx,0000000000000011b		; CX = number of residual bytes
  rep	movsb				; move the residual bytes, if any
  dec	dx				; decrement loop counter
  jnz	@@LoopA				; restart loop if necesary

  pop	di si ds			; restore registers
  leave					; clean up call stack
  retf					; return
ENDP


;*****
;***** putLine
;*****

PROC    C       putLine_scr_copy
	ARG	lineNum:WORD, xOff:WORD, lineLen:WORD, buf:DATAPTR
  push	ds si di
  cld
  mov   ax,0A000h
  mov   es,ax
  xor   di,di
  lds	si,[buf]

  mov	bx,[lineNum]
  shl	bx,1
  add	di,[cs:lineOffs+bx]
  add	di,[xOff]
  mov	cx,[lineLen]
  rep   movsb

  pop	di si ds
  leave
  retf
ENDP
;PROC	C	putLine_scr_copy
;	ARG	lineNum:WORD, xOff:WORD, lineLen:WORD, buf:DWORD
;  push	ds si di
;
;  cld
;  mov	ax,0A000h
;  mov	es,ax
;  lds	si,[buf]
;
;  mov	bx,[lineNum]
;  shl	bx,1
;  mov	di,[lineOffs+bx]
;  add	di,[xOff]
;  mov	cx,[lineLen]
;
;  mov   dx,cx                           ; DX = CX
;  shr	cx,2				; CX = line length in dwords
;  rep	movsd				; copy the dwords
;  mov	cx,dx				; CX = line length in bytes
;  and	cx,0000000000000011b		; CX = number of residual bytes
;  rep	movsb				; copy the residual bytes, if any
;
;  pop	di si ds			; restore registers
;  leave					; clean up call stack
;  retf					; return
;ENDP

PROC	C	putLine_mem_copy
	ARG	y:WORD, xOff:WORD, lineLen:WORD, buf:DWORD
  push	ds si di			; save these registers

  xor	eax,eax				; clear EAX
  xor	edx,edx				; clear EDX
  xor	edi,edi				; clear EDI

  les	di,[outAddr]			; load output address
  mov	ax,[y]
  mov	dx,[outScreenWide]
  mul	edx			    	; EDX:EAX = offset - EDI - x
  add	edi,eax				; EDI = offset - x
  mov	dx,[xOff]
  add	edi,edx				; EDI = offset
  mov	edx,edi				; EDX = offset
  shr	edx,4				; EDX = # of paragraphs (segments)
  add	dx,[outSeg]			; DX = new segment
  mov	es,dx				; ES = new segment
  and	di,0000000000001111b	     	; DI = offset within new segment

  lds	si,[buf]			; load input address
  mov	dx,si				; DX = offset
  shr	dx,4				; DX = # of paragraphs (segments)
  mov	ax,ds				; AX = DS
  add	ax,dx				; AX = new DS
  mov	ds,ax				; DS = new DS
  and	si,0000000000001111b		; SI = offset within new segment

  cld
  mov	cx,[lineLen]			; CX = line length
  mov	dx,cx				; DX = line length
  shr	cx,2				; CX = line length in dwords
  rep	movsd			     	; move the dwords if necessary
  mov	cx,dx				; CX = line length in bytes
  and	cx,0000000000000011b		; CX = any residual bytes
  rep	movsb				; move the bytes if necessary

  pop	di si ds			; restore registers
  leave					; clean up
  retf					; return
ENDP


;*****
;***** putLineInv
;*****

PROC	C	putLineInv_mem_copy
	ARG	y:WORD, xOff:WORD, lineLen:WORD, buf:DWORD
  push	ds si di			; save these registers

  xor	eax,eax				; clear EAX
  xor	edx,edx				; clear EDX
  xor	edi,edi				; clear EDI

  les	di,[outAddr]			; load output address
  mov	ax,[y]
  mov	dx,[outScreenWide]
  mul	edx			    	; EDX:EAX = offset - EDI - x
  add	edi,eax				; EDI = offset - x
  mov	dx,[xOff]
  add	edi,edx				; EDI = offset
  mov	edx,edi				; EDX = offset
  shr	edx,4				; EDX = # of paragraphs (segments)
  add	dx,[outSeg]			; DX = new segment
  mov	es,dx				; ES = new segment
  and	di,0000000000001111b	     	; DI = offset within new segment

  lds	si,[buf]			; load input address
  mov	dx,si				; DX = offset
  shr	dx,4				; DX = # of paragraphs (segments)
  mov	ax,ds				; AX = DS
  add	ax,dx				; AX = new DS
  mov	ds,ax				; DS = new DS
  and	si,0000000000001111b		; SI = offset within new segment

  cld
  mov	cx,[lineLen]			; CX = line length

	@@loopStart:
  lodsb					; grab a pixel
  or	al,al				; is it a zero pixel?
  jz	@@invisible			; yes, don't copy it
  stosb					; no, copy it ...
  loop	@@loopStart			; ... and start loop again
  jmp	short	@@loopDone		; quit loop when completed
	@@invisible:
  inc	di				; skip past pixel ...
  loop	@@loopStart			; ... and start loop again

	@@loopDone:
  pop	di si ds			; restore registers
  leave					; clean up
  retf					; return
ENDP


;*****
;***** getLine
;*****

PROC	C	getLine_scr
	ARG	y:WORD, xOff:WORD, lineLen:WORD, buf:DWORD
  cld
  push	ds si di
  les	di,[buf]
  lds	si,[inAddr]

  mov	bx,[lineNum]
  shl	bx,1
  add	si,[lineOffs+bx]
  add	si,[xOff]
  mov	cx,[lineLen]

  mov	dx,cx				; DX = CX
  shr	cx,2				; CX = line length in dwords
  rep	movsd				; copy the dwords
  mov	cx,dx				; CX = line length in bytes
  and	cx,0000000000000011b		; CX = number of residual bytes
  rep	movsb				; copy the residual bytes, if any

  pop	di si ds			; restore registers
  leave					; clean up call stack
  retf					; return
ENDP

PROC	C	getLine_mem
	ARG	y:WORD, xOff:WORD, lineLen:WORD, buf:DWORD
  push	ds si di			; save these registers

  xor	eax,eax				; clear EAX
  xor	edx,edx				; clear EDX
  xor	esi,esi				; clear ESI

  les	si,[inAddr]			; load input address
  mov	ax,[y]
  mov	dx,[inScreenWide]
  mul	edx			    	; EDX:EAX = offset - ESI - x
  add	esi,eax				; ESI = offset - x
  mov	dx,[xOff]
  add	esi,edx				; ESI = offset
  mov	edx,esi				; EDX = offset
  shr	edx,4				; EDX = # of paragraphs (segments)
  add	dx,[inSeg]			; DX = new segment
  mov	ds,dx				; ES = new segment
  and	si,0000000000001111b	     	; SI = offset within new segment

  les	di,[buf]			; load output address
  mov	dx,di				; DX = offset
  shr	dx,4				; DX = # of paragraphs (segments)
  mov	ax,es				; AX = ES
  add	ax,dx				; AX = new ES
  mov	es,ax				; ES = new ES
  and	di,0000000000001111b		; DI = offset within new segment

  cld
  mov	cx,[lineLen]			; CX = line length
  mov	dx,cx				; DX = line length
  shr	cx,2				; CX = line length in dwords
  rep	movsd			     	; move the dwords if necessary
  mov	cx,dx				; CX = line length in bytes
  and	cx,0000000000000011b		; CX = any residual bytes
  rep	movsb				; move the bytes if necessary

  pop	di si ds			; restore registers
  leave					; clean up
  retf					; return
ENDP


;*****
;***** putPixel
;*****

; Copy a pixel to the screen
PROC	C	putPixel_scr_copy
	ARG	x:WORD,y:WORD,colour:BYTE
  mov   ax,0A000h
  mov	es,ax
  mov	bx,[y]
  shl	bx,1
  mov	bx,[lineOffs+bx]
  add	bx,[x]
  mov	al,[colour]
  mov	[es:bx],al
  leave
  retf
ENDP

; AND a pixel to the screen
PROC	C	putPixel_scr_and
	ARG	x:WORD,y:WORD,colour:BYTE
  mov   ax,0A000h
  mov	es,ax
  mov	bx,[y]
  shl	bx,1
  mov	bx,[lineOffs+bx]
  add	bx,[x]
  mov	al,[es:bx]
  and	al,[colour]
  mov	[es:bx],al
  leave
  retf
ENDP

; NOT a pixel to the screen
PROC	C	putPixel_scr_not
	ARG	x:WORD,y:WORD,colour:BYTE
  mov   ax,0A000h
  mov	es,ax
  mov	bx,[y]
  shl	bx,1
  mov	bx,[lineOffs+bx]
  add	bx,[x]
  mov	al,[colour]
  not	al
  mov	[es:bx],al
  leave
  retf
ENDP

; OR a pixel to the screen
PROC	C	putPixel_scr_or
	ARG	x:WORD,y:WORD,colour:BYTE
  mov   ax,0A000h
  mov	es,ax
  mov	bx,[y]
  shl	bx,1
  mov	bx,[lineOffs+bx]
  add	bx,[x]
  mov	al,[es:bx]
  or	al,[colour]
  mov	[es:bx],al
  leave
  retf
ENDP

; XOR a pixel to the screen
PROC	C	putPixel_scr_xor
	ARG	x:WORD,y:WORD,colour:BYTE
  mov   ax,0A000h
  mov	es,ax
  mov	bx,[y]
  shl	bx,1
  mov	bx,[lineOffs+bx]
  add	bx,[x]
  mov	al,[es:bx]
  xor	al,[colour]
  mov	[es:bx],al
  leave
  retf
ENDP

; Copy a pixel to memory
PROC	C	putPixel_mem_copy
	ARG	x:WORD,y:WORD,colour:BYTE
  xor	eax,eax				; clear EAX
  xor	ebx,ebx				; clear EBX
  xor	edx,edx				; clear EDX

  les	bx,[outAddr]			; load output address
  mov	ax,[y]
  mov	dx,[outScreenWide]
  mul	edx			    	; EDX:EAX = offset - EBX - x
  add	ebx,eax				; EBX = offset - x
  mov	dx,[x]
  add	ebx,edx				; EBX = offset

  mov	edx,ebx				; EDX = offset
  shr	edx,4				; EDX = # of paragraphs (segments)
  add	dx,[outSeg]			; DX = new segment
  mov	es,dx				; ES = new segment
  and	bx,0000000000001111b	     	; BX = offset within new segment

  mov	al,[colour]			; colour in AL
  mov	[es:bx],al			; store pixel
  leave					; clean up
  retf					; return
ENDP

; AND a pixel to memory
PROC	C	putPixel_mem_and
	ARG	x:WORD,y:WORD,colour:BYTE
  xor	eax,eax				; clear EAX
  xor	ebx,ebx				; clear EBX
  xor	edx,edx				; clear EDX

  les	bx,[outAddr]			; load output address
  mov	ax,[y]
  mov	dx,[outScreenWide]
  mul	edx			    	; EDX:EAX = offset - EBX - x
  add	ebx,eax				; EBX = offset - x
  mov	dx,[x]
  add	ebx,edx				; EBX = offset

  mov	edx,ebx				; EDX = offset
  shr	edx,4				; EDX = # of paragraphs (segments)
  add	dx,[outSeg]			; DX = new segment
  mov	es,dx				; ES = new segment
  and	bx,0000000000001111b	     	; BX = offset within new segment

  mov	al,[colour]			; colour in AL
  and	[es:bx],al			; AND pixel
  leave					; clean up
  retf					; return
ENDP

; NOT a pixel to memory
PROC	C	putPixel_mem_not
	ARG	x:WORD,y:WORD,colour:BYTE
  xor	eax,eax				; clear EAX
  xor	ebx,ebx				; clear EBX
  xor	edx,edx				; clear EDX

  les	bx,[outAddr]			; load output address
  mov	ax,[y]
  mov	dx,[outScreenWide]
  mul	edx			    	; EDX:EAX = offset - EBX - x
  add	ebx,eax				; EBX = offset - x
  mov	dx,[x]
  add	ebx,edx				; EBX = offset

  mov	edx,ebx				; EDX = offset
  shr	edx,4				; EDX = # of paragraphs (segments)
  add	dx,[outSeg]			; DX = new segment
  mov	es,dx				; ES = new segment
  and	bx,0000000000001111b	     	; BX = offset within new segment

  mov	al,[colour]			; colour in AL
  not	al				; NOT it
  mov	[es:bx],al			; store NOTed pixel
  leave					; clean up
  retf					; return
ENDP

; OR a pixel to memory
PROC	C	putPixel_mem_or
	ARG	x:WORD,y:WORD,colour:BYTE
  xor	eax,eax				; clear EAX
  xor	ebx,ebx				; clear EBX
  xor	edx,edx				; clear EDX

  les	bx,[outAddr]			; load output address
  mov	ax,[y]
  mov	dx,[outScreenWide]
  mul	edx			    	; EDX:EAX = offset - EBX - x
  add	ebx,eax				; EBX = offset - x
  mov	dx,[x]
  add	ebx,edx				; EBX = offset

  mov	edx,ebx				; EDX = offset
  shr	edx,4				; EDX = # of paragraphs (segments)
  add	dx,[outSeg]			; DX = new segment
  mov	es,dx				; ES = new segment
  and	bx,0000000000001111b	     	; BX = offset within new segment

  mov	al,[colour]			; colour in AL
  or	[es:bx],al			; OR pixel
  leave					; clean up
  retf					; return
ENDP

; XOR a pixel to memory
PROC	C	putPixel_mem_xor
	ARG	x:WORD,y:WORD,colour:BYTE
  xor	eax,eax				; clear EAX
  xor	ebx,ebx				; clear EBX
  xor	edx,edx				; clear EDX

  les	bx,[outAddr]			; load output address
  mov	ax,[y]
  mov	dx,[outScreenWide]
  mul	edx			    	; EDX:EAX = offset - EBX - x
  add	ebx,eax				; EBX = offset - x
  mov	dx,[x]
  add	ebx,edx				; EBX = offset

  mov	edx,ebx				; EDX = offset
  shr	edx,4				; EDX = # of paragraphs (segments)
  add	dx,[outSeg]			; DX = new segment
  mov	es,dx				; ES = new segment
  and	bx,0000000000001111b	     	; BX = offset within new segment

  mov	al,[colour]			; colour in AL
  xor	[es:bx],al			; XOR pixel
  leave					; clean up
  retf					; return
ENDP


;*****
;***** getPixel
;*****

; Get a pixel from the screen
PROC	C	getPixel_scr
	ARG	x:WORD,y:WORD
  mov	ax,0A000h
  mov	es,ax
  mov	bx,[y]
  shl	bx,1
  mov	bx,[lineOffs+bx]
  add	bx,[x]
  mov	al,[es:bx]
  xor	ah,ah
  leave
  retf
ENDP

; Get a pixel from memory
PROC	C	getPixel_mem
	ARG	x:WORD,y:WORD
  xor	eax,eax				; clear EAX
  xor	ebx,ebx				; clear EBX
  xor	edx,edx				; clear EDX

  les	bx,[inAddr]			; load input address
  mov	ax,[y]
  mov	dx,[inScreenWide]
  mul	edx			    	; EDX:EAX = offset - EBX - x
  add	ebx,eax				; EBX = offset - x
  mov	dx,[x]
  add	ebx,edx				; EBX = offset

  mov	edx,ebx				; EDX = offset
  shr	edx,4				; EDX = # of paragraphs (segments)
  add	dx,[inSeg]			; DX = new segment
  mov	es,dx				; ES = new segment
  and	bx,0000000000001111b	     	; BX = offset within new segment

  xor	ax,ax				; clear AX
  mov	al,[es:bx]			; store pixel in AX for return
  leave					; clean up
  retf					; return
ENDP


;*****
;***** line
;*****

; The following line routine is a modified version of code in Richard
; Wilton's book "Programmer's Guide to PC and PS/2 Video Systems".
SCREENSEG	EQU	0A000h

MACRO	PIXELADDR
  xchg	ah,al			; ax=256*y
  add	bx,ax			; bx=256*y+x
  shr	ax,2
  add	bx,ax			; bx=320*y+x (+ offset in buffer if needed.)
  mov	ax,SCREENSEG
  mov	es,ax			; es:bx=address of pixel
ENDM

	MASM

; void line(x1,y1,x2,y2,colour)
; int x1,y1,x2,y2;
; unsigned char colour;
ARGx1		EQU	word ptr [bp+6]
ARGy1		EQU	word ptr [bp+8]
ARGx2		EQU	word ptr [bp+10]
ARGy2		EQU	word ptr [bp+12]
ARGcolour	EQU	byte ptr [bp+14]
VARincr1	EQU	word ptr [bp-2]
VARincr2	EQU	word ptr [bp-4]
VARroutine	EQU	word ptr [bp-6]
line	PROC	FAR
	push	bp
        mov	bp,sp
        sub	sp,6
        push	si
        push	di

; check for vertical lines
	mov	si,320				; initial y-increment
        mov	cx,ARGx2
	sub	cx,ARGx1			; cx=x2-x1
	jz	vertLine

; force x1 < x2
	jns	L01				; jump if x1<x2
        neg	cx				; cx=x1-x2
	mov	bx,ARGx2			; exchange x1 and x2
        xchg	bx,ARGx1
	mov	ARGx2,bx
        mov	bx,ARGy2			; exchange y1 and y2
        xchg	bx,ARGy1
        mov	ARGy2,bx

; calculate dy=abs(y2-y1)
L01:	mov	bx,ARGy2
        sub	bx,ARGy1			; bx=y2-y1
	jz	@@horizLine
	jns	L03				; jump if slope is positive
	neg	bx				; bx=y1-y2
        neg	si				; negate y-increment

; select appropriate routine for slope of line
L03:	push	si
	mov	VARroutine,offset loSlopeLine
        cmp	bx,cx
        jle	L04				; jump if dy<=dx (slope <=1)
	mov     VARroutine,offset hiSlopeLine
        xchg	bx,cx				; exchange dy and dx

; calculate initial decision variable and increments
L04:	shl	bx,1				; bx=2*dy
	mov	VARincr1,bx			; incr1=2*dy
        sub	bx,cx
        mov	si,bx				; si=d=2*dy-dx
        sub	bx,cx
        mov	VARincr2,bx			; incr2=2*(dy-dx)

; calculate first pixel address
	push	cx
        mov	ax,ARGy1
	mov	bx,ARGx1
	PIXELADDR
        mov	di,bx
	pop	cx
        inc	cx				; cx=# of pixels to draw
        pop	bx				; bx=y-increment
	jmp	VARroutine			; jump to appropriate routine

; routine for vertical lines
vertLine:	mov	ax,ARGy1
		mov	bx,ARGy2
                mov	cx,bx
                sub	cx,ax			; cx=dy
                jge	L31			; jump if dy>=0
                neg	cx			; force dy>=0
                mov	ax,bx			; ax=y2
L31:	inc	cx				; cx=# of pixels to draw
	mov	bx,ARGx1			; bx=x
	PIXELADDR				; es:bx
        mov	di,bx				; es:di
	dec	si				; si=bytes/line-1
	mov	al,ARGcolour

L32:	stosb
	add	di,si				; next line
        loop	L32
	jmp	short	Lexit

; routine for horizontal lines (slope=0)
@@horizLine:	push	cx
		mov	ax,ARGy1
                mov	bx,ARGx1
                PIXELADDR			; es:bx
                mov	di,bx			; es:di
                pop	cx
                inc	cx			; cx=# of pixels to draw
                mov	al,ARGcolour
		rep	stosb
		jmp	short	Lexit

; routine for dy<=dx (slope<=1)			; es:di -> video buffer
						; bx=y-increment
                                                ; cx =# of pixels to draw
						; si=decision variable
loSlopeLine:    mov	al,ARGcolour

L11:	stosb					; put pixel, increment x
	or	si,si				; test sign of d
	jns	L12				; jump id d>=0
        add	si,VARincr1			; d=d+incr1
        loop	L11
        jmp	short	Lexit

L12:    add	si,VARincr2			; d=d+incr2
	add	di,bx				; increment y
        loop	L11
        jmp	short	Lexit

; routine for dy>dx (slope >1)			; es:di -> video buffer
						; bx=y-increment
						; cx=# of pixels to draw
						; si=decision variable
hiSlopeLine:    mov	al,ARGcolour

L21:	stosb					; put pixel, increment x
	add	di,bx				; increment y

L22:	or	si,si				; test sign of d
	jns	L23				; jump if d>=0
	add	si,VARincr1			; d=d+incr1
	dec	di				; dec x (inc by stosb)
	loop	L21
	jmp	short	Lexit

L23:    add	si,VARincr2			; d=d+incr2
	loop	L21

Lexit:	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret
line	ENDP

	IDEAL


;*****
;***** horizLine
;*****

PROC	C	horizLine_scr_copy
	ARG	y:WORD, x1:WORD, x2:WORD, colour:BYTE
  push	di

  les	di,[outAddr]			; load output address
  mov	bx,[y]				; BX = y-coordinate
  mov	ax,[x1]				; CX = # of pixels
  mov	cx,[x2]
  sub	cx,ax
  inc	cx

  shl	bx,1				; DI = output address
  add	di,[lineOffs+bx]
  add	di,ax

  mov	al,[colour]			; AL = colour
  mov	ah,al				; AH = colour
  mov	dx,ax				; DX = colour:colour
  shl	eax,16				; EAX = colour:colour:?:?
  mov	ax,dx				; EAX = colour:colour:colour:colour
  mov	dx,cx				; DX = CX
  shr	cx,2				; CX = line length in dwords
  rep	stosd				; store the dwords
  mov	cx,dx				; CX = line length in bytes
  and	cx,0000000000000011b		; CX = number of residual bytes
  rep	stosb				; store the residual bytes, if any

  pop	di				; restore DI
  leave					; clean up the stack
  retf					; return
ENDP

PROC	C	horizLine_mem_copy
	ARG	y:WORD, x1:WORD, x2:WORD, colour:BYTE
  push	edi				; store EDI

  xor	eax,eax				; clear EAX
  xor	edi,edi				; clear EDI
  xor	edx,edx				; clear EDX

  les	di,[outAddr]			; load output address
  mov	ax,[y]
  mov	dx,[outScreenWide]
  mul	edx			    	; EDX:EAX = offset - EDI - x
  add	edi,eax				; EDI = offset - x
  mov	dx,[x1]
  add	edi,edx				; EDI = offset
  mov	edx,edi				; EDX = offset
  shr	edx,4				; EDX = # of paragraphs (segments)
  add	dx,[outSeg]			; DX = new segment
  mov	es,dx				; ES = new segment
  and	di,0000000000001111b	     	; DI = offset within new segment

  mov	al,[colour]			; colour in AL
  mov	ah,al				; colour in AH
  mov	bx,ax
  shl	eax,16
  mov	ax,bx 				; colour in each byte of EAX

  mov	cx,[x2]
  sub	cx,[x1]
  inc	cx				; CX = line length in pixels
  mov	dx,cx				; DX = line length in bytes

  shr	cx,2				; CX = line length in dwords
  rep	stosd				; store four bytes at a time
  mov	cx,dx				; CX = line length in pixels
  and	cx,0000000000000011b		; CX = any remaining bytes
  rep	stosb				; store the remaining bytes

  pop	edi			    	; restore EDI
  leave					; clean up
  retf					; return
ENDP


;*****
;***** filledRect
;*****

PROC	C	filledRect
	ARG	x1:WORD, y1:WORD, x2:WORD, y2:WORD, colour:BYTE
  push	di si
  les	di,[outAddr]

  mov	ax,[y1]		; y coords
  mov	bx,[y2]
  cmp	ax,bx
  jbe	@@L1
  xchg	ax,bx

	@@L1:
  sub	bx,ax				; ySize in BX
  inc	bx
  mov	si,320				; update DI
  mul	si
  add	di,ax

  mov	dx,[x1]		; x coords
  mov	cx,[x2]
  cmp	dx,cx
  jbe	@@L2
  xchg	cx,dx

	@@L2:
  add	di,dx

  sub	cx,dx				; xSize in CX
  inc	cx

  sub	si,cx				; xAdd in SI

  push	dx				; store DX
  mov	al,[colour]			; AL = colour
  mov	ah,al				; AH = colour
  mov	dx,ax				; DX = colour:colour
  shl	eax,16				; EAX = colour:colour:?:?
  mov	ax,dx				; EAX = colour:colour:colour:colour
  pop	dx				; restore DX

  cld					; clear direction flag
  mov	dx,cx				; BX = CX
  jmp	short	@@LoopB			; jump to loop entry point
	@@LoopA:
  mov	cx,dx				; CX = line length in bytes
  add	di,si				; update DI
	@@LoopB:
  push	cx				; store CX
  shr	cx,2				; CX = line length in dwords
  rep	stosd				; store the dwords
  pop	cx				; restore CX
  and	cx,0000000000000011b		; CX = number of residual bytes
  rep	stosb				; store the residual bytes, if any
  dec	bx				; decrement loop counter
  jnz	@@LoopA				; restart loop if necesary

  pop	si di				; restore registers
  leave				       	; clean up call stack
  retf					; return
ENDP


;*****
;***** setPaletteReg
;*****

PROC	C	setPaletteReg
	ARG	palNum:WORD,red:BYTE,green:BYTE,blue:BYTE
  mov	bx,[palNum]

  mov	dh,[red]			; red
  shr	dh,2
  jnc	@@L1
  cmp	dh,63
  je	@@L1
  inc	dh

	@@L1:
  mov	ch,[green]			; green
  shr	ch,2
  jnc	@@L2
  cmp	ch,63
  je	@@L2
  inc	ch

	@@L2:
  mov	cl,[blue]			; blue
  shr	cl,2
  jnc	@@L3
  cmp	cl,63
  je	@@L3
  inc	cl

	@@L3:
  mov	ax,1010h
  int	10h
  leave
  retf
ENDP


;*****
;***** getPaletteReg
;*****

PROC	C	getPaletteReg
	ARG	palNum:WORD,red:DWORD,green:DWORD,blue:DWORD
  push	ds si
  mov	ax,1015h
  mov	bx,[palNum]
  int	10h
  lds	si,[red]
  shl	dh,2
  mov	[ds:si],dh
  lds	si,[green]
  shl	ch,2
  mov	[ds:si],ch
  lds	si,[blue]
  shl	cl,2
  mov	[ds:si],cl
  pop	si ds
  leave
  retf
ENDP


;*****
;***** setBlockPalette
;*****

PROC	C	setBlockPalette
	ARG	firstReg:WORD,lastReg:WORD,paletteData:DWORD
  push	ds si di
  lds	si,[paletteData]
  push	cs
  pop	es
  mov	di,OFFSET tempPalette
  mov	cx,[lastReg]
  inc	cx
  push	cx
  cld

	@@LLoop:
  lodsb			; red
  shr	al,2
  jnc	@@L1
  cmp	al,63
  je	@@L1
  inc	al
	@@L1:
  stosb
  lodsb			; green
  shr	al,2
  jnc	@@L2
  cmp	al,63
  je	@@L2
  inc	al
	@@L2:
  stosb
  lodsb			; blue
  shr	al,2
  jnc	@@L3
  cmp	al,63
  je	@@L3
  inc	al
	@@L3:
  stosb
  loop	@@LLoop

  mov	ax,1012h
  mov	bx,[firstReg]			; first colour reg
  pop	cx				; # of colour regs
  mov	dx,OFFSET tempPalette		; ES = CS
  int	10h

	@@LExit:
  pop	di si ds
  leave
  retf
ENDP


;*****
;***** getBlockPalette
;*****

PROC	C	getBlockPalette
	ARG	firstReg:WORD,lastReg:WORD,paletteData:DWORD
  push	ds si di
  mov	ax,1017h
  mov	bx,[firstReg]
  mov	cx,[lastReg]
  inc	cx
  push	cs
  pop	es
  mov	dx,OFFSET tempPalette
  int	10h

  les	di,[paletteData]
  mov	ax,cs
  mov	ds,ax
  mov	si,OFFSET tempPalette
  mov	cx,[lastReg]
  inc	cx
  cld

	@@L1:
  lodsb
  shl	al,2
  stosb		; red
  lodsb
  shl	al,2
  stosb		; green
  lodsb
  shl	al,2
  stosb		; blue
  loop	@@L1

	@@LExit:
  pop	di si ds
  leave
  retf
ENDP


;*****
;***** clearGraphics
;*****

PROC	C	clearGraphics
	ARG	colour:BYTE
  push	di
  mov	ax,0A000h			; point ES:DI to screen
  mov	es,ax
  xor	di,di
  mov	al,[colour]			; EAX = colour:colour:colour:colour
  mov	ah,al
  mov	bx,ax
  shl	eax,16
  mov	ax,bx
  mov	cx,16000			; clear the screen
  cld
  rep	stosd
  pop	di
  leave
  retf
ENDP

ENDS
END
