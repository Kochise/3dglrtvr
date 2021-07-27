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

{Unit for Engine Functions Control -
                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit GFunCont;

Interface

Procedure Rotating_Head_Light;
Procedure Common_Functions_Update;

Implementation

Uses
     GTypes3D,
     Globals,
     GVxLight,
     GMisc;

Var
    HeadLightRotate:single;

Procedure Rotating_Head_Light;
begin
  LightVertex[litmax].x := CameraEye.x + 200;
  LightVertex[litmax].y := CameraEye.y + 200 * sin(HeadLightRotate);
  LightVertex[litmax].z := CameraEye.z + 200 * cos(HeadLightRotate);
  LightR[litmax]:=GeneralLightR;
  LightG[litmax]:=GeneralLightG;
  LightB[litmax]:=GeneralLightB;
  LightIntensity[litmax]:=GeneralLightIntensity*1500;
  LightVertexTmp[litmax]:=LightVertex[litmax];
  mainlightv[litmax]:=LightVertex[litmax];
  LightShadow[litmax] := ShadowType;

  LightVertex[litmax-1].x := CameraEye.x + 200;
  LightVertex[litmax-1].y := CameraEye.y + 200 * sin(HeadLightRotate + Pi);
  LightVertex[litmax-1].z := CameraEye.z + 200 * cos(HeadLightRotate + Pi);
  LightR[litmax-1]:=GeneralLightB;
  LightG[litmax-1]:=GeneralLightR;
  LightB[litmax-1]:=GeneralLightG;
  LightIntensity[litmax-1]:=GeneralLightIntensity*1500;
  LightVertexTmp[litmax-1]:=LightVertex[litmax-1];
  mainlightv[litmax-1]:=LightVertex[litmax-1];
  LightShadow[litmax-1] := ShadowType;

  HeadLightRotate +:= 0.2;
  if litmax < 1 then
    HeadLight := false;
end;

Procedure Common_Functions_Update;
begin
  Dynamic_Light_Movement;

  if (ScrShot) or (autoscrshot) then
    begin
      Take_Screenshot;
      ScrShot := FALSE;
     end;

  if (HeadLight) then
    Rotating_Head_Light;

  // Check if Animate key is pressed => Do the Animation
  if (Animate) then
    agolx +:= 0.01;
end;

END.