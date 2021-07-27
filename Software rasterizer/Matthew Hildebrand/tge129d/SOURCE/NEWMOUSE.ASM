; NEWMOUSE.ASM
; Copyright (c) 1993 by Matthew Hildebrand
; Turbo Assembler syntax
; This module may not be overlaid

; Provides an interrupt-driven mouse driver extension by trapping the 33h
; interrupt and installing a mouse event handler.  The current position is
; monitored.  A user-definable external routine is used to draw the mouse
; pointer, which is a definable bitmap.


IDEAL
MODEL LARGE TGENEWMOUSE_TEXT
P286N
JUMPS

STRUC	buttonInfo
  state		db	?
  pressX	dw	?
  pressY	dw	?
  pressCount	dw	?
  releaseX	dw	?
  releaseY	dw	?
  releaseCount	dw	?
ENDS


		CODESEG TGENEWMOUSE_TEXT

	LABEL	oldVect		DWORD
old33Off	dw	?
old33Seg	dw	?
	LABEL	putPointer	DWORD
putPointerOff	dw	?
putPointerSeg	dw	?
	LABEL	putBitmap	DWORD
putBitmapOff	dw	?
putBitmapSeg	dw	?
	LABEL	getBitmap	DWORD
getBitmapOff	dw	?
getBitmapSeg	dw	?
	LABEL	pointerAddr	DWORD
pointerOff	dw	?
pointerSeg	dw	?
	LABEL	backgroundAddr	DWORD
backgroundOff	dw	OFFSET background
backgroundSeg	dw	SEG background

curx		dw	?
cury		dw	?
minx		dw	?
maxx		dw	?
miny		dw	?
maxy		dw	?
horizMickeys	dw	?
maxHorizMickeys	dw	?
vertMickeys	dw	?
maxVertMickeys	dw	?
xAdjMickeys	dw	?
yAdjMickeys	dw	?
pointerShown	dw	?
pointerWide	dw	?
pointerDeep	dw	?
xHotSpot	dw	?
yHotSpot	dw	?
oldx		dw	?
oldy		dw	?
oldButtonStatus	dw	?
buttonStatus	dw	?
leftButton	buttonInfo	?
rightButton	buttonInfo	?
centerButton	buttonInfo	?

; This array will store the contents of the screen behind the mouse pointer.
; If exceptionally large pointers will be used, increase the size of this
; buffer from 512 bytes to an appropriate value.  The default value should
; be sufficient for most applications.
background	db	512	DUP(?)

busyFlag	dw	0
active		dw	1


	PUBLIC	C	TGE_initNewMouse
PROC	C	TGE_initNewMouse
	ARG	putPointerPtr:CODEPTR, putBitmapPtr:CODEPTR, getBitmapPtr:CODEPTR
  push	ds

  xor	ax,ax				; set up default values
  mov	[minx],ax
  mov	[maxx],319
  mov	[miny],ax
  mov	[maxy],199
  mov	[curx],ax
  mov	[cury],ax
  mov	[xAdjMickeys],ax
  mov	[yAdjMickeys],ax
  mov	[pointerShown],ax
  lds	bx,[putPointerPtr]
  mov	[putPointerOff],bx
  mov	[putPointerSeg],ds
  lds	bx,[putBitmapPtr]
  mov	[putBitmapOff],bx
  mov	[putBitmapSeg],ds
  lds	bx,[getBitmapPtr]
  mov	[getBitmapOff],bx
  mov	[getBitmapSeg],ds
  mov	[oldx],ax
  mov	[oldy],ax

  mov	ax,3				; get button status
  int	33h
  mov	[buttonStatus],bx
  mov	[oldButtonStatus],bx

  mov	ax,1Bh			        ; get sensitivity
  int	33h
  mov	ax,100
  sub	ax,bx
  shr	ax,3
  inc	ax
  mov	[horizMickeys],ax
  mov	ax,100
  sub	ax,cx
  shr	ax,3
  inc	ax
  mov	[vertMickeys],ax
  call	setMaxMickeys

  mov	ax,000Ch			; set user-defined event handler
  mov	cx,1111111b		; mouse movement or button changed condition
  mov	dx,SEG handler
  mov	es,dx
  mov	dx,OFFSET handler
  int	33h

  mov	ax,0Bh				; clear motion counters
  int	33h

  mov	ax,3533h			; get the old 33h vector
  int	21h
  mov	[old33Off],bx
  mov	[old33Seg],es

  mov	ax,2533h			; set the new 33h vector
  mov	dx,SEG new33
  mov	ds,dx
  mov	dx,OFFSET new33
  int	21h

  pop	ds
  leave
  retcode
ENDP

	PUBLIC	C	TGE_deInitNewMouse
PROC	C	TGE_deInitNewMouse
  push	ds

  mov	ax,2533h			; restore the old 33h vector
  mov	ds,[old33Seg]
  mov	dx,[old33Off]
  int	21h

  mov	ax,000Ch			; disable event handler
  xor	cx,cx
  int	33h

  pop	ds
  retcode
ENDP

	PUBLIC	C	TGE_disableNewMouse
PROC	C	TGE_disableNewMouse
  mov	ax,000Ch			; disable event handler
  xor	cx,cx
  int	33h

  mov	[active],0
  retcode
ENDP

	PUBLIC	C	TGE_enableNewMouse
PROC	C	TGE_enableNewMouse
  mov	ax,000Ch			; set user-defined event handler
  mov	cx,1111111b		; mouse movement or button changed condition
  mov	dx,SEG handler
  mov	es,dx
  mov	dx,OFFSET handler
  pushf
  call	[oldVect]

  mov	ax,3
  pushf
  call	[oldVect]
  mov	[buttonStatus],bx
  mov	[oldButtonStatus],bx

  mov	[active],1
  retcode
ENDP


PROC	new33	FAR
  cmp	[active],1
  jne	@@Exit

  cmp	ax,1				; Show mouse pointer
  je	showPointer

  cmp	ax,2				; Hide mouse pointer
  je	hidePointer

  cmp	ax,3				; Get mouse position and button status
  je	getPos

  cmp	ax,4				; Set mouse pointer position
  je	setPos

  cmp	ax,5				; Get button press information
  je	getPressInfo

  cmp	ax,6				; Get button release information
  je	getReleaseInfo

  cmp	ax,7				; Set horizontal limits for pointer
  je	setHorizLimits

  cmp	ax,8				; Set vertical limits for pointer
  je	setVertLimits

  cmp	ax,9				; Set graphics pointer shape
  je	setPointerShape

  cmp	ax,0Fh				; Set mickeys to pixels ratio
  je	setRatio

  cmp	ax,1Ah				; Set mouse sensitivity
  je	setSensitivity

  cmp	ax,1Bh				; Get mouse sensitivity
  je	getSensitivity

  	@@Exit:
  pushf
  call	[cs:oldVect]			; transfer to the old 33h handler
  iret
ENDP

PROC	showPointer	FAR
  mov	[busyFlag],1
  inc	[pointerShown]
  cmp	[pointerShown],1
  jne	@@Exit
  call	drawPointer

	@@Exit:
  mov	[busyFlag],0
  iret
ENDP

PROC	hidePointer	FAR
  mov	[busyFlag],1
  dec	[pointerShown]
  cmp	[pointerShown],0
  jne	@@Exit
  call	killPointer

  	@@Exit:
  mov	[busyFlag],0
  iret
ENDP

PROC	getPos	FAR
  mov	bx,[buttonStatus]
  mov	cx,[curx]
  mov	dx,[cury]
  iret
ENDP

PROC	setPos	FAR
  cmp	[pointerShown],0
  jle	@@setPosition
  push	cx dx
  call	killPointer
  pop	dx cx

  	@@setPosition:
  mov	[curx],cx			; store coordinates
  mov	[cury],dx
  call	ensureInBounds
  mov	ax,[curx]			; store new mickey counts
  xor	dx,dx
  mul	[horizMickeys]
  mov	[xAdjMickeys],ax
  mov	ax,[cury]
  xor	dx,dx
  mul	[vertMickeys]
  mov	[yAdjMickeys],ax

  cmp	[pointerShown],0
  jle	@@Exit
  call	drawPointer

  	@@Exit:
  iret
ENDP

PROC	getPressInfo	FAR
  test	bx,000b
  je	@@left
  test	bx,010b
  je	@@right
  test	bx,100b
  je	@@center
  iret

  mov	ax,[buttonStatus]

	@@left:
  mov   bx,[leftButton.pressCount]
  mov	[leftButton.pressCount],0
  mov	cx,[leftButton.pressX]
  mov	dx,[leftButton.pressY]
  iret

  	@@right:
  mov   bx,[rightButton.pressCount]
  mov	[rightButton.pressCount],0
  mov	cx,[rightButton.pressX]
  mov	dx,[rightButton.pressY]
  iret

        @@center:
  mov   bx,[centerButton.pressCount]
  mov	[centerButton.pressCount],0
  mov	cx,[centerButton.pressX]
  mov	dx,[centerButton.pressY]
  iret
ENDP

PROC	getReleaseInfo	FAR
  test	bx,000b
  je	@@left
  test	bx,010b
  je	@@right
  test	bx,100b
  je	@@center
  iret

  mov	ax,[buttonStatus]

	@@left:
  mov   bx,[leftButton.releaseCount]
  mov	[leftButton.releaseCount],0
  mov	cx,[leftButton.releaseX]
  mov	dx,[leftButton.releaseY]
  iret

  	@@right:
  mov   bx,[rightButton.releaseCount]
  mov	[rightButton.releaseCount],0
  mov	cx,[rightButton.releaseX]
  mov	dx,[rightButton.releaseY]
  iret

        @@center:
  mov   bx,[centerButton.releaseCount]
  mov	[centerButton.releaseCount],0
  mov	cx,[centerButton.releaseX]
  mov	dx,[centerButton.releaseY]
  iret
ENDP

PROC	setHorizLimits	FAR
  mov	[minx],cx
  mov	[maxx],dx
  call	setMaxMickeys
  call	ensureInBounds
  iret
ENDP

PROC	setVertLimits	FAR
  mov	[miny],cx
  mov	[maxy],dx
  call	setMaxMickeys
  call	ensureInBounds
  iret
ENDP

PROC	setPointerShape	FAR
  push	ax bx cx dx 			; preserve registers

  cmp	[pointerShown],0		; erase pointer if it's on-screen
  jle	@@setAddr
  push  bx cx dx es
  call	killPointer
  pop   es dx cx bx

  	@@setAddr:
  mov	[xHotSpot],bx		  	; set up pointer information
  mov	[yHotSpot],cx
  mov	[pointerSeg],es
  mov	[pointerOff],dx
  mov	bx,dx
  mov	ax,[es:bx]
  mov	[pointerWide],ax
  add	bx,2
  mov	ax,[es:bx]
  mov	[pointerDeep],ax

  cmp	[pointerShown],0		; redraw pointer if necessary
  jle	@@Exit
  call	drawPointer

  	@@Exit:
  pop	dx cx bx ax			; clean up and go home
  iret
ENDP

PROC	setRatio	FAR
  shr	cx,3				; CX = horizontal mickeys/8 pixels
  or	cx,cx
  jnz	@@ySensitivity
  inc	cx

	@@ySensitivity:
  shr	dx,3				; DX = vertical mickeys/8 pixels
  or	dx,dx
  jnz	@@setVars
  inc	dx

  	@@setVars:
  mov	[horizMickeys],cx
  mov	[vertMickeys],dx
  call	setMaxMickeys
  iret
ENDP

PROC	setSensitivity	FAR
  shr	bx,3				; BX = horizontal mickeys/8 pixels
  or	bx,bx
  jnz	@@ySensitivity
  inc	bx

	@@ySensitivity:
  shr	cx,3				; CX = vertical mickeys/8 pixels
  or	cx,cx
  jnz	@@setVars
  inc	cx

  	@@setVars:
  mov	[horizMickeys],bx
  mov	[vertMickeys],cx
  call	setMaxMickeys
  iret
ENDP

PROC	getSensitivity	FAR
  mov	bx,[horizMickeys]
  shl	bx,3
  mov	cx,[vertMickeys]
  shl	cx,3
  xor	dx,dx
  iret
ENDP

PROC	drawPointer	NEAR
  mov	ax,[curx]			; get what'll be behind pointer
  sub	ax,[xHotSpot]
  or	ax,ax				; ensure >= 0
  jns	@@L1
  xor	ax,ax

  	@@L1:
  mov	cx,ax
  add	cx,[pointerWide]

  mov	bx,[cury]
  sub	bx,[yHotSpot]
  or	bx,bx				; ensure >= 0
  jns	@@L2
  xor	bx,bx

  	@@L2:
  mov	dx,bx
  add	dx,[pointerDeep]

  call	[getBitmap] C, ax, bx, cx, dx, [backgroundAddr]	; get background

  mov	ax,[curx]
  sub	ax,[xHotSpot]
  mov	bx,[cury]
  sub	bx,[yHotSpot]
  call	[putPointer] C, ax, bx, [pointerAddr]	; draw pointer

  	@@Exit:
  ret
ENDP

PROC	killPointer	NEAR
  mov	ax,[curx]
  sub	ax,[xHotSpot]
  or	ax,ax
  jns	@@L1
  xor	ax,ax
  	@@L1:
  mov	bx,[cury]
  sub	bx,[yHotSpot]
  or	bx,bx
  jns	@@L2
  xor	bx,bx
  	@@L2:
  call	[putBitmap] C, ax, bx, [backgroundAddr]   ; restore background

  	@@Exit:
  ret
ENDP

; On entrance to handler:
; AX = mouse event flags (1111111b)
; BX = button state
; CX = X coordinate
; DX = Y coordinate
; SI = last raw horizontal mickey count
; DI = last raw vertical mickey count
; DS = mouse driver data segment
PROC	handler	FAR
  mov	ax,[buttonStatus]		; has button status changed?
  cmp	ax,bx
  jne	@@Button			; yes, button change

  cmp	[busyFlag],1			; is NEWMOUSE busy?
  je	@@Exit				; yes, exit
  mov	[busyFlag],1			; set busy flag to true

  cmp	[pointerShown],0		; kill pointer if necessary
  jle	@@proceed
  call	killPointer

  	@@proceed:
  mov	ax,0Bh				; read mouse motion counters
  pushf
  call	[oldVect]
  mov	bx,[xAdjMickeys]
  add	bx,cx

  or	bx,bx
  jns	@@notOffLeft			; not off left edge
  mov	[xAdjMickeys],0
  jmp	short	@@checkVert
  	@@notOffLeft:
  cmp	bx,[maxHorizMickeys]
  jle	@@notOffRight			; not off right edge
  mov	bx,[maxHorizMickeys]
  mov	[xAdjMickeys],bx
  jmp	short	@@checkVert
  	@@notOffRight:
  mov	[xAdjMickeys],bx

  	@@checkVert:
  mov	bx,[yAdjMickeys]
  add	bx,dx				; DX = delta y from before

  or	bx,bx
  jns	@@notOffTop			; not off left edge
  mov	[yAdjMickeys],0
  jmp	short	@@findCoords
  	@@notOffTop:
  cmp	bx,[maxVertMickeys]
  jle	@@notOffBottom			; not off right edge
  mov	bx,[maxVertMickeys]
  mov	[yAdjMickeys],bx
  jmp	short	@@findCoords
  	@@notOffBottom:
  mov	[yAdjMickeys],bx

  	@@findCoords:
  xor	dx,dx				; calculate x coordinate
  mov	ax,[xAdjMickeys]
  div	[horizMickeys]
  mov	[curx],ax

  xor	dx,dx				; calculate y coordinate
  mov	ax,[yAdjMickeys]
  div	[vertMickeys]
  mov	[cury],ax

  cmp	[pointerShown],0
  jle	@@Exit
  call	drawPointer

  	@@Exit:
  mov	[busyFlag],0			; set busy flag to false
  retf


  	@@Button:
  mov	[buttonStatus],bx
  and	bx,[oldButtonStatus]		; find changed button
  xor	bx,001b
  jnz	@@left				; left button
  and	bx,110b
  xor	bx,010b
  jnz	@@right				; right button
  and	bx,011b
  xor	bx,100b
  jnz	@@center			; center button
  retf

	@@left:
  mov	bx,[buttonStatus]
  and	bx,1
  je	@@leftPress
  inc	[leftButton.releaseCount]
  mov	ax,[curx]
  mov	[leftButton.releaseX],ax
  mov	ax,[cury]
  mov	[leftButton.releaseY],ax
  mov	[leftButton.state],0
  	@@leftPress:
  inc	[leftButton.pressCount]
  mov	ax,[curx]
  mov	[leftButton.pressX],ax
  mov	ax,[cury]
  mov	[leftButton.pressY],ax
  mov	[leftButton.state],1
  retf

  	@@right:
  mov	bx,[buttonStatus]
  and	bx,1
  je	@@rightPress
  inc	[rightButton.releaseCount]
  mov	ax,[curx]
  mov	[rightButton.releaseX],ax
  mov	ax,[cury]
  mov	[rightButton.releaseY],ax
  mov	[rightButton.state],0
  	@@rightPress:
  inc	[rightButton.pressCount]
  mov	ax,[curx]
  mov	[rightButton.pressX],ax
  mov	ax,[cury]
  mov	[rightButton.pressY],ax
  mov	[rightButton.state],1
  retf

  	@@center:
  mov	bx,[buttonStatus]
  and	bx,1
  je	@@centerPress
  inc	[centerButton.releaseCount]
  mov	ax,[curx]
  mov	[centerButton.releaseX],ax
  mov	ax,[cury]
  mov	[centerButton.releaseY],ax
  mov	[centerButton.state],0
  	@@centerPress:
  inc	[centerButton.pressCount]
  mov	ax,[curx]
  mov	[centerButton.pressX],ax
  mov	ax,[cury]
  mov	[centerButton.pressY],ax
  mov	[centerButton.state],1
  retf
ENDP

; Destroys AX
PROC	ensureInBounds	NEAR
  mov	ax,[curx]			; x coordinate
  cmp	ax,[minx]
  jge	@@L1
  mov	ax,[minx]
  mov	[curx],ax
  jmp	short	@@L2
	@@L1:
  cmp	ax,[maxx]
  jle	@@L2
  mov	ax,[maxx]
  mov	[curx],ax

	@@L2:				; y coordinate
  mov	ax,[cury]
  cmp	ax,[miny]
  jge	@@L3
  mov	ax,[miny]
  mov	[cury],ax
  jmp	short	@@L4
	@@L3:
  cmp	ax,[maxy]
  jle	@@L4
  mov	ax,[maxy]
  mov	[cury],ax

  	@@L4:
  ret
ENDP

; Destroys AX and DX
PROC	setMaxMickeys	NEAR
  mov	ax,[maxx]
  mov	dx,[horizMickeys]
  mul	dx
  mov	[maxHorizMickeys],ax

  mov	ax,[maxy]
  mov	dx,[vertMickeys]
  mul	dx
  mov	[maxVertMickeys],ax

  ret
ENDP

	ENDS
END
