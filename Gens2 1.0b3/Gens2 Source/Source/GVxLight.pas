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

{Unit for the Lights Handling Functions -
                                                                      - Made by Dime Vangelovski
                                                                                     05.09.2005}
Unit GVxLight;

Interface

Procedure AddStaticLight;
Procedure Dynamic_Light_Movement;

Implementation

Uses
     GTypes3D,
     Globals,
     GPhysics,
     GMath3D;

{Procedure for adding static lights}
Procedure AddStaticLight;
begin
  inc(litmax);
  LightVertex[litmax]:=CameraEye;
  LightR[litmax]:=GeneralLightR;
  LightG[litmax]:=GeneralLightG;
  LightB[litmax]:=GeneralLightB;
  LightIntensity[litmax]:=GeneralLightIntensity*1500;
  LightVertexTmp[litmax]:=LightVertex[litmax];
  mainlightv[litmax]:=LightVertex[litmax];
  LightShadow[litmax] := ShadowType;
end;

Procedure Dynamic_Light_Movement;
begin
   // Calculate the movement vectors for the Dynamic Light(s)
   DynLightVec.x:=(WorldAxisX.x*LightVectorMove.x+
                   WorldAxisX.y*LightVectorMove.y+
                   WorldAxisX.z*LightVectorMove.z);

   DynLightVec.y:=(WorldAxisY.x*LightVectorMove.x+
                   WorldAxisY.y*LightVectorMove.y+
                   WorldAxisY.z*LightVectorMove.z);

   DynLightVec.z:=(WorldAxisZ.x*LightVectorMove.x+
                   WorldAxisZ.y*LightVectorMove.y+
                   WorldAxisZ.z*LightVectorMove.z);

   DynamicLightVelocity.x := DynLightVec.x * DynLightSpeed;
   DynamicLightVelocity.y := DynLightVec.y * DynLightSpeed;
   DynamicLightVelocity.z := DynLightVec.z * DynLightSpeed;

   // This checks for a Dynamic Light and advances it
   For ii := 1 to litmax do
      if Is_Dynamic[ii] then
        begin
          MovingLightCoordinate[ii].x +:= LightMovingFactor[ii].x;
          MovingLightCoordinate[ii].y +:= LightMovingFactor[ii].y;
          MovingLightCoordinate[ii].z +:= LightMovingFactor[ii].z;

          LightVertex[ii] := MovingLightCoordinate[ii];
          LightVertexTmp[ii] := LightVertex[ii];
          mainlightv[ii] := LightVertex[ii];
        end;

   For ii := 1 to litmax do
      if (Is_Dynamic[ii]) and (Sphere_Polygon_Collision(mainlightv[ii], 50, NewPos)) then
        begin
          NewDynFactor.x := (NewPos.x - mainlightv[ii].x) + LightMovingFactor[ii].x;
          NewDynFactor.y := (NewPos.y - mainlightv[ii].y) + LightMovingFactor[ii].y;
          NewDynFactor.z := (NewPos.z - mainlightv[ii].z) + LightMovingFactor[ii].z;
          VectorMagnitude := Vector_Magnitude (NewDynFactor);
//          VectorMagnitude := Mag_Vect_3DNow (@NewDynFactor);
          NewDynFactor.x /:= VectorMagnitude;
          NewDynFactor.y /:= VectorMagnitude;
          NewDynFactor.z /:= VectorMagnitude;

          LightMovingFactor[ii].x := NewDynFactor.x * DynLightSpeed;
          LightMovingFactor[ii].y := NewDynFactor.y * DynLightSpeed;
          LightMovingFactor[ii].z := NewDynFactor.z * DynLightSpeed;

          // Weaken the light intensity with every bounce
          //if LightIntensity[ii] > 0 then
          //  LightIntensity[ii] := Round(LightIntensity[ii] / 1.03);
        end;
end;

END.