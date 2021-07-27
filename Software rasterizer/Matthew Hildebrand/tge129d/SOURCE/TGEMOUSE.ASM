; TGEMOUSE.ASM
; Assorted routines for interfacing to a Microsoft-compatible mouse driver.
; Copyright (c) 1993 by Matthew Hildebrand
; Turbo Assembler syntax


IDEAL
MODEL   LARGE TGETGEMOUSE_TEXT
P286N


		CODESEG TGETGEMOUSE_TEXT

; Resets the mouse driver.  Returns 1 if mouse installed, 0 if not.
; int resetMouse(void);
	PUBLIC	C	TGE_resetMouse
PROC	C	TGE_resetMouse
  xor	ax,ax
  int	33h
  cmp	ax,0FFFFh
  jne	@@L1
  mov	ax,1
  retcode
	@@L1:
  xor	ax,ax
  retcode
ENDP

; Returns the number of buttons on the mouse.
; CAUTION!  Also resets the mouse.
; int getButtonsMouse(void);
	PUBLIC	C	TGE_getButtonsMouse
PROC	C	TGE_getButtonsMouse
  xor	ax,ax
  int	33h
  mov	al,bl
  xor	ah,ah
  retcode
ENDP

; Shows the mouse pointer.
; void showMouse(void);
	PUBLIC	C	TGE_showMouse
PROC	C	TGE_showMouse
  mov	ax,1
  int	33h
  retcode
ENDP

; Hides the mouse pointer.
; void hideMouse(void);
	PUBLIC	C	TGE_hideMouse
PROC	C	TGE_hideMouse
  mov	ax,2
  int	33h
  retcode
ENDP

; Returns the current mouse (x,y) coordinates.
; void getPosMouse(int far *x, int far *y);
	PUBLIC	C	TGE_getPosMouse
PROC	C	TGE_getPosMouse
	ARG	x:DWORD, y:DWORD
  mov	ax,3
  int	33h
  les	bx,[x]
  mov	[es:bx],cx
  les	bx,[y]
  mov	[es:bx],dx
  leave
  retcode
ENDP

; Returns true if a button is pressed.
; int buttonMouse(void);
	PUBLIC	C	TGE_buttonMouse
PROC	C	TGE_buttonMouse
  mov	ax,3
  int	33h
  test	bl,1
  jz	@@L1
  mov	ax,1
  retcode

	@@L1:
  test	bl,2
  jz	@@L2
  mov	ax,2
  retcode
  	@@L2:
  test	bl,4
  jz	@@L3
  mov	ax,3
  retcode
  	@@L3:
  xor	ax,ax
  retcode
ENDP

; Returns true if the left button is pressed.
; int leftButtonMouse(void);
	PUBLIC	C	TGE_leftButtonMouse
PROC	C	TGE_leftButtonMouse
  mov	ax,3
  int	33h
  test	bl,1
  jnz	@@L1
  xor	ax,ax
  retcode
	@@L1:
  mov	ax,1
  retcode
ENDP

; Returns true if the right button is pressed.
; int rightButtonMouse(void);
	PUBLIC	C	TGE_rightButtonMouse
PROC	C	TGE_rightButtonMouse
  mov	ax,3
  int	33h
  test	bl,2
  jnz	@@L1
  xor	ax,ax
  retcode
	@@L1:
  mov	ax,1
  retcode
ENDP

; Returns true if the center button is pressed.
; int centerButtonMouse(void);
	PUBLIC	C	TGE_centerButtonMouse
PROC	C	TGE_centerButtonMouse
  mov	ax,3
  int	33h
  test	bl,4
  jnz	@@L1
  xor	ax,ax
  retcode
	@@L1:
  mov	ax,1
  retcode
ENDP

; Sets the position of the mouse pointer.
; void setPosMouse(unsigned x,unsigned y);
	PUBLIC	C	TGE_setPosMouse
PROC	C	TGE_setPosMouse
	ARG	x:WORD, y:WORD
  mov	ax,4
  mov	cx,[x]
  mov	dx,[y]
  int	33h
  leave
  retcode
ENDP

; Returns the button press counter.
; unsigned buttonPressMouse(unsigned button, far *x, far *y);
	PUBLIC	C	TGE_buttonPressMouse
PROC	C	TGE_buttonPressMouse
	ARG	button:WORD, x:DWORD, y:DWORD
  mov	ax,5
  mov	bx,[button]
  dec	bx
  int	33h
  mov	ax,bx				; save press counter for return
  les	bx,[x]
  mov	[es:bx],cx			; x coordinate
  les	bx,[y]
  mov	[es:bx],dx			; y coordinate
  leave
  retcode
ENDP

; Returns the button release counter.
; unsigned buttonReleaseMouse(unsigned button, far *x, far *y);
	PUBLIC	C	TGE_buttonReleaseMouse
PROC	C	TGE_buttonReleaseMouse
	ARG	button:WORD, x:DWORD, y:DWORD
  mov	ax,6
  mov	bx,[button]
  dec	bx
  int	33h
  mov	ax,bx				; save release counter for return
  les	bx,[x]
  mov	[es:bx],cx			; x coordinate
  les	bx,[y]
  mov	[es:bx],dx			; y coordinate
  leave
  retcode
ENDP

; Set the horizontal limits for the mouse pointer.
; void setHorizLimitsMouse(unsigned min,unsigned max);
	PUBLIC	C	TGE_setHorizLimitsMouse
PROC	C	TGE_setHorizLimitsMouse
	ARG	min:WORD, max:WORD
  mov	ax,7
  mov	cx,[min]
  mov	dx,[max]
;  shl	dx,1				; adjust for mode 13h bug
  int	33h
  leave
  retcode
ENDP

; Set the vertical limits for the mouse pointer.
; void setVertLimitsMouse(unsigned min,unsigned max);
	PUBLIC	C	TGE_setVertLimitsMouse
PROC	C	TGE_setVertLimitsMouse
	ARG	min:WORD, max:WORD
  mov	ax,8
  mov	cx,[min]
  mov	dx,[max]
  int	33h
  leave
  retcode
ENDP

; Set the graphics pointer shape.
; void setPointerMouse(int xoff,int yoff,void *p);
	PUBLIC	C	TGE_setPointerMouse
PROC	C	TGE_setPointerMouse
	ARG	xOff:WORD, yOff:WORD, p:DWORD
  mov	ax,9
  mov	bx,[xOff]
  mov	cx,[yOff]
  les	dx,[p]
  int	33h
  leave
  retcode
ENDP

; Returns the size of the mouse save state buffer.
	PUBLIC	C	TGE_getSaveSizeMouse
PROC	C	TGE_getSaveSizeMouse
  mov	ax,0015h
  int	33h
  mov	ax,bx
  retcode
ENDP

; Save the current state of the mouse driver.
	PUBLIC	C	TGE_saveStateMouse
PROC	C	TGE_saveStateMouse
	ARG	data:DWORD
  mov	ax,0016h
  les	dx,[data]
  int	33h
  leave
  retcode
ENDP

; Restore the state of the mouse driver.
	PUBLIC	C	TGE_restoreStateMouse
PROC	C	TGE_restoreStateMouse
	ARG	data:DWORD
  mov	ax,0017h
  les	dx,[data]
  int	33h
  leave
  retcode
ENDP

; Set the mickeys to pixels ratio (mickeys/8 pixels).
; void setRatioMouse(unsigned horiz, unsigned vert);
	PUBLIC	C	TGE_setRatioMouse
PROC	C	TGE_setRatioMouse
  	ARG	horiz:WORD, vert:WORD
  mov	ax,000Fh
  mov	cx,[horiz]
  mov	dx,[vert]
  int	33h
  leave
  retcode
ENDP

; Get the mouse sensitivity (mickeys/8 pixels).
; void getSensitivityMouse(unsigned far *horiz, unsigned far *vert,
;   unsigned far *doubleSpeed);
	PUBLIC	C	TGE_getSensitivityMouse
PROC	C	TGE_getSensitivityMouse
	ARG	horiz:DWORD, vert:DWORD, doubleSpeed:DWORD
  push	di

  mov	ax,001Bh
  int	33h
  les	di,[horiz]
  mov	[es:di],bx
  les	di,[vert]
  mov	[es:di],cx
  les	di,[doubleSpeed]
  mov	[es:di],dx

  pop	di
  leave
  retcode
ENDP

; Same as resetMouse(), but no initialization of mouse hardware.
; void softResetMouse(void);
	PUBLIC	C	TGE_softResetMouse
PROC	C	TGE_softResetMouse
  mov	ax,21h
  int	33h
  retcode
ENDP

; Waits for the specified button to be released before returning.
; void waitReleaseMouse(int button);
	PUBLIC	C	TGE_waitReleaseMouse
PROC	C	TGE_waitReleaseMouse
	ARG	button:WORD
  dec	[button]
	@@L1:
  mov	ax,3
  int	33h
  mov	cx,[button]
  mov	dx,1
  shl	dx,cl
  and	bx,dx
  or	bx,bx
  jnz	@@L1
  leave
  retcode
ENDP

ENDS
END
