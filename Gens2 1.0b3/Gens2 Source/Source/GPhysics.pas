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

{Unit the sphere collision detection routines and basic physics like gravity and jumping -

                                                                      - Made by Dime Vangelovski
                                                                                     26.04.2005}
Unit GPhysics;

Interface

Uses
     GTypes3D;

Const
      Player_Sphere_Radius = 100;
      FreeFall_Gravity     = 9.81;
      Gravity_on_Ground    = 3;
      Max_Steepness        = 60;    {Maximal steepness the player can climb - in degrees}

Var
    GlobalTime,TimeStep,Gravity:single;
    Player_Standing_on_Ground:boolean;

Function Check_if_on_Ground(TestNormal:vertex):boolean;
Function Calc_Sphere_Intersection_Point(CurrentPolygon:Integer; SphereCenter:vertex; SphereRadius:single; var IntPoint:vertex):byte;
Function EdgeSphereCollision(SphereCenter:vertex; SphereRadius:single; CurrentPolygon:Integer; var Int_Point:vertex):boolean;
Procedure GetCollisionBounceOffset(SphereCenter:vertex; SphereRadius:single; IntersectedPolygon:Integer; var vBounceNormal:vertex);
Procedure GetCollisionBounceOffset_Edge(SphereCenter:vertex; SphereRadius:single; IntersectionP:vertex; var eBounceNormal:vertex);
Function Sphere_Polygon_Collision(SphereCenter:vertex; SphereRadius:single; var NewPosition:vertex):boolean;
Procedure Player_Physics_Update;

Implementation

Uses
     Globals,
     Windows,
     GMath3D,
     GKeysDef;

{Checks if the player is standing on a horizontal ground}
Function Check_if_on_Ground(TestNormal:vertex):boolean;
var VertVec:vertex;
    DotCos:single;
begin
  Check_if_on_Ground := false;
  VertVec.x := -1;
  DotCos := VertVec.x * TestNormal.x;
  if (DotCos > cos(Max_Steepness * Pi / 180)) then
    Check_if_on_Ground := true;
end;

Function Calc_Sphere_Intersection_Point(CurrentPolygon:Integer; SphereCenter:vertex; SphereRadius:single; var IntPoint:vertex):byte;
var PolyNormal:vertex;
    Dist:single;
begin
  Calc_Sphere_Intersection_Point := 2;   // By default we take that the sphere is BEHIND the plane
  CalcNormal(mainpoly[CurrentPolygon], PolyNormal);
  Dist := Distance2Plane(SphereCenter, PolyNormal, mainpoly[CurrentPolygon].T1);

  if Abs(Dist) < SphereRadius then
    begin
      PolyNormal.x *:= Dist;
      PolyNormal.y *:= Dist;
      PolyNormal.z *:= Dist;

      SphereCenter.x -:= PolyNormal.x;
      SphereCenter.y -:= PolyNormal.y;
      SphereCenter.z -:= PolyNormal.z;
      IntPoint := SphereCenter;
      Calc_Sphere_Intersection_Point := 0;  // The Sphere is INTERSECTING the plane
      Exit;
    end
  else
    if Dist >= SphereRadius then
      Calc_Sphere_Intersection_Point := 1;  // The Sphere is in FRONT of the plane
end;

{Checks if a sphere is colliding with a polygon edge}
Function EdgeSphereCollision(SphereCenter:vertex; SphereRadius:single; CurrentPolygon:Integer; var Int_Point:vertex):boolean;
var SphereDistance:single;
begin
  EdgeSphereCollision := false;
  // Go through all of the vertices in the polygon
  ClosestPointOnLine(mainpoly[CurrentPolygon].T1, mainpoly[CurrentPolygon].T2, SphereCenter, Int_Point);
  SphereDistance := Safe_Sqrt(sqr(Int_Point.x - SphereCenter.x) + sqr(Int_Point.y - SphereCenter.y) + sqr(Int_Point.z - SphereCenter.z));
  if SphereDistance < SphereRadius then
    begin
      EdgeSphereCollision := true;
      Exit;
    end;
  ClosestPointOnLine(mainpoly[CurrentPolygon].T2, mainpoly[CurrentPolygon].T3, SphereCenter, Int_Point);
  SphereDistance := Safe_Sqrt(sqr(Int_Point.x - SphereCenter.x) + sqr(Int_Point.y - SphereCenter.y) + sqr(Int_Point.z - SphereCenter.z));
  if SphereDistance < SphereRadius then
    begin
      EdgeSphereCollision := true;
      Exit;
    end;
  ClosestPointOnLine(mainpoly[CurrentPolygon].T3, mainpoly[CurrentPolygon].T4, SphereCenter, Int_Point);
  SphereDistance := Safe_Sqrt(sqr(Int_Point.x - SphereCenter.x) + sqr(Int_Point.y - SphereCenter.y) + sqr(Int_Point.z - SphereCenter.z));
  if SphereDistance < SphereRadius then
    begin
      EdgeSphereCollision := true;
      Exit;
    end;
  ClosestPointOnLine(mainpoly[CurrentPolygon].T4, mainpoly[CurrentPolygon].T1, SphereCenter, Int_Point);
  SphereDistance := Safe_Sqrt(sqr(Int_Point.x - SphereCenter.x) + sqr(Int_Point.y - SphereCenter.y) + sqr(Int_Point.z - SphereCenter.z));
  if SphereDistance < SphereRadius then
    begin
      EdgeSphereCollision := true;
      Exit;
    end;
end;

{Calculates a bounce vector (how much to go back) when a collision occured}
Procedure GetCollisionBounceOffset(SphereCenter:vertex; SphereRadius:single; IntersectedPolygon:Integer; var vBounceNormal:vertex);
var Sphere_Distance,BounceFactor:single;
begin
  CalcNormal(mainpoly[IntersectedPolygon], vBounceNormal);
  Sphere_Distance := Distance2Plane(SphereCenter, vBounceNormal, mainpoly[IntersectedPolygon].T1);
  BounceFactor := SphereRadius - Sphere_Distance;

  vBounceNormal.x *:= BounceFactor;
  vBounceNormal.y *:= BounceFactor;
  vBounceNormal.z *:= BounceFactor;
end;

{Calculates a bounce vector (how much to go back) when a collision occured - when a polygon edge is colided}
Procedure GetCollisionBounceOffset_Edge(SphereCenter:vertex; SphereRadius:single; IntersectionP:vertex; var eBounceNormal:vertex);
var eSphere_Distance,eBounceFactor:single;
begin
  TangentNormal(SphereCenter, SphereRadius, IntersectionP, eBounceNormal);
  eSphere_Distance := Distance2Plane(SphereCenter, eBounceNormal, IntersectionP);
  eBounceFactor := SphereRadius - eSphere_Distance;

  eBounceNormal.x *:= eBounceFactor;
  eBounceNormal.y *:= eBounceFactor;
  eBounceNormal.z *:= eBounceFactor;
end;

{Tests if a collision occured, and gives back the new calculated sphere position}
Function Sphere_Polygon_Collision(SphereCenter:vertex; SphereRadius:single; var NewPosition:vertex):boolean;
var Intersection_Point,BounceNormal,Edge_Intersection,GroundNormal:vertex;
    CurrentPolygon:Integer;
Begin
  Player_Standing_on_Ground := false;
  Sphere_Polygon_Collision := false;
  For CurrentPolygon := 1 to polmax do
     if Calc_Sphere_Intersection_Point(CurrentPolygon, SphereCenter, SphereRadius, Intersection_Point) = 0 then
       if InsideTest(mainpoly[CurrentPolygon], Intersection_Point) then
         begin
           CalcNormal(mainpoly[CurrentPolygon], GroundNormal);
           if Check_if_on_Ground(GroundNormal) then
             Player_Standing_on_Ground := true;
           GetCollisionBounceOffset(SphereCenter, SphereRadius, CurrentPolygon, BounceNormal);
{           if NOT((Safe_Sqrt(Sqr(SphereCenter.x - Intersection_Point.x) +
                             Sqr(SphereCenter.y - Intersection_Point.y) +
                             Sqr(SphereCenter.z - Intersection_Point.z)) < SphereRadius / 4) and (BounceNormal.x < 0)) then}
           SphereCenter.x -:= BounceNormal.x;
           SphereCenter.y -:= BounceNormal.y;
           SphereCenter.z -:= BounceNormal.z;
           Sphere_Polygon_Collision := true;
         end
       else
         if EdgeSphereCollision(SphereCenter, SphereRadius, CurrentPolygon, Edge_Intersection) then
           begin
//             CalcNormal(mainpoly[CurrentPolygon], GroundNormal);                      // Uncomment for standard normal
             TangentNormal(SPhereCenter, SphereRadius, Edge_Intersection, GroundNormal);// Remove these lines if you
             if Check_if_on_Ground(GroundNormal) then                                   // don't want Tomb Raider style
               Player_Standing_on_Ground := true;                                       // edge-wall climbing/catching
             GetCollisionBounceOffset_Edge(SphereCenter, SphereRadius, Edge_Intersection, BounceNormal);
             SphereCenter.x -:= BounceNormal.x;
             SphereCenter.y -:= BounceNormal.y;
             SphereCenter.z -:= BounceNormal.z;
             Sphere_Polygon_Collision := true;
           end;
  NewPosition := SphereCenter;
End;

{Add all the physics stuff :) }
Procedure Player_Physics_Update;
begin
   // This is for jumping - just temporary improvisation
   // FIXME: Implement REAL, physically correct jumping !!!
   if (KeyState(VK_SPACE)) or (KeyState(VK_RBUTTON)) then
     CameraEye.x +:= PlayerMoveSpeed;

   // Check for collision and add gravity
   if NOT(noclip) then
     begin
       // Check for collision with every poligon in the world
       Sphere_Polygon_Collision(CameraEye, Player_Sphere_Radius, CameraEye);

       if Player_Standing_on_Ground then
         begin
           GlobalTime := TimeStep;
           Gravity := Gravity_on_Ground;
         end
       else
         begin
           GlobalTime +:= TimeStep;
           Gravity := FreeFall_Gravity;
         end;
       CameraEye.x -:= Gravity * GlobalTime;
     end;
end;

END.