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

{Unit for Ray-Tracing Shadow Functions -
                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit GRShadow;

Interface

Uses
     GTypes3D;

Function RayShadow(LightSource,LightDestination:vertex; dbli,LightNo:Integer;
                   var AlphaPolygons:AlphaPols; var AlphaPolCount:Dword):Boolean;
Function RayShadowVertex(LightSource,LightDestination:vertex; dbli,LightNo:Integer; var AlphaPolygons:AlphaPols; var AlphaPolCount:Dword):Boolean;

Implementation

Uses
     Globals,
     GMath3D;

{This function tests if a light vector (ray) defined by LightSource and LightDestination,
 intersects a world polygon, to test if a shadow occurred.
 This function is used in the per-pixel shadowing algorithm}

Function RayShadow(LightSource,LightDestination:vertex; dbli,LightNo:Integer;
                   var AlphaPolygons:AlphaPols; var AlphaPolCount:Dword):Boolean;
Var TT,TTRec,FuckedF,Distance2Shadow,Distance2Intersection:single;
    Toc,Lightv,Pn:Vertex;
    NumP:Integer;
Begin
  Lightv.X:=LightDestination.X-LightSource.X;
  Lightv.Y:=LightDestination.Y-LightSource.Y;
  Lightv.Z:=LightDestination.Z-LightSource.Z;
  Distance2Shadow:=Lightv.X*Lightv.X+Lightv.Y*Lightv.Y+Lightv.Z*Lightv.Z;

  RayShadow := false;
  AlphaPolCount := 0;
  IF ShadowOccured[LightNo] then
   Begin
     ii:=PotentialShadowPoly[LightNo];
     IF dbli<>ii then
       Begin
         Pn := NormalLookup[ii];

         FuckedF:=Pn.X*Lightv.X+Pn.Y*Lightv.Y+Pn.Z*Lightv.Z;
         IF FuckedF<>0 then
           Begin
             TTRec:=Pn.X*(RotPoly[ii].T1.X-LightSource.X)+
                    Pn.Y*(RotPoly[ii].T1.Y-LightSource.Y)+
                    Pn.Z*(RotPoly[ii].T1.Z-LightSource.Z);
             TT:=TTRec/FuckedF;

             IF TT>0 then
               Begin
                 Toc.X:=LightSource.X+Lightv.X*TT;
                 Toc.Y:=LightSource.Y+Lightv.Y*TT;
                 Toc.Z:=LightSource.Z+Lightv.Z*TT;

                 Distance2Intersection:=sqr(Toc.X-LightSource.X)+
                                        sqr(Toc.Y-LightSource.Y)+
                                        sqr(Toc.Z-LightSource.Z);
                 IF Distance2Intersection<Distance2Shadow then
                   IF InsideTest(RotPoly[ii],Toc) then
                     if alphap[ii] > 0 then
                       begin
                         Inc(AlphaPolCount);
                         AlphaPolygons[AlphaPolCount] := ii;
                       end
                     else
                       begin
                         RayShadow:=true;
                         Exit;
                       end;
               End;
           End;
       End;
   End;

  ShadowOccured[LightNo]:=false;
  For NumP:=1 to RayPolys[LightNo] do
   Begin
     ii:=PolyNo[LightNo,NumP];
     IF dbli<>ii then
       Begin
         Pn := NormalLookup[ii];

         FuckedF:=Pn.X*Lightv.X+Pn.Y*Lightv.Y+Pn.Z*Lightv.Z;
         IF FuckedF<>0 then
           Begin
             TTRec:=Pn.X*(RotPoly[ii].T1.X-LightSource.X)+
                    Pn.Y*(RotPoly[ii].T1.Y-LightSource.Y)+
                    Pn.Z*(RotPoly[ii].T1.Z-LightSource.Z);
             TT:=TTRec/FuckedF;

             IF TT>0 then
               Begin
                 Toc.X:=LightSource.X+Lightv.X*TT;
                 Toc.Y:=LightSource.Y+Lightv.Y*TT;
                 Toc.Z:=LightSource.Z+Lightv.Z*TT;

                 Distance2Intersection:=sqr(Toc.X-LightSource.X)+
                                        sqr(Toc.Y-LightSource.Y)+
                                        sqr(Toc.Z-LightSource.Z);
                 IF Distance2Intersection<Distance2Shadow then
                   IF InsideTest(RotPoly[ii],Toc) then
                     if alphap[ii] > 0 then
                       begin
                         Inc(AlphaPolCount);
                         AlphaPolygons[AlphaPolCount] := ii;
                       end
                     else
                       begin
                         RayShadow:=true;
                         ShadowOccured[LightNo]:=true;
                         PotentialShadowPoly[LightNo]:=ii;
                         Exit;
                       end;
               End;
           End;
       End;
   End;
End;

{This function tests if a light vector (ray) defined by LightSource and LightDestination,
 intersects a world polygon, to test if a shadow occurred.
 This function is used in the per-vertex shadowing algorithm}

Function RayShadowVertex(LightSource,LightDestination:vertex; dbli,LightNo:Integer; var AlphaPolygons:AlphaPols; var AlphaPolCount:Dword):Boolean;
Var xu,yu,zu,xv,yv,zv,TT,TTRec,FuckedF,Distance2Shadow,Distance2Intersection:single;
    Toc,Lightv,Pn:Vertex;
    NumP:Integer;
Begin
  Lightv.X:=LightDestination.X-LightSource.X;
  Lightv.Y:=LightDestination.Y-LightSource.Y;
  Lightv.Z:=LightDestination.Z-LightSource.Z;
  Distance2Shadow:=Lightv.X*Lightv.X+Lightv.Y*Lightv.Y+Lightv.Z*Lightv.Z;

  RayShadowVertex := false;
  AlphaPolCount := 0;
  IF ShadowOccured[LightNo] then
   Begin
     ii:=PotentialShadowPoly[LightNo];
     IF dbli<>ii then
       Begin
         xu:=mainpoly[ii].t4.x-mainpoly[ii].t1.x;
         yu:=mainpoly[ii].t4.y-mainpoly[ii].t1.y;
         zu:=mainpoly[ii].t4.z-mainpoly[ii].t1.z;
         xv:=mainpoly[ii].t2.x-mainpoly[ii].t1.x;
         yv:=mainpoly[ii].t2.y-mainpoly[ii].t1.y;
         zv:=mainpoly[ii].t2.z-mainpoly[ii].t1.z;
         Pn.X:=yu*zv-zu*yv;
         Pn.Y:=zu*xv-xu*zv;
         Pn.Z:=xu*yv-yu*xv;

         FuckedF:=Pn.X*Lightv.X+Pn.Y*Lightv.Y+Pn.Z*Lightv.Z;
         IF FuckedF<>0 then
           Begin
             TTRec:=Pn.X*(mainpoly[ii].T1.X-LightSource.X)+
                    Pn.Y*(mainpoly[ii].T1.Y-LightSource.Y)+
                    Pn.Z*(mainpoly[ii].T1.Z-LightSource.Z);
             TT:=TTRec/FuckedF;

             IF TT>0 then
               Begin
                 Toc.X:=LightSource.X+Lightv.X*TT;
                 Toc.Y:=LightSource.Y+Lightv.Y*TT;
                 Toc.Z:=LightSource.Z+Lightv.Z*TT;

                 Distance2Intersection:=sqr(Toc.X-LightSource.X)+
                                        sqr(Toc.Y-LightSource.Y)+
                                        sqr(Toc.Z-LightSource.Z);
                 IF Distance2Intersection<Distance2Shadow then
                   IF InsideTest(mainpoly[ii],Toc) then
                     if alphap[ii] > 0 then
                       begin
                         Inc(AlphaPolCount);
                         AlphaPolygons[AlphaPolCount] := ii;
                       end
                     else
                       begin
                         RayShadowVertex:=true;
                         Exit;
                       end;
               End;
           End;
       End;
   End;

  ShadowOccured[LightNo]:=false;
  For NumP:=1 to RayPolys[LightNo] do
   Begin
     ii:=PolyNo[LightNo,NumP];
     IF dbli<>ii then
       Begin
         xu:=mainpoly[ii].t4.x-mainpoly[ii].t1.x;
         yu:=mainpoly[ii].t4.y-mainpoly[ii].t1.y;
         zu:=mainpoly[ii].t4.z-mainpoly[ii].t1.z;
         xv:=mainpoly[ii].t2.x-mainpoly[ii].t1.x;
         yv:=mainpoly[ii].t2.y-mainpoly[ii].t1.y;
         zv:=mainpoly[ii].t2.z-mainpoly[ii].t1.z;
         Pn.X:=yu*zv-zu*yv;
         Pn.Y:=zu*xv-xu*zv;
         Pn.Z:=xu*yv-yu*xv;

         FuckedF:=Pn.X*Lightv.X+Pn.Y*Lightv.Y+Pn.Z*Lightv.Z;
         IF FuckedF<>0 then
           Begin
             TTRec:=Pn.X*(mainpoly[ii].T1.X-LightSource.X)+
                    Pn.Y*(mainpoly[ii].T1.Y-LightSource.Y)+
                    Pn.Z*(mainpoly[ii].T1.Z-LightSource.Z);
             TT:=TTRec/FuckedF;

             IF TT>0 then
               Begin
                 Toc.X:=LightSource.X+Lightv.X*TT;
                 Toc.Y:=LightSource.Y+Lightv.Y*TT;
                 Toc.Z:=LightSource.Z+Lightv.Z*TT;

                 Distance2Intersection:=sqr(Toc.X-LightSource.X)+
                                        sqr(Toc.Y-LightSource.Y)+
                                        sqr(Toc.Z-LightSource.Z);
                 IF Distance2Intersection<Distance2Shadow then
                   IF InsideTest(mainpoly[ii],Toc) then
                     if alphap[ii] > 0 then
                       begin
                         Inc(AlphaPolCount);
                         AlphaPolygons[AlphaPolCount] := ii;
                       end
                     else
                       begin
                         RayShadowVertex:=true;
                         ShadowOccured[LightNo]:=true;
                         PotentialShadowPoly[LightNo]:=ii;
                         Exit;
                       end;
               End;
           End;
       End;
   End;
End;

END.