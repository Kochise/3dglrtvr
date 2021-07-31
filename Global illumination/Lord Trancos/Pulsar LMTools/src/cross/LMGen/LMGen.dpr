// ---------------------------------------------------------------------------
// Pulsar LMTools v1.1
// Copyright (C) 2002, 2003 Lord Trancos. All rights reserved.
// ---------------------------------------------------------------------------
// 
// This file is part of Pulsar LMTools
// 
// Pulsar LMTools is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or any later version.
//
// Pulsar LMTools is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details (license.txt).
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// You should also be able to get a copy of the license here:
//
//   http://www.gnu.org/licenses/gpl.txt
//
// ---------------------------------------------------------------------------
//
// Developed by Lord Trancos <lordtrancos@softhome.net>
//
// ---------------------------------------------------------------------------
//
// DXLab - Visit us at: http://www.dxlab.tk or http://dxlab.host.sk
//
// ---------------------------------------------------------------------------

// LMGen -  Lightmap Generation Utility

// The planar mapping function (GetPlanarMapping) it's based on the lightmap
// tutorial and VC++ sample written by Tobias Johansson.
//    http://come.to/polygone
//    http://home.bip.net/tobias.johansson1/tut_lightmap.htm

program LMGen;

// ---------------------------------------------------------------------------

{$apptype console}

uses SysUtils,
  sse in '../sse.pas',
  lmts in '../lmts.pas',
  filez in '../filez.pas',
  vTypes in '../vTypes.pas',
  MathVect in '../MathVect.pas',
  MathTri in '../MathTri.pas',
  Octree in '../Octree.pas',
  aabb_tri in '../aabb_tri.pas',
  argb in '../argb.pas',
  tga in '../tga.pas',
  bitmap in '../bitmap.pas',
  RGBColor in '../RGBColor.pas',
  lmdb in '../lmdb.pas',
  point_t in '../point_t.pas',
  point_te in '../point_te.pas',
  point_ls in '../point_ls.pas',
  aabb_ls in '../aabb_ls.pas',
  global in 'global.pas',
  scrlang in 'scrlang.pas',
  lights in 'lights.pas',
  shadowc in 'shadowc.pas';

// ---------------------------------------------------------------------------

procedure ShowAppTitle;

begin
  Writeln(APPTITLE);
end;

// ---------------------------------------------------------------------------

procedure SafeHalt(_exitCode: integer);

begin
  // close log file if it's open
  if g_LogFile_IsOpen then CloseFile(g_LogFile);

  // call halt
  Halt(_exitCode);
end;

// ---------------------------------------------------------------------------

procedure CheckParams;

var
  _s : string;
  _cnt : longint;

begin
  // check parameters
  if paramcount = 0 then
  begin
    Writeln(USAGE);
    SafeHalt(1);
  end;

  // first parameter will be uses as filename base
  g_FileNameBase := paramstr(1);

  // process second parameter
  if paramcount > 1 then
  begin
    _s := UpperCase(paramstr(2));
    for _cnt := 1 to length(_s) do
    case _s[_cnt] of
      'A' : begin
              g_Use_Alpha_Channel := not C_DEFAULT_USE_ALPHA;
              Writeln(C_PARAMMSG00);
            end;
      'F' : begin
              g_Filter_LightMaps := not C_DEFAULT_FILTER_LMAPS;
              Writeln(C_PARAMMSG01);
            end;
      'H' : begin
              g_Soft_Shadows := not C_DEFAULT_SOFT_SHADOWS;
              Writeln(C_PARAMMSG02);
            end;
(*
      'L' : begin
              g_Log_Warnings := not C_DEFAULT_LOG_WARNINGS;
              Writeln(C_PARAMMSG04);
            end;
*)
      'S' : begin
              g_Cast_Shadows := not C_DEFAULT_CAST_SHADOWS;
              Writeln(C_PARAMMSG03);
            end;
      else Writeln(ERR09 + _s[_cnt]);
    end;
    Writeln;
  end;
end;

// ---------------------------------------------------------------------------

procedure LogMsg(_txt: string);

begin
  // write to the console
  Writeln(_txt);

  // must log ?
  if not g_Log_Warnings then exit;

  // if isn't open, open it
  if not g_LogFile_IsOpen then
  begin
    {$I-}
    AssignFile(g_LogFile, g_FileNameBase + C_EXT_LMLOG);
    Rewrite(g_LogFile);
    {$I+}
    g_LogFile_IsOpen := (IOResult = 0);
    if (not g_LogFile_IsOpen) then // cannot open log file
    begin
      Writeln(ERR10);
      g_Log_Warnings := false; // disable logging
    end;
  end;

  // write log msg
  Writeln(g_LogFile, _txt);
end;

// ---------------------------------------------------------------------------

function RunScript: boolean;

var
  _fn : string;
  _f : Text;

  // -----------------------------

  function ProcessScript: boolean;

  var
    _line : longint;
    _s, _err : string;

  begin
    Result := false;

    // process line by line
    _line := 1;
    while not eof(_f) do
    begin
      // read line
      Readln(_f, _s);
      // process
      _err := scr_Run(g_ScriptLang, _s);
      // any error ?
      if _err <> '' then
      begin
        Writeln(Format(ERR04, [_line, _err]));
        exit;
      end;
      // next
      inc(_line);
    end;

    // all right
    Result := true;
  end;

  // -----------------------------

begin
  Result := false;

  // open script file
  {$I-}
  _fn := g_FileNameBase + C_EXT_SCRIPT;
  AssignFile(_f, _fn);
  Reset(_f);
  {$I+}
  if (IOResult <> 0) then
  begin
    Writeln(Format(ERR03, [_fn]));
    exit;
  end;

  // process script
  Result := ProcessScript;

  // close script file
  CloseFile(_f);
end;

// ---------------------------------------------------------------------------

procedure Load3Dfile;

var
  _fn : string;
  _cnt : longint;  

begin
  // load 3d file
  _fn := g_FileNameBase + '.1' + C_EXT_3DFILE;
  g_LMTS := LMTS_Load(_fn);
  if not g_LMTS.ok then
  begin
    Writeln(Format(ERR05, [_fn]));
    SafeHalt(1);
  end;
  Writeln(Format(TXT03, [g_LMTS.header.nTris, g_LMTS.header.nSubsets]));

  // create some buffers
  try
    SetLength(g_DisabledSubsets, g_LMTS.header.nSubsets);
    for _cnt := 0 to g_LMTS.header.nSubsets - 1
      do g_DisabledSubsets[_cnt] := false;
    SetLength(g_TriData, g_LMTS.header.nTris); // buffer for calc. triangle data
  except
    Writeln(ERR06);
    SafeHalt(1);
  end;
end;

// ---------------------------------------------------------------------------

procedure DrawLightmap(_triIdx: word; _lm: TARGB;
                       _uv00, _uvEdge1, _uvEdge2, _normal: P_Vec);

type
  T5Colors = array[0..4] of cardinal;

var
  _x, _y, _z : longint;
  _lp : T_Vec;
  _color, _cT, _sC : cardinal;
  _soft : T5Colors;
  _cursor : ^cardinal;
  _v : T_Vec;

  // ----------------------------

  function GetPoint(_sx, _sy : single): T_Vec;

  var _u, _v : single;

  begin
    _u := _sx / _lm.header.width;
    _v := _sy / _lm.header.height;

    Result[X] := _uv00^[X] + _uvEdge1^[X] * _u + _uvEdge2^[X] * _v;
    Result[Y] := _uv00^[Y] + _uvEdge1^[Y] * _u + _uvEdge2^[Y] * _v;
    Result[Z] := _uv00^[Z] + _uvEdge1^[Z] * _u + _uvEdge2^[Z] * _v;
  end;

  // ----------------------------

  function Merge5(const _c: T5Colors): cardinal;

  var
    _r, _g, _b : cardinal;
    _cnt : byte;

  begin
    // red
    _r := 0;
    for _cnt := 0 to 4 do _r := _r + ((_c[_cnt] shr 16) and $FF);
    _r := _r div 5;

    // green
    _g := 0;
    for _cnt := 0 to 4 do _g := _g + ((_c[_cnt] shr 8) and $FF);
    _g := _g div 5;

    // blue
    _b := 0;
    for _cnt := 0 to 4 do _b := _b + (_c[_cnt] and $FF);
    _b := _b div 5;

    // ok
    Result := (_r shl 16) + (_g shl 8) + _b;
  end;

  // ----------------------------

begin
  // if soft shadows are enabled we need extra calculations.
  // trace rays from lumel's corners.
  if g_Cast_Shadows and g_Soft_Shadows then
    for _y := 0 to _lm.header.height do
      for _x := 0 to _lm.header.width  do
      begin
        // get lumel position
        _lp := GetPoint(_x, _y);

        // process directional lights
        if length(gADirectLight) > 0 then
          for _z := 0 to length(gADirectLight)-1 do
            if (gADirectLight[_z].Enabled) and (gADirectLight[_z].Ena4Tri) and
               (gADirectLight[_z].CastShadows) then
            begin
              _v := vectMulS(@gADirectLight[_z].InvDir, g_MaxRayLength);
              _v := vectAddV(@_lp, @_v);
              _cT := ShadowCast(_triIdx, @_v, @_lp);
              gADirectLight[_z].SCCorners[(_y * g_LMap_Max_Size) + _x] := _cT;
            end;

        // process omni point lights
        if length(gAOmni_Point) > 0 then
          for _z := 0 to length(gAOmni_Point)-1 do
            if (gAOmni_Point[_z].Enabled) and (gAOmni_Point[_z].Ena4Tri) and
               (gAOmni_Point[_z].CastShadows) then
            begin
              _cT := ShadowCast(_triIdx, @gAOmni_Point[_z].Position, @_lp);
              gAOmni_Point[_z].SCCorners[(_y * g_LMap_Max_Size) + _x] := _cT;
            end;

        // process spotlights
        if length(gASpotLight) > 0 then
          for _z := 0 to length(gASpotLight)-1 do
            if (gASpotLight[_z].Enabled) and (gASpotLight[_z].Ena4Tri) and
               (gASpotLight[_z].CastShadows) then
            begin
              _cT := ShadowCast(_triIdx, @gASpotLight[_z].Position, @_lp);
              gASpotLight[_z].SCCorners[(_y * g_LMap_Max_Size) + _x] := _cT;
            end;
      end;

  // compute lights
  _cursor := _lm.data;
  for _y := 0 to _lm.header.height - 1 do
    for _x := 0 to _lm.header.width - 1 do
    begin
      // get lumel position
      _lp := GetPoint(_x + 0.5, _y + 0.5);

      // get initial color value
      _color := gAmbient_Global.Color;

      // process directional lights
      if length(gADirectLight) > 0 then
        for _z := 0 to length(gADirectLight)-1 do
          if (gADirectLight[_z].Enabled) and (gADirectLight[_z].Ena4Tri) then
          begin
            if _color = $FFFFFF then break;

            // shadow cast
            if (g_Cast_Shadows) and (gADirectLight[_z].CastShadows) then
            begin
              _v := vectMulS(@gADirectLight[_z].InvDir, g_MaxRayLength);
              _v := vectAddV(@_lp, @_v);
              _sC := ShadowCast(_triIdx, @_v, @_lp);

              if g_Soft_Shadows then
              begin
                _soft[0] := _sC;
                _soft[1] := gADirectLight[_z].SCCorners[(_y * g_LMap_Max_Size) + _x];
                _soft[2] := gADirectLight[_z].SCCorners[(_y * g_LMap_Max_Size) + _x + 1];
                _soft[3] := gADirectLight[_z].SCCorners[((_y+1) * g_LMap_Max_Size) + _x];
                _soft[4] := gADirectLight[_z].SCCorners[((_y+1) * g_LMap_Max_Size) + _x + 1];
                _sC := Merge5(_soft);
              end;
            end else _sC := $FFFFFF;

            if _sC <> 0 then
            begin
              // light color
              _cT := lit_DirectLight(_normal, @gADirectLight[_z]);

              // filter light
              _sC := FilterRGB8Color(_cT, _sC);

              // add color value
              _color := SumRGB8Colors(_color, _sC);
            end;
          end;

      // process omni point lights
      if length(gAOmni_Point) > 0 then
        for _z := 0 to length(gAOmni_Point)-1 do
          if (gAOmni_Point[_z].Enabled) and (gAOmni_Point[_z].Ena4Tri) then
          begin
            if _color = $FFFFFF then break;

            // shadow cast
            if (g_Cast_Shadows) and (gAOmni_Point[_z].CastShadows) then
            begin
              _sC := ShadowCast(_triIdx, @gAOmni_Point[_z].Position, @_lp);

              if g_Soft_Shadows then
              begin
                _soft[0] := _sC;
                _soft[1] := gAOmni_Point[_z].SCCorners[(_y * g_LMap_Max_Size) + _x];
                _soft[2] := gAOmni_Point[_z].SCCorners[(_y * g_LMap_Max_Size) + _x + 1];
                _soft[3] := gAOmni_Point[_z].SCCorners[((_y+1) * g_LMap_Max_Size) + _x];
                _soft[4] := gAOmni_Point[_z].SCCorners[((_y+1) * g_LMap_Max_Size) + _x + 1];
                _sC := Merge5(_soft);
              end;
            end else _sC := $FFFFFF;

            if _sC <> 0 then
            begin
              // light color
              _cT := lit_Omni_Point(@_lp, _normal, @gAOmni_Point[_z]);

              // filter light
              _sC := FilterRGB8Color(_cT, _sC);

              // add color value
              _color := SumRGB8Colors(_color, _sC);
            end;
          end;

      // process spotlights
      if length(gASpotLight) > 0 then
        for _z := 0 to length(gASpotLight)-1 do
          if (gASpotLight[_z].Enabled) and (gASpotLight[_z].Ena4Tri) then
          begin
            if _color = $FFFFFF then break;

            // shadow cast
            if (g_Cast_Shadows) and (gASpotLight[_z].CastShadows) then
            begin
              _sC := ShadowCast(_triIdx, @gASpotLight[_z].Position, @_lp);

              if g_Soft_Shadows then
              begin
                _soft[0] := _sC;
                _soft[1] := gASpotLight[_z].SCCorners[(_y * g_LMap_Max_Size) + _x];
                _soft[2] := gASpotLight[_z].SCCorners[(_y * g_LMap_Max_Size) + _x + 1];
                _soft[3] := gASpotLight[_z].SCCorners[((_y+1) * g_LMap_Max_Size) + _x];
                _soft[4] := gASpotLight[_z].SCCorners[((_y+1) * g_LMap_Max_Size) + _x + 1];
                _sC := Merge5(_soft);
              end;
            end else _sC := $FFFFFF;

            if _sC <> 0 then
            begin
              // light color
              _cT := lit_SpotLight(@_lp, _normal, @gASpotLight[_z]);

              // filter light
              _sC := FilterRGB8Color(_cT, _sC);

              // add color value
              _color := SumRGB8Colors(_color, _sC);
            end;
          end;

      // Store final color.
      _cursor^ := ($FF000000 or _color);

      // Next lumel
      inc(_cursor);
    end;
end;

// ---------------------------------------------------------------------------

procedure SaveLightmap(var _lm: TARGB);

begin
  if not ARGB_SaveToFile(g_LMDB, _lm) then
  begin
    Writeln(Format(ERR07, [g_LMDB_FN]));
    SafeHalt(1);
  end;
end;

// ---------------------------------------------------------------------------

procedure FilterLightmap(_lm: TARGB);

type
  PA8R8G8B8 = ^TA8R8G8B8;
  TA8R8G8B8 = array[0..3] of byte;

var
  _x, _y : longint;
  _cur : ^cardinal;
  _col : array[0..8] of PA8R8G8B8;

  // ---------------------------------------

  function GetXY(_i, _j: longint): PA8R8G8B8;

  begin
    Result := g_TempARGB.data;
    inc(Result, (_j * g_TempARGB.header.width) + _i);
  end;

  // ---------------------------------------

  function Average3x3: cardinal;

  var
    _minX, _minY, _maxX, _maxY : longint;
    _cnt, _a, _r, _g, _b : longint;

  begin
    if _x > 0 then _minX := _x - 1 else _minX := _x;
    if _y > 0 then _minY := _y - 1 else _minY := _y;

    if _x < _lm.header.width-1 then _maxX := _x + 1 else _maxX := _x;
    if _y < _lm.header.height-1 then _maxY := _y + 1 else _maxY := _y;

    _col[0] := GetXY(_minX, _minY);
    _col[1] := GetXY(   _x, _minY);
    _col[2] := GetXY(_maxX, _minY);

    _col[3] := GetXY(_minX, _y);
    _col[4] := GetXY(   _x, _y);
    _col[5] := GetXY(_maxX, _y);

    _col[6] := GetXY(_minX, _maxY);
    _col[7] := GetXY(   _x, _maxY);
    _col[8] := GetXY(_maxX, _maxY);

    _a := 0;
    for _cnt := 0 to 8 do _a := _a + _col[_cnt][3];
    _a := _a div 9;

    _r := 0;
    for _cnt := 0 to 8 do _r := _r + _col[_cnt][2];
    _r := _r div 9;

    _g := 0;
    for _cnt := 0 to 8 do _g := _g + _col[_cnt][1];
    _g := _g div 9;

    _b := 0;
    for _cnt := 0 to 8 do _b := _b + _col[_cnt][0];
    _b := _b div 9;

    Result := (_a shl 24) + (_r shl 16) + (_g shl 8) + _b;
  end;

  // ---------------------------------------

begin
  _cur := _lm.data;
  for _y := 0 to _lm.header.height-1 do
    for _x := 0 to _lm.header.width-1 do
    begin
      cardinal(_cur^) := Average3x3;
      // next pixel
      inc(_cur);
    end;
end;

// ---------------------------------------------------------------------------

procedure GetPlanarMapping(Const _triIdx: word;
                           var _uv00, _uvE1, _uvE2: T_Vec;
                           var _dU, _dV: single);

// this function it's based on the lightmap tutorial and VC++ sample
// written by Tobias Johansson.

//    http://come.to/polygone
//    http://home.bip.net/tobias.johansson1/tut_lightmap.htm

var
  _vec1, _vec2 : T_Vec;
  _nlc : TXYZ;
  _minU, _minV, _maxU, _maxV, _s, _dist, _val : single;

  // ----------------------------

  procedure getUV1(_p : P_Vec; var _u, _v : single);

  begin
    case _nlc of
      vectX : begin _u := _p^[Z]; _v := _p^[Y]; end;
      vectY : begin _u := _p^[X]; _v := _p^[Z]; end;
      vectZ : begin _u := _p^[X]; _v := _p^[Y]; end;
    end;
  end;

  // ----------------------------

  procedure getUV2(var _u, _v: single);

  begin
    _u := _u - _minU;
    _v := _v - _minV;
    _u := _u / _dU;
    _v := _v / _dV;
  end;

  // ----------------------------


begin
  with g_TriData[_triIdx] do
  begin
    // Get normal's largest component.
    _nlc := vectLargestComponent(@n);

    // Get "2D" bounding rect
    getUV1(@a^, a^.u2, a^.v2);
    getUV1(@b^, b^.u2, b^.v2);
    getUV1(@c^, c^.u2, c^.v2);

    _minU := a^.u2;
    _minV := a^.v2;
    _maxU := a^.u2;
    _maxV := a^.v2;

    if (b^.u2 < _minU) then _minU := b^.u2;
    if (b^.v2 < _minV) then _minV := b^.v2;
    if (b^.u2 > _maxU) then _maxU := b^.u2;
    if (b^.v2 > _maxV) then _maxV := b^.v2;

    if (c^.u2 < _minU) then _minU := c^.u2;
    if (c^.v2 < _minV) then _minV := c^.v2;
    if (c^.u2 > _maxU) then _maxU := c^.u2;
    if (c^.v2 > _maxV) then _maxV := c^.v2;

    // Adjust bounding rect to the "axis algined grid"
    _s := Int(_minU / g_LMap_Lumel_Size) * g_LMap_Lumel_Size;
    if _s > _minU then _minU := _s - g_LMap_Lumel_Size else _minU := _s;
    _s := Int(_minV / g_LMap_Lumel_Size) * g_LMap_Lumel_Size;
    if _s > _minV then _minV := _s - g_LMap_Lumel_Size else _minV := _s;

    _s := Int(_maxU / g_LMap_Lumel_Size) * g_LMap_Lumel_Size;
    if _s < _maxU then _maxU := _s + g_LMap_Lumel_Size else _maxU := _s;
    _s := Int(_maxV / g_LMap_Lumel_Size) * g_LMap_Lumel_Size;
    if _s < _maxV then _maxV := _s + g_LMap_Lumel_Size else _maxV := _s;

    // Add border
    _minU := _minU - g_LMap_Lumel_Size;
    _minV := _minV - g_LMap_Lumel_Size;
    _maxU := _maxU + g_LMap_Lumel_Size;
    _maxV := _maxV + g_LMap_Lumel_Size;

    // Get dimensions of the "2D" bounding rect
    _dU := _maxU - _minU;
    _dV := _maxV - _minV;

    // Get lightmap UV coordinates (values from 0 to 1)
    getUV2(a^.u2, a^.v2);
    getUV2(b^.u2, b^.v2);
    getUV2(c^.u2, c^.v2);

    // --
    _dist := - vectDot(@n, @a^);
    case _nlc of
      vectX : begin
        _val  := - (n[Z] * _minU + n[Y] * _minV + _dist) / n[X];
        _uv00 := vect(_val, _minV, _minU);

        _val  := - (n[Z] * _maxU + n[Y] * _minV + _dist) / n[X];
        _vec1 := vect(_val, _minV, _maxU);

        _val  := - (n[Z] * _minU + n[Y] * _maxV + _dist) / n[X];
        _vec2 := vect(_val, _maxV, _minU);
      end;

      vectY : begin
        _val  := - (n[X] * _minU + n[Z] * _minV + _dist) / n[Y];
        _uv00 := vect(_minU, _val, _minV);

        _val  := - (n[X] * _maxU + n[Z] * _minV + _dist) / n[Y];
        _vec1 := vect(_maxU, _val, _minV);

        _val  := - (n[X] * _minU + n[Z] * _maxV + _dist) / n[Y];
        _vec2 := vect(_minU, _val, _maxV);
      end;

      vectZ : begin
        _val  := - (n[X] * _minU + n[Y] * _minV + _dist) / n[Z];
        _uv00 := vect(_minU, _minV, _val);

        _val  := - (n[X] * _maxU + n[Y] * _minV + _dist) / n[Z];
        _vec1 := vect(_maxU, _minV, _val);

        _val  := - (n[X] * _minU + n[Y] * _maxV + _dist) / n[Z];
        _vec2 := vect(_minU, _maxV, _val);
      end;
    end;

    _uvE1 := vectSubV(@_vec1, @_uv00);
    _uvE2 := vectSubV(@_vec2, @_uv00);
  end;
end;

// ---------------------------------------------------------------------------

procedure CreateLightmap(_triIdx: word);

var
  _resX, _resY : longint;
  _lm : TARGB;

begin
  with g_TriData[_triIdx] do
  begin
    // get lightmap dimensions
    _resX := round(dU / g_LMap_Lumel_Size);
    _resY := round(dV / g_LMap_Lumel_Size);

    // check dimensions
    if (_resX > g_LMap_Max_Size) or (_resY > g_LMap_Max_Size) then
    begin
      LogMsg(Format(TXT07, [_triIdx]));
      if (_resX > g_LMap_Max_Size) then _resX := g_LMap_Max_Size;
      if (_resY > g_LMap_Max_Size) then _resY := g_LMap_Max_Size;
    end;

    // create lightmap
    _lm := ARGB_Create(_resX, _resY);
    if not _lm.ok then
    begin
      Writeln(ERR06);
      SafeHalt(1);
    end;

    // draw lightmap (if needed)
    if vectSame(@uv00, @g_LastLM.uv00) and
       vectSame(@uvE1, @g_LastLM.uvEdge1) and
       vectSame(@uvE2, @g_LastLM.uvEdge2) then
    begin
      // copy instead of draw :)
      Move(g_LastLM.lm.data^, _lm.data^,
           4 * _lm.header.width * _lm.header.height);
    end else DrawLightmap(_triIdx,_lm, @uv00, @uvE1, @uvE2, @n);

    // save lightmap as last lightmap
    ARGB_Free(g_LastLM.lm);
    g_LastLM.uv00    := uv00;
    g_LastLM.uvEdge1 := uvE1;
    g_LastLM.uvEdge2 := uvE2;
    g_LastLM.lm := ARGB_Create(_resX, _resY);
    if not _lm.ok then
    begin
      Writeln(ERR06);
      SafeHalt(1);
    end;
    Move(_lm.data^, g_LastLM.lm.data^, 4 * _lm.header.width * _lm.header.height);

    // filter lightmap
    if g_Filter_LightMaps then
    begin
      // first copy the lightmap on another buffer
      ARGB_Draw(_lm, g_TempARGB, 0, 0);
      // filter
      FilterLightmap(_lm);
    end;

    // save lightmap to file
    SaveLightmap(_lm);

    // free lightmap
    ARGB_Free(_lm);
  end;
end;

// ---------------------------------------------------------------------------

procedure DisableAllLights;

var
  _cnt : longint;

begin
  if length(gADirectLight) > 0 then
    for _cnt := 0 to length(gADirectLight)-1
    do gADirectLight[_cnt].Ena4Tri := false;

  if length(gAOmni_Point) > 0 then
    for _cnt := 0 to length(gAOmni_Point)-1
    do gAOmni_Point[_cnt].Ena4Tri := false;

  if length(gASpotLight) > 0 then
    for _cnt := 0 to length(gASpotLight)-1
    do gASpotLight[_cnt].Ena4Tri := false;
end;

// ---------------------------------------------------------------------------

procedure EnableLightsForTri(_triIdx: longint);

// before calling this function we should call DisableAllLights

var
  _cnt : longint;
  _a, _b, _c : P_Vec;
  _e : single;
  _closest, _v : T_Vec;

begin
  with g_TriData[_triIdx] do
  begin
    _a := P_Vec(a);
    _b := P_Vec(b);
    _c := P_Vec(c);

    // enable/disable directional lights
    if length(gADirectLight) > 0 then
      for _cnt := 0 to length(gADirectLight)-1 do
      with gADirectLight[_cnt] do
      if not Ena4Tri then
      begin
        // "light direction faces triangle's plane" check
        _e := vectDot(@n, @Direction);
        Ena4Tri := (_e < 0);
      end;

    // enable/disable omni point lights
    if length(gAOmni_Point) > 0 then
      for _cnt := 0 to length(gAOmni_Point)-1 do
      with gAOmni_Point[_cnt] do
      if not Ena4Tri then
      begin
        // "light position faces triangle's plane" check
        // and a simple "attenuation end" check
        _e := vectDot(@n, @Position);
        Ena4Tri := (_e > d) and ((AttEnd = 0) or (_e - d <= AttEnd));

        // more precise "attenuation end" check
        if (Ena4Tri) and (AttEnd > 0) then
        begin
          _closest := closest_point_on_triangle(_a, _b, _c, @Position);
          _v := vectSubV(@_closest, @Position);
          Ena4Tri := (vectLen(@_v) <= AttEnd);
        end;
      end;

    // enable/disable spotlights
    if length(gASpotLight) > 0 then
      for _cnt := 0 to length(gASpotLight)-1 do
      with gASpotLight[_cnt] do
      if not Ena4Tri then
      begin
        // "light position faces triangle's plane" check
        // and a simple "attenuation end" check
        _e := vectDot(@n, @Position);
        Ena4Tri := (_e > d) and ((AttEnd = 0) or (_e - d <= AttEnd));

        // more precise "attenuation end" check
        if (Ena4Tri) and (AttEnd > 0) then
        begin
          _closest := closest_point_on_triangle(_a, _b, _c, @Position);
          _v := vectSubV(@_closest, @Position);
          Ena4Tri := (vectLen(@_v) <= AttEnd);
        end;
      end;
  end;
end;

// ---------------------------------------------------------------------------

function SetupLights(_triIdx: cardinal): boolean;

// Enable and disable lights for selected triangle.
// returns false if the triangle is not valid.

begin
  Result := false;

  // check triangle
  if (not g_TriData[_triIdx].valid) then exit;

  // clear all lights
  DisableAllLights;

  // enable lights for this triangle
  EnableLightsForTri(_triIdx);

  // If next tri has the same UVMap, enable also lights for that map
  // since their lightmaps will be shared later.
  if (g_TriData[_triIdx+1].valid) and
     (_triIdx+1 <= g_LMTS.header.nTris - 1) then
    if (vectSame(@g_TriData[_triIdx].uv00, @g_TriData[_triIdx+1].uv00)) and
       (vectSame(@g_TriData[_triIdx].uvE1, @g_TriData[_triIdx+1].uvE1)) and
       (vectSame(@g_TriData[_triIdx].uvE2, @g_TriData[_triIdx+1].uvE2))
    then EnableLightsForTri(_triIdx+1);

  // alright! ^_^
  Result := true;
end;

// ---------------------------------------------------------------------------

procedure CreateLightmaps;

var
  _h : TLMDB_Header;
  _cnt : cardinal;
  _argb : TARGB;
  _void : boolean;

begin
  ARGB_Reset(g_LastLM.lm);
  g_LastLM.uv00    := vect(0, 0, 0);
  g_LastLM.uvEdge1 := vect(0, 0, 0);
  g_LastLM.uvEdge2 := vect(0, 0, 0);

  // open LMDB file
  g_LMDB_FN := g_FileNameBase + C_EXT_LMFILE;
  if not filezOpenBin(g_LMDB_FN, obfWrite, g_LMDB) then
  begin
    Writeln(Format(ERR07, [g_LMDB_FN]));
    SafeHalt(1);
  end;

  // Write LMDB header
  try
    _h.ID := C_LMDB_ID;
    _h.Ver := C_LMDB_VER;
    _h.nLightmaps := g_LMTS.header.nTris;
    _h.MaxLMapSize := g_LMap_Max_Size;
    BlockWrite(g_LMDB, _h, SizeOf(_h));
  except
    Writeln(Format(ERR07, [g_LMDB_FN]));
    SafeHalt(1);
  end;

  // create 0x0 lightmap
  _argb := ARGB_Create(0, 0);

  // create lightmaps
  for _cnt := 0 to g_LMTS.header.nTris-1 do
  begin
    Write(Format(TXT04, [((_cnt+1) * 100) div g_LMTS.header.nTris]));

    // check if this triangle's subset is enabled
    _void := true;
    if (not g_DisabledSubsets[LMTS_GetTriSubsetIdx(@g_LMTS, _cnt)]) then
    begin
      // check if triangle is ok, then setup lights for that triangle.
      // if tri isn't ok show warning message and do not create the lightmap.
      if (SetupLights(_cnt)) then
      begin
        // create a lightmap for this triangle
        with g_TriData[_cnt] do CreateLightmap(_cnt);
        _void := false;
      end;
    end;

    // if didn't created a lightmap, save a void one (0x0)
    if (_void) then SaveLightmap(_argb);
  end;

  // free 0x0 lightmap
  ARGB_Free(_argb);

  // close LMDB file
  CloseFile(g_LMDB);

  // free last lightmap
  ARGB_Free(g_LastLM.lm);

  // All right
  Writeln(TXT05);
end;

// ---------------------------------------------------------------------------

procedure LoadTextures;

var
  _c : longint;
  _fn : string;
  _cur : PLMTS_TexData;
  _tga : TTGA;

begin
  // create ARGB image used as work buffer in the texture filtering process
  if g_Filter_LightMaps then
  begin
    g_TempARGB := ARGB_Create(g_LMap_Max_Size, g_LMap_Max_Size);
    if not g_TempARGB.ok then
    begin
      Writeln(ERR06);
      SafeHalt(1);
    end;
  end;

  // we need to load the textures ?
  if (not g_Cast_Shadows) or (not g_Use_Alpha_Channel) then exit;

  // create an array for textures
  try
    SetLength(g_MeshTextures, g_LMTS.header.nTris);
  except
    Writeln(ERR06);
    SafeHalt(1);
  end;

  // load textures
  _cur := g_LMTS.texData;
  for _c := 0 to g_LMTS.header.nTexts - 1 do
  begin
    // load texture as targa
    _fn := ChangeFileExt(ExtractFilePath(g_FileNameBase) + StrPas(_cur^.FName), '.tga');
    _tga := TGA_Load(_fn);

    // loaded correctly
    if not _tga.ok then
    begin
      Writeln(Format(ERR05, [_fn]));
      SafeHalt(1);
    end;

    // be sure that tga order is TOP_LEFT
    TGA_ChangeOrder(_tga, C_TGA_ORDER_TOP_LEFT);

    // place texture on the texture array as ARGB image
    with g_MeshTextures[_c] do
    begin
      header.ID := C_ARGB_ID;
      header.Ver := C_ARGB_VER;
      header.width := _tga.header.ImageSpec.ImageWidth;
      header.height := _tga.header.ImageSpec.ImageHeight;
      data := _tga.data;
      ok := _tga.ok;
    end;

    // next one
    inc(_cur);
  end;

  // all right
  Writeln(Format(TXT08, [g_LMTS.header.nTexts]));
end;

// ---------------------------------------------------------------------------

procedure FreeTextures;

var
  _c : longint;

begin
  ARGB_Free(g_TempARGB);
  if assigned(g_MeshTextures) then
    for _c := 0 to g_LMTS.header.nTexts - 1
    do ARGB_Free(g_MeshTextures[_c]);
  g_MeshTextures := NIL;
end;

// ---------------------------------------------------------------------------

procedure OctreeGetTriangleCB(_index: longint; var _tri: T_Tri); stdcall;

var
  _c : pointer;

begin
  _c := g_LMTS.tris;
  inc(longint(_c), g_LMTS.header.vtxSize * 3 * _index);
  Move(_c^, _tri[0], SizeOf(T_Vec));
  inc(longint(_c), g_LMTS.header.vtxSize);
  Move(_c^, _tri[1], SizeOf(T_Vec));
  inc(longint(_c), g_LMTS.header.vtxSize);
  Move(_c^, _tri[2], SizeOf(T_Vec));
end;

// ---------------------------------------------------------------------------

procedure CreateOctree;

var _v : T_Vec;

begin
  octReset(g_Octree);

  // we need an octree ?
  if not g_Cast_Shadows then exit;

  // create octree
  Write(TXT09);
  g_Octree := octBuild(g_LMTS.header.nTris, g_Oct_MinNodeTri,
                       g_Oct_MinNodeSize, g_Oct_MaxLevels,
                       OctreeGetTriangleCB);
  if not g_Octree.ok then
  begin
    Writeln(ERR08);
    SafeHalt(1);
  end;

  // get farest distance
  _v := vectSubV(@g_Octree.root.aabb.max, @g_Octree.root.aabb.min);
  g_MaxRayLength := vectLen(@_v);

  // all right
  Writeln(TXT10);
end;

// ---------------------------------------------------------------------------

procedure FreeOctree;

begin
  octFree(g_Octree);
end;

// ---------------------------------------------------------------------------

procedure InitLights;

var _cnt, _siz : longint;

begin
  if (not g_Cast_Shadows) or (not g_Soft_Shadows) then exit;

  // create buffers needed ONLY for soft shadows
  try
    _siz := (g_LMap_Max_Size+1) * (g_LMap_Max_Size+1);
    // directional
    if length(gADirectLight) > 0 then
      for _cnt := 0 to length(gADirectLight) - 1
      do SetLength(gADirectLight[_cnt].SCCorners, _siz);
    // omni
    if length(gAOmni_Point) > 0 then
      for _cnt := 0 to length(gAOmni_Point) - 1
      do SetLength(gAOmni_Point[_cnt].SCCorners, _siz);
    // spotlights
    if length(gASpotLight) > 0 then
      for _cnt := 0 to length(gASpotLight) - 1
      do SetLength(gASpotLight[_cnt].SCCorners, _siz);
  except
    Writeln(ERR06);
    SafeHalt(1);
  end;
end;

// ---------------------------------------------------------------------------

procedure FreeLights;

var _cnt : longint;

begin
  // directional
  if length(gADirectLight) > 0 then
    for _cnt := 0 to length(gADirectLight) - 1
    do gADirectLight[_cnt].SCCorners := NIL;
  gADirectLight := NIL;
  // omni
  if length(gAOmni_Point) > 0 then
    for _cnt := 0 to length(gAOmni_Point) - 1
    do gAOmni_Point[_cnt].SCCorners := NIL;
  gAOmni_Point := NIL;
  // spotlights
  if length(gASpotLight) > 0 then
    for _cnt := 0 to length(gASpotLight) - 1
    do gASpotLight[_cnt].SCCorners := NIL;
  gASpotLight := NIL;
end;

// ---------------------------------------------------------------------------

procedure CheckCriticalVars;

begin
  // check lumel size
  if (g_LMap_Lumel_Size < 0) then
  begin
    // set default value
    g_LMap_Lumel_Size := C_DEFAULT_LMAP_LUMEL_SIZE;
    // show warning message
    LogMsg(TXT12);
  end;

  // Check collision distance, if not set by the user use a default value
  // based on lumel size.
  if (g_Collision_Dist < 0) then g_Collision_Dist := g_LMap_Lumel_Size / 4;
end;

// ---------------------------------------------------------------------------

procedure Calc3DData;

var
  _cnt : longint;
  _lmi : word;

begin
  Write(TXT13);

  // fill triangle data
  for _cnt := 0 to g_LMTS.header.nTris - 1 do
  with g_TriData[_cnt] do
  begin
    valid := false;
    lastRayID := 0;

    // get triangle subset
    subsetID := LMTS_GetTriSubsetIdx(@g_LMTS, _cnt);

    // get texture' IDs
    LMTS_GetTriTextureID(@g_LMTS, subsetID, textID, _lmi);

    // Get triangle vertices
    GetTri(_cnt, P_Vec(a), P_Vec(b), P_Vec(c));

    // Get triangle edges
    triTwoEdges(P_Vec(a), P_Vec(b), P_Vec(c), e0, e1);

    try
      // Get normal
      n := triNormal(@e0, @e1);

      // Get D (used for plane's equation)
      d := vectDot(@n, P_Vec(a));

      // Get planar mapping
      GetPlanarMapping(_cnt, uv00, uvE1, uvE2, dU, dV);

      // this triangle is valid ^_^
      valid := true;
    except
      // isn't valid!!
      LogMsg(Format(TXT11, [_cnt]));
    end;
  end;

  // all right
  Writeln(TXT14);
end;

// ---------------------------------------------------------------------------

begin
  // title
  ShowAppTitle;

  // check command line parameters
  CheckParams;

  // define the script language
  if not scrLangInit then
  begin
    scrLangClose;
    Writeln(ERR01);
    SafeHalt(1);
  end;
  Writeln(TXT01);

  // load 3d world
  Load3DFile;

  // run script
  if not RunScript then
  begin
    scrLangClose;
    Writeln(ERR02);
    SafeHalt(1);
  end;
  Writeln(TXT02);

  // free script language
  scrLangClose;

  // check that some critical variables were modified by the user's script
  CheckCriticalVars;

  // Load textures
  LoadTextures;

  // Build octree
  CreateOctree;

  // init lights
  InitLights;

  // Calculate some triangle data
  Calc3DData;

  // Create lightmaps
  CreateLightmaps;

  // Save LMTS file
  g_DestFN := g_FileNameBase + '.2' + C_EXT_3DFILE;
  if not LMTS_Save(g_DestFN, g_LMTS) then
  begin
    Writeln(Format(ERR07, [g_DestFN]));
    ExitCode := 1; // halt(1);
  end;

  // free lights
  FreeLights;

  // Free octree
  FreeOctree;

  // Free textures
  FreeTextures;

  // free 3d world and some buffers
  LMTS_Free(g_LMTS);
  g_DisabledSubsets := NIL;
  g_TriData := NIL;

  // close log file
  if g_LogFile_IsOpen then CloseFile(g_LogFile);

  // done! :)
  Writeln(TXT06);
end.
