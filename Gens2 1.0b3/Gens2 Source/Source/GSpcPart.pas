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

{Unit for the Space Partitioning Functions -
                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit GSpcPart;

Interface

Uses
     GTypes3D;

Procedure Determine_Zone_Locality;
Function Is_Polygon_In_Player_Zone(poly:lice):boolean;
Function Is_Vertex_In_Player_Zone(VertP:vertex):boolean;

Implementation

Uses
     Globals,
     GMath3D;

{Determine in which sphere zones the player is located}
Procedure Determine_Zone_Locality;
var
    Rastojanie_na_Player_do_centar_na_zona:array[1..maxzones] of single;
    numzones:Integer;
begin
  In_Zona_Count := 0;
  For numzones := 1 to ZoneCount do
     begin
       Rastojanie_na_Player_do_centar_na_zona[numzones] :=
                Safe_Sqrt(Sqr(CameraEye.x - Main_Zone_Center[numzones].x) +
                          Sqr(CameraEye.y - Main_Zone_Center[numzones].y) +
                          Sqr(CameraEye.z - Main_Zone_Center[numzones].z));
       if Rastojanie_na_Player_do_centar_na_zona[numzones] <= Sphere_Zone_Radius[numzones] then
         begin
           Inc(In_Zona_Count);
           Vo_koi_zoni_se_naogjam[In_Zona_Count] := numzones;
         end;
     end;
end;

Function Is_Polygon_In_Player_Zone(poly:lice):boolean;
var
    Rastojanie_na_poligonov_do_centarot_na_zonata:single;
    numzones,Current_Zone:Integer;
begin
  Result := false;
  If ZoneCount = 0 then
    begin
      Result := true;
      Exit;
    end;
  For numzones := 1 to In_Zona_Count do
     begin
       Current_Zone := Vo_koi_zoni_se_naogjam[numzones];
       Rastojanie_na_poligonov_do_centarot_na_zonata :=
              Safe_Sqrt(Sqr(Poly.T1.x - Sphere_Zone_Center[Current_Zone].x) +
                        Sqr(Poly.T1.y - Sphere_Zone_Center[Current_Zone].y) +
                        Sqr(Poly.T1.z - Sphere_Zone_Center[Current_Zone].z));
       if Rastojanie_na_poligonov_do_centarot_na_zonata <= Sphere_Zone_Radius[Current_Zone] then
         begin
           Result := true;
           Exit;
         end;
       Rastojanie_na_poligonov_do_centarot_na_zonata :=
              Safe_Sqrt(Sqr(Poly.T2.x - Sphere_Zone_Center[Current_Zone].x) +
                        Sqr(Poly.T2.y - Sphere_Zone_Center[Current_Zone].y) +
                        Sqr(Poly.T2.z - Sphere_Zone_Center[Current_Zone].z));
       if Rastojanie_na_poligonov_do_centarot_na_zonata <= Sphere_Zone_Radius[Current_Zone] then
         begin
           Result := true;
           Exit;
         end;
       Rastojanie_na_poligonov_do_centarot_na_zonata :=
              Safe_Sqrt(Sqr(Poly.T3.x - Sphere_Zone_Center[Current_Zone].x) +
                        Sqr(Poly.T3.y - Sphere_Zone_Center[Current_Zone].y) +
                        Sqr(Poly.T3.z - Sphere_Zone_Center[Current_Zone].z));
       if Rastojanie_na_poligonov_do_centarot_na_zonata <= Sphere_Zone_Radius[Current_Zone] then
         begin
           Result := true;
           Exit;
         end;
       Rastojanie_na_poligonov_do_centarot_na_zonata :=
              Safe_Sqrt(Sqr(Poly.T4.x - Sphere_Zone_Center[Current_Zone].x) +
                        Sqr(Poly.T4.y - Sphere_Zone_Center[Current_Zone].y) +
                        Sqr(Poly.T4.z - Sphere_Zone_Center[Current_Zone].z));
       if Rastojanie_na_poligonov_do_centarot_na_zonata <= Sphere_Zone_Radius[Current_Zone] then
         begin
           Result := true;
           Exit;
         end;
     end;
end;

Function Is_Vertex_In_Player_Zone(VertP:vertex):boolean;
var
    Rastojanie_na_vertex_do_centarot_na_zonata:single;
    Safe_res:single;
    numzones,Current_Zone:Integer;
begin
  Result := false;
  If ZoneCount = 0 then
    begin
      Result := true;
      Exit;
    end;
  For numzones := 1 to In_Zona_Count do
     begin
       Current_Zone := Vo_koi_zoni_se_naogjam[numzones];
       Safe_res := Sqr(VertP.x - Sphere_Zone_Center[Current_Zone].x) +
                   Sqr(VertP.y - Sphere_Zone_Center[Current_Zone].y) +
                   Sqr(VertP.z - Sphere_Zone_Center[Current_Zone].z);
       Rastojanie_na_vertex_do_centarot_na_zonata := Safe_Sqrt(Safe_res);
       if Rastojanie_na_vertex_do_centarot_na_zonata <= Sphere_Zone_Radius[Current_Zone] then
         begin
           Result := true;
           Exit;
         end;
     end;
end;

END.