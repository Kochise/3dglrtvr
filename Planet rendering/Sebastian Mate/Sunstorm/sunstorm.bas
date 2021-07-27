'    SUNSTORM 3D Version 0.32
'    Copyright 2003 Sebastian Mate

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


' All integers for speed
DEFINT A-Z

' Declare all Subroutines ...
COMMON SHARED BasePort%, LenPort%, Channel%, IRQ%, DMA%
DECLARE SUB DMAPlay (Segment&, Offset&, Length&, Freq&)
DECLARE SUB WriteDSP (byte%)
DECLARE FUNCTION GetInteger% ()
DECLARE SUB DoPlayIt ()
DECLARE SUB InitSnd ()
DECLARE SUB sortthem ()
DECLARE SUB FFix ()
DECLARE SUB QuickSort (Low%, High%)
DECLARE FUNCTION RandInt% (lower%, upper%)
DIM PAL AS STRING * 768

'$INCLUDE: 'DIRECTQB.BI'
DIM texture(2049)

ON ERROR GOTO fehler
fehler:
RESUME NEXT

SCREEN 13

IF COMMAND$ = "/NODQB" THEN NoDQB% = 1
IF COMMAND$ = "/NOTEX" THEN NoTex% = 1

' Let's initialize the library with no extra layers nor sounds nor EMS
IF NoDQB% = 0 THEN
   IF DQBinit(4, 0, 0) THEN
      DQBclose
      PRINT DQBerror$
      NoDQB% = 1
      GOTO NoDQBUse:
   END IF
   DQBInitVGA
END IF

NoDQBUse:

PC% = 0
FOR PA% = 32 TO 47
   PC% = PC% + 3
   PALETTE PA%, 65536 * 0 + 256 * PC% + 0
NEXT PA%

PC% = 0
FOR PA% = 48 TO 63
   PC% = PC% + 3
   PALETTE PA%, 65536 * PC% + 256 * 0 + 0
NEXT PA%

PC% = 0
FOR PA% = 63 TO 79
   PC% = PC% + 3
   PALETTE PA%, 65536 * 0 + 256 * 0 + PC%
NEXT PA%

FOR A% = 0 TO 256
   LINE (A%, 0)-(A%, 20), A%
NEXT

WindAdd! = .01
WindReach! = .5


phi = 1: theta = 1
ANG# = 230
ang2# = 2: level! = -100

' Calculate SIN and COS tables for speed ...
DIM C!(360), S!(360)
FOR I = 1 TO 360
   C!(I) = COS(I * 3.14 / 180): S!(I) = SIN(I * 3.14 / 180)
NEXT


DIM PAL2(10) AS STRING * 768

IF NoTex% = 0 AND NoDQB% = 0 THEN
   N% = DQBloadImage(1, 0, 0, "sunstorm.pcx", PAL$, 320, 200)
   DQBSetPal PAL$
   DQBCopyLayer 1, Video
   DQBClearLayer 1
END IF
SLEEP 2

' Use Victor's FFix library for speed:
FFix

IF NoDQB% = 0 THEN
   IF NoTex% = 0 THEN
      N% = DQBloadImage(2, 0, 0, "textures.pcx", PAL$, 320, 200)
      N% = DQBloadImage(3, 0, 0, "sky.pcx", PAL$, 320, 200)
      DQBSetPal PAL$
   END IF
  
   ' Clear the screen:
   DQBClearLayer Video
END IF

CONST fokus = 160

TYPE PUNKT3D
X1 AS INTEGER
Y1 AS INTEGER
Z1 AS INTEGER
X2 AS INTEGER
Y2 AS INTEGER
Z2 AS INTEGER
X3 AS INTEGER
Y3 AS INTEGER
Z3 AS INTEGER
END TYPE

TYPE PUNKT3D2
X1 AS SINGLE
Y1 AS SINGLE
Z1 AS SINGLE
X2 AS SINGLE
Y2 AS SINGLE
Z2 AS SINGLE
X3 AS SINGLE
Y3 AS SINGLE
Z3 AS SINGLE
END TYPE

TYPE EFFECT
e1 AS INTEGER
e2 AS INTEGER
e3 AS INTEGER
e4 AS INTEGER
END TYPE

TYPE Enemy
X AS INTEGER
Y AS INTEGER
Z AS INTEGER
DIR AS INTEGER 'direction
END TYPE

'DIM SHARED WavBuffer AS STRING * 5000' Our wave-buffer for the DMA-player
'DIM SHARED ToDelay!, StartZeit!, Length%

DIM SHARED TPoly AS PUNKT3D
DIM SHARED Enemies(1 TO 10) AS Enemy
DIM SHARED PolygonsScreen(701) AS PUNKT3D2
DIM SHARED PolygonsPool(5001) AS PUNKT3D
DIM SHARED PolygonsVisible(701) AS PUNKT3D
DIM SHARED Eff(701) AS EFFECT
DIM SHARED EffectDataFile(5001) AS EFFECT
DIM SHARED Eff3(701) AS EFFECT
DIM SHARED Tex(701) AS INTEGER
DIM SHARED TextureDataFile(5001)  AS INTEGER
DIM SHARED Tex3(701) AS INTEGER
DIM SHARED AverageY(0 TO 701) AS SINGLE
DIM SHARED AverageX(0 TO 701) AS SINGLE
DIM SHARED PolyCount%

ReloadEnvironment% = 1
FoggCol% = 192
RainCol% = 215
'Nature% = 1
RANDOMIZE TIMER
Nature% = INT(RND * 3) + 1  'Fogg, Rain or Normal


FOR E% = 1 TO 10
   Enemies(E%).X = (RND * 50) * 100
   Enemies(E%).Y = (RND * 50) * 100
   Enemies(E%).DIR = RND * 4
NEXT

MoveSpeed% = 5
IF NoDQB% = 0 THEN DQBsetMousePos 160, 0
Resort% = 1
FirstRun% = 1

'OPEN "forest.wav" FOR BINARY AS #3
'WavBuffer$ = INPUT$(4000, #3)
'InitSnd
'DoPlayIt
'z! = TIMER

DO
   
   '<<<< ---- Non-working sound stuff ... :-)
   'IF z! + .272 < TIMER THEN
   ' z! = TIMER
   ' WavBuffer$ = INPUT$(4000, #3)
   ' DoPlayIt
   ' IF EOF(3) THEN CLOSE #3: OPEN "forest.wav" FOR BINARY AS #3
   'END IF
   ' ---- >>>>
   
   I$ = INKEY$
   IF I$ <> "" THEN NoInfoMessage% = 1
   IF I$ = "1" THEN Nature% = 1: WasNoMove% = 0: Resort% = 1
   IF I$ = "2" THEN Nature% = 2: WasNoMove% = 0: Resort% = 1
   IF I$ = "3" THEN Nature% = 3: WasNoMove% = 0: Resort% = 1
   
   'I needed that to determine the Fogg/Rain colors ;-)
   'IF I$ = "8" THEN FoggCol% = FoggCol% + 1: WasNoMove% = 0: Resort% = 1
   'IF I$ = "7" THEN FoggCol% = FoggCol% - 1: WasNoMove% = 0: Resort% = 1
   'IF I$ = "5" THEN RainCol% = RainCol% + 1: WasNoMove% = 0: Resort% = 1
   'IF I$ = "4" THEN RainCol% = RainCol% - 1: WasNoMove% = 0: Resort% = 1
   
   Dist% = SQR(((RadiusX! - ABS(TotPosX# / 100)) ^ 2) + ((RadiusY! - ABS(TotPosY# / 100)) ^ 2))
   
   IF ReloadEnvironment% = 1 OR Dist% >= 15 THEN
      
      ' --- Reload data of polygons which are near us from the file
      '     into our Polygon-"Pool". This pool contains all polygons which
      '     are in the outer circle on the upper left screen. ---
      
      LOCATE 1, 1: PRINT "Reloading environment data ..."
      
      ReloadEnvironment% = 0
      WasNoMove% = 0
      OPEN "1.3d" FOR INPUT AS #1
      polygons% = 1
      Resort% = 1
      
      DO
         
         N% = polygons%
         INPUT #1, PolygonsPool(N%).X1
         INPUT #1, PolygonsPool(N%).Y1
         INPUT #1, PolygonsPool(N%).Z1
         INPUT #1, PolygonsPool(N%).X2
         INPUT #1, PolygonsPool(N%).Y2
         INPUT #1, PolygonsPool(N%).Z2
         INPUT #1, PolygonsPool(N%).X3
         INPUT #1, PolygonsPool(N%).Y3
         INPUT #1, PolygonsPool(N%).Z3
         INPUT #1, TextureDataFile(N%)
         INPUT #1, EffectDataFile(N%).e1
         INPUT #1, EffectDataFile(N%).e2
         INPUT #1, EffectDataFile(N%).e3
         INPUT #1, EffectDataFile(N%).e4
         
         Dist! = SQR(((PolygonsPool(N%).X1 - ABS(TotPosX#)) ^ 2) + ((PolygonsPool(N%).Y1 - ABS(TotPosY#)) ^ 2))
         
         ' If it's near enough, then use it:
         IF Dist! < 1900 THEN polygons% = polygons% + 1
         
         RadiusX! = ABS(TotPosX#) / 100
         RadiusY! = ABS(TotPosY#) / 100
         
      LOOP UNTIL EOF(1) OR N% = 5000
      
      polygons% = polygons% - 1
      PolyCount% = polygons%
      CLOSE #1
      
      ' Add our current position to the polygon data:
      FOR N% = 1 TO polygons%
         PolygonsPool(N%).X1 = PolygonsPool(N%).X1 + TotPosX#
         PolygonsPool(N%).X2 = PolygonsPool(N%).X2 + TotPosX#
         PolygonsPool(N%).X3 = PolygonsPool(N%).X3 + TotPosX#
         PolygonsPool(N%).Y1 = PolygonsPool(N%).Y1 + TotPosY#
         PolygonsPool(N%).Y2 = PolygonsPool(N%).Y2 + TotPosY#
         PolygonsPool(N%).Y3 = PolygonsPool(N%).Y3 + TotPosY#
      NEXT
   END IF
   
   IF WasNoMove% = 0 THEN ' If we have moved ...
      
      IF Resort% = 1 THEN ' ... resort all polygons (instead of z-buffering)
         Resort% = 0
         
         FOR N = 1 TO polygons%
            PolygonsPool(N).X1 = PolygonsPool(N).X1 + xPosX#
            PolygonsPool(N).X2 = PolygonsPool(N).X2 + xPosX#
            PolygonsPool(N).X3 = PolygonsPool(N).X3 + xPosX#
            PolygonsPool(N).Y1 = PolygonsPool(N).Y1 + xPosY#
            PolygonsPool(N).Y2 = PolygonsPool(N).Y2 + xPosY#
            PolygonsPool(N).Y3 = PolygonsPool(N).Y3 + xPosY#
         NEXT
         
         TotPosX# = TotPosX# + xPosX#: xPosX# = 0
         TotPosY# = TotPosY# + xPosY#: xPosY# = 0
         
         ' --- Find out, which of our loaded "Pool"-polygons are in the
         '     visible area before sorting. Only use 600 max. ---
         
         R% = 0
         FOR A% = 1 TO polygons%
            AVY = -PolygonsPool(A%).X1 * C!(ANG#) - PolygonsPool(A%).Y1 * S!(ANG#)
            IF AVY < 1200 AND AVY > 0 THEN
               AvX = -PolygonsPool(A%).X1 * S!(ANG#) + PolygonsPool(A%).Y1 * C!(ANG#)
               IF AvX < (AVY + 100) AND AvX > -(AVY / 2 + 100) AND R% <= 600 THEN
                  R% = R% + 1
                  PolygonsVisible(R%).X1 = -PolygonsPool(A%).X1 * S!(ANG#) + PolygonsPool(A%).Y1 * C!(ANG#)
                  PolygonsVisible(R%).Y1 = -PolygonsPool(A%).X1 * C!(ANG#) - PolygonsPool(A%).Y1 * S!(ANG#)
                  PolygonsVisible(R%).Z1 = PolygonsPool(A%).Z1
                  PolygonsVisible(R%).X2 = -PolygonsPool(A%).X2 * S!(ANG#) + PolygonsPool(A%).Y2 * C!(ANG#)
                  PolygonsVisible(R%).Y2 = -PolygonsPool(A%).X2 * C!(ANG#) - PolygonsPool(A%).Y2 * S!(ANG#)
                  PolygonsVisible(R%).Z2 = PolygonsPool(A%).Z2
                  PolygonsVisible(R%).X3 = -PolygonsPool(A%).X3 * S!(ANG#) + PolygonsPool(A%).Y3 * C!(ANG#)
                  PolygonsVisible(R%).Y3 = -PolygonsPool(A%).X3 * C!(ANG#) - PolygonsPool(A%).Y3 * S!(ANG#)
                  PolygonsVisible(R%).Z3 = PolygonsPool(A%).Z3
                  Tex3(R%) = TextureDataFile(A%)
                  IF NoDQB% = 0 AND NoTex% = 0 THEN
                     IF AVY > 700 AND Nature% = 2 THEN Tex3(R%) = -FoggCol%
                     IF AVY > 700 AND Nature% = 3 THEN Tex3(R%) = -RainCol%
                     IF AVY > 700 AND level! <= -105 THEN Tex3(R%) = -RainCol%
                  END IF
                  Eff3(R%) = EffectDataFile(A%)
                  
               END IF
            END IF
         NEXT
         
         MaxPolyGons% = R%
         IF MaxPolyGons% > 600 THEN MaxPolyGons% = 600
         
         ' Now copy all visible polygons (better: polygondata) to a third
         ' buffer. We need this for the final calculations later:
         
         FOR A% = 1 TO MaxPolyGons%
            PolygonsScreen(A%).X1 = PolygonsVisible(A%).X1
            PolygonsScreen(A%).X2 = PolygonsVisible(A%).X2
            PolygonsScreen(A%).X3 = PolygonsVisible(A%).X3
            PolygonsScreen(A%).Y1 = PolygonsVisible(A%).Y1
            PolygonsScreen(A%).Y2 = PolygonsVisible(A%).Y2
            PolygonsScreen(A%).Y3 = PolygonsVisible(A%).Y3
            PolygonsScreen(A%).Z1 = PolygonsVisible(A%).Z1
            PolygonsScreen(A%).Z2 = PolygonsVisible(A%).Z2
            PolygonsScreen(A%).Z3 = PolygonsVisible(A%).Z3
            Tex(A%) = Tex3(A%)
            Eff(A%) = Eff3(A%)
            AverageY(A%) = ((PolygonsScreen(A%).Y1 + PolygonsScreen(A%).Y2 + PolygonsScreen(A%).Y3) / 3) + ((PolygonsScreen(A%).Z1 + PolygonsScreen(A%).Z2 + PolygonsScreen(A%).Z3) / 3)
         NEXT
         
         ' Now the heavy task: Do the Z-Sorting!!!
         QuickSort 1, MaxPolyGons%
      END IF
      
      'Not working enemy stuff. The first 10 polygons should be diplayed white.
      'Doen't work, though.
      
      'FOR E% = 1 TO 10
      ' PolygonsScreen(E%).X1 = Enemies(E%).X
      ' PolygonsScreen(E%).X2 = Enemies(E%).X + 10
      ' PolygonsScreen(E%).X3 = Enemies(E%).X + 10
      ' PolygonsScreen(E%).Y1 = Enemies(E%).Y
      ' PolygonsScreen(E%).Y2 = Enemies(E%).Y + 10
      ' PolygonsScreen(E%).Y3 = Enemies(E%).Y + 10
      ' PolygonsScreen(E%).Z1 = -50
      ' PolygonsScreen(E%).Z2 = -50
      ' PolygonsScreen(E%).Z3 = -50
      ' Tex(E%) = 8
      'NEXT
      
      ' Find out which is the nearest polygon. We need this to determine the
      ' level of hight.
      
      Maxi! = 100000
      FOR N% = 1 TO MaxPolyGons%
         R! = (SQR(PolygonsScreen(N%).X1 ^ 2 + PolygonsScreen(N%).Y1 ^ 2) + SQR(PolygonsScreen(N%).X2 ^ 2 + PolygonsScreen(N%).Y2 ^ 2) + SQR(PolygonsScreen(N%).X3 ^ 2 + PolygonsScreen(N%).Y3 ^ 2)) / 3
         IF R! < Maxi! AND Eff(N%).e4 = 1 THEN Maxi! = R!: xLevel! = -((PolygonsScreen(N%).Z1 + PolygonsScreen(N%).Z2 + PolygonsScreen(N%).Z3) / 3) + 35
      NEXT
      
      WasNoMove% = 1
   END IF
   
   
   'IF xlevel! < -100 THEN xlevel! = -100
   'IF xlevel! > 100 THEN xlevel! = 100
   
   
   theta = 90 ' <- ??? I better leave it here ;-)
   
   'Do the water animation:
   
   IF ANG# > 360 THEN ANG# = 1
   IF ANG# < 1 THEN ANG# = 360
   WaterCount! = WaterCount! + .05
   IF WaterCount! > 10000 THEN WaterCount! = 0
   FOR N% = 1 TO MaxPolyGons%
      IF Eff(N%).e1 = 2 THEN
         PolygonsScreen(N%).Z1 = PolygonsScreen(N%).Z1 + SIN(WaterCount! + PolygonsScreen(N%).X1) / 10 + COS(WaterCount! + PolygonsScreen(N%).Y1) / 10
      END IF
      IF Eff(N%).e2 = 2 THEN
         PolygonsScreen(N%).Z2 = PolygonsScreen(N%).Z2 + SIN(WaterCount! + PolygonsScreen(N%).X2) / 10 + COS(WaterCount! + PolygonsScreen(N%).Y2) / 10
      END IF
      IF Eff(N%).e3 = 2 THEN
         PolygonsScreen(N%).Z3 = PolygonsScreen(N%).Z3 + SIN(WaterCount! + PolygonsScreen(N%).X3) / 10 + COS(WaterCount! + PolygonsScreen(N%).Y3) / 10
      END IF
   NEXT
   
   
   ' Now the tree-animation:
   
   WindCnt! = WindCnt! + .05
   IF WindCnt! > 10000 THEN WindCnt! = 0
   FOR N% = 1 TO MaxPolyGons%
      IF Eff(N%).e1 = 1 THEN
         PolygonsScreen(N%).X1 = PolygonsScreen(N%).X1 + SIN(WindCnt!) / 3
      END IF
      IF Eff(N%).e2 = 1 THEN
         PolygonsScreen(N%).X2 = PolygonsScreen(N%).X2 + SIN(WindCnt!) / 3
      END IF
      IF Eff(N%).e3 = 1 THEN
         PolygonsScreen(N%).X3 = PolygonsScreen(N%).X3 + SIN(WindCnt!) / 3
      END IF
   NEXT
   
   'Copy the background to our current background. This is still messy and
   'messes up the screen, sometimes. Any ideas???
   
   IF NoDQB% = 0 AND NoTex% = 0 THEN
      FOR Y% = 0 TO 199 STEP 20
         FOR X% = 0 TO 319 STEP 20
            X2% = X% - ((((ANG#) / 360) * 320 * 4.9) MOD 320)
            Y2% = Y% - ang2#
            DQBget 3, X2%, Y2%, X2% + 20, Y2% + 20, VARSEG(texture(0)), VARPTR(texture(0))
            DQBput 1, X%, Y%, VARSEG(texture(0)), VARPTR(texture(0))
         NEXT
      NEXT
     
      C% = DQBpoint(3, 160, 0)
      IF ang2# > 0 THEN DQBBoxF 1, 0, 0, 319, ang2#, C%
      C% = DQBpoint(3, 160, 199)
      IF ang2# + 200 < 200 THEN DQBBoxF 1, 0, ang2# + 200, 319, 199, C%
   END IF
   
   WasFogg% = 0
   WasRain% = 0
   
   ' Do the final calculations. Convert the polygon 3D-data to 2D-screen data:
   
   xcenter = 100
   ycenter = 100
   zcenter = -10
   
   IF NoDQB% = 1 THEN
      ClsCount% = ClsCount% + 1
      IF ClsCount% = 20 THEN CLS : ClsCount% = 0
   END IF
   
   FOR N = MaxPolyGons% TO 1 STEP -1
      
      AC% = 16
      IF Tex(N) = 1 THEN AC% = 32
      IF Tex(N) = 2 THEN AC% = 32
      IF Tex(N) = 3 THEN AC% = 64
      IF Tex(N) = 4 THEN AC% = 48
      IF Tex(N) = 5 THEN AC% = 32
      Scolor% = -((-(N / 40) + 15) + AC%)
      
      
      X1 = -PolygonsScreen(N).X1 * S!(theta) + PolygonsScreen(N).Y1 * C!(theta)
      Y1 = -PolygonsScreen(N).X1 * C!(theta) * S!(phi) - PolygonsScreen(N).Y1 * S!(theta) * S!(phi) - (PolygonsScreen(N).Z1 + level!) * C!(phi) + P
      Z1 = -PolygonsScreen(N).X1 * C!(theta) * C!(phi) - PolygonsScreen(N).Y1 * S!(theta) * C!(phi) + (PolygonsScreen(N).Z1 + level!) * S!(phi)
      XX1 = 256 * (X1 / (Z1 + zcenter)) + xcenter
      yy1 = 656 * (Y1 / (Z1 + zcenter)) + ycenter
      X1 = -PolygonsScreen(N).X2 * S!(theta) + PolygonsScreen(N).Y2 * C!(theta)
      Y1 = -PolygonsScreen(N).X2 * C!(theta) * S!(phi) - PolygonsScreen(N).Y2 * S!(theta) * S!(phi) - (PolygonsScreen(N).Z2 + level!) * C!(phi) + P
      Z1 = -PolygonsScreen(N).X2 * C!(theta) * C!(phi) - PolygonsScreen(N).Y2 * S!(theta) * C!(phi) + (PolygonsScreen(N).Z2 + level!) * S!(phi)
      XX2 = 256 * (X1 / (Z1 + zcenter)) + xcenter
      YY2 = 656 * (Y1 / (Z1 + zcenter)) + ycenter
      X1 = -PolygonsScreen(N).X3 * S!(theta) + PolygonsScreen(N).Y3 * C!(theta)
      Y1 = -PolygonsScreen(N).X3 * C!(theta) * S!(phi) - PolygonsScreen(N).Y3 * S!(theta) * S!(phi) - (PolygonsScreen(N).Z3 + level!) * C!(phi) + P
      Z1 = -PolygonsScreen(N).X3 * C!(theta) * C!(phi) - PolygonsScreen(N).Y3 * S!(theta) * C!(phi) + (PolygonsScreen(N).Z3 + level!) * S!(phi)
      XX3 = 256 * (X1 / (Z1 + zcenter)) + xcenter
      YY3 = 656 * (Y1 / (Z1 + zcenter)) + ycenter
      
      '<<<< ---- Dither routines for fogg, rain etc.
      IF NoDQB% = 0 AND NoTex% = 0 THEN
         IF Nature% <> 1 OR level! <= -105 THEN
            IF Nature% = 2 THEN CurCol% = FoggCol%
            IF Nature% = 3 THEN CurCol% = RainCol%
            IF level! <= -105 THEN CurCol% = RainCol%
            IF PolygonsScreen(N).Y1 < 450 AND WasFogg% = 0 THEN
               WasFogg% = WasFogg + 1
               FOR X! = 0 TO 319 STEP 2
                  FOR Y! = 0 TO 199 STEP 2
                     DQBPSET 1, X!, Y!, CurCol%
                     DQBPSET 1, X! + 1, Y! + 1, CurCol%
                  NEXT
               NEXT
            END IF
            IF PolygonsScreen(N).Y1 < 350 AND WasFogg% = 1 THEN
               WasFogg% = WasFogg + 1
               FOR X! = 0 TO 319 STEP 2
                  FOR Y! = 0 TO 199 STEP 2
                     DQBPSET 1, X!, Y!, CurCol%
                  NEXT
               NEXT
            END IF
         END IF
         IF Nature% = 3 AND level! > -105 THEN
            X% = RND * 320
            Y% = RND * 200 - 5
            DQBLINE 1, X%, Y%, X% + 3, Y% + 10, RainCol%
         END IF
      END IF
      ' ---- >>>>
      
      NoPaint% = 0
      IF XX1 < 0 AND XX2 < 0 AND XX3 < 0 THEN NoPaint% = 1
      IF XX1 > 319 AND XX2 > 319 AND XX3 > 319 THEN NoPaint% = 1
      IF yy1 > 199 - ang2# AND YY2 > 199 - ang2# AND YY3 > 199 - ang2# THEN NoPaint% = 1
      
      IF NoPaint% = 0 THEN
         IF PolygonsScreen(N).Y1 > 0 AND PolygonsScreen(N).Y2 > 0 AND PolygonsScreen(N).Y3 > 0 THEN
            IF NoDQB% = 0 THEN
               IF NoTex% = 1 OR Tex(N) < 0 THEN
                  IF NoTex% = 1 THEN
                     DQBtri 1, XX1, yy1 + ang2#, XX2, YY2 + ang2#, XX3, YY3 + ang2#, -Scolor%
                     ELSE
                     DQBtri 1, XX1, yy1 + ang2#, XX2, YY2 + ang2#, XX3, YY3 + ang2#, -Tex(N)
                  END IF
                  ELSE
                  PY% = 0
                  TX% = Tex(N)
                  IF TX% > 4 THEN TX% = TX% - 4: PY% = 64
                  DQBget 2, Tex(N) * 64, PY%, (Tex(N) * 64) + 63, PY% + 63, VARSEG(texture(0)), VARPTR(texture(0))
                  DQBttri 1, XX1, yy1 + ang2#, XX2, YY2 + ang2#, XX3, YY3 + ang2#, 0, 0, 63, 0, 0, 63, VARSEG(texture(0)), VARPTR(texture(0))
               END IF
            END IF
            
            IF NoDQB% = 1 THEN
               LINE (XX1, yy1 + ang2#)-(XX2, YY2 + ang2#), -Scolor%
               LINE (XX2, YY2 + ang2#)-(XX3, YY3 + ang2#), -Scolor%
               LINE (XX3, YY3 + ang2#)-(XX1, yy1 + ang2#), -Scolor%
            END IF
         END IF
      END IF
   NEXT
   
   IF Nature% = 3 THEN
      LN% = RND * 50
      IF LN% = 0 THEN
         DQBBoxF 1, 0, 0, 319, 199, 204
         WaitForFlash% = 1
      END IF
   END IF
   
   IF NoDQB% = 0 THEN
      DQBPrint 1, STR$(xfps&) + " fps," + STR$(MaxPolyGons%) + " polygons", 0, 1, 250
      'DQBPrint 1, STR$(FoggCol%), 0, 10, 250
      DQBPrint 1, "SUNSTORM v0.32", 215 - 8, 192, 2
      
      IF NoInfoMessage% = 0 AND NoTex% = 0 AND NoDQB% = 0 THEN
         DQBPrint 1, "Welcome to the SUNSTORM demo!", 8, 50, 255
         DQBPrint 1, "Use your mouse to look and move.", 8, 50 + 32, 255
         DQBPrint 1, "Press any key to disable this message,", 8, 50 + 48, 255
         DQBPrint 1, "      'x' to exit, ", 8, 50 + 64, 255
         DQBPrint 1, "      '1' to '3' to change the weather ", 8, 50 + 64 + 16, 255
      END IF
      
      DQBCopyLayer 1, Video
      DQBClearLayer 1
   END IF
   
   IF WaitForFlash% = 1 THEN
      ZEIT! = TIMER
      DO: LOOP UNTIL ZEIT! + .1 < TIMER
   END IF
   
   WaitForFlash% = 0
   
   ' --- Some very early enemy stuff. Only shows dots on the screen. ---
   'FOR E% = 1 TO 10
   '  PSET (Enemies(E%).X / 100, Enemies(E%).Y / 100), 100
   '  IF Enemies(E%).DIR = 0 THEN
   '   Enemies(E%).X = Enemies(E%).X + 1
   '  END IF
   '  IF Enemies(E%).DIR = 1 THEN
   '   Enemies(E%).X = Enemies(E%).X - 1
   '  END IF
   '  IF Enemies(E%).DIR = 2 THEN
   '   Enemies(E%).Y = Enemies(E%).Y + 1
   '  END IF
   '  IF Enemies(E%).DIR = 3 THEN
   '   Enemies(E%).Y = Enemies(E%).Y - 1
   '  END IF
   'NEXT
   'EnemyChange% = EnemyChange% + 1
   'IF EnemyChange% = 200 THEN
   ' EnemyChange% = 0
   'FOR E% = 1 TO 10
   '  Enemies(E%).DIR = INT(RND * 4)
   'NEXT
   'END IF
   
   LINE (-TotPosX# / 100, -TotPosY# / 100)-((C!((ANG# + 180) MOD 360) * 5) - TotPosX# / 100, (S!((ANG# + 180) MOD 360) * 5) - TotPosY# / 100), 11
   CIRCLE (RadiusX!, RadiusY!), 15
   CIRCLE (RadiusX!, RadiusY!), 19
   FPS& = FPS& + 1
   
   IF ZEIT! < TIMER THEN
      ZEIT! = TIMER + 1
      xfps& = FPS&
      FPS& = 0
   END IF
   
   IF NoDQB% = 0 THEN
      IsMouse% = DQBmouseLB
      ELSE
      IsMouse% = False
      IF I$ = "8" THEN IsMouse% = TRUE
   END IF
   
   IF IsMouse% THEN
      WasNoMove% = 0
      Moves% = Moves% + 1
      ReLoadMoves% = ReLoadMoves% + 1
      IF Moves% = 20 THEN Resort% = 1: Moves% = 0
      xPosX# = xPosX# + ((C!(ANG#)) * MoveSpeed%)
      xPosY# = xPosY# + ((S!(ANG#)) * MoveSpeed%)
      FOR N = 1 TO MaxPolyGons%
         PolygonsScreen(N).Y1 = PolygonsScreen(N).Y1 - MoveSpeed%
         PolygonsScreen(N).Y2 = PolygonsScreen(N).Y2 - MoveSpeed%
         PolygonsScreen(N).Y3 = PolygonsScreen(N).Y3 - MoveSpeed%
      NEXT
      IF level! < xLevel! THEN level! = level! + (xLevel! - level!) / 10
      IF level! > xLevel! THEN level! = level! - (level! - xLevel!) / 10
   END IF
   
   IF FirstRun% = 1 THEN
      level! = xLevel!
      FirstRun% = 0
   END IF
   
   IF NoDQB% = 0 THEN
      IsMouse% = DQBmouseRB
      ELSE
      IsMouse% = False
      IF I$ = "2" THEN IsMouse% = TRUE
   END IF
   
   IF IsMouse% THEN
      WasNoMove% = 0
      Moves% = Moves% + 1
      ReLoadMoves% = ReLoadMoves% + 1
      IF Moves% >= 10 THEN Resort% = 1: Moves% = 0
      xPosX# = xPosX# - ((C!(ANG#)) * MoveSpeed%)
      xPosY# = xPosY# - ((S!(ANG#)) * MoveSpeed%)
      FOR N = 1 TO MaxPolyGons%
         PolygonsScreen(N).Y1 = PolygonsScreen(N).Y1 + MoveSpeed%
         PolygonsScreen(N).Y2 = PolygonsScreen(N).Y2 + MoveSpeed%
         PolygonsScreen(N).Y3 = PolygonsScreen(N).Y3 + MoveSpeed%
      NEXT
      IF level! < xLevel! THEN level! = level! + (xLevel! - level!) / 10
      IF level! > xLevel! THEN level! = level! - (level! - xLevel!) / 10
   END IF
   
   IF NoDQB% = 0 THEN
      MouseAngX = 160 - DQBmouseX
      MouseAngY = 100 - DQBmouseY
      ELSE
      MouseAngX = 0
      MouseAngY = 0
      IF I$ = "4" THEN MouseAngX = 2
      IF I$ = "6" THEN MouseAngX = -2
      IF I$ = "7" THEN MouseAngY = 2
      IF I$ = "1" THEN MouseAngY = -2
      
   END IF
   
   IF MouseAngX <> 0 OR MouseAngY <> 0 THEN
      ANG# = ANG# + (MouseAngX)
      ang2# = ang2# + (MouseAngY)
      IF MouseAngX <> 0 THEN Resort% = 1
      IF NoDQB% = 0 THEN DQBsetMousePos 160, 100
      WasNoMove% = 0
      'QuickSort 1, MaxPolyGons%
   END IF
   
LOOP UNTIL I$ = "x"

IF NoDQB% = 0 THEN DQBclose

REM Source cleaned up with BASCLEAN 1.10, Copyright (C) 2000-2001 Sebastian Mate
REM                     ---   www.sm-gimi.de   ---

REM $DYNAMIC
SUB DMAPlay (Segment&, Offset&, Length&, Freq&)
   ' DMA-Player based on Mike Huff!
   ' Transfers and plays the contents of the buffer.
   'print "DMAPLAY"
   Length& = Length& - 1
   Page% = 0
   MemLoc& = Segment& * 16 + Offset&
   SELECT CASE Channel%
   CASE 0
   PgPort% = &H87
   AddPort% = &H0
   LenPort% = &H1
   ModeReg% = &H48
   CASE 1
   PgPort% = &H83
   AddPort% = &H2
   LenPort% = &H3
   ModeReg% = &H49
   CASE 2
   PgPort% = &H81
   AddPort% = &H4
   LenPort% = &H5
   ModeReg% = &H4A
   CASE 3
   PgPort% = &H82
   AddPort% = &H6
   LenPort% = &H7
   ModeReg% = &H4B
   CASE ELSE
   'print "DMA channels 0-3 only are supported."
   EXIT SUB
   END SELECT
   
   'print "1"
   
   OUT &HA, &H4 + Channel%
   OUT &HC, &H0
   OUT &HB, ModeReg%
   OUT AddPort%, MemLoc& AND &HFF
   OUT AddPort%, (MemLoc& AND &HFFFF&) \ &H100
   IF (MemLoc& AND 65536) THEN Page% = Page% + 1
   IF (MemLoc& AND 131072) THEN Page% = Page% + 2
   IF (MemLoc& AND 262144) THEN Page% = Page% + 4
   IF (MemLoc& AND 524288) THEN Page% = Page% + 8
   OUT PgPort%, Page%
   OUT LenPort%, Length& AND &HFF
   OUT LenPort%, (Length& AND &HFFFF&) \ &H100
   OUT &HA, Channel%
   
   'print "2"
   
   IF Freq& < 21000 THEN
      'print "3"
      TimeConst% = 256 - 1000000 \ Freq&
      WriteDSP &H40
      WriteDSP TimeConst%
      WriteDSP &H14
      WriteDSP (Length& AND &HFF)
      WriteDSP ((Length& AND &HFFFF&) \ &H100)
      ELSE
      'print "4"
      WriteDSP &HE1
      'print "5"
      
      'DO: LOOP UNTIL INP(BasePort% + 14) AND &H80
      ReadDSP% = INP(BasePort% + 10)
      Temp% = ReadDSP%
      Temp2% = ReadDSP%
      DSPVersion! = VAL(STR$(Temp%) + "." + STR$(Temp2%))
      'print "6"
      
      IF DSPVersion! >= 3 THEN
         'print "7"
         
         TimeConst% = ((65536 - 256000000 \ Freq&) AND &HFFFF&) \ &H100
         WriteDSP &H40
         'print "8"
         
         WriteDSP TimeConst%
         'print "9"
         
         WriteDSP (Length& AND &HFF)
         'print "10"
         
         WriteDSP ((Length& AND &HFFFF&) \ &H100)
         'print "11"
         
         WriteDSP &H91
         'print "12"
         
         ELSE
         'print "You need a Sound Blaster with a DSP v3.x+ to play at high speed."
         EXIT SUB
      END IF
   END IF
   'print "13"
   
END SUB

SUB DoPlayIt ' Plays the wave data if required
   
   Frequ& = 22050
   Length& = 22050
   DMAPlay VARSEG(WavBuffer$), VARPTR(WavBuffer$), Length&, Frequ&
END SUB

FUNCTION GetInteger%
   GetInteger% = CVI(INPUT$(2, #1))
END FUNCTION

SUB InitSnd
   ' Initialize Sound Blaster
   
   ' Get Sound Blaster Information
   FOR Length% = 1 TO LEN(ENVIRON$("BLASTER"))
      SELECT CASE MID$(ENVIRON$("BLASTER"), Length%, 1)
      CASE "A"
      BasePort% = VAL("&H" + MID$(ENVIRON$("BLASTER"), Length% + 1, 3))
      CASE "I"
      IRQ% = VAL(MID$(ENVIRON$("BLASTER"), Length% + 1, 1))
      CASE "D"
      DMA% = VAL(MID$(ENVIRON$("BLASTER"), Length% + 1, 1))
      Channel% = DMA%
      END SELECT
      BasePort% = &H240
      IRQ% = 5
      DMA% = 1
      Channel% = DMA%
   NEXT
   
   ' Resets the DSP
   OUT BasePort% + 6, 1
   FOR Count% = 1 TO 4
      junk% = INP(BasePort% + 6)
   NEXT
   OUT BasePort% + 6, 0
   
   WriteDSP &HD1 ' Turn Speaker on
   
   ' Set Volume:
   OUT BasePort% + 4, &H22
   OUT BasePort% + 5, (13 + 13 * 16) AND &HFF
END SUB

' ============================== QuickSort ===================================
'   QuickSort works by picking a random "pivot" element in AverageY, then
'   moving every element that is bigger to one side of the pivot, and every
'   element that is smaller to the other side.  QuickSort is then called
'   recursively with the two subdivisions created by the pivot.  Once the
'   number of elements in a subdivision reaches two, the recursive calls end
'   and the array is sorted.
' ============================================================================
'
SUB QuickSort (Low, High)
   IF Low < High THEN
      
      ' Only two elements in this subdivision; swap them if they are out of
      ' order, then end recursive calls:
      IF High - Low = 1 THEN
         IF AverageY(Low) > AverageY(High) THEN
            SWAP AverageY(Low), AverageY(High)
            SWAP PolygonsScreen(Low), PolygonsScreen(High)
            SWAP Tex(Low), Tex(High)
            SWAP Eff(Low), Eff(High)
         END IF
         ELSE
         
         ' Pick a pivot element at random, then move it to the end:
         RandIndex = RandInt%(Low, High)
         SWAP AverageY(High), AverageY(RandIndex)
         
         SWAP PolygonsScreen(High), PolygonsScreen(RandIndex)
         SWAP Tex(High), Tex(RandIndex)
         SWAP Eff(High), Eff(RandIndex)
         'SwapBars High, RandIndex
         Partition = AverageY(High)
         DO
            
            ' Move in from both sides towards the pivot element:
            I = Low: j = High
            DO WHILE (I < j) AND (AverageY(I) <= Partition)
            I = I + 1
         LOOP
         DO WHILE (j > I) AND (AverageY(j) >= Partition)
         j = j - 1
      LOOP
      
      ' If we haven't reached the pivot element, it means that two
      ' elements on either side are out of order, so swap them:
      IF I < j THEN
         SWAP AverageY(I), AverageY(j)
         SWAP PolygonsScreen(I), PolygonsScreen(j)
         SWAP Tex(I), Tex(j)
         SWAP Eff(I), Eff(j)
         
         'SwapBars i, J
      END IF
   LOOP WHILE I < j
   
   ' Move the pivot element back to its proper place in the array:
   SWAP AverageY(I), AverageY(High)
   SWAP PolygonsScreen(I), PolygonsScreen(High)
   SWAP Eff(I), Eff(High)
   SWAP Tex(I), Tex(High)
   'SwapBars i, High
   
   ' Recursively call the QuickSort procedure (pass the smaller
   ' subdivision first to use less stack space):
   IF (I - Low) < (High - I) THEN
      QuickSort Low, I - 1
      QuickSort I + 1, High
      ELSE
      QuickSort I + 1, High
      QuickSort Low, I - 1
   END IF
END IF
END IF
END SUB

REM $STATIC
' =============================== RandInt% ===================================
'   Returns a random integer greater than or equal to the Lower parameter
'   and less than or equal to the Upper parameter.
' ============================================================================
'
FUNCTION RandInt% (lower, upper) STATIC
   RandInt% = INT(RND * (upper - lower + 1)) + lower
END FUNCTION

SUB sortthem
   
   'FOR A% = 0 TO PolyGons%
   ' IF AverageY(A%) > 1000 THEN
   ' PolygonsScreen(A%).y1 = 0
   ' PolygonsScreen(A%).y2 = 0
   ' PolygonsScreen(A%).y3 = 0
   ' END IF
   'NEXT
   
END SUB

SUB WriteDSP (byte%)
   ' Writes a byte to the DSP
   DO: LOOP WHILE INP(BasePort% + 12) AND &H80
   OUT BasePort% + 12, byte%
END SUB

