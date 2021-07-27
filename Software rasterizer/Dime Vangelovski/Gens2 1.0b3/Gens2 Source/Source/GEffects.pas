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

{Unit for some Graphical Effects -
                                                                   - Made by Dime Vangelovski
                                                                                  29.08.2005}
Unit GEffects;

Interface

Procedure Filter(x1,y1,x2,y2,pp,MipFac,ResF,TexOfs:Dword);
Procedure fsa32bpp(where:Pointer);
Procedure fillfinta(x1,y1,x2,y2:Integer; color:byte; where:Pointer);
Procedure fillrectfalpha(x1,y1,x2,y2:Integer; transi:byte; r,g,b:Dword; where:Pointer);

Implementation

Uses
     Globals,
     GDrwPrim;

{PROCEDURE for Prerendered BILINEAR Filtering - General
x1,y1,x2,y2 are the size of the texture, pp is the texture
index, MipFac is the scale factor for size, ResF is the
scale factor for x-size, TexOfs is the texture offset}
Procedure Filter(x1,y1,x2,y2,pp,MipFac,ResF,TexOfs:Dword);
var
    xx,yy,boj1,boj2,boj3,boj4,boj5,filr,filg,filb,
    boj1r,boj1g,boj1b,boj2r,boj2g,boj2b,boj3r,boj3g,
    boj3b,boj4r,boj4g,boj4b,boj5r,boj5g,boj5b,addr:Dword;
begin
  for xx:=x1 to x2 do
    for yy:=y1 to y2 do
       begin
         {This copies values from the texture to boj variables}
         {from coordinates around the pixel with xx,yy coord}
         addr:=TexOfs+(pp-1) shl MipFac;
         if xx>x1 then
           boj1:=memd[addr+(yy shl ResF+xx-1) shl 2]
         else
           boj1:=memd[addr+(yy shl ResF+xx) shl 2];
         if xx<x2 then
           boj2:=memd[addr+(yy shl ResF+xx+1) shl 2]
         else
           boj2:=memd[addr+(yy shl ResF+xx) shl 2];
         if yy>y1 then
           boj3:=memd[addr+((yy-1) shl ResF+xx) shl 2]
         else
           boj3:=memd[addr+(yy shl ResF+xx) shl 2];
         if yy<y2 then
           boj4:=memd[addr+((yy+1) shl ResF+xx) shl 2]
         else
           boj4:=memd[addr+(yy shl ResF+xx) shl 2];
         boj5:=memd[addr+(yy shl ResF+xx) shl 2];

         {This converts values from the boj variables in to RGB componets}
         Color2RGB(boj1, boj1r, boj1g, boj1b);
         Color2RGB(boj2, boj2r, boj2g, boj2b);
         Color2RGB(boj3, boj3r, boj3g, boj3b);
         Color2RGB(boj4, boj4r, boj4g, boj4b);
         Color2RGB(boj5, boj5r, boj5g, boj5b);

         {This calculates an average sum from the RGB boj variables}
         filr := (((boj1r + boj2r + boj3r + boj4r) shr 2) + boj5r) shr 1;
         filg := (((boj1g + boj2g + boj3g + boj4g) shr 2) + boj5g) shr 1;
         filb := (((boj1b + boj2b + boj3b + boj4b) shr 2) + boj5b) shr 1;

         {This converts the average RGB sum to color value and writes
         it back to the texture on coordinate xx,yy}
         memd[addr + (yy shl ResF + xx) shl 2] := RGB2Color(filr, filg, filb);
       end;
end;

{Full Screen Antialiasing (FSAA) PROCEDURE, Where is the start memory adress}
Procedure fsa32bpp(where:Pointer);assembler;
var
    prva,vtora,treta,cetvrta,peta,
    r1,r2,r3,r4,r5,g1,g2,g3,g4,g5,
    b1,b2,b3,b4,b5,rr,gg,bb:Dword;
asm
   mov EDX, [Res_Xend_m1]
@x:
   mov ECX, [Res_Yend_m1]
@y:
   {Calculate address}
   mov EAX, ECX
   mov EBX, EAX

   push ECX
   mov Cl, [Res_Shl1]
   shl EAX, Cl
   and EBX, [Res_Mask]
   mov Cl, [Res_Shl2]
   shl EBX, Cl
   pop ECX

   add EBX, EAX
   add EBX, EDX

   {This part copyes color values from the screen and puts them in
    prva,vtora,treta... variables. It takes the pixels surrounding
    the x,y coordinate by 1 pixel}
   mov EDI, [where]
   cmp EDX, 0
   jle @next1
   mov ESI, EBX
   dec ESI
   mov EAX, [EDI+ESI*4]   {x-1}
   mov [prva], EAX
@next1:
   cmp EDX, [Res_Xend]
   jge @next2
   mov ESI, EBX
   inc ESI
   mov EAX, [EDI+ESI*4]   {x+1}
   mov [vtora], EAX
@next2:
   cmp ECX, 0
   jle @next3
   mov ESI, EBX
   sub ESI, [Res_Xend_p1]
   mov EAX, [EDI+ESI*4]   {y-1}
   mov [treta], EAX
@next3:
   cmp ECX, [Res_Yend]
   jge @next4
   mov ESI, EBX
   add ESI, [Res_Xend_p1]
   mov EAX, [EDI+ESI*4]   {y+1}
   mov [cetvrta], EAX
@next4:
   mov EAX, [EDI+EBX*4]   {x,y}
   mov [peta], EAX

   {Check if these 5 pixels are all with the same values. If they are
   skip the calculation and draw the first pixel}
   mov ESI, [cetvrta]
   cmp EAX, ESI         {5,4}
   jnz @tocno
   mov EAX, [treta]
   cmp ESI, EAX         {4,3}
   jnz @tocno
   mov ESI, [vtora]
   cmp EAX, ESI         {3,2}
   jnz @tocno
   mov EAX, [prva]
   cmp ESI, EAX         {2,1}
   jnz @tocno

   mov [EDI+EBX*4], EAX
   jmp @netocno

@tocno:
   {converts the values from prva,vtora,treta... to RGB components}
   push EBX
   mov EAX, [peta]
   mov EBX, [peta]
   shr EAX, 16
   mov [r5], EAX
   shl EAX, 16
   sub EBX, EAX
   mov ESI, EBX
   shr EBX, 8
   mov [g5], EBX
   shl EBX, 8
   sub ESI, EBX
   mov [b5], ESI

   mov EAX, [prva]
   mov EBX, [prva]
   shr EAX, 16
   mov [r1], EAX
   shl EAX, 16
   sub EBX, EAX
   mov ESI, EBX
   shr EBX, 8
   mov [g1], EBX
   shl EBX, 8
   sub ESI, EBX
   mov [b1], ESI

   mov EAX, [vtora]
   mov EBX, [vtora]
   shr EAX, 16
   mov [r2], EAX
   shl EAX, 16
   sub EBX, EAX
   mov ESI, EBX
   shr EBX, 8
   mov [g2], EBX
   shl EBX, 8
   sub ESI, EBX
   mov [b2], ESI

   mov EAX, [treta]
   mov EBX, [treta]
   shr EAX, 16
   mov [r3], EAX
   shl EAX, 16
   sub EBX, EAX
   mov ESI, EBX
   shr EBX, 8
   mov [g3], EBX
   shl EBX, 8
   sub ESI, EBX
   mov [b3], ESI

   mov EAX, [cetvrta]
   mov EBX, [cetvrta]
   shr EAX, 16
   mov [r4], EAX
   shl EAX, 16
   sub EBX, EAX
   mov ESI, EBX
   shr EBX, 8
   mov [g4], EBX
   shl EBX, 8
   sub ESI, EBX
   mov [b4], ESI

   {Calculate an average sum of all the 5 pixels}
   mov EAX, [g1]
   mov EBX, [g2]
   mov ESI, [g3]
   add EAX, EBX
   add EAX, ESI
   mov EBX, [g4]
   mov ESI, [g5]
   add EAX, EBX
   shr EAX, 2
   add EAX, ESI
   shr EAX, 1
   mov [gg], EAX

   mov EAX, [b1]
   mov EBX, [b2]
   mov ESI, [b3]
   add EAX, EBX
   add EAX, ESI
   mov EBX, [b4]
   mov ESI, [b5]
   add EAX, EBX

   shr EAX, 2
   add EAX, ESI
   shr EAX, 1
   mov [bb], EAX

   mov EAX, [r1]
   mov EBX, [r2]
   mov ESI, [r3]
   add EAX, EBX
   add EAX, ESI
   mov EBX, [r4]
   mov ESI, [r5]
   add EAX, EBX
   shr EAX, 2
   add EAX, ESI
   shr EAX, 1

   {Convert from RGB back to Color value}
   mov EBX, [gg]
   shl EAX, 16
   mov ESI, [bb]
   shl EBX, 8
   add EAX, EBX
   pop EBX
   add EAX, ESI

   {Draw the pixel}
   mov [EDI+EBX*4], EAX

@netocno:
   {Jump to @y until ECX=0}
   dec ECX
   jnz @y

   {Jump to @x until EDX=0}
   dec EDX
   jnz @x
end;

{To Fill a rectangle by some tweak fashion -
 (the starting GENS and LOADING font dysplays)}
Procedure fillfinta(x1,y1,x2,y2:Integer; color:byte; where:Pointer);
var sadr:Dword;
begin
  sadr := y1 shl Res_Shl1 + (y1 and Res_Mask) shl Res_Shl2 + x1;
asm
  {Initialize the variables}
  mov ESI, [x1]
  mov ECX, [x2]
  mov EBX, [y2]
  mov EDX, [y1]
  {Check if x1>x2 switch them}
  cmp ESI, ECX
  jle @skip
  xchg ESI, ECX
@skip:
  {Check if y1>y2 switch them}
  cmp EDX, EBX
  jle @skp
  xchg EDX, EBX
@skp:
  {Starting address}
  mov EDI, [sadr]
  xor EAX, EAX
  mov al, [color]
  mov ah, al
  {Calculate the length x2-x1 and y2-y1}
  inc EBX
  sub EBX, EDX
  inc ECX
  sub ECX, ESI
  mov ESI, ECX
  mov EDX, EDI
 {Repeat for y:=y1 to y2 do rep stosd}
@rep:
  mov ECX, ESI
  mov EDI, EDX
  add EDI, [where]
  push ECX
  and ECX, 3
  jz  @skok
  rep stosb
@Skok:
  pop ECX
  shr ECX, 2
  push EAX
  shl EAX, 16
  pop EAX
  rep stosd     {for x:=x1 to x2 do Draw 4 pixels at a time}

  add EDX, [Res_Xend_p1]
  dec EBX
  jnz @rep
end;
end;

{To Fill a rectangle with alpha blending, TRANSI is the translucency index
 (0 < transi < 255) the higher number the more translucent}
Procedure fillrectfalpha(x1,y1,x2,y2:Integer; transi:byte; r,g,b:Dword; where:Pointer);
var color,n,prva,vtora,r1,r2,g1,g2,b1,b2,rr,gg,bb,sadr:Dword;
begin
  sadr := y1 shl Res_Shl1 + (y1 and Res_Mask) shl Res_Shl2 + x1;
asm
  {Convert RGB to color value}
  mov EAX, [r]
  mov EBX, [g]
  shl EAX, 16
  mov ECX, [b]
  shl EBX, 8
  add EAX, EBX
  add EAX, ECX
  mov [color], EAX

  {Initialize the variables}
  mov ESI, [x1]
  mov ECX, [x2]
  mov EBX, [y2]
  mov EDX, [y1]
  {Check if x1>x2 switch them}
  cmp ESI, ECX
  jle @skip
  xchg ESI, ECX
@skip:
  {Check if y1>y2 switch them}
  cmp EDX, EBX
  jle @skp
  xchg EDX, EBX
@skp:
  {Start address}
  mov EDI, [sadr]
  {Calculate x2-x1 and y2-y1}
  inc EBX
  sub EBX, EDX
  inc ECX
  sub ECX, ESI
  mov EDX, EDI
  mov [n], ECX
  dec EBX

@rep:
  mov ECX, [n]
  dec ECX
 {Repeat for y:=y1 to y2 do draw pixel}
@rep1:
  mov ESI, ECX
  add ESI, EDX
  mov EDI, [where]
  mov EAX, [color]
  shl ESI, 2
  {Copy the work pixel and the screen pixel to prva,vtora}
  mov [prva], EAX
  mov EAX, [EDI+ESI]
  mov [vtora], EAX

  {convert color value to RGB}
  push EBX
  push EDX
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
  pop EDX
  pop EBX

  {draw the pixel}
  mov [EDI+ESI], EAX
  dec ECX
  jnz @rep1

  add EDX, [Res_Xend_p1]
  dec EBX
  jnz @rep
end;
end;

END.