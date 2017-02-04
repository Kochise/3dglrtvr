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

{Unit for the Texture Mapping Functions (Per-Vertex Lighting) in pure PASCAL code -

                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit Draw_pas;

Interface

Uses
     GTypes3D;

Procedure Scan_Edge(x1, y1, x2, y2, cc1R, cc1G, cc1B, cc2R, cc2G, cc2B, zz1, zz2, xtx1, ytx1, xtx2, ytx2: Integer);
Procedure Scan_Edge_Precise(poli:lice; x1, y1, x2, y2, cc1R, cc1G, cc1B, cc2R, cc2G, cc2B, xtx1, ytx1, xtx2, ytx2: Integer);
Procedure Textured_Polygon_Alpha(y1, y2, ip: Integer; transi: Byte; where: Pointer);
Procedure Textured_Polygon(y1, y2, ip: Integer; where: Pointer);
Procedure Textured_Polygon_Old_Renderer(y1, y2, ip: Integer; where: Pointer);

Implementation

Uses
     Globals,
     GDrwPrim,
     GMath3D;

{This is the SCANEDGE PROCEDURE to scan the edges of a polygon to get
 the values for the horizontal line rendering.
 x1,y1,x2,y2 are the current polygon edge coordinates,
 cc1R,... cc2R,... are the start and end color RGB values on the EDGE,
 zz1,zz2 are Z coordinates on start and end,
 xtx1,ytx1,xtx2,ytx2, are the start and end texture coordinates (u,v)}

Procedure Scan_Edge(x1, y1, x2, y2, cc1R, cc1G, cc1B, cc2R, cc2G, cc2B,
                    zz1, zz2, xtx1, ytx1, xtx2, ytx2: Integer);
var
    x,xc,zz,zzz,ccR,cccR,ccG,cccG,ccB,cccB,xx,yy,xclip,yclip,y,tmp,xkor,zkor,ygr: Integer;
begin
  if (y1 > y2) then
    begin
      tmp := y1; y1 := y2; y2 := tmp;
      tmp := x1; x1 := x2; x2 := tmp;
      tmp := zz1; zz1 := zz2; zz2 := tmp;
      tmp := cc2R; cc2R := cc1R; cc1R := tmp;
      tmp := cc2G; cc2G := cc1G; cc1G := tmp;
      tmp := cc2B; cc2B := cc1B; cc1B := tmp;
      tmp := xtx1; xtx1 := xtx2; xtx2 := tmp;
      tmp := ytx1; ytx1 := ytx2; ytx2 := tmp;
    end;

  {Calculate the starting values in 16.16 fixed point for speed}
  xc := (x2 - x1) shl 16;
  zzz := (zz2 - zz1) shl 16;
  cccR := (cc2R - cc1R) shl 16;
  cccG := (cc2G - cc1G) shl 16;
  cccB := (cc2B - cc1B) shl 16;
  xclip := (xtx2 - xtx1) shl 16;
  yclip := (ytx2 - ytx1) shl 16;
  xkor := y2 - y1;
  if (xkor <> 0) then
    begin
      xc := xc div xkor;
      cccR := cccR div xkor;
      cccG := cccG div xkor;
      cccB := cccB div xkor;
      zzz := zzz div xkor;
      xclip := xclip div xkor;
      yclip := yclip div xkor;
    end;
  x := x1 shl 16;
  ccR := cc1R shl 16;
  ccG := cc1G shl 16;
  ccB := cc1B shl 16;
  zz := zz1 shl 16;
  xx := xtx1 shl 16;
  yy := ytx1 shl 16;

  {Repeat for the whole vertical length of the polygon}
  for y := y1 to y2 do
     begin
       {Check if y is out of screen => skip computation}
       if (y >= 0) and (y < Res_Yend_p1) then
         begin
           xkor := x shr 16;
           zkor := zz shr 16;

           {Check if this is the left side of the polygon}
           if (xkor < poly[y,1]) then
             begin
               {Calculate the appropriate values for gouraud color,
                texture mapping, and the Z-Buffer for the left sides}

               poly[y,1] := xkor;
               colR[y,1] := ccR shr 16;
               colG[y,1] := ccG shr 16;
               colB[y,1] := ccB shr 16;

               tex[y,1] := xx shr 16;
               tex[y,2] := yy shr 16;

               ztmp[y,1] := zkor;
             end;

           {Check if this is the right side of the polygon}
           if (xkor > poly[y,2]) then
             begin
               {Calculate the appropriate values for gouraud color,
                texture mapping, and the Z-Buffer for the right sides}

               poly[y,2] := xkor;
               colR[y,2] := ccR shr 16;
               colG[y,2] := ccG shr 16;
               colB[y,2] := ccB shr 16;

               tex[y,3] := xx shr 16;
               tex[y,4] := yy shr 16;

               ztmp[y,2] := zkor;
             end;
         end;

       {increase the appropriate values for the next pixel in the polygon edge}
       x +:= xc;
       zz +:= zzz;
       ccR +:= cccR;
       ccG +:= cccG;
       ccB +:= cccB;
       xx +:= xclip;
       yy +:= yclip;
     end;
end;

{This is the precise SCANEDGE PROCEDURE to scan the edges of a polygon to get
 the values for the horizontal line rendering. It calculates the real Z-coord 
 instead of interpolating it, to avoid the affine distortion of the edges of 
 the polygon and to get more precise Z-buffering.
 x1,y1,x2,y2 are the current polygon edge coordinates,
 cc1R,... cc2R,... are the start and end color RGB values on the EDGE,
 zz1,zz2 are Z coordinates on start and end,
 xtx1,ytx1,xtx2,ytx2, are the start and end texture coordinates (u,v)}

Procedure Scan_Edge_Precise(poli:lice; x1, y1, x2, y2, cc1R, cc1G, cc1B, cc2R, cc2G, cc2B,
                            xtx1, ytx1, xtx2, ytx2: Integer);
var
    x,xc,ccR,cccR,ccG,cccG,ccB,cccB,xx,yy,xclip,yclip,y,tmp,xkor,zkor,ygr: Integer;
    xu,yu,zu,xv,yv,zv,FokX,FokY,rac,NormalX,NormalY,NormalZ,Broitel: Single;

begin
  if (y1 > y2) then
    begin
      tmp := y1; y1 := y2; y2 := tmp;
      tmp := x1; x1 := x2; x2 := tmp;
      tmp := cc2R; cc2R := cc1R; cc1R := tmp;
      tmp := cc2G; cc2G := cc1G; cc1G := tmp;
      tmp := cc2B; cc2B := cc1B; cc1B := tmp;
      tmp := xtx1; xtx1 := xtx2; xtx2 := tmp;
      tmp := ytx1; ytx1 := ytx2; ytx2 := tmp;
    end;

  {Calculate the starting values in 16.16 fixed point for speed}
  xc := (x2 - x1) shl 16;
  cccR := (cc2R - cc1R) shl 16;
  cccG := (cc2G - cc1G) shl 16;
  cccB := (cc2B - cc1B) shl 16;
  xclip := (xtx2 - xtx1) shl 16;
  yclip := (ytx2 - ytx1) shl 16;
  xkor := y2 - y1;
  if (xkor <> 0) then
    begin
      xc := xc div xkor;
      cccR := cccR div xkor;
      cccG := cccG div xkor;
      cccB := cccB div xkor;
      xclip := xclip div xkor;
      yclip := yclip div xkor;
    end;
  x := x1 shl 16;
  ccR := cc1R shl 16;
  ccG := cc1G shl 16;
  ccB := cc1B shl 16;
  xx := xtx1 shl 16;
  yy := ytx1 shl 16;

  //////////////////////////////////////////////////////////////////////////////
  // Precise Z-Coordinate calculation
  FokX := 1 / FocusX;
  FokY := 1 / FocusY;
  xu := poli.t4.x - poli.t1.x;
  yu := poli.t4.y - poli.t1.y;
  zu := poli.t4.z - poli.t1.z;
  xv := poli.t2.x - poli.t1.x;
  yv := poli.t2.y - poli.t1.y;
  zv := poli.t2.z - poli.t1.z;
  NormalX := yu * zv - zu * yv;
  NormalY := zu * xv - xu * zv;
  NormalZ := xu * yv - yu * xv;
  rac := Safe_Sqrt(NormalX * NormalX + NormalY * NormalY + NormalZ * NormalZ);
  NormalX := NormalX / rac;
  NormalY := NormalY / rac;
  NormalZ := NormalZ / rac;
  Broitel := NormalX * poli.t1.x + NormalY * poli.t1.y + NormalZ * poli.t1.z;
  //////////////////////////////////////////////////////////////////////////////

  {Repeat for the whole vertical length of the polygon}
  for y := y1 to y2 do
     begin
       {Check if y is out of screen => skip computation}
       if (y >= 0) and (y < Res_Yend_p1) then
         begin
           xkor := x shr 16;

           // Precise Z-Coordinate
           zkor := Round(Broitel / (NormalX * (xkor - xcenter) * FokX + NormalY * (y - ycenter) * FokY + NormalZ));

           {Check if this is the left side of the polygon}
           if (xkor < poly[y,1]) then
             begin
               {Calculate the appropriate values for gouraud color,
                texture mapping, and the Z-Buffer for the left sides}

               poly[y,1] := xkor;
               colR[y,1] := ccR shr 16;
               colG[y,1] := ccG shr 16;
               colB[y,1] := ccB shr 16;

               tex[y,1] := xx shr 16;
               tex[y,2] := yy shr 16;

               ztmp[y,1] := zkor;
             end;

           {Check if this is the right side of the polygon}
           if (xkor > poly[y,2]) then
             begin
               {Calculate the appropriate values for gouraud color,
                texture mapping, and the Z-Buffer for the right sides}

               poly[y,2] := xkor;
               colR[y,2] := ccR shr 16;
               colG[y,2] := ccG shr 16;
               colB[y,2] := ccB shr 16;

               tex[y,3] := xx shr 16;
               tex[y,4] := yy shr 16;

               ztmp[y,2] := zkor;
             end;
         end;

       {increase the appropriate values for the next pixel in the polygon edge}
       x +:= xc;
       ccR +:= cccR;
       ccG +:= cccG;
       ccB +:= cccB;
       xx +:= xclip;
       yy +:= yclip;
     end;
end;

{This procedure renders a transparent polygon from y1 to y2 using horizontal scanlines
 ip is the current texture index applied to the polygon. transi is the alpha value
 0 - non transparent, 255 - completely transparent. where is the address in mem}

Procedure Textured_Polygon_Alpha(y1, y2, ip: Integer; transi: Byte; where: Pointer);
var
    x1,x2,c1R,c1G,c1B,c2R,c2G,c2B,texx1,texy1,texx2,texy2,z1,z2,
    xxx,yyy,tmp,textx,texty,dxx,dyy,kor,z,zz,cR,ccR,cG,ccG,cB,ccB: Integer;
    yff,fst,TexColor,rrt,ggt,bbt,r1,g1,b1,r2,g2,b2,rfin,gfin,bfin: Dword;
begin
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
       z1 := ztmp[yyy, 1];
       z2 := ztmp[yyy, 2];

       {Check if the line is out of screen => exit}
       if (x1 < 0) and (x2 < 0) then Continue;
       if (yyy < 0) or (yyy > Res_Yend) then Continue;
       if (x1 > Res_Xend) and (x2 > Res_Xend) then Continue;
       yff := yyy shl Res_Shl1 + (yyy and Res_Mask) shl Res_Shl2;

       if (x1 > x2) then
         begin
           tmp := x1; x1 := x2; x2 := tmp;
           tmp := z1; z1 := z2; z2 := tmp;
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
       zz := (z2 - z1) shl 16;
       kor := x2 - x1;
       if (kor <> 0) then
         begin
           ccR := ccR div kor;
           ccG := ccG div kor;
           ccB := ccB div kor;
           dxx := dxx div kor;
           dyy := dyy div kor;
           zz := zz div kor;
         end;
       cR := c1R shl 16;
       cG := c1G shl 16;
       cB := c1B shl 16;
       textx := texx1 shl 16;
       texty := texy1 shl 16;
       z := z1 shl 16;

       for xxx := x1 to x2-1 do
          begin
            if (xxx >= 0) and (xxx <= Res_Xend) then
              begin
                kor := z shr 16;
                fst := xxx + yff;
                if kor < zbuf[fst] then
                  begin
//                    zbuf[fst] := kor;

                    case MipMapResF of
                      3: TexColor := miptex5[ip, textX shr 16, textY shr 16];
                      4: TexColor := miptex4[ip, textX shr 16, textY shr 16];
                      5: TexColor := miptex3[ip, textX shr 16, textY shr 16];
                      6: TexColor := miptex2[ip, textX shr 16, textY shr 16];
                      7: TexColor := miptex1[ip, textX shr 16, textY shr 16];
                      8: TexColor := texbuf[ip, textX shr 16, textY shr 16];
                    end;

                    Color2RGB(TexColor, rrt, ggt, bbt);

                    r1 := (((cR shr 16 * rrt) shr 8) * (255 - transi)) shr 8;
                    g1 := (((cG shr 16 * ggt) shr 8) * (255 - transi)) shr 8;
                    b1 := (((cB shr 16 * bbt) shr 8) * (255 - transi)) shr 8;

                    if (r1 > 255) then r1 := 255;
                    if (g1 > 255) then g1 := 255;
                    if (b1 > 255) then b1 := 255;

                    Color2RGB(memD[DWORD(where) + fst * 4], r2, g2, b2);

                    rfin := ((r1 * (256 - transi)) shr 8) + ((r2 * transi) shr 8);
                    gfin := ((g1 * (256 - transi)) shr 8) + ((g2 * transi) shr 8);
                    bfin := ((b1 * (256 - transi)) shr 8) + ((b2 * transi) shr 8);

                    memD[DWORD(where) + fst * 4] := RGB2Color(rfin, gfin, bfin);
                  end;
              end;
            z +:= zz;
            cR +:= ccR;
            cG +:= ccG;
            cB +:= ccB;
            textX +:= dxx;
            textY +:= dyy;
          end;
     end;
end;


{This procedure renders a polygon from y1 to y2 using horizontal scanlines.
 ip is the current texture index applied to the polygon. where is the address in mem}

Procedure Textured_Polygon(y1, y2, ip: Integer; where: Pointer);
var
    x1,x2,c1R,c1G,c1B,c2R,c2G,c2B,texx1,texy1,texx2,texy2,z1,z2,
    xxx,yyy,tmp,textx,texty,dxx,dyy,kor,z,zz,cR,ccR,cG,ccG,cB,ccB: Integer;
    yff,fst,TexColor,rrt,ggt,bbt: Dword;
begin
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
       z1 := ztmp[yyy, 1];
       z2 := ztmp[yyy, 2];

       {Check if the line is out of screen => exit}
       if (x1 < 0) and (x2 < 0) then Continue;
       if (yyy < 0) or (yyy > Res_Yend) then Continue;
       if (x1 > Res_Xend) and (x2 > Res_Xend) then Continue;
       yff := yyy shl Res_Shl1 + (yyy and Res_Mask) shl Res_Shl2;

       if (x1 > x2) then
         begin
           tmp := x1; x1 := x2; x2 := tmp;
           tmp := z1; z1 := z2; z2 := tmp;
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
       zz := (z2 - z1) shl 16;
       kor := x2 - x1;
       if (kor <> 0) then
         begin
           ccR := ccR div kor;
           ccG := ccG div kor;
           ccB := ccB div kor;
           dxx := dxx div kor;
           dyy := dyy div kor;
           zz := zz div kor;
         end;
       cR := c1R shl 16;
       cG := c1G shl 16;
       cB := c1B shl 16;
       textx := texx1 shl 16;
       texty := texy1 shl 16;
       z := z1 shl 16;

       for xxx := x1 to x2-1 do
          begin
            if (xxx >= 0) and (xxx <= Res_Xend) then
              begin
                kor := z shr 16;
                fst := xxx + yff;
                if kor < zbuf[fst] then
                  begin
                    zbuf[fst] := kor;

                    case MipMapResF of
                      3: TexColor := miptex5[ip, textX shr 16, textY shr 16];
                      4: TexColor := miptex4[ip, textX shr 16, textY shr 16];
                      5: TexColor := miptex3[ip, textX shr 16, textY shr 16];
                      6: TexColor := miptex2[ip, textX shr 16, textY shr 16];
                      7: TexColor := miptex1[ip, textX shr 16, textY shr 16];
                      8: TexColor := texbuf[ip, textX shr 16, textY shr 16];
                    end;

                    Color2RGB(TexColor, rrt, ggt, bbt);

                    rrt := (cR shr 16 * rrt) shr 8;
                    ggt := (cG shr 16 * ggt) shr 8;
                    bbt := (cB shr 16 * bbt) shr 8;

                    if (rrt > 255) then rrt := 255;
                    if (ggt > 255) then ggt := 255;
                    if (bbt > 255) then bbt := 255;

                    memD[DWORD(where) + fst * 4] := RGB2Color(rrt, ggt, bbt);
                  end;
              end;
            z +:= zz;
            cR +:= ccR;
            cG +:= ccG;
            cB +:= ccB;
            textX +:= dxx;
            textY +:= dyy;
          end;
     end;
end;


{This procedure renders a polygon from y1 to y2 using horizontal scanlines
 ip is the current texture index applied to the polygon. where is the address in mem
 This procedure uses the old (Gens1) lighting engine (simple light addition)}

Procedure Textured_Polygon_Old_Renderer(y1, y2, ip: Integer; where: Pointer);
var
    x1,x2,c1R,c1G,c1B,c2R,c2G,c2B,texx1,texy1,texx2,texy2,z1,z2,
    xxx,yyy,tmp,textx,texty,dxx,dyy,kor,z,zz,cR,ccR,cG,ccG,cB,ccB: Integer;
    yff,fst,TexColor,rrt,ggt,bbt: Dword;
begin
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
       z1 := ztmp[yyy, 1];
       z2 := ztmp[yyy, 2];

       {Check if the line is out of screen => exit}
       if (x1 < 0) and (x2 < 0) then Continue;
       if (yyy < 0) or (yyy > Res_Yend) then Continue;
       if (x1 > Res_Xend) and (x2 > Res_Xend) then Continue;
       yff := yyy shl Res_Shl1 + (yyy and Res_Mask) shl Res_Shl2;

       if (x1 > x2) then
         begin
           tmp := x1; x1 := x2; x2 := tmp;
           tmp := z1; z1 := z2; z2 := tmp;
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
       zz := (z2 - z1) shl 16;
       kor := x2 - x1;
       if (kor <> 0) then
         begin
           ccR := ccR div kor;
           ccG := ccG div kor;
           ccB := ccB div kor;
           dxx := dxx div kor;
           dyy := dyy div kor;
           zz := zz div kor;
         end;
       cR := c1R shl 16;
       cG := c1G shl 16;
       cB := c1B shl 16;
       textx := texx1 shl 16;
       texty := texy1 shl 16;
       z := z1 shl 16;

       for xxx := x1 to x2-1 do
          begin
            if (xxx >= 0) and (xxx <= Res_Xend) then
              begin
                kor := z shr 16;
                fst := xxx + yff;
                if kor < zbuf[fst] then
                  begin
                    zbuf[fst] := kor;

                    case MipMapResF of
                      3: TexColor := miptex5[ip, textX shr 16, textY shr 16];
                      4: TexColor := miptex4[ip, textX shr 16, textY shr 16];
                      5: TexColor := miptex3[ip, textX shr 16, textY shr 16];
                      6: TexColor := miptex2[ip, textX shr 16, textY shr 16];
                      7: TexColor := miptex1[ip, textX shr 16, textY shr 16];
                      8: TexColor := texbuf[ip, textX shr 16, textY shr 16];
                    end;

                    Color2RGB(TexColor, rrt, ggt, bbt);

                    rrt := (cR shr 18 + rrt);
                    ggt := (cG shr 18 + ggt);
                    bbt := (cB shr 18 + bbt);

                    if (rrt > 255) then rrt := 255;
                    if (ggt > 255) then ggt := 255;
                    if (bbt > 255) then bbt := 255;

                    memD[DWORD(where) + fst * 4] := RGB2Color(rrt, ggt, bbt);
                  end;
              end;
            z +:= zz;
            cR +:= ccR;
            cG +:= ccG;
            cB +:= ccB;
            textX +:= dxx;
            textY +:= dyy;
          end;
     end;
end;

END.