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

{Unit for the Ray-Tracing System (Per-Pixel and Per-Vertex) -
                                                                      - Made by Dime Vangelovski
                                                                                     05.09.2005}
Unit RayTrace;

Interface

Uses
     GTypes3D;

Procedure Vertex_Ray_Tracing(poly: lice; ip, dbli: Integer);
Procedure Textured_polygon_per_pixel_Alpha(Poli: Lice; y1, y2, ip_main, dbli: Integer; transi: Byte; where: Pointer);
Procedure Textured_Polygon_per_pixel(Poli: Lice; y1, y2, ip_main, dbli: Integer; where: Pointer);

Implementation

Uses
     Globals,
     GMath3D,
     GRShadow;

{This is the per-vertex Ray-Tracing shadowing and lighting function.
 Poly is the current polygon, dbli is the current polygon index, ip is the current texture index}

Procedure Vertex_Ray_Tracing(poly: lice; ip, dbli: Integer);
var
    c1vR,c1vG,c1vB,c2vR,c2vG,c2vB,c3vR,c3vG,c3vB,c4vR,c4vG,c4vB:Dword;
    AlphaPolCount,AlCo,prozR,prozG,prozB,AIndex:Dword;
    magtmp,teta1,teta2,teta3,teta4:Single;
    AlphaPolygons:AlphaPols;
    nlights,LightCo:Dword;
    Normal,LRay:Vertex;
    VShadow:boolean;
begin
  CalcNormal(poly, Normal);

  c1R := 0;
  c1G := 0;
  c1B := 0;
  c2R := 0;
  c2G := 0;
  c2B := 0;
  c3R := 0;
  c3G := 0;
  c3B := 0;
  c4R := 0;
  c4G := 0;
  c4B := 0;

  If (NOT(LightsOFF)) and (NOT(DisableLightSystem)) then
    If ShadowMap_Lights > 0 then
      for LightCo := 1 to ShadowMap_Lights do
      begin
        nlights := ShadowMap_LightCull[LightCo];

        {This calculates the average angle on the first polygon vertex}
        LRay.X := poly.t1.x - LightVertex[nlights].x;
        LRay.Y := poly.t1.y - LightVertex[nlights].y;
        LRay.Z := poly.t1.z - LightVertex[nlights].z;
        magtmp := LRay.X * LRay.X + LRay.Y * LRay.Y + LRay.Z * LRay.Z;
        teta1 := (Normal.x * LRay.X + Normal.y * LRay.Y + Normal.z * LRay.Z) /
                 (Safe_Sqrt(magtmp) * magtmp);

        {This calculates the average angle on the second polygon vertex}
        LRay.X := poly.t2.x - LightVertex[nlights].x;
        LRay.Y := poly.t2.y - LightVertex[nlights].y;
        LRay.Z := poly.t2.z - LightVertex[nlights].z;
        magtmp := LRay.X * LRay.X + LRay.Y * LRay.Y + LRay.Z * LRay.Z;
        teta2 := (Normal.x * LRay.X + Normal.y * LRay.Y + Normal.z * LRay.Z) /
                 (Safe_Sqrt(magtmp) * magtmp);

        {This calculates the average angle on the third polygon vertex}
        LRay.X := poly.t3.x - LightVertex[nlights].x;
        LRay.Y := poly.t3.y - LightVertex[nlights].y;
        LRay.Z := poly.t3.z - LightVertex[nlights].z;
        magtmp := LRay.X * LRay.X + LRay.Y * LRay.Y + LRay.Z * LRay.Z;
        teta3 := (Normal.x * LRay.X + Normal.y * LRay.Y + Normal.z * LRay.Z) /
                 (Safe_Sqrt(magtmp) * magtmp);

        {This calculates the average angle on the fourth polygon vertex}
        LRay.X := poly.t4.x - LightVertex[nlights].x;
        LRay.Y := poly.t4.y - LightVertex[nlights].y;
        LRay.Z := poly.t4.z - LightVertex[nlights].z;
        magtmp := LRay.X * LRay.X + LRay.Y * LRay.Y + LRay.Z * LRay.Z;
        teta4 := (Normal.x * LRay.X + Normal.y * LRay.Y + Normal.z * LRay.Z) /
                 (Safe_Sqrt(magtmp) * magtmp);

        if (doublesided[dbli]) then
          begin
            teta1 := Abs(teta1);
            teta2 := Abs(teta2);
            teta3 := Abs(teta3);
            teta4 := Abs(teta4);
          end;

        {This calculates the lighting and shadowing for the first polygon vertex}
        if (teta1 > 0) then
          begin
            c1vR := Round(LightIntensity[nlights] * LightR[nlights] * teta1);
            c1vG := Round(LightIntensity[nlights] * LightG[nlights] * teta1);
            c1vB := Round(LightIntensity[nlights] * LightB[nlights] * teta1);

            VShadow:=false;
            AlphaPolCount := 0;
            If (CastShadows) and (LightShadow[nlights] <> 0) then
              begin
                VShadow:=RayShadowVertex(mainlightv[nlights],mainpoly[dbli].t1,Dbli,nlights,AlphaPolygons,AlphaPolCount);
                Inc(RaysCasted);
              end;

//            if (AlphaPolCount > 0) then
              for AlCo := 1 to AlphaPolCount do
                 begin
                   AIndex := AlphaPolygons[AlCo];
                   prozR := (SrBoja_R[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                   prozG := (SrBoja_G[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                   prozB := (SrBoja_B[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                   if (prozR > 255) then prozR := 255;
                   if (prozG > 255) then prozG := 255;
                   if (prozB > 255) then prozB := 255;
                   c1vR := c1vR * prozR shr 8;
                   c1vG := c1vG * prozG shr 8;
                   c1vB := c1vB * prozB shr 8;
                 end;

            If NOT(VShadow) then
              begin
                c1R +:= c1vR;
                c1G +:= c1vG;
                c1B +:= c1vB;
              end;
          end;

        {This calculates the lighting and shadowing for the second polygon vertex}
        If (teta2 > 0) then
          begin
            c2vR := Round(LightIntensity[nlights] * LightR[nlights] * teta2);
            c2vG := Round(LightIntensity[nlights] * LightG[nlights] * teta2);
            c2vB := Round(LightIntensity[nlights] * LightB[nlights] * teta2);

            VShadow:=false;
            AlphaPolCount := 0;
            If (CastShadows) and (LightShadow[nlights] <> 0) then
              begin
                VShadow:=RayShadowVertex(mainlightv[nlights],mainpoly[dbli].T2,Dbli,nlights,AlphaPolygons,AlphaPolCount);
                Inc(RaysCasted);
              end;

//            if AlphaPolCount > 0 then
              for AlCo := 1 to AlphaPolCount do
                 begin
                   AIndex := AlphaPolygons[AlCo];
                   prozR := (SrBoja_R[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                   prozG := (SrBoja_G[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                   prozB := (SrBoja_B[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                   if (prozR > 255) then prozR := 255;
                   if (prozG > 255) then prozG := 255;
                   if (prozB > 255) then prozB := 255;
                   c2vR := c2vR * prozR shr 8;
                   c2vG := c2vG * prozG shr 8;
                   c2vB := c2vB * prozB shr 8;
                 end;

            If NOT(VShadow) then
              begin
                c2R +:= c2vR;
                c2G +:= c2vG;
                c2B +:= c2vB;
              end;
          end;

        {This calculates the lighting and shadowing for the third polygon vertex}
        If (teta3 > 0) then
          begin
            c3vR := Round(LightIntensity[nlights] * LightR[nlights] * teta3);
            c3vG := Round(LightIntensity[nlights] * LightG[nlights] * teta3);
            c3vB := Round(LightIntensity[nlights] * LightB[nlights] * teta3);

            VShadow:=false;
            AlphaPolCount := 0;
            If (CastShadows) and (LightShadow[nlights] <> 0) then
              begin
                VShadow:=RayShadowVertex(mainlightv[nlights],mainpoly[dbli].T3,Dbli,nlights,AlphaPolygons,AlphaPolCount);
                Inc(RaysCasted);
              end;

//            if AlphaPolCount > 0 then
              for AlCo := 1 to AlphaPolCount do
                 begin
                   AIndex := AlphaPolygons[AlCo];
                   prozR := (SrBoja_R[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                   prozG := (SrBoja_G[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                   prozB := (SrBoja_B[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                   if (prozR > 255) then prozR := 255;
                   if (prozG > 255) then prozG := 255;
                   if (prozB > 255) then prozB := 255;
                   c3vR := c3vR * prozR shr 8;
                   c3vG := c3vG * prozG shr 8;
                   c3vB := c3vB * prozB shr 8;
                 end;

            If NOT(VShadow) then
              begin
                c3R +:= c3vR;
                c3G +:= c3vG;
                c3B +:= c3vB;
              end;
          end;

        {This calculates the lighting and shadowing for the fourth polygon vertex}
        If (teta4 > 0) then
          begin
            c4vR := Round(LightIntensity[nlights] * LightR[nlights] * teta4);
            c4vG := Round(LightIntensity[nlights] * LightG[nlights] * teta4);
            c4vB := Round(LightIntensity[nlights] * LightB[nlights] * teta4);

            VShadow:=false;
            AlphaPolCount := 0;
            If (CastShadows) and (LightShadow[nlights] <> 0) then
              begin
                VShadow:=RayShadowVertex(mainlightv[nlights],mainpoly[dbli].T4,Dbli,nlights,AlphaPolygons,AlphaPolCount);
                Inc(RaysCasted);
              end;

//            if AlphaPolCount > 0 then
              for AlCo := 1 to AlphaPolCount do
                 begin
                   AIndex := AlphaPolygons[AlCo];
                   prozR := (SrBoja_R[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                   prozG := (SrBoja_G[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                   prozB := (SrBoja_B[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                   if (prozR > 255) then prozR := 255;
                   if (prozG > 255) then prozG := 255;
                   if (prozB > 255) then prozB := 255;
                   c4vR := c4vR * prozR shr 8;
                   c4vG := c4vG * prozG shr 8;
                   c4vB := c4vB * prozB shr 8;
                 end;

            If NOT(VShadow) then
              begin
                c4R +:= c4vR;
                c4G +:= c4vG;
                c4B +:= c4vB;
              end;
          end;
    end;
end;


{This is the per-pixel Ray-Tracing shadowing and lighting function. It draws the whole polygon with transparency.
 Poly is the current polygon, dbli is the current polygon index, ip_main is the current texture index}

Procedure Textured_polygon_per_pixel_Alpha(Poli: Lice; y1, y2, ip_main, dbli: Integer;
                                           transi: Byte; where: Pointer);
Var
    x1,x2,c1R,c1G,c1B,c2R,c2G,c2B,texx1,texy1,texx2,texy2,ip,xxx,yyy,tmp,textx,texty,dxx,dyy,kor,
    cR,ccR,cG,ccG,cB,ccB,tmpR,tmpG,tmpB,ppColor,SkyColor,ppR,ppG,ppB,
    rrtt,ggtt,bbtt,SurUnLit,SurLit,SumR,SumG,SumB,FinR,FinG,FinB,LightCo,AIndex:Integer;
    yff,fst,xf,yf,ttt,aaa,fad,rrt,ggt,bbt,ShFactor,nlights:Dword;
    FokX,FokY,LightvX,LightvY,LightvZ,Teta,Distance,ShPoly,SqLite,NormLite,
    Zok,Broitel,Imdel1,Imdel2:single;
    AlphaPolCount,AlCo,prozR,prozG,prozB,transadj,
    r1,g1,b1,r2,g2,b2,rfin,gfin,bfin:Dword;
    ShadowTmp:array[1..maxlights] of boolean;
    AlphaPolCountTmp:array[1..maxlights] of Dword;
    AlphaPolygons:AlphaPols;
    Vert,Normal:Vertex;
    Shadow:boolean;
begin
  FokX := 1 / FocusX;
  FokY := 1 / FocusY;
  CalcNormal(Poli, Normal);
  Broitel := Normal.X * poli.t1.x + Normal.Y * poli.t1.y + Normal.Z * poli.t1.z;
  Imdel1 := Normal.X * FokX;
  transadj := 255 - transi;

  for yyy := y1 to y2 do
     begin
       x1 := poly[yyy, 1];
       x2 := poly[yyy, 2];
       c1R := colR[yyy, 1];
       c1G := colG[yyy, 1];
       c1B := colB[yyy, 1];
       c2R := colR[yyy, 2];
       c2G := colG[yyy, 2];
       c2B := colB[yyy, 2];
       texx1 := tex[yyy, 1];
       texy1 := tex[yyy, 2];
       texx2 := tex[yyy, 3];
       texy2 := tex[yyy, 4];
       ip := ip_main;

       {Check if the line is out of screen => Next line}
       if (yyy < 0) or (yyy > Res_Yend) then Continue;
       if (x1 > Res_Xend) and (x2 > Res_Xend) then Continue;
       if (x1 < 0) and (x2 < 0) then Continue;
       yff := yyy shl Res_Shl1 + (yyy and Res_Mask) shl Res_Shl2;

       if (x1 > x2) then
         begin
           tmp := x1; x1 := x2; x2 := tmp;
           tmp := c1R; c1R := c2R; c2R := tmp;
           tmp := c1G; c1G := c2G; c2G := tmp;
           tmp := c1B; c1B := c2B; c2B := tmp;
           tmp := texx1; texx1 := texx2; texx2 := tmp;
           tmp := texy1; texy1 := texy2; texy2 := tmp;
         end;

       {Calculate starting values in 16.16 fixed point for speed}
       ccR := (c2R - c1R) shl 16;
       ccG := (c2G - c1G) shl 16;
       ccB := (c2B - c1B) shl 16;
       dxx := (texx2 - texx1) shl 16;
       dyy := (texy2 - texy1) shl 16;
       kor := x2 - x1;
       if (kor <> 0) then
         begin
           ccR := ccR div kor;
           ccG := ccG div kor;
           ccB := ccB div kor;
           dxx := dxx div kor;
           dyy := dyy div kor;
         end;
       cR := c1R shl 16;
       cG := c1G shl 16;
       cB := c1B shl 16;
       textx := texx1 shl 16;
       texty := texy1 shl 16;

       Imdel2 := Normal.Y * (yyy - ycenter) * FokY + Normal.Z;

       for LightCo := 1 to RayTrace_Lights do
          begin
            nlights := RayTrace_LightCull[LightCo];
            AlphaPolCountTmp[nlights] := 0;
            if (CastShadows) then
              ShadowTmp[nlights] := TRUE
            else
              ShadowTmp[nlights] := FALSE;
          end;

       asm
          push ECX
          push ESI
          mov ESI, [ip]
          dec ESI
          mov Cl, [MipMapFactor]
          shl ESI, Cl
          mov [ip], ESI
          pop ESI
          pop ECX
       end;

       for xxx := x1 to x2 - 1 do
          begin
            if (xxx >= 0) and (xxx <= Res_Xend) then
              begin
                Zok := Broitel / ((xxx - xcenter) * Imdel1 + Imdel2); // The real Z coordinate of the current polygon pixel being rendered
                fst := xxx + yff;
                if (Zok < zbuf[fst]) then
                  begin
                    asm
                       push EAX
                       push ECX
                       push EDI
                       push ESI

                       {calculate current texel address in the texture}
                       mov ESI, [ip]
                       mov EDI, [textY]               -- U          - 1
                       shr EDI, 16                    -- U
                       mov EAX, [textX]               -- V          - 1
                       shr EAX, 16                    -- U          - 1
                       push ECX
                       mov Cl, [MipMapResF]
                       shl EAX, Cl                    -- U          - 1
                       pop ECX
                       add EAX, EDI                   -- U
                       shl EAX, 2
                       add EAX, ESI
                       mov EDI, [MipMapOffset]        {The MipMap Texture Offset}
                       add EDI, EAX
                       mov EAX, [EDI]                 -- U + AGI  { EAX has the 32bpp color }

                       {Convert from color to RGB value}
                       mov EDI, EAX
                       shr EAX, 16
                       mov [rrt], EAX
                       shl EAX, 16
                       sub EDI, EAX
                       mov ESI, EDI
                       shr EDI, 8
                       mov [ggt], EDI
                       shl EDI, 8
                       sub ESI, EDI
                       mov [bbt], ESI

                       pop ESI
                       pop EDI
                       pop ECX
                       pop EAX
                    end;

                    // The actual coordinates (World Space) of the pixel being rendered
                    Vert.X := Zok * (xxx - xcenter) * FokX;
                    Vert.Y := Zok * (yyy - ycenter) * FokY;
                    Vert.Z := Zok;

                    SumR := 0;
                    SumG := 0;
                    SumB := 0;
                    if (NOT(LightsOFF)) and (NOT(DisableLightSystem)) then
                      for LightCo := 1 to RayTrace_Lights do
                         begin
                           nlights := RayTrace_LightCull[LightCo];

                           LightvX := Vert.X - LightVertex[nlights].X;
                           LightvY := Vert.Y - LightVertex[nlights].Y;
                           LightvZ := Vert.Z - LightVertex[nlights].Z;

                           Distance := LightvX * LightvX + LightvY * LightvY + LightvZ * LightvZ;
                           Teta := (LightvX * Normal.X + LightvY * Normal.Y + LightvZ * Normal.Z) /
                                   (Safe_Sqrt(Distance) * Distance);

                           Teta := Abs(Teta);
//                           if (Teta > 0) then
//                             begin
                               ppR := round(LightIntensity[nlights] * LightR[nlights] * Teta);
                               ppG := round(LightIntensity[nlights] * LightG[nlights] * Teta);
                               ppB := round(LightIntensity[nlights] * LightB[nlights] * Teta);

                               SurLit := (rrt * ppR + ggt * ppG + bbt * ppB) shr 8 * transadj shr 8;

                               Shadow := ShadowTmp[nlights];
                               AlphaPolCount := AlphaPolCountTmp[nlights];
                               if (CastShadows) and (Surlit >= ShadowTolerance) then
                                 begin
                                   if (RayTraceSkipFactor > 1) then
                                     begin
                                       if ((xxx mod RayTraceSkipFactor = 0) or (xxx = x1) or (xxx = x2-1) or (xxx = 0) or (xxx = Res_Xend)) then
                                         begin
                                           Shadow := RayShadow(LightVertex[nlights], Vert, dbli, nlights, AlphaPolygons, AlphaPolCount);
                                           AlphaPolCountTmp[nlights] := AlphaPolCount;
                                           ShadowTmp[nlights] := Shadow;
                                           Inc(RaysCasted);
                                         end;
                                     end
                                   else
                                     begin
                                       Shadow := RayShadow(LightVertex[nlights], Vert, dbli, nlights, AlphaPolygons, AlphaPolCount);
                                       Inc(RaysCasted);
                                     end;
                                 end;

                               for AlCo := 1 to AlphaPolCount do
                                  begin
                                    AIndex := AlphaPolygons[AlCo];
                                    prozR := (SrBoja_R[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                                    prozG := (SrBoja_G[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                                    prozB := (SrBoja_B[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                                    if (prozR > 255) then prozR := 255;
                                    if (prozG > 255) then prozG := 255;
                                    if (prozB > 255) then prozB := 255;
                                    ppR := ppR * prozR shr 8;
                                    ppG := ppG * prozG shr 8;
                                    ppB := ppB * prozB shr 8;
                                  end;

                               if (NOT(Shadow)){ and (SurLit>ShadowTolerance)} then
                                 begin
                                   SumR +:= ppR;
                                   SumG +:= ppG;
                                   SumB +:= ppB;
                                 end;
//                             end;
                         end;

                    SumR +:= cR shr 16;
                    SumG +:= cG shr 16;
                    SumB +:= cB shr 16;

                    r1 := rrt * SumR shr 8 * transadj shr 8;
                    g1 := ggt * SumG shr 8 * transadj shr 8;
                    b1 := bbt * SumB shr 8 * transadj shr 8;

                    if (r1 > 255) then r1 := 255;
                    if (g1 > 255) then g1 := 255;
                    if (b1 > 255) then b1 := 255;

                    asm
                       push EAX
                       push EBX
                       push EDI

                       mov EDI, [where]
                       mov EBX, [fst]
                       mov EAX, [EDI+EBX*4]

                       mov EBX, EAX
                       shr EAX, 16
                       mov [r2], EAX
                       shl EAX, 16
                       sub EBX, EAX
                       mov EDI, EBX
                       shr EBX, 8
                       mov [g2], EBX
                       shl EBX, 8
                       sub EDI, EBX
                       mov [b2], EDI

                       mov Al, [transi]
                       mov Bl, 256
                       sub Bl, Al

                       {Calculate alpha values by:
                       color=F*Bl+S*transi (F is the color of the working pixel, S is the color
                       of the screen, Bl is the factor, transi is the translucency}
                       {for red color}
                       mov EAX, [r1]
                       mul Bl
                       shr EAX, 8
                       mov EDI, EAX
                       mov EAX, [r2]
                       mul [transi]
                       shr EAX, 8
                       add EAX, EDI
                       mov [rfin], EAX

                       {for green color}
                       mov EAX, [g1]
                       mul Bl
                       shr EAX, 8
                       mov EDI, EAX
                       mov EAX, [g2]
                       mul [transi]
                       shr EAX, 8
                       add EAX, EDI
                       mov [gfin], EAX

                       {for blue color}
                       mov EAX, [b1]
                       mul Bl
                       shr EAX, 8
                       mov EDI, EAX
                       mov EAX, [b2]
                       mul [transi]
                       shr EAX, 8
                       add EAX, EDI
                       mov [bfin], EAX

                       {convert from RGB back to color}
                       mov EAX, [rfin]
                       mov EDI, [gfin]
                       shl EAX, 16
                       shl EDI, 8
                       add EAX, EDI
                       add EAX, [bfin]

                       {draw the curent pixel}
                       mov EDI, [where]
                       mov EBX, [fst]
                       mov [EDI+EBX*4], EAX

                       pop EDI
                       pop EBX
                       pop EAX
                    end;
                  end;
              end;
            textX +:= dxx;
            textY +:= dyy;
            cR +:= ccR;
            cG +:= ccG;
            cB +:= ccB;
          end;
     end;
end;

{This is the per-pixel Ray-Tracing shadowing and lighting function. It draws the whole polygon.
 Poly is the current polygon, dbli is the current polygon index, ip_main is the current texture index}

Procedure Textured_Polygon_per_pixel(Poli: Lice; y1, y2, ip_main, dbli: Integer; where: Pointer);
Var
    x1,x2,c1R,c1G,c1B,c2R,c2G,c2B,texx1,texy1,texx2,texy2,ip,xxx,yyy,tmp,textx,texty,
    dxx,dyy,kor,cR,ccR,cG,ccG,cB,ccB,tmpR,tmpG,tmpB,ppColor,SkyColor,ppR,ppG,ppB,
    rrtt,ggtt,bbtt,SurUnLit,SurLit,SumR,SumG,SumB,FinR,FinG,FinB,LightCo,AIndex,
    rmin,gmin,bmin,TempInt:Integer;

    yff,fst,xf,yf,ttt,aaa,fad,rrt,ggt,bbt,ShFactor,nlights,AlphaPolCount,AlCo,
    prozR,prozG,prozB,r1,g1,b1,r2,g2,b2,rfin,gfin,bfin:Dword;

    FokX,FokY,LightvX,LightvY,LightvZ,Teta,Distance,ShPoly,SqLite,NormLite,
    Zok,Broitel,Imdel1,Imdel2,Temp3DNow:single;

    ShadowTmp:array[1..maxlights] of boolean;
    AlphaPolCountTmp:array[1..maxlights] of Dword;
    AlphaPolygons:AlphaPols;
    Vert,Normal:Vertex;
    Shadow:boolean;
begin
  FokX := 1 / FocusX;
  FokY := 1 / FocusY;
  CalcNormal(Poli, Normal);
  Broitel := Normal.X * poli.t1.x + Normal.Y * poli.t1.y + Normal.Z * poli.t1.z;
  Imdel1 := Normal.X * FokX;

  for yyy := y1 to y2 do
     begin
       x1 := poly[yyy, 1];
       x2 := poly[yyy, 2];
       c1R := colR[yyy, 1];
       c1G := colG[yyy, 1];
       c1B := colB[yyy, 1];
       c2R := colR[yyy, 2];
       c2G := colG[yyy, 2];
       c2B := colB[yyy, 2];
       texx1 := tex[yyy, 1];
       texy1 := tex[yyy, 2];
       texx2 := tex[yyy, 3];
       texy2 := tex[yyy, 4];
       ip := ip_main;

       {Check if the line is out of screen => Next line}
       if (yyy < 0) or (yyy > Res_Yend) then Continue;
       if (x1 > Res_Xend) and (x2 > Res_Xend) then Continue;
       if (x1 < 0) and (x2 < 0) then Continue;
       yff := yyy shl Res_Shl1 + (yyy and Res_Mask) shl Res_Shl2;

       if (x1 > x2) then
         begin
           tmp := x1; x1 := x2; x2 := tmp;
           tmp := c1R; c1R := c2R; c2R := tmp;
           tmp := c1G; c1G := c2G; c2G := tmp;
           tmp := c1B; c1B := c2B; c2B := tmp;
           tmp := texx1; texx1 := texx2; texx2 := tmp;
           tmp := texy1; texy1 := texy2; texy2 := tmp;
         end;

       {Calculate starting values in 16.16 fixed point for speed}
       ccR := (c2R - c1R) shl 16;
       ccG := (c2G - c1G) shl 16;
       ccB := (c2B - c1B) shl 16;
       dxx := (texx2 - texx1) shl 16;
       dyy := (texy2 - texy1) shl 16;
       kor := x2 - x1;
       if (kor <> 0) then
         begin
           ccR := ccR div kor;
           ccG := ccG div kor;
           ccB := ccB div kor;
           dxx := dxx div kor;
           dyy := dyy div kor;
         end;
       cR := c1R shl 16;
       cG := c1G shl 16;
       cB := c1B shl 16;
       textx := texx1 shl 16;
       texty := texy1 shl 16;

       Imdel2 := Normal.Y * (yyy - ycenter) * FokY + Normal.Z;

       for LightCo := 1 to RayTrace_Lights do
          begin
            nlights := RayTrace_LightCull[LightCo];
            AlphaPolCountTmp[nlights] := 0;
            if (CastShadows) then
              ShadowTmp[nlights] := TRUE
            else
              ShadowTmp[nlights] := FALSE;
          end;

       asm
          push ECX
          push ESI
          mov ESI, [ip]
          dec ESI
          mov Cl, [MipMapFactor]
          shl ESI, Cl
          mov [ip], ESI
          pop ESI
          pop ECX
       end;

       for xxx := x1 to x2 - 1 do
          begin
            if (xxx >= 0) and (xxx <= Res_Xend) then
              begin
                Zok := Broitel / ((xxx - xcenter) * Imdel1 + Imdel2); // The real Z coordinate of the current polygon pixel being rendered
                kor := Round(Zok);
                fst := xxx + yff;
                if (kor < zbuf[fst]) then
                  begin
                    asm
                       push EAX
                       push ECX
                       push EDI
                       push ESI

                       {calculate current texel address in the texture}
                       mov ESI, [ip]
                       mov EDI, [textY]               -- U          - 1
                       shr EDI, 16                    -- U
                       mov EAX, [textX]               -- V          - 1
                       shr EAX, 16                    -- U          - 1
                       push ECX
                       mov Cl, [MipMapResF]
                       shl EAX, Cl                    -- U          - 1
                       pop ECX
                       add EAX, EDI                   -- U
                       shl EAX, 2
                       add EAX, ESI
                       mov EDI, [MipMapOffset]        {The MipMap Texture Offset}
                       add EDI, EAX
                       mov EAX, [EDI]                 -- U + AGI  { EAX has the 32bpp color }

                       {Convert from color to RGB value}
                       mov EDI, EAX
                       shr EAX, 16
                       mov [rrt], EAX
                       shl EAX, 16
                       sub EDI, EAX
                       mov ESI, EDI
                       shr EDI, 8
                       mov [ggt], EDI
                       shl EDI, 8
                       sub ESI, EDI
                       mov [bbt], ESI

                       mov ESI, [fst]
                       mov EAX, [kor]
                       lea EDI, zbuf[ESI * 2]
                       mov [EDI], AX

                       pop ESI
                       pop EDI
                       pop ECX
                       pop EAX
                    end;

                    //zbuf[fst] := kor;

/*
asm
//   push    EAX
//   mov     EAX, [xxx]
//   sub     EAX, [xcenter]
//   mov     [TempInt], EAX
//   pop     EAX

   femms

   movd    mm2, [xxx]
   movd    mm4, [xcenter]
   pfsub   mm2, mm4

   movd    mm0, [FokX]
   movd    mm3, [Zok]
   pfmul   mm0, mm3

   pfmul   mm0, mm2

   movd    [Vert.X], mm0

   femms
end;
*/

                    // The actual coordinates (World Space) of the pixel being rendered
                    Vert.X := Zok * (xxx - xcenter) * FokX;
                    Vert.Y := Zok * (yyy - ycenter) * FokY;
                    Vert.Z := Zok;

                    SumR := 0;
                    SumG := 0;
                    SumB := 0;
                    if (NOT(LightsOFF)) and (NOT(DisableLightSystem)) then
                      for LightCo := 1 to RayTrace_Lights do
                         begin
                           nlights := RayTrace_LightCull[LightCo];

                           LightvX := Vert.X - LightVertex[nlights].X;
                           LightvY := Vert.Y - LightVertex[nlights].Y;
                           LightvZ := Vert.Z - LightVertex[nlights].Z;

                           Distance := LightvX * LightvX + LightvY * LightvY + LightvZ * LightvZ;
                           Teta := (LightvX * Normal.X + LightvY * Normal.Y + LightvZ * Normal.Z) /
                                   (Safe_Sqrt(Distance) * Distance);

                           if (Teta > 0) then
                             begin
                               ppR := round(LightIntensity[nlights] * LightR[nlights] * Teta);
                               ppG := round(LightIntensity[nlights] * LightG[nlights] * Teta);
                               ppB := round(LightIntensity[nlights] * LightB[nlights] * Teta);

                               SurLit := (rrt * ppR + ggt * ppG + bbt * ppB) shr 8;

                               Shadow := ShadowTmp[nlights];
                               AlphaPolCount := AlphaPolCountTmp[nlights];
                               if (CastShadows) and (Surlit >= ShadowTolerance) then
                                 begin
                                   if (RayTraceSkipFactor > 1) then
                                     begin
                                       if ((xxx mod RayTraceSkipFactor = 0) or (xxx = x1) or (xxx = x2-1) or (xxx = 0) or (xxx = Res_Xend)) then
                                         begin
                                           Shadow := RayShadow(LightVertex[nlights], Vert, dbli, nlights, AlphaPolygons, AlphaPolCount);
                                           AlphaPolCountTmp[nlights] := AlphaPolCount;
                                           ShadowTmp[nlights] := Shadow;
                                           Inc(RaysCasted);
                                         end;
                                     end
                                   else
                                     begin
                                       Shadow := RayShadow(LightVertex[nlights], Vert, dbli, nlights, AlphaPolygons, AlphaPolCount);
                                       Inc(RaysCasted);
                                     end;
                                 end;

                               for AlCo := 1 to AlphaPolCount do
                                  begin
                                    AIndex := AlphaPolygons[AlCo];
                                    prozR := (SrBoja_R[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                                    prozG := (SrBoja_G[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                                    prozB := (SrBoja_B[pol2tex[AIndex]] + alphap[AIndex]) * alphap[AIndex] shr 8;
                                    if (prozR > 255) then prozR := 255;
                                    if (prozG > 255) then prozG := 255;
                                    if (prozB > 255) then prozB := 255;
                                    ppR := ppR * prozR shr 8;
                                    ppG := ppG * prozG shr 8;
                                    ppB := ppB * prozB shr 8;
                                  end;

                               if (NOT(Shadow)) {and (SurLit>ShadowTolerance)} then
                                 begin
//                                   rmin := ppR - ShadowTolerance;
//                                   gmin := ppG - ShadowTolerance;
//                                   bmin := ppB - ShadowTolerance;
//                                   if (rmin < 0) then rmin := 0;
//                                   if (gmin < 0) then gmin := 0;
//                                   if (bmin < 0) then bmin := 0;
//                                   SumR +:= rmin;
//                                   SumG +:= gmin;
//                                   SumB +:= bmin;
                                   SumR +:= ppR;
                                   SumG +:= ppG;
                                   SumB +:= ppB;
                                 end;
                             end;
                         end;

                    SumR +:= cR shr 16;
                    SumG +:= cG shr 16;
                    SumB +:= cB shr 16;

                    FinR := rrt * SumR shr 8;
                    FinG := ggt * SumG shr 8;
                    FinB := bbt * SumB shr 8;

                    if (FinR > 255) then FinR := 255;
                    if (FinG > 255) then FinG := 255;
                    if (FinB > 255) then FinB := 255;

                    ppColor := FinR shl 16 + FinG shl 8 + FinB;

                    asm
                       mov EDI, [where]
                       mov EBX, [fst]
                       mov EAX, [ppColor]
                       mov [EDI+EBX*4], EAX
                    end;
                  end;
              end;
            textX +:= dxx;
            textY +:= dyy;
            cR +:= ccR;
            cG +:= ccG;
            cB +:= ccB;
          end;
     end;
end;

END.