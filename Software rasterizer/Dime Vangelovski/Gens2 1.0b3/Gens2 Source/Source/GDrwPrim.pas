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

{Unit for Some Drawing and Conversion functions -
                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit GDrwPrim;

Interface

function RGB2Color(R, G, B:Dword):Dword;
procedure Color2RGB(Color:Dword; var R, G, B:Dword);
Procedure Cls32bpp(r,g,b:Dword; where:Pointer);
Procedure Tocka32bpp(x,y,r,g,b:Dword; where:Pointer);
Procedure Tocka32bppAlpha(x,y:Dword; transi:byte; r,g,b:Dword; where:Pointer);
Procedure Line32bpp(x1,y1,x2,y2:Integer; r,g,b:Dword; where:Pointer);

Implementation

Uses
     Globals;

{RGB to 32BPP Color Conversion}
function RGB2Color(R, G, B:Dword):Dword; assembler;
asm
   mov EAX, [R]
   mov EBX, [G]
   shl EAX, 16
   mov ECX, [B]
   shl EBX, 8
   add EAX, EBX
   add EAX, ECX
end;

{32BPP Color to RGB Conversion}
procedure Color2RGB(Color:Dword; var R, G, B:Dword); assembler;
asm
   mov EAX, [Color]
   mov EBX, [Color]
   shr EAX, 16
   mov EDI, [R]
   mov [EDI], EAX
   shl EAX, 16
   sub EBX, EAX
   mov ECX, EBX
   shr EBX, 8
   mov EDI, [G]
   mov [EDI], EBX
   shl EBX, 8
   sub ECX, EBX
   mov EDI, [B]
   mov [EDI], ECX
end;

{Clear Screen for 32BPP Modes}
Procedure Cls32bpp(r,g,b:Dword; where:Pointer);assembler;
asm
   cld
   mov  EAX, [r]
   mov  EBX, [g]
   shl  EAX, 16
   mov  ECX, [b]
   shl  EBX, 8
   add  EAX, EBX
   add  EAX, ECX

   mov  EDI, [where]
   mov  ECX, [Res_PixNum]
   ALIGN     4
   rep  stosd
end;

{Draws a 32BPP RGB Pixel on X,Y,  Where is the memory addres to start}
Procedure Tocka32bpp(x,y,r,g,b:Dword; where:Pointer);
var sadr:Dword;
begin
  sadr := y shl Res_Shl1 + (y and Res_Mask) shl Res_Shl2 + x;
  asm
     mov EAX, [r]
     mov EBX, [g]
     shl EAX, 16
     mov ECX, [b]
     shl EBX, 8
     add EAX, EBX
     add EAX, ECX

     mov EBX, [sadr]
     mov EDI, [where]
     mov [EDI+EBX*4], EAX
  end;
end;

{Draws a 32BPP RGB Pixel on X,Y, transi is an alpha translucency value - bigger value = more translucent
 Where is the memory addres to start}
Procedure Tocka32bppAlpha(x,y:Dword; transi:byte; r,g,b:Dword; where:Pointer);
var prva,vtora,r1,r2,g1,g2,b1,b2,rr,gg,bb,sadr:Dword;
begin
  sadr := y shl Res_Shl1 + (y and Res_Mask) shl Res_Shl2 + x;
  asm
     mov EAX, [r]
     mov EBX, [g]
     shl EAX, 16
     mov ECX, [b]
     shl EBX, 8
     add EAX, EBX
     add EAX, ECX
     mov [prva], EAX

     mov EBX, [sadr]
     mov EDI, [where]
     mov EAX, [EDI+EBX*4]
     mov [vtora], EAX

  {convert color value to RGB}
  mov EAX, [prva]
  mov EBX, [prva]
  shr EAX, 16
  mov [r1], EAX
  shl EAX, 16
  sub EBX, EAX
  mov EDX, EBX
  shr EBX, 8
  mov [g1], EBX
  shl EBX, 8
  sub EDX, EBX
  mov [b1], EDX

  mov EAX, [vtora]
  mov EBX, [vtora]
  shr EAX, 16
  mov [r2], EAX
  shl EAX, 16
  sub EBX, EAX
  mov EDX, EBX
  shr EBX, 8
  mov [g2], EBX
  shl EBX, 8
  sub EDX, EBX
  mov [b2], EDX

  {Calculate a factor}
  mov Al, [transi]
  mov Bl, 256
  sub Bl, Al

   {Calculate alpha values by:
    color=F*Bl+S*transi (F is the color of the working pixel, S is the color
    of the screen, Bl is the factor, transi is the translucency}
   {for green color}
   mov EAX, [g1]
   mul Bl
   shr EAX, 8
   mov EDX, EAX
   mov EAX, [g2]
   mul [transi]
   shr EAX, 8
   add EAX, EDX
   mov [gg], EAX

   {for blue color}
   mov EAX, [b1]
   mul Bl
   shr EAX, 8
   mov EDX, EAX
   mov EAX, [b2]
   mul [transi]
   shr EAX, 8
   add EAX, EDX
   mov [bb], EAX

   {for red color}
   mov EAX, [r1]
   mul Bl
   shr EAX, 8
   mov EDX, EAX
   mov EAX, [r2]
   mul [transi]
   shr EAX, 8
   add EAX, EDX

  {convert from RGB back to color value}
  mov EBX, [gg]
  shl EAX, 16
  mov EDX, [bb]
  shl EBX, 8
  add EAX, EBX
  add EAX, EDX

     mov EBX, [sadr]
     mov EDI, [where]
     mov [EDI+EBX*4], EAX
  end;
end;

{Draws a 32BPP Line with RGB}
Procedure Line32bpp(x1,y1,x2,y2:Integer; r,g,b:Dword; where:Pointer);
var i,deltax,deltay,numpixels,
    d,dinc1,dinc2,
    x,xinc1,xinc2,
    y,yinc1,yinc2:Integer;
    color,xend,yend:Dword;
begin
  deltax:=abs(x2-x1);
  deltay:=abs(y2-y1);

  if deltax >= deltay then
    begin
     numpixels:=deltax+1;
     d:=(2*deltay)-deltax;
     dinc1:=deltay shl 1;
     dinc2:=(deltay-deltax) shl 1;
     xinc1:=1;
     xinc2:=1;
     yinc1:=0;
     yinc2:=1;
    end
   else
    begin
     numpixels:=deltay+1;
     d:=(2*deltax)-deltay;
     dinc1:=deltax shl 1;
     dinc2:=(deltax-deltay) shl 1;
     xinc1:=0;
     xinc2:=1;
     yinc1:=1;
     yinc2:=1;
   end;

 if x1>x2 then
   begin
    xinc1:=-xinc1;
    xinc2:=-xinc2;
   end;
 if y1>y2 then
   begin
    yinc1:=-yinc1;
    yinc2:=-yinc2;
   end;

 x:=x1;
 y:=y1;

asm
  {Convert from RGB to color}
  mov EAX, [r]
  mov EBX, [g]
  shl EAX, 16
  mov ECX, [b]
  shl EBX, 8
  add EAX, EBX
  add EAX, ECX
  mov [color], EAX

  mov ECX, [numpixels]
  mov EDI, [x]
  mov ESI, [y]
  mov EDX, [d]

@rept:
  {Check if the current pixel is out of screen => don't draw it}
  cmp EDI, 0
  jl  @fuckit
  cmp ESI, 0
  jl  @fuckit
  cmp EDI, [Res_Xend]
  jg  @fuckit
  cmp ESI, [Res_Yend]
  jg  @fuckit

  {Calculate the address}
  mov EAX, ESI
  mov EBX, EAX

  push ECX
  mov Cl, [Res_Shl1]
  shl EAX, Cl
  and EBX, [Res_Mask]
  mov Cl, [Res_Shl2]
  shl EBX, Cl
  pop ECX

  add EBX, EAX
  add EBX, EDI
  mov EAX, [color]
  add EBX, EBX
  add EBX, EBX
  add EBX, [where]
  mov [EBX], EAX
@fuckit:

  {Check if EDX<0 then dinc1,xinc1... else dinc2,xinc2...}
  cmp EDX, 0
  jl  @tnext
  add EDX, [dinc2]
  add EDI, [xinc2]
  add ESI, [yinc2]
  jmp @thenext
@tnext:
  add EDX, [dinc1]
  add EDI, [xinc1]
  add ESI, [yinc1]
@thenext:

  dec ECX
  jnz @rept
end;
end;

END.