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

{Unit for Input Control Events (Keyboard and Mouse) -
                                                                      - Made by Dime Vangelovski
                                                                                     05.09.2005}
Unit GInput;

Interface

Procedure Key_AutoRepeat_INPUT;
Procedure Key_INPUT(Key_Code:Integer; Repetitions:Integer);
Procedure Mouse_Buttons_INPUT(Button:Integer; Repetitions:Integer);

Implementation

Uses
     GTypes3D,
     Globals,
     Windows,
     GDDraw7M,
     GKeysDef,
     GVxLight,
     GInit;

Procedure Key_AutoRepeat_INPUT;
begin
  // [,] - This is for decreasing/increasing the Mouse speed
  if (KeyState(key_zin)) and (Mspeed > 0.0001) then
    Mspeed -:= 0.0005;
  if (KeyState(key_zout)) and (Mspeed < 1) then
    Mspeed +:= 0.0005;

  // 7,8 - This is for decreasing/increasing the Keyboard speed
  if (KeyState(key_sedum)) and (Kspeed > 0.001) then
    Kspeed -:= 0.005;
  if (KeyState(key_osum)) and (Kspeed < 1) then
    Kspeed +:= 0.005;

  // 3,4 - This is for decreasing/increasing the FOV
  if (KeyState(key_tri)) and (FOV > MinFOV) then
    Dec(FOV, 2);
  if (KeyState(key_cetiri)) and (FOV < MaxFOV) then
    Inc(FOV, 2);

  // 5,6 - This is for decreasing/increasing the Dynamic Light Rocket Speed
  if (KeyState(key_pet)) and (DynLightSpeed > 1) then
    DynLightSpeed -:= 0.5;
  if (KeyState(key_ses)) and (DynLightSpeed < 50) then
    DynLightSpeed +:= 0.5;

  // <,> - This is for decreasing/increasing the shadow tolerance factor (used for optimization)
  if (KeyState(key_pomalo)) and (ShadowTolerance > 0) then
    Dec(ShadowTolerance);
  if (KeyState(key_pogolemo)) and (ShadowTolerance < Max_Shadow_Toler) then
    Inc(ShadowTolerance);

  // 9,0 - This is for decreasing/increasing the General Light Intensity
  if (KeyState(key_devet)) and (GeneralLightIntensity > 0) then
    Dec(GeneralLightIntensity, 50);
  if (KeyState(key_nula)) and (GeneralLightIntensity < 10000) then
    Inc(GeneralLightIntensity, 50);
end;

{This is the Functions Control PROCEDURE for the engine
 interactive Functions and Commands}
Procedure Key_INPUT(Key_Code:Integer; Repetitions:Integer);
begin
  // F5 - This makes the Dynamic Light Rocket to cast a White Light
  if (Key_Code = VK_F5) then
    begin
      GeneralLightR := 200;
      GeneralLightG := 200;
      GeneralLightB := 200;
    end;

  // F6 - This makes the Dynamic Light Rocket to cast a Red Light
  if (Key_Code = VK_F6) then
    begin
      GeneralLightR := 200;
      GeneralLightG := 0;
      GeneralLightB := 0;
    end;

  // F7 - This makes the Dynamic Light Rocket to cast a Green Light
  if (Key_Code = VK_F7) then
    begin
      GeneralLightR := 0;
      GeneralLightG := 200;
      GeneralLightB := 0;
    end;

  // F8 - This makes the Dynamic Light Rocket to cast a Blue Light
  if (Key_Code = VK_F8) then
    begin
      GeneralLightR := 0;
      GeneralLightG := 0;
      GeneralLightB := 200;
    end;

  // This is for removing all static lights, one by one, starting from the last created
  if (Key_Code = VK_BACK) and NOT(Repetitions > 1) then
    begin
      Is_Dynamic[litmax] := false;
      LightShadow[litmax] := 0;
      dec(litmax);
      if litmax < 0 then litmax := 0;
    end;

  // Q - Toggles Auto ScreenShots
  if (Key_Code = key_q) and NOT(Repetitions > 1) then
    autoscrshot := NOT(autoscrshot);

  // F3 - This toggles outline drawing on the polygons
  if (Key_Code = VK_F3) and NOT(Repetitions > 1) then
    showtris := NOT(showtris);

  // F2 - This toggles the on screen polygon count Display
  if (Key_Code = VK_F2) and NOT(Repetitions > 1) then
    displaypolys := NOT(displaypolys);

  // V - This toggles the Vsync
  if (Key_Code = key_v) and NOT(Repetitions > 1) then
    vsync := NOT(vsync);

  // Y - This toggles no clip mode
  if (Key_Code = key_y) and NOT(Repetitions > 1) then
    noclip := NOT(noclip);

  // I - This toggles Invert Mouse (vertical)
  if (Key_Code = key_i) and NOT(Repetitions > 1) then
    invertm := NOT(invertm);

  // F - This toggles Full Screen AntiAliasing (FSAA)
  if (Key_Code = key_f) and NOT(Repetitions > 1) then
    fsaa := NOT(fsaa);

  // M - This toggles MipMapping
  if (Key_Code = key_m) and NOT(Repetitions > 1) then
    mipmap := NOT(mipmap);

  // F11 - cycles down resolutions
   if (Key_Code = VK_F11) and NOT(Repetitions > 1) then
     begin
       Dec(Vid_Mode);
       if Vid_Mode < 0 then Vid_Mode := 4;
       Case (Vid_Mode) of
         0: Set_Mode(320, 200, 32, Screen_Refresh_320);
         1: Set_Mode(512, 384, 32, Screen_Refresh_512);
         2: Set_Mode(640, 480, 32, Screen_Refresh_640);
         3: Set_Mode(1024, 768, 32, Screen_Refresh_1024);
         4: Set_Mode(1280, 960, 32, Screen_Refresh_1280);
       end;
     end;

  // F12 - cycles up resolutions
   if (Key_Code = VK_F12) and NOT(Repetitions > 1) then
     begin
       Inc(Vid_Mode);
       if Vid_Mode > 4 then Vid_Mode := 0;
       Case (Vid_Mode) of
         0: Set_Mode(320, 200, 32, Screen_Refresh_320);
         1: Set_Mode(512, 384, 32, Screen_Refresh_512);
         2: Set_Mode(640, 480, 32, Screen_Refresh_640);
         3: Set_Mode(1024, 768, 32, Screen_Refresh_1024);
         4: Set_Mode(1280, 960, 32, Screen_Refresh_1280);
       end;
     end;

   // T - Toggles wheather you fire static or dynamic light
   if (Key_Code = key_t) and NOT(Repetitions > 1) then
     lighttype := NOT(lighttype);

   // K - Toggles Lens Flares
   if (Key_Code = key_k) and NOT(Repetitions > 1) then
     lensfv := NOT(lensfv);

   // F4 - Toggles Head Lights
   if (Key_Code = VK_F4) and NOT(Repetitions > 1) then
     if HeadLight then
       begin
         if litmax > 1 then
           begin
             HeadLight:=false;
             Dec(litmax, 2);
           end;
       end
     else
       begin
         if litmax < 99 then
           begin
             HeadLight:=true;
             Inc(litmax, 2);
           end;
       end;

   // T - Toggles PerPixel Lighting
   if (Key_Code = key_p) and NOT(Repetitions > 1) then
     prpixel := NOT(prpixel);

   // C - Toggles Shadow Casting
   if (Key_Code = key_c) and NOT(Repetitions > 1) then
     CastShadows := NOT(CastShadows);

   // Page Down - Toggles Shadow LowQuality
//   if (Key_Code = VK_NEXT) and NOT(Repetitions > 1) then
//     LowResShadowEnable := NOT(LowResShadowEnable);

   // PG_DN,PG_UP - This is for decreasing/increasing the shadow quality (used for optimization)
   if (Key_Code = VK_PRIOR) and NOT(Repetitions > 1) and (RayTraceSkipFactor > 1) then
     Dec(RayTraceSkipFactor);
   if (Key_Code = VK_NEXT) and NOT(Repetitions > 1) and (RayTraceSkipFactor < 16) then
     Inc(RayTraceSkipFactor);

   // R - Toggles Old and New Rendering
   if (Key_Code = key_r) and NOT(Repetitions > 1) then
     OldRendering := NOT(OldRendering);

   // END - This centers the view vertically
   if (Key_Code = VK_END) then
     agoly := 270 * Pi / 180;

   // O - This resets the FOV to 90
   if (Key_Code = key_o) then
     FOV := DefaultFOV;

   // L - This toggles Static Lights ON/OFF
   if (Key_Code = key_l) and NOT(Repetitions > 1) then
     lightsoff := NOT(lightsoff);

   // N - This toggles Rotating Animation
   if (Key_Code = key_n) and NOT(Repetitions > 1) then
     animate := NOT(animate);

   // F1 - This toggles On Screen Help
   if (Key_Code = VK_F1) and NOT(Repetitions > 1) then
     if (scrhelp) and (pagehelpGens = endpageGens) then
       begin
         scrhelp := false;
         pagehelpGens := 0;
       end
     else
       begin
         Inc(pagehelpGens);
         scrhelp := true;
       end;

   // R - This Resets the Engine
   if (Key_Code = VK_HOME) then
     Reset_Engine;
end;

Procedure Mouse_Buttons_INPUT(Button:Integer; Repetitions:Integer);
begin
  // This is for adding Static Lights
  if (Button = 1) and NOT(Repetitions > 1) and
     (lighttype) and (litmax < maxlights) then
    AddStaticLight;

  // This is the Rocket Camera Control
  if (Button = 1) and NOT(Repetitions > 1) and
      NOT(lighttype) and (litmax<maxlights) then
    begin
      AddStaticLight;
      MovingLightCoordinate[litmax] := CameraEye;
      LightMovingFactor[litmax] := DynamicLightVelocity;
      Is_Dynamic[litmax] := true;
    end;
end;

END.