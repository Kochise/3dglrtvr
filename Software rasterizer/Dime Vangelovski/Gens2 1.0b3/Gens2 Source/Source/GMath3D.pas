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

{Unit for common mathematics related functions -
                                                                   - Made by Dime Vangelovski
                                                                                  29.08.2005}
Unit GMath3D;

Interface

Uses
     GTypes3D;

Function Dot_Product (a, b:vertex): Single;
Function Cross_Product (a, b:vertex): Vertex;
Function Safe_Sqrt(v:single):single;
Function Vector_Magnitude (vec: vertex):single;
Function Normalize_Vector (Vec:vertex): Vertex;
Function Cos_Theta (a, b:vertex): Single;
Function Digits_Count(Number:Extended):Integer;
Function InsideTest(VPoly: Lice; Xpoint: Vertex): Boolean;
Function Distance2Plane(Mtoc, Nvec, Ptoc:vertex):single;
Function Distance2Vector(VecStart,VecEnd,TestPoint:vertex):single;

Procedure CalcNormal(Poly:lice; var Normal:vertex);
Procedure TangentNormal(SCenter:vertex; SRadius:single; point:vertex; var TanNormal:vertex);
Procedure ClosestPointOnLine(vA, vB, vPoint : vertex; var ClsPoint:vertex);
Procedure ArbitraryRotation(OldPoint:vertex; var RotPoint:vertex; RotCenter,RotAngle:vertex);
Procedure rottoccka(stara:vertex; var rotirana:vertex; cx, sx, cy, sy, cz, sz:single);

Implementation

{$I Gdefines.inc}

{$ifdef __USE_3DNOW__}
Uses
     G3DNowFn;
{$endif}

Function Dot_Product (a, b:vertex): Single;
begin
  Result := a.x * b.x + a.y * b.y + a.z * b.z;
end;

Function Cross_Product (a, b:vertex): Vertex;
begin
  Result.x := a.y * b.z - a.z * b.y;
  Result.y := a.z * b.x - a.x * b.z;
  Result.z := a.x * b.y - a.y * b.x;
end;

{Function which ensures that SQRT doesn't get a ZERO or a NEGATIVE value}
Function Safe_Sqrt(v:single):single;
begin
  if (v = 0.0) then
    Result := 0.0001
  else
    Result := Sqrt(Abs(v));
end;

Function Vector_Magnitude (vec: vertex):single;
begin
  Result := Safe_Sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
end;

Function Normalize_Vector (Vec:vertex): Vertex;
var
    mag:single;
begin
  mag := Safe_Sqrt(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z);
  Result.x := Vec.x / mag;
  Result.y := Vec.y / mag;
  Result.z := Vec.z / mag;
end;

Function Cos_Theta (a, b:vertex): Single;
begin
  Result := (a.x * b.x + a.y * b.y + a.z * b.z) /
            (Safe_Sqrt((a.x * a.x + a.y * a.y + a.z * a.z) *
                       (b.x * b.x + b.y * b.y + b.z * b.z)));
end;

{Function that returns the number of digits in a number}
Function Digits_Count(Number:Extended):Integer;
Var
    Log: Extended;
begin
  if (Number = 0.0) then
    begin
      Result := 1;
      Exit;
    end;
  Log := Ln(Abs(Number)) / Ln(10.0);
  Result := Round(Log + 0.50000000000001);
end;

{$ifdef __USE_3DNOW__}
Function InsideTest(VPoly: Lice; Xpoint: Vertex): Boolean;
var
    Vec1, Vec2: Vertex;
    Dot: Single;
Begin
  Result := FALSE;

  Vec1.X := Xpoint.X - VPoly.T1.X;
  Vec1.Y := Xpoint.Y - VPoly.T1.Y;
  Vec1.Z := Xpoint.Z - VPoly.T1.Z;

  Vec2.X := VPoly.T2.X - VPoly.T1.X;
  Vec2.Y := VPoly.T2.Y - VPoly.T1.Y;
  Vec2.Z := VPoly.T2.Z - VPoly.T1.Z;

  Dot := Dot_3DNow (@Vec1, @Vec2);
  if (Dot < 0) then Exit;

  Vec1.X := Xpoint.X - VPoly.T2.X;
  Vec1.Y := Xpoint.Y - VPoly.T2.Y;
  Vec1.Z := Xpoint.Z - VPoly.T2.Z;

  Vec2.X := VPoly.T3.X - VPoly.T2.X;
  Vec2.Y := VPoly.T3.Y - VPoly.T2.Y;
  Vec2.Z := VPoly.T3.Z - VPoly.T2.Z;

  Dot := Dot_3DNow (@Vec1, @Vec2);
  if (Dot < 0) then Exit;

  Vec1.X := Xpoint.X - VPoly.T3.X;
  Vec1.Y := Xpoint.Y - VPoly.T3.Y;
  Vec1.Z := Xpoint.Z - VPoly.T3.Z;

  Vec2.X := VPoly.T4.X - VPoly.T3.X;
  Vec2.Y := VPoly.T4.Y - VPoly.T3.Y;
  Vec2.Z := VPoly.T4.Z - VPoly.T3.Z;

  Dot := Dot_3DNow (@Vec1, @Vec2);
  if (Dot < 0) then Exit;

  Vec1.X := Xpoint.X - VPoly.T4.X;
  Vec1.Y := Xpoint.Y - VPoly.T4.Y;
  Vec1.Z := Xpoint.Z - VPoly.T4.Z;

  Vec2.X := VPoly.T1.X - VPoly.T4.X;
  Vec2.Y := VPoly.T1.Y - VPoly.T4.Y;
  Vec2.Z := VPoly.T1.Z - VPoly.T4.Z;

  Dot := Dot_3DNow (@Vec1, @Vec2);
  if (Dot < 0) then Exit;

  Result := TRUE;
End;

{$else}

{Test if the point Xpoint is inside the polygon VPoly}
Function InsideTest(VPoly: Lice; Xpoint: Vertex): Boolean;
var
    Dot:single;
Begin
  Result := FALSE;

  Dot := (Xpoint.x - VPoly.T1.x) * (VPoly.T2.x - VPoly.T1.x) +
         (Xpoint.y - VPoly.T1.y) * (VPoly.T2.y - VPoly.T1.y) +
         (Xpoint.z - VPoly.T1.z) * (VPoly.T2.z - VPoly.T1.z);
  if (Dot < 0) then Exit;

  Dot := (Xpoint.x - VPoly.T2.x) * (VPoly.T3.x - VPoly.T2.x) +
         (Xpoint.y - VPoly.T2.y) * (VPoly.T3.y - VPoly.T2.y) +
         (Xpoint.z - VPoly.T2.z) * (VPoly.T3.z - VPoly.T2.z);
  if (Dot < 0) then Exit;

  Dot := (Xpoint.x - VPoly.T3.x) * (VPoly.T4.x - VPoly.T3.x) +
         (Xpoint.y - VPoly.T3.y) * (VPoly.T4.y - VPoly.T3.y) +
         (Xpoint.z - VPoly.T3.z) * (VPoly.T4.z - VPoly.T3.z);
  if (Dot < 0) then Exit;

  Dot := (Xpoint.x - VPoly.T4.x) * (VPoly.T1.x - VPoly.T4.x) +
         (Xpoint.y - VPoly.T4.y) * (VPoly.T1.y - VPoly.T4.y) +
         (Xpoint.z - VPoly.T4.z) * (VPoly.T1.z - VPoly.T4.z);
  if (Dot < 0) then Exit;

  Result := TRUE;
End;
{$endif}

{Calculates a Normal vector to the polygon Poly}
Procedure CalcNormal(Poly:lice; var Normal:vertex);
Var
    xu,yu,zu,xv,yv,zv,rac:single;
begin
  xu := Poly.t4.x - Poly.t1.x;
  yu := Poly.t4.y - Poly.t1.y;
  zu := Poly.t4.z - Poly.t1.z;
  xv := Poly.t2.x - Poly.t1.x;
  yv := Poly.t2.y - Poly.t1.y;
  zv := Poly.t2.z - Poly.t1.z;
  Normal.X := yu * zv - zu * yv;
  Normal.Y := zu * xv - xu * zv;
  Normal.Z := xu * yv - yu * xv;
  rac := Safe_Sqrt(Normal.X * Normal.X + Normal.Y * Normal.Y + Normal.Z * Normal.Z);
  Normal.X /:= rac;
  Normal.Y /:= rac;
  Normal.Z /:= rac;
end;

Procedure TangentNormal(SCenter:vertex; SRadius:single; point:vertex; var TanNormal:vertex);
var
    pv:vertex;
    r2,rac:single;
begin
  pv.x := point.x - SCenter.x;
  pv.y := point.y - SCenter.y;
  pv.z := point.z - SCenter.z;

  r2 := SRadius * SRadius;

  TanNormal.x := pv.x / r2;
  TanNormal.y := pv.y / r2;
  TanNormal.z := pv.z / r2;
  rac := Safe_Sqrt(TanNormal.X * TanNormal.X + TanNormal.Y * TanNormal.Y + TanNormal.Z * TanNormal.Z);
  TanNormal.X /:= rac;
  TanNormal.Y /:= rac;
  TanNormal.Z /:= rac;
end;

Function Distance2Plane(Mtoc, Nvec, Ptoc:vertex):single;
begin
  Result := Abs(Nvec.X * Mtoc.X + Nvec.Y * Mtoc.Y + Nvec.Z * Mtoc.Z -
                Nvec.X * Ptoc.X - Nvec.Y * Ptoc.Y - Nvec.Z * Ptoc.Z) /
                Safe_Sqrt(sqr(Nvec.X) + sqr(Nvec.Y) + sqr(Nvec.Z));
end;

Procedure ClosestPointOnLine(vA, vB, vPoint : vertex; var ClsPoint:vertex);
var
    vVector1,vVector2,vVector3:vertex;
    rac,d,t:single;
begin
  vVector1.x := vPoint.x - vA.x;
  vVector1.y := vPoint.y - vA.y;
  vVector1.z := vPoint.z - vA.z;

  vVector2.x := vB.x - vA.x;
  vVector2.y := vB.y - vA.y;
  vVector2.z := vB.z - vA.z;
  rac := Safe_Sqrt(vVector2.x * vVector2.x + vVector2.y * vVector2.y + vVector2.z * vVector2.z);
  vVector2.x /:= rac;
  vVector2.y /:= rac;
  vVector2.z /:= rac;

  d := Safe_Sqrt(sqr(vA.x - vB.x) + sqr(vA.y - vB.y) + sqr(vA.z - vB.z));

  t := vVector2.x * vVector1.x + vVector2.y * vVector1.y + vVector2.z * vVector1.z;

  if t <= 0 then
    begin
      ClsPoint := vA;
      Exit;
    end;

  if t >= d then
    begin
      ClsPoint := vB;
      Exit;
    end;

  vVector3.x := vVector2.x * t;
  vVector3.y := vVector2.y * t;
  vVector3.z := vVector2.z * t;

  ClsPoint.x := vA.x + vVector3.x;
  ClsPoint.y := vA.y + vVector3.y;
  ClsPoint.z := vA.z + vVector3.z;
end;

{Rotates a point for a given angle, but the rotation center can be anywhere}
Procedure ArbitraryRotation(OldPoint:vertex; var RotPoint:vertex; RotCenter,RotAngle:vertex);
var
    TranslateVector, Origin, TransPoint, TransRotPoint:vertex;
    Tmp1,Tmp2:single;
begin
  Origin.X := 0;
  Origin.Y := 0;
  Origin.Z := 0;

  TranslateVector.X := Origin.X - RotCenter.X;
  TranslateVector.Y := Origin.Y - RotCenter.Y;
  TranslateVector.Z := Origin.Z - RotCenter.Z;

  TransPoint.X := OldPoint.X + TranslateVector.X;
  TransPoint.Y := OldPoint.Y + TranslateVector.Y;
  TransPoint.Z := OldPoint.Z + TranslateVector.Z;


  TransRotPoint.X := TransPoint.X;
  TransRotPoint.Y := TransPoint.Y;
  TransRotPoint.Z := TransPoint.Z;

  Tmp1 := TransRotPoint.Y * Cos(RotAngle.X) - TransRotPoint.Z * Sin(RotAngle.X);
  Tmp2 := TransRotPoint.Z * Cos(RotAngle.X) + TransRotPoint.Y * Sin(RotAngle.X);
  TransRotPoint.Y := Tmp1;
  TransRotPoint.Z := Tmp2;

  Tmp1 := TransRotPoint.Z * Cos(RotAngle.Y) - TransRotPoint.X * Sin(RotAngle.Y);
  Tmp2 := TransRotPoint.X * Cos(RotAngle.Y) + TransRotPoint.Z * Sin(RotAngle.Y);
  TransRotPoint.Z := Tmp1;
  TransRotPoint.X := Tmp2;

  Tmp1 := TransRotPoint.X * Cos(RotAngle.Z) - TransRotPoint.Y * Sin(RotAngle.Z);
  Tmp2 := TransRotPoint.Y * Cos(RotAngle.Z) + TransRotPoint.X * Sin(RotAngle.Z);
  TransRotPoint.X := Tmp1;
  TransRotPoint.Y := Tmp2;


  RotPoint.X := TransRotPoint.X - TranslateVector.X;
  RotPoint.Y := TransRotPoint.Y - TranslateVector.Y;
  RotPoint.Z := TransRotPoint.Z - TranslateVector.Z;
end;

{This is the ROTATION PROCEDURE that rotates a VERTEX 'stara' in to 'rotirana'
 by angles 'agolx,agoly,agolz'. The cx,sx,... are the cosinus and sinus values}
Procedure rottoccka(stara:vertex; var rotirana:vertex; cx, sx, cy, sy, cz, sz:single);
var
    pom1,pom2:single;
begin
  rotirana.x := stara.x;
  rotirana.y := stara.y;
  rotirana.z := stara.z;

  {Compute the rotated vertices}
  pom1 := rotirana.y * cx - rotirana.z * sx;
  pom2 := rotirana.z * cx + rotirana.y * sx;
  rotirana.y := pom1;
  rotirana.z := pom2;
  pom1 := rotirana.z * cy - rotirana.x * sy;
  pom2 := rotirana.x * cy + rotirana.z * sy;
  rotirana.z := pom1;
  rotirana.x := pom2;
  pom1 := rotirana.x * cz - rotirana.y * sz;
  pom2 := rotirana.y * cz + rotirana.x * sz;
  rotirana.x := pom1;
  rotirana.y := pom2;
end;

Function Distance2Vector(VecStart,VecEnd,TestPoint:vertex):single;
var
    PVector,PM,VectP:vertex;
begin
  PVector.x := VecEnd.x - VecStart.x;
  PVector.y := VecEnd.y - VecStart.y;
  PVector.z := VecEnd.z - VecStart.z;
  PM.x := TestPoint.x - VecStart.x;
  PM.y := TestPoint.y - VecStart.y;
  PM.z := TestPoint.z - VecStart.z;

  VectP.x := PVector.y * PM.z - PVector.z * PM.y;
  VectP.y := PVector.z * PM.x - PVector.x * PM.z;
  VectP.z := PVector.x * PM.y - PVector.y * PM.x;

  Result := Safe_Sqrt(sqr(VectP.x) + sqr(VectP.y) + sqr(VectP.z)) /
            Safe_Sqrt(sqr(PVector.x) + sqr(PVector.y) + sqr(PVector.z));
end;

END.