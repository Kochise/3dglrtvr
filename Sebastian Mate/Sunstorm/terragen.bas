'    SUNSTORM 0.3 compatible map generator.
'    Copyright 2003 Sebastian Mate
'    Uses source code by Danny Beardsley.

'    Modify the parameters below to customize your map!


'    This program is free software; you can redistribute it and/or modify
'    it under the terms of the GNU General Public License as published by
'    the Free Software Foundation; either version 2 of the License, or
'    (at your option) any later version.

'    This program is distributed in the hope that it will be useful,
'    but WITHOUT ANY WARRANTY; without even the implied warranty of
'    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
'    GNU General Public License for more details.

'    You should have received a copy of the GNU General Public License
'    along with this program; if not, write to the Free Software
'    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

DEFINT A-Z
DECLARE SUB pal ()
DECLARE SUB LOADVAR ()
DECLARE SUB INITPAL ()
DECLARE SUB Splitbox (x1%, y1%, x2%, y2%)
DECLARE SUB Newcolor (XA%, YA%, X%, Y%, XB%, YB%)
DECLARE SUB Branch (XX!, YY!, length!, angle!)
DECLARE SUB FileOUT (Para%)
DECLARE SUB DRW3DLAND ()
DECLARE SUB DRAWLAND ()
DECLARE SUB MAKELAND ()
DECLARE SUB setpal (Attr!, Red!, GREEN!, Blue!)
CONST normaL = 0
CONST GREENVALLEY = 1
CONST ALPS = 2
DIM SHARED MaxX AS INTEGER, MaxY AS INTEGER, MAX.COLOR AS INTEGER, ROUGH!, K AS STRING, MUX   AS INTEGER, MUY AS INTEGER, MBY, MBX, BBB AS INTEGER
DIM SHARED ww AS INTEGER
DIM SHARED Colr
SCREEN 13
MaxX = 100
MaxY = 100
MAX.COLOR = 254
DIM SHARED Z(MaxX ^ 2) AS INTEGER, atx%, aty%
DIM SHARED C!(360), S!(360)

DIM SHARED br%, ord%, A&

A& = 1
br% = 3
ko% = 3

DIM SHARED angle(br%) AS SINGLE, ratio(br%) AS SINGLE, di(br%) AS SINGLE, o%, ps

RANDOMIZE t&
FOR N% = 1 TO br%
   aa(N%) = 50 + RND * 50
   ra(N%) = 50 + RND * 50
   da(N%) = 50 + RND * 50
NEXT N%

RANDOMIZE TIMER


'-------------SPECIFY THE PARAMETERS HERE---------------

ROUGH! = 15   'ROUGHNESS OF TERRAIN 0 = SMOOTH 5 = VERY ROUGH
Colr = 3      'THREE DIF COLOR PALETTES
ww = 100      'WATER LEVEL 0-250

MAKELAND

pi = ATN(1) * 4

FOR I = 1 TO 360
   'C!(i) = COS(i * 3.14 / 180): S!(i) = SIN(i * 3.14 / 180)
   C!(I) = COS(I): S!(I) = SIN(I)
NEXT

'CLS
INITPAL

'CLS
'DRW3DLAND

TYPE PUNKT3D
	x1 AS DOUBLE
	y1 AS DOUBLE
	z1 AS DOUBLE
	x2 AS DOUBLE
	y2 AS DOUBLE
	z2 AS DOUBLE
	x3 AS DOUBLE
	y3 AS DOUBLE
	z3 AS DOUBLE
END TYPE

TYPE effect
e1 AS INTEGER
e2 AS INTEGER
e3 AS INTEGER
END TYPE

DIM SHARED FLOOR(MaxX + 1, MaxY + 1) AS INTEGER
DIM SHARED FLOOR2(MaxX + 1, MaxY + 1) AS INTEGER

DIM SHARED Polygon(0 TO 20) AS PUNKT3D
DIM SHARED COL(0 TO 100) AS INTEGER
DIM SHARED EFF1(0 TO 100) AS effect

FOR N% = 1 TO 16
   READ Polygon(N%).x1
   READ Polygon(N%).y1
   READ Polygon(N%).z1
   READ Polygon(N%).x2
   READ Polygon(N%).y2
   READ Polygon(N%).z2
   READ Polygon(N%).x3
   READ Polygon(N%).y3
   READ Polygon(N%).z3
   READ COL(N%)
   READ EFF1(N%).e1
   READ EFF1(N%).e2
   READ EFF1(N%).e3
NEXT

FOR X% = 0 TO MaxX
   FOR Y% = 0 TO MaxY
      C% = POINT(X%, Y%)
      FLOOR(X%, Y%) = C%' - 120
   NEXT
NEXT

OPEN "1.3D" FOR OUTPUT AS #1

' Create the crop fields: ' ---- Experimental, not working!

'FOR N% = 1 TO 20
'   X% = INT(RND * (MaxX - 10))
'   Y% = INT(RND * (MaxY - 10))
'
'   FOR XX% = X% TO X% + 10
'
'      FOR YY% = Y% TO Y% + 10
'         FLOOR2(XX%, YY%) = 1
'         PSET (XX%, YY%), 0
'         FileOUT XX% * 100
'         FileOUT YY% * 100
'         FileOUT FLOOR(XX%, YY%) - 10
'         FileOUT XX% * 100
'         FileOUT YY% * 100 + 100
'         FileOUT FLOOR(XX%, YY% + 1) - 10
'         FileOUT XX% * 100 + 100
'         FileOUT YY% * 100 + 100
'         FileOUT FLOOR(XX% + 1, YY% + 1) - 10
'         FileOUT 8
'         FileOUT 0
'         FileOUT 0
'         FileOUT 0
'         FileOUT 0
'         FileOUT XX% * 100
'         FileOUT YY% * 100
'         FileOUT FLOOR(XX%, YY%) - 10
'         FileOUT XX% * 100 + 100
'         FileOUT YY% * 100
'         FileOUT FLOOR(XX% + 1, YY%) - 10
'         FileOUT XX% * 100 + 100
'         FileOUT YY% * 100 + 100
'         FileOUT FLOOR(XX% + 1, YY% + 1) - 10
'         FileOUT 8
'         FileOUT 0
'         FileOUT 0
'         FileOUT 0
'         FileOUT 0
'      NEXT
'   NEXT
'NEXT



Tex1% = 0
Tex2% = 0

FOR X% = 0 TO MaxX * 100 STEP 100
   FOR Y% = 0 TO MaxY * 100 STEP 100
     
    IF FLOOR2(X% / 100, Y% / 100) = 0 THEN

      FileOUT X%
      FileOUT Y%
      FileOUT FLOOR(X% / 100, Y% / 100)
      
      FileOUT X%
      FileOUT Y% + 100
      FileOUT FLOOR(X% / 100, Y% / 100 + 1)
      
      FileOUT X% + 100
      FileOUT Y% + 100
      FileOUT FLOOR(X% / 100 + 1, Y% / 100 + 1)
      
      PSET (X% / 100, Y% / 100), 1
      
      IF FLOOR(X% / 100, Y% / 100) >= FLOOR(X% / 100, Y% / 100 + 1) THEN
		 IF FLOOR(X% / 100, Y% / 100) >= FLOOR(X% / 100 + 1, Y% / 100 + 1) THEN
		    Max1# = FLOOR(X% / 100, Y% / 100)
		 END IF
      END IF
      IF FLOOR(X% / 100, Y% / 100 + 1) >= FLOOR(X% / 100, Y% / 100) THEN
		 IF FLOOR(X% / 100, Y% / 100 + 1) >= FLOOR(X% / 100 + 1, Y% / 100 + 1) THEN
		    Max1# = FLOOR(X% / 100, Y% / 100 + 1)
		 END IF
      END IF
      IF FLOOR(X% / 100 + 1, Y% / 100 + 1) >= FLOOR(X% / 100, Y% / 100) THEN
		 IF FLOOR(X% / 100 + 1, Y% / 100 + 1) >= FLOOR(X% / 100, Y% / 100 + 1) THEN
		    Max1# = FLOOR(X% / 100 + 1, Y% / 100 + 1)
		 END IF
      END IF
      IF FLOOR(X% / 100, Y% / 100) <= FLOOR(X% / 100, Y% / 100 + 1) THEN
		 IF FLOOR(X% / 100, Y% / 100) <= FLOOR(X% / 100 + 1, Y% / 100 + 1) THEN
		    Min1# = FLOOR(X% / 100, Y% / 100)
		 END IF
      END IF
      IF FLOOR(X% / 100, Y% / 100 + 1) <= FLOOR(X% / 100, Y% / 100) THEN
		 IF FLOOR(X% / 100, Y% / 100 + 1) <= FLOOR(X% / 100 + 1, Y% / 100 + 1) THEN
		    Min1# = FLOOR(X% / 100, Y% / 100 + 1)
		 END IF
      END IF
      IF FLOOR(X% / 100 + 1, Y% / 100 + 1) <= FLOOR(X% / 100, Y% / 100) THEN
		 IF FLOOR(X% / 100 + 1, Y% / 100 + 1) <= FLOOR(X% / 100, Y% / 100 + 1) THEN
		    Min1# = FLOOR(X% / 100 + 1, Y% / 100 + 1)
		 END IF
      END IF
      
      Tex1% = 0
      IF Max1# - Min1# < 20 THEN Tex1% = 5
      IF Max1# - Min1# < 15 THEN Tex1% = 1
      
      'Cropfield:
      'IF FLOOR2(X% / 100, Y% / 100) = 1 THEN Tex1% = 8
      
      Addwater% = 0
      IF FLOOR(X% / 100, Y% / 100) > 85 AND FLOOR(X% / 100, Y% / 100 + 1) > 85 AND FLOOR(X% / 100 + 1, Y% / 100 + 1) > 85 THEN
	 Tex1% = 0
	 IF Max1# - Min1# < 30 THEN Tex1% = 6
      END IF
      
      IF FLOOR(X% / 100, Y% / 100) >= 90 OR FLOOR(X% / 100, Y% / 100 + 1) >= 90 OR FLOOR(X% / 100 + 1, Y% / 100 + 1) >= 90 THEN Addwater% = 1
      
      
      Effa1% = 0
      IF Tex1% = 4 THEN Effa1% = 2
      
      FileOUT Tex1%
      FileOUT Effa1%
      FileOUT Effa1%
      FileOUT Effa1%
      FileOUT 1
      
      IF Addwater% = 1 THEN
	 FileOUT X%
	 FileOUT Y%
	 FileOUT 100
	 
	 FileOUT X%
	 FileOUT Y% + 100
	 FileOUT 100
	 
	 FileOUT X% + 100
	 FileOUT Y% + 100
	 FileOUT 100
	 
	 FileOUT 4
	 FileOUT 2
	 FileOUT 2
	 FileOUT 2
	 FileOUT 0
	 
      END IF
      
      FileOUT X%
      FileOUT Y%
      FileOUT FLOOR(X% / 100, Y% / 100)
      
      FileOUT X% + 100
      FileOUT Y%
      FileOUT FLOOR(X% / 100 + 1, Y% / 100)
      
      FileOUT X% + 100
      FileOUT Y% + 100
      FileOUT FLOOR(X% / 100 + 1, Y% / 100 + 1)
      
      IF FLOOR(X% / 100, Y% / 100) >= FLOOR(X% / 100 + 1, Y% / 100) THEN
		 IF FLOOR(X% / 100, Y% / 100) >= FLOOR(X% / 100 + 1, Y% / 100 + 1) THEN
		    Max2# = FLOOR(X% / 100, Y% / 100)
		 END IF
      END IF
      IF FLOOR(X% / 100 + 1, Y% / 100) >= FLOOR(X% / 100, Y% / 100) THEN
		 IF FLOOR(X% / 100 + 1, Y% / 100) >= FLOOR(X% / 100 + 1, Y% / 100 + 1) THEN
		    Max2# = FLOOR(X% / 100 + 1, Y% / 100)
		 END IF
      END IF
      IF FLOOR(X% / 100 + 1, Y% / 100 + 1) >= FLOOR(X% / 100, Y% / 100) THEN
		 IF FLOOR(X% / 100 + 1, Y% / 100 + 1) >= FLOOR(X% / 100 + 1, Y% / 100) THEN
		    Max2# = FLOOR(X% / 100 + 1, Y% / 100 + 1)
		 END IF
      END IF
      IF FLOOR(X% / 100, Y% / 100) <= FLOOR(X% / 100 + 1, Y% / 100) THEN
		 IF FLOOR(X% / 100, Y% / 100) <= FLOOR(X% / 100 + 1, Y% / 100 + 1) THEN
		    Min2# = FLOOR(X% / 100, Y% / 100)
		 END IF
      END IF
      IF FLOOR(X% / 100 + 1, Y% / 100) <= FLOOR(X% / 100, Y% / 100) THEN
		 IF FLOOR(X% / 100 + 1, Y% / 100) <= FLOOR(X% / 100 + 1, Y% / 100 + 1) THEN
		    Min2# = FLOOR(X% / 100 + 1, Y% / 100)
		 END IF
      END IF
      IF FLOOR(X% / 100 + 1, Y% / 100 + 1) <= FLOOR(X% / 100, Y% / 100) THEN
		 IF FLOOR(X% / 100 + 1, Y% / 100 + 1) <= FLOOR(X% / 100 + 1, Y% / 100) THEN
		    Min2# = FLOOR(X% / 100 + 1, Y% / 100 + 1)
		 END IF
      END IF
      
      Polys% = Polys% + 2
      
      Tex2% = 0
      
      IF Max2# - Min2# < 20 THEN Tex2% = 5
      IF Max2# - Min2# < 15 THEN Tex2% = 1
      
      'Cropfield:
      'IF FLOOR2(X% / 100, Y% / 100) = 1 THEN Tex2% = 8
      
      Addwater% = 0
      IF FLOOR(X% / 100, Y% / 100) > 85 AND FLOOR(X% / 100, Y% / 100 + 1) > 85 AND FLOOR(X% / 100 + 1, Y% / 100 + 1) > 85 THEN
	 Tex2% = 0
	 IF Max1# - Min1# < 30 THEN Tex2% = 6
      END IF
      IF FLOOR(X% / 100, Y% / 100) >= 90 OR FLOOR(X% / 100, Y% / 100 + 1) >= 90 OR FLOOR(X% / 100 + 1, Y% / 100 + 1) >= 90 THEN Addwater% = 1
      
      Effa2% = 0
      IF Tex2% = 4 THEN Effa2% = 2
      
      FileOUT Tex2%
      FileOUT Effa2%
      FileOUT Effa2%
      FileOUT Effa2%
      FileOUT 1
      
      IF Addwater% = 1 THEN
	 
	 FileOUT X%
	 FileOUT Y%
	 FileOUT 100
	 
	 FileOUT X% + 100
	 FileOUT Y%
	 FileOUT 100
	 
	 FileOUT X% + 100
	 FileOUT Y% + 100
	 FileOUT 100
	 
	 FileOUT 4
	 FileOUT 2
	 FileOUT 2
	 FileOUT 2
	 FileOUT 0
      END IF
      
      r% = RND * 40
      
      'Schilf
      
      IF FLOOR(X% / 100, Y% / 100) > 90 AND FLOOR(X% / 100, Y% / 100) < 110 THEN
		 FOR N% = 1 TO 5
		
		    LX% = (RND * 40 + X%) - 20
		    LY% = (RND * 40 + Y%) - 20
		    LZ% = FLOOR(X% / 100, Y% / 100)
		
		    FileOUT LX%
		    FileOUT LY%
		    FileOUT FLOOR(X% / 100, Y% / 100)
		
		    FileOUT LX% + (RND * 4) - 2
		    FileOUT LY% + (RND * 4) - 2
		    FileOUT FLOOR(X% / 100, Y% / 100)
	 
		    FileOUT LX% + (RND * 40 - 20)
		    FileOUT LY% + (RND * 40 - 20)
		    FileOUT FLOOR(X% / 100, Y% / 100) - 20 - (RND * 20)
		
		    FileOUT 1
		    FileOUT 0
		    FileOUT 0
		    FileOUT 1
		    FileOUT 0
		 NEXT
      END IF
      
      ' Laubbaum
      IF r% = 18 AND Tex2% <> 4 AND Tex1% <> 4 AND FLOOR(X% / 100, Y% / 100) < 85 AND FLOOR(X% / 100, Y% / 100 + 1) < 85 AND FLOOR(X% / 100 + 1, Y% / 100 + 1) < 85 THEN
		
		 FOR N% = 13 TO 16
		
		  ang# = 1
		  Polys% = Polys% + 4
		  size! = RND * 1.2
		  IF size! < .3 THEN size! = .3
		
		    FileOUT ((-(Polygon(N%).x1 * size!) * S!(ang#) + (Polygon(N%).y1 * size!) * C!(ang#)) + X%)
		    FileOUT ((-(Polygon(N%).x1 * size!) * C!(ang#) - (Polygon(N%).y1 * size!) * S!(ang#)) + Y%)
		    FileOUT ((Polygon(N%).z1 * size!) + FLOOR(X% / 100, Y% / 100))
		    FileOUT ((-(Polygon(N%).x2 * size!) * S!(ang#) + (Polygon(N%).y2 * size!) * C!(ang#)) + X%)
		    FileOUT ((-(Polygon(N%).x2 * size!) * C!(ang#) - (Polygon(N%).y2 * size!) * S!(ang#)) + Y%)
		    FileOUT ((Polygon(N%).z2 * size!) + FLOOR(X% / 100, Y% / 100))
		    FileOUT ((-(Polygon(N%).x3 * size!) * S!(ang#) + (Polygon(N%).y3 * size!) * C!(ang#)) + X%)
		    FileOUT ((-(Polygon(N%).x3 * size!) * C!(ang#) - (Polygon(N%).y3 * size!) * S!(ang#)) + Y%)
		    FileOUT ((Polygon(N%).z3 * size!) + FLOOR(X% / 100, Y% / 100))
		    FileOUT COL(N%)
		    FileOUT EFF1(N%).e1
		    FileOUT EFF1(N%).e2
		    FileOUT EFF1(N%).e3
		    FileOUT 0
		 NEXT
	 
		 FOR N% = 1 TO 40
		
		    LX% = (RND * 200 + X%) - 100
		    LY% = (RND * 200 + Y%) - 100
		    LZ% = RND * 50 + FLOOR(X% / 100, Y% / 100) - 100
		
		    FileOUT LX% + (RND * 200 - 100)
		    FileOUT LY% + (RND * 200 - 100)
		    FileOUT LZ% + (RND * 30 - 15)
		    FileOUT LX% + (RND * 200 - 100)
		    FileOUT LY% + (RND * 200 - 100)
		    FileOUT LZ% + (RND * 30 - 15)
		    FileOUT LX% + (RND * 200 - 100)
		    FileOUT LY% + (RND * 200 - 100)
		    FileOUT LZ% + (RND * 30 - 15)
	 
		    FileOUT 1
		    FileOUT 1
		    FileOUT 1
		    FileOUT 1
		    FileOUT 0
		 NEXT
      END IF
      
      IF r% = 1 AND Tex2% <> 4 AND Tex1% <> 4 AND FLOOR(X% / 100, Y% / 100) < 85 AND FLOOR(X% / 100, Y% / 100 + 1) < 85 AND FLOOR(X% / 100 + 1, Y% / 100 + 1) < 85 THEN
		 ang# = RND * 360
		 Polys% = Polys% + 16
		 size! = RND * 1.2
		 IF size! < .3 THEN size! = .3
		 FOR N% = 1 TO 16
		    FileOUT ((-(Polygon(N%).x1 * size!) * S!(ang#) + (Polygon(N%).y1 * size!) * C!(ang#)) + X%)
		    FileOUT ((-(Polygon(N%).x1 * size!) * C!(ang#) - (Polygon(N%).y1 * size!) * S!(ang#)) + Y%)
		    FileOUT ((Polygon(N%).z1 * size!) + FLOOR(X% / 100, Y% / 100))
		    FileOUT ((-(Polygon(N%).x2 * size!) * S!(ang#) + (Polygon(N%).y2 * size!) * C!(ang#)) + X%)
		    FileOUT ((-(Polygon(N%).x2 * size!) * C!(ang#) - (Polygon(N%).y2 * size!) * S!(ang#)) + Y%)
		    FileOUT ((Polygon(N%).z2 * size!) + FLOOR(X% / 100, Y% / 100))
		    FileOUT ((-(Polygon(N%).x3 * size!) * S!(ang#) + (Polygon(N%).y3 * size!) * C!(ang#)) + X%)
		    FileOUT ((-(Polygon(N%).x3 * size!) * C!(ang#) - (Polygon(N%).y3 * size!) * S!(ang#)) + Y%)
		    FileOUT ((Polygon(N%).z3 * size!) + FLOOR(X% / 100, Y% / 100))
		    'IF N% > 4 THEN
		    ' FileOUT i%
		    'ELSE
		    FileOUT COL(N%)
		
		    FileOUT EFF1(N%).e1
		    FileOUT EFF1(N%).e2
		    FileOUT EFF1(N%).e3
		    FileOUT 0
		 NEXT
      END IF
   END IF
  
   NEXT
NEXT

CLS
PRINT Polys%; "polygons total"

DATA -80,-80,-10, 80,-80,-10, 0,0,-60, 2, 1, 1, 1
DATA -80,80,-10, 80,80,-10, 0,0,-60, 2, 1, 1, 1
DATA -80,80,-10, -80,-80,-10, 0,0,-60, 2, 1, 1, 1
DATA 80,80,-10, 80,-80,-10, 0,0,-60, 2, 1, 1, 1

			
DATA -60,-60,-40, 60,-60,-40, 0,0,-90, 2, 1, 1, 1
DATA -60,60,-40, 60,60,-40, 0,0,-90, 2, 1, 1, 1
DATA -60,60,-40, -60,-60,-40, 0,0,-90, 2, 1, 1, 1
DATA 60,60,-40, 60,-60,-40, 0,0,-90, 2, 1, 1, 1

DATA -25,-25,-75, 25,-25,-75, 0,0,-100, 2, 1, 1, 1
DATA -25,25,-75, 25,25,-75, 0,0,-100, 2, 1, 1, 1
DATA -25,25,-75, -25,-25,-75, 0,0,-100, 2, 1, 1, 1
DATA 25,25,-75, 25,-25,-75, 0,0,-100, 2, 1, 1, 1

'Stamm:
DATA -15,-15,10, 15,-15,10, 0,0,-100, 3 , 0, 0, 1
DATA -15,15,10, 15,15,10, 0,0,-100, 3, 0, 0, 1
DATA -15,15,10, -15,-15,10, 0,0,-100, 3, 0, 0, 1
DATA 15,15,10, 15,-15,10, 0,0,-100, 3, 0, 0, 1

REM Source cleaned up with BASCLEAN 1.10, Copyright (C) 2000-2001 Sebastian Mate
REM                     ---   www.sm-gimi.de   ---

DEFSNG A-Z
SUB Branch (X, Y, length, angle)
   ord% = ord% + 1: IF ord% > o% THEN EXIT SUB
   
   FOR N% = 1 TO br%
      
	XStart = X + length * ratio(N%) * SIN(angle)
	YStart = Y - length * ratio(N%) * COS(angle)
      
	newangle = angle + angle(N%)
	newlength = length * di(N%)
	XEnd = XStart + newlength * SIN(newangle)
	YEnd = YStart - newlength * COS(newangle)
	LINE (XStart, YStart)-(XEnd, YEnd), 15:  A& = A& + 1
      
	FileOUT atx%
	FileOUT aty% + XStart
	FileOUT FLOOR(atx% / 100, aty% / 100) + YStart - 60
      
	FileOUT atx%
	FileOUT aty% + XStart
	FileOUT FLOOR(atx% / 100, aty% / 100) + YStart - 60
      
	FileOUT atx%
	FileOUT aty% + XEnd
	FileOUT FLOOR(atx% / 100, aty% / 100) + YEnd - 60
      
      FileOUT 3
      FileOUT 0
      FileOUT 0
      FileOUT 0
      FileOUT 0
      
      
	IF ord% = o% THEN PSET (XEnd, YEnd), 15
	Branch XStart, YStart, newlength, newangle
      ord% = ord% - 1
   NEXT N%
END SUB

REM $DYNAMIC
SUB DRAWLAND
   DEFINT A-Z
   
   
   'RANDOMIZE TIMER
   'SLEEP 4
   CLS
   PSET (0, 0), (RND * MAX.COLOR) + 1     'set the corners
   PSET (0, MaxY), (RND * MAX.COLOR) + 1     'set the corners
   PSET (MaxX, 0), (RND * MAX.COLOR) + 1     'set the corners
   PSET (MaxX, MaxY), (RND * MAX.COLOR) + 1     'set the corners
   
   'use Set13Pixel to plant "seed" pixels here, if wanted
   
   Splitbox 0, 0, MaxX, MaxY
   
END SUB

DEFSNG A-Z
SUB DRW3DLAND
   DEFINT A-Z
   LOCATE 25, 7
   PRINT "(PRESS THE SPACEBAR TO EXIT)"
   N = 0
   x1 = (6) / (374 / 300) + 174
   y2 = 255
   x2 = (MaxX * 6) / (474 / 300) + 174
   y1 = 255 - 255 / (474 / 300)
   AX1 = 6
   AY1 = MaxY * 2
   AX2 = MaxX * 6
   AY2 = MaxY * 2 + 254
   FOR Y = 1 TO MaxY - 1
      IF INKEY$ <> "" THEN END
      FOR X = 1 TO MaxX - 1
		 YO = Z(N) / ((175 - Y + 300) / 300)
		 YO2 = Z(N + MaxX) / ((175 - Y + 300) / 300)
		 N = N + 1
		 x1 = (X * 6) / ((175 - Y + 300) / 300) + (175 - Y) - 2
		 y1 = Y * 2 + 254 - YO - 2
		 x2 = ((X + 1) * 6) / ((175 - Y + 300) / 300) + (175 - Y) + 2
		 y2 = Y * 2 + 254 - YO2 + 2
		 LINE (x1 / 3.2, y1 / 3.2)-(x2 / 3.2, y2 / 3.2), Z(N), BF
      NEXT X
      N = N + 1
   NEXT Y
   
   'SLEEP
   
   EXIT SUB
   FOR X = 1 TO MaxX - 1
      YO = Z(N - MaxX)
      YO2 = 0
      N = N + 1
      x1 = (X * 6) - 2
      y1 = 174 * 2 + 254 - YO
      x2 = ((X + 1) * 6) + 2
      y2 = Y * 2 + 254 - YO2
      LINE (x1 / 4, y1 / 4)-(x2 / 4, y2 / 4), 0, BF
   NEXT X
   'DRWBOX 1, 253, AX1, AY1, AX2, AY2
END SUB

REM $STATIC
SUB FileOUT (Para%)
   'PRINT #1, MKI$(Para%);
   PRINT #1, Para%
END SUB

DEFSNG A-Z
SUB INITPAL
   SELECT CASE Colr
   CASE normaL
   FOR I = 1 TO 63
      setpal I, 0, (63 - I) / 2 + 20, 0
   NEXT I
   FOR I = 64 TO 85
      setpal I, (I - 64) * 1.5, 21 - (I - 64) / 4, 1
   NEXT I
   FOR I = 1 TO 63
      setpal I + 85, 32 - (I / 4), 15 - (I / 8), 3
   NEXT I
   FOR I = 149 TO 170
      setpal I, 16 + (I - 149) / 1.3, 7 + (I - 149), 4 + (I - 149)
   NEXT I
   FOR I = 1 TO 84
      setpal I + 170, 32 + I / 3, 28 + I / 2.5, 25 + I / 2.6
   NEXT I
   
   CASE 3
   FOR I = 1 TO 63
      setpal I, 0, (63 - I) / 2 + 20, 0
   NEXT I
   FOR I = 64 TO 85
      setpal I, (I - 64) / 1.5, 21 - (I - 64) / 1.5, 1
   NEXT I
   FOR I = 1 TO 63
      setpal I + 85, 16 + (I / 4), 7 + (I / 8), 3
   NEXT I
   FOR I = 149 TO 170
      setpal I, 32, 15 + (I - 149) / 1.6, 4 + (I - 149)
   NEXT I
   FOR I = 1 TO 84
      setpal I + 170, 32 + I / 3, 28 + I / 2.5, 25 + I / 2.6
   NEXT I
   
   CASE 4
   FOR I = 1 TO 63
      setpal I, 0, (63 - I) / 2 + 20, 0
   NEXT I
   FOR I = 64 TO 85
      setpal I, (I - 64) * 1.5, 21 - (I - 64) / 4, 1
   NEXT I
   FOR I = 1 TO 150
      setpal I + 85, 32 - (I / 10), 15 - (I / 20), 3
   NEXT I
   FOR I = 235 TO 255
      setpal I, 16 + (I - 235) / 1.3, 7 + (I - 235), 4 + (I - 235)
   NEXT I
   
   CASE GREENVALLEY
   FOR I = 1 TO 85
      setpal I, 0, ((85 - I) / 1.349) / 2 + 20, 0
   NEXT I
   FOR I = 85 TO 170
      setpal I, (I - 85) / 2.666, 21 - (I - 85) / 16, 1
   NEXT I
   FOR I = 1 TO 85
      setpal I + 170, 32 - (I / 4), 15 - (I / 8), 3
   NEXT I
   
   CASE ALPS
   FOR I = 1 TO 63
      setpal I, 32 - (I / 4), 15 - (I / 8), 3
   NEXT I
   FOR I = 64 TO 149
      setpal I, 16 + (I - 64) / 5, 7 + (I - 64) / 4, 4 + (I - 64) / 4
   NEXT I
   FOR I = 1 TO 105
      setpal I + 149, 32 + I / 4, 28 + I / 4, 25 + I / 3
   NEXT I
   
   END SELECT
   FOR I = 1 TO ww
      setpal I, 0, 0, 255
   NEXT I
END SUB

SUB LOADVAR
   OPEN "C:\WINDOWS\LANSCAPE.INI" FOR INPUT AS 1
   IF LOF(1) < 1 THEN
      Colr = INT(RND * 3)
      ww = INT(RND * 70)
      ROUGH! = (RND * 3)
      KILL "LANSCAPE.INI"
      CLOSE 1
      EXIT SUB
   END IF
   INPUT #1, Colr, w$, ROUG$, CR, WR, ROR
   ww = VAL(w$)
   ROUGH! = VAL(ROUG$) / 10
   CLOSE 1
END SUB

REM $DYNAMIC
DEFINT A-Z
SUB MAKELAND
   DRAWLAND
   FOR X = 1 TO MaxX
      'MOUSESTATUS MUX, MUY, BBB%
      'IF MBX <> MUX OR MUY <> MBY THEN VIDEOMODESET VMODE: END
      'MBX = MUX: MUY = MBY
      IF INKEY$ <> "" THEN END
      FOR Y = 1 TO MaxY
			 r = POINT(X, Y)
			 IF r < ww THEN r = ww
			 Z(N) = r
			 N = N + 1
      NEXT Y
   NEXT X
   'SLEEP
   'CLS
END SUB

SUB Newcolor (XA, YA, X, Y, XB, YB)       'puts a new color based on average
   IF POINT(X, Y) <> 0 THEN EXIT SUB
   avg = ABS(XA - XB) + ABS(YA - YB)
   COLOUR = (POINT(XA, YA) + POINT(XB, YB)) / 2 + (RND - .5) * avg * ROUGH!
   IF COLOUR > MAX.COLOR THEN COLOUR = MAX.COLOR
   IF COLOUR < 1 THEN COLOUR = 1
   PSET (X, Y), COLOUR
   
END SUB

REM $STATIC
DEFSNG A-Z
SUB setpal (Attr, Red, GREEN, Blue)
   OUT &H3C7, Attr
   OUT &H3C8, Attr
   OUT &H3C9, Red
   OUT &H3C9, GREEN
   OUT &H3C9, Blue
   IF INKEY$ <> "" THEN END
END SUB

REM $DYNAMIC
DEFINT A-Z
'
SUB Splitbox (x1, y1, x2, y2)            'this is the main subroutine
   IF (x2 - x1 < 2) AND (y2 - y1 < 2) THEN EXIT SUB
   
   X = (x1 + x2) / 2
   Y = (y1 + y2) / 2
   Newcolor x1, y1, x1, Y, x1, y2
   Newcolor x1, y2, X, y2, x2, y2
   Newcolor x2, y2, x2, Y, x2, y1
   Newcolor x2, y1, X, y1, x1, y1
   'MOUSESTATUS MUX, MUY, BBB
   'IF MBX <> MUX OR MUY <> MBY THEN VIDEOMODESET VMODE: END
   'MBX = MUX: MUY = MBY
   
   IF POINT(X, Y) = 0 THEN
      IF INKEY$ <> "" THEN END
      COLOUR = (POINT(x1, y1) + POINT(x2, y1) + POINT(x1, y2) + POINT(x2, y2)) / 4
      IF COLOUR > MAX.COLOR THEN COLOUR = MAX.COLOR
      IF COLOUR < 1 THEN COLOUR = 1
      PSET (X, Y), COLOUR
   END IF
   
   Splitbox x1, y1, X, Y
   Splitbox X, y1, x2, Y
   Splitbox X, Y, x2, y2
   Splitbox x1, Y, X, y2
   
END SUB

