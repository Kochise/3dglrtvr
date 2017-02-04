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

{Unit for the Camera Control -
                                                                      - Made by Dime Vangelovski
                                                                                     05.09.2005}
Unit GCamera;

Interface

Procedure Keyboard_Look;
Procedure Mouse_Look;
Procedure Key_Move;
Procedure Camera_Update;

Implementation

Uses
     GTypes3D,
     Globals,
     Windows,
     GKeysDef;

Var
    xpos,ypos:Integer;
    mousedx,mousedy,old_mouse_x,old_mouse_y: Integer;
    mouse_x,mouse_y: Single;
    current_pos: TPoint;

// Keyboard Controls for FreeLook
Procedure Keyboard_Look;
begin
  xpos := 0;
  ypos := 0;
  if KeyState(VK_LEFT) then
    Dec(xpos);
  if KeyState(VK_RIGHT) then
    Inc(xpos);
  if KeyState(VK_UP) then
    Dec(ypos);
  if KeyState(VK_DOWN) then
    Inc(ypos);

  xpos := round ( xpos * sKeybSensitivity );
  ypos := round ( ypos * sKeybSensitivity );

  agolx := agolx - (xpos * Kspeed);
  if invertm then
    agoly := agoly - (ypos * Kspeed)
  else
    agoly := agoly + (ypos * Kspeed);

  if agoly < Pi + 0.001 then agoly := Pi + 0.001;
  if agoly > 2 * Pi - 0.001 then agoly := 2 * Pi - 0.001;
end;

// Mouse Controls for FreeLook
Procedure Mouse_Look;
begin
  if NOT(GetCursorPos (current_pos)) then
    Exit;

  mousedx := current_pos.x - xcenter;
  mousedy := current_pos.y - ycenter;

  if m_filter then
    begin
      mouse_x := (mousedx + old_mouse_x) * 0.5;
      mouse_y := (mousedy + old_mouse_y) * 0.5;
    end
  else
    begin
      mouse_x := mousedx;
      mouse_y := mousedy;
    end;

  old_mouse_x := mousedx;
  old_mouse_y := mousedy;

  mouse_x := mouse_x * sMouseSensitivity;
  mouse_y := mouse_y * sMouseSensitivity;

  agolx := agolx - (mouse_x * Mspeed);
  if (invertm) then
    agoly := agoly - (mouse_y * Mspeed)
  else
    agoly := agoly + (mouse_y * Mspeed);

  // Force the mouse cursor to the center of the screen to have more room to move
  if (mousedx <> 0) or (mousedy <> 0) then
    SetCursorPos (xcenter, ycenter);

  if agoly < Pi + 0.001 then agoly := Pi + 0.001;
  if agoly > 2 * Pi - 0.001 then agoly := 2 * Pi - 0.001;
end;

// Camera Move Procedure
Procedure Key_Move;
begin
  if NOT(noclip) then
    begin
      // Adjust for motion when viewing up/down
      if agoly > 3 * Pi / 2 then
        PlayerVectorMove.y := -0.9
      else
        PlayerVectorMove.y := 0.9;
    end
  else
    PlayerVectorMove.y := 0;


   // Calculate the forward-backward moving vector
   PlayerMove.x:=(WorldAxisX.x*PlayerVectorMove.x+
                  WorldAxisX.y*PlayerVectorMove.y+
                  WorldAxisX.z*PlayerVectorMove.z)*PlayerMoveSpeed;

   PlayerMove.y:=(WorldAxisY.x*PlayerVectorMove.x+
                  WorldAxisY.y*PlayerVectorMove.y+
                  WorldAxisY.z*PlayerVectorMove.z)*PlayerMoveSpeed;

   PlayerMove.z:=(WorldAxisZ.x*PlayerVectorMove.x+
                  WorldAxisZ.y*PlayerVectorMove.y+
                  WorldAxisZ.z*PlayerVectorMove.z)*PlayerMoveSpeed;

   if KeyState(key_w) then
     begin
       if noclip then
         CameraEye.x +:= PlayerMove.x;
       CameraEye.y +:= PlayerMove.y;
       CameraEye.z +:= PlayerMove.z;
     end;

   if KeyState(key_s) then
     begin
       if noclip then
         CameraEye.x -:= PlayerMove.x;
       CameraEye.y -:= PlayerMove.y;
       CameraEye.z -:= PlayerMove.z;
     end;

   // Calculate the left-right moving vector
   PlayerStrafe.x:=(WorldAxisX.x*PlayerVectorStrafe.x+
                    WorldAxisX.y*PlayerVectorStrafe.y+
                    WorldAxisX.z*PlayerVectorStrafe.z)*PlayerMoveSpeed;

   PlayerStrafe.y:=(WorldAxisY.x*PlayerVectorStrafe.x+
                    WorldAxisY.y*PlayerVectorStrafe.y+
                    WorldAxisY.z*PlayerVectorStrafe.z)*PlayerMoveSpeed;

   PlayerStrafe.z:=(WorldAxisZ.x*PlayerVectorStrafe.x+
                    WorldAxisZ.y*PlayerVectorStrafe.y+
                    WorldAxisZ.z*PlayerVectorStrafe.z)*PlayerMoveSpeed;

   if KeyState(key_a) then
     begin
       if noclip then
         CameraEye.x -:= PlayerStrafe.x;
       CameraEye.y -:= PlayerStrafe.y;
       CameraEye.z -:= PlayerStrafe.z;
     end;

   if KeyState(key_d) then
     begin
       if noclip then
         CameraEye.x +:= PlayerStrafe.x;
       CameraEye.y +:= PlayerStrafe.y;
       CameraEye.z +:= PlayerStrafe.z;
     end;
end;

Procedure Camera_Update;
begin
  Keyboard_Look;

  Mouse_Look;

  CameraDirectionAngle.x := agolx;
  CameraDirectionAngle.y := agoly;
  CameraDirectionAngle.z := agolz;

  Key_Move;
end;

END.