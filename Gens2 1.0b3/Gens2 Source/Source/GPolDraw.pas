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

{Unit for the Polygon Drawing (Rendering) Functions -
                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit GPolDraw;

Interface

Uses
     Globals,
     GTypes3D;

Type
     gouraudcolor = array [1..10] of Integer;
     Sclippedtexel = array [1..10] of single;
     clippedtexel = array [1..10] of Integer;
     clippedpoly = array [1..10] of vertex;


Procedure PolyClip(invertex:lice; var outvertex:clippedpoly;
                   var outvertexcount:Integer; var cliptexx, cliptexy:clippedtexel;
                   c1R, c1G, c1B, c2R, c2G, c2B, c3R, c3G, c3B, c4R, c4G, c4B:Integer;
                   var gouraudcR, gouraudcG, gouraudcB:gouraudcolor; ip:Integer);
Procedure Draw_Poly_Unclipped(poli:lice; x1, y1, x2, y2, x3, y3, x4, y4, c1R, c1G, c1B, c2R,
                              c2G, c2B, c3R, c3G, c3B, c4R, c4G, c4B, z1, z2, z3, z4:Integer;
                              ip:Integer; altr:byte; dbli:Integer; where:Pointer);
Procedure Draw_Poly_Clipped(poli:lice; xp, yp:clippedtexel; zp:Sclippedtexel; vertco:Integer;
                            gorcR, gorcG, gorcB:gouraudcolor; cliptexelx, cliptexely:clippedtexel;
                            ip:Integer; altr:byte; dbli:Integer; where:Pointer);
Procedure Render_Clipped_Polygon(poli:lice; ip:Integer; altr:byte; dbli:Integer; where:Pointer);
Procedure MipMap_Calculation(x1, y1, x2, y2, x3, y3, x4, y4:single; ip:Integer);
Function In_Frustum(poly:lice; FOVtan:single):Boolean;
Procedure Calc_Perspective_Unclipped(poly:lice; var x1, y1, x2, y2, x3, y3, x4, y4:Single);
Function BackFace_Cull(x1, y1, x2, y2, x4, y4:Single; poly:lice): Boolean;
Procedure Adjust_View_Params(var FOVtan:single);
Procedure Render_Polygon(poly:lice; ip:Integer; altr:byte; dbli:Integer; where:Pointer);
Procedure QZSort(Var a,ip : sort; Lo,Hi : Integer; Direction : Boolean);
Procedure Clear_Z_Buffer;
Procedure Render_Scene(CamEye, CamDirAngles:Vertex);

Implementation

{$I Gdefines.inc}

Uses
{$ifdef __PASCAL_DRAW__}
     Draw_pas,
{$else}
     Draw_asm,
{$endif}
     RayTrace,
     GShOptmz,
     GDrwPrim,
     GMath3D,
     GTexture,
     GTransfr,
     GSpcPart,
     GLCorona;

Var
    ind,outvertexcount:Integer;
    zsort,ipol:sort;
    pan:Dword;

// Variables for the 3D-clipping function
    clippedpolygonvertex:clippedpoly;
    clippedtexelx,clippedtexely,pixelclipx,pixelclipy:clippedtexel;
    gouraudcR,gouraudcG,gouraudcB:gouraudcolor;


{This is the POLYGON CLIPPING PROCEDURE. It does REAL 3-D Polygon clipping.
 It clips towards the Z-Plane (View Plane). I should do clipping to the screen
 borders (bounding box clippind) it would speed up rendering, but i didn't
 have enough time, so probably in the next version.
 Invertex is the original unclipped polygon vertices, outvertex is the clipped
 vertices, outvertexcount is the number of the vertices in the clipped poly,
 cliptexx,cliptexy are the clipped texture coordinates (u,v),
 c1R,..., c2R,... are the input vertex RGB colors,
 gouraudcR,gouraudcG,gouraudcB are the clipped vertex RGB colors,
 ip is the current texture index}

Procedure PolyClip(invertex:lice; var outvertex:clippedpoly;
                   var outvertexcount:Integer; var cliptexx, cliptexy:clippedtexel;
                   c1R, c1G, c1B, c2R, c2G, c2B, c3R, c3G, c3B, c4R, c4G, c4B:Integer;
                   var gouraudcR, gouraudcG, gouraudcB:gouraudcolor; ip:Integer);
var
    tmpvertex:array[1..poly_vertex_count] of vertex;
    textmpx,textmpy:array[1..poly_vertex_count] of Integer;
    vertexloop,vcount,texturetmpx,texturetmpy:Integer;
    gortmpR,gortmpG,gortmpB:array[1..poly_vertex_count] of Integer;
    xclp,yclp,zclp,zfaktor,zeroz:single;
    gorclipR,gorclipG,gorclipB:Integer;
begin
  {Initialize the starting variables}
  tmpvertex[1] := invertex.t1;
  tmpvertex[2] := invertex.t2;
  tmpvertex[3] := invertex.t3;
  tmpvertex[4] := invertex.t4;

  textmpx[1] := 0; textmpy[1] := 0;
  textmpx[2] := TexXres[ip]; textmpy[2] := 0;
  textmpx[3] := TexXres[ip]; textmpy[3] := TexYres[ip];
  textmpx[4] := 0; textmpy[4] := TexYres[ip];

  gortmpR[1] := c1R;
  gortmpG[1] := c1G;
  gortmpB[1] := c1B;
  gortmpR[2] := c2R;
  gortmpG[2] := c2G;
  gortmpB[2] := c2B;
  gortmpR[3] := c3R;
  gortmpG[3] := c3G;
  gortmpB[3] := c3B;
  gortmpR[4] := c4R;
  gortmpG[4] := c4G;
  gortmpB[4] := c4B;

  {Repeat for all polygon vertices - 1}
  vcount := 0;
  for vertexloop := 1 to poly_vertex_count-1 do
  begin
    {Check if the current vertex Z coordinate is in front of the viewer
    (View Plane) and can be seen, then increase the output vertex count
    and add that vertex to the output vertex list}
    if tmpvertex[vertexloop].z>NearZClip then
    begin
      inc(vcount);
      outvertex[vcount] := tmpvertex[vertexloop];
      cliptexx[vcount] := textmpx[vertexloop];
      cliptexy[vcount] := textmpy[vertexloop];
      gouraudcR[vcount] := gortmpR[vertexloop];
      gouraudcG[vcount] := gortmpG[vertexloop];
      gouraudcB[vcount] := gortmpB[vertexloop];
    end;
    {Check if two adjacent vertices are seperated by the view plane i.e.
    one is in front and on is behind the view plane, then calculate the
    vertex that lies on the view plane and add that vertex to the output
    vertex list, and discard the vertex behind the plane}
    if ((tmpvertex[vertexloop].z<NearZClip) and (tmpvertex[vertexloop+1].z>NearZClip)) or
       ((tmpvertex[vertexloop].z>NearZClip) and (tmpvertex[vertexloop+1].z<NearZClip)) then
    begin
      inc(vcount);
      zclp := NearZClip;
      zeroz := tmpvertex[vertexloop+1].z-tmpvertex[vertexloop].z;
      if zeroz=0 then zeroz := 0.001;
      {Calculate a factor for the vertex that lies on the view plane}
      zfaktor := (zclp-tmpvertex[vertexloop].z)/zeroz;
      xclp := zfaktor*(tmpvertex[vertexloop+1].x-tmpvertex[vertexloop].x)+
            tmpvertex[vertexloop].x;
      yclp := zfaktor*(tmpvertex[vertexloop+1].y-tmpvertex[vertexloop].y)+
            tmpvertex[vertexloop].y;
      outvertex[vcount].x := xclp;
      outvertex[vcount].y := yclp;
      outvertex[vcount].z := zclp;

      {Calculate the texture clipping coordinates on the new vertex
      for appropriate texture mapping}
      texturetmpx := round(zfaktor*(textmpx[vertexloop+1]-textmpx[vertexloop])+
                         textmpx[vertexloop]);
      texturetmpy := round(zfaktor*(textmpy[vertexloop+1]-textmpy[vertexloop])+
                         textmpy[vertexloop]);

      {Clamp the values of the texture u,v coordinates
      to the size of the texture}
      if texturetmpx<0 then texturetmpx := 0;
      if texturetmpx>TexXres[ip] then texturetmpx := TexXres[ip];
      if texturetmpy<0 then texturetmpy := 0;
      if texturetmpy>TexYres[ip] then texturetmpy := TexYres[ip];


      {Add the texture coordinates to the output list}
      cliptexx[vcount] := texturetmpx;
      cliptexy[vcount] := texturetmpy;

      {Calculate the Gouraud Color RGB values for the new vertex for
      appropriate gouraud lighting}
      gorclipR := round(zfaktor*(gortmpR[vertexloop+1]-gortmpR[vertexloop])+
               gortmpR[vertexloop]);
      gorclipG := round(zfaktor*(gortmpG[vertexloop+1]-gortmpG[vertexloop])+
               gortmpG[vertexloop]);
      gorclipB := round(zfaktor*(gortmpB[vertexloop+1]-gortmpB[vertexloop])+
               gortmpB[vertexloop]);

      {Clamp the Gouraud Color RGB values to 0 as
      low limit and 255 as high limit}
      if gorclipR<0 then gorclipR := 0;
//      if gorclipR>1023 then gorclipR := 1023;
      if gorclipG<0 then gorclipG := 0;
//      if gorclipG>1023 then gorclipG := 1023;
      if gorclipB<0 then gorclipB := 0;
//      if gorclipB>1023 then gorclipB := 1023;

      {Add the Gouraud Color values to the output list}
      gouraudcR[vcount] := gorclipR;
      gouraudcG[vcount] := gorclipG;
      gouraudcB[vcount] := gorclipB;
    end;
  end;

  {This part is for the last pixel in the polygon}

  {Check if the vertex Z coordinate is in front of the viewer
  (View Plane) and can be seen, then increase the output vertex count
  and add that vertex to the output vertex list}
  if tmpvertex[poly_vertex_count].z>NearZClip then
  begin
    inc(vcount);
    outvertex[vcount] := tmpvertex[poly_vertex_count];
    cliptexx[vcount] := textmpx[poly_vertex_count];
    cliptexy[vcount] := textmpy[poly_vertex_count];
    gouraudcR[vcount] := gortmpR[poly_vertex_count];
    gouraudcG[vcount] := gortmpG[poly_vertex_count];
    gouraudcB[vcount] := gortmpB[poly_vertex_count];
  end;
  {Check if two adjacent vertices are seperated by the view plane i.e.
  one is in front and on is behind the view plane, then calculate the
  vertex that lies on the view plane and add that vertex to the output
  vertex list, and discard the vertex behind the plane}
  if ((tmpvertex[poly_vertex_count].z<NearZClip) and (tmpvertex[1].z>NearZClip)) or
     ((tmpvertex[poly_vertex_count].z>NearZClip) and (tmpvertex[1].z<NearZClip)) then
  begin
    inc(vcount);
    zclp := NearZClip;
    zeroz := tmpvertex[1].z-tmpvertex[poly_vertex_count].z;
    if zeroz=0 then zeroz := 0.001;
    {Calculate a factor for the vertex that lies on the view plane}
    zfaktor := (zclp-tmpvertex[poly_vertex_count].z)/zeroz;
    xclp := zfaktor*(tmpvertex[1].x-tmpvertex[poly_vertex_count].x)+
          tmpvertex[poly_vertex_count].x;
    yclp := zfaktor*(tmpvertex[1].y-tmpvertex[poly_vertex_count].y)+
          tmpvertex[poly_vertex_count].y;
    outvertex[vcount].x := xclp;
    outvertex[vcount].y := yclp;
    outvertex[vcount].z := zclp;

    {Calculate the texture clipping coordinates on the new vertex
    for appropriate texture mapping}
    texturetmpx := round(zfaktor*(textmpx[1]-textmpx[poly_vertex_count])+
                       textmpx[poly_vertex_count]);
    texturetmpy := round(zfaktor*(textmpy[1]-textmpy[poly_vertex_count])+
                       textmpy[poly_vertex_count]);

    {Clamp the values of the texture u,v coordinates
    to the size of the texture}
    if texturetmpx<0 then texturetmpx := 0;
    if texturetmpx>TexXres[ip] then texturetmpx := TexXres[ip];
    if texturetmpy<0 then texturetmpy := 0;
    if texturetmpy>TexYres[ip] then texturetmpy := TexYres[ip];

    {Add the texture coordinates to the output list}
    cliptexx[vcount] := texturetmpx;
    cliptexy[vcount] := texturetmpy;

    {Calculate the Gouraud Color RGB values for the new vertex for
    appropriate gouraud lighting}
    gorclipR := round(zfaktor*(gortmpR[1]-gortmpR[poly_vertex_count])+
             gortmpR[poly_vertex_count]);
    gorclipG := round(zfaktor*(gortmpG[1]-gortmpG[poly_vertex_count])+
             gortmpG[poly_vertex_count]);
    gorclipB := round(zfaktor*(gortmpB[1]-gortmpB[poly_vertex_count])+
             gortmpB[poly_vertex_count]);

    {Clamp the Gouraud Color RGB values to 0 as
    low limit and 255 as high limit}
    if gorclipR<0 then gorclipR := 0;
//    if gorclipR>1023 then gorclipR := 1023;
    if gorclipG<0 then gorclipG := 0;
//    if gorclipG>1023 then gorclipG := 1023;
    if gorclipB<0 then gorclipB := 0;
//    if gorclipB>1023 then gorclipB := 1023;

    {Add the Gouraud Color values to the output list}
    gouraudcR[vcount] := gorclipR;
    gouraudcG[vcount] := gorclipG;
    gouraudcB[vcount] := gorclipB;
  end;
  outvertexcount := vcount;
end;

{This is the unclipped POLYGON rendering PROCEDURE that takes transformed
 and perspective projected values of the polygon's vertices x1,y1,x2,y2,...
 the vertex RGB color values c1R,c1G,c1B,..., the Z values of the vertices
 z1,z2,z3,z4, then does SCANEDGE and then it renders the polygon.
 alpha is a check if the current polygon is with alpha blending,
 ip is the current texture index, altr is the translucency index,
 dbli is the current polygon index, where is the start memory adress}

Procedure Draw_Poly_Unclipped(poli:lice; x1, y1, x2, y2, x3, y3, x4, y4, c1R, c1G, c1B, c2R,
                              c2G, c2B, c3R, c3G, c3B, c4R, c4G, c4B, z1, z2, z3, z4:Integer;
                              ip:Integer; altr:byte; dbli:Integer; where:Pointer);
var
    mny,mxy,y:Integer;
    PPNormal:vertex;
begin
  {Find the minimum (highest) y coordinate from the polygon for starting
  point for the polygon rendering}
  mny := y1;
  mxy := y1;
  if y2 < mny then mny := y2;
  if y2 > mxy then mxy := y2;
  if y3 < mny then mny := y3;
  if y3 > mxy then mxy := y3;
  if y4 < mny then mny := y4;
  if y4 > mxy then mxy := y4;

  {This checks if the polygon is out of screen => exit.
  Also does some clamping and initial setup for the scanedge PROCEDURE}
  if mny < 0 then mny := 0;
  if mxy > Res_Yend then mxy := Res_Yend;
  if mxy < 0 then Exit;
  if mny > Res_Yend then Exit;

  for y := 0 to Res_Yend do
    begin
      poly[y,1] := Res_Xend_p1;
      poly[y,2] := -1;
    end;

  {This calculates the needed values for rendering the polygon for all
  four sides of the polygon}
  Scan_Edge(x1, y1, x2, y2, c1R, c1G, c1B, c2R, c2G, c2B, z1, z2, 0, 0, TexXres[ip], 0);
  Scan_Edge(x2, y2, x3, y3, c2R, c2G, c2B, c3R, c3G, c3B, z2, z3, TexXres[ip], 0, TexXres[ip], TexYres[ip]);
  Scan_Edge(x3, y3, x4, y4, c3R, c3G, c3B, c4R, c4G, c4B, z3, z4, TexXres[ip], TexYres[ip], 0, TexYres[ip]);
  Scan_Edge(x4, y4, x1, y1, c4R, c4G, c4B, c1R, c1G, c1B, z4, z1, 0, TexYres[ip], 0, 0);

  {This renders the current polygon by vertically drawing all the horizontal
  lines starting from the top (lowest) y coordinate of the polygon}
  if (prpixel) then
    begin
      if (altr > 0) then
        Textured_Polygon_per_pixel_Alpha(poli, mny, mxy, ip, dbli, altr, where)
      else
        Textured_Polygon_per_pixel(poli, mny, mxy, ip, dbli, where);
    end
  else
    if (altr > 0) then
      Textured_Polygon_Alpha(mny, mxy, ip, altr, where)
    else
      if (OldRendering) then
        Textured_Polygon_Old_Renderer(mny, mxy, ip, where)
      else
        Textured_Polygon(mny, mxy, ip, where);
end;

{This is the clipped POLYGON rendering PROCEDURE that takes transformed
 and perpective projected values of the polygon's vertices xp,yp,
 vertco is the clipped polygon vertex count,
 the vertex RGB color values gorcR,gorcG,gorcB, the Z values of the vertices
 zc, the clipped texture coordinates (u,v) cliptexelx,cliptexely,
 then does SCANEDGE and then it renders the polygon.
 alpha is a check if the current polygon is with alpha blending,
 ip is the current texture index, altr is the translucency index,
 dbli is the current polygon index, where is the start memory address}

Procedure Draw_Poly_Clipped(poli:lice; xp, yp:clippedtexel; zp:Sclippedtexel; vertco:Integer;
                            gorcR, gorcG, gorcB:gouraudcolor; cliptexelx, cliptexely:clippedtexel;
                            ip:Integer; altr:byte; dbli:Integer; where:Pointer);
var
    mny,mxy,mnx,mxx,y,vco:Integer;
    PPNormal:vertex;
begin
  {Find the minimum (highest) y coordinate from the polygon for starting
  point for the polygon rendering}
  mny := yp[1];
  mxy := yp[1];
  for vco := 2 to vertco do
  begin
    if (yp[vco] < mny) then mny := yp[vco];
    if (yp[vco] > mxy) then mxy := yp[vco];
  end;

  {This checks if the polygon is out of screen => exit.
  Also does some clamping and initial setup for the scanedge PROCEDURE}
  if mny < 0 then mny := 0;
  if mxy > Res_Yend then mxy := Res_Yend;
  if mxy < 0 then exit;
  if mny > Res_Yend then exit;

  for y := 0 to Res_Yend do
    begin
      poly[y,1] := Res_Xend_p1;
      poly[y,2] := -1;
    end;

  {This calculates the needed values for rendering the polygon for all
  four sides of the polygon}
  for vco := 1 to vertco-1 do
  Scan_Edge(xp[vco],yp[vco],xp[vco+1],yp[vco+1],gorcR[vco],gorcG[vco],gorcB[vco],
            gorcR[vco+1],gorcG[vco+1],gorcB[vco+1],round(zp[vco]),round(zp[vco+1]),
            cliptexelx[vco],cliptexely[vco],cliptexelx[vco+1],cliptexely[vco+1]);
  vco := vertco;
  Scan_Edge(xp[vco],yp[vco],xp[1],yp[1],gorcR[vco],gorcG[vco],gorcB[vco],
            gorcR[1],gorcG[1],gorcB[1],round(zp[vco]),round(zp[1]),
            cliptexelx[vco],cliptexely[vco],cliptexelx[1],cliptexely[1]);

  {This renders the current polygon by vertically drawing all the horizontal
  lines starting from the top (lowest) y coordinate of the polygon}
  if (prpixel) then
    begin
      if (altr > 0) then
        Textured_Polygon_per_pixel_Alpha(poli, mny, mxy, ip, dbli, altr, where)
      else
        Textured_Polygon_per_pixel(poli, mny, mxy, ip, dbli, where);
    end
  else
    if (altr > 0) then
      Textured_Polygon_Alpha(mny, mxy, ip, altr, where)
    else
      if (OldRendering) then
        Textured_Polygon_Old_Renderer(mny, mxy, ip, where)
      else
        Textured_Polygon(mny, mxy, ip, where);
end;

{This is the clipped polygon TRANSFORM and PROJECT PROCEDURE and it also
 calculates the LIGHT of the polygon}

Procedure Render_Clipped_Polygon(poli:lice; ip:Integer; altr:byte; dbli:Integer; where:Pointer);
var
    vc,count:Integer;
    zclip:Sclippedtexel;
    testwx,testwy,testwz,testxu,testyu,
    testzu,testxv,testyv,testzv,nor,test:single;

BEGIN
  // Optimize lighting for the polygons in the frustum by checking if a
  // light ray actually changes the light condition on the observed polygon
  Optimize_Viewable_Polygons_Lighting(dbli);

  {Calculate the light on the polygon according to the Real Lighting System}
  Vertex_Ray_Tracing(poli, ip, dbli);

  {Clip the polygon to the view plane}
  PolyClip(poli, clippedpolygonvertex, outvertexcount,
           clippedtexelx, clippedtexely, c1R, c1G, c1B, c2R, c2G, c2B,
           c3R, c3G, c3B, c4R, c4G, c4B, gouraudcR, gouraudcG, gouraudcB, ip);

  {Arrange Z values of the polygon for calculating perspective projection}
  for vc := 1 to outvertexcount do zclip[vc] := clippedpolygonvertex[vc].z;

  {Calculate perspective projection for all polygon vertices}
    for vc := 1 to outvertexcount do
       begin
         pixelclipx[vc] := round((clippedpolygonvertex[vc].x * focusX) / (zclip[vc]) + xcenter);
         pixelclipy[vc] := round((clippedpolygonvertex[vc].y * focusY) / (zclip[vc]) + ycenter);
       end;

  {Render the clipped polygon with the appropriate parameters}
  Draw_Poly_Clipped(poli, pixelclipx, pixelclipy, zclip, outvertexcount, gouraudcR, gouraudcG, gouraudcB,
                    clippedtexelx, clippedtexely, ip, altr, dbli, where);

  {Increase the polygon count for tracking how many polygons
  are rendered on screen}
  Inc(ScreenPolyCount);

  {This draws the outlines of the polygon in white color}
  if (showtris) then
    for vc := 1 to outvertexcount do
       line32bpp(pixelclipx[vc], pixelclipy[vc],
                 pixelclipx[vc mod outvertexcount + 1], pixelclipy[vc mod outvertexcount + 1],
                 200, 200, 200, where);
END;

{Calculates an appropriate mipmap size based on the actual screen polygon surface}
Procedure MipMap_Calculation(x1, y1, x2, y2, x3, y3, x4, y4:single; ip:Integer);
var
    mipp,aa,bb,cc,dd,ee,ss1,ss2,pp:single;
begin
  // This calculates the polygon sides
  if (mipmap) then
    begin
      aa := Safe_Sqrt( sqr(x2 - x1) + sqr(y2 - y1) );
      bb := Safe_Sqrt( sqr(x3 - x2) + sqr(y3 - y2) );
      cc := Safe_Sqrt( sqr(x4 - x3) + sqr(y4 - y3) );
      dd := Safe_Sqrt( sqr(x4 - x1) + sqr(y4 - y1) );
      ee := Safe_Sqrt( sqr(x3 - x1) + sqr(y3 - y1) );

      ss1  :=  (aa + bb + ee) / 2;
      ss2  :=  (cc + dd + ee) / 2;

      // Calculate a mipmap factor according to the polygon surface
      pp := Safe_Sqrt(ss1 * (ss1 - aa) * (ss1 - bb) * (ss1 - ee)) +
            Safe_Sqrt(ss2 * (ss2 - cc) * (ss2 - dd) * (ss2 - ee));
      mipp := Safe_Sqrt(pp);

      // This procedure determines the mipmap sizes
      MipMapDetermine(mipp, ip);
    end
  else
    begin
      MipMapOffset := Ofs(texbuf);
      MipMapFactor := 18;
      MipMapResF := 8;
      TexXres[ip] := 255;
      TexYres[ip] := 255;
    end;
end;

{Checks if a polygon is in the view frustum (view pyramid)}
Function In_Frustum(poly:lice; FOVtan:single):Boolean;
var
    maxclip:Single;
    tempZ:lice;
begin
  tempZ.t1.z := poly.t1.z * FOVtan;
  tempZ.t2.z := poly.t2.z * FOVtan;
  tempZ.t3.z := poly.t3.z * FOVtan;
  tempZ.t4.z := poly.t4.z * FOVtan;

  {This part Clips the Polygon to the FRUSTUM for optimization}
  if (poly.t1.x <= -tempZ.t1.z) and (poly.t2.x <= -tempZ.t2.z) and
     (poly.t3.x <= -tempZ.t3.z) and (poly.t4.x <= -tempZ.t4.z) then
    begin
      Result := FALSE;
      Exit;
    end;

  if (poly.t1.x >= tempZ.t1.z) and (poly.t2.x >= tempZ.t2.z) and
     (poly.t3.x >= tempZ.t3.z) and (poly.t4.x >= tempZ.t4.z) then
    begin
      Result := FALSE;
      Exit;
    end;

  if (poly.t1.y <= -tempZ.t1.z) and (poly.t2.y <= -tempZ.t2.z) and
     (poly.t3.y <= -tempZ.t3.z) and (poly.t4.y <= -tempZ.t4.z) then
    begin
      Result := FALSE;
      Exit;
    end;

  if (poly.t1.y >= tempZ.t1.z) and (poly.t2.y >= tempZ.t2.z) and
     (poly.t3.y >= tempZ.t3.z) and (poly.t4.y >= tempZ.t4.z) then
    begin
      Result := FALSE;
      Exit;
    end;

  maxclip := poly.t1.z;
  if (poly.t2.z > maxclip) then maxclip := poly.t2.z;
  if (poly.t3.z > maxclip) then maxclip := poly.t3.z;
  if (poly.t4.z > maxclip) then maxclip := poly.t4.z;
  if (maxclip <= NearZClip) or (maxclip >= FarZClip) then
    begin
      Result := FALSE;
      Exit;
    end;

  Result := TRUE;
end;

Procedure Calc_Perspective_Unclipped(poly:lice; var x1, y1, x2, y2, x3, y3, x4, y4:Single);
var
    minz,minfac:Single;
begin
  {This part checks if the polygon is clipped and calculates a factor
  for advancing Z coordinates to clipping polygons for appropriate
  mipmapping calculation}
  if (poly.t1.z <= NearZClip) or (poly.t2.z <= NearZClip) or
     (poly.t3.z <= NearZClip) or (poly.t4.z <= NearZClip) then
    begin
      minz := poly.t1.z;
      if (poly.t2.z < minz) then minz := poly.t2.z;
      if (poly.t3.z < minz) then minz := poly.t3.z;
      if (poly.t4.z < minz) then minz := poly.t4.z;
      minfac := Abs(minz) + 1;
    end
  else
    minfac := 0;

  {Calculate perspective projection for all polygon vertices}
  x1 := poly.t1.x * FocusX / (poly.t1.z + minfac) + xcenter;
  y1 := poly.t1.y * FocusY / (poly.t1.z + minfac) + ycenter;
  x2 := poly.t2.x * FocusX / (poly.t2.z + minfac) + xcenter;
  y2 := poly.t2.y * FocusY / (poly.t2.z + minfac) + ycenter;
  x3 := poly.t3.x * FocusX / (poly.t3.z + minfac) + xcenter;
  y3 := poly.t3.y * FocusY / (poly.t3.z + minfac) + ycenter;
  x4 := poly.t4.x * FocusX / (poly.t4.z + minfac) + xcenter;
  y4 := poly.t4.y * FocusY / (poly.t4.z + minfac) + ycenter;
end;

Function BackFace_Cull(x1, y1, x2, y2, x4, y4:Single; poly:lice): Boolean;
var
    PNormal,TestVect:vertex;
    testxu,testyu,testzu,testxv,testyv,testzv:single;
begin
  testxu := x2 - x1;
  testyu := y2 - y1;
  testzu := poly.t2.z - poly.t1.z;
  testxv := x4 - x1;
  testyv := y4 - y1;
  testzv := poly.t4.z - poly.t1.z;

  {Calculate a normal for the polygon}
  PNormal.x := testyu * testzv - testzu * testyv;
  PNormal.y := testzu * testxv - testxu * testzv;
  PNormal.z := testxu * testyv - testyu * testxv;

  {Fixed Vector, perpendicular to the view plane, faced away from the viewer.
  An angle is calculated between this vector and the polygon normal, and if
  the angle is < 0 then the polygon is facing away from the viewer => don't render}
  TestVect.x := 0;
  TestVect.y := 0;
  TestVect.z := -100;

  {Test if the polygon normal is faced away from the viewer => don't render}
  if (Cos_Theta(PNormal, TestVect) > 0) then
    Result := FALSE
  else
    Result := TRUE;
end;

Procedure Adjust_View_Params(var FOVtan:single);
var
    FOVFac,FOVan:single;
begin
  // These are the View Calculations
  FOVan := (FOV / 2) * Pi / 180;
  FOVtan := Sin(FOVan) / Cos(FOVan);
  FocusX := Res_Xend_p1 / 2 / FOVtan;

  if (Res_Xend_p1 = 320) then
    FocusY := FocusX / 1.2
  else
    FocusY := FocusX;

  // Tweak the View/Clip Plane Position to lower the affine texture mapping distortion:
  FOVFac := (MaxNearZClip - MinNearZClip) / (MaxFOV - DefaultFOV);

  if (FOV >= DefaultFOV) then
    NearZClip := MaxNearZClip - (FOV mod DefaultFOV) * FOVFac
  else
    NearZClip := MaxNearZClip;
end;

{This is the Main Polygon Rendering procedure. It renders both clipped and unclipped polygons,
 with texture mapping, with alpha blending, with light per_vertex/per_pixel.
 Also it takes care for all the setup involved: Frustum Culling, Backface Culling,
 FOV Calculations, Perspective Projection, MipMap Calculations}

Procedure Render_Polygon(poly:lice; ip:Integer; altr:byte; dbli:Integer; where:Pointer);
var
    x1,y1,x2,y2,x3,y3,x4,y4:single;
begin
  if NOT(In_Frustum(poly, FOVtan)) then Exit;

  Calc_Perspective_Unclipped(poly, x1, y1, x2, y2, x3, y3, x4, y4);

  MipMap_Calculation(x1, y1, x2, y2, x3, y3, x4, y4, ip);

  // Is this polygon intersecting the View Plane ?
  if (poly.t1.z <= NearZClip) or (poly.t2.z <= NearZClip) or
     (poly.t3.z <= NearZClip) or (poly.t4.z <= NearZClip) then
    begin
      // Then call it's dedicated function
      Render_Clipped_Polygon(poly, ip, altr, dbli, where);
      Exit;
    end;

  if NOT(BackFace_Cull(x1, y1, x2, y2, x4, y4, poly)) or (doublesided[dbli]) then
    begin
      // Optimize lighting for the polygons in the frustum by checking if a
      // light ray actually changes the light condition on the observed polygon
      Optimize_Viewable_Polygons_Lighting(dbli);

      // Calculate the light on the polygon according to the real lighting system
      Vertex_Ray_Tracing(poly, ip, dbli);

      // Render the unclipped polygon with the appropriate parameters
      Draw_Poly_Unclipped(poly, round(x1), round(y1), round(x2), round(y2),
                          round(x3), round(y3), round(x4), round(y4),
                          c1R, c1G, c1B, c2R, c2G, c2B, c3R, c3G, c3B, c4R, c4G, c4B,
                          round(poly.t1.z), round(poly.t2.z), round(poly.t3.z), round(poly.t4.z),
                          ip, altr, dbli, where);

      // Increase the polygon count for tracking how many polygons are rendered on screen
      Inc(ScreenPolyCount);
    end;

  {This draws the outlines of the polygon in white color}
  if (showtris) then
    begin
      line32bpp(round(x1), round(y1), round(x2), round(y2), 200, 200, 200, where);
      line32bpp(round(x2), round(y2), round(x3), round(y3), 200, 200, 200, where);
      line32bpp(round(x3), round(y3), round(x4), round(y4), 200, 200, 200, where);
      line32bpp(round(x4), round(y4), round(x1), round(y1), 200, 200, 200, where);
    end;
end;

{Z-SORTING PROCEDURE (Quick Sort)
 if Direction is "FALSE" it does FRONT-TO-BACK sort, if it's "TRUE" it does BACK-TO-FRONT}

Procedure QZSort(Var a,ip : sort; Lo,Hi : Integer; Direction : Boolean);

procedure sort(l,r:integer);
var
  i,j,x,y: integer;
begin
  i:=l; j:=r; x:=a[(l+r) DIV 2];

  if NOT(Direction) then
    repeat
      while a[i]<x do inc(i);
      while x<a[j] do dec(j);

      if i<=j then
      begin
        y:=a[i]; a[i]:=a[j]; a[j]:=y;
        y:=ip[i]; ip[i]:=ip[j]; ip[j]:=y;
        inc(i); dec(j);
      end;
    until i>j
  else
    repeat
      while a[i]>x do inc(i);
      while x>a[j] do dec(j);

      if i<=j then
      begin
        y:=a[i]; a[i]:=a[j]; a[j]:=y;
        y:=ip[i]; ip[i]:=ip[j]; ip[j]:=y;
        inc(i); dec(j);
      end;
    until i>j;

  if l<j then sort(l,j);
  if i<r then sort(i,r);
end;

begin
    sort(Lo,Hi);
end;


{Clears the Z-Buffer}
Procedure Clear_Z_Buffer; assembler;
asm
   cld
   mov ECX, [Res_PixNum]
   shr ECX, 1
   mov EAX, 32767
   shl EAX, 16
   mov AX, 32767
   lea EDI, [zbuf]
   ALIGN    4
   rep stosd
end;


{------------------------------------------------------------------------------------}
{--- Main Rendering PROCEDURE -------------------------------------------------------}
{--- It Renders the scene at current Camera position and view -----------------------}
{--- CamEye is the position of the camera's eye, and the CamDirAngles are -----------}
{--- direction angles relative to the world axis x,y,z (where the Cam is pointing)---}
{------------------------------------------------------------------------------------}
Var
    LensStatic:array[1..pol] of boolean;

Procedure Render_Scene(CamEye, CamDirAngles:Vertex);
BEGIN
  // Polygons Displayed Initialization
  ScreenPolyCount := 0;
  RaysCasted := 0;

  Adjust_View_Params(FOVtan);

  // Transform the 3-D World according to Camera position and Direction
  RotateObjects;
  TranslateWorld(CamEye);
  RotateWorld(CamDirAngles);

  Determine_Zone_Locality;
  Tweak_Shadow_Tolerance;

  // First Render all polygons without the alpha polygons

  // Setup the Z-Coordinates of the polygons for Z-Sorting including Lens Flares
  for pa := 1 to polmax do
     begin
       zsort[pa] := round(rotpoly[pa].t1.z + rotpoly[pa].t2.z +
                          rotpoly[pa].t3.z + rotpoly[pa].t4.z);
       ipol[pa] := pa;
       LensStatic[pa] := false;
     end;
  pan := 1;
  for pa := polmax + 1 to polmax + litmax do
     begin
       zsort[pa] := round(LightVertex[pan].z);
       ipol[pa] := pa;
       LensStatic[pa] := true;
       inc(pan);
     end;

  // Sort the polygons Front-to-Back
  QZSort(zsort, ipol, 0, polmax - 1, False);

  // Cull polygons from shadow casting objects if they are looking away from the light
  If (CastShadows) then
    begin
      Optimize_Shadow_Casting_Objects;
      SortShadowCastingPolygons;
      ComputeNormals;
    end;

  // DO the Final Rendering (without alpha polygons)
  for pan := 1 to polmax do
     begin
       ind := ipol[pan];
       if (alphap[ind] = 0) and (NOT((NOT(RecvShadowPoly[ind])) and (CastShadowPoly[ind]))) and
          (Is_Polygon_In_Player_Zone(rotpoly[ind])) then
         Render_Polygon(rotpoly[ind], pol2tex[ind], alphap[ind], ind, VirtBuffer);
     end;

  // Render all Alpha polygons including Lens Flares

  // Sort the polygons Back-to-Front
  QZSort(zsort, ipol, 0, polmax - 1, True);

  // DO the Final Rendering (only alpha polygons and Lens Flares)
  for pan := 1 to polmax + litmax do
     begin
       ind := ipol[pan];
       if not(lightsoff) and (litmax > 0) and (LensStatic[ind]) and (lensfv) and
          (Is_Vertex_In_Player_Zone(LightVertex[ind - polmax])) then
         DrawLightSprite(LightVertex[ind - polmax], NearZClip, LightR[ind - polmax], LightG[ind - polmax],
                         LightB[ind - polmax], LightIntensity[ind - polmax],Is_Dynamic[ind-polmax],VirtBuffer);
       if (alphap[ind] > 0) and (NOT((NOT(RecvShadowPoly[ind])) and (CastShadowPoly[ind]))) and
          (Is_Polygon_In_Player_Zone(rotpoly[ind])) then
         Render_Polygon(rotpoly[ind], pol2tex[ind], alphap[ind], ind, VirtBuffer);
     end;
END;

END.