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

{Unit for the Ray-Trace Shadow Optimization Functions -
                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit GShOptmz;

Interface

Uses
     GTypes3D;

Function ClipShadowCastingPolygon(poly:lice; slight:vertex):boolean;
Procedure Optimize_Viewable_Polygons_Lighting(ii:Integer);
Function MinDistance(LightNo,PolyNo:Dword):Single;
Procedure SortShadowCastingPolygons;
Procedure ComputeNormals;
Procedure Optimize_Shadow_Casting_Objects;
Procedure Tweak_Shadow_Tolerance;

Implementation

Uses
     Globals,
     GMath3D,
     GSpcPart;

Function ClipShadowCastingPolygon(poly:lice; slight:vertex):boolean;
var maxclip,zclipfov1,zclipfov2,zclipfov3,zclipfov4,zclipfov5,
    sclipfov,lv,mv,nv,TFactor,RayDiv,RayLength,TestVFov,
    StranaA,StranaB,SrStrana,SpecFov,FOVFac:single;
    Sredna,TestV,LMVertex,FactorV1,FactorV2,LMStart,LMEnd,FactorU:vertex;
    NoCount,NNX,NNY,VertexN:Integer;
    NadvorE:boolean;
begin
  ClipShadowCastingPolygon := FALSE;

  Sredna.X := (Poly.T1.X + Poly.T3.X) / 2;
  Sredna.Y := (Poly.T1.Y + Poly.T3.Y) / 2;
  Sredna.Z := (Poly.T1.Z + Poly.T3.Z) / 2;

  {Advance the Z value according to the FOV transition}
  zclipfov1 := FOVtan * poly.t1.z;
  zclipfov2 := FOVtan * poly.t2.z;
  zclipfov3 := FOVtan * poly.t3.z;
  zclipfov4 := FOVtan * poly.t4.z;
  zclipfov5 := FOVtan * Sredna.Z;
  sclipfov := FOVtan * slight.z;

  NadvorE := TRUE;
  StranaA := Safe_Sqrt(sqr(poly.t2.x - poly.t1.x) + sqr(poly.t2.y - poly.t1.y) + sqr(poly.t2.z - poly.t1.z));
  StranaB := Safe_Sqrt(sqr(poly.t4.x - poly.t1.x) + sqr(poly.t4.y - poly.t1.y) + sqr(poly.t4.z - poly.t1.z));
  SrStrana := (StranaA + StranaB) / 2;
  if SrStrana > 100 then
    begin
      VertexN := Round(SrStrana / 40);
      FactorV1.X := (Poly.T2.X - poly.T1.X) / VertexN;
      FactorV1.Y := (poly.T2.Y - poly.T1.Y) / VertexN;
      FactorV1.Z := (poly.T2.Z - poly.T1.Z) / VertexN;
      FactorV2.X := (poly.T3.X - poly.T4.X) / VertexN;
      FactorV2.Y := (poly.T3.Y - poly.T4.Y) / VertexN;
      FactorV2.Z := (poly.T3.Z - poly.T4.Z) / VertexN;
      LMStart := poly.T1;
      LMEnd := poly.T4;
      For NNY := 0 to VertexN do
         begin
           if NNY > 0 then
             begin
               LMStart.X +:= FactorV1.X;
               LMStart.Y +:= FactorV1.Y;
               LMStart.Z +:= FactorV1.Z;
               LMEnd.X +:= FactorV2.X;
               LMEnd.Y +:= FactorV2.Y;
               LMEnd.Z +:= FactorV2.Z;
             end;
           FactorU.X := (LMEnd.X - LMStart.X) / VertexN;
           FactorU.Y := (LMEnd.Y - LMStart.Y) / VertexN;
           FactorU.Z := (LMEnd.Z - LMStart.Z) / VertexN;
           LMVertex := LMStart;
           For NNX := 0 to VertexN do
              begin
                if NNX > 0 then
                  begin
                    LMVertex.X +:= FactorU.X;
                    LMVertex.Y +:= FactorU.Y;
                    LMVertex.Z +:= FactorU.Z;
                  end;
                SpecFov := FOVtan * LMVertex.Z;
                if NOT((LMVertex.x < -SpecFov) or (LMVertex.x > SpecFov) or
                       (LMVertex.y < -SpecFov) or (LMVertex.y > SpecFov) or
                       (LMVertex.z < NearZClip) or (LMVertex.z > FarZClip)) then
                  begin
                    NadvorE := false;
                    Break;
                  end;
              end;
         end;
    end
  else
    if NOT(((poly.t1.x < -zclipfov1) or (poly.t1.x > zclipfov1) or
            (poly.t1.y < -zclipfov1) or (poly.t1.y > zclipfov1) or
            (poly.t1.z < NearZClip) or (poly.t1.z > FarZClip)) and
           ((poly.t2.x < -zclipfov2) or (poly.t2.x > zclipfov2) or
            (poly.t2.y < -zclipfov2) or (poly.t2.y > zclipfov2) or
            (poly.t2.z < NearZClip) or (poly.t2.z > FarZClip)) and
           ((poly.t3.x < -zclipfov3) or (poly.t3.x > zclipfov3) or
            (poly.t3.y < -zclipfov3) or (poly.t3.y > zclipfov3) or
            (poly.t3.z < NearZClip) or (poly.t3.z > FarZClip)) and
           ((poly.t4.x < -zclipfov4) or (poly.t4.x > zclipfov4) or
            (poly.t4.y < -zclipfov4) or (poly.t4.y > zclipfov4) or
            (poly.t4.z < NearZClip) or (poly.t4.z > FarZClip)) and
           ((sredna.x < -zclipfov5) or (sredna.x > zclipfov5) or
            (sredna.y < -zclipfov5) or (sredna.y > zclipfov5) or
            (sredna.z < NearZClip) or (sredna.z > FarZClip))) then
      NadvorE := false;

  if NadvorE then
    if (((slight.x > -sclipfov) and (slight.x < sclipfov)) and
       ((slight.y > -sclipfov) and (slight.y < sclipfov)) and
       ((slight.z > NearZClip) and (slight.z < FarZClip))) then
      begin
        ClipShadowCastingPolygon := true;
        Exit;
      end
    else
      begin
        StranaA := Safe_Sqrt(sqr(poly.t2.x - poly.t1.x) + sqr(poly.t2.y - poly.t1.y) + sqr(poly.t2.z - poly.t1.z));
        StranaB := Safe_Sqrt(sqr(poly.t4.x - poly.t1.x) + sqr(poly.t4.y - poly.t1.y) + sqr(poly.t4.z - poly.t1.z));
        SrStrana := (StranaA + StranaB) / 2;
        if SrStrana > 100 then
          begin
            ClipShadowCastingPolygon := true;
            RayDiv := 50;
            RayLength := 5000;
            VertexN := Round(SrStrana / 40);
            FactorV1.X := (Poly.T2.X - poly.T1.X) / VertexN;
            FactorV1.Y := (poly.T2.Y - poly.T1.Y) / VertexN;
            FactorV1.Z := (poly.T2.Z - poly.T1.Z) / VertexN;
            FactorV2.X := (poly.T3.X - poly.T4.X) / VertexN;
            FactorV2.Y := (poly.T3.Y - poly.T4.Y) / VertexN;
            FactorV2.Z := (poly.T3.Z - poly.T4.Z) / VertexN;
            LMStart := poly.T1;
            LMEnd := poly.T4;
            For NNY := 0 to VertexN do
               begin
                 if NNY > 0 then
                   begin
                     LMStart.X +:= FactorV1.X;
                     LMStart.Y +:= FactorV1.Y;
                     LMStart.Z +:= FactorV1.Z;
                     LMEnd.X +:= FactorV2.X;
                     LMEnd.Y +:= FactorV2.Y;
                     LMEnd.Z +:= FactorV2.Z;
                   end;
                 FactorU.X := (LMEnd.X - LMStart.X) / VertexN;
                 FactorU.Y := (LMEnd.Y - LMStart.Y) / VertexN;
                 FactorU.Z := (LMEnd.Z - LMStart.Z) / VertexN;
                 LMVertex := LMStart;
                 For NNX := 0 to VertexN do
                    begin
                      if NNX > 0 then
                        begin
                          LMVertex.X +:= FactorU.X;
                          LMVertex.Y +:= FactorU.Y;
                          LMVertex.Z +:= FactorU.Z;
                        end;
                      TestV := LMVertex;
                      lv := TestV.x - slight.x;
                      mv := TestV.y - slight.y;
                      nv := TestV.z - slight.z;
                      TFactor := (RayLength / RayDiv) / Safe_Sqrt(lv*lv+mv*mv+nv*nv);
                      NoCount := 0;
                      While NoCount <= RayDiv do
                        begin
                          TestVFov := FOVtan * TestV.z;
                          if (((TestV.x > -TestVFov) and (TestV.x < TestVFov)) and
                             ((TestV.y > -TestVFov) and (TestV.y < TestVFov)) and
                             ((TestV.z > NearZClip) and (TestV.z < FarZClip))) then
                            begin
                              ClipShadowCastingPolygon := false;
                              Exit;
                            end;
                          TestV.X +:= lv * TFactor;
                          TestV.Y +:= mv * TFactor;
                          TestV.Z +:= nv * TFactor;
                          Inc(NoCount);
                        end;
                    end;
               end;
          end
        else
          begin
            ClipShadowCastingPolygon := true;
            RayDiv := 50;
            RayLength := 5000;

            TestV := poly.t1;
            lv := TestV.x - slight.x;
            mv := TestV.y - slight.y;
            nv := TestV.z - slight.z;
            TFactor := (RayLength / RayDiv) / Safe_Sqrt(lv*lv+mv*mv+nv*nv);

            NoCount := 0;
            While NoCount <= RayDiv do
              begin
                TestVFov := FOVtan * TestV.z;
                if (((TestV.x > -TestVFov) and (TestV.x < TestVFov)) and
                   ((TestV.y > -TestVFov) and (TestV.y < TestVFov)) and
                   ((TestV.z > NearZClip) and (TestV.z < FarZClip))) then
                  begin
                    ClipShadowCastingPolygon := false;
                    Exit;
                  end;
                TestV.X +:= lv * TFactor;
                TestV.Y +:= mv * TFactor;
                TestV.Z +:= nv * TFactor;
                Inc(NoCount);
              end;

            TestV := poly.t2;
            lv := TestV.x - slight.x;
            mv := TestV.y - slight.y;
            nv := TestV.z - slight.z;
            TFactor := (RayLength / RayDiv) / Safe_Sqrt(lv*lv+mv*mv+nv*nv);

            NoCount := 0;
            While NoCount <= RayDiv do
              begin
                TestVFov := FOVtan * TestV.z;
                if (((TestV.x > -TestVFov) and (TestV.x < TestVFov)) and
                   ((TestV.y > -TestVFov) and (TestV.y < TestVFov)) and
                   ((TestV.z > NearZClip) and (TestV.z < FarZClip))) then
                  begin
                    ClipShadowCastingPolygon := false;
                    Exit;
                  end;
                TestV.X +:= lv * TFactor;
                TestV.Y +:= mv * TFactor;
                TestV.Z +:= nv * TFactor;
                Inc(NoCount);
              end;

            TestV := poly.t3;
            lv := TestV.x - slight.x;
            mv := TestV.y - slight.y;
            nv := TestV.z - slight.z;
            TFactor := (RayLength / RayDiv) / Safe_Sqrt(lv*lv+mv*mv+nv*nv);

            NoCount := 0;
            While NoCount <= RayDiv do
              begin
                TestVFov := FOVtan * TestV.z;
                if (((TestV.x > -TestVFov) and (TestV.x < TestVFov)) and
                   ((TestV.y > -TestVFov) and (TestV.y < TestVFov)) and
                   ((TestV.z > NearZClip) and (TestV.z < FarZClip))) then
                  begin
                    ClipShadowCastingPolygon := false;
                    Exit;
                  end;
                TestV.X +:= lv * TFactor;
                TestV.Y +:= mv * TFactor;
                TestV.Z +:= nv * TFactor;
                Inc(NoCount);
              end;

            TestV := poly.t4;
            lv := TestV.x - slight.x;
            mv := TestV.y - slight.y;
            nv := TestV.z - slight.z;
            TFactor := (RayLength / RayDiv) / Safe_Sqrt(lv*lv+mv*mv+nv*nv);

            NoCount := 0;
            While NoCount <= RayDiv do
              begin
                TestVFov := FOVtan * TestV.z;
                if (((TestV.x > -TestVFov) and (TestV.x < TestVFov)) and
                   ((TestV.y > -TestVFov) and (TestV.y < TestVFov)) and
                   ((TestV.z > NearZClip) and (TestV.z < FarZClip))) then
                  begin
                    ClipShadowCastingPolygon := false;
                    Exit;
                  end;
                TestV.X +:= lv * TFactor;
                TestV.Y +:= mv * TFactor;
                TestV.Z +:= nv * TFactor;
                Inc(NoCount);
              end;

            TestV := Sredna;
            lv := TestV.x - slight.x;
            mv := TestV.y - slight.y;
            nv := TestV.z - slight.z;
            TFactor := (RayLength / RayDiv) / Safe_Sqrt(lv*lv+mv*mv+nv*nv);

            NoCount := 0;
            While NoCount <= RayDiv do
              begin
                TestVFov := FOVtan * TestV.z;
                if (((TestV.x > -TestVFov) and (TestV.x < TestVFov)) and
                   ((TestV.y > -TestVFov) and (TestV.y < TestVFov)) and
                   ((TestV.z > NearZClip) and (TestV.z < FarZClip))) then
                  begin
                    ClipShadowCastingPolygon := false;
                    Exit;
                  end;
                TestV.X +:= lv * TFactor;
                TestV.Y +:= mv * TFactor;
                TestV.Z +:= nv * TFactor;
                Inc(NoCount);
              end;
          end;
      end;
end;

Procedure Optimize_Viewable_Polygons_Lighting(ii:Integer);
var Sredna:vertex;
    xu,yu,zu,xv,yv,zv,rac,Distance,
    LightvX,LightvY,LightvZ,NormalX,NormalY,NormalZ,Teta:single;
    nlights,SumR,SumG,SumB,ppR,ppG,ppB,surlit,FinR,FinG,FinB,
    ppR1,ppG1,ppB1,ppR2,ppG2,ppB2,ppR3,ppG3,ppB3,ppR4,ppG4,ppB4,
    TCol,TCol1,TCol2,TCol3,TCol4,TCol5,Ti,Tj,Td,rrt,ggt,bbt:Integer;
    rr1,rr2,rr3,rr4,rr5,gg1,gg2,gg3,gg4,gg5,bb1,bb2,bb3,bb4,bb5:Dword;
Begin
  Sredna.X:=(RotPoly[ii].T1.X + RotPoly[ii].T3.X) / 2;
  Sredna.Y:=(RotPoly[ii].T1.Y + RotPoly[ii].T3.Y) / 2;
  Sredna.Z:=(RotPoly[ii].T1.Z + RotPoly[ii].T3.Z) / 2;

  xu:=rotpoly[ii].t4.x - rotpoly[ii].t1.x;
  yu:=rotpoly[ii].t4.y - rotpoly[ii].t1.y;
  zu:=rotpoly[ii].t4.z - rotpoly[ii].t1.z;
  xv:=rotpoly[ii].t2.x - rotpoly[ii].t1.x;
  yv:=rotpoly[ii].t2.y - rotpoly[ii].t1.y;
  zv:=rotpoly[ii].t2.z - rotpoly[ii].t1.z;
  NormalX:=yu*zv-zu*yv;
  NormalY:=zu*xv-xu*zv;
  NormalZ:=xu*yv-yu*xv;
  rac:=Safe_Sqrt(NormalX*NormalX+NormalY*NormalY+NormalZ*NormalZ);
  NormalX:=NormalX/rac;
  NormalY:=NormalY/rac;
  NormalZ:=NormalZ/rac;

  Surface_Reflectivity_Coeficient_R := SrBoja_R[pol2tex[ii]] / 256;
  Surface_Reflectivity_Coeficient_G := SrBoja_G[pol2tex[ii]] / 256;
  Surface_Reflectivity_Coeficient_B := SrBoja_B[pol2tex[ii]] / 256;

  SumR:=0;
  SumG:=0;
  SumB:=0;
  ShadowMap_Lights := 0;
  RayTrace_Lights := 0;
  DisableLightSystem:=false;
  If NOT(LightsOFF) then
    for nlights:=1 to litmax do
     if NOT(Light_Disabled[nlights]) then
       begin
         LightvX:=Sredna.X-LightVertex[nlights].X;
         LightvY:=Sredna.Y-LightVertex[nlights].Y;
         LightvZ:=Sredna.Z-LightVertex[nlights].Z;

         Distance := LightvX * LightvX + LightvY * LightvY + LightvZ * LightvZ;
         Teta:=(LightvX*NormalX+LightvY*NormalY+LightvZ*NormalZ)/
               (Safe_Sqrt(Distance)*Distance);

         if doublesided[ii] then
           teta := abs(teta);

         IF Teta > 0 then
           begin
             ppR := round(LightIntensity[nlights] * LightR[nlights] * Teta);
             ppG := round(LightIntensity[nlights] * LightG[nlights] * Teta);
             ppB := round(LightIntensity[nlights] * LightB[nlights] * Teta);

             SurLit:=Round(Surface_Reflectivity_Coeficient_R * ppR +
                           Surface_Reflectivity_Coeficient_G * ppG +
                           Surface_Reflectivity_Coeficient_B * ppB);

             IF (SurLit >= LightTolerance) and (Is_Vertex_In_Player_Zone(LightVertex[nlights])) then
               begin
                 if (LightShadow[nlights] = 0) or (LightShadow[nlights] = 1) or NOT(PrPixel) then
                   begin
                     Inc(ShadowMap_Lights);
                     ShadowMap_LightCull[ShadowMap_Lights] := nlights;
                   end
                 else
                   begin
                     Inc(RayTrace_Lights);
                     RayTrace_LightCull[RayTrace_Lights] := nlights;
                   end;
                 SumR := SumR + ppR;
                 SumG := SumG + ppG;
                 SumB := SumB + ppB;
               end;
           end;
       end;
  If (SumR = 0) and (SumG = 0) and (SumB = 0) then
    begin
      DisableLightSystem := true;
      Exit;
    end;
  SurLit := Round(Surface_Reflectivity_Coeficient_R * SumR +
                  Surface_Reflectivity_Coeficient_G * SumG +
                  Surface_Reflectivity_Coeficient_B * SumB);
  IF SurLit < LightTolerance then
    DisableLightSystem := true;
End;

Function MinDistance(LightNo,PolyNo:Dword):Single;
var Dist1,Dist2,Dist3,Dist4,Dist5,MinD:single;
    Sredna:vertex;
begin
  Sredna.X:=(RotPoly[PolyNo].T1.X+RotPoly[PolyNo].T3.X)/2;
  Sredna.Y:=(RotPoly[PolyNo].T1.Y+RotPoly[PolyNo].T3.Y)/2;
  Sredna.Z:=(RotPoly[PolyNo].T1.Z+RotPoly[PolyNo].T3.Z)/2;
  Dist1:=sqr(RotPoly[PolyNo].T1.X-LightVertex[LightNo].X)+
         sqr(RotPoly[PolyNo].T1.Y-LightVertex[LightNo].Y)+
         sqr(RotPoly[PolyNo].T1.Z-LightVertex[LightNo].Z);
  Dist2:=sqr(RotPoly[PolyNo].T2.X-LightVertex[LightNo].X)+
         sqr(RotPoly[PolyNo].T2.Y-LightVertex[LightNo].Y)+
         sqr(RotPoly[PolyNo].T2.Z-LightVertex[LightNo].Z);
  Dist3:=sqr(RotPoly[PolyNo].T3.X-LightVertex[LightNo].X)+
         sqr(RotPoly[PolyNo].T3.Y-LightVertex[LightNo].Y)+
         sqr(RotPoly[PolyNo].T3.Z-LightVertex[LightNo].Z);
  Dist4:=sqr(RotPoly[PolyNo].T4.X-LightVertex[LightNo].X)+
         sqr(RotPoly[PolyNo].T4.Y-LightVertex[LightNo].Y)+
         sqr(RotPoly[PolyNo].T4.Z-LightVertex[LightNo].Z);
  Dist5:=sqr(Sredna.X-LightVertex[LightNo].X)+
         sqr(Sredna.Y-LightVertex[LightNo].Y)+
         sqr(Sredna.Z-LightVertex[LightNo].Z);
  MinD:=Dist1;
  if Dist2<MinD then
    MinD:=Dist2;
  if Dist3<MinD then
    MinD:=Dist3;
  if Dist4<MinD then
    MinD:=Dist4;
  if Dist5<MinD then
    MinD:=Dist5;
  MinDistance:=MinD;
end;

Procedure SortShadowCastingPolygons;
var LightNo,TmpPoly:Dword;
begin
For LightNo:=1 to litmax do
   For ii:=1 to RayPolys[LightNo]-1 do
      For jj:=ii+1 to RayPolys[LightNo] do
         If MinDistance(LightNo,PolyNo[LightNo,ii]) > MinDistance(LightNo,PolyNo[LightNo,jj]) then
           begin
             TmpPoly:=PolyNo[LightNo,ii];
             PolyNo[LightNo,ii]:=PolyNo[LightNo,jj];
             PolyNo[LightNo,jj]:=TmpPoly;
           end;
end;

Procedure ComputeNormals;
var xu,yu,zu,xv,yv,zv,rac:single;
    NormalTmp:vertex;
    CastNo:Dword;
begin
  For CastNo := ShadowStart to ShadowEnd do
     begin
       ii := PolyNoMain[CastNo];
       xu := rotpoly[ii].t4.x - rotpoly[ii].t1.x;
       yu := rotpoly[ii].t4.y - rotpoly[ii].t1.y;
       zu := rotpoly[ii].t4.z - rotpoly[ii].t1.z;
       xv := rotpoly[ii].t2.x - rotpoly[ii].t1.x;
       yv := rotpoly[ii].t2.y - rotpoly[ii].t1.y;
       zv := rotpoly[ii].t2.z - rotpoly[ii].t1.z;
       NormalTmp.X := yu * zv - zu * yv;
       NormalTmp.Y := zu * xv - xu * zv;
       NormalTmp.Z := xu * yv - yu * xv;
       rac := Safe_Sqrt(NormalTmp.X * NormalTmp.X + NormalTmp.Y * NormalTmp.Y + NormalTmp.Z * NormalTmp.Z);
       NormalTmp.X /:= rac;
       NormalTmp.Y /:= rac;
       NormalTmp.Z /:= rac;
       NormalLookup[ii] := NormalTmp;
     end;
end;

Procedure Optimize_Shadow_Casting_Objects;
var Sredna,RayVector,Normal:vertex;
    xu,yu,zu,xv,yv,zv,rac,RayAgol,Distance:single;
    CastNo,LightNo,FinR,FinG,FinB,ppR,ppG,ppB,surlit,
    ppR1,ppG1,ppB1,ppR2,ppG2,ppB2,ppR3,ppG3,ppB3,ppR4,ppG4,ppB4,ppR5,ppG5,ppB5,
    TCol,TCol1,TCol2,TCol3,TCol4,TCol5,Ti,Tj,Td,rrt,ggt,bbt,AmbLit:Integer;
    rr1,rr2,rr3,rr4,rr5,gg1,gg2,gg3,gg4,gg5,bb1,bb2,bb3,bb4,bb5:Dword;
Begin
  For LightCo:=1 to litmax do
     RayPolys[LightCo]:=0;
  For CastNo:=ShadowStart to ShadowEnd do
     begin
       ii:=PolyNoMain[CastNo];
       Sredna.X:=(RotPoly[ii].T1.X + RotPoly[ii].T3.X) / 2;
       Sredna.Y:=(RotPoly[ii].T1.Y + RotPoly[ii].T3.Y) / 2;
       Sredna.Z:=(RotPoly[ii].T1.Z + RotPoly[ii].T3.Z) / 2;

       xu:=RotPoly[ii].t4.x - RotPoly[ii].t1.x;
       yu:=RotPoly[ii].t4.y - RotPoly[ii].t1.y;
       zu:=RotPoly[ii].t4.z - RotPoly[ii].t1.z;
       xv:=RotPoly[ii].t2.x - RotPoly[ii].t1.x;
       yv:=RotPoly[ii].t2.y - RotPoly[ii].t1.y;
       zv:=RotPoly[ii].t2.z - RotPoly[ii].t1.z;
       Normal.X:=yu*zv-zu*yv;
       Normal.Y:=zu*xv-xu*zv;
       Normal.Z:=xu*yv-yu*xv;

       For LightCo:=1 to litmax do
          begin
            RayVector.X:=Sredna.X-LightVertex[LightCo].X;
            RayVector.Y:=Sredna.Y-LightVertex[LightCo].Y;
            RayVector.Z:=Sredna.Z-LightVertex[LightCo].Z;

            Distance:=sqr(RayVector.X) + sqr(RayVector.Y) + sqr(RayVector.Z);
            RayAgol:=(RayVector.X * Normal.X + RayVector.Y * Normal.Y + RayVector.Z * Normal.Z) /
                     (Safe_Sqrt(Distance * (sqr(Normal.X) + sqr(Normal.Y) + sqr(Normal.Z))));
            if doublesided[ii] then
              RayAgol := abs(RayAgol);

            IF RayAgol > 0 then
              begin
                ppR5:=Round(LightIntensity[LightCo]*LightR[LightCo]*RayAgol/Distance);
                ppG5:=Round(LightIntensity[LightCo]*LightG[LightCo]*RayAgol/Distance);
                ppB5:=Round(LightIntensity[LightCo]*LightB[LightCo]*RayAgol/Distance);

                RayVector.X:=RotPoly[ii].t1.x-LightVertex[LightCo].X;
                RayVector.Y:=RotPoly[ii].t1.y-LightVertex[LightCo].Y;
                RayVector.Z:=RotPoly[ii].t1.z-LightVertex[LightCo].Z;

                Distance:=sqr(RayVector.X) + sqr(RayVector.Y) + sqr(RayVector.Z);
                RayAgol:=(RayVector.X * Normal.X + RayVector.Y * Normal.Y + RayVector.Z * Normal.Z) /
                         (Safe_Sqrt(Distance * (sqr(Normal.X) + sqr(Normal.Y) + sqr(Normal.Z))));

                if doublesided[ii] then
                  RayAgol := abs(RayAgol);

                ppR1:=Round(LightIntensity[LightCo]*LightR[LightCo]*RayAgol/Distance);
                ppG1:=Round(LightIntensity[LightCo]*LightG[LightCo]*RayAgol/Distance);
                ppB1:=Round(LightIntensity[LightCo]*LightB[LightCo]*RayAgol/Distance);


                RayVector.X:=RotPoly[ii].t2.x-LightVertex[LightCo].X;
                RayVector.Y:=RotPoly[ii].t2.y-LightVertex[LightCo].Y;
                RayVector.Z:=RotPoly[ii].t2.z-LightVertex[LightCo].Z;

                Distance:=sqr(RayVector.X) + sqr(RayVector.Y) + sqr(RayVector.Z);
                RayAgol:=(RayVector.X * Normal.X + RayVector.Y * Normal.Y + RayVector.Z * Normal.Z) /
                         (Safe_Sqrt(Distance * (sqr(Normal.X) + sqr(Normal.Y) + sqr(Normal.Z))));

                if doublesided[ii] then
                  RayAgol := abs(RayAgol);

                ppR2:=Round(LightIntensity[LightCo]*LightR[LightCo]*RayAgol/Distance);
                ppG2:=Round(LightIntensity[LightCo]*LightG[LightCo]*RayAgol/Distance);
                ppB2:=Round(LightIntensity[LightCo]*LightB[LightCo]*RayAgol/Distance);


                RayVector.X:=RotPoly[ii].t3.x-LightVertex[LightCo].X;
                RayVector.Y:=RotPoly[ii].t3.y-LightVertex[LightCo].Y;
                RayVector.Z:=RotPoly[ii].t3.z-LightVertex[LightCo].Z;

                Distance:=sqr(RayVector.X) + sqr(RayVector.Y) + sqr(RayVector.Z);
                RayAgol:=(RayVector.X * Normal.X + RayVector.Y * Normal.Y + RayVector.Z * Normal.Z) /
                         (Safe_Sqrt(Distance * (sqr(Normal.X) + sqr(Normal.Y) + sqr(Normal.Z))));

                if doublesided[ii] then
                  RayAgol := abs(RayAgol);

                ppR3:=Round(LightIntensity[LightCo]*LightR[LightCo]*RayAgol/Distance);
                ppG3:=Round(LightIntensity[LightCo]*LightG[LightCo]*RayAgol/Distance);
                ppB3:=Round(LightIntensity[LightCo]*LightB[LightCo]*RayAgol/Distance);


                RayVector.X:=RotPoly[ii].t4.x-LightVertex[LightCo].X;
                RayVector.Y:=RotPoly[ii].t4.y-LightVertex[LightCo].Y;
                RayVector.Z:=RotPoly[ii].t4.z-LightVertex[LightCo].Z;

                Distance:=sqr(RayVector.X) + sqr(RayVector.Y) + sqr(RayVector.Z);
                RayAgol:=(RayVector.X * Normal.X + RayVector.Y * Normal.Y + RayVector.Z * Normal.Z) /
                         (Safe_Sqrt(Distance * (sqr(Normal.X) + sqr(Normal.Y) + sqr(Normal.Z))));

                if doublesided[ii] then
                  RayAgol := abs(RayAgol);

                ppR4:=Round(LightIntensity[LightCo]*LightR[LightCo]*RayAgol/Distance);
                ppG4:=Round(LightIntensity[LightCo]*LightG[LightCo]*RayAgol/Distance);
                ppB4:=Round(LightIntensity[LightCo]*LightB[LightCo]*RayAgol/Distance);


                ppR:=(ppR1+ppR2+ppR3+ppR4+ppR5) div 5;
                ppG:=(ppG1+ppG2+ppG3+ppG4+ppG5) div 5;
                ppB:=(ppB1+ppB2+ppB3+ppB4+ppB5) div 5;


                Surface_Reflectivity_Coeficient_R := SrBoja_R[pol2tex[ii]] / 256;
                Surface_Reflectivity_Coeficient_G := SrBoja_G[pol2tex[ii]] / 256;
                Surface_Reflectivity_Coeficient_B := SrBoja_B[pol2tex[ii]] / 256;

                SurLit := Round(Surface_Reflectivity_Coeficient_R * ppR +
                                Surface_Reflectivity_Coeficient_G * ppG +
                                Surface_Reflectivity_Coeficient_B * ppB);

                IF //(SurLit >= ShadowTolerance) and
                   (NOT(ClipShadowCastingPolygon(RotPoly[ii], LightVertex[LightCo]))) and
                   ((Is_Polygon_In_Player_Zone(RotPoly[ii])) and
                   (Is_Vertex_In_Player_Zone(LightVertex[LightCo]))) then
                  Begin
                    Inc(RayPolys[LightCo]);
                    PolyNo[LightCo, RayPolys[LightCo]] := ii;
                  End;
              end;
          end;
     end;
End;

Procedure Tweak_Shadow_Tolerance;
var
    LightCo,nlights:Integer;
begin
//  LowResShadowEnable := true;
  CurrentZoneLightCount := 0;

  for LightCo := 1 to litmax do
     if (Is_Vertex_In_Player_Zone(LightVertex[LightCo])) then
       Inc(CurrentZoneLightCount);

  if lightsoff then
      CurrentZoneLightCount := 0;

{  if (CurrentZoneLightCount <= 1) then
    ShadowTolerance := 50
  else
    ShadowTolerance := 3;}
end;

END.