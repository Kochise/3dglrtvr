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

{Unit for 3-D World Transform Functions -
                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit GTransfr;

Interface

Uses
     GTypes3D;

Procedure RotateObjects;
Procedure RotateWorld(CamAngles:Vertex);
Procedure TranslateWorld(CamEyePos:Vertex);

Implementation

Uses
     Globals,
     GMath3D;

{Rotate all the custom defined rotating objects in the world}
Procedure RotateObjects;
var
    RotObjectNo,RotObjectPol,RotAnglesIndex:integer;
    anglex,angley,anglez,cx,cy,cz,sx,sy,sz:single;
begin
  for RotObjectNo := 1 to RotObjectsMax do
     for RotObjectPol := 1 to RotObjs2Pol[RotObjectNo] do
        begin
          ArbitraryRotation(MainPolyWorld[Rot2World[RotObjectNo, RotObjectPol]].T1,
                            MainPoly[Rot2World[RotObjectNo, RotObjectPol]].T1,
                            RotCenter[RotObject2Center[RotObjectNo]], RotObjAngles[RotObjectNo]);
          ArbitraryRotation(MainPolyWorld[Rot2World[RotObjectNo, RotObjectPol]].T2,
                            MainPoly[Rot2World[RotObjectNo, RotObjectPol]].T2,
                            RotCenter[RotObject2Center[RotObjectNo]], RotObjAngles[RotObjectNo]);
          ArbitraryRotation(MainPolyWorld[Rot2World[RotObjectNo, RotObjectPol]].T3,
                            MainPoly[Rot2World[RotObjectNo, RotObjectPol]].T3,
                            RotCenter[RotObject2Center[RotObjectNo]], RotObjAngles[RotObjectNo]);
          ArbitraryRotation(MainPolyWorld[Rot2World[RotObjectNo, RotObjectPol]].T4,
                            MainPoly[Rot2World[RotObjectNo, RotObjectPol]].T4,
                            RotCenter[RotObject2Center[RotObjectNo]], RotObjAngles[RotObjectNo]);
        end;

  for RotAnglesIndex := 1 to RotObjectsMax do
     begin
       RotObjAngles[RotAnglesIndex].X +:= RotObjAnglesAdd[RotAnglesIndex].X;
       RotObjAngles[RotAnglesIndex].Y +:= RotObjAnglesAdd[RotAnglesIndex].Y;
       RotObjAngles[RotAnglesIndex].Z +:= RotObjAnglesAdd[RotAnglesIndex].Z;
     end;

  {Calculate the rotation angle and sin,cos for that angle}
  anglex := 90 * Pi / 180;
  angley := 90 * Pi / 180;
  anglez := 0;
  cx := sin(anglex);
  sx := cos(anglex);
  cy := sin(angley);
  sy := cos(angley);
  cz := sin(anglez);
  sz := cos(anglez);

  {Rotate the world vertices according to the calculated angle}
  for RotObjectNo := 1 to RotObjectsMax do
     for RotObjectPol := 1 to RotObjs2Pol[RotObjectNo] do
        begin
          rottoccka(MainPoly[Rot2World[RotObjectNo, RotObjectPol]].T1,
                    MainPoly[Rot2World[RotObjectNo, RotObjectPol]].T1, cx, sx, cy, sy, cz, sz);
          rottoccka(MainPoly[Rot2World[RotObjectNo, RotObjectPol]].T2,
                    MainPoly[Rot2World[RotObjectNo, RotObjectPol]].T2, cx, sx, cy, sy, cz, sz);
          rottoccka(MainPoly[Rot2World[RotObjectNo, RotObjectPol]].T3,
                    MainPoly[Rot2World[RotObjectNo, RotObjectPol]].T3, cx, sx, cy, sy, cz, sz);
          rottoccka(MainPoly[Rot2World[RotObjectNo, RotObjectPol]].T4,
                    MainPoly[Rot2World[RotObjectNo, RotObjectPol]].T4, cx, sx, cy, sy, cz, sz);
        end;
end;

{-----------------------------------------------------------------------------}
{--- Main Rotation PROCEDURE -------------------------------------------------}
{--- It Rotates all the vertices in the world by angles CamAngles ------------}
{-----------------------------------------------------------------------------}
Procedure RotateWorld(CamAngles:Vertex);
begin
  {Calculate the sin and cos from the Camera Direction Angles}
  cx:=sin(CamAngles.x);
  sx:=cos(CamAngles.x);
  cy:=sin(CamAngles.y);
  sy:=cos(CamAngles.y);
  cz:=sin(CamAngles.z);
  sz:=cos(CamAngles.z);

  {Rotate all the vertices in the world according to the cam angles}
  for ii:=1 to polmax do
     begin
       rottoccka(poligon[ii].t1,rotpoly[ii].t1,cx,sx,cy,sy,cz,sz);
       rottoccka(poligon[ii].t2,rotpoly[ii].t2,cx,sx,cy,sy,cz,sz);
       rottoccka(poligon[ii].t3,rotpoly[ii].t3,cx,sx,cy,sy,cz,sz);
       rottoccka(poligon[ii].t4,rotpoly[ii].t4,cx,sx,cy,sy,cz,sz);
     end;
  for ii:=1 to litmax do
     rottoccka(LightVertexTmp[ii],LightVertex[ii],cx,sx,cy,sy,cz,sz);

  {Rotate zone centers according to the cam angles}
  for ii:=1 to ZoneCount do
     rottoccka(Sphere_Zone_Center_Tmp[ii],Sphere_Zone_Center[ii],cx,sx,cy,sy,cz,sz);

  {Rotate the Dynamic light rocket and the World Axis to
  accomodate for the viewer movement}
  rottoccka(WorldAxisXTmp,WorldAxisX,cx,sx,cy,sy,cz,sz);
  rottoccka(WorldAxisYTmp,WorldAxisY,cx,sx,cy,sy,cz,sz);
  rottoccka(WorldAxisZTmp,WorldAxisZ,cx,sx,cy,sy,cz,sz);
end;

{-----------------------------------------------------------------------------}
{--- Main Translation PROCEDURE ----------------------------------------------}
{--- It Translates (Moves) all the vertices in the world to Camera -----------}
{--- Eye position given by CamEyePos -----------------------------------------}
{-----------------------------------------------------------------------------}
Procedure TranslateWorld(CamEyePos:Vertex);
begin
  {Translate the world Vertices according to Camera Eye Position}
  for ii:=1 to polmax do
     begin
       poligon[ii].t1.x:=mainpoly[ii].t1.x-CamEyePos.x;
       poligon[ii].t2.x:=mainpoly[ii].t2.x-CamEyePos.x;
       poligon[ii].t3.x:=mainpoly[ii].t3.x-CamEyePos.x;
       poligon[ii].t4.x:=mainpoly[ii].t4.x-CamEyePos.x;

       poligon[ii].t1.y:=mainpoly[ii].t1.y-CamEyePos.y;
       poligon[ii].t2.y:=mainpoly[ii].t2.y-CamEyePos.y;
       poligon[ii].t3.y:=mainpoly[ii].t3.y-CamEyePos.y;
       poligon[ii].t4.y:=mainpoly[ii].t4.y-CamEyePos.y;

       poligon[ii].t1.z:=mainpoly[ii].t1.z-CamEyePos.z;
       poligon[ii].t2.z:=mainpoly[ii].t2.z-CamEyePos.z;
       poligon[ii].t3.z:=mainpoly[ii].t3.z-CamEyePos.z;
       poligon[ii].t4.z:=mainpoly[ii].t4.z-CamEyePos.z;
     end;

  {Translate the Fixed Light Vertices according to Camera Eye Position}
  for ii:=1 to litmax do
     begin
       LightVertexTmp[ii].x:=mainlightv[ii].x-CamEyePos.x;
       LightVertexTmp[ii].y:=mainlightv[ii].y-CamEyePos.y;
       LightVertexTmp[ii].z:=mainlightv[ii].z-CamEyePos.z;
     end;

  {Translate the Fixed Light Vertices according to Camera Eye Position}
  for ii:=1 to ZoneCount do
     begin
       Sphere_Zone_Center_Tmp[ii].x := Main_Zone_Center[ii].x - CamEyePos.x;
       Sphere_Zone_Center_Tmp[ii].y := Main_Zone_Center[ii].y - CamEyePos.y;
       Sphere_Zone_Center_Tmp[ii].z := Main_Zone_Center[ii].z - CamEyePos.z;
     end;
end;

END.