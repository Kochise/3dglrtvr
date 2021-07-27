; 800x600x256 (requires register-compatible VGA+)
; Loadable driver for The Graphics Engine
; Copyright (c) 1993 by Matthew Hildebrand
; Turbo Assembler syntax
; Portions by John Bridges

IDEAL
MODEL LARGE TGE800X600_TEXT
P386N


SCREEN_WIDE	=	800
SCREEN_DEEP	=	600
NUM_COLOURS	=	256


		CODESEG TGE800X600_TEXT
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
_putPixel_scr_and	dw      0
			dw	?
_putPixel_scr_not	dw      0
			dw	?
_putPixel_scr_or	dw      0
			dw	?
_putPixel_scr_xor	dw	0
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
scrnMaxX		dw	799	; physical dimensions
scrnMaxY		dw	599
maxColour		dw	255	; maximum colour number
xRatio		       	dw	4	; aspect ratio 4:3 (800:600 in
yRatio			dw	3	;   lowest terms)
bitsPerPixel		dw	8	; 8 bits per pixel
inMaxX			dw	799	; current input screen dimensions
inMaxY			dw	599
outMaxX			dw	799	; current output screen dimensions
outMaxY			dw	599
inScreenWide		dw	?	; needed only for virtual screens
outScreenWide		dw	?
;*** Viewport information
inViewportULX		dw	0
inViewportULY		dw	0
inViewportLRX		dw	799
inViewportLRY		dw	599
outViewportULX		dw	0
outViewportULY		dw	0
outViewportLRX		dw	799
outViewportLRY		dw	599
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
lineOffs	dw	SCREEN_DEEP	DUP(?)
bankNum		dw	SCREEN_DEEP	DUP(?)
bankChanges	dw	SCREEN_DEEP	DUP(?)
curBank		dw	?
screenWide	dw	?

bankadr		dw	OFFSET _nobank
vgamem		dw	?
bksize		dw	?
bksizeShl10Dec	dw	?
retval		dw	?		; first return value from whichVGA()
scanline	dw	?

aheada		dw	?
aheadb		dw	?
ativga		dw	?
chipstech 	dw	?
everex		dw	?
genoa		dw	?
ncr		dw	?
oak067		dw	?
paradise 	dw	?
trident		dw	?
t8900		dw	?
tseng		dw	?
tseng4		dw	?
video7		dw	?
vesa		dw	?


; VESA information
	STRUC	vgainfo
VESASignature	db	4 dup (?)	; 4 signature bytes
VESAVersion	dw	?		; VESA version number
OEMStringPtr	dd	?		; Pointer to OEM string
Capabilities	db	4 dup (?)	; Capabilities of the video environment
VideoModePtr	dd	?		; Pointer to supported Super VGA modes
	ENDS
	STRUC	vesamode
ModeAttributes	dw	?	; mode attributes
WinAAttributes	db	?	; window A attributes
WinBAttributes	db	?	; window B attributes
WinGranularity	dw	?	; window granularity
WinSize		dw	?	; window size
WinASegment	dw	?	; window A start segment
WinBSegment	dw	?	; window B start segment
WinFuncPtr	dd	?	; pointer to window function
BytesPerLine	dw	?	; bytes per scan line
;
; optional information (provided if bit D1 of ModeAttributes is set)
;
XResolution	dw	?	; horizontal resolution
YResolution	dw	?	; vertical resolution
XCharSize	db	?	; character cell width
YCharSize	db	?	; character cell height
NumberOfPlanes	db	?	; number of memory planes
BitsPerPixel	db	?	; bits per pixel
NumberOfBanks	db	?	; number of banks
MemoryModel	db	?	; memory model type
BankSize	db	?	; bank size in kb
	db	227	DUP(?)	; pad to 256 bytes
	ENDS

vesabuf		db	256	DUP(?)
modebuf		vesamode	?


MACRO	NEWBANK
  call	[bankadr]
ENDM


;*****
;***** initGraphics
;*****

PROC	C	initGraphics
  cmp	[inited],0
  je	@@NotInited
  call	setMode
  mov	ax,1
  retf

	@@NotInited:
  call	whichVGA
  or	ax,ax				; was function successful
  jz	@@Error				; no, quit
  call	setMode
  or	ax,ax				; was function successful?
  jz	@@Error				; no, quit
  call	makeAddrTable

  cmp	[vgamem],512			; ensure enough memory
  jb	@@Error				; abort if <512 K of video RAM

  mov	ax,[bksize]			; initialize bksizeShl10Dec
  shl	ax,10				; shift it
  dec	ax				; decrement it
  mov	[bksizeShl10Dec],ax		; store it

  mov	ax,1
  mov	[inited],al
  retf

	@@Error:
  xor	ax,ax
  retf
ENDP


;*****
;***** putLine
;*****

PROC	C	putLine_scr_copy
	ARG	y:WORD, xOff:WORD, lineLen:WORD, buf:DWORD
  push	ds si di

  cld
  mov	bx,[y]				; Decide if bank changes mid-line
  shl	bx,1
  cmp	[cs:bankChanges+bx],0
  jne	@@BankChanged

  mov	ax,[cs:bankNum+bx]
  cmp	ax,[curBank]			; set bank only if necessary
  je	@@NoNewBank
  NEWBANK

	@@NoNewBank:
  mov	cx,[lineLen]			; blast the line into video memory
  mov	ax,0A000h
  mov	es,ax
  mov	di,[cs:lineOffs+bx]
  add	di,[xOff]
  lds	si,[buf]
  mov	dx,cx				; DX = CX
  shr	cx,2				; CX = line length in dwords
  rep	movsd				; copy the dwords
  mov	cx,dx				; CX = line length in bytes
  and	cx,0000000000000011b		; CX = number of residual bytes
  rep	movsb				; copy the residual bytes, if any
  pop	di si ds			; restore registers
  leave				    	; clean up call stack
  retf					; return

	@@BankChanged:		      	; slow pixel-by-pixel
  mov	cx,[lineLen]
  mov	dx,[y]
  mov	bx,[xOff]
  lds	si,[buf]
	@@Loop:
  lodsb
  push	bx cx dx si
  call	far putPixel_scr_copy C,bx,dx,ax
  pop	si dx cx bx
  inc	bx
  loop	@@Loop

  pop	di si ds
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
	ARG	y:WORD, xOff:WORD, lineLen:WORD, buf:DWORD
  push	ds si di

  cld
  mov	bx,[y]				; Decide if bank changes mid-line
  shl	bx,1
  cmp	[cs:bankChanges+bx],0
  jne	@@BankChanged

  mov	ax,[cs:bankNum+bx]
  cmp	ax,[curBank]			; set bank only if necessary
  je	@@NoNewBank
  NEWBANK

	@@NoNewBank:
  mov	cx,[lineLen]			; blast the line into video memory
  mov	ax,0A000h
  mov	ds,ax
  mov	si,[cs:lineOffs+bx]
  add	si,[xOff]
  les	di,[buf]
  mov	dx,cx				; DX = CX
  shr	cx,2				; CX = line length in dwords
  rep	movsd				; copy the dwords
  mov	cx,dx				; CX = line length in bytes
  and	cx,0000000000000011b		; CX = number of residual bytes
  rep	movsb				; copy the residual bytes, if any
  pop	di si ds			; restore registers
  leave				    	; clean up call stack
  retf					; return

	@@BankChanged:		      	; slow pixel-by-pixel
  mov	cx,[lineLen]
  mov	dx,[y]
  mov	bx,[xOff]
  les	di,[buf]
	@@Loop:
  push	bx cx dx di es
  call	far getPixel_scr C,bx,dx
  pop	es di dx cx bx
  stosb
  inc	bx
  loop	@@Loop

  pop	di si ds
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
  mov	bx,[y]				; BX = y coordinate
  shl	bx,1				; to access a table of words
  mov	ax,[cs:bankNum+bx]		; AX = bank at start of line
  mov	bx,[cs:lineOffs+bx]		; DX = offset at start of line
  add	bx,[x]				; BX = offset of pixel
  adc	ax,0				; in case of overflow, inc AX
  mov	dx,[bksizeShl10Dec]		; DX = bank size in bytes - 1
  cmp	bx,dx				; is offset > than bank size
  jbe	@@offsetOK			; no, proceed

  sub	bx,dx				; BX = fixed offset
  inc	ax				; AX = new bank

	@@offsetOK:
  cmp	ax,[curBank]
  je	@@NoNew
  NEWBANK				; switch banks if a new bank entered
	@@NoNew:
  mov	ax,0A000h			; setup screen segment A000
  mov	es,ax
  mov	al,[colour]			; get color of pixel to plot
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
	ARG	x:WORD, y:WORD
  mov	bx,[y]				; BX = y coordinate
  shl	bx,1				; to access a table of words
  mov	ax,[cs:bankNum+bx]		; AX = bank at start of line
  mov	bx,[cs:lineOffs+bx]		; DX = offset at start of line
  add	bx,[x]				; BX = offset of pixel
  adc	ax,0				; in case of overflow, inc AX
  mov	dx,[bksizeShl10Dec]		; DX = bank size in bytes - 1
  cmp	bx,dx				; is offset > than bank size
  jb	@@offsetOK			; no, proceed

  sub	bx,dx				; BX = fixed offset
  inc	ax				; AX = new bank

	@@offsetOK:
  cmp	ax,[curBank]
  je	@@NoNew
  NEWBANK				; switch banks if a new bank entered
	@@NoNew:
  mov	ax,0A000h			; setup screen segment A000
  mov	es,ax
  mov	al,[es:bx]			; AL = colour for return
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
  push	di

  cld
  mov	bx,[y]				; Decide if bank changes mid-line
  shl	bx,1
  cmp	[cs:bankChanges+bx],0
  jne	@@BankChanged

  mov	ax,[cs:bankNum+bx]
  cmp	ax,[curBank]			; set bank only if necessary
  je	@@NoNewBank
  NEWBANK

	@@NoNewBank:
  mov	ax,0A000h
  mov	es,ax
  mov	ax,[x1]
  mov	di,[cs:lineOffs+bx]
  add	di,ax
  mov	cx,[x2]				; blast the line into video memory
  inc	cx
  sub	cx,ax
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
  leave					; clean up call stack
  retf					; return

	@@BankChanged:		      	; slow pixel-by-pixel
  mov	bx,[x1]
  mov	cx,[x2]
  inc	cx
  sub	cx,bx
  mov	dx,[y]
	@@Loop:
  push	bx cx dx
  mov	al,[colour]
  call	far putPixel_scr_copy C,bx,dx,ax
  pop	dx cx bx
  inc	bx
  loop	@@Loop

  pop	di
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
  inc	cx				; CX = line length in bytes
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

  mov	dx,3C8h				; set for correct palette register
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

  mov	dx,3C7h				; set for correct palette register
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
;***** Bank switching routines
;*****

_aheada:				; Ahead Systems Ver A
  push	ax dx cx
  cli
  mov	[curBank],ax
  mov	ch,al
  mov	dx,3CEh			; Enable extended registers
  mov	ax,200Fh
  out	dx,ax
  mov	dl,0CCh			; bit 0
  in	al,dx
  mov	dl,0C2h
  and	al,11011111b
  shr	ch,1
  jnc	@@skpa
  or	al,00100000b
	@@skpa:
  out	dx,al
  mov	dl,0CFh			; bits 1,2,3
  xor	al,al
  out	dx,al
  inc	dx
  in	al,dx
  dec	dx
  and	al,11111000b
  or	al,ch
  mov	ah,al
  xor	al,al
  out	dx,ax
  sti
  pop	cx dx ax
  ret

_aheadb:				; Ahead Systems Ver B
  push	ax dx cx
  cli
  mov	[curBank],ax
  mov	ch,al
  mov	dx,3CEh			; Enable extended registers
  mov	ax,200Fh
  out	dx,ax
  mov	ah,ch
  mov	cl,4
  shl	ah,cl
  or	ah,ch
  mov	al,0Dh
  out	dx,ax
  sti
  pop	cx dx ax
  ret

_ativga:				; ATI VGA Wonder
  push	ax dx
  cli
  mov	[curBank],ax
  mov	ah,al
  mov	dx,1CEh
  mov	al,0B2h
  out	dx,al
  inc	dl
  in	al,dx
  shl	ah,1
  and	al,0E1h
  or	ah,al
  mov	al,0B2h
  dec	dl
  out	dx,ax
  sti
  pop	dx ax
  ret

_chipstech:				; Chips & Tech
  push	ax dx ax
  cli
  mov	[curBank],ax
  mov	dx,46E8h		; place chip in setup mode
  mov	ax,1Eh
  out	dx,ax
  mov	dx,103h			; enable extended registers
  mov	ax,0080h
  out	dx,ax
  mov	dx,46E8h		; bring chip out of setup mode
  mov	ax,0Eh
  out	dx,ax
  pop	ax
  mov	ah,al
  mov	al,10h
  mov	dx,3D6h
  out	dx,ax
  sti
  pop	dx ax
  ret

_everex:				; Everex
  push	ax dx cx
  cli
  mov	[curBank],ax
  mov	cl,al
  mov	dx,3C4h
  mov	al,8
  out	dx,al
  inc	dl
  in	al,dx
  dec	dl
  shl	al,1
  shr	cl,1
  rcr	al,1
  mov	ah,al
  mov	al,8
  out	dx,ax
  mov	dl,0CCh
  in	al,dx
  mov	dl,0C2h
  and	al,0DFh
  shr	cl,1
  jc	@@nob2
  or	al,20h
  	@@nob2:
  out	dx,al
  sti
  pop	cx dx ax
  ret

_genoa:					; GENOA GVGA
  push	ax dx
  cli
  mov	[curBank],ax
  mov	ah,al
  shl	al,3
  or	ah,al
  mov	al,6
  or	ah,40h
  mov	dx,3C4h
  out	dx,ax
  sti
  pop	dx ax
  ret

_ncr:					; NCR 77C22E
  push	ax dx
  cli
  mov	[curBank],ax
  mov	ah,al
  mov	al,18h
  mov	dx,3C4h
  out	dx,ax
  mov	ax,19h
  out	dx,ax
  sti
  pop	dx ax
  ret

_oak067:				; Oak Technology Inc OTI-067
  push	ax dx
  cli
  mov	[curBank],ax
  and	al,15
  mov	ah,al
  shl	al,4
  or	ah,al
  mov	al,11h
  mov	dx,3DEh
  out	dx,ax
  sti
  pop	dx ax
  ret

_paradise:				; Paradise
  push	ax dx ax
  cli
  mov	[curBank],ax
  mov	dx,3CEh
  mov	ax,50Fh			; turn off write protect on VGA registers
  out	dx,ax
  pop	ax
  mov	ah,al
  mov	al,9
  out	dx,ax
  sti
  pop	dx ax
  ret

_trident:				; Trident
  push	ax dx ax
  cli
  mov	[curBank],ax
  mov	dx,3CEh			; set pagesize to 64k
  mov	al,6
  out	dx,al
  inc	dl
  in	al,dx
  dec	dl
  or	al,4
  mov	ah,al
  mov	al,6
  out	dx,ax

  mov	dl,0C4h			; switch to BPS mode
  mov	al,0Bh
  out	dx,al
  inc	dl
  in	al,dx
  dec	dl

  pop	ax
  mov	ah,al
  xor	ah,2
  mov	dx,3C4h
  mov	al,0Eh
  out	dx,ax
  sti
  pop	dx ax
  ret

_tseng:					; Tseng
  push	ax dx
  cli
  mov	[curBank],ax
  and	al,7
  mov	ah,al
  shl	al,3
  or	al,ah
  or	al,01000000b
  mov	dx,3CDh
  out	dx,al
  sti
  pop	dx ax
  ret

_tseng4:				; Tseng 4000 series
  push	ax dx
  cli
  mov	[curBank],ax
  mov	ah,al
  mov	dx,3BFh			; Enable access to extended registers
  mov	al,3
  out	dx,al
  mov	dl,0D8h
  mov	al,0A0h
  out	dx,al
  and	ah,15
  mov	al,ah
  shl	al,4
  or	al,ah
  mov	dl,0CDh
  out	dx,al
  sti
  pop	dx ax
  ret

_video7:				; Video 7
  push	ax dx cx
  cli
  mov	[curBank],ax
  and	ax,15
  mov	ch,al
  mov	dx,3C4h
  mov	ax,0EA06h
  out	dx,ax
  mov	ah,ch
  and	ah,1
  mov	al,0F9h
  out	dx,ax
  mov	al,ch
  and	al,1100b
  mov	ah,al
  shr	ah,2
  or	ah,al
  mov	al,0F6h
  out	dx,al
  inc	dx
  in	al,dx
  dec	dx
  and	al,NOT 1111b
  or	ah,al
  mov	al,0F6h
  out	dx,ax
  mov	ah,ch
  mov	cl,4
  shl	ah,cl
  and	ah,100000b
  mov	dl,0CCh
  in	al,dx
  mov	dl,0C2h
  and	al,NOT 100000b
  or	al,ah
  out	dx,al
  sti
  pop	cx dx ax
  ret

_vesa:				; Vesa SVGA interface
  push	ax bx dx
  cli
  mov	[curBank],ax
  mov	dx,ax
  xor	bx,bx
  mov	ax,4F05h
  int	10h
  sti
  pop	dx bx ax
  ret

_nobank:
  cli
  mov	[curBank],ax
  sti
  ret


MACRO	BKADR	func
  mov	[func],1
  mov	[bankadr],OFFSET _&func
ENDM

MACRO	NOJMP
  local	lbl
  jmp	short	lbl
	lbl:
ENDM


PROC	whichVGA	NEAR
  push	si di

  cmp	[inited],1
  jne	@@goTest
  mov	ax,[retval]
  pop	di si
  ret

	@@goTest:
  mov	[bankadr],OFFSET _nobank	; Initialize variables
  mov	[vgamem],256
  mov	[bksize],64
  xor	ax,ax
  mov	[curBank],ax
  mov	[aheada],ax
  mov	[aheadb],ax
  mov	[ativga],ax
  mov	[chipstech],ax
  mov	[everex],ax
  mov	[genoa],ax
  mov	[ncr],ax
  mov	[oak067],ax
  mov	[paradise],ax
  mov	[trident],ax
  mov	[t8900],ax
  mov	[tseng],ax
  mov	[tseng4],ax
  mov	[video7],ax
  mov	[vesa],ax

  mov	ax,cs				; Test for VESA
  mov	es,ax
  mov	di,OFFSET vesabuf
  mov	ax,4F00h
  int	10h
  cmp	ax,004Fh
  jne	@@noVESA
  BKADR	vesa
  mov	[bksize],64			; assume 64 K bank size (for now)
  mov	[vgamem],512			; assume 512 K if VESA
  jmp	@@fini

	@@noVESA:
  mov	si,1

  mov	ax,0C000h			; Test for ATI
  mov	es,ax
  cmp	[word ptr es:40h],'13'	; ATI Signiture on the Video BIOS
  jnz	@@noATI
  BKADR	ativga
  mov	[bksize],64		; 64K bank size
  mov	dx,[es:10h]		; get value of ATI extended register
  mov	bl,[es:43h]		; get value of ATI chip version
  cmp	bl,'3'
  jae	@@v6up			; use different method to find memory size
  mov	al,0BBh
  cli
  out	dx,al
  inc	dx
  in	al,dx
  sti
  test	al,20h
  jz	@@no512
  mov	[vgamem],512
  jmp	short	@@no512
  	@@v6up:
  mov	al,0B0h			; method for newer ATIs
  cli
  out	dx,al
  inc	dx
  in	al,dx			; get RAM size for versions 3-5
  sti
  test	al,10h			; check if 256 K or 512 K
  jz	@@v7up
  mov	[vgamem],512
  	@@v7up:
  cmp	bl,'4'			; get RAM size for versions 4 & 5
  jb	@@no512
  test	al,8			; check if version 5 chip has 1024 K
  jz	@@no512
  mov	[vgamem],1024
  	@@no512:
  jmp	@@fini

	@@noATI:
  mov	ax,7000h			; Test for Everex
  xor	bx,bx
  cld
  int	10h
  cmp	al,70h
  jnz	@@noEverex
  BKADR	everex
  mov	[bksize],64		; 64 K bank size
  and	ch,11000000b		; how much memory on board?
  jz	@@skp
  mov	[vgamem],512
 	@@skp:			; fall through for Everex boards using Trident or Tseng4000

	@@noEverex:
  mov	dx,3C4h				; Test for NCR 77C22E
  mov	ax,0FF05h
  call	_isport2
  jnz	@@noNCR
  mov	ax,5		       	; Disable extended registers
  out	dx,ax
  mov	ax,0FF10h		; Try to write to extended register 10
  call	_isport2		; If it writes then not NCR
  jz	@@noNCR
  mov	ax,105h			; Enable extended registers
  out	dx,ax
  mov	ax,0FF10h
  call	_isport2
  jnz	@@noNCR			; If it does NOT write then not NCR
  BKADR	ncr
  mov	[bksize],16		; 16 K bank size
  mov	[vgamem],512
  jmp	@@fini

	@@noNCR:
  mov	dx,3C4h          		; Test for Trident
  mov	al,0Bh
  out	dx,al
  inc	dl
  in	al,dx
  and	al,0Fh
  cmp	al,06h
  ja	@@noTrident
  cmp	al,2
  jb   	@@noTrident
  BKADR	trident			; Trident found, at least 8800
  mov	[bksize],64
  cmp	al,3			; is it 8900?
  jb	@@no8900		; no
  mov	[t8900],1		; yes
  mov	dx,3D5h
  mov	al,1Fh
  out	dx,al
  inc	dx
  in	al,dx
  and	al,3
  cmp	al,1
  jb	@@notmem
  mov	[vgamem],512
  je	@@notmem
  mov	[vgamem],1024
	@@notmem:
  jmp	@@fini

	@@no8900:
  mov	[vgamem],512
  jmp	@@fini

	@@noTrident:
  mov	ax,6F00h			; Test for Video 7
  xor	bx,bx
  cld
  int	10h
  cmp	bx,'V7'
  jnz	@@noVideo7
  BKADR	video7
  mov	[bksize],64		; 64 K bank size
  mov	ax,6F07h
  cld
  int	10h
  and	ah,7Fh
  cmp	ah,1
  jbe	@@skp2
  mov	[vgamem],512
	@@skp2:
  cmp	ah,3
  jbe	@@skp3
  mov	[vgamem],1024
	@@skp3:
  jmp	@@fini

	@@noVideo7:
  mov	dx,3D4h				; Test for GENOA GVGA
  mov	ax,032Eh		; check for Herchi Register
  call	_isport2
  jnz	@@noGenoa
  mov	dx,3C4h			; check for memory segment register
  mov	ax,3F06h
  call	_isport2
  jnz	@@noGenoa
  BKADR	genoa
  mov	[bksize],64		; 64 K bank size
  mov	[vgamem],512
  jmp	@@fini

	@@noGenoa:
  mov	dx,3CEh				; Test for Paradise
  mov	al,9			; check Bank switch register
  out	dx,al
  inc	dx
  in	al,dx
  dec	dx
  or	al,al
  jnz	@@noParadise

  mov	ax,50Fh			; turn off write protect on VGA registers
  out	dx,ax
  mov	dx,OFFSET _paradise
  mov	cx,1
  call	_chkbk
  jc	@@noParadise		; if bank 0 and 1 same not paradise
  BKADR	paradise
  mov	[bksize],4		; 4 K bank size
  mov	dx,3CEh
  mov	al,0Bh			; 512k detect from Bob Berry
  out	dx,al
  inc	dx
  in	al,dx
  test	al,80h			; if top bit set then 512k
  jz	@@nop512
  mov	[vgamem],512
	@@nop512:
  jmp	@@fini

	@@noParadise:
  mov	ax,5F00h			; Test for Chips & Tech
  xor	bx,bx
  cld
  int	10h
  cmp	al,5Fh
  jnz	@@noChipsTech
  BKADR	chipstech
  mov	[bksize],16		; 16 K bank size
  cmp	bh,1
  jb	@@skp4
  mov	[vgamem],512
  cmp	bh,2
  jb	@@skp4
  mov	[vgamem],1024
	@@skp4:
  jmp	@@fini

	@@noChipsTech:
  xor	ch,ch				; check for Tseng 4000 series
  mov	dx,3D4h
  mov	ax,0F33h
  call	_isport2
  jnz	@@noTseng4
  mov	ch,1

  mov	dx,3BFh			; Enable access to extended registers
  mov	al,3
  out	dx,al
  mov	dx,3D8h
  mov	al,0A0h
  out	dx,al
  jmp	short @@yes4

	@@noTseng4:
  mov	dx,3D4h			; Test for Tseng 3000 or 4000
  mov	ax,1F25h		; is the Overflow High register there?
  call	_isport2
  jnz	@@noTseng
  mov	al,03Fh			; bottom six bits only
  jmp	short @@yes3
	@@yes4:
  mov	al,0FFh
	@@yes3:
  mov	dx,3CDh			; test bank switch register
  call	_isport1
  jnz	@@noTseng
  BKADR	tseng
  mov	[bksize],64		; 64 K bank size
  or	ch,ch
  jnz	@@t4mem
  mov	[vgamem],512
  jmp	@@fini

	@@t4mem:
  mov	dx,3D4h			; Tseng 4000 memory detect 1meg
  mov	al,37h
  out	dx,al
  inc	dx
  in	al,dx
  test	al,1000b		; if using 64kx4 RAMs then no more than 256k
  jz	@@nomem
  and	al,3
  cmp	al,1			; if 8 bit wide bus then only two 256kx4 RAMs
  jbe	@@nomem
  mov	[vgamem],512
  cmp	al,2			; if 16 bit wide bus then four 256kx4 RAMs
  je	@@nomem
  mov	[vgamem],1024		; full meg with eight 256kx4 RAMs
	@@nomem:
  BKADR	tseng4
  mov	[bksize],64		; 64 K bank size
  jmp	@@fini

	@@noTseng:
  mov	dx,3CEh				; Test for Above A or B chipsets
  mov	ax,200Fh
  out	dx,ax
  inc	dx
  NOJMP
  in	al,dx
  cmp	al,21h
  jz	@@verB
  cmp	al,20h
  jnz	@@noAbove
  BKADR	aheada
  mov	[bksize],64		; 64 K bank size
  mov	[vgamem],512
  jmp	@@fini
	@@verB:
  BKADR	aheadb
  mov	[bksize],64		; 64 K bank size
  mov	[vgamem],512
  jmp   @@fini

	@@noAbove:
  mov	dx,3DEh				; Test for Oak Technology
  mov	ax,0FF11h		; look for bank switch register
  call	_isport2
  jnz	@@fini
  BKADR	oak067
  mov	[bksize],64		; 64 K bank size
  mov	al,0Dh
  out	dx,al
  inc	dx
  NOJMP
  in	al,dx
  test	al,11000000b
  jz	@@fini
  mov	[vgamem],512
  test	al,01000000b
  jz	@@fini
  mov	[vgamem],1024
  jmp	short	@@fini

  	@@noSVGA:
  xor	si,si				; set error flag

	@@fini:
  mov	ax,si				; success code in AX for return
  mov	[retval],ax
  pop	di si
  ret
ENDP


PROC	_chkbk	NEAR			; bank switch check routine
  mov	di,0B800h
  mov	es,di
  xor	di,di
  mov	bx,1234h
  call	_gochk
  jnz	@@badchk
  mov	bx,4321h
  call	_gochk
  jnz	@@badchk
  clc
  ret
	@@badchk:
  stc
  ret
ENDP


PROC	_gochk	NEAR
  push	si
  mov	si,bx

  mov	al,cl
  call	dx
  xchg	bl,[es:di]
  mov	al,ch
  call	dx
  xchg	bh,[es:di]

  xchg	si,bx

  mov	al,cl
  call	dx
  xor	bl,[es:di]
  mov	al,ch
  call	dx
  xor	bh,[es:di]

  xchg	si,bx

  mov	al,ch
  call	dx
  mov	[es:di],bh
  mov	al,cl
  call	dx
  mov	[es:di],bl

  xor	al,al
  call	dx
  or	si,si
  pop	si
  ret
ENDP


PROC	_isport2	NEAR		; check for valid I/O port
					; AL is index, AH is bit mask
  push	bx
  mov	bx,ax
  out	dx,al
  mov	ah,al
  inc	dx
  in	al,dx
  dec	dx
  xchg	al,ah
  push	ax
  mov	ax,bx
  out	dx,ax
  out	dx,al
  mov	ah,al
  inc	dx
  in	al,dx
  dec	dx
  and	al,bh
  cmp	al,bh
  jnz	@@noport
  mov	al,ah
  xor	ah,ah
  out	dx,ax
  out	dx,al
  mov	ah,al
  inc	dx
  in	al,dx
  dec	dx
  and	al,bh
  cmp	al,0
	@@noport:
  pop	ax
  out	dx,ax
  pop	bx
  ret
ENDP


PROC	_isport1	NEAR		; check for valid I/O port
					; AL is bit mask
  mov	ah,al
  in	al,dx
  push	ax
  mov	al,ah
  out	dx,al
  in	al,dx
  and	al,ah
  cmp	al,ah
  jnz	@@noport
  xor	al,al
  out	dx,al
  in	al,dx
  and	al,ah
  cmp	al,0
	@@noport:
  pop	ax
  out	dx,al
  ret
ENDP


PROC	setMode	NEAR			; Set 800x600x256
  mov	ax,SCREEN_WIDE
  mov	[scanline],ax

  cmp	[vesa],0
  jz	@@noVESA
  mov	bx,103h
  call	VESAset
  or	ax,ax				; was function successful?
JUMPS
  jz	@@noSVGA			; no, quit
NOJUMPS
  jmp	@@godo2
  	@@noVESA:
  cmp	[genoa],0
  jz	@@noGenoa
  mov	ax,5Eh
  jmp	@@godo
	@@noGenoa:
  cmp	[ncr],0
  jz	@@noNCR
  mov	ax,5Ch
  jmp	@@godo
	@@noNCR:
  cmp	[oak067],0
  jz	@@noOak067
  mov	ax,54h
  jmp	@@godo
	@@noOak067:
  cmp	[aheada],0
  jnz	@@
  cmp	[aheadb],0
  jz	@@noAbove
	@@:
  mov	ax,62h
  jmp	short @@godo
	@@noAbove:
  cmp	[everex],0
  jz	@@noEverex
  mov	ax,70h
  mov	bl,31h
  jmp	short @@godo
	@@noEverex:
  cmp	[ativga],0
  jz	@@noATI
  mov	ax,63h
  jmp	short @@godo
	@@noATI:
  cmp	[trident],0
  jz	@@noTrident
  mov	ax,5Eh
  jmp	short @@godo
	@@noTrident:
  cmp	[video7],0
  jz	@@noVideo7
  mov	ax,6F05h
  mov	bl,69h
  jmp	short @@godo
	@@noVideo7:
  cmp	[chipstech],0
  jz	@@noChipsTech
  mov	ax,7Bh
  jmp	short @@godo
	@@noChipsTech:
  cmp	[paradise],0
  jz	@@noParadise
  mov	ax,5Ch
  jmp	short	@@godo
	@@noParadise:
  cmp	[tseng],0
  jz	@@noSVGA
  mov	ax,30h

	@@godo:
  int	10h
  	@@godo2:
  mov	[curBank],-1
  mov	ax,1				; return success code
  ret

	@@noSVGA:
  xor	ax,ax				; return error code
  ret
ENDP


PROC	VESAset		NEAR
  push	di

  push	bx
  mov	ax,4F02h
  int	10h		   		; set the mode
  pop	cx
  or	ah,ah				; was function successful?
  jz	@@noError		    	; yes
  xor	ax,ax				; set error code (for setMode)
  jmp	short	@@Exit

  	@@noError:
  mov	ax,cs
  mov	es,ax
  mov	di,OFFSET modebuf
  mov	ax,4F01h
  int	10h				; get the mode information
  mov	ax,[modebuf.WinSize]
  mov	[bksize],ax
  mov	ax,[modebuf.BytesPerLine]
  mov	[scanline],ax
  mov	ax,1				; set success code (for setMode)

  	@@Exit:
  pop	di
  ret
ENDP


PROC	makeAddrTable	NEAR
  LOCAL	temp:WORD
  push	si di

  mov	si,OFFSET lineOffs
  mov	di,OFFSET bankNum
  xor	bx,bx				; current line number = 0

  mov	ax,[bksize]			; separate process if bksize=64 K
  cmp	ax,64
  je	@@noBnk

  mov	cl,10
  shl	ax,cl				; AX = bank size in bytes
  dec	ax
  mov	[temp],ax			; [temp] = banksize in bytes - 1
  mov	cl,10
  mov	ax,[bksize]			; AX = bank size in K
  	@@shlp:
  inc	cl
  shr	ax,1
  jnz	@@shlp				; loop until AX = 0

  xor	ax,ax				; AX = 0
  xor	dx,dx				; DX = 0
  	@@lp:
  push	ax				; store AX
  shr	ax,cl
  add	dx,ax				; update current bank number
  pop	ax				; restore AX
  and	ax,[temp]			; mask offset so it's < bank size
  mov	[cs:si],ax			; save line offset
  add	si,2				; update pointer
  mov	[cs:di],dx			; save bank number
  add	di,2				; update pointer
  add	ax,[scanline]			; update offset
  inc	bx				; update line number
  cmp	bx,SCREEN_DEEP
  jb	@@lp				; loop until all lines done
  jmp	short	@@calcBankChange	; calculate bank change table

  	@@noBnk:
  xor	ax,ax				; AX = 0
  xor	dx,dx				; DX = 0
  mov	cx,[scanline]			; CX = width of line in bytes
  	@@nlp:
  mov	[cs:si],ax			; store line offset
  add	si,2				; update pointer
  mov	[cs:di],dx			; store bank number
  add	di,2				; update pointer
  add	ax,cx				; add another line width
  adc	dx,0				; increase bank number if overflow
  inc	bx				; update current line number
  cmp	bx,SCREEN_DEEP
  jb	@@nlp				; jump until all lines done

  	@@calcBankChange:
  mov	di,OFFSET bankChanges		; initialize pointer
  xor	ax,ax				; AX = current line
  	@@bankChangeLoop:
  mov	bx,ax				; BX = current line
  shl	bx,1				; to access a table of words
  mov	dx,[cs:bankNum+bx]		; DX = bank at start of current line
  mov	bx,ax				; BX = current line
  inc	bx				; BX = next line
  shl	bx,1				; to access a table of words
  mov	cx,[cs:bankNum+bx]		; CX = bank at start of next line
  mov	si,[cs:lineOffs+bx]		; SI = offset at start of next line
  or	si,si				; is SI=0?
  jz	@@L1				; yes, jump
  dec	cx				; CX = bank at end of current line
  	@@L1:
  cmp	cx,dx				; are start and end banks the same?
  jne	@@newBank			; bank changed, jump
  mov	[word ptr cs:di],1		; store bank changed flag
  jmp	short	@@L2			; jump
  	@@newBank:
  mov	[word ptr cs:di],0		; store bank same flag
  	@@L2:
  add	di,2				; update pointer
  inc	ax				; update current line number
  cmp	ax,SCREEN_DEEP
  jb    @@bankChangeLoop		; loop until all lines done

  pop	di si
  ret
ENDP


	ENDS

END
