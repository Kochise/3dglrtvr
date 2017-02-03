; 320x400x256 (requires register-compatible VGA+)
; Loadable driver for The Graphics Engine
; Copyright (c) 1993 by Matthew Hildebrand
; Turbo Assembler syntax

IDEAL
MODEL LARGE TGE320X400_TEXT
P386N


SCREEN_WIDE	=	320
SCREEN_DEEP	=	400
NUM_COLOURS	=	256


		CODESEG TGE320X400_TEXT
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
_getImage_scr		dw	0			; getImage
			dw	?
_getImage_mem		dw	0
			dw	?
_getLine_scr		dw      getLine_scr	      	; getLine
			dw	?
_getLine_mem		dw	getLine_mem
			dw	?
_getPixel_scr		dw	getPixel_scr		; getPixel
			dw	?
_getPixel_mem		dw	getPixel_mem
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
_putLine_scr_copy      	dw	putLine_scr_copy	; putLine
			dw	?
_putLine_scr_and      	dw      0
			dw	?
_putLine_scr_not      	dw      0
			dw	?
_putLine_scr_or      	dw      0
			dw	?
_putLine_scr_xor      	dw	0
			dw	?
_putLine_mem_copy	dw      putLine_mem_copy
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
_putPixel_mem_not	dw      putPixel_mem_not
			dw	?
_putPixel_mem_or	dw      putPixel_mem_or
			dw	?
_putPixel_mem_xor	dw	putPixel_mem_xor
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
_horizLine_scr_copy	dw	horizLine_scr_copy  	; horizLine
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
_clearGraphics_scr_copy	dw	clearGraphics_scr_copy	; clearGraphics
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
scrnMaxY		dw	399
maxColour		dw	255	; maximum colour number
xRatio		       	dw	4	; aspect ratio 4:5 (320:400 in
yRatio			dw	5	;   lowest terms)
bitsPerPixel		dw	8	; 8 bits per pixel
inMaxX			dw	319	; current input screen dimensions
inMaxY			dw	399
outMaxX			dw	319	; current output screen dimensions
outMaxY			dw	399
inScreenWide		dw	?	; needed only for virtual screens
outScreenWide		dw	?
;*** Viewport information
inViewportULX		dw	0
inViewportULY		dw	0
inViewportLRX		dw	319
inViewportLRY		dw	399
outViewportULX		dw	0
outViewportULY		dw	0
outViewportLRX		dw	319
outViewportLRY		dw	399
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


inited		db	0
colourPalette	db	768	DUP(?)
lineOffs	dw	400	DUP(?)

; Plane masks for horizLine
leftEdgeMask	db	1111b, 1110b, 1100b, 1000b
rightEdgeMask	db	0001b, 0011b, 0111b, 1111b


SC_INDEX	EQU	3C4h    ; Sequence Controller Index
GC_INDEX	EQU	3CEh	; Graphics Controller Index register
CRTC_INDEX	EQU	3D4h    ; CRT Controller Index
MISC_OUTPUT	EQU	3C2h    ; Miscellaneous Output register
MAP_MASK	EQU     02h     ; index in SC of Map Mask register
READ_MAP	EQU	4	; Read Map register index in GC
MEMORY_MODE	EQU	4	; Memory mode register in SC
GRAPHICS_MODE	EQU	5	; Graphics Mode register index in GC
MISCELLANEOUS	EQU	6	; Miscellaneous register index in GC
MAX_SCAN_LINE	EQU	9      	; Maximum Scan Line reg index in CRTC
START_ADDR_HIGH	EQU	0Ch	; Start Address High reg index in CRTC
UNDERLINE	EQU	14h	; Underline Location reg index in CRTC
MODE_CONTROL	EQU	17h	; Mode Control register index in CRTC
INPUT_STATUS_1  EQU     03DAh   ; Input Status 1 register
START_ADDR_HIGH EQU	0Ch     ; start address high byte
START_ADDR_LOW  EQU	0Dh     ; start address low byte
WORD_OUTS_OK	EQU	1	; Set to 0 for VGAs that can't handle
				; word OUTs to indexed VGA registers


MACRO	OUT_WORD
if WORD_OUTS_OK
  out	dx,ax
else
  out	dx,al
  inc	dx
  xchg	ah,al
  out	dx,al
  dec	dx
  xchg	ah,al
endif
ENDM

MACRO	CONSTANT_TO_INDEXED_REG ADDRESS, INDEX, VALUE
  mov	dx,ADDRESS
  mov	ax,(VALUE shl 8) + INDEX
  OUT_WORD
ENDM


;*****
;***** initGraphics
;*****

PROC	C	initGraphics
  mov	ax,0013h			; set mode 13h
  int	10h

  ; change CPU addressing to linear
  mov	dx,SC_INDEX
  mov	al,MEMORY_MODE
  out	dx,al
  inc	dx
  in	al,dx
  and	al,NOT 08h			; turn off chain 4
  or 	al,04h				; turn off odd/even
  out	dx,al
  mov	dx,GC_INDEX
  mov	al,GRAPHICS_MODE
  out	dx,al
  inc	dx
  in	al,dx
  and	al,NOT 10h			; turn off odd/even
  out	dx,al
  dec	dx
  mov	al,MISCELLANEOUS
  out	dx,al
  inc	dx
  in	al,dx
  and	al,NOT 02h			; turn off chain
  out	dx,al

  ; clear all 256k of video RAM
  CONSTANT_TO_INDEXED_REG SC_INDEX, MAP_MASK, 0Fh	; enable writes to all planes
  mov	ax,0A000h
  mov	es,ax
  xor	di,di
  mov	ax,di
  mov	cx,8000h			; # of words in 64k
  cld
  rep	stosw				; clear all of display RAM

  ; Tweak the mode to 320x400 by not scanning each line twice
  mov	dx,CRTC_INDEX
  mov	al,MAX_SCAN_LINE
  out	dx,al
  inc	dx
  in	al,dx
  and	al,NOT 1Fh			; set max scan line to 0
  out	dx,al
  dec	dx

  ; Change CRTC scanning from DWORD to BYTE mode, allowing the CRTC to scan
  ; more than 64k of video data.
  mov	al,UNDERLINE
  out	dx,al
  inc	dx
  in	al,dx
  and	al,NOT 40h			; turn off doubleword
  out	dx,al
  dec	dx
  mov	al,MODE_CONTROL
  out	dx,al
  inc	dx
  in	al,dx
  or	al,40h				; turn on byte mode bit
  out	dx,al

  mov	al,[inited]			; restore palette if necessary
  or	al,al
  jz	@@notInited
  mov	ax,1
  retf

	@@notInited:
  mov	[inited],1
  push	si
  mov	si,OFFSET lineOffs
  mov	cx,SCREEN_DEEP
  xor	bx,bx

	@@LLoop:
  mov	ax,SCREEN_WIDE/4
  mul	bx
  mov	[cs:si],ax
  add	si,2
  inc	bx
  loop	@@LLoop
  pop	si
  mov	ax,1
  retf
ENDP


;*****
;***** putLine
;*****

PROC	C	putLine_scr_copy
	ARG	lineNum:WORD, xOff:WORD, lineLen:WORD, buf:DWORD
	LOCAL	x2:WORD
  push	ds si di

  mov	bx,[lineNum]			; start address in ES:DI
  shl	bx,1
  mov	di,[lineOffs+bx]
  mov	dx,[xOff]
  shr	dx,2
  add	di,dx
  mov	ax,0A000h
  mov	es,ax
  lds	si,[buf]			; source in DS:SI
  cld

  mov	bx,[xOff]
  add	bx,[lineLen]			; calculate x2
  dec	bx
  mov	[x2],bx

  mov	cx,[lineLen]			; line is short
  cmp	cx,4
JUMPS
  jle	@@ShortLine
NOJUMPS

  mov	cx,[xOff]			; calculate starting plane
  mov	bx,cx
  mov	ax,0102h
  and	cl,3
  shl	ah,cl
  mov	dx,SC_INDEX
  OUT_WORD
  push	si
  mov	bx,[x2]
  mov	cx,[xOff]
	@@Loop1:
  cmp	cx,bx
  jg	@@Loop1Done
  lodsb
  stosb
  add	cx,4
  add	si,3
  jmp	short	@@Loop1

	@@Loop1Done:
  pop	si
  inc	si
  push	si
  mov	bx,[xOff]
  inc	bx
  mov	cx,bx
  mov	ax,0102h
  and	cl,3
  shl	ah,cl
  mov	dx,SC_INDEX
  OUT_WORD
  mov	cx,bx
  mov	bx,[lineNum]			; start address in ES:DI
  shl	bx,1
  mov	di,[lineOffs+bx]
  shr	cx,2
  add	di,cx
  mov	bx,[x2]
  mov	cx,[xOff]
  inc	cx
	@@Loop2:
  cmp	cx,bx
  jg	@@Loop2Done
  lodsb
  stosb
  add	cx,4
  add	si,3
  jmp	short	@@Loop2

	@@Loop2Done:
  pop	si
  inc	si
  push	si
  mov	bx,[xOff]
  add	bx,2
  mov	cx,bx
  mov	ax,0102h
  and	cl,3
  shl	ah,cl
  mov	dx,SC_INDEX
  OUT_WORD
  mov	cx,bx
  mov	bx,[lineNum]
  shl	bx,1
  mov	di,[lineOffs+bx]
  shr	cx,2
  add	di,cx
  mov	bx,[x2]
  mov	cx,[xOff]
  add	cx,2
	@@Loop3:
  cmp	cx,bx
  jg	@@Loop3Done
  lodsb
  stosb
  add	cx,4
  add	si,3
  jmp	short	@@Loop3

	@@Loop3Done:
  pop	si
  inc	si
  mov	bx,[xOff]
  add	bx,3
  mov	cx,bx
  mov	ax,0102h
  and	cl,3
  shl	ah,cl
  mov	dx,SC_INDEX
  OUT_WORD
  mov	cx,bx
  mov	bx,[lineNum]
  shl	bx,1
  mov	di,[lineOffs+bx]
  shr	cx,2
  add	di,cx
  mov	bx,[x2]
  mov	cx,[xOff]
  add	cx,3
	@@Loop4:
  cmp	cx,bx
  jg	@@Exit
  lodsb
  stosb
  add	cx,4
  add	si,3
  jmp	short	@@Loop4

	@@ShortLine:
  mov	cx,[xOff]
  mov	bx,cx
  and	cl,3
  mov	ax,0102h
  shl	ah,cl
  mov	dx,SC_INDEX
  OUT_WORD
  mov	cx,bx
  shr	bx,2
  mov	di,bx
  mov	bx,[lineNum]
  shl	bx,1
  add	di,[lineOffs+bx]
  movsb
  cmp	cx,[x2]
  jge	@@Exit
  inc	[xOff]
  jmp	short	@@ShortLine

	@@Exit:
  pop   di si ds
  leave
  retf
ENDP

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
	ARG	lineNum:WORD, xOff:WORD, lineLen:WORD, buf:DWORD
	LOCAL	x2:WORD
  push	ds si di

  mov	bx,[lineNum]			; start address in ES:DI
  shl	bx,1
  mov	si,[lineOffs+bx]
  mov	dx,[xOff]
  shr	dx,2
  add	si,dx
  mov	ax,0A000h
  mov	ds,ax
  les	di,[buf]			; buffer addr in DS:SI
  cld

  mov	bx,[xOff]
  add	bx,[lineLen]			; calculate x2
  dec	bx
  mov	[x2],bx

  mov	cx,[lineLen]			; line is short
  cmp	cx,4
JUMPS
  jle	@@ShortLine
NOJUMPS

  mov	ax,[xOff]			; calculate starting plane
  mov	bx,ax
  and	al,3
  mov	ah,al
  mov	al,READ_MAP
  mov	dx,GC_INDEX
  OUT_WORD
  push	di
  mov	bx,[x2]
  mov	cx,[xOff]
	@@Loop1:
  cmp	cx,bx
  jg	@@Loop1Done
  lodsb
  stosb
  add	cx,4
  add	di,3
  jmp	short	@@Loop1

	@@Loop1Done:
  pop	di
  inc	di
  push	di
  mov	bx,[xOff]
  inc	bx
  mov	ax,bx
  and	al,3
  mov	ah,al
  mov	al,READ_MAP
  mov	dx,GC_INDEX
  OUT_WORD
  mov	cx,bx
  mov	bx,[lineNum]			; start address in ES:DI
  shl	bx,1
  mov	si,[lineOffs+bx]
  shr	cx,2
  add	si,cx
  mov	bx,[x2]
  mov	cx,[xOff]
  inc	cx
	@@Loop2:
  cmp	cx,bx
  jg	@@Loop2Done
  lodsb
  stosb
  add	cx,4
  add	di,3
  jmp	short	@@Loop2

	@@Loop2Done:
  pop	di
  inc	di
  push	di
  mov	bx,[xOff]
  add	bx,2
  mov	ax,bx
  and	al,3
  mov	ah,al
  mov	al,READ_MAP
  mov	dx,GC_INDEX
  OUT_WORD
  mov	cx,bx
  mov	bx,[lineNum]
  shl	bx,1
  mov	si,[lineOffs+bx]
  shr	cx,2
  add	si,cx
  mov	bx,[x2]
  mov	cx,[xOff]
  add	cx,2
	@@Loop3:
  cmp	cx,bx
  jg	@@Loop3Done
  lodsb
  stosb
  add	cx,4
  add	di,3
  jmp	short	@@Loop3

	@@Loop3Done:
  pop	di
  inc	di
  mov	bx,[xOff]
  add	bx,3
  mov	ax,bx
  and	al,3
  mov	ah,al
  mov	al,READ_MAP
  mov	dx,GC_INDEX
  OUT_WORD
  mov	cx,bx
  mov	bx,[lineNum]
  shl	bx,1
  mov	si,[lineOffs+bx]
  shr	cx,2
  add	si,cx
  mov	bx,[x2]
  mov	cx,[xOff]
  add	cx,3
	@@Loop4:
  cmp	cx,bx
  jg	@@Exit
  lodsb
  stosb
  add	cx,4
  add	di,3
  jmp	short	@@Loop4

	@@ShortLine:
  mov	ax,[xOff]
  mov	bx,ax
  and	al,3
  mov	ah,al
  mov	al,READ_MAP
  mov	dx,GC_INDEX
  OUT_WORD
  mov	cx,bx
  shr	bx,2
  mov	si,bx
  mov	bx,[lineNum]
  shl	bx,1
  add	si,[lineOffs+bx]
  movsb
  cmp	cx,[x2]
  jge	@@Exit
  inc	[xOff]
  jmp	short	@@ShortLine

	@@Exit:
  pop   di si ds
  leave
  retf
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
	ARG	x:WORD, y:WORD, colour:BYTE
  mov	ax,0A000h			; point ES:BX to screen
  mov	es,ax
  mov	bx,[y]				; point BX to start of desired row
  shl	bx,1
  mov	bx,[lineOffs+bx]
  mov	dx,[x]				; DX = x coordinate of pixel
  mov	cx,dx				; CX = x coordinate of pixel
  shr	dx,2				; DX = x coordinate of pixel / 4
  add	bx,dx				; ES:BX points to pixel
  and	cl,3				; get the plane # of the pixel
  mov	ax,102h				; set the bit corresponding to ...
  shl	ah,cl				; ... the pixel's plane
  mov	dx,SC_INDEX
  OUT_WORD
  mov	al,[colour]			; AL = pixel value
  mov	[es:bx],al			; store pixel value
  leave					; clean up call stack
  retf					; return
ENDP

; AND a pixel to the screen
PROC	C	putPixel_scr_and
	ARG	x:WORD, y:WORD, colour:BYTE
  mov	ax,0A000h			; point ES:BX to screen
  mov	es,ax
  mov	bx,[y]				; point BX to start of desired row
  shl	bx,1
  mov	bx,[lineOffs+bx]
  mov	dx,[x]				; DX = x coordinate of pixel
  mov	cx,dx				; CX = x coordinate of pixel
  shr	dx,2				; DX = x coordinate of pixel / 4
  add	bx,dx				; ES:BX points to pixel
  and	cl,3				; get the plane # of the pixel
  mov	ax,102h				; set the bit corresponding to ...
  shl	ah,cl				; ... the pixel's plane
  mov	dx,SC_INDEX
  OUT_WORD
  mov	al,[colour]			; AL = pixel value
  and	[es:bx],al			; AND pixel value
  leave					; clean up call stack
  retf					; return
ENDP

; NOT a pixel to the screen
PROC	C	putPixel_scr_not
	ARG	x:WORD, y:WORD, colour:BYTE
  mov	ax,0A000h			; point ES:BX to screen
  mov	es,ax
  mov	bx,[y]				; point BX to start of desired row
  shl	bx,1
  mov	bx,[lineOffs+bx]
  mov	dx,[x]				; DX = x coordinate of pixel
  mov	cx,dx				; CX = x coordinate of pixel
  shr	dx,2				; DX = x coordinate of pixel / 4
  add	bx,dx				; ES:BX points to pixel
  and	cl,3				; get the plane # of the pixel
  mov	ax,102h				; set the bit corresponding to ...
  shl	ah,cl				; ... the pixel's plane
  mov	dx,SC_INDEX
  OUT_WORD
  mov	al,[colour]			; AL = pixel value
  not	al				; NOT pixel value
  mov	[es:bx],al			; store NOTed pixel value
  leave					; clean up call stack
  retf					; return
ENDP

; OR a pixel to the screen
PROC	C	putPixel_scr_or
	ARG	x:WORD, y:WORD, colour:BYTE
  mov	ax,0A000h			; point ES:BX to screen
  mov	es,ax
  mov	bx,[y]				; point BX to start of desired row
  shl	bx,1
  mov	bx,[lineOffs+bx]
  mov	dx,[x]				; DX = x coordinate of pixel
  mov	cx,dx				; CX = x coordinate of pixel
  shr	dx,2				; DX = x coordinate of pixel / 4
  add	bx,dx				; ES:BX points to pixel
  and	cl,3				; get the plane # of the pixel
  mov	ax,102h				; set the bit corresponding to ...
  shl	ah,cl				; ... the pixel's plane
  mov	dx,SC_INDEX
  OUT_WORD
  mov	al,[colour]			; AL = pixel value
  or	[es:bx],al			; OR pixel value
  leave					; clean up call stack
  retf					; return
ENDP

; XOR a pixel to the screen
PROC	C	putPixel_scr_xor
	ARG	x:WORD, y:WORD, colour:BYTE
  mov	ax,0A000h			; point ES:BX to screen
  mov	es,ax
  mov	bx,[y]				; point BX to start of desired row
  shl	bx,1
  mov	bx,[lineOffs+bx]
  mov	dx,[x]				; DX = x coordinate of pixel
  mov	cx,dx				; CX = x coordinate of pixel
  shr	dx,2				; DX = x coordinate of pixel / 4
  add	bx,dx				; ES:BX points to pixel
  and	cl,3				; get the plane # of the pixel
  mov	ax,102h				; set the bit corresponding to ...
  shl	ah,cl				; ... the pixel's plane
  mov	dx,SC_INDEX
  OUT_WORD
  mov	al,[colour]			; AL = pixel value
  xor	[es:bx],al			; XOR pixel value
  leave					; clean up call stack
  retf					; return
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
	ARG	x:WORD, y:WORD
  push	di

  mov	ax,0A000h
  mov	es,ax
  mov	bx,[y]				; point to start of desired row
  shl	bx,1
  mov	bx,[lineOffs+bx]
  mov	dx,[x]
  mov	cx,dx				; store x coord
  shr	dx,2
  add	bx,dx
  mov	di,bx				; ES:DI points to pixel
  and	cl,3				; get the plane # of the pixel
  mov	al,READ_MAP
  mov	ah,cl
  mov	dx,GC_INDEX			; set the bit corresponding to plane
  OUT_WORD
  xor	ax,ax
  mov	al,[es:di]

  pop	di
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
;***** horizLine
;*****

PROC	C	horizLine_scr_copy
	ARG	y:WORD, x1:WORD, x2:WORD, colour:BYTE
	LOCAL	adj1:WORD, adj2:WORD
  push	si di
  cld

  mov	ax,[x1]				; set up adj1
  and	ax,3
  jz	@@adj1A
  mov	bx,4
  sub	bx,ax
  mov	ax,[x1]
  add	ax,bx
  mov	[adj1],ax
  jmp	short	@@adj1B
	@@adj1A:
  mov	ax,[x1]
  mov	[adj1],ax
	@@adj1B:

  mov	ax,[x2]				; set up adj2
  mov	[adj2],ax
  and	ax,3
  sub	[adj2],ax

  mov	ax,[x1]				; ensure x1 <= x2
  cmp	ax,[x2]
  jbe	@@setup
  mov	bx,[x2]
  mov	[x1],bx
  mov	[x2],ax

	@@setup:
  mov	ax,0A000h			; set ES:DI to line start address
  mov	es,ax
  mov	bx,[y]
  shl	bx,1
  mov	di,[lineOffs+bx]
  mov	bx,[x1]				; current x counter
  shr	bx,2
  add	di,bx

  mov	cx,[adj2]			; CX = # of 4-pixel sets starting
  shr	cx,2                            ; on plane 0
  mov	bx,[adj1]
  shr	bx,2
  sub	cx,bx

  cmp	cx,1
  jl	@@shortLine

  ; Draw the left edge
  mov	ax,[x1]				; get plane number of pixel #1
  and	ax,3
  jz	@@planeZeroStart		; starts at plane zero
  mov	si,ax				; set for pixels
  mov	ah,[leftEdgeMask+si]
  mov	al,MAP_MASK
  mov	dx,SC_INDEX
  OUT_WORD
  mov	al,[colour]
  stosb

  ; Draw the interior stretch
	@@planeZeroStart:
  mov	ax,0F02h			; enable writes to all planes
  mov	dx,SC_INDEX
  OUT_WORD
  mov	al,[colour]			; EAX = colour:colour:colour:colour
  mov	ah,al
  mov	dx,ax
  shl	eax,16
  mov	ax,dx
  mov	dx,cx				; DX = CX
  shr	cx,2				; CX = line length in dwords
  rep	stosd				; store the dwords
  mov	cx,dx				; CX = line length in bytes
  and	cx,0000000000000011b		; CX = number of residual bytes
  rep	stosb				; store the residual bytes

  ; Draw the right edge
	@@rightEdge:
  mov	ax,[x2]				; get plane number of last pixel
  and	ax,3
  mov	si,ax				; set for pixels
  mov	ah,[rightEdgeMask+si]
  mov	al,MAP_MASK
  mov	dx,SC_INDEX
  OUT_WORD
  mov	al,[colour]
  stosb

	@@Exit:
  pop	di si				; clean up and go home
  leave
  retf

	@@shortLine:			; for very small lines
  mov	al,[colour]
  mov	cx,[x1]
  mov	dx,[y]
	@@Loop:
  push	ax cx dx			; slow loop until done
  call	putPixel_scr_copy C, cx, dx, ax
  pop	dx cx ax
  inc	cx
  cmp	cx,[x2]
  jle	@@Loop

  pop 	di si
  leave
  retf
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
;***** setPaletteReg
;*****

PROC	C	setPaletteReg
	ARG	palNum:WORD,red:BYTE,green:BYTE,blue:BYTE
  mov	bx,[palNum]

  mov	dx,3C8h				; set for the right palette register
  mov	ax,[palNum]
  out	dx,al
  inc	dx

  mov	al,[red]			; red
  shr	al,2
  jnc	@@L1
  cmp	al,63
  je	@@L1
  inc	al
	@@L1:
  out	dx,al

  mov	al,[green]			; green
  shr	al,2
  jnc	@@L2
  cmp	al,63
  je	@@L2
  inc	al
	@@L2:
  out	dx,al

  mov	al,[blue]			; blue
  shr	al,2
  jnc	@@L3
  cmp	al,63
  je	@@L3
  inc	al
	@@L3:
  out	dx,al

  leave
  retf
ENDP


;*****
;***** getPaletteReg
;*****

PROC	C	getPaletteReg
	ARG	palNum:WORD,red:DWORD,green:DWORD,blue:DWORD
  push	ds si

  mov	dx,3C7h				; set for right palette register
  mov	ax,[palNum]
  out	dx,al
  mov	dx,3C9h

  in	al,dx				; red
  lds	si,[red]
  shl	al,2
  mov	[ds:si],al
  in	al,dx				; green
  lds	si,[green]
  shl	al,2
  mov	[ds:si],al
  in	al,dx				; blue
  lds	si,[blue]
  shl	al,2
  mov	[ds:si],al

  pop	si ds
  leave
  retf
ENDP


;*****
;***** setBlockPalette
;*****

PROC	C	setBlockPalette
	ARG	firstReg:WORD,lastReg:WORD,paletteData:DWORD
  push	ds si

  lds	si,[paletteData]		; set up
  mov	dx,3C8h
  mov	ax,[firstReg]
  out	dx,al
  inc	dx
  mov	cx,[lastReg]			; CX = number of registers
  sub	cx,ax
  inc	cx
  cld

	@@LLoop:
  lodsb			; red
  shr	al,2
  jnc	@@L1
  cmp	al,63
  je	@@L1
  inc	al
  	@@L1:
  out	dx,al

  lodsb			; green
  shr	al,2
  jnc	@@L2
  cmp	al,63
  je	@@L2
  inc	al
  	@@L2:
  out	dx,al

  lodsb			; blue
  shr	al,2
  jnc	@@L3
  cmp	al,63
  je	@@L3
  inc	al
  	@@L3:
  out	dx,al

  loop	@@LLoop

	@@LExit:
  pop	si ds
  leave
  retf
ENDP


;*****
;***** getBlockPalette
;*****

PROC	C	getBlockPalette
	ARG	firstReg:WORD,lastReg:WORD,paletteData:DWORD
  push	di

  les	di,[paletteData]		; set up
  mov	dx,3C7h
  mov	ax,[firstReg]
  out	dx,al
  mov	dx,3C9h
  mov	cx,[lastReg]			; CX = number of registers
  sub	cx,ax
  inc	cx
  cld

	@@L1:
  in	al,dx
  shl	al,2
  stosb			; red
  in	al,dx
  shl	al,2
  stosb			; green
  in	al,dx
  shl	al,2
  stosb			; blue
  loop	@@L1

	@@LExit:
  pop	di
  leave
  retf
ENDP


;*****
;***** clearGraphics
;*****

PROC	C	clearGraphics_scr_copy
	ARG	colour:BYTE
  push	di				; store DI

  mov   dx,SC_INDEX			; enable writes to all four planes
  mov   ax,0F02h
  out   dx,ax

  mov	ax,0A000h			; point ES:DI to screen
  mov	es,ax
  xor	di,di
  mov	al,[colour]			; EAX = colour:colour:colour:colour
  mov	ah,al
  mov	bx,ax
  shl	eax,16
  mov	ax,bx
  mov	cx,8000				; clear the screen
  cld
  rep	stosd

  pop	di				; restore DI
  leave					; clean up call stack
  retf					; return
ENDP


	ENDS
END
