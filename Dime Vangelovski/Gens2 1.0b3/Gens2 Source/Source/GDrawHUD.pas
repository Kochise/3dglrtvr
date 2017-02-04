/* **********************************************************
 * Copyright 2005 Dimitar Vangelovski
 *
 * This file is part of the Gens2 3D Engine.
 * 
 * The Gens2 3D Engine is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 * 
 * The Gens2 3D Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with the Gens2 3D Engine; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 * **********************************************************/

{Unit for HUD (Heads Up Display) drawing functions -
                                                                   - Made by Dime Vangelovski
                                                                                  01.09.2005}
Unit GDrawHUD;

Interface

Procedure cross32bpp(x,y:Integer; r,g,b:Dword; where:Pointer);
Procedure DrawFPS;
Procedure Show_HUD;
Procedure OnscrHelp;

Implementation

Uses
     Globals,
     GDDraw7M,
     GMath3D,
     GEffects,
     GDrwPrim;

{Draws the CrossHair on X,Y with RGB}
Procedure cross32bpp(x,y:Integer; r,g,b:Dword; where:Pointer);
begin
  line32bpp(x+1-4,y+1,x+1-2,y+1,0,0,0,where);
  line32bpp(x+1+2,y+1,x+1+4,y+1,0,0,0,where);
  line32bpp(x+1,y+1-2,x+1,y+1-4,0,0,0,where);
  line32bpp(x+1,y+1+2,x+1,y+1+4,0,0,0,where);
  tocka32bpp(x+1,y+1,0,0,0,where);
  line32bpp(x-4,y,x-2,y,r,g,b,where);
  line32bpp(x+2,y,x+4,y,r,g,b,where);
  line32bpp(x,y-2,x,y-4,r,g,b,where);
  line32bpp(x,y+2,x,y+4,r,g,b,where);
  tocka32bpp(x,y,r,g,b,where);
end;

{This PROCEDURE displays the FRAMES PER SECOND count on screen}
Procedure DrawFPS;
Var
   fpsX:Integer;
begin
  GetDCHandle;

  fpsX := Digits_Count(CurrentFPS) * 8;
  if (Res_Xend_p1 = 320) then
    begin
      WriteNo32bpp(CurrentFPS,0,0,Res_Xend - 53 - fpsX,6,0,0,0);
      WriteNo32bpp(CurrentFPS,0,0,Res_Xend - 54 - fpsX,5,255,255,255);
      WriteTxt32bpp('fps',Res_Xend - 50,6,0,0,0);
      WriteTxt32bpp('fps',Res_Xend - 51,5,255,255,255);
    end
  else
    begin
      WriteNo32bpp(CurrentFPS,0,0,Res_Xend - 103 - fpsX,16 + Vid_YMove_Factor,0,0,0);
      WriteNo32bpp(CurrentFPS,0,0,Res_Xend - 104 - fpsX,15 + Vid_YMove_Factor,255,255,255);
      WriteTxt32bpp('fps',Res_Xend - 100,16 + Vid_YMove_Factor,0,0,0);
      WriteTxt32bpp('fps',Res_Xend - 101,15 + Vid_YMove_Factor,255,255,255);
    end;

  ReleaseDCHandle;
end;

{This displays the current polygon count viewed (rendered) on screen, the number of lights and some other engine params}
Procedure Show_HUD;
var
   CastTotal: Integer;
begin
  GetDCHandle;

    LightCo:=litmax;
    if lightsoff then
      LightCo:=0;
    if (Res_Xend_p1 <> 320) then
    begin
      if lighttype then
        begin
          WriteTxt32bpp('Static Light Mode',31,Res_Yend - 20,0,0,0);
          WriteTxt32bpp('Static Light Mode',30,Res_Yend - 21,255,255,255);
        end
      else
        begin
          WriteTxt32bpp('Dynamic Light Mode',31,Res_Yend - 20,0,0,0);
          WriteTxt32bpp('Dynamic Light Mode',30,Res_Yend - 21,255,255,255);
        end;

      WriteNo32bpp(Res_Xend_p1,0,0, Res_Xend - 100, Res_Yend - 20,0,0,0);
      WriteNo32bpp(Res_Xend_p1,0,0, Res_Xend - 101, Res_Yend - 21,255,255,255);
      WriteTxt32bpp('x', Res_Xend - 67 + Vid_YMove_Factor div 2, Res_Yend - 20,0,0,0);
      WriteTxt32bpp('x', Res_Xend - 68 + Vid_YMove_Factor div 2, Res_Yend - 21,255,255,255);
      WriteNo32bpp(Res_Yend_p1,0,0, Res_Xend - 50, Res_Yend - 20,0,0,0);
      WriteNo32bpp(Res_Yend_p1,0,0, Res_Xend - 51, Res_Yend - 21,255,255,255);

      WriteTxt32bpp('Polygons on Screen',31,16,0,0,0);
      WriteNo32bpp(screenpolycount,0,0,184,16,0,0,0);
      WriteTxt32bpp('Polygons on Screen',30,15,255,255,255);
      WriteNo32bpp(screenpolycount,0,0,183,15,255,255,255);

      WriteTxt32bpp('World Lights Count:',31,31 + Vid_YMove_Factor,0,0,0);
      WriteTxt32bpp('World Lights Count:',30,30 + Vid_YMove_Factor,255,255,255);
      WriteNo32bpp(LightCo,0,0,166,31 + Vid_YMove_Factor,0,0,0);
      WriteNo32bpp(LightCo,0,0,165,30 + Vid_YMove_Factor,255,255,255);

      WriteTxt32bpp('Rendered:',201,31 + Vid_YMove_Factor,0,0,0);
      WriteTxt32bpp('Rendered:',200,30 + Vid_YMove_Factor,255,255,255);
      WriteNo32bpp(CurrentZoneLightCount,0,0,281,31 + Vid_YMove_Factor,0,0,0);
      WriteNo32bpp(CurrentZoneLightCount,0,0,280,30 + Vid_YMove_Factor,255,255,255);

      WriteTxt32bpp('Light Intensity',31,41 + Vid_YMove_Factor * 2,0,0,0);
      WriteNo32bpp(GeneralLightIntensity,0,0,161,41 + Vid_YMove_Factor * 2,0,0,0);
      WriteTxt32bpp('Light Intensity',30,40 + Vid_YMove_Factor * 2,255,255,255);
      WriteNo32bpp(GeneralLightIntensity,0,0,160,40 + Vid_YMove_Factor * 2,255,255,255);

      WriteTxt32bpp('Shadow Casting Polygons:',31,51 + Vid_YMove_Factor * 3,0,0,0);
      WriteTxt32bpp('Shadow Casting Polygons:',30,50 + Vid_YMove_Factor * 3,255,255,255);
      WriteTxt32bpp('Processed (All Lights):',31,61 + Vid_YMove_Factor * 4,0,0,0);
      WriteTxt32bpp('Processed (All Lights):',30,60 + Vid_YMove_Factor * 4,255,255,255);
      WriteTxt32bpp('Total:',31,71 + Vid_YMove_Factor * 5,0,0,0);
      WriteTxt32bpp('Total:',30,70 + Vid_YMove_Factor * 5,255,255,255);
      WriteNo32bpp(ShadowEnd+1-ShadowStart,0,0,91,71 + Vid_YMove_Factor * 5,0,0,0);
      WriteNo32bpp(ShadowEnd+1-ShadowStart,0,0,90,70 + Vid_YMove_Factor * 5,255,255,255);

      CastTotal := 0;
      For ii := 1 to litmax do
         CastTotal +:= RayPolys[ii];

      WriteNo32bpp(CastTotal,0,0,186,61 + Vid_YMove_Factor * 4,0,0,0);
      WriteNo32bpp(CastTotal,0,0,185,60 + Vid_YMove_Factor * 4,255,255,255);

      WriteTxt32bpp('Rays (All Lights):',31,91 + Vid_YMove_Factor * 7,0,0,0);
      WriteTxt32bpp('Rays (All Lights):',30,90 + Vid_YMove_Factor * 7,255,255,255);
      WriteNo32bpp(RaysCasted,0,0,156,91 + Vid_YMove_Factor * 7,0,0,0);
      WriteNo32bpp(RaysCasted,0,0,155,90 + Vid_YMove_Factor * 7,255,255,255);

      WriteTxt32bpp('ShadowTolerance:',31,101 + Vid_YMove_Factor * 8,0,0,0);
      WriteTxt32bpp('ShadowTolerance:',30,100 + Vid_YMove_Factor * 8,255,255,255);
      WriteNo32bpp(ShadowTolerance,0,0,156,101 + Vid_YMove_Factor * 8,0,0,0);
      WriteNo32bpp(ShadowTolerance,0,0,155,100 + Vid_YMove_Factor * 8,255,255,255);

      WriteTxt32bpp('Lights Rendered:',31,111 + Vid_YMove_Factor * 9,0,0,0);
      WriteTxt32bpp('Lights Rendered:',30,110 + Vid_YMove_Factor * 9,255,255,255);
      WriteNo32bpp(CurrentZoneLightCount,0,0,156,111 + Vid_YMove_Factor * 9,0,0,0);
      WriteNo32bpp(CurrentZoneLightCount,0,0,155,110 + Vid_YMove_Factor * 9,255,255,255);
    end else
    begin
      if pagehelpGens=0 then
      begin
        if lighttype then
          begin
            WriteTxt32bpp('Static Light Mode',21,186 - Vid_YMove_Factor,0,0,0);
            WriteTxt32bpp('Static Light Mode',20,185 - Vid_YMove_Factor,255,255,255);
          end
        else
          begin
            WriteTxt32bpp('Dynamic Light Mode',21,186 - Vid_YMove_Factor,0,0,0);
            WriteTxt32bpp('Dynamic Light Mode',20,185 - Vid_YMove_Factor,255,255,255);
          end;
      end;

      WriteTxt32bpp('320 x 200', 231, 186 - Vid_YMove_Factor, 0,0,0);
      WriteTxt32bpp('320 x 200', 230, 185 - Vid_YMove_Factor, 255,255,255);

      WriteTxt32bpp('Polygons on Screen',11,6,0,0,0);
      WriteNo32bpp(screenpolycount,0,0,164,6,0,0,0);
      WriteTxt32bpp('Polygons on Screen',10,5,255,255,255);
      WriteNo32bpp(screenpolycount,0,0,163,5,255,255,255);

      WriteTxt32bpp('World Lights Count:',11,16 + Vid_YMove_Factor,0,0,0);
      WriteTxt32bpp('World Lights Count:',10,15 + Vid_YMove_Factor,255,255,255);
      WriteNo32bpp(LightCo,0,0,146,16 + Vid_YMove_Factor,0,0,0);
      WriteNo32bpp(LightCo,0,0,145,15 + Vid_YMove_Factor,255,255,255);

      WriteTxt32bpp('Light Int',181,16 + Vid_YMove_Factor,0,0,0);
      WriteNo32bpp(GeneralLightIntensity,0,0,261,16 + Vid_YMove_Factor,0,0,0);
      WriteTxt32bpp('Light Int',180,15 + Vid_YMove_Factor,255,255,255);
      WriteNo32bpp(GeneralLightIntensity,0,0,260,15 + Vid_YMove_Factor,255,255,255);

      WriteTxt32bpp('Shadow Casting Polygons:',11,31 + Vid_YMove_Factor * 2,0,0,0);
      WriteTxt32bpp('Shadow Casting Polygons:',10,30 + Vid_YMove_Factor * 2,255,255,255);
      WriteTxt32bpp('Processed (All Lights):',11,41 + Vid_YMove_Factor * 3,0,0,0);
      WriteTxt32bpp('Processed (All Lights):',10,40 + Vid_YMove_Factor * 3,255,255,255);
      WriteTxt32bpp('Total:',11,51 + Vid_YMove_Factor * 4,0,0,0);
      WriteTxt32bpp('Total:',10,50 + Vid_YMove_Factor * 4,255,255,255);
      WriteNo32bpp(ShadowEnd+1-ShadowStart,0,0,91,51 + Vid_YMove_Factor * 4,0,0,0);
      WriteNo32bpp(ShadowEnd+1-ShadowStart,0,0,90,50 + Vid_YMove_Factor * 4,255,255,255);

      CastTotal := 0;
      For ii := 1 to litmax do
         CastTotal +:= RayPolys[ii];

      WriteNo32bpp(CastTotal,0,0,176,41 + Vid_YMove_Factor * 3,0,0,0);
      WriteNo32bpp(CastTotal,0,0,175,40 + Vid_YMove_Factor * 3,255,255,255);

      WriteTxt32bpp('Rays (All Lights):',11,61 + Vid_YMove_Factor * 5,0,0,0);
      WriteTxt32bpp('Rays (All Lights):',10,60 + Vid_YMove_Factor * 5,255,255,255);
      WriteNo32bpp(RaysCasted,0,0,136,61 + Vid_YMove_Factor * 5,0,0,0);
      WriteNo32bpp(RaysCasted,0,0,135,60 + Vid_YMove_Factor * 5,255,255,255);

      WriteTxt32bpp('ShadowTolerance:',11,71 + Vid_YMove_Factor * 6,0,0,0);
      WriteTxt32bpp('ShadowTolerance:',10,70 + Vid_YMove_Factor * 6,255,255,255);
      WriteNo32bpp(ShadowTolerance,0,0,136,71 + Vid_YMove_Factor * 6,0,0,0);
      WriteNo32bpp(ShadowTolerance,0,0,135,70 + Vid_YMove_Factor * 6,255,255,255);

      WriteTxt32bpp('Lights Rendered:',11,81 + Vid_YMove_Factor * 7,0,0,0);
      WriteTxt32bpp('Lights Rendered:',10,80 + Vid_YMove_Factor * 7,255,255,255);
      WriteNo32bpp(CurrentZoneLightCount,0,0,136,81 + Vid_YMove_Factor * 7,0,0,0);
      WriteNo32bpp(CurrentZoneLightCount,0,0,135,80 + Vid_YMove_Factor * 7,255,255,255);
    end;

  ReleaseDCHandle;
end;

{This displays ON SCREEN HELP and doing alpha blending over the entire screen
to add an efect}
Procedure OnscrHelp;
begin
  SurfaceLock;

  {Apply alpha blending over the entire screen for easier
  reading and to add an efect}
  fillrectfalpha(0,0,xcenter*2,ycenter*2,100,0,0,200,VirtBuffer);

  SurfaceUnlock;

  {Rotate the viewer camera}
  agolx +:= 0.03;

  GetDCHandle;

  {Display the first page from the on screen help}
  if pagehelpGens=1 then
  begin
    WriteTxt32bpp('Function Keys:',5,10 + Vid_YMove_Factor,255,255,255);
    WriteTxt32bpp('F1 - OnScr Help',5,30 + Vid_YMove_Factor * 2,255,255,255);
    WriteTxt32bpp('F2 - Show HUD',5,40 + Vid_YMove_Factor * 3,255,255,255);
    WriteTxt32bpp('F3 - Show Polys',5,50 + Vid_YMove_Factor * 4,255,255,255);
    WriteTxt32bpp('F5 - White Light',5,60 + Vid_YMove_Factor * 5,255,255,255);
    WriteTxt32bpp('F6 - Red Light',5,70 + Vid_YMove_Factor * 6,255,255,255);
    WriteTxt32bpp('F7 - Green Light',5,80 + Vid_YMove_Factor * 7,255,255,255);
    WriteTxt32bpp('F8 - Blue Light',5,90 + Vid_YMove_Factor * 8,255,255,255);
    WriteTxt32bpp('F9 - SCREENSHOT',5,100 + Vid_YMove_Factor * 9,255,255,255);
    WriteTxt32bpp('ESC - QUIT GENS',5,110 + Vid_YMove_Factor * 10,255,255,255);
    WriteTxt32bpp('F11,F12 - CHG RES',5,130 + Vid_YMove_Factor * 12,255,255,255);
    WriteTxt32bpp('HOME - RST ENGINE',5,140 + Vid_YMove_Factor * 13,255,255,255);
    WriteTxt32bpp('F4 - Tg. HdLights',5,150 + Vid_YMove_Factor * 14,255,255,255);
    WriteTxt32bpp('T - TG. STAT/DYNAM LIGHT MODE',30,170 + Vid_YMove_Factor * 16,255,255,255);

    if (Res_Xend_p1 = 320) then
      WriteTxt32bpp('P - Tg. PPx Raytrace',160,20 + Vid_YMove_Factor,255,255,255)
    else
      WriteTxt32bpp('P - Tg. PerPixel Raytrace',160,20 + Vid_YMove_Factor,255,255,255);
    WriteTxt32bpp('V - Tg. Vsync',160,30 + Vid_YMove_Factor * 2,255,255,255);
    WriteTxt32bpp('I - Tg. Inv Mouse',160,40 + Vid_YMove_Factor * 3,255,255,255);
    WriteTxt32bpp('F - Tg. FSAA',160,50 + Vid_YMove_Factor * 4,255,255,255);
    WriteTxt32bpp('L - Tg. Lights',160,60 + Vid_YMove_Factor * 5,255,255,255);
    WriteTxt32bpp('K - Tg. LightCoronas',160,70 + Vid_YMove_Factor * 6,255,255,255);
    WriteTxt32bpp('M - Tg. MipMapping',160,80 + Vid_YMove_Factor * 7,255,255,255);
    WriteTxt32bpp('PGUP/PGDN - PerPixel Shadow Quality',160,90 + Vid_YMove_Factor * 8,255,255,255);
    WriteTxt32bpp('C - Tg. Shadows',160,100 + Vid_YMove_Factor * 9,255,255,255);
    WriteTxt32bpp('O - FOV 90',160,110 + Vid_YMove_Factor * 10,255,255,255);
    WriteTxt32bpp('BkSpc - REMOVE LIGHT',160,120 + Vid_YMove_Factor * 11,255,255,255);
    WriteTxt32bpp('END - CENTER VIEW',160,140 + Vid_YMove_Factor * 13,255,255,255);
    WriteTxt32bpp('Q - AUTOSCRSHOTS',160,150 + Vid_YMove_Factor * 14,255,255,255);
    WriteTxt32bpp('Page  1/3',5,190 + Vid_YMove_Factor * 18,255,255,255);
    WriteTxt32bpp('Press F1 to Continue',150,190 + Vid_YMove_Factor * 18,255,255,255);
 end;

  {Display the second page from the on screen help}
  if pagehelpGens=2 then
  begin
    WriteTxt32bpp('* Fire Light - left mouse button',5,40,255,255,255);
    WriteTxt32bpp('* W,S,A,D - move and strafe',5,50 + Vid_YMove_Factor,255,255,255);
    WriteTxt32bpp('* Freelook with mouse or arrow keys',5,60 + Vid_YMove_Factor * 2,255,255,255);
    WriteTxt32bpp('* Right Mouse Button or Space - Fly Up/Jump',5,70 + Vid_YMove_Factor * 3,255,255,255);
    WriteTxt32bpp('* Delete or E - Fly Down',5,80 + Vid_YMove_Factor * 4,255,255,255);
    WriteTxt32bpp('* Y - Tg. No Clip (Gravity & Collision)',5,90 + Vid_YMove_Factor * 5,255,255,255);

    WriteTxt32bpp('* 9,0 - Light Intensity',5,110 + Vid_YMove_Factor * 7,255,255,255);
    WriteTxt32bpp('* Change Mouse Speed with [,]',5,120 + Vid_YMove_Factor * 8,255,255,255);
    WriteTxt32bpp('* Change Keyboard Speed with 7,8',5,130 + Vid_YMove_Factor * 9,255,255,255);
    WriteTxt32bpp('* Change Dyn.Light speed with 5,6',5,140 + Vid_YMove_Factor * 10,255,255,255);
    WriteTxt32bpp('* Change FOV with 3,4',5,150 + Vid_YMove_Factor * 11,255,255,255);
    WriteTxt32bpp('Page  2/3',5,190 + Vid_YMove_Factor * 15,255,255,255);
    WriteTxt32bpp('Press F1 to Continue',150,190 + Vid_YMove_Factor * 15,255,255,255);
  end;

  {Display the fourth page from the on screen help}
  if pagehelpGens=3 then
  begin
    WriteTxt32bpp(' About:',5,40,255,255,255);
    WriteTxt32bpp(Engine_Version,5,60 + Vid_YMove_Factor * 2,255,255,255);
    WriteTxt32bpp('  Copyright (c) 2004,2005 -',5,80 + Vid_YMove_Factor * 4,255,255,255);
    WriteTxt32bpp('                 - by Dime Vangelovski',5,100 + Vid_YMove_Factor * 6,255,255,255);
    WriteTxt32bpp('Page  3/3',5,190 + Vid_YMove_Factor * 15,255,255,255);
    WriteTxt32bpp('Press F1 to Exit ScrHelp',120,190 + Vid_YMove_Factor * 15,255,255,255);
  end;

  ReleaseDCHandle;
end;

END.