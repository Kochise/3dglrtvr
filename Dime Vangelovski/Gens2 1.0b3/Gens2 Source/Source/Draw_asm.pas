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

{Unit for the Texture Mapping Functions (Per-Vertex Lighting) in pure ASSEMBLER code -

                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit Draw_asm;

Interface

Procedure Scan_Edge(x1, y1, x2, y2, cc1R, cc1G, cc1B, cc2R, cc2G, cc2B, zz1, zz2, xtx1, ytx1, xtx2, ytx2: Integer);
Procedure Textured_Polygon_Alpha(y1, y2, ip_main:Integer; transi:byte; where:Pointer);
Procedure Textured_Polygon(y1, y2, ip_main: Integer; where: Pointer);
Procedure Textured_Polygon_Old_Renderer(y1, y2, ip_main:Integer; where:Pointer);

Implementation

Uses
     Globals;

{This is the SCANEDGE PROCEDURE to scan the edges of a polygon to get
 the values for the horizontal line rendering.
 x1,y1,x2,y2 are the current polygon edge coordinates,
 cc1R,... cc2R,... are the start and end color RGB values on the EDGE,
 zz1,zz2 are Z coordinates on start and end,
 xtx1,ytx1,xtx2,ytx2, are the start and end texture coordinates (u,v)}

Procedure Scan_Edge(x1, y1, x2, y2, cc1R, cc1G, cc1B, cc2R, cc2G, cc2B,
                    zz1, zz2, xtx1, ytx1, xtx2, ytx2: Integer);
ASSEMBLER;
var
    x,xc,zz,zzz,ccR,cccR,ccG,cccG,ccB,cccB,xx,yy,xclip,yclip,y,tmp,xkor,zkor,ygr: Integer;
asm
       mov EAX, [y1]
       mov EDI, [y2]
       cmp EAX, EDI
       jle @nexchg

       mov [y2], EAX
       mov [y1], EDI

       mov EAX, [x1]
       mov EDI, [x2]
       mov [x2], EAX
       mov [x1], EDI

       mov EAX, [zz1]
       mov EDI, [zz2]
       mov [zz2], EAX
       mov [zz1], EDI

       mov EAX, [cc1R]
       mov EDI, [cc2R]
       mov [cc2R], EAX
       mov [cc1R], EDI

       mov EAX, [cc1G]
       mov EDI, [cc2G]
       mov [cc2G], EAX
       mov [cc1G], EDI

       mov EAX, [cc1B]
       mov EDI, [cc2B]
       mov [cc2B], EAX
       mov [cc1B], EDI

       mov EAX, [xtx1]
       mov EDI, [xtx2]
       mov [xtx2], EAX
       mov [xtx1], EDI

       mov EAX, [ytx1]
       mov EDI, [ytx2]
       mov [ytx2], EAX
       mov [ytx1], EDI
@nexchg:

       mov EDI, [x2]
       sub EDI, [x1]
       shl EDI, 16
       mov [xc], EDI

       mov EDI, [cc2R]
       sub EDI, [cc1R]
       shl EDI, 16
       mov [cccR], EDI

       mov EDI, [cc2G]
       sub EDI, [cc1G]
       shl EDI, 16
       mov [cccG], EDI

       mov EDI, [cc2B]
       sub EDI, [cc1B]
       shl EDI, 16
       mov [cccB], EDI

       mov EDI, [zz2]
       sub EDI, [zz1]
       shl EDI, 16
       mov [zzz], EDI

       mov EDI, [xtx2]
       sub EDI, [xtx1]
       shl EDI, 16
       mov [xclip], EDI

       mov EDI, [ytx2]
       sub EDI, [ytx1]
       shl EDI, 16
       mov [yclip], EDI

       mov EDI, [y2]
       sub EDI, [y1]
       cmp EDI, 0
       jle @zerokor

       mov EAX, [xc]
       cdq
       idiv EDI
       mov [xc], EAX

       mov EAX, [cccR]
       cdq
       idiv EDI
       mov [cccR], EAX

       mov EAX, [cccG]
       cdq
       idiv EDI
       mov [cccG], EAX

       mov EAX, [cccB]
       cdq
       idiv EDI
       mov [cccB], EAX

       mov EAX, [xclip]
       cdq
       idiv EDI
       mov [xclip], EAX

       mov EAX, [yclip]
       cdq
       idiv EDI
       mov [yclip], EAX

       mov EAX, [zzz]
       cdq
       idiv EDI
       mov [zzz], EAX
@zerokor:

       mov EAX, [x1]
       shl EAX, 16
       mov [x], EAX

       mov EAX, [cc1R]
       shl EAX, 16
       mov [ccR], EAX

       mov EAX, [cc1G]
       shl EAX, 16
       mov [ccG], EAX

       mov EAX, [cc1B]
       shl EAX, 16
       mov [ccB], EAX

       mov EAX, [zz1]
       shl EAX, 16
       mov [zz], EAX

       mov EAX, [xtx1]
       shl EAX, 16
       mov [xx], EAX

       mov EAX, [ytx1]
       shl EAX, 16
       mov [yy], EAX

    mov EDX, [xc]
    mov ESI, [zzz]
    mov ECX, [y1]

  {Repeat for the whole vertical length of the polygon}
@repeat:
    cmp ECX, 0
    jl  @skip
    cmp ECX, [Res_Yend_p1]
    jge @skip

    mov EBX, [x]
    sar EBX, 16                       // EBX = xkor

    mov EAX, [zz]
    shr EAX, 16
    mov [zkor], EAX

    lea EDI, poly[ECX * 8]
    mov EAX, [EDI]                    // EAX = poly[y, 1]

    cmp EBX, EAX
    jge @next

    lea EDI, poly[ECX * 8]
    mov [EDI], EBX                     // poly[y, 1] = xkor

    lea EDI, colR[ECX * 8]
    mov EAX, [ccR]
    shr EAX, 16
    mov [EDI], EAX                     // colR[y, 1] = ccR shr 16

    lea EDI, colG[ECX * 8]
    mov EAX, [ccG]
    shr EAX, 16
    mov [EDI], EAX                     // colG[y, 1] = ccG shr 16

    lea EDI, colB[ECX * 8]
    mov EAX, [ccB]
    shr EAX, 16
    mov [EDI], EAX                     // colB[y, 1] = ccB shr 16

    lea EAX, [ECX * 4]
    lea EDI, tex[EAX * 4]
    mov EAX, [xx]
    shr EAX, 16
    mov [EDI], EAX                     // tex[y, 1] = xx shr 16

    lea EAX, [ECX * 4 + 1]
    lea EDI, tex[EAX * 4]
    mov EAX, [yy]
    shr EAX, 16
    mov [EDI], EAX                     // tex[y, 2] = yy shr 16

    lea EDI, ztmp[ECX * 8]
    mov EAX, [zkor]
    mov [EDI], EAX                     // ztmp[y, 1] = zkor

@next:
    lea EDI, poly[ECX * 8 + 4]
    mov EAX, [EDI]                    // EAX = poly[y, 2]

    cmp EBX, EAX
    jle @skip

    lea EDI, poly[ECX * 8 + 4]
    mov [EDI], EBX                     // poly[y, 2] = xkor

    lea EDI, colR[ECX * 8 + 4]
    mov EAX, [ccR]
    shr EAX, 16
    mov [EDI], EAX                     // colR[y, 2] = ccR shr 16

    lea EDI, colG[ECX * 8 + 4]
    mov EAX, [ccG]
    shr EAX, 16
    mov [EDI], EAX                     // colG[y, 2] = ccG shr 16

    lea EDI, colB[ECX * 8 + 4]
    mov EAX, [ccB]
    shr EAX, 16
    mov [EDI], EAX                     // colB[y, 2] = ccB shr 16

    lea EAX, [ECX * 4 + 2]
    lea EDI, tex[EAX * 4]
    mov EAX, [xx]
    shr EAX, 16
    mov [EDI], EAX                     // tex[y, 3] = xx shr 16

    lea EAX, [ECX * 4 + 3]
    lea EDI, tex[EAX * 4]
    mov EAX, [yy]
    shr EAX, 16
    mov [EDI], EAX                     // tex[y, 4] = yy shr 16

    lea EDI, ztmp[ECX * 8 + 4]
    mov EAX, [zkor]
    mov [EDI], EAX                     // ztmp[y, 2] = zkor

@skip:
    {increase the appropriate values for the next pixel in the polygon edge}
    add [x], EDX
    add [zz], ESI
    mov EAX, [cccR]
    mov EDI, [cccG]
    add [ccR], EAX
    add [ccG], EDI
    mov EAX, [cccB]
    mov EDI, [xclip]
    add [ccB], EAX
    add [xx], EDI
    mov EAX, [yclip]
    inc ECX
    add [yy], EAX

    cmp ECX, [y2]
    jle @repeat
end;


{This procedure renders a transparent polygon from y1 to y2 using horizontal scanlines
 ip_main is the current texture index applied to the polygon. transi is the alpha value
 0 - non transparent, 255 - completely transparent. where is the address in mem}

Procedure Textured_Polygon_Alpha(y1, y2, ip_main:Integer; transi:byte; where:Pointer);
ASSEMBLER;
var
    x1,x2,c1R,c1G,c1B,c2R,c2G,c2B,texx1,texy1,texx2,texy2,z1,z2,ip,xxx,yyy,tmp,textx,texty,
    dxx,dyy,korx,kory,kor,ccol,z,zz,cR,cG,cB,ccR,ccG,ccB,proz,tmpR,tmpG,tmpB:Integer;
    yff,fst,aaa,memm,xftmp,yftmp,fad,alm,tmpadr,r1,r2,g1,g2,b1,b2,
    rfin,gfin,bfin,transadj,reso,rrt,ggt,bbt:Dword;
asm
       mov EAX, [y1]
       mov [yyy], EAX

@Repeat_Hline:
       mov EBX, [yyy]
       mov EAX, [ip_main]
       mov [ip], EAX

       lea EDI, poly[EBX * 8]
       mov EAX, [EDI]
       mov [x1], EAX                     // x1 = poly[yyy, 1]

       lea EDI, poly[EBX * 8 + 4]
       mov EDX, [EDI]
       mov [x2], EDX                     // x2 = poly[yyy, 2]

       cmp EAX, 0
       jl  @andx2
       jmp @continue
@andx2:
       cmp EDX, 0
       jl  @Exit
@continue:

       cmp EAX, [Res_Xend]
       jg  @aandx2
       jmp @cont
@aandx2:
       cmp EDX, [Res_Xend]
       jg  @Exit

@cont:
       mov EAX, EBX
       cmp EAX, 0
       jl  @Exit
       cmp EAX, [Res_Yend]
       jg  @Exit

       mov EDI, EAX

       push ECX
       mov Cl, [Res_Shl1]
       shl EAX, Cl
       and EDI, [Res_Mask]
       mov Cl, [Res_Shl2]
       shl EDI, Cl
       pop ECX

       add EAX, EDI
       mov [yff], EAX

       lea EDI, ztmp[EBX * 8]
       mov EAX, [EDI]
       mov [z1], EAX                     // z1 = ztmp[yyy, 1]

       lea EDI, ztmp[EBX * 8 + 4]
       mov EAX, [EDI]
       mov [z2], EAX                     // z2 = ztmp[yyy, 2]

       lea EDI, colR[EBX * 8]
       mov EAX, [EDI]
       mov [c1R], EAX                     // c1R = colR[yyy, 1]

       lea EDI, colG[EBX * 8]
       mov EAX, [EDI]
       mov [c1G], EAX                     // c1G = colG[yyy, 1]

       lea EDI, colB[EBX * 8]
       mov EAX, [EDI]
       mov [c1B], EAX                     // c1B = colB[yyy, 1]

       lea EDI, colR[EBX * 8 + 4]
       mov EAX, [EDI]
       mov [c2R], EAX                     // c2R = colR[yyy, 2]

       lea EDI, colG[EBX * 8 + 4]
       mov EAX, [EDI]
       mov [c2G], EAX                     // c2G = colG[yyy, 2]

       lea EDI, colB[EBX * 8 + 4]
       mov EAX, [EDI]
       mov [c2B], EAX                     // c2B = colB[yyy, 2]

       lea EAX, [EBX * 4]
       lea EDI, tex[EAX * 4]
       mov EAX, [EDI]
       mov [texx1], EAX                   // texx1 = tex[yyy, 1]

       lea EAX, [EBX * 4 + 1]
       lea EDI, tex[EAX * 4]
       mov EAX, [EDI]
       mov [texy1], EAX                   // texy1 = tex[yyy, 2]

       lea EAX, [EBX * 4 + 2]
       lea EDI, tex[EAX * 4]
       mov EAX, [EDI]
       mov [texx2], EAX                   // texx2 = tex[yyy, 3]

       lea EAX, [EBX * 4 + 3]
       lea EDI, tex[EAX * 4]
       mov EAX, [EDI]
       mov [texy2], EAX                   // texy2 = tex[yyy, 4]


       mov EAX, [x1]
       mov EDI, [x2]
       cmp EAX, EDI
       jle @nexchg

       mov [x2], EAX
       mov [x1], EDI

       mov EAX, [z1]
       mov EDI, [z2]
       mov [z2], EAX
       mov [z1], EDI

       mov EAX, [c1R]
       mov EDI, [c2R]
       mov [c2R], EAX
       mov [c1R], EDI

       mov EAX, [c1G]
       mov EDI, [c2G]
       mov [c2G], EAX
       mov [c1G], EDI

       mov EAX, [c1B]
       mov EDI, [c2B]
       mov [c2B], EAX
       mov [c1B], EDI

       mov EAX, [texx1]
       mov EDI, [texx2]
       mov [texx2], EAX
       mov [texx1], EDI

       mov EAX, [texy1]
       mov EDI, [texy2]
       mov [texy2], EAX
       mov [texy1], EDI
@nexchg:

       mov EDI, [c2R]
       sub EDI, [c1R]
       shl EDI, 16
       mov [ccR], EDI

       mov EDI, [c2G]
       sub EDI, [c1G]
       shl EDI, 16
       mov [ccG], EDI

       mov EDI, [c2B]
       sub EDI, [c1B]
       shl EDI, 16
       mov [ccB], EDI

       mov EDI, [z2]
       sub EDI, [z1]
       shl EDI, 16
       mov [zz], EDI

       mov EDI, [texx2]
       sub EDI, [texx1]
       shl EDI, 16
       mov [korx], EDI

       mov EDI, [texy2]
       sub EDI, [texy1]
       shl EDI, 16
       mov [kory], EDI

       mov EDI, [x2]
       sub EDI, [x1]
       cmp EDI, 0
       jle @zerokor

       mov EAX, [ccR]
       cdq
       idiv EDI
       mov [ccR], EAX

       mov EAX, [ccG]
       cdq
       idiv EDI
       mov [ccG], EAX

       mov EAX, [ccB]
       cdq
       idiv EDI
       mov [ccB], EAX

       mov EAX, [korx]
       cdq
       idiv EDI
       mov [dxx], EAX

       mov EAX, [kory]
       cdq
       idiv EDI
       mov [dyy], EAX

       mov EAX, [zz]
       cdq
       idiv EDI
       mov [zz], EAX
@zerokor:

       mov EAX, [c1R]
       mov EBX, [c1G]
       shl EAX, 16
       mov EDX, [c1B]
       shl EBX, 16
       mov [cR], EAX
       shl EDX, 16
       mov [cG], EBX
       mov [cB], EDX

       mov EAX, [texx1]
       mov EBX, [texy1]
       shl EAX, 16
       mov EDX, [z1]
       shl EBX, 16
       mov [textx], EAX
       shl EDX, 16
       mov [texty], EBX
       mov [z], EDX

       xor EAX, EAX
       mov Al, 255
       sub Al, [transi]
       mov [transadj], EAX

{***************** Prozdiren poligon ***********************************************************}
   {Initialize the variables}
   mov ESI, [ip]
   dec ESI
   mov Cl, [MipMapFactor]
   shl ESI, Cl
   mov [ip], ESI
   mov ECX, [x1]
   mov EAX, [x2]
   dec EAX
   mov [x2], EAX
   mov EDX, [z]
   mov EBX, [cR]

@rep_prodziren:
   {check if the current pixel on the line is out of screen => skip}
   cmp ECX, 0                     -- U
   jl  @next_prodziren            -- V          - 1
   cmp ECX, [Res_Xend]            -- U
   jg  @next_prodziren            -- V          - 1

   mov ESI, [yff]                 -- U
   add ESI, ECX                   -- V          - 1

   {check the z-buffer. if this z<zbuf draw else skip}
   lea EDI, zbuf[ESI*2]           -- U
   mov EAX, EDX                   -- V          - 1
   shr EAX, 16                    -- U          - 1
   cmp AX,[EDI]                   -- U
   jge @next_prodziren            -- V          - 5

   {write the new Z value in the z-buffer}
//   mov [EDI], AX                  -- U          - 1

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
   mov [r1], EAX
   shl EAX, 16
   sub EDI, EAX
   mov ESI, EDI
   shr EDI, 8
   mov [ggt], EDI
   mov [g1], EDI
   shl EDI, 8
   sub ESI, EDI
   mov [b1], ESI

   {calculate the RGB light from the 16.16 fixed point}
   mov EDI, EBX                   -- U
   shr EDI, 16                    -- U
   mov EAX, [cG]
   shr EAX, 16
   mov [tmpG], EAX
   mov EAX, [cB]
   shr EAX, 16
   mov [tmpB], EAX

   {add the green light to the texel and clamp the value}
   push EDX
   mov EAX, [tmpG]
   mul [ggt]
   shrd EAX, EDX, 8
   mul [transadj]
   shrd EAX, EDX, 8
   cmp EAX, 255
   jle @gg_prodziren
   mov EAX, 255
@gg_prodziren:
   mov [g1], EAX

   {add the blue light to the texel and clamp the value}
   mov EAX, [tmpB]
   mul ESI
   shrd EAX, EDX, 8
   mul [transadj]
   shrd EAX, EDX, 8
   cmp EAX, 255
   jle @bb_prodziren
   mov EAX, 255
@bb_prodziren:
   mov [b1], EAX

   {add the red light to the texel and clamp the value}
   mov EAX, EDI
   mul [rrt]
   shrd EAX, EDX, 8
   mul [transadj]
   shrd EAX, EDX, 8
   cmp EAX, 255
   jle @rr_prodziren
   mov EAX, 255
@rr_prodziren:
   mov [r1], EAX
   pop EDX

   {read the background pixel - from screen}
   mov EDI, [yff]
   add EDI, ECX
   mov ESI, [where]
   mov EAX, [ESI+EDI*4]

   push EBX
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
   pop EBX

   {convert from RGB back to color}
   mov EAX, [rfin]
   mov EDI, [gfin]
   shl EAX, 16
   shl EDI, 8
   add EAX, EDI
   add EAX, [bfin]

   {draw the curent pixel}
   mov EDI, [yff]
   add EDI, ECX
   mov ESI, [where]
   mov [ESI+EDI*4], EAX           -- U          - 1

@next_prodziren:
   {increase the appropriate values by their factors for the next pixel}
   add EDX, [zz]                  -- U
   add EBX, [ccR]                 -- V          - 2
   mov EAX, [ccG]
   mov EDI, [ccB]
   add [cG], EAX
   add [cB], EDI

   mov EAX, [dxx]                 -- U
   mov EDI, [dyy]                  -- V          - 1
   add [textX], EAX               -- U
   add [textY], EDI               -- V          - 3

   inc ECX                        -- U
   mov EAX, [x2]                  -- V          - 1
   cmp ECX, EAX                   -- V          - 1
   jle @rep_prodziren             -- U          - 4    = 39 clocks per texel = 308 ns

@Exit:
   inc [yyy]
   mov EAX, [yyy]
   cmp EAX, [y2]
   jle @Repeat_Hline
end;

{This procedure renders a polygon from y1 to y2 using horizontal scanlines.
 ip_main is the current texture index applied to the polygon. where is the address in mem}

Procedure Textured_Polygon(y1, y2, ip_main: Integer; where: Pointer);
ASSEMBLER;
var
    x1,x2,c1R,c1G,c1B,c2R,c2G,c2B,texx1,texy1,texx2,texy2,z1,z2,ip,xxx,yyy,tmp,textx,texty,
    dxx,dyy,korx,kory,kor,ccol,z,zz,cR,cG,cB,ccR,ccG,ccB,proz,tmpR,tmpG,tmpB: Integer;
    yff,fst,aaa,memm,xftmp,yftmp,fad,alm,tmpadr,r1,r2,g1,g2,b1,b2,
    rfin,gfin,bfin,transadj,reso,rrt,ggt,bbt: Dword;
asm
       mov EAX, [y1]
       mov [yyy], EAX

@Repeat_Hline:
       mov EBX, [yyy]
       mov EAX, [ip_main]
       mov [ip], EAX

       lea EDI, poly[EBX * 8]
       mov EAX, [EDI]
       mov [x1], EAX                     // x1 = poly[yyy, 1]

       lea EDI, poly[EBX * 8 + 4]
       mov EDX, [EDI]
       mov [x2], EDX                     // x2 = poly[yyy, 2]

       cmp EAX, 0
       jl  @andx2
       jmp @continue
@andx2:
       cmp EDX, 0
       jl  @Exit
@continue:

       cmp EAX, [Res_Xend]
       jg  @aandx2
       jmp @cont
@aandx2:
       cmp EDX, [Res_Xend]
       jg  @Exit

@cont:
       mov EAX, EBX
       cmp EAX, 0
       jl  @Exit
       cmp EAX, [Res_Yend]
       jg  @Exit

       mov EDI, EAX

       push ECX
       mov Cl, [Res_Shl1]
       shl EAX, Cl
       and EDI, [Res_Mask]
       mov Cl, [Res_Shl2]
       shl EDI, Cl
       pop ECX

       add EAX, EDI
       mov [yff], EAX

       lea EDI, ztmp[EBX * 8]
       mov EAX, [EDI]
       mov [z1], EAX                     // z1 = ztmp[yyy, 1]

       lea EDI, ztmp[EBX * 8 + 4]
       mov EAX, [EDI]
       mov [z2], EAX                     // z2 = ztmp[yyy, 2]

       lea EDI, colR[EBX * 8]
       mov EAX, [EDI]
       mov [c1R], EAX                     // c1R = colR[yyy, 1]

       lea EDI, colG[EBX * 8]
       mov EAX, [EDI]
       mov [c1G], EAX                     // c1G = colG[yyy, 1]

       lea EDI, colB[EBX * 8]
       mov EAX, [EDI]
       mov [c1B], EAX                     // c1B = colB[yyy, 1]

       lea EDI, colR[EBX * 8 + 4]
       mov EAX, [EDI]
       mov [c2R], EAX                     // c2R = colR[yyy, 2]

       lea EDI, colG[EBX * 8 + 4]
       mov EAX, [EDI]
       mov [c2G], EAX                     // c2G = colG[yyy, 2]

       lea EDI, colB[EBX * 8 + 4]
       mov EAX, [EDI]
       mov [c2B], EAX                     // c2B = colB[yyy, 2]

       lea EAX, [EBX * 4]
       lea EDI, tex[EAX * 4]
       mov EAX, [EDI]
       mov [texx1], EAX                   // texx1 = tex[yyy, 1]

       lea EAX, [EBX * 4 + 1]
       lea EDI, tex[EAX * 4]
       mov EAX, [EDI]
       mov [texy1], EAX                   // texy1 = tex[yyy, 2]

       lea EAX, [EBX * 4 + 2]
       lea EDI, tex[EAX * 4]
       mov EAX, [EDI]
       mov [texx2], EAX                   // texx2 = tex[yyy, 3]

       lea EAX, [EBX * 4 + 3]
       lea EDI, tex[EAX * 4]
       mov EAX, [EDI]
       mov [texy2], EAX                   // texy2 = tex[yyy, 4]


       mov EAX, [x1]
       mov EDI, [x2]
       cmp EAX, EDI
       jle @nexchg

       mov [x2], EAX
       mov [x1], EDI

       mov EAX, [z1]
       mov EDI, [z2]
       mov [z2], EAX
       mov [z1], EDI

       mov EAX, [c1R]
       mov EDI, [c2R]
       mov [c2R], EAX
       mov [c1R], EDI

       mov EAX, [c1G]
       mov EDI, [c2G]
       mov [c2G], EAX
       mov [c1G], EDI

       mov EAX, [c1B]
       mov EDI, [c2B]
       mov [c2B], EAX
       mov [c1B], EDI

       mov EAX, [texx1]
       mov EDI, [texx2]
       mov [texx2], EAX
       mov [texx1], EDI

       mov EAX, [texy1]
       mov EDI, [texy2]
       mov [texy2], EAX
       mov [texy1], EDI
@nexchg:

       mov EDI, [c2R]
       sub EDI, [c1R]
       shl EDI, 16
       mov [ccR], EDI

       mov EDI, [c2G]
       sub EDI, [c1G]
       shl EDI, 16
       mov [ccG], EDI

       mov EDI, [c2B]
       sub EDI, [c1B]
       shl EDI, 16
       mov [ccB], EDI

       mov EDI, [z2]
       sub EDI, [z1]
       shl EDI, 16
       mov [zz], EDI

       mov EDI, [texx2]
       sub EDI, [texx1]
       shl EDI, 16
       mov [korx], EDI

       mov EDI, [texy2]
       sub EDI, [texy1]
       shl EDI, 16
       mov [kory], EDI

       mov EDI, [x2]
       sub EDI, [x1]
       cmp EDI, 0
       jle @zerokor

       mov EAX, [ccR]
       cdq
       idiv EDI
       mov [ccR], EAX

       mov EAX, [ccG]
       cdq
       idiv EDI
       mov [ccG], EAX

       mov EAX, [ccB]
       cdq
       idiv EDI
       mov [ccB], EAX

       mov EAX, [korx]
       cdq
       idiv EDI
       mov [dxx], EAX

       mov EAX, [kory]
       cdq
       idiv EDI
       mov [dyy], EAX

       mov EAX, [zz]
       cdq
       idiv EDI
       mov [zz], EAX
@zerokor:

       mov EAX, [c1R]
       mov EBX, [c1G]
       shl EAX, 16
       mov EDX, [c1B]
       shl EBX, 16
       mov [cR], EAX
       shl EDX, 16
       mov [cG], EBX
       mov [cB], EDX

       mov EAX, [texx1]
       mov EBX, [texy1]
       shl EAX, 16
       mov EDX, [z1]
       shl EBX, 16
       mov [textx], EAX
       shl EDX, 16
       mov [texty], EBX
       mov [z], EDX

{***************** Standarden poligon **********************************************************}
   {Initialize the variables}
   mov ESI, [ip]
   dec ESI
   mov Cl, [MipMapFactor]
   shl ESI, Cl
   mov [ip], ESI
   mov ECX, [x1]
   mov EAX, [x2]
   dec EAX
   mov [x2], EAX
   mov EDX, [z]
   mov EBX, [cR]

@rep:
   {check if the current pixel on the line is out of screen => skip}
   cmp ECX, 0                     -- U
   jl  @next                      -- V          - 1
   cmp ECX, [Res_Xend]            -- U
   jg  @next                      -- V          - 1

   mov ESI, [yff]                 -- U
   add ESI, ECX                   -- V          - 1

   {check the z-buffer. if this z < zbuf draw else skip}
   lea EDI, zbuf[ESI*2]           -- U
   mov EAX, EDX                   -- V          - 1
   shr EAX, 16                    -- U          - 1
   cmp AX, [EDI]                  -- U
   jge @next                      -- V          - 5

   {write the new Z value in the z-buffer}
   mov [EDI], AX                  -- U          - 1

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

   {add the green light to the texel and clamp the value}
   push EDX
   mov EAX, [cG]
   mul [ggt]
   shrd EAX, EDX, 24
   cmp EAX, 255
   jle @gg
   mov EAX, 255
@gg:
   mov [ggt], EAX

   {add the blue light to the texel and clamp the value}
   mov EAX, [cB]
   mul ESI
   shrd EAX, EDX, 24
   cmp EAX, 255
   jle @bb
   mov EAX, 255
@bb:
   mov ESI, EAX

   {add the red light to the texel and clamp the value}
   mov EAX, [rrt]
   mul EBX
   shrd EAX, EDX, 24
   cmp EAX, 255
   jle @rr
   mov EAX, 255
@rr:
   pop EDX

   {convert from RGB back to color}
   mov EDI, [ggt]
   shl EAX, 16
   shl EDI, 8
   add EAX, EDI
   add EAX, ESI

   {draw the curent pixel}
   mov ESI, [yff]
   add ESI, ECX
   mov EDI, [where]
   mov [EDI+ESI*4], EAX           -- U          - 1

@next:
   {increase the appropriate values by their factors for the next pixel}
   add EDX, [zz]                  -- U
   add EBX, [ccR]                 -- V          - 2
   mov EAX, [ccG]
   mov EDI, [ccB]
   add [cG], EAX
   add [cB], EDI

   mov EAX, [dxx]                 -- U
   mov EDI, [dyy]                  -- V          - 1
   add [textX], EAX               -- U
   add [textY], EDI               -- V          - 3

   inc ECX                        -- U
   mov EAX, [x2]                  -- V          - 1
   cmp ECX, EAX                   -- V          - 1
   jle @rep                       -- U          - 4    = 39 clocks per texel = 308 ns

@Exit:
   inc [yyy]
   mov EAX, [yyy]
   cmp EAX, [y2]
   jle @Repeat_Hline
end;


{This procedure renders a polygon from y1 to y2 using horizontal scanlines
 ip_main is the current texture index applied to the polygon. where is the address in mem
 This procedure uses the old (Gens1) lighting engine (simple light addition)}

Procedure Textured_Polygon_Old_Renderer(y1, y2, ip_main:Integer; where:Pointer);
ASSEMBLER;
var
    x1,x2,c1R,c1G,c1B,c2R,c2G,c2B,texx1,texy1,texx2,texy2,z1,z2,ip,xxx,yyy,tmp,textx,texty,
    dxx,dyy,korx,kory,kor,ccol,z,zz,cR,cG,cB,ccR,ccG,ccB,proz,tmpR,tmpG,tmpB:Integer;
    yff,fst,aaa,memm,xftmp,yftmp,fad,alm,tmpadr,r1,r2,g1,g2,b1,b2,
    rfin,gfin,bfin,transadj,reso,rrt,ggt,bbt:Dword;
asm
       mov EAX, [y1]
       mov [yyy], EAX

@Repeat_Hline:
       mov EBX, [yyy]
       mov EAX, [ip_main]
       mov [ip], EAX

       lea EDI, poly[EBX * 8]
       mov EAX, [EDI]
       mov [x1], EAX                     // x1 = poly[yyy, 1]

       lea EDI, poly[EBX * 8 + 4]
       mov EDX, [EDI]
       mov [x2], EDX                     // x2 = poly[yyy, 2]

       cmp EAX, 0
       jl  @andx2
       jmp @continue
@andx2:
       cmp EDX, 0
       jl  @Exit
@continue:

       cmp EAX, [Res_Xend]
       jg  @aandx2
       jmp @cont
@aandx2:
       cmp EDX, [Res_Xend]
       jg  @Exit

@cont:
       mov EAX, EBX
       cmp EAX, 0
       jl  @Exit
       cmp EAX, [Res_Yend]
       jg  @Exit

       mov EDI, EAX

       push ECX
       mov Cl, [Res_Shl1]
       shl EAX, Cl
       and EDI, [Res_Mask]
       mov Cl, [Res_Shl2]
       shl EDI, Cl
       pop ECX

       add EAX, EDI
       mov [yff], EAX

       lea EDI, ztmp[EBX * 8]
       mov EAX, [EDI]
       mov [z1], EAX                     // z1 = ztmp[yyy, 1]

       lea EDI, ztmp[EBX * 8 + 4]
       mov EAX, [EDI]
       mov [z2], EAX                     // z2 = ztmp[yyy, 2]

       lea EDI, colR[EBX * 8]
       mov EAX, [EDI]
       mov [c1R], EAX                     // c1R = colR[yyy, 1]

       lea EDI, colG[EBX * 8]
       mov EAX, [EDI]
       mov [c1G], EAX                     // c1G = colG[yyy, 1]

       lea EDI, colB[EBX * 8]
       mov EAX, [EDI]
       mov [c1B], EAX                     // c1B = colB[yyy, 1]

       lea EDI, colR[EBX * 8 + 4]
       mov EAX, [EDI]
       mov [c2R], EAX                     // c2R = colR[yyy, 2]

       lea EDI, colG[EBX * 8 + 4]
       mov EAX, [EDI]
       mov [c2G], EAX                     // c2G = colG[yyy, 2]

       lea EDI, colB[EBX * 8 + 4]
       mov EAX, [EDI]
       mov [c2B], EAX                     // c2B = colB[yyy, 2]

       lea EAX, [EBX * 4]
       lea EDI, tex[EAX * 4]
       mov EAX, [EDI]
       mov [texx1], EAX                   // texx1 = tex[yyy, 1]

       lea EAX, [EBX * 4 + 1]
       lea EDI, tex[EAX * 4]
       mov EAX, [EDI]
       mov [texy1], EAX                   // texy1 = tex[yyy, 2]

       lea EAX, [EBX * 4 + 2]
       lea EDI, tex[EAX * 4]
       mov EAX, [EDI]
       mov [texx2], EAX                   // texx2 = tex[yyy, 3]

       lea EAX, [EBX * 4 + 3]
       lea EDI, tex[EAX * 4]
       mov EAX, [EDI]
       mov [texy2], EAX                   // texy2 = tex[yyy, 4]


       mov EAX, [x1]
       mov EDI, [x2]
       cmp EAX, EDI
       jle @nexchg

       mov [x2], EAX
       mov [x1], EDI

       mov EAX, [z1]
       mov EDI, [z2]
       mov [z2], EAX
       mov [z1], EDI

       mov EAX, [c1R]
       mov EDI, [c2R]
       mov [c2R], EAX
       mov [c1R], EDI

       mov EAX, [c1G]
       mov EDI, [c2G]
       mov [c2G], EAX
       mov [c1G], EDI

       mov EAX, [c1B]
       mov EDI, [c2B]
       mov [c2B], EAX
       mov [c1B], EDI

       mov EAX, [texx1]
       mov EDI, [texx2]
       mov [texx2], EAX
       mov [texx1], EDI

       mov EAX, [texy1]
       mov EDI, [texy2]
       mov [texy2], EAX
       mov [texy1], EDI
@nexchg:

       mov EDI, [c2R]
       sub EDI, [c1R]
       shl EDI, 16
       mov [ccR], EDI

       mov EDI, [c2G]
       sub EDI, [c1G]
       shl EDI, 16
       mov [ccG], EDI

       mov EDI, [c2B]
       sub EDI, [c1B]
       shl EDI, 16
       mov [ccB], EDI

       mov EDI, [z2]
       sub EDI, [z1]
       shl EDI, 16
       mov [zz], EDI

       mov EDI, [texx2]
       sub EDI, [texx1]
       shl EDI, 16
       mov [korx], EDI

       mov EDI, [texy2]
       sub EDI, [texy1]
       shl EDI, 16
       mov [kory], EDI

       mov EDI, [x2]
       sub EDI, [x1]
       cmp EDI, 0
       jle @zerokor

       mov EAX, [ccR]
       cdq
       idiv EDI
       mov [ccR], EAX

       mov EAX, [ccG]
       cdq
       idiv EDI
       mov [ccG], EAX

       mov EAX, [ccB]
       cdq
       idiv EDI
       mov [ccB], EAX

       mov EAX, [korx]
       cdq
       idiv EDI
       mov [dxx], EAX

       mov EAX, [kory]
       cdq
       idiv EDI
       mov [dyy], EAX

       mov EAX, [zz]
       cdq
       idiv EDI
       mov [zz], EAX
@zerokor:

       mov EAX, [c1R]
       mov EBX, [c1G]
       shl EAX, 16
       mov EDX, [c1B]
       shl EBX, 16
       mov [cR], EAX
       shl EDX, 16
       mov [cG], EBX
       mov [cB], EDX

       mov EAX, [texx1]
       mov EBX, [texy1]
       shl EAX, 16
       mov EDX, [z1]
       shl EBX, 16
       mov [textx], EAX
       shl EDX, 16
       mov [texty], EBX
       mov [z], EDX

{***************** Star Renderer ***************************************************************}
   {Initialize the variables}
   mov ESI, [ip]
   dec ESI
   mov Cl, [MipMapFactor]
   shl ESI, Cl
   mov [ip], ESI
   mov ECX, [x1]
   mov EAX, [x2]
   dec EAX
   mov [x2], EAX
   mov EDX, [z]
   mov EBX, [cR]

@rep_star:
   {check if the current pixel on the line is out of screen => skip}
   cmp ECX, 0                     -- U
   jl  @next_star                 -- V          - 1
   cmp ECX, [Res_Xend]            -- U
   jg  @next_star                 -- V          - 1

   mov ESI, [yff]                 -- U
   add ESI, ECX                   -- V          - 1

   {check the z-buffer. if this z<zbuf draw else skip}
   lea EDI, zbuf[ESI*2]           -- U
   mov EAX, EDX                   -- V          - 1
   shr EAX, 16                    -- U          - 1
   cmp AX,[EDI]                   -- U
   jge @next_star                 -- V          - 5

   {write the new Z value in the z-buffer}
   mov [EDI],AX                   -- U          - 1

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

   {calculate the RGB light from the 16.16 fixed point}
   mov EDI, EBX                   -- U
   shr EDI, 18                    -- U
   mov EAX, [cG]
   shr EAX, 18
   mov [tmpG], EAX
   mov EAX, [cB]
   shr EAX, 18
   mov [tmpB], EAX

   {add the green light to the texel and clamp the value}
   mov EAX, [ggt]
   add EAX, [tmpG]
   cmp EAX, 255
   jle @bb_star
   mov EAX, 255
@bb_star:
   mov [ggt], EAX

   {add the red light to the texel and clamp the value}
   mov EAX, [rrt]
   add EAX, EDI
   cmp EAX, 255
   jle @gg_star
   mov EAX, 255
@gg_star:

   {add the blue light to the texel and clamp the value}
   add ESI, [tmpB]
   cmp ESI, 255
   jle @fin_star
   mov ESI, 255
@fin_star:

   {convert from RGB back to color}
   mov EDI, [ggt]
   shl EAX, 16
   shl EDI, 8
   add EAX, EDI
   add EAX, ESI

   {draw the curent pixel}
   mov EDI, [yff]
   add EDI, ECX
   mov ESI, [where]
   mov [ESI+EDI*4], EAX           -- U          - 1

@next_star:
   {increase the appropriate values by their factors for the next pixel}
   add EDX, [zz]                  -- U
   add EBX, [ccR]                 -- V          - 2
   mov EAX, [ccG]
   mov EDI, [ccB]
   add [cG], EAX
   add [cB], EDI

   mov EAX, [dxx]                 -- U
   mov EDI, [dyy]                  -- V          - 1
   add [textX], EAX               -- U
   add [textY], EDI               -- V          - 3

   inc ECX                        -- U
   mov EAX, [x2]                  -- V          - 1
   cmp ECX, EAX                   -- V          - 1
   jle @rep_star                  -- U          - 4    = 39 clocks per texel = 308 ns

@Exit:
   inc [yyy]
   mov EAX, [yyy]
   cmp EAX, [y2]
   jle @Repeat_Hline
end;

END.