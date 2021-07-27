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

{Unit for the Initialization functions -
                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit GInit;

Interface

Procedure Init_Engine;
Procedure Reset_Engine;

Implementation

Uses
     GTypes3D,
     Globals,
     Windows,
     GWinMnge,
     GDDraw7M,
     GLoadASE,
     GTextures,
     GPhysics,
     GKeysDef,
     GOut2Log;

{This PROCEDURE Initializes all Engine variables, loads  and sets up the 3-D WORLD}
Procedure Init_Engine;
Var
    strResult: String;
begin
  Wind := 0;  // Just to make sure it's not something else
  LoadingText_Counter := 0;
  Engine_Initialized := false;

  if NOT(GM_CreateWindow(hInstance, CmdShow, STARTWINDOW_WIDTH, STARTWINDOW_HEIGHT, true)) then
    begin
      DisplayMessage('Error creating main window !!!', WINDOW_TITLE, MSG_ERROR);
      Halt(0);
    end;

  Inc(LoadingText_Counter);
  WindowText(Engine_Version, 10, LoadingText_Counter * 16, 0, 255, 255);
  Inc(LoadingText_Counter);
  WindowText(Engine_Copyright, 10, LoadingText_Counter * 16, 0, 255, 255);
  Inc(LoadingText_Counter);
  Inc(LoadingText_Counter);
  WindowText('***  Hit F1 during rendering for On-Screen Help  ***', 10, LoadingText_Counter * 16, 0, 255, 255);
  Inc(LoadingText_Counter);
  Inc(LoadingText_Counter);

  StartEndTime;
  WindowText('Initializing Engine...', 10, LoadingText_Counter * 16, 255, 255, 255);
  Write2Log('///////////////////////////////////////////////////////////////////', true);
  Write2Log('Initializing Engine...', true);
  Inc(LoadingText_Counter);

  VK_Codes;

  SceneDime;

  Reset_Engine;

 {Texture Loading and Setup}
  LoadTextures;

 {Set up the FPS counter}
  ScrShotNo := 0;

  m_filter := true;
  sMouseSensitivity := 2;
  sKeybSensitivity := 2;
  Engine_Initialized := true;

  WriteSumTime;
  Write2Log('///////////////////////////////////////////////////////////////////', true);

  GM_DestroyWindow(hInstance);

  if NOT(GM_CreateWindow(hInstance, CmdShow, 0, 0, false)) then
    begin
      DisplayMessage('Error creating main window !!!', WINDOW_TITLE, MSG_ERROR);
      Halt(0);
    end;

  Vid_Mode := 3;
  Init_DirectDraw7(1024, 768, 32, Screen_Refresh_1024);
end;

{This PROCEDURE RESETS the Engine parameters and variables to DEFAULT settings}
Procedure Reset_Engine;
begin
  vsync:=false; showtris:=false; invertm:=true;
  pagehelpGens:=0; scrhelp:=false; lightsoff:=false; displaypolys:=false;
  fsaa:=false; animate:=false; endpageGens:=3; mipmap:=true; autoscrshot:=false;
  agolx:=(270-PlayerViewAngle.x)*pi/180; agoly:=(270+PlayerViewAngle.y)*pi/180;
  agolz:=-PlayerViewAngle.z*pi/180;  Mspeed:=0.005; Kspeed:=0.05;

  prpixel:=false; CastShadows:=true; lensfv:=true; noclip := false;
  GeneralLightIntensity:=200; FOV := DefaultFOV;
  PlayerMoveSpeed:=20; DynLightSpeed:=30; lighttype:=false;
  GeneralLightR:=200; GeneralLightG:=200; GeneralLightB:=200;

  {Main Moving and Viewing variables}
  CameraEye.x:=0; CameraEye.y:=0; CameraEye.z:=0;
  CameraDirectionAngle.x:=agolx; CameraDirectionAngle.y:=agoly;
  CameraDirectionAngle.z:=agolz; OldRendering := false;
  WorldAxisX.x:=1; WorldAxisX.y:=0; WorldAxisX.z:=0;
  WorldAxisY.x:=0; WorldAxisY.y:=1; WorldAxisY.z:=0;
  WorldAxisZ.x:=0; WorldAxisZ.y:=0; WorldAxisZ.z:=1;
  WorldAxisXTmp.x:=1; WorldAxisXTmp.y:=0; WorldAxisXTmp.z:=0;
  WorldAxisYTmp.x:=0; WorldAxisYTmp.y:=1; WorldAxisYTmp.z:=0;
  WorldAxisZTmp.x:=0; WorldAxisZTmp.y:=0; WorldAxisZTmp.z:=1;
  PlayerVectorMove.x:=0; PlayerVectorMove.y:=0.9; PlayerVectorMove.z:=1;

  PlayerVectorStrafe.x:=1; PlayerVectorStrafe.y:=0; PlayerVectorStrafe.z:=0;
  LightVectorMove.x:=0; LightVectorMove.y:=0; LightVectorMove.z:=1;
  LowResShadowEnable := true; LightTolerance := 3; ShadowTolerance := 3;
  ShadowType := 2; TimeStep := 3 / FreeFall_Gravity; GlobalTime := TimeStep;
  RayTraceSkipFactor := 4;
end;

END.