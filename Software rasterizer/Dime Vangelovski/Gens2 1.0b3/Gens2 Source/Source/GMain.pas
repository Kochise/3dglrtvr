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

/*
  GENS II 3-D Engine  ver 1.0  Beta 3 (Build 200) -

                                               -  Made by Dime Vangelovski,
                                                     Republic of Macedonia
                                                                11.09.2005
*/

Program GMain;

Uses
     GTypes3D,
     Globals,
     Windows,
     Messages,
     GWinMnge,
     GDDraw7M,
     GPolDraw,
     GCamera,
     GPhysics,
     GInput,
     GEffects,
     GDrawHUD,
     GDrwPrim,
     GFunCont,
     GInit,
     GOut2Log;

{$R Gens2.res}

Procedure Render_Frame;
begin
  // Clear the Z-buffer
  Clear_Z_Buffer;

  // Lock the Rendering surface
  SurfaceLock;

  // Clear the Screen
  Cls32bpp(0, 0, 0, VirtBuffer);

  // Render the scene at current camera view
  Render_Scene(CameraEye, CameraDirectionAngle);

  // FSAA Function
  If fsaa then
    Fsa32bpp(VirtBuffer);

  // Draw the Crosshair
  Cross32bpp(xcenter,ycenter,255,255,255,VirtBuffer);

  // Unlock the Rendering surface
  SurfaceUnlock;

  // Show Head Up Display
  if displaypolys then
    Show_HUD;

  // On ScreenHelp Function
  If scrhelp then
    OnScrHelp;

  // Write the FPS count on Screen
  DrawFPS;

  // Player Camera Menagement
  Camera_Update;

  // Check for collision, add gravity, jumping...
  Player_Physics_Update;

  // Check for repeated keyboard events
  Key_AutoRepeat_INPUT;

  // Update some common functions
  Common_Functions_Update;

  // Flip the Rendered Image from Virtual Screen to Graphical Screen
  FlipSurfaces;

  // Increase the FPS Count
  Inc(FPS);
end;


/******************************************************************************
***** The MAIN Program ********************************************************
******************************************************************************/

Var
    msg: TMsg;
    dwStart: Dword;

BEGIN // MAIN

  // Initialize the Engine
  Init_Engine;

  // Main application loop (message loop)
  WHILE (TRUE) DO
    BEGIN
      if (PeekMessage(msg, 0, 0, 0, PM_REMOVE)) then
        begin
          if (msg.message = WM_QUIT) then
            begin
              Write2Log('WM_QUIT message received inside message loop ... ', true);
              Write2Log('Exiting Gens2 ... ', true);
              Write2Log('', true);
              StartEndTime;
              Break;
            end;
          TranslateMessage(msg);
          DispatchMessage(msg);
        end
      else
        // Frame rendering
        if (g_AppActive) and NOT(Window_Closed) then
          begin
            // Read the Frame Start Time
            dwStart := GetTickCount;

            // Render next frame
            Render_Frame;

            // Lock the Frame Rate to 25 FPS
            while ((GetTickCount - dwStart) < 32) do begin end;
          end
        else
          WaitMessage;
    END;
END. // MAIN