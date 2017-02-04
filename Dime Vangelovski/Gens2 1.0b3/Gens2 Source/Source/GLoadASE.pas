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

{Unit for Loading the 3-D World from .ASE ASCII file and World Setup -
                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit GLoadASE;

Interface

Procedure Load_3DWorld(filename:string; var PolyCount,LightCount,SphereZonesCount,RotObjectsCount:Integer);
Procedure FixScene(PolyCount,LightCount,ZoneCo:Integer);
Procedure SceneDime;

Implementation

Uses
     GTypes3D,
     Globals,
     Windows,
     GWinMnge,
     GMath3D,
     GOut2Log;

Const
      ZnaciVert = ['0'..'9','.','-'];

Var
    VertexCount,ci,mii,tpoc,tkraj,apoc,akraj,texc,vpol,codn,MaskA,MaskB,MaskC,MaskD,
    PolyCountStart,PolyCountEnd,MaterialID,transp,RotCenterCount,RotCenterIndex:Integer;
    AlphaVal,LightIntens,LRR,LGG,LBB:Single;
    TheRest,Transparency,TmpStr,NumStr,xxstr,yystr,zzstr:string;
    Vertices:array[1..MaxVertex] of vertex;
    vert:array[1..10] of Integer;
    verti:array[1..pol,1..Poly_vertex_count] of Integer;
    WrdStr:array[1..10] of Integer;
    vxx:array[1..10] of string;
    AlphaValue:array[1..maxtex+1] of byte;
    dbl,RotObject:boolean;
    AngleAdd:vertex;
    code:smallint;
    Fw3d:text;
    alp:byte;

{******************************************************************************
******************* Read 3-D World data from file *****************************
******************************************************************************}
Procedure Load_3DWorld(filename:string; var PolyCount,LightCount,SphereZonesCount,RotObjectsCount:Integer);
var strMSG, strConv: String;
    texPoc, texKraj: Byte;
    ZoneRadius:single;

BEGIN
{******************************************************************************
******************* This is for 3D Studio .ASE Loading ************************
******************************************************************************}
  assign(Fw3d, filename);
  FileMode := 0;  // Just READ the file, don't open for WRITING - so it can work from a CD-ROM
  reset(Fw3d);

  TheRest := '';
  CastCount := 0;
  PolyCount := 0;
  LightCount := 0;
  SphereZonesCount := 0;
  RotObjectsCount := 0;
  RotCenterCount := 0;
  while NOT(Eof(Fw3d)) do
  Begin
  {*****************************************************************************
  ** Load the Materials used in the world **************************************
  *****************************************************************************}
    If pos('*MATERIAL_LIST {',TheRest)<>0 then
      Begin
        texc:=0;
        transp:=0;
        MaterialCount:=0;
        while (pos('*GEOMOBJECT {',TheRest)=0) and (pos('*LIGHTOBJECT {',TheRest)=0) do
          begin
            readln(Fw3d, TheRest);
            If pos('*MATERIAL_TRANSPARENCY',TheRest)<>0 then
              begin
                inc(transp);
                TmpStr:=TheRest;
                j:=pos('Y',TmpStr);
                Delete(TmpStr,1,j+1);
                val(TmpStr,AlphaVal,codn);
                AlphaValue[transp]:=round(AlphaVal*255);
              end;
            If pos('*BITMAP "',TheRest)<>0 then
              begin
                If (pos('.bmp"',TheRest)=0) and (pos('.BMP"',TheRest)=0) then
                  begin
                    if DisplayMessage('Some of the textures is not a .BMP file !', WINDOW_TITLE, MSG_ERROR) = IDOK then
                      begin
                        PostQuitMessage(0);
                        Halt(0);
                      end;
                  end;
                inc(texc);
                inc(MaterialCount);
                If texc>maxtex then
                  begin
                    Str(maxtex, strConv);
                    strMSG := 'The texture number exceeds the maximum of ' + strConv + ' textures allowed !   ';
                    if DisplayMessage(strMSG, WINDOW_TITLE, MSG_ERROR) = IDOK then
                      begin
                        PostQuitMessage(0);
                        Halt(0);
                      end;
                  end;
                if pos('textures',TheRest) <> 0 then
                  texPoc := pos('textures',TheRest)
                else
                  if pos('TEXTURES',TheRest) <> 0 then
                    texPoc := pos('TEXTURES',TheRest)
                  else
                    if pos('Textures',TheRest) <> 0 then
                      texPoc := pos('Textures',TheRest);
                if pos('p"',TheRest) <> 0 then
                  texKraj := pos('p"',TheRest) + 1
                else
                  if pos('P"',TheRest) <> 0 then
                    texKraj := pos('P"',TheRest) + 1;
                Texture_Filename[texc]:=copy(TheRest, texPoc, texKraj - texPoc);
              end;
          end;
      End;

  {****************************************************************************
   * Load the Geometrical shapes **********************************************
   ****************************************************************************}
    If pos('*GEOMOBJECT {',TheRest)<>0 then
      Begin
        readln(Fw3d, TheRest);
        // Default Object Mask - Applies to Box, Sphere, Teapot, TorusKnot,...
        MaskA:=1;
        MaskB:=2;
        MaskC:=3;
        MaskD:=4;
        // Do some tweak for some of the objects to load properly
        If pos('*NODE_NAME "',TheRest)<>0 then
          begin
            If ((pos('"Torus',TheRest)<>0) or (pos('"Cylinder',TheRest)<>0) or (pos('"Cone',TheRest)<>0) or
                (pos('"Tube',TheRest)<>0)) and (pos('Knot',TheRest)=0) then
              begin
                MaskA:=1;
                MaskB:=3;
                MaskC:=4;
                MaskD:=2;
              end;
            If pos('"Plane',TheRest) <> 0 then
              begin
                MaskA:=4;
                MaskB:=1;
                MaskC:=3;
                MaskD:=2;
              end;
            If pos('RotObject',TheRest) <> 0 then
              begin
                Inc(RotObjectsCount);
                RotObject := true;

                ci := 1;
                For mii := pos('RotCenter',TheRest) to length(TheRest) - 1 do
                   if (TheRest[mii] in ZnaciVert) and not(TheRest[mii + 1] in ZnaciVert) then
                     begin
                       WrdStr[ci] := mii;
                       Inc(ci);
                     end;
                WrdStr[ci] := mii + 1;

                NumStr := copy(TheRest, pos('RotCenter',TheRest) + 9, WrdStr[1] - (pos('RotCenter',TheRest) + 8));
                xxstr := copy(TheRest, wrdstr[1] + 2, wrdstr[2] - (wrdstr[1] + 1));
                yystr := copy(TheRest, wrdstr[2] + 2, wrdstr[3] - (wrdstr[2] + 1));
                zzstr := copy(TheRest, wrdstr[3] + 2, wrdstr[4] - (wrdstr[3] + 1));

                val(NumStr, RotCenterIndex, codn);
                val(xxstr, AngleAdd.X, codn);
                val(yystr, AngleAdd.Y, codn);
                val(zzstr, AngleAdd.Z, codn);

                RotObjAnglesAdd[RotObjectsCount].X := AngleAdd.X * Pi / 180;
                RotObjAnglesAdd[RotObjectsCount].Y := - AngleAdd.Z * Pi / 180;
                RotObjAnglesAdd[RotObjectsCount].Z := AngleAdd.Y * Pi / 180;
              end
            else
              RotObject := false;
          end;

        while pos('*MESH_VERTEX_LIST {',TheRest)=0 do
          readln(Fw3d, TheRest);

        // This is for loading Vertices
        If pos('*MESH_VERTEX_LIST {',TheRest)<>0 then
          begin
            i:=0;
            readln(Fw3d, TheRest);
            while pos('}',TheRest)=0 do
              begin
                TmpStr:=TheRest;
                str(i,NumStr);
                j:=pos(NumStr,TmpStr);
                Delete(TmpStr,1,j+length(NumStr));

                ci:=1;
                For mii := 1 to length(TmpStr)-1 do
                   if (TmpStr[mii] in ZnaciVert) and not(TmpStr[mii+1] in ZnaciVert) then
                     begin
                       WrdStr[ci]:=mii;
                       inc(ci);
                     end;
                WrdStr[ci]:=mii+1;

                xxstr:=copy(Tmpstr,1,wrdstr[1]);
                yystr:=copy(Tmpstr,wrdstr[1]+1,wrdstr[2]-wrdstr[1]);
                zzstr:=copy(Tmpstr,wrdstr[2]+1,wrdstr[3]-wrdstr[2]);
                val(xxstr,Vertices[i+1].x,codn);
                val(yystr,Vertices[i+1].z,codn);
                val(zzstr,Vertices[i+1].y,codn);
                Vertices[i+1].y:=-Vertices[i+1].y;
                readln(Fw3d, TheRest);
                inc(i);
                If i >= MaxVertex-1 then
                  begin
                    Str(MaxVertex, strConv);
                    strMSG := 'The total number of Vertices exceeded ' + strConv + '   ';
                    if DisplayMessage(strMSG, WINDOW_TITLE, MSG_ERROR) = IDOK then
                      begin
                        PostQuitMessage(0);
                        Halt(0);
                      end;
                  end;
              end;
          end;
        readln(Fw3d, TheRest);

        // This is for reading POLYGONS
        If pos('*MESH_FACE_LIST {',TheRest)<>0 then
          begin
            i:=0;
            readln(Fw3d, TheRest);
            PolyCountStart:=PolyCount+1;
            while pos('}',TheRest)=0 do
              begin
                wrdstr[1]:=pos('A:',TheRest);
                wrdstr[2]:=pos('B:',TheRest);
                wrdstr[3]:=pos('C:',TheRest);
                wrdstr[4]:=pos('AB:',TheRest);
                vxx[1]:=copy(TheRest,wrdstr[1]+2,(wrdstr[2]-1)-(wrdstr[1]+2));
                vxx[2]:=copy(TheRest,wrdstr[2]+2,(wrdstr[3]-1)-(wrdstr[2]+2));
                vxx[3]:=copy(TheRest,wrdstr[3]+2,(wrdstr[4]-1)-(wrdstr[3]+2));

                readln(Fw3d, TheRest);
                wrdstr[1]:=pos('A:',TheRest);
                wrdstr[2]:=pos('B:',TheRest);
                wrdstr[3]:=pos('C:',TheRest);
                wrdstr[4]:=pos('AB:',TheRest);
                vxx[5]:=copy(TheRest,wrdstr[1]+2,(wrdstr[2]-1)-(wrdstr[1]+2));
                vxx[6]:=copy(TheRest,wrdstr[2]+2,(wrdstr[3]-1)-(wrdstr[2]+2));
                vxx[7]:=copy(TheRest,wrdstr[3]+2,(wrdstr[4]-1)-(wrdstr[3]+2));

                if (vxx[5]<>vxx[1]) and (vxx[5]<>vxx[2]) and (vxx[5]<>vxx[3]) then
                    vxx[4]:=copy(TheRest,wrdstr[1]+2,(wrdstr[2]-1)-(wrdstr[1]+2))
                else
                  if (vxx[6]<>vxx[1]) and (vxx[6]<>vxx[2]) and (vxx[6]<>vxx[3]) then
                      vxx[4]:=copy(TheRest,wrdstr[2]+2,(wrdstr[3]-1)-(wrdstr[2]+2))
                  else
                    if (vxx[7]<>vxx[1]) and (vxx[7]<>vxx[2]) and (vxx[7]<>vxx[3]) then
                        vxx[4]:=copy(TheRest,wrdstr[3]+2,(wrdstr[4]-1)-(wrdstr[3]+2));

                val(vxx[1],vert[MaskA],codn);
                val(vxx[2],vert[MaskB],codn);
                val(vxx[3],vert[MaskC],codn);
                val(vxx[4],vert[MaskD],codn);

                Inc(PolyCount);
                If RotObject then
                  begin
                    Inc(RotObjs2Pol[RotObjectsCount]);
                    Rot2World[RotObjectsCount, RotObjs2Pol[RotObjectsCount]] := PolyCount;
                    RotObject2Center[RotObjectsCount] := RotCenterIndex;
                  end;

                If PolyCount>=Pol-1 then
                  begin
                    Str(Pol, strConv);
                    strMSG := 'Maximum number of ' + strConv + ' polygons exceeded !   ';
                    if DisplayMessage(strMSG, WINDOW_TITLE, MSG_ERROR) = IDOK then
                      begin
                        PostQuitMessage(0);
                        Halt(0);
                      end;
                  end;

                poligon[PolyCount].t1 := Vertices[vert[1]+1];
                poligon[PolyCount].t2 := Vertices[vert[2]+1];
                poligon[PolyCount].t3 := Vertices[vert[3]+1];
                poligon[PolyCount].t4 := Vertices[vert[4]+1];
                rotpoly[PolyCount] := poligon[PolyCount];
                MainPolyWorld[PolyCount] := poligon[PolyCount];

                readln(Fw3d, TheRest);
                inc(i);
              end;
            PolyCountEnd:=PolyCount;
          end;

        while pos('*PROP_CASTSHADOW',TheRest)=0 do
          readln(Fw3d, TheRest);

        // Check if this object casts a shadow
        If pos('*PROP_CASTSHADOW',TheRest)<>0 then
          begin
            TmpStr:=TheRest;
            j:=pos('W',TmpStr);
            Delete(TmpStr,1,j+1);
            val(TmpStr,CastCheck,codn);

            If CastCheck=1 then
             // Assign these polygons to cast a shadow
              for ii:=PolyCountStart to PolyCountEnd do
                 begin
                   inc(CastCount);
                   PolyNoMain[CastCount]:=ii;
                   CastShadowPoly[ii]:=true;
                 end;
          end;
        readln(Fw3d, TheRest);

        // Check if this object receieves a shadow
        If pos('*PROP_RECVSHADOW',TheRest)<>0 then
          begin
            TmpStr:=TheRest;
            j:=pos('W',TmpStr);
            Delete(TmpStr,1,j+1);
            val(TmpStr,CastCheck,codn);

            If CastCheck=1 then
             // Assign these polygons to receieve a shadow
              for ii:=PolyCountStart to PolyCountEnd do
                 RecvShadowPoly[ii]:=true;
          end;
        readln(Fw3d, TheRest);

        // Find which Material is associated with this object
        If pos('*MATERIAL_REF',TheRest)<>0 then
          begin
            TmpStr:=TheRest;
            j:=pos('F',TmpStr);
            Delete(TmpStr,1,j+1);
            val(TmpStr,MaterialID,codn);

            // Associate the Material to the object
            for ii:=PolyCountStart to PolyCountEnd do
               begin
                 pol2tex[ii]:=MaterialID+1;
                 alphap[ii]:=AlphaValue[MaterialID+1];
                 If AlphaValue[MaterialID+1]>0 then
                   doublesided[ii]:=true;
               end;
          end
        else
          // Associate the Material to the object
          for ii:=PolyCountStart to PolyCountEnd do
             pol2tex[ii] := MaterialCount + 1;
      End;

  {*****************************************************************************
   ** Load Fixed Light Data ****************************************************
   *****************************************************************************}
    If pos('*LIGHTOBJECT {',TheRest)<>0 then
      Begin
        readln(Fw3d, TheRest);
        If pos('*NODE_NAME',TheRest) <> 0 then
          if pos('"mr Area Omni',TheRest) = 0 then
           if pos('"RotCenter',TheRest) <> 0 then
             begin
              Inc(RotCenterCount);
              while pos('*TM_POS',TheRest)=0 do
                readln(Fw3d, TheRest);

              If pos('*TM_POS',TheRest)<>0 then
                begin
                  TmpStr:=TheRest;
                  j:=pos('S',TmpStr);
                  Delete(TmpStr,1,j+1);

                  ci:=1;
                  For mii:=1 to length(TmpStr)-1 do
                     If (TmpStr[mii] in ZnaciVert) and not(TmpStr[mii+1] in ZnaciVert) then
                       begin
                         WrdStr[ci]:=mii;
                         inc(ci);
                       end;
                  WrdStr[ci]:=mii+1;

                  xxstr:=copy(Tmpstr,1,wrdstr[1]);
                  yystr:=copy(Tmpstr,wrdstr[1]+1,wrdstr[2]-wrdstr[1]);
                  zzstr:=copy(Tmpstr,wrdstr[2]+1,wrdstr[3]-wrdstr[2]);
                  val(xxstr,RotCenter[RotCenterCount].x,codn);
                  val(yystr,RotCenter[RotCenterCount].z,codn);
                  val(zzstr,RotCenter[RotCenterCount].y,codn);
                  RotCenter[RotCenterCount].y := -RotCenter[RotCenterCount].y;
                end;
             end
           else
            begin
              inc(LightCount);
              while pos('*TM_POS',TheRest)=0 do
                readln(Fw3d, TheRest);

              If pos('*TM_POS',TheRest)<>0 then
                begin
                  TmpStr:=TheRest;
                  j:=pos('S',TmpStr);
                  Delete(TmpStr,1,j+1);

                  ci:=1;
                  For mii:=1 to length(TmpStr)-1 do
                     If (TmpStr[mii] in ZnaciVert) and not(TmpStr[mii+1] in ZnaciVert) then
                       begin
                         WrdStr[ci]:=mii;
                         inc(ci);
                       end;
                  WrdStr[ci]:=mii+1;

                  xxstr:=copy(Tmpstr,1,wrdstr[1]);
                  yystr:=copy(Tmpstr,wrdstr[1]+1,wrdstr[2]-wrdstr[1]);
                  zzstr:=copy(Tmpstr,wrdstr[2]+1,wrdstr[3]-wrdstr[2]);
                  val(xxstr,LightVertex[LightCount].x,codn);
                  val(yystr,LightVertex[LightCount].z,codn);
                  val(zzstr,LightVertex[LightCount].y,codn);
                  LightVertex[LightCount].y:=-LightVertex[LightCount].y;
                  LightVertexTmp[LightCount]:=LightVertex[LightCount];
                end;

              while pos('*LIGHT_SHADOWS',TheRest)=0 do
                readln(Fw3d, TheRest);

              If pos('*LIGHT_SHADOWS',TheRest) <> 0 then
                if pos('Off', TheRest) <> 0 then
                  LightShadow[LightCount] := 0
                else
                  if pos('Mapped',TheRest) <> 0 then
                    LightShadow[LightCount] := 1
                  else
                    if pos('Raytraced',TheRest) <> 0 then
                      LightShadow[LightCount] := 2;

              while pos('*LIGHT_COLOR',TheRest)=0 do
                readln(Fw3d, TheRest);

              If pos('*LIGHT_COLOR',TheRest)<>0 then
                begin
                  TmpStr:=TheRest;
                  j:=pos('R',TmpStr);
                  Delete(TmpStr,1,j+1);

                  ci:=1;
                  For mii:=1 to length(TmpStr)-1 do
                     If (TmpStr[mii] in ZnaciVert) and not(TmpStr[mii+1] in ZnaciVert) then
                       begin
                         WrdStr[ci]:=mii;
                         inc(ci);
                       end;
                  WrdStr[ci]:=mii+1;

                  xxstr:=copy(Tmpstr,1,wrdstr[1]);
                  yystr:=copy(Tmpstr,wrdstr[1]+1,wrdstr[2]-wrdstr[1]);
                  zzstr:=copy(Tmpstr,wrdstr[2]+1,wrdstr[3]-wrdstr[2]);
                  val(xxstr,LRR,codn);
                  val(yystr,LGG,codn);
                  val(zzstr,LBB,codn);
                  LightR[LightCount]:=round(LRR*255);
                  LightG[LightCount]:=round(LGG*255);
                  LightB[LightCount]:=round(LBB*255);

                  readln(Fw3d, TheRest);
                  If pos('*LIGHT_INTENS',TheRest)<>0 then
                    begin
                      TmpStr:=TheRest;
                      j:=pos('S',TmpStr);
                      Delete(TmpStr,1,j+1);

                      For mii:=1 to length(TmpStr)-1 do
                         If (TmpStr[mii] in ZnaciVert) and not(TmpStr[mii+1] in ZnaciVert) then
                           WrdStr[1]:=mii;
                      xxstr:=copy(Tmpstr,1,wrdstr[1]);
                      val(xxstr,LightIntens,codn);
                      LightIntensity[LightCount]:=round(LightIntens) * 1500;
                    end;
                end;
            end
          else
            begin
  {****************************************************************************
   * Load Sphere Zone Data ****************************************************
   ****************************************************************************}
              Inc(SphereZonesCount);
              while pos('*TM_POS',TheRest)=0 do
                readln(Fw3d, TheRest);

              If pos('*TM_POS',TheRest)<>0 then
                begin
                  TmpStr:=TheRest;
                  j:=pos('S',TmpStr);
                  Delete(TmpStr,1,j+1);

                  ci:=1;
                  For mii:=1 to length(TmpStr)-1 do
                     If (TmpStr[mii] in ZnaciVert) and not(TmpStr[mii+1] in ZnaciVert) then
                       begin
                         WrdStr[ci]:=mii;
                         inc(ci);
                       end;
                  WrdStr[ci]:=mii+1;

                  xxstr:=copy(Tmpstr,1,wrdstr[1]);
                  yystr:=copy(Tmpstr,wrdstr[1]+1,wrdstr[2]-wrdstr[1]);
                  zzstr:=copy(Tmpstr,wrdstr[2]+1,wrdstr[3]-wrdstr[2]);
                  val(xxstr,Sphere_Zone_Center[SphereZonesCount].x,codn);
                  val(yystr,Sphere_Zone_Center[SphereZonesCount].z,codn);
                  val(zzstr,Sphere_Zone_Center[SphereZonesCount].y,codn);
                  Sphere_Zone_Center[SphereZonesCount].y := -Sphere_Zone_Center[SphereZonesCount].y;
                  Sphere_Zone_Center_Tmp[SphereZonesCount] := Sphere_Zone_Center[SphereZonesCount];
                end;

              while (pos('*LIGHT_ATTNEND',TheRest) = 0) do
                readln(Fw3d, TheRest);

              If pos('*LIGHT_ATTNEND',TheRest)<>0 then
                begin
                  TmpStr:=TheRest;
                  j:=pos('D',TmpStr);
                  Delete(TmpStr,1,j+1);

                  For mii:=1 to length(TmpStr)-1 do
                     If (TmpStr[mii] in ZnaciVert) and not(TmpStr[mii+1] in ZnaciVert) then
                       WrdStr[1]:=mii;
                  xxstr:=copy(Tmpstr,1,wrdstr[1]);
                  val(xxstr,ZoneRadius,codn);
                  Sphere_Zone_Radius[SphereZonesCount] := ZoneRadius;
                end;
            end;
      End;

    // Move to the next line in the file
    ReadLn(Fw3d, TheRest);
  End;

 --- Set the start and end for shadow casting polygons -----------------------
  ShadowStart := 1;
  ShadowEnd := CastCount;
  PotentialShadowPoly[1] := ShadowStart;

 --- Associate a texture for the Lens Flare ----------------------------------
  LightLens := Light_Corona_texture;

  Close(Fw3d);
  FileMode := 2;  // Return the default value - READ/WRITE
END;

{This PROCEDURE rotates the X and Y axis by 90 degrees counter clockwise,
 to correspond to the main axis in the input file.
 It is something i should fix becouse the main rotation is
 different from what i need, but LATER}
Procedure FixScene(PolyCount,LightCount,ZoneCo:Integer);
var
    anglex,angley,anglez,cx,cy,cz,sx,sy,sz:single;
    iii:Integer;
BEGIN
  // Calculate the rotation angle and sin,cos for that angle
  anglex:=90*pi/180;
  angley:=90*pi/180;
  anglez:=0;
  cx:=sin(anglex);
  sx:=cos(anglex);
  cy:=sin(angley);
  sy:=cos(angley);
  cz:=sin(anglez);
  sz:=cos(anglez);

  // Rotate the world vertices according to the calculated angle
  for iii:=1 to PolyCount do
  begin
    rottoccka(poligon[iii].t1, poligon[iii].t1,cx,sx,cy,sy,cz,sz);
    rottoccka(poligon[iii].t2, poligon[iii].t2,cx,sx,cy,sy,cz,sz);
    rottoccka(poligon[iii].t3, poligon[iii].t3,cx,sx,cy,sy,cz,sz);
    rottoccka(poligon[iii].t4, poligon[iii].t4,cx,sx,cy,sy,cz,sz);
  end;
  for iii:=1 to LightCount do
     rottoccka(LightVertex[iii], LightVertex[iii],cx,sx,cy,sy,cz,sz);
  for iii:=1 to ZoneCo do
     rottoccka(Sphere_Zone_Center[iii], Sphere_Zone_Center[iii],cx,sx,cy,sy,cz,sz);
END;

{This PROCEDURE SETS UP the 3-D WORLD scene.
 It loads the 3-D World, then it transforms the coordinates according to the
 player's position}
Procedure SceneDime;
begin
  // Load the entire 3-D World
  Inc(LoadingText_Counter);
  WindowText('Loading 3-D World...', 10, LoadingText_Counter * 16, 255, 255, 0);
  Write2Log('Loading 3-D World...  ', false);

  Load_3DWorld(WorldFile, polmax, litmax, ZoneCount, RotObjectsMax);

  WindowText('OK', 160, LoadingText_Counter * 16, 0, 255, 0);
  Write2Log('OK', true);

  // Rotate the world to correspond to the main axis
  Inc(LoadingText_Counter);
  WindowText('Setup 3-D Data...', 10, LoadingText_Counter * 16, 255, 255, 0);
  Write2Log('Setup 3-D Data...  ', false);

  FixScene(polmax,litmax,ZoneCount);

  // Put the Final 3-D World data in to general working variables
  for pa:=1 to polmax do
     mainpoly[pa] := poligon[pa];
  for pa:=1 to litmax do
     mainlightv[pa] := LightVertex[pa];
  for pa:=1 to ZoneCount do
     Main_Zone_Center[pa] := Sphere_Zone_Center[pa];

  WindowText('OK', 140, LoadingText_Counter * 16, 0, 255, 0);
  Write2Log('OK', true);
end;

END.