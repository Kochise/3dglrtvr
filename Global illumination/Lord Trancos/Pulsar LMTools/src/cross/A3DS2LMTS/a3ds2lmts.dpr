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

// 3DS2LMTS -  Autodesk 3D Studio (3DS) to LMTS converter.

program A3DS2LMTS;

// ---------------------------------------------------------------------------

{$apptype console}

uses SysUtils, Math,
  global in 'global.pas',
  filez in '../filez.pas',
  lmts in '../lmts.pas',
  a3ds in '../a3ds.pas';

// ---------------------------------------------------------------------------

procedure ShowAppTitle;

begin
  Writeln(APPTITLE);
end;

// ---------------------------------------------------------------------------

procedure CheckParams;

begin
  // check parameters
  if paramcount = 0 then
  begin
    Writeln(USAGE);
    halt(1);
  end;

  // output .SCR file ?
  if (paramcount > 1) then g_SCR_Output := (LowerCase(paramstr(2)) = C_PARAM);
end;

// ---------------------------------------------------------------------------

procedure Load3DSFile;

var
  _fn : string;

begin
  _fn := paramstr(1) + C_EXT_3DS;

  // load 3ds file
  Write(TXT01);
  g_3DS := A3DS_Load(_fn);

  // err ?
  if not g_3DS.ok then
  begin
    Writeln(Format(ERR01, [_fn]));
    Halt(1);
  end;

  // alright!
  Writeln(TXT02);
end;

// ---------------------------------------------------------------------------

procedure EnumMats(_chunk: pointer; var _stop: boolean); stdcall;

var
  _name : PA3DS_Chunk_Text;
  _mat1 : PA3DS_Chunk;
  _mat1fn : PA3DS_Chunk_Text;

begin
  // clear entry
  Fillchar(g_MatList[g_MatCount], SizeOf(g_MatList[g_MatCount]), 0);

  // get material name
  _name := A3DS_FindChunk(_chunk, NIL, C_3DSCHK_MATNAME);
  if _name = NIL then
  begin
    Writeln(Format(ERR05, [g_MatCount]));
    Halt(1);
  end;

  // copy name and upper case
  StrCopy(@g_MatList[g_MatCount].MatName[0], @_name^.First);
  StrUpper(@g_MatList[g_MatCount].MatName[0]);

  // now search material texture 1
  _mat1 := A3DS_FindChunk(_chunk, NIL, C_3DSCHK_MAT01);
  if assigned(_mat1) then
  begin
    _mat1fn := A3DS_FindChunk(_mat1, NIL, C_3DSCHK_MATFN);
    if assigned(_mat1fn) then
      if _mat1fn^.First <> #0 then
      begin
        // get texture 1 filename
        StrCopy(@g_MatList[g_MatCount].TextFN[0], @_mat1fn^.First);
        StrUpper(@g_MatList[g_MatCount].TextFN[0]);
        g_MatList[g_MatCount].TextID := g_TextCount+1;
        // inc text. count
        inc(g_TextCount);
      end;
  end;

  // inc material count
  inc(g_MatCount);
end;

procedure BuildMaterialList;

begin
  // first count how many materials
  g_MatCount := A3DS_EnumChunks(g_3DEdit, NIL, C_3DSCHK_MAT, NIL);

  // any material
  if (g_MatCount > 0) then
  begin
    // create buffer for materials
    try
      SetLength(g_MatList, g_MatCount);
    except
      Writeln(ERR04);
      Halt(1);
    end;

    // fill material buffer with data from 3DS file
    g_MatCount := 0;
    g_TextCount := 0;
    A3DS_EnumChunks(g_3DEdit, NIL, C_3DSCHK_MAT, EnumMats);
  end;

  // show material count
  Writeln(Format(TXT03, [g_MatCount]));
end;

// ---------------------------------------------------------------------------

procedure CountVerticesAndFaces(_chunk, _subChunks: pointer; _objName: pChar;
                                var _stop: boolean); stdcall;

var
  _mesh : PA3DS_Chunk;
  _vtxl : PA3DS_Chunk_VtxList;
  _fcel : PA3DS_Chunk_FaceList;
  _tcl  : PA3DS_Chunk_TextCoorList;

begin
  // find mesh chunk
  _mesh := A3DS_FindChunk(_chunk, _subChunks, C_3DSCHK_MESH);
  if assigned(_mesh) then
  begin
    // get pointers to vertex list, face list chunks
    _vtxl := A3DS_FindChunk(_mesh, NIL, C_3DSCHK_VTXLST);
    _fcel := A3DS_FindChunk(_mesh, NIL, C_3DSCHK_FACELST);
    _tcl  := A3DS_FindChunk(_mesh, NIL, C_3DSCHK_TXTCOOR);

    // validate
    if assigned(_vtxl) and assigned(_tcl) then
      if (_vtxl^.Count <> _tcl^.Count) then
      begin
        Writeln(Format(ERR07, [StrPas(_objName)]));
        Halt(1);
      end;

    // inc counters
    if assigned(_vtxl) then inc(g_VtxCount, _vtxl^.Count);
    if assigned(_fcel) then inc(g_FaceCount, _fcel^.Count);
  end;
end;

procedure EnumFacMatList(_chunk: pointer; var _stop: boolean); stdcall;

var
  _matName : array[0..C_3DSMAT_MAXNAMELEN] of char;
  _matID : longint;
  _face : longint;
  _cnt : longint;

  _nFace : word;
  _pWord : ^word;

begin
  // get material name
  inc(longint(_chunk), SizeOf(TA3DS_Chunk));
  FillChar(_matName, SizeOf(_matName), 0);
  StrCopy(@_matName[0], _chunk);
  StrUpper(@_matName[0]);
  A3DS_SkipString(_chunk);

  // find material on material list
  _matID := -1;
  for _cnt := 0 to g_MatCount-1 do
  if StrComp(@_matName[0], @g_MatList[_cnt].MatName[0]) = 0 then
  begin
    _matID := _cnt;
    break;
  end;

  // not found ?
  if _matID = -1 then
  begin
    Writeln(Format(TXT04, [StrPas(_matName)]));
    exit;
  end;

  // get face count
  _pWord := _chunk;
  _nFace := _pWord^;
  inc(_pWord);

  // get face-list
  for _cnt := 0 to _nFace - 1 do
  begin
    _face := g_FaceOffset + _pWord^;
    g_FaceList[_face].matID := _matID + 1; // assign mat ID to this face
    inc(_pWord);
  end;
end;

procedure EnumVerticesAndFaces(_chunk, _subChunks: pointer; _objName: pChar;
                               var _stop: boolean); stdcall;

var
  _vtxOffset : longint;
  _mesh : PA3DS_Chunk;
  _vtxl : PA3DS_Chunk_VtxList;
  _fcel : PA3DS_Chunk_FaceList;
  _tcl  : PA3DS_Chunk_TextCoorList;
  _vtx  : PA3DS_Vector;
  _coor : PA3DS_TextCoor;
  _face : PA3DS_Face;
  _cnt : longint;

begin
  _vtxOffset := g_VtxCount;
  g_FaceOffset := g_FaceCount;

  // find mesh chunk
  _mesh := A3DS_FindChunk(_chunk, _subChunks, C_3DSCHK_MESH);
  if assigned(_mesh) then
  begin
    // get pointers to vertex list, face and text. coor. list chunks
    _vtxl := A3DS_FindChunk(_mesh, NIL, C_3DSCHK_VTXLST);
    _fcel := A3DS_FindChunk(_mesh, NIL, C_3DSCHK_FACELST);
    _tcl  := A3DS_FindChunk(_mesh, NIL, C_3DSCHK_TXTCOOR);

    // fill vertices
    if assigned(_tcl) then _coor := @_tcl^.First else _coor := NIL;
    if assigned(_vtxl) then
    begin
      _vtx := @_vtxl^.First;

      for _cnt := 0 to _vtxl^.Count - 1 do
      begin
        // get vertex and texture coordinates
        Fillchar(g_VtxList[g_VtxCount], SizeOf(g_VtxList[g_VtxCount]), 0);
        g_VtxList[g_VtxCount].x := _vtx^.x;
        g_VtxList[g_VtxCount].y := _vtx^.y;
        g_VtxList[g_VtxCount].z := _vtx^.z;
        if assigned(_coor) then
        begin
          g_VtxList[g_VtxCount].u := _coor^.u;
          g_VtxList[g_VtxCount].v := _coor^.v;
          inc(_coor);
        end;

        // next vertex
        inc(_vtx);
        inc(g_VtxCount);
      end;
    end;

    // fill faces
    if assigned(_fcel) then
    begin
      // faces
      _face := @_fcel^.First;
      for _cnt := 0 to _fcel^.Count - 1 do
      begin
        g_FaceList[g_FaceCount].a := _face^.a + _vtxOffset;
        g_FaceList[g_FaceCount].b := _face^.b + _vtxOffset;
        g_FaceList[g_FaceCount].c := _face^.c + _vtxOffset;
        g_FaceList[g_FaceCount].matID := 0;
        // next face
        inc(_face);
        inc(g_FaceCount);
      end;

      // face material lists
      A3DS_EnumChunks(_mesh, pointer(_face), C_3DSCHK_FACDESC, EnumFacMatList);
    end;
  end;
end;

procedure BuildMeshData;

begin
  // first count #vertices and #faces
  g_VtxCount  := 0;
  g_FaceCount := 0;
  A3DS_EnumObjects(g_3DS, CountVerticesAndFaces);

  // check counts
  if (g_VtxCount < 1) or (g_FaceCount < 1) then
  begin
    Writeln(ERR06);
    Halt(1);
  end;

  // get buffers for data
  try
    SetLength(g_VtxList, g_VtxCount);
    SetLength(g_FaceList, g_FaceCount);
  except
    Writeln(ERR04);
    Halt(1);
  end;

  // fill buffers
  g_VtxCount  := 0;
  g_FaceCount := 0;
  A3DS_EnumObjects(g_3DS, EnumVerticesAndFaces);

  // show counters
  Writeln(Format(TXT05,[g_VtxCount, g_FaceCount]));
end;

// ---------------------------------------------------------------------------

procedure Process3DSFile;

var
  _v : longint;
  _ver : PA3DS_Chunk_Ver;

begin
  // get a pointer to the 3D Edit chunk
  g_3DEdit := A3DS_FindChunk(g_3DS.Data, NIL, C_3DSCHK_3DEDIT);
  if g_3DEdit = NIL then
  begin
    Writeln(ERR02);
    Halt(1);
  end;

  // try to find 3ds file version.
  _v := -1;
  _ver := A3DS_FindChunk(g_3DS.Data, NIL, C_3DSCHK_VER);
  if (_ver <> NIL) then _v := _ver^.Version;

  // if version not found or not supported exit
  if (_v <> C_3DS_VERSION) then
  begin
    Writeln(ERR03);
    Halt(1);
  end;

  // build material list
  BuildMaterialList;

  // build mesh data
  BuildMeshData;
end;

// ---------------------------------------------------------------------------

procedure BuildLMTSData;

var
  _v : longint;
  _s : longint;
  _c, _l : longint;
  _tex : PLMTS_TexData;
  _sub : PLMTS_Subset;
  _vtx : PLMTS_Vertex;
  _triCount  : longint;
  _triOffset : longint;
  _nMats : longint;

  // -----------------------------

  procedure CopyVtx(_idx: longint);

  begin
    _vtx^.x := g_VtxList[_idx].x;
    _vtx^.y := g_VtxList[_idx].y;
    _vtx^.z := g_VtxList[_idx].z;
    try
      _vtx^.u1 := g_VtxList[_idx].u;
      _vtx^.v1 := - g_VtxList[_idx].v;
    except
      writeln(Format(TXT10, [_idx]));
      _vtx^.u1 := 0;
      _vtx^.v1 := 0;
    end;
    inc(_vtx);
  end;

  // -----------------------------

begin
  _nMats := g_MatCount;

  // check if there is any face without material
  _v := 1;
  for _l := 0 to g_FaceCount - 1 do
  if g_FaceList[_l].MatID = 0 then
  begin
    inc(g_MatCount);
    _v := 0;
    break;
  end;

  // create buffers
  try
    _s := SizeOf(TLMTS_TexData) * g_TextCount;
    if (_s > 0) then
    begin
      GetMem(g_LMTS.texData, _s);
      FillChar(g_LMTS.texData^, _s, 0);
    end;

    _s := SizeOf(TLMTS_Subset) * g_MatCount;
    GetMem(g_LMTS.subsets, _s);
    FillChar(g_LMTS.subsets^, _s, 0);

    _s := SizeOf(TLMTS_Vertex) * g_FaceCount * 3;
    GetMem(g_LMTS.tris, _s);
    FillChar(g_LMTS.tris^, _s, 0);
  except
    Writeln(ERR04);
    Halt(1);
  end;

  // fill texture buffer
  _tex := g_LMTS.texData;
  for _l := 0 to _nMats - 1 do
    if (g_MatList[_l].TextFN[0] <> #0) then
    begin
      StrCopy(@_tex^.FName[0], @g_MatList[_l].TextFN[0]);
      _tex^.Flags := 0;
      // next
      inc(_tex);
    end;

  // fill subset and triangle buffers
  _triOffset := 0;
  _sub := g_LMTS.subsets;
  _vtx := g_LMTS.tris;
  for _l := 0 to g_MatCount - 1 do
  begin
    // copy tris for this subset
    _triCount := 0;
    for _c := 0 to g_FaceCount - 1 do
    begin
      if g_FaceList[_c].matID = _l + _v then
      begin
        CopyVtx(g_FaceList[_c].a);
        CopyVtx(g_FaceList[_c].b);
        CopyVtx(g_FaceList[_c].c);
        inc(_triCount);
      end;
    end;

    // fill subset info
    _sub^.Offset  := _triOffset;
    _sub^.Count   := _triCount;

    if (_v = 0) and (_l = 0) then
    begin
      _sub^.TextID1 := $FFFF;
    end else
    begin
      if g_MatList[_l - 1 + _v].TextID > 0
        then _sub^.TextID1 := g_MatList[_l - 1 + _v].TextID-1
          else _sub^.TextID1 := $FFFF;
    end;

    _sub^.TextID2 := $FFFF; // no texture
    inc(_triOffset, _triCount);

    // next subset
    inc(_sub);
  end;

  // fill header
  with g_LMTS.header do
  begin
    ID         := C_LMTS_ID;
    Ver        := C_LMTS_VER;
    headerSize := SizeOf(TLMTS_Header);
    nTexts     := g_TextCount;
    nSubsets   := g_MatCount;
    nTris      := g_FaceCount;
    subSize    := SizeOf(TLMTS_Subset);
    vtxSize    := SizeOf(TLMTS_Vertex);
  end;

  // alright
  g_LMTS.ok := true;
end;

// ---------------------------------------------------------------------------

procedure SaveLMTSfile;

var _fn : string;

begin
  Write(TXT06);
  _fn := paramstr(1) + C_EXT_LMTS;
  if not LMTS_Save(_fn, g_LMTS) then
  begin
    Writeln(Format(ERR08, [_fn]));
    Halt(1);
  end;
  Writeln(TXT07);
end;

// ---------------------------------------------------------------------------

procedure CreateSCRFile(_fn: string);

begin
  {$I-}
  AssignFile(g_SCR, _fn);
  Rewrite(g_SCR);
  {$I+}
  if (IOResult <> 0) then
  begin
    Writeln(Format(ERR09, [_fn]));
    Halt(1);
  end;
end;

// ---------------------------------------------------------------------------

procedure WriteAmbientLight;

var
  _p : PA3DS_Chunk;
  _c : PA3DS_Chunk_RGB_F;

begin
  // find ambient light
  _p := A3DS_FindChunk(g_3DEdit, NIL, C_3DSCHK_AMBIENT);
  if _p = NIL then exit;

  // find color
  _c := A3DS_FindChunk(_p, NIL, C_3DSCHK_RGB_F);
  if _c = NIL then exit;

  // write ambient light
  Writeln(g_SCR, SCR16);
  Writeln(g_SCR, SCR15 + IntToHex(A3DS_RGB_F_To_RGB_I(@_c^.rgb), 8));
  Writeln(g_SCR);

  // increase light count
  inc(g_LitCount);
end;

procedure EnumLights(_chunk, _subChunks: pointer; _objName: pChar;
                     var _stop: boolean); stdcall;

var
  _s : string;
  _l : TLight;
  _lit : PA3DS_Chunk_Light;
  _color : PA3DS_Chunk_RGB_F;
  _spot  : PA3DS_Chunk_Spot;
  _mult  : PA3DS_Chunk_OneFloat;
  _att   : PA3DS_Chunk;
  _attS, _attE : PA3DS_Chunk_OneFloat;

begin
  _lit := A3DS_FindChunk(_chunk, _subChunks, C_3DSCHK_LIGHT);

  if assigned(_lit) then
  begin
    // set default values for the light
    FillChar(_l, SizeOf(_l), 0);
    _l.mult := 1;
    _l.color := $FFFFFF;
    _l.cast_shadows := true;
    _l.enabled := true;

    // light pos
    _l.x := _lit^.Pos.x;
    _l.y := _lit^.Pos.y;
    _l.z := _lit^.Pos.z;

    // light color
    _color := A3DS_FindChunk(pointer(_lit), @_lit^.FirstSubChunk,
                             C_3DSCHK_RGB_F);
    if assigned(_color) then
    begin
      _l.color := A3DS_RGB_F_To_RGB_I(@_color^.RGB);
    end;

    // multiplier
    _mult := A3DS_FindChunk(pointer(_lit), @_lit^.FirstSubChunk,
                            C_3DSCHK_LITMUL);
    if assigned(_mult) then _l.mult := _mult^.f;

    // attenuation ?
    _att := A3DS_FindChunk(pointer(_lit), @_lit^.FirstSubChunk,
                           C_3DSCHK_LITATT);
    if assigned(_att) then
    begin
      // attenuation start
      _attS := A3DS_FindChunk(pointer(_lit), @_lit^.FirstSubChunk,
                              C_3DSCHK_LITATTS);
      _attE := A3DS_FindChunk(pointer(_lit), @_lit^.FirstSubChunk,
                              C_3DSCHK_LITATTE);

      if assigned(_attS) then _l.att_start := _attS^.f;
      if assigned(_attE) then _l.att_end := _attE^.f;
    end;

    // light is spot ?
    _spot := A3DS_FindChunk(pointer(_lit), @_lit^.FirstSubChunk, C_3DSCHK_SPOT);
    if assigned(_spot) then
    begin
      _l.isSpot := true;
      _l.tx := _spot^.Target.x;
      _l.ty := _spot^.Target.y;
      _l.tz := _spot^.Target.z;
      _l.theta := DegToRad(_spot^.HotSpot);
      _l.phi := DegToRad(_spot^.FallOff);
    end;

    // write light into SCR file
    Writeln(g_SCR, SCR02 + StrPas(_objName));
    if not _l.isSpot then _s := SCR03 else _s := SCR04;
    Writeln(g_SCR, Format(_s + SCR06, [_l.x, _l.y, _l.z]));
    if _l.isSpot then
    begin
      Writeln(g_SCR, Format(_s + SCR12, [_l.tx, _l.ty, _l.tz]));
      Writeln(g_SCR, Format(_s + SCR13, [_l.theta]));
      Writeln(g_SCR, Format(_s + SCR14, [_l.phi]));
    end;
    Writeln(g_SCR, Format(_s + SCR07, [_l.mult]));
    Writeln(g_SCR, Format(_s + SCR08, [_l.att_start, _l.att_end]));
    Writeln(g_SCR, _s + SCR09 + IntToHex(_l.color, 8));
    Writeln(g_SCR, _s + SCR10);
    Writeln(g_SCR, _s + SCR11);
    Writeln(g_SCR, _s + SCR05);
    Writeln(g_SCR);

    // increase light count
    inc(g_LitCount);
  end;
end;

procedure WriteSCRfile;

begin
  // write some comments
  Writeln(g_SCR, SCR01);
  Writeln(g_SCR, SCR17);
  Writeln(g_SCR);

  // reset light counter
  g_LitCount := 0;

  // find and write ambient light
  WriteAmbientLight;

  // write lights
  A3DS_EnumObjects(g_3DS, EnumLights);
end;

// ---------------------------------------------------------------------------

procedure OutputSCRfile;

begin
  if not g_SCR_Output then exit;

  // create .scr file
  CreateSCRFile(paramstr(1)+C_EXT_SCR);

  // write script file with light info
  WriteSCRfile;

  // close .scr file
  CloseFile(g_SCR);

  // alright
  Writeln(Format(TXT09, [g_LitCount]));
end;

// ---------------------------------------------------------------------------

begin
  DecimalSeparator := '.';
  LMTS_Reset(g_LMTS);

  // title
  ShowAppTitle;

  // check command line parameters
  CheckParams;

  // load 3ds file
  Load3DSFile;

  // Process 3ds file
  Process3DSFile;

  // Build LMTS data
  BuildLMTSData;

  // save LMTS file
  SaveLMTSfile;

  // free LMTS and data used for it
  LMTS_Free(g_LMTS);
  g_MatList  := NIL;
  g_VtxList  := NIL;
  g_FaceList := NIL;

  // output .SCR file if needed
  OutputSCRfile;

  // free 3ds file
  A3DS_Free(g_3DS);

  // done! :)
  Writeln(TXT08);
end.
