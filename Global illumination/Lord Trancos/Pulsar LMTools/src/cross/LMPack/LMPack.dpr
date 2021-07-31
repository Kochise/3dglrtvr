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

// LMPack -  Lightmap Packing Utility

// The method I'm using for packing lightmaps is based
// on a paper by Jim Scott (jimscott@blackpawn.com) that
// can be found here:
//
//   http://www.blackpawn.com/
//   http://www.blackpawn.com/texts/lightmaps/default.html

program LMPack;

// ---------------------------------------------------------------------------

{$apptype console}

uses SysUtils,
  global in 'global.pas',
  lmts in '../lmts.pas',
  lmdb in '../lmdb.pas',
  argb in '../argb.pas',
  tga in '../tga.pas',
  wbmp in '../wbmp.pas',
  bitmap in '../bitmap.pas',
  filez in '../filez.pas';

// ---------------------------------------------------------------------------

procedure ShowAppTitle;

begin
  Writeln(APPTITLE);
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
    halt(1);
  end;

  // first parameter will be used as filename base
  g_FileNameBase := paramstr(1);

  // process second parameter
  if paramcount > 1 then
  begin
    _s := UpperCase(paramstr(2));
    for _cnt := 1 to length(_s) do
    case _s[_cnt] of
      'B' : begin
              g_Output_Format := C_OUTPUT_24BPP_BMP;
              Writeln(C_PARAMMSG02);
            end;
      'L' : begin
              g_Output_Format := C_OUTPUT_16BPP_TGA;
              Writeln(C_PARAMMSG00);
            end;
      'R' : begin
              g_Allow_LMRotation := not C_DEFAULT_LMROTATION;
              Writeln(C_PARAMMSG01);
            end;
      else Writeln(ERR10 + _s[_cnt]);
    end;
    Writeln;
  end;
end;

// ---------------------------------------------------------------------------

procedure Load3Dfile;

var _fn : string;

begin
  _fn := g_FileNameBase + '.2' + C_EXT_3DFILE;
  g_LMTS := LMTS_Load(_fn);
  if not g_LMTS.ok then
  begin
    Writeln(Format(ERR05, [_fn]));
    halt(1);
  end;
  Writeln(Format(TXT03, [g_LMTS.header.nTris, g_LMTS.header.nSubsets]));
end;

// ---------------------------------------------------------------------------

function GetDstLightmapFN(_idx: longint): string;

var _ext : string;

begin
  Case g_Output_Format of
    C_OUTPUT_32BPP_TGA : _ext := 'tga';
    C_OUTPUT_16BPP_TGA : _ext := 'tga';
    C_OUTPUT_24BPP_BMP : _ext := 'bmp';
  end;

  Result := UpperCase(Format('%s.lm%5.5d.'+_ext, [g_FileNameBase, _idx+1]));
end;

// ---------------------------------------------------------------------------

procedure LoadSrcLightmaps;

var
  _cnt : cardinal;
  _tsi : longint;
  _fn : string;
  _f : file;
  _h : TLMDB_Header;

begin
  try
    // create arrays for source lightmaps and other data
    SetLength(g_SrcLightmaps, g_LMTS.header.nTris);
    SetLength(g_SharedWith, g_LMTS.header.nTris);
    for _cnt := 0 to Length(g_SharedWith) - 1 do g_SharedWith[_cnt] := -1;
    SetLength(g_WasRotated, g_LMTS.header.nTris);
    for _cnt := 0 to Length(g_WasRotated) - 1 do g_WasRotated[_cnt] := false;
    SetLength(g_TriSubsetID, g_LMTS.header.nTris);
    for _cnt := 0 to Length(g_TriSubsetID) - 1 do
    begin
      _tsi := LMTS_GetTriSubsetIdx(@g_LMTS, _cnt);
      if _tsi = -1 then
      begin
        Writeln(ERR07);
        halt(1);
      end;
      g_TriSubsetID[_cnt] := _tsi;
    end;
  except
    Writeln(ERR06);
    halt(1);
  end;

  // open LMDB file
  _fn := g_FileNameBase + C_EXT_LMFILE;
  if not filezOpenBin(_fn, obfRead, _f) then
  begin
    Writeln(Format(ERR05, [_fn]));
    Halt(1);
  end;

  // Read LMDB header
  try
    BlockRead(_f, _h, SizeOf(_h));
  except
    Writeln(Format(ERR07, [_fn]));
    Halt(1);
  end;

  // Verify LMDB
  if (_h.ID <> C_LMDB_ID) or
     (_h.Ver <> C_LMDB_VER) or
     (_h.nLightmaps <> g_LMTS.header.nTris) then
  begin
    Writeln(Format(ERR09, [_fn]));
    Halt(1);
  end;

  // set destination lightmap size
  g_LMap_Max_Size := _h.MaxLMapSize;

  // load
  for _cnt := 0 to g_LMTS.header.nTris - 1 do
  begin
    Write(Format(TXT02, [(_cnt * 100) div g_LMTS.header.nTris]));
    g_SrcLightmaps[_cnt] := ARGB_LoadFromFile(_f);
    if not g_SrcLightmaps[_cnt].ok then
    begin
      Writeln(Format(ERR05, [_fn]));
      halt(1);
    end;
  end;

  // close LMDB file
  CloseFile(_f);

  // ok
  Writeln(TXT01);
end;

// ---------------------------------------------------------------------------

procedure PreFilterLightmaps;

// if output format is 16 bit, filter now the lightmaps to help
// optimization and sharing process.

var
  _cur : ^cardinal;
  _cnt : cardinal;
  _xyz : cardinal;

begin
  if g_Output_Format <> C_OUTPUT_16BPP_TGA then exit;

  // pre-filter lightmaps
  for _cnt := 0 to Length(g_SrcLightmaps) - 1 do
  begin
    if g_SrcLightmaps[_cnt].data <> NIL then
    begin
      _cur := g_SrcLightmaps[_cnt].data;
      with g_SrcLightmaps[_cnt].header do
        for _xyz := 0 to (width * height)-1 do
        begin
          _cur^ := (_cur^ and $80F8F8F8);
          inc(_cur);
        end;
    end;
  end;
end;

// ---------------------------------------------------------------------------

procedure SaveDstLightmaps;

var
  _16b : pointer;
  _tga : TTGA;
  _bmp : TWBMP;
  _bpl : longint;
  _cnt : cardinal;
  _fn : string;

begin
  _16b := NIL;

  // save
  for _cnt := 0 to Length(g_DstLightmaps) - 1 do
  begin
    // get dest filename
    Write(Format(TXT07, [(_cnt * 100) div cardinal(Length(g_DstLightmaps))]));
    _fn := GetDstLightmapFN(_cnt);

    // save as 32 or 16 bpp TGA ?
    if (g_Output_Format = C_OUTPUT_32BPP_TGA) or
       (g_Output_Format = C_OUTPUT_16BPP_TGA) then
    begin
      // convert ARGB to TGA
      TGA_Reset(_tga);
      with g_DstLightmaps[_cnt] do
      begin
        _tga.header := Get32ARGBTGAHeader(header.width, header.height);
        _tga.data := data;
        _tga.ok := ok;
      end;

      // if needed convert data to 16 bit
      if (g_Output_Format = C_OUTPUT_16BPP_TGA) then
      begin
        with _tga.header.ImageSpec do
        begin
          // get memory for 16 bit buffer
          try
            GetMem(_16b, 2 * ImageWidth * ImageHeight);
          except
            Writeln(ERR06);
            halt(1);
          end;

          // convert 32 to 16
          bitmap_X8R8G8B8toR5G6B5(_tga.data, _16b, ImageWidth, ImageHeight);

          // change TGA to 16
          PixelDepth := 16;
          _tga.data := _16b;
        end;
      end;

      // save tga
      if not TGA_Save(_fn, _tga) then
      begin
        Writeln(Format(ERR04, [_fn]));
        halt(1);
      end;

      // free 16 bit buffer
      if (g_Output_Format = C_OUTPUT_16BPP_TGA) then
      try
        with _tga.header.ImageSpec do FreeMem(_16b, 2 * ImageWidth * ImageHeight);
      except
      end;
    end;

    // save as 24 bpp Windows BMP
    if (g_Output_Format = C_OUTPUT_24BPP_BMP) then
    begin
      // create bmp
      with g_DstLightmaps[_cnt]
      do _bmp := WBMP_Create24bpp(header.width, header.height);
      if not _bmp.ok then
      begin
        Writeln(ERR06);
        halt(1);
      end;

      // copy lightmap inside
      _bpl := _bmp.infoH.SizeImage div _bmp.infoH.Height;
      bitmap_X8R8G8B8toB8G8R8(g_DstLightmaps[_cnt].data, _bmp.data, _bpl,
                              _bmp.infoH.Width, _bmp.infoH.Height);

      // flip
      if not bitmap_flip(_bmp.infoH.Height, _bpl, _bmp.data) then
      begin
        Writeln(ERR06);
        halt(1);
      end;

      // save bmp
      if not WBMP_Save(_fn, _bmp) then
      begin
        Writeln(Format(ERR04, [_fn]));
        halt(1);
      end;

      // free bmp
      WBMP_Free(_bmp);
    end;
  end;

  // ok
  Writeln(TXT08);
end;

// ---------------------------------------------------------------------------

procedure FreeAllLightmaps;

var
  _cnt : cardinal;

begin
  // source lightmaps
  for _cnt := 0 to Length(g_SrcLightmaps) - 1
  do ARGB_Free(g_SrcLightmaps[_cnt]);
  g_SrcLightmaps := NIL;

  // destination lightmaps
  for _cnt := 0 to Length(g_DstLightmaps) - 1
  do ARGB_Free(g_DstLightmaps[_cnt]);
  g_DstLightmaps := NIL;

  // other stuff
  g_SharedWith := NIL;
  g_WasRotated := NIL;
end;

// ---------------------------------------------------------------------------

procedure SetSameUV(_tri: longint; _u, _v: single);

var
  _cur : PLMTS_Vertex;
  _cnt : longint;

begin
  _cur := g_LMTS.tris;
  inc(longint(pointer(_cur)), _tri * 3 * g_LMTS.header.vtxSize);

  for _cnt := 0 to 2 do
  begin
    _cur^.u2 := _u;
    _cur^.v2 := _v;
    // next vertex
    inc(longint(pointer(_cur)), g_LMTS.header.vtxSize);
  end;
end;

// ---------------------------------------------------------------------------

procedure AdjustUV(_tri, _lmtri, _x, _y: longint);

var
  _u, _v : single;
  _cur : PLMTS_Vertex;
  _cnt : longint;

begin
  _cur := g_LMTS.tris;
  inc(longint(pointer(_cur)), _tri * 3 * g_LMTS.header.vtxSize);

  for _cnt := 0 to 2 do
  begin
    if (not g_WasRotated[_lmtri]) then
    begin
      _u := _cur^.u2 * g_SrcLightmaps[_lmtri].header.width;
      _v := _cur^.v2 * g_SrcLightmaps[_lmtri].header.height;
    end else
    begin
      _u := (1 - _cur^.v2) * g_SrcLightmaps[_lmtri].header.width;
      _v := _cur^.u2 * g_SrcLightmaps[_lmtri].header.height;
    end;
    _u := _u + _x;
    _v := _v + _y;
    _cur^.u2 := _u / g_LMap_Max_Size;
    _cur^.v2 := _v / g_LMap_Max_Size;
    // next vertex
    inc(longint(pointer(_cur)), g_LMTS.header.vtxSize);
  end;
end;

// ---------------------------------------------------------------------------

procedure OptimizeLightmaps;

var
  _cnt, _z, _oc : cardinal;
  _cur : pointer;
  _1st : cardinal;
  _siz : cardinal;
  _opt : boolean;

begin
  _oc := 0;

  for _cnt := 0 to Length(g_SrcLightmaps) - 1 do
  begin
    // check if lightmap pixels are all the same
    _opt := true;
    with g_SrcLightmaps[_cnt].header do _siz := width * height;
    if _siz > 1 then
    begin
      _cur := g_SrcLightmaps[_cnt].data;
      _1st := cardinal(_cur^); // get first pixel
      inc(longint(_cur), 4);

      for _z := 1 to _siz - 1 do
      begin
        if (cardinal(_cur^) <> _1st) then
        begin
          _opt := false;
          break;
        end;
        inc(longint(_cur), 4);
      end;
    end else _opt := false;

    // optimize if requiered.
    if (_opt) then
    begin
      inc(_oc);
      // free previous lightmap
      ARGB_Free(g_SrcLightmaps[_cnt]);
      // create new one
      g_SrcLightmaps[_cnt] := ARGB_Create(3, 3);
      if not g_SrcLightmaps[_cnt].ok then
      begin
        Writeln(ERR06);
        halt(1);
      end;
      // fill it
      with g_SrcLightmaps[_cnt].header do _siz := width * height;
      _cur := g_SrcLightmaps[_cnt].data;
      for _z := 0 to _siz-1 do
      begin
        cardinal(_cur^) := _1st;
        inc(longint(_cur), 4);
      end;
      // modify triangle UVs
      SetSameUV(_cnt, 0.5, 0.5);
    end;
  end;

  // write how many lightmaps had been optimized
  Writeln(Format(TXT04, [_oc, Length(g_SrcLightmaps),
                        (_oc * 100) / Length(g_SrcLightmaps)]));
end;

// ---------------------------------------------------------------------------

procedure ShareLightmaps;

var
  _l1, _l2 : longint;
  _s1, _s2, _sc : longint;
  _sha : boolean;

begin
  // find wich lightmaps are identical
  _sc := 0;
  for _l1 := Length(g_SrcLightmaps) - 1 downto 1 do
    begin
      _sha := false;

      for _l2 := _l1 - 1 downto 0 do
      if (g_TriSubsetID[_l1] = g_TriSubsetID[_l2]) then
        begin
          with g_SrcLightmaps[_l1].header do _s1 := width * height * 4;
          with g_SrcLightmaps[_l2].header do _s2 := width * height * 4;
          if (_s1 > 0) and (_s2 > 0) and (_s1 = _s2) then
            if CompareMem(g_SrcLightmaps[_l1].data,
                          g_SrcLightmaps[_l2].data, _s1) then
            begin
              _sha := true;
              g_SharedWith[_l1] := _l2;
            end;
        end;

      if (_sha) then inc(_sc);
    end;

  // write how many lightmaps had been shared
  Writeln(Format(TXT05, [_sc, Length(g_SrcLightmaps),
                        (_sc * 100) / Length(g_SrcLightmaps)]));
end;

// ---------------------------------------------------------------------------

procedure PackLightmaps;

var
  _sub : longint;
  _tri : longint;
  _ilm : longint;

  // ---------------------------------

  function Insert(_n: PLMPackNode; _treeID: longint): boolean;

  var
    _tmpARGB : TARGB;
    _dw, _dh : longint;
    _sha : longint;

  begin
    Result := false;

    if (_n^.child[0] <> NIL) and (_n^.child[1] <> NIL) then
    begin
      Result := Insert(_n^.child[0], _treeID);
      if Result then exit;

      Result := Insert(_n^.child[1], _treeID);
      if Result then exit;

      exit;
    end;

    // if it's in use, exit
    if _n^.lmidx <> -1 then exit;

    // lm fits here?
    if (_n^.w < g_SrcLightmaps[_tri].header.width) or
       (_n^.h < g_SrcLightmaps[_tri].header.height) then
    begin

      // Doesn't fit! but, what about if it's rotated ?
      if (g_Allow_LMRotation) then
      begin
        if (_n^.h >= g_SrcLightmaps[_tri].header.width) and
           (_n^.w >= g_SrcLightmaps[_tri].header.height) then
        begin
          // lmap will fit if rotated!!

          // create new ARGB for rotated lightmap
          with g_SrcLightmaps[_tri].header
            do _tmpARGB := ARGB_Create(height, width);

          // ok?
          if (not _tmpARGB.ok) then
          begin
            Writeln(ERR06);
            halt(1);
          end; 

          // copy rotated lightmap into new ARGB
          with g_SrcLightmaps[_tri]
            do bitmap_rotateR(header.width, header.height, 32, data, _tmpARGB.data);

          // free previous ARGB
          ARGB_Free(g_SrcLightmaps[_tri]);

          // replace with new one
          Move(_tmpARGB, g_SrcLightmaps[_tri], SizeOf(TARGB));

          // All right
          g_WasRotated[_tri] := true;
        end else exit; // cannot rotate, exit
      end else exit; // rotation not allowed by user, exit

    end;

    // if node rect and lightmap have the same size, place here the lm
    if (_n^.w = g_SrcLightmaps[_tri].header.width) and
       (_n^.h = g_SrcLightmaps[_tri].header.height) then
    begin
      // modify UV coordinates for this triangle
      AdjustUV(_tri, _tri, _n^.x, _n^.y);
      // modify UV coordinates for triangles with the same lightmap
      for _sha := 0 to Length(g_SharedWith)-1 do
        if g_SharedWith[_sha] = _tri
          then AdjustUV(_sha, _tri, _n^.x, _n^.y);
      // draw
      ARGB_Draw(g_SrcLightmaps[_tri], g_DstLightmaps[_treeID], _n^.x, _n^.y);
      _n^.lmidx := _tri;
      Result := true;
      exit;
    end;

    // otherwise, split node
    try
      GetMem(_n^.child[0], SizeOf(TLMPackNode));
      GetMem(_n^.child[1], SizeOf(TLMPackNode));
      Fillchar(_n^.child[0]^, SizeOf(TLMPackNode), 0);
      Fillchar(_n^.child[1]^, SizeOf(TLMPackNode), 0);

      g_LMPackTrees[_treeID].prev^.next := _n^.child[0];
      _n^.child[0]^.next := _n^.child[1];
      g_LMPackTrees[_treeID].prev := _n^.child[1];

      _n^.child[0]^.lmidx := -1;
      _n^.child[1]^.lmidx := -1;
    except
      Writeln(ERR06);
      halt(1);
    end;

    // decide wich way to split
    _dw := _n^.w - g_SrcLightmaps[_tri].header.width;
    _dh := _n^.h - g_SrcLightmaps[_tri].header.height;

    if (_dw > _dh) then
    begin
      _n^.child[0]^.x := _n^.x;
      _n^.child[0]^.y := _n^.y;
      _n^.child[0]^.w := g_SrcLightmaps[_tri].header.width;
      _n^.child[0]^.h := _n^.h;

      _n^.child[1]^.x := _n^.x + g_SrcLightmaps[_tri].header.width;
      _n^.child[1]^.y := _n^.y;
      _n^.child[1]^.w := _n^.w - g_SrcLightmaps[_tri].header.width;
      _n^.child[1]^.h := _n^.h;
    end else
    begin
      _n^.child[0]^.x := _n^.x;
      _n^.child[0]^.y := _n^.y;
      _n^.child[0]^.w := _n^.w;
      _n^.child[0]^.h := g_SrcLightmaps[_tri].header.height;

      _n^.child[1]^.x := _n^.x;
      _n^.child[1]^.y := _n^.y + g_SrcLightmaps[_tri].header.height;
      _n^.child[1]^.w := _n^.w;
      _n^.child[1]^.h := _n^.h - g_SrcLightmaps[_tri].header.height;
    end;

    // place into first node created
    Result := Insert(_n^.child[0], _treeID);
  end;

  // ---------------------------------

  procedure PackLightmap;

  var
    _dlm  : longint;
    _node : PLMPackNode;

  begin
    _dlm := _ilm;

    repeat
      // if needed create a new tree
      if Length(g_LMPackTrees)-1 < _dlm then
      begin
        // create new tree
        SetLength(g_LMPackTrees, Length(g_LMPackTrees)+1);
        with g_LMPackTrees[Length(g_LMPackTrees)-1] do
        begin
          Node.lmidx := -1;
          Node.child[0] := NIL;
          Node.child[1] := NIL;
          Node.next := NIL;
          Node.x := 0;
          Node.y := 0;
          Node.w := g_LMap_Max_Size;
          Node.h := g_LMap_Max_Size;
          Prev := @Node;
        end;

        // also create new destination lightmap
        SetLength(g_DstLightmaps, Length(g_DstLightmaps)+1);
        g_DstLightmaps[Length(g_DstLightmaps)-1] := ARGB_Create(g_LMap_Max_Size,
                                                                g_LMap_Max_Size);
        if not g_DstLightmaps[Length(g_DstLightmaps)-1].ok then
        begin
          Writeln(ERR06);
          halt(1);
        end;
        with g_DstLightmaps[Length(g_DstLightmaps)-1]
        do Fillchar(data^, header.width * header.height * 4, $FF);
      end;

      // search a place on this tree
      _node := @g_LMPackTrees[_dlm].Node;

      // next tree
      inc(_dlm);
    until Insert(_node, _dlm-1);
  end;

  // ---------------------------------

begin
  // Search a place to store each lightmap using trees.
  // Each tree will be a destination lightmap.
  _ilm := 0;
  for _sub := 0 to g_LMTS.header.nSubsets - 1 do
  begin
    for _tri := 0 to Length(g_SrcLightmaps) - 1 do
    begin
      if (g_SrcLightmaps[_tri].header.width > 0) and
         (g_SrcLightmaps[_tri].header.height > 0) and
         (_sub = g_TriSubsetID[_tri]) and (g_SharedWith[_tri] = -1)
        then PackLightmap;
    end;
    // first destination lightmap for the next subset will
    // be the last destination lightmap of the current subset
    _ilm := Length(g_LMPackTrees)-1;
  end;

  // write how many destination lightmaps are needed
  Writeln(Format(TXT06, [Length(g_LMPackTrees)]));
end;

// ---------------------------------------------------------------------------

procedure BuildNew3DData;

var
  _i, _j, _siz, _sub, _tree, _tsiz : longint;
  _cnt, _cnt2 : cardinal;
  _dst : PLMTS_TexData;
  _treeNode : PLMPackNode;
  _subset : PLMTS_Subset;
  _tfn : array[0..C_LMTS_TEXFNLEN] of char;
  _sfn : string[255];
  _triData : array of TLMTriData;
  _prvData : TLMTriData;

  _soffset : longint;

  // ----------------------------

  procedure CopyTri(_tri: longint);

  var
    _src : pointer;

  begin
    _triData[_cnt].subID  := _sub;
    _triData[_cnt].treeID := _tree;

    // copy triangle
    _src := g_LMTS.tris;
    inc(longint(_src), _tSiz * _tri);
    Move(_src^, _dst^, _tSiz);

    // next
    inc(longint(_dst), _tSiz);
    inc(_cnt);
  end;

  // ----------------------------

  function GetSubsetText(_subsetID: longint): word;

  var _sub : PLMTS_Subset;

  begin
    _sub := g_LMTS.subsets;
    inc(_sub, _subsetID);
    Result := _sub^.TextID1;
  end;

  // ----------------------------

begin
  // create texture data buffer
  try
    g_New_nTexts := g_LMTS.header.nTexts + Length(g_DstLightmaps);
    _siz := g_New_nTexts * SizeOf(TLMTS_TexData);
    GetMem(g_New_TexData, _siz);
  except
    Writeln(ERR06);
    halt(1);
  end;

  // fill it
  _siz := g_LMTS.header.nTexts * SizeOf(TLMTS_TexData);
  Move(g_LMTS.texData^, g_New_TexData^, _siz);
  _dst := g_New_TexData;
  inc(_dst, g_LMTS.header.nTexts);
  for _cnt := 0 to Length(g_DstLightmaps)-1 do // add lightmaps
  begin
    FillChar(_tfn, SizeOf(_tfn), 0);
    _sfn := ExtractFilename(GetDstLightmapFN(_cnt));
    Move(_sfn[1], _tfn[0], length(_sfn));
    Move(_tfn[0], _dst^.FName, SizeOf(_dst^.FName));
    _dst^.Flags := 1;
    inc(_dst);
  end;

  // create the new vertex buffer
  try
    _siz := g_LMTS.header.nTris * 3 * g_LMTS.header.vtxSize;
    GetMem(g_New_Tris, _siz);
    _triData := NIL;
    SetLength(_triData, g_LMTS.header.nTris);
  except
    Writeln(ERR06);
    halt(1);
  end;

  // fill it - first triangles with lightmaps
  _cnt := 0;
  _dst := g_New_Tris;
  _tsiz := 3 * g_LMTS.header.vtxSize;
  for _sub := 0 to g_LMTS.header.nSubsets-1 do
  begin
    for _tree := 0 to Length(g_LMPackTrees)-1 do
    begin
      _treeNode := @g_LMPackTrees[_tree].Node;
      while _treeNode <> NIL do
      begin
        if (_treeNode^.lmidx > -1) then
          if (g_TriSubsetID[_treeNode^.lmidx] = _sub) then
          begin
            CopyTri(_treeNode^.lmidx);
            // now copy triangles with the same lightmaps
            for _j := 0 to Length(g_SharedWith)-1 do
              if (g_SharedWith[_j] = _treeNode^.lmidx) then CopyTri(_j);
          end;
        _treeNode := _treeNode^.next;
      end;
    end;
  end;

  // fill it - now triangles without lightmaps
  _tree := -1;
  for _cnt2 := 0 to g_LMTS.header.nTris-1 do
    if (g_SrcLightmaps[_cnt2].header.width  = 0) or
       (g_SrcLightmaps[_cnt2].header.height = 0) then
    begin
      _sub := LMTS_GetTriSubsetIdx(@g_LMTS, _cnt2);
      CopyTri(_cnt2);
    end;

  // verify
  if (_cnt <> g_LMTS.header.nTris) then
  begin
    Writeln(ERR08);
    Halt(1);
  end;

  // count subsets
  g_New_nSubsets := 1;
  _prvData := _triData[0];
  for _cnt := 1 to Length(_triData)-1 do
  begin
    if (_triData[_cnt].SubID <> _prvData.SubID) or
       (_triData[_cnt].TreeID <> _prvData.TreeID) then inc(g_New_nSubsets);
    _prvData := _triData[_cnt];
  end;

  // create subset buffer
  try
    GetMem(g_New_Subsets, g_New_nSubsets * g_LMTS.header.subSize);
  except
    Writeln(ERR06);
    halt(1);
  end;

  // fill it
  _subset  := g_New_Subsets;
  _soffset := 0;
  _prvData := _triData[0];

  if Length(_triData) = 1 then 
  begin
    _subset^.Offset := 0;
    _subset^.Count := 1;
    _subset^.TextID1 := GetSubsetText(_triData[0].SubID);
    if _triData[0].TreeID >= 0
      then _subset^.TextID2 := g_LMTS.header.nTexts
        else _subset^.TextID2 := $FFFF;
  end;

  for _i := 1 to Length(_triData)-1 do
  begin
    if (_triData[_i].SubID <> _prvData.SubID) or
       (_triData[_i].TreeID <> _prvData.TreeID) or
       (_i = Length(_triData)-1) then
    begin
      _subset^.Offset  := _soffset;
      _subset^.Count   := _i - _soffset;
      if (_i = Length(_triData)-1) then inc(_subset^.Count);
      _subset^.TextID1 := GetSubsetText(_triData[_i-1].SubID);
      if _triData[_i-1].TreeID >= 0
        then _subset^.TextID2 := g_LMTS.header.nTexts + _triData[_i-1].TreeID
          else _subset^.TextID2 := $FFFF;
      // next subset
       _soffset := _i;
      inc(longint(pointer(_subset)), g_LMTS.header.subSize);
    end;
    _prvData := _triData[_i];
  end;

  // Free triangle data buffer
  _triData := NIL;
end;

// ---------------------------------------------------------------------------

procedure FreeNew3DData;

begin
  try
    FreeMem(g_New_TexData, g_New_nTexts * SizeOf(TLMTS_TexData));
    FreeMem(g_New_Tris, g_LMTS.header.nTris * 3 * g_LMTS.header.vtxSize);
    FreeMem(g_New_Subsets, g_New_nSubsets * g_LMTS.header.subSize);
  except
  end;
end;

// ---------------------------------------------------------------------------

procedure Save3DFile;

var
  _fn : string;
  _f  : file;
  _ok : boolean;

  // ----------------------------------------

  function SaveData: boolean;

  var _4cc : cardinal;
      _siz : longint;
      _h : TLMTS_Header;

  begin
    Result := false;

    try
      // write header
      Move(g_LMTS.header, _h, SizeOf(TLMTS_Header));
      _h.nTexts := g_New_nTexts;
      _h.nSubsets := g_New_nSubsets;
      BlockWrite(_f, _h, SizeOf(TLMTS_Header));
      // write "user" data
      if (g_LMTS.usrSize > 0)
        then BlockWrite(_f, g_LMTS.usrData^, g_LMTS.usrSize);
      // write texture file names
      _4cc := C_LMTS_TEXT;
      BlockWrite(_f, _4cc, SizeOf(_4cc));
      _siz := _h.nTexts * SizeOf(TLMTS_TexData);
      BlockWrite(_f, g_New_TexData^, _siz);
      // write subset data
      _4cc := C_LMTS_SUBS;
      BlockWrite(_f, _4cc, SizeOf(_4cc));
      _siz := _h.nSubsets * g_LMTS.header.subSize;
      BlockWrite(_f, g_New_Subsets^, _siz);
      // write vertex data
      _4cc := C_LMTS_TRIS;
      BlockWrite(_f, _4cc, SizeOf(_4cc));
      _siz := _h.nTris * 3 * _h.vtxSize;
      BlockWrite(_f, g_New_Tris^, _siz);
    except
      exit;
    end;

    // all right
    Result := true;
  end;

  // ----------------------------------------

begin
  _ok := false;

  _fn := g_FileNameBase + '.3' + C_EXT_3DFILE;

  // create new data needed
  g_New_nTexts := g_LMTS.header.nTexts + Length(g_DstLightmaps);

  // open file
  if filezOpenBin(_fn, obfWrite, _f) then
  begin
    // save data
    _ok := SaveData;

    // close file
    CloseFile(_f);
  end;

  // all right ?
  if not _ok then
  begin
    Writeln(Format(ERR04, [_fn]));
    halt(1);
  end;
end;

// ---------------------------------------------------------------------------

procedure FreeTrees;

var
  _cnt : longint;
  _node, _n : PLMPackNode;

begin
  // free trees' nodes
  if length(g_LMPackTrees) > 0 then
    for _cnt := 0 to length(g_LMPackTrees)-1 do
    begin
      _node := @g_LMPackTrees[_cnt].Node.Next^;
      while _node <> NIL do
      begin
        _n := _node^.next;
        FreeMem(_node, SizeOf(TLMPackNode));
        _node := _n;
      end;
    end;

  // free array of trees
  g_LMPackTrees := NIL;
end;

// ---------------------------------------------------------------------------

begin
  // title
  ShowAppTitle;

  // check command line parameters
  CheckParams;

  // load 3d world
  Load3DFile;

  // load source lightmaps
  LoadSrcLightmaps;

  // pre-filter
  PreFilterLightmaps;

  // optimize lightmaps
  OptimizeLightmaps;

  // share lightmaps
  ShareLightmaps;

  // pack lightmaps
  PackLightmaps;

  // build new 3D data
  BuildNew3DData;

  // Save 3d world
  Save3DFile;

  // free new 3d data
  FreeNew3DData;

  // free 3d world
  LMTS_Free(g_LMTS);

  // save destination lightmaps
  SaveDstLightmaps;

  // free all lightmaps
  FreeAllLightmaps;

  // free trees
  FreeTrees;

  // done! :)
  Writeln(TXT09);
end.
