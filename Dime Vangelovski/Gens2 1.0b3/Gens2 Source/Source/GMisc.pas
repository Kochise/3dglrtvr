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

{Unit for Some Misc Functions -
                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit GMisc;

Interface

Procedure GetRes_HiLo_Values(Res:Dword; var Hi,Lo:Byte);
Procedure Take_ScreenShot;

Implementation

Uses
     Globals,
     GDDraw7M;

Var
    scrshotbuf:array[0..Max_XRes * Max_YRes * 3] of byte;
    sscolor,Ri,Gi,Bi,xshot,yshot:Dword;
    ssR,ssG,ssB,Res_Hi,Res_Lo:byte;
    ssname,sno,opt,ssfile:string;
    zuma:single;
    sshot:file;

Procedure GetRes_HiLo_Values(Res:Dword; var Hi,Lo:Byte);assembler;
asm
   mov EAX, [Res]
   shr EAX, 8
   mov EDI, [Hi]
   mov [EDI], Al
   shl EAX, 8
   mov EDX, [Res]
   sub EDX, EAX
   mov EDI, [Lo]
   mov [EDI], Dl
end;

{This PROCEDURE takes SCREENSHOT and saves it in a 24BPP .BMP file in the
 location \SCRSHOTS\ with the name GENSNo.BMP and increases the number (No)
 of screenshots taken.
 The next time the engine starts the counter returns to zero}
Procedure Take_ScreenShot;
var
    path,curdir:string;
begin
  {Checks if there isn't a directory called "SCRSHOTS" => create it}
  getdir(0,curdir);
  {$I-}
  path:='ScreenShots';
  chdir(path); if IOResult <> 0 then mkdir(path);
  {$I+}
  chdir(curdir);

  {Increase the screenshot count and setup the name of the file to be written}
  inc(ScrShotNo);
  str(ScrShotNo,sno);
  ssname:='ScreenShots\Gens'+sno+'.bmp';
  ssfile:='Gens'+sno+'.bmp';

  {Copies the screen in to a screenshot buffer}
  bi:=0;
  gi:=1;
  ri:=2;

  SurfaceLock;

  {Repeat for the entire screen}
  for yshot := Res_Yend downto 0 do
   for xshot := 0 to Res_Xend do
    begin
      {Copy the current screen pixel color to sscolor variable}
  asm
     mov EAX, [yshot]
     mov EBX, EAX

     push ECX
     mov Cl, [Res_Shl1]
     shl EAX, Cl
     and EBX, [Res_Mask]
     mov Cl, [Res_Shl2]
     shl EBX, Cl
     pop ECX

     add EBX, EAX
     add EBX, [xshot]

     mov EDI, [VirtBuffer]
     mov EAX, [EDI+EBX*4]

     mov EBX, EAX
     shr EAX, 16
     mov [ssR], Al
     shl EAX, 16
     sub EBX, EAX
     mov ECX, EBX
     shr EBX, 8
     mov [ssG], Bl
     shl EBX, 8
     sub ECX, EBX
     mov [ssB], Cl
  end;

      {Write the values in the screenshot buffer}
      scrshotbuf[Ri] := ssR;
      scrshotbuf[Gi] := ssG;
      scrshotbuf[Bi] := ssB;
      inc(ri,3);
      inc(gi,3);
      inc(bi,3);
    end;

    SurfaceUnlock;

    GetDCHandle;

    WriteTxt32bpp('Writing ScreenShot '+ssfile,Res_Xend - 290,Res_Yend - 40 - Vid_YMove_Factor,0,0,0);
    WriteTxt32bpp('Writing ScreenShot '+ssfile,Res_Xend - 291,Res_Yend - 41 - Vid_YMove_Factor,255,255,255);

    assign(sshot,ssname);
    rewrite(sshot,1);

    {Set up the header parameters for 24bpp .BMP}
    GetRes_HiLo_Values(Res_Xend_p1, Res_Hi, Res_Lo);
    shotheader[18] := Res_Lo;
    shotheader[19] := Res_Hi;
    GetRes_HiLo_Values(Res_Yend_p1, Res_Hi, Res_Lo);
    shotheader[22] := Res_Lo;
    shotheader[23] := Res_Hi;
    shotheader[28] := 24;

    {Write the .BMP to the hard disk}
    blockwrite(sshot,shotheader, 54);
    blockwrite(sshot,scrshotbuf, Res_Xend_p1 * Res_Yend_p1 * 3);
    close(sshot);

    zuma +:= Res_Xend_p1 * Res_Yend_p1 * 3 / sqr(1024);
    WriteTxt32bpp('Written:',Res_Xend - 290,Res_Yend - 30,0,0,0);
    WriteTxt32bpp('Written:',Res_Xend - 291,Res_Yend - 31,255,255,255);
    WriteNo32bpp(zuma,0,1,Res_Xend - 210,Res_Yend - 30,0,0,0);
    WriteNo32bpp(zuma,0,1,Res_Xend - 211,Res_Yend - 31,255,255,255);
    WriteTxt32bpp('MB',Res_Xend - 170 + Vid_YMove_Factor,Res_Yend - 30,0,0,0);
    WriteTxt32bpp('MB',Res_Xend - 171 + Vid_YMove_Factor,Res_Yend - 31,255,255,255);

    ReleaseDCHandle;
end;

END.