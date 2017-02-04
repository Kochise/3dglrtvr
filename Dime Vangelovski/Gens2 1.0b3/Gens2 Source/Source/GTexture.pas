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

{Unit for Texture Loading, MipMapping and Filtering functions -
                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit GTexture;

Interface

Uses
     Globals;

Procedure LoadBMP (Filename:string; Texi:Integer);
Procedure LoadBMPFree (Filename:string; var FreeTex:BMPtex);
Procedure GenerateMipMaps(SrcInd,DestInd:Dword);
Procedure MipMapDetermine(mipp:single; ip:Integer);
Procedure SetupMipMaps;
Procedure Generate_NoTex;
Procedure LoadTextures;

Function Average_Color(a, b: Dword): Dword;

Implementation

Uses
     GTypes3D,
     Windows,
     GWinMnge,
     GEffects,
     GDrwPrim,
     GOut2Log;

Var
    bpp,nxx,nyy,Rii,Gii,Bii:Integer;
    bmprgb:array[0..256 * 256 * 3] Of Byte;
    fR,fG,fB:byte;
    fff:file;

{Loads a 24BPP .BMP image in to TEXBUF (main 256x256 texture) and does
 some SETUP. Texi is the current texture number}

Procedure LoadBMP (Filename:string; Texi:Integer);
Var
    strMSG: String;
begin
  assign(fff, filename);
  FileMode := 0;  {Just READ the file, don't open for WRITING - so it can work from a CD-ROM}
  reset(fff);

  BlockRead(fff,shotheader,54);

  {Read the header}
  for i:=0 to 53 do
  begin
    tmp:=shotheader[i];
    case i of
      18: xres:=tmp;
      19: xres:=tmp*256+xres;
      22: yres:=tmp;
      23: yres:=tmp*256+yres;
      28: bpp:=tmp;
    end;
  end;

  {Check if it's 24bpp}
  if bpp<>24 then
  begin
    strMSG :='The file ' + filename + ' is not a 24bpp BMP !   ';
    if DisplayMessage(strMSG, WINDOW_TITLE, MSG_ERROR) = IDOK then
      begin
        PostQuitMessage(0);
        Halt(0);
      end;
  end;

  TexXres[texi]:=xres-1;
  TexYres[texi]:=yres-1;

  Seek(fff,FileSize(fff)-xres*yres*3);
  BlockRead(fff,bmprgb,xres*yres*3);

  {Read the .BMP and write in to TEXBUF}
  Bii:=0;
  Gii:=1;
  Rii:=2;
  for nyy:=yres-1 downto 0 do
  for nxx:=0 to xres-1 do
  begin
    fR:=bmprgb[Rii];
    fG:=bmprgb[Gii];
    fB:=bmprgb[Bii];
    texbuf[texi,nyy,nxx]:=rgb2color(fR,fG,fB);
    inc(Rii,3);
    inc(Gii,3);
    inc(Bii,3);
  end;

  close(fff);
  FileMode := 2;  {Return the default value - READ/WRITE}
end;

{Loads a 24BPP .BMP image in to FreeTex (for Lens Flare) and does some SETUP}
Procedure LoadBMPFree (Filename:string; var FreeTex:BMPtex);
Var
    strMSG: String;
begin
  assign(fff, filename);
  FileMode := 0;  {Just READ the file, don't open for WRITING - so it can work from a CD-ROM}
  reset(fff);

  BlockRead(fff,shotheader,54);

  {Read the header}
  for i:=0 to 53 do
  begin
    tmp:=shotheader[i];
    case i of
      18: xres:=tmp;
      19: xres:=tmp*256+xres;
      22: yres:=tmp;
      23: yres:=tmp*256+yres;
      28: bpp:=tmp;
    end;
  end;

  {Check if it's 24bpp}
  if bpp<>24 then
  begin
    strMSG :='The file ' + filename + ' is not a 24bpp BMP !   ';
    if DisplayMessage(strMSG, WINDOW_TITLE, MSG_ERROR) = IDOK then
      begin
        PostQuitMessage(0);
        Halt(0);
      end;
  end;

  TexLightX:=xres-1;
  TexLightY:=yres-1;

  Seek(fff,FileSize(fff)-xres*yres*3);
  BlockRead(fff,bmprgb,xres*yres*3);

  {Read the .BMP and write in to FREETEX}
  Bii:=0;
  Gii:=1;
  Rii:=2;
  for nyy:=yres-1 downto 0 do
  for nxx:=0 to xres-1 do
  begin
    fR:=bmprgb[Rii];
    fG:=bmprgb[Gii];
    fB:=bmprgb[Bii];
    FreeTex[nyy,nxx]:=rgb2color(fR,fG,fB);
    inc(Rii,3);
    inc(Gii,3);
    inc(Bii,3);
  end;

  close(fff);
  FileMode := 2;  {Return the default value - READ/WRITE}
end;

Function Average_Color(a, b: Dword): Dword; assembler;
asm
   mov EAX, [a]
   mov EBX, [b]
   mov EDX, EAX

   xor EAX, EBX
   and EAX, 0FFFEFEFEh
   shr EAX, 1

   and EBX, EDX
   add EAX, EBX
   mov [Result], EAX
end;

{Scale the main texture to diferent MipMap levels.
ScrInd and DestInd are indexes of textures}

Procedure GenerateMipMaps(SrcInd,DestInd:Dword);
Var
    rr1,gg1,bb1,rr2,gg2,bb2,rr3,gg3,bb3,rr4,gg4,bb4,rr5,gg5,bb5:byte;
    rrt,ggt,bbt,texboja,sumR,sumG,sumB,flx,fly,flblock:Dword;
begin
  flblock := 1;
  {Scale the main 256x256 texture to 128x128}
  ii:=0;
  for i:=0 to 63 do
  begin
    jj:=0;
    for j:=0 to 63 do
    begin
      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := ii - flblock to ii + flblock do
             for fly := jj - flblock to jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex1[DestInd,i,j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex1[DestInd,i,j]:=texbuf[SrcInd, ii, jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := ii - flblock to ii + flblock do
             for fly := 255-jj - flblock to 255-jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex1[DestInd,i,127-j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex1[DestInd,i,127-j]:=texbuf[SrcInd, ii, 255-jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := 255-ii - flblock to 255-ii + flblock do
             for fly := jj - flblock to jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex1[DestInd,127-i,j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex1[DestInd,127-i,j]:=texbuf[SrcInd, 255-ii, jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := 255-ii - flblock to 255-ii + flblock do
             for fly := 255-jj - flblock to 255-jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex1[DestInd,127-i,127-j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex1[DestInd,127-i,127-j]:=texbuf[SrcInd, 255-ii, 255-jj];

      jj:=jj+2;
    end;
    ii:=ii+2;
  end;
  flblock := 3;
  {64x64}
  ii:=0;
  for i:=0 to 31 do
  begin
    jj:=0;
    for j:=0 to 31 do
    begin
      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := ii - flblock to ii + flblock do
             for fly := jj - flblock to jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex2[DestInd,i,j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex2[DestInd,i,j]:=texbuf[SrcInd, ii, jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := ii - flblock to ii + flblock do
             for fly := 255-jj - flblock to 255-jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex2[DestInd,i,63-j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex2[DestInd,i,63-j]:=texbuf[SrcInd, ii, 255-jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := 255-ii - flblock to 255-ii + flblock do
             for fly := jj - flblock to jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex2[DestInd,63-i,j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex2[DestInd,63-i,j]:=texbuf[SrcInd, 255-ii, jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := 255-ii - flblock to 255-ii + flblock do
             for fly := 255-jj - flblock to 255-jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex2[DestInd,63-i,63-j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex2[DestInd,63-i,63-j]:=texbuf[SrcInd, 255-ii, 255-jj];

      jj:=jj+4;
    end;
    ii:=ii+4;
  end;
  flblock := 7;
  {32x32}
  ii:=0;
  for i:=0 to 15 do
  begin
    jj:=0;
    for j:=0 to 15 do
    begin
      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := ii - flblock to ii + flblock do
             for fly := jj - flblock to jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex3[DestInd,i,j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex3[DestInd,i,j]:=texbuf[SrcInd, ii, jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := ii - flblock to ii + flblock do
             for fly := 255-jj - flblock to 255-jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex3[DestInd,i,31-j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex3[DestInd,i,31-j]:=texbuf[SrcInd, ii, 255-jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := 255-ii - flblock to 255-ii + flblock do
             for fly := jj - flblock to jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex3[DestInd,31-i,j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex3[DestInd,31-i,j]:=texbuf[SrcInd, 255-ii, jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := 255-ii - flblock to 255-ii + flblock do
             for fly := 255-jj - flblock to 255-jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex3[DestInd,31-i,31-j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex3[DestInd,31-i,31-j]:=texbuf[SrcInd, 255-ii, 255-jj];

      jj:=jj+8;
    end;
    ii:=ii+8;
  end;
  flblock := 15;
  {16x16}
  ii:=0;
  for i:=0 to 7 do
  begin
    jj:=0;
    for j:=0 to 7 do
    begin
      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := ii - flblock to ii + flblock do
             for fly := jj - flblock to jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex4[DestInd,i,j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex4[DestInd,i,j]:=texbuf[SrcInd, ii, jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := ii - flblock to ii + flblock do
             for fly := 255-jj - flblock to 255-jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex4[DestInd,i,15-j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex4[DestInd,i,15-j]:=texbuf[SrcInd, ii, 255-jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := 255-ii - flblock to 255-ii + flblock do
             for fly := jj - flblock to jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex4[DestInd,15-i,j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex4[DestInd,15-i,j]:=texbuf[SrcInd, 255-ii, jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := 255-ii - flblock to 255-ii + flblock do
             for fly := 255-jj - flblock to 255-jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex4[DestInd,15-i,15-j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex4[DestInd,15-i,15-j]:=texbuf[SrcInd, 255-ii, 255-jj];

      jj:=jj+16;
    end;
    ii:=ii+16;
  end;
  flblock := 31;
  {8x8}
  ii:=0;
  for i:=0 to 3 do
  begin
    jj:=0;
    for j:=0 to 3 do
    begin
      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := ii - flblock to ii + flblock do
             for fly := jj - flblock to jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex5[DestInd,i,j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex5[DestInd,i,j]:=texbuf[SrcInd, ii, jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := ii - flblock to ii + flblock do
             for fly := 255-jj - flblock to 255-jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex5[DestInd,i,7-j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex5[DestInd,i,7-j]:=texbuf[SrcInd, ii, 255-jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := 255-ii - flblock to 255-ii + flblock do
             for fly := jj - flblock to jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex5[DestInd,7-i,j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex5[DestInd,7-i,j]:=texbuf[SrcInd, 255-ii, jj];

      if (ii>0) and (jj>0) then
        begin
          sumR := 0;
          sumG := 0;
          sumB := 0;
          for flx := 255-ii - flblock to 255-ii + flblock do
             for fly := 255-jj - flblock to 255-jj + flblock do
                begin
                  texboja := texbuf[SrcInd, flx, fly];
                  Color2RGB(texboja, rrt, ggt, bbt);
                  sumR +:= rrt;
                  sumG +:= ggt;
                  sumB +:= bbt;
                end;
          rrt := sumR div sqr(2*flblock+1);
          ggt := sumG div sqr(2*flblock+1);
          bbt := sumB div sqr(2*flblock+1);
          miptex5[DestInd,7-i,7-j]:=rgb2color(rrt, ggt, bbt);
        end
      else
        miptex5[DestInd,7-i,7-j]:=texbuf[SrcInd, 255-ii, 255-jj];

      jj:=jj+32;
    end;
    ii:=ii+32;
  end;
end;

{PROCEDURE to determine the apropriate MipMap texture and size according
 to the mipp parameter. ip is the current texture number}

Procedure MipMapDetermine(mipp:single; ip:Integer);
Var
    mipscale1,mipscale2:single;
begin
  {8x8}
  mipscale2:=8*MipBias;
  if mipp<mipscale2 then
    begin
      MipMapOffset:=ofs(miptex5);
      MipMapFactor:=8;
      MipMapResF:=3;
      TexXres[ip]:=7;
      TexYres[ip]:=7;
      Exit;
    end;
  {16x16}
  mipscale1:=8*MipBias;
  mipscale2:=16*MipBias;
  if (mipp>mipscale1) and (mipp<mipscale2) then
    begin
      MipMapOffset:=ofs(miptex4);
      MipMapFactor:=10;
      MipMapResF:=4;
      TexXres[ip]:=15;
      TexYres[ip]:=15;
      Exit;
    end;
  {32x32}
  mipscale1:=16*MipBias;
  mipscale2:=32*MipBias;
  if (mipp>mipscale1) and (mipp<mipscale2) then
    begin
      MipMapOffset:=ofs(miptex3);
      MipMapFactor:=12;
      MipMapResF:=5;
      TexXres[ip]:=31;
      TexYres[ip]:=31;
      Exit;
    end;
  {64x64}
  mipscale1:=32*MipBias;
  mipscale2:=64*MipBias;
  if (mipp>mipscale1) and (mipp<mipscale2) then
    begin
      MipMapOffset:=ofs(miptex2);
      MipMapFactor:=14;
      MipMapResF:=6;
      TexXres[ip]:=63;
      TexYres[ip]:=63;
      Exit;
    end;
  {128x128}
  mipscale1:=64*MipBias;
  mipscale2:=128*MipBias;
  if (mipp>mipscale1) and (mipp<mipscale2) then
    begin
      MipMapOffset:=ofs(miptex1);
      MipMapFactor:=16;
      MipMapResF:=7;
      TexXres[ip]:=127;
      TexYres[ip]:=127;
      Exit;
    end;
  {256x256}
  mipscale1:=128*MipBias;
  if mipp>mipscale1 then
    begin
      MipMapOffset:=ofs(texbuf);
      MipMapFactor:=18;
      MipMapResF:=8;
      TexXres[ip]:=255;
      TexYres[ip]:=255;
      Exit;
    end;
end;

{Generate MipMaps and Bilinear Filter them}
Procedure SetupMipMaps;
Var
    texind:Dword;
begin
  {Filter the Main 256x256 texture}
  Inc(LoadingText_Counter);
  WindowText('Filtering Detail Textures...', 10, LoadingText_Counter * 16, 255, 255, 0);
  Write2Log('Filtering Detail Textures...  ', false);

  for texind:=1 to MaterialCount + 1 do
     begin
       Source:=ofs(texbuf);
       Filter(0,0,255,255,texind,18,8,Source);
       Filter(0,0,255,255,texind,18,8,Source);
       Filter(0,0,255,255,texind,18,8,Source);
     end;

  WindowText('OK', 200, LoadingText_Counter * 16, 0, 255, 0);
  Write2Log('OK', true);

  {Generate MipMaps}
  Inc(LoadingText_Counter);
  WindowText('Generating MipMap Textures...', 10, LoadingText_Counter * 16, 255, 255, 0);
  Write2Log('Generating MipMap Textures...  ', false);

  for texind:=1 to MaterialCount + 1 do
     GenerateMipMaps(texind,texind);

  WindowText('OK', 232, LoadingText_Counter * 16, 0, 255, 0);
  Write2Log('OK', true);

  {Filter the MipMaps}
  Inc(LoadingText_Counter);
  WindowText('Filtering MipMap Textures...', 10, LoadingText_Counter * 16, 255, 255, 0);
  Write2Log('Filtering MipMap Textures...  ', false);

  for texind:=1 to MaterialCount + 1 do
     begin
       Dest:=ofs(miptex1);
       Filter(0,0,127,127,texind,16,7,Dest);
       Dest:=ofs(miptex2);
       Filter(0,0,63,63,texind,14,6,Dest);
       Dest:=ofs(miptex3);
       Filter(0,0,31,31,texind,12,5,Dest);
       Dest:=ofs(miptex4);
       Filter(0,0,15,15,texind,10,4,Dest);
       Dest:=ofs(miptex5);
       Filter(0,0,7,7,texind,8,3,Dest);
     end;

  WindowText('OK', 214, LoadingText_Counter * 16, 0, 255, 0);
  Write2Log('OK', true);
end;

Procedure Generate_NoTex;
begin
  for ii := 0 to 100 do
     for jj := 0 to 100 do
        texbuf[MaterialCount + 1, ii, jj] := rgb2color(127, 127, 127);
  for ii := 0 to 100 do
     for jj := 0 to 100 do
        texbuf[MaterialCount + 1, 255 - ii, jj] := rgb2color(127, 127, 127);
  for ii := 0 to 100 do
     for jj := 0 to 100 do
        texbuf[MaterialCount + 1, ii, 255 - jj] := rgb2color(127, 127, 127);
  for ii := 0 to 100 do
     for jj := 0 to 100 do
        texbuf[MaterialCount + 1, 255 - ii, 255 - jj] := rgb2color(127, 127, 127);
end;

{Loads and sets up all the textures used in the world}
Procedure LoadTextures;
var texboja,rrt,ggt,bbt,sumR,sumG,sumB:Dword;
begin
  {Load all the world textures except the lens flare texture}
  Inc(LoadingText_Counter);
  WindowText('Loading Textures...', 10, LoadingText_Counter * 16, 255, 255, 0);
  Write2Log('Loading Textures...  ', false);

  Generate_NoTex;

  for pa := 1 to MaterialCount + 1 do
     begin
       if pa <= MaterialCount then
         LoadBMP(texture_filename[pa],pa);

       sumR := 0;
       sumG := 0;
       sumB := 0;
       for ii := 0 to 255 do
          for jj := 0 to 255 do
             begin
               texboja := texbuf[pa, ii, jj];
               Color2RGB(texboja, rrt, ggt, bbt);
               sumR +:= rrt;
               sumG +:= ggt;
               sumB +:= bbt;
             end;
       SrBoja_R[pa] := sumR shr 16;
       SrBoja_G[pa] := sumG shr 16;
       SrBoja_B[pa] := sumB shr 16;
     end;

  {Load only the lens flare texture}
  LoadBMPFree(lightlens,texlight);

  WindowText('OK', 152, LoadingText_Counter * 16, 0, 255, 0);
  Write2Log('OK', true);

  {Generate and Setup the MipMaps}
  SetupMipMaps;
end;

END.