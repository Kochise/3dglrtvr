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

{Unit for the Lens Flare (Light Corona) Rendering functions -
                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit GLCorona;

Interface

Uses
     GTypes3D;

Procedure Textured_Light(x1,x2,yyy,c1R,c1G,c1B,texx1,texy1,texx2,texy2,z1,z2:Integer; intens:Dword; where:Pointer);
Procedure DrawLightSprite(LightPos:vertex; zcl:single; r,g,b,intens:Dword; IsDyn:boolean; where:Pointer);

Implementation

{$I Gdefines.inc}

Uses
     Globals,
     GMath3D,
{$ifdef __PASCAL_DRAW__}
     Draw_pas,
{$else}
     Draw_asm,
{$endif}
     GDrwPrim;

{Render 32BPP textured horizontal line on X1,X2,Y with texture TEXLIGHT (lens flare),
 c1R,.. c2R,... are start and end RGB values for the gouraud renderer,
 texx1,texy1,texx2,texy2 are start and end texture coordinates (u,v).
 This PROCEDURE does additive alpha blending}
Procedure Textured_Light(x1,x2,yyy,c1R,c1G,c1B,texx1,texy1,texx2,texy2,z1,z2:Integer; intens:Dword; where:Pointer);
var xxx,tmp,textx,texty,dxx,dy,korx,kory,kor,ccol,z,zz,cR,cG,cB,ccR,ccG,ccB,proz,tmpR,tmpG,tmpB:Integer;
    yff,fst,aaa,memm,xftmp,yftmp,fad,alm,tmpadr,r1,r2,g1,g2,b1,b2,
    rfin,gfin,bfin:Dword;
begin
  {Check if the line is out of screen => exit}
  if (yyy < 0) or (yyy > Res_Yend) then exit;
  if (x1 > Res_Xend) and (x2 > Res_Xend) then exit;
  if (x1 < 0) and (x2 < 0) then exit;
  yff := yyy shl Res_Shl1 + (yyy and Res_Mask) shl Res_Shl2;

  if x1>x2 then
    begin
      tmp:=x1; x1:=x2; x2:=tmp;
      tmp:=texx1; texx1:=texx2; texx2:=tmp;
      tmp:=texy1; texy1:=texy2; texy2:=tmp;
    end;
  {Calculate starting values in 16.16 fixed point for speed}
  kor:=x2-x1;
  korx:=(texx2-texx1) shl 16;
  kory:=(texy2-texy1) shl 16;
  if kor<>0 then
    begin
      dxx:=korx div (x2-x1);
      dy:=kory div (x2-x1);
    end;
  textx:=texx1 shl 16;
  texty:=texy1 shl 16;
  intens := intens div 1500;
  if intens > 300 then intens := 300;

asm
   {Initialize the variables}
   mov ECX, [x1]
   mov EAX, [x2]
   dec EAX
   mov [x2], EAX

@rep:
   {check if the current pixel on the line is out of screen => skip}
   cmp ECX, 0                     -- U
   jl  @next                      -- V

   cmp ECX, [Res_Xend]            -- U
   jg  @next                      -- V          - 1

   {calculate current texel address in the texture}
   mov EDI, [textY]               -- V
   shr EDI, 16                    -- U
   mov EAX, [textX]               -- U
   shr EAX, 16                    -- U
   shl EAX, 8                     -- U
   add EAX, EDI
   lea EDI, texlight[EAX*4]       -- U + AGI    - 2 {The texture}
   mov EAX, [EDI]                 { EAX has the 32bpp color }
   mov [alm], EAX

   {copy the pixel from the screen at the coordinate ECX}
   mov EDI, [yff]
   add EDI, ECX
   shl EDI, 2
   add EDI, [where]
   mov [tmpadr], EDI
   mov EAX, [EDI]                 -- V + AGI

   {convert color to RGB}
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

   mov EAX, [alm]
   mov EBX, [alm]
   shr EAX, 16
   mov [r1], EAX
   shl EAX, 16
   sub EBX, EAX
   mov EDI, EBX
   shr EBX, 8
   mov [g1], EBX
   shl EBX, 8
   sub EDI, EBX
   mov [b1], EDI

   {this part colours the texture with RGB, and so, different
    color textures are not needed}

   {add the red value to the texel and clamp the value}
   mov EAX, 255
   sub EAX, [r1]
   mov EBX, [c1R]
   sub EBX, EAX
   cmp EBX, 0
   jge @great1
   mov EBX, 0
@great1:
   mov EAX, [r1]
   add EAX, EBX
   mov [r1], EAX

   {add the green value to the texel and clamp the value}
   mov EAX, 255
   sub EAX, [g1]
   mov EBX, [c1G]
   sub EBX, EAX
   cmp EBX, 0
   jge @great2
   mov EBX, 0
@great2:
   mov EAX, [g1]
   add EAX, EBX
   mov [g1], EAX

   {add the blue value to the texel and clamp the value}
   mov EAX, 255
   sub EAX, [b1]
   mov EBX, [c1B]
   sub EBX, EAX
   cmp EBX, 0
   jge @great3
   mov EBX, 0
@great3:
   mov EAX, [b1]
   add EAX, EBX
   mov [b1], EAX

   {adjust the lens flare intensity according to the light intensity}
   mov EAX, [r1]
   mul [intens]
   shrd EAX, EDX, 8
   cmp EAX, 255
   jle @nrr
   mov EAX, 255
@nrr:
   mov [r1], EAX

   mov EAX, [g1]
   mul [intens]
   shrd EAX, EDX, 8
   cmp EAX, 255
   jle @ngg
   mov EAX, 255
@ngg:
   mov [g1], EAX

   mov EAX, [b1]
   mul [intens]
   shrd EAX, EDX, 8
   cmp EAX, 255
   jle @nbb
   mov EAX, 255
@nbb:
   mov [b1], EAX

   {add the current texel and the screen pixel and clamp the value -
    - this is additive alpha blending}
   {green values}
   mov EAX, [g1]
   add EAX, [g2]
   cmp EAX, 255
   jle @nx22
   mov EAX, 255
@nx22:
   mov [gfin], EAX

   {blue values}
   mov EAX, [b1]
   add EAX, [b2]
   cmp EAX, 255
   jle @nx33
   mov EAX, 255
@nx33:
   mov [bfin], EAX

   {red values}
   mov EAX, [r1]
   add EAX, [r2]
   cmp EAX, 255
   jle @nx11
   mov EAX, 255
@nx11:

   {convert from RGB back to color}
   mov EBX, [gfin]
   shl EAX, 16
   mov EDI, [bfin]
   shl EBX, 8
   add EAX, EBX
   add EAX, EDI

   {draw current pixel}
   mov EDI, [tmpadr]
   mov [EDI], EAX                 -- V + AGI

@next:
   {increase the appropriate values by their factors for the next pixel}
   mov EAX, [dxx]                 -- U
   mov EDI, [dy]                  -- V
   add [textX], EAX               -- U
   add [textY], EDI               -- V

   inc ECX                        -- V

   mov EAX, [x2]                  -- U
   inc ESI                        -- U
   cmp ECX, EAX                   -- V
   jle @rep                       -- V
end;
end;

{This Calculates and Draws the Lens Flare Sprites for STATIC lights
 with position RocketPos, RGB color, on adress where}
Procedure DrawLightSprite(LightPos:vertex; zcl:single; r,g,b,intens:Dword; IsDyn:boolean; where:Pointer);
var tstx1,tsty1,rocx1,rocx2,rocx3,rocx4,rocy1,rocy2,rocy3,rocy4,rrx,rry,zchk,mny,mxy,y:Integer;
    scalecor,fixfac,intenscale:single;
begin
  // FIXME: This disables the flares for 1280x960 res. A nasty bug there is.
  // I don't know how to remove it yet. It has something to do with LightPos.z ????
  if (Res_Xend_p1 = 1280) then Exit;

  {Check if the lens flare is behind the viewer => exit}
  if LightPos.z < 3 then Exit;

  {Check if the lens flare is behind some object => don't draw it,
  and also checks if the lens is out of screen => don't draw it}
  tstx1:=round((LightPos.x*focusX)/(LightPos.z)+xcenter);
  tsty1:=round((LightPos.y*focusY)/(LightPos.z)+ycenter);
  if (tstx1<0) or (tstx1>Res_Xend) then exit;
  if (tsty1<0) or (tsty1>Res_Yend) then exit;
  if LightPos.z>zbuf[tsty1 shl Res_Shl1 + (tsty1 and Res_Mask) shl Res_Shl2 + tstx1] then exit;

  {This calculates the size of the lens flare according to the viewer
  distance and does some clamping for size limiting}
  scalecor:=Safe_Sqrt(sqr(PlayerVectorMove.x-LightPos.x)+sqr(PlayerVectorMove.y-LightPos.y)+
                      sqr(PlayerVectorMove.z-LightPos.z)) / 7;
  if scalecor<100 then scalecor:=100;

  if IsDyn then
    scalecor:=100;

 {This calculates the polygon coordinates according to the scalecor parameter}

 {Calculate the polygon coordinates for the flare}
  rocx1:=round(((LightPos.x-scalecor)*focusX)/(LightPos.z)+xcenter);
  rocy1:=round(((LightPos.y-scalecor)*focusY)/(LightPos.z)+ycenter);

  rocx2:=round(((LightPos.x-scalecor)*focusX)/(LightPos.z)+xcenter);
  rocy2:=round(((LightPos.y+scalecor)*focusY)/(LightPos.z)+ycenter);

  rocx3:=round(((LightPos.x+scalecor)*focusX)/(LightPos.z)+xcenter);
  rocy3:=round(((LightPos.y+scalecor)*focusY)/(LightPos.z)+ycenter);

  rocx4:=round(((LightPos.x+scalecor)*focusX)/(LightPos.z)+xcenter);
  rocy4:=round(((LightPos.y-scalecor)*focusY)/(LightPos.z)+ycenter);
  zchk:=round(LightPos.z);

  {Find the minimum (highest) y coordinate from the polygon for starting
  point for the polygon rendering}
  mny:=rocy1;
  mxy:=rocy1;
  if rocy2<mny then mny:=rocy2;
  if rocy2>mxy then mxy:=rocy2;
  if rocy3<mny then mny:=rocy3;
  if rocy3>mxy then mxy:=rocy3;
  if rocy4<mny then mny:=rocy4;
  if rocy4>mxy then mxy:=rocy4;

  {This checks if the polygon is out of screen => exit.
  Also does some clamping and initial setup for the scanedge PROCEDURE}
  if mny<0 then mny:=0;
  if mxy<0 then exit;

  if mxy>Res_Yend then mxy:=Res_Yend;
  if mny>Res_Yend then exit;

  for y := 0 to Res_Yend do
    begin
      poly[y,1]:=Res_Xend_p1;
      poly[y,2]:=-1;
    end;

  {This calculates the needed values for rendering the polygon for all
  four sides of the polygon}
  Scan_Edge(rocx1,rocy1,rocx2,rocy2,c1R,c1G,c1B,c2R,c2G,c2B,zchk,zchk,0,0,TexLightX,0);
  Scan_Edge(rocx2,rocy2,rocx3,rocy3,c2R,c2G,c2B,c3R,c3G,c3B,zchk,zchk,TexLightX,0,TexLightX,TexLightY);
  Scan_Edge(rocx3,rocy3,rocx4,rocy4,c3R,c3G,c3B,c4R,c4G,c4B,zchk,zchk,TexLightX,TexLightY,0,TexLightY);
  Scan_Edge(rocx4,rocy4,rocx1,rocy1,c4R,c4G,c4B,c1R,c1G,c1B,zchk,zchk,0,TexLightY,0,0);

  {This draws the outlines of the polygon in white color}
  if (showtris) then
  begin
    line32bpp(rocx1, rocy1, rocx2, rocy2, 200, 200, 200, where);
    line32bpp(rocx2, rocy2, rocx3, rocy3, 200, 200, 200, where);
    line32bpp(rocx3, rocy3, rocx4, rocy4, 200, 200, 200, where);
    line32bpp(rocx4, rocy4, rocx1, rocy1, 200, 200, 200, where);
  end;

  {This renders the lens flare polygon with additive alpha blending by
  vertically drawing all the horizontal lines starting from the top (lowest)
  y coordinate of the polygon}
  for y := mny to mxy do
     Textured_Light(poly[y,1],poly[y,2],y,R,G,B,tex[y,1],tex[y,2],tex[y,3],tex[y,4],ztmp[y,1],ztmp[y,2],intens,where);
end;

END.