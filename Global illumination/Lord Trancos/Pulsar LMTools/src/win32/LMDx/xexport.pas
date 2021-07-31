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

unit xexport;

// ------------------------------------------------------------------

interface

uses Windows, SysUtils, DirectXGraphics, DX8NOIDX, DX8TEXT, DX8LPE,
     lmts, tga, filez, common;

var
  g_Exp_PreserveTFN : boolean; // preserve texture filenames

procedure ExportToLMTS(_wnd: hWnd; _path, _fn: string);
procedure ExportToTGAs(_wnd: hWnd; _path: string);

// ------------------------------------------------------------------

implementation

// ------------------------------------------------------------------

function SaveNonIndexedData(var _f: file; _nidx: PNonIndex): boolean;

var
  _h : TLMTS_Header;
  _id : cardinal;
  _cnt : longint;
  _tid : pWord;
  _tri : TLMTS_Vertex;
  _trc : pointer;
  _tfn : TLMTS_TexData;
  _sfn : string[255];
  _sub : TLMTS_Subset;
  _sbc : PNISub;

begin
  Result := false;

  // save data
  try
    // header
    _h.ID         := C_LMTS_ID;
    _h.Ver        := C_LMTS_VER;
    _h.headerSize := SizeOf(_h);
    _h.nTris      := _nidx^.nTri;
    _h.nTexts     := TE_Count;
    _h.nSubsets   := _nidx^.nSub;
    _h.subSize    := SizeOf(TLMTS_Subset);
    _h.vtxSize    := SizeOf(TLMTS_Vertex);
    // save header
    blockWrite(_f, _h, SizeOf(_h));

    // save texture info
    _id := C_LMTS_TEXT;
    blockWrite(_f, _id, SizeOf(_id));

    for _cnt := 0 to _h.nTexts-1 do
    begin
      _sfn := ExtractFilename(TE_GetFilename(_cnt));
      if not g_Exp_PreserveTFN then _sfn := ChangeFileExt(_sfn, '.TGA');
      FillChar(_tfn, SizeOf(_tfn), 0);
      Move(_sfn[1], _tfn.FName[0], length(_sfn));
      blockWrite(_f, _tfn, SizeOf(_tfn));
    end;

    // save subset table
    _id := C_LMTS_SUBS;
    blockWrite(_f, _id, SizeOf(_id));
    _sbc := _nidx^.subs;
    _tid := _nidx^.txts;
    for _cnt := 0 to _nidx^.nSub - 1 do
    begin
      _sub.Offset  := _sbc^.offset;
      _sub.Count   := _sbc^.count;
      _sub.TextID1 := _tid^;
      _sub.TextID2 := C_NO_TEXTURE;
      blockWrite(_f, _sub, SizeOf(TLMTS_Subset));
      // next subset
      inc(_sbc);
      inc(_tid);
    end;

    // save triangle array ID
    _id := C_LMTS_TRIS;
    blockWrite(_f, _id, SizeOf(_id));
    _trc := _nidx^.tris;
    _tri.u2 := 0;
    _tri.v2 := 0;
    for _cnt := 0 to (_nidx^.nTri * 3) - 1 do
    begin
      Move(_trc^, _tri, SizeOf(single)*3);
      if _nidx^.fvf = C_VALID_FVF2
        then inc(longint(_trc), SizeOf(single)*6)
          else inc(longint(_trc), SizeOf(single)*3);
      Move(_trc^, _tri.u1, SizeOf(single));
      inc(longint(_trc), SizeOf(single));
      Move(_trc^, _tri.v1, SizeOf(single));
      inc(longint(_trc), SizeOf(single));
      // write
      blockWrite(_f, _tri, SizeOf(_tri));
    end;
  except
    exit;
  end;

  // all right
  Result := true;
end;

// ------------------------------------------------------------------

function SaveNonIndexed(_fn: string; _nidx: PNonIndex): boolean;

var _f : file;

begin
  Result := false;

  // open file
  if not filezOpenBin(_fn, obfWrite, _f) then exit;

  // save data
  Result := SaveNonIndexedData(_f, _nidx);

  // close file
  CloseFile(_f);
end;

// ------------------------------------------------------------------

procedure ExportToLMTS(_wnd: hWnd; _path, _fn: string);

var
  _nidx : TNonIndex;

begin
  // convert mesh to non indexed mesh
  resetNonIndexed(_nidx);
  with G_Mesh0 do if not indexedToNonIndexed(Mesh, Mats, Txts, _nidx) then
  begin
    MessageBox(_wnd, ERR06, ERRXX, MB_ICONERROR);
    exit;
  end;

  // save non indexed as LMTS
  _fn := _path + ChangeFileExt(_fn, C_EXPORT_EXT3D);
  if not SaveNonIndexed(_fn, @_nidx) then
  begin
    MessageBox(_wnd, pChar(ERR07 + _fn), ERRXX, MB_ICONERROR);
  end;

  // free non indexed
  freeNonIndexed(_nidx);
end;

// ------------------------------------------------------------------

function SaveTextureToFile(var _f: file;
                           _s: IDirect3DSurface8;
                           _width, _height: longint): boolean;

var
  _r : TD3DLocked_Rect;
  _p : pointer;
  _y : longint;
  _h : TTGA_Header;

begin
  Result := false;

  // lock surface
  if failed(_s.LockRect(_r, NIL, D3DLOCK_READONLY)) then exit;
  _p := _r.pBits;

  // save data
  try
    // header
    _h := Get32ARGBTGAHeader(_width, _height);
    // save header
    blockWrite(_f, _h, SizeOf(_h));
    // save line by line
    for _y := 0 to _height - 1 do
    begin
      blockWrite(_f, _p^, _width*4);
      inc(longint(_p), _r.Pitch);
    end;
  except
    // unlock surface
    _s.UnlockRect;
    exit;
  end;

  // unlock surface
  _s.UnlockRect;

  // all right
  Result := true;
end;

// ------------------------------------------------------------------

function ExportTexture(_fn: string; _s: IDirect3DSurface8;
                       _width, _height: longint): boolean;

var _f : file;

begin
  Result := false;

  // open file
  if not filezOpenBin(_fn, obfWrite, _f) then exit;

  // save data
  Result := SaveTextureToFile(_f, _s, _width, _height);

  // close file
  CloseFile(_f);
end;

// ------------------------------------------------------------------

procedure ExportToTGAs(_wnd: hWnd; _path: string);

var
  _cnt : longint;
  _sn  : string;
  _fn  : string;
  _srf : IDirect3DSurface8;
  _w, _h : longint;

begin
  for _cnt := 0 to G_TE_MaxTextNum - 1 do
    if assigned(G_TE_Textures[_cnt].Text) then
    begin
      _sn := TE_GetFilename(_cnt);

      // load texture as a 32bit ARGB surface
      _srf := LPE_LoadSurfaceFromFile(pChar(_sn), _w, _h, D3DFMT_A8R8G8B8);
      if _srf = NIL then
      begin
        MessageBox(_wnd, pChar(ERR08 + _sn), ERRXX, MB_ICONERROR);
        continue;
      end;

      // save surface
      _fn := _path + ChangeFileExt(ExtractFileName(_sn), C_EXPORT_EXT2D);
      if not ExportTexture(_fn, _srf, _w, _h) then
      begin
        _srf := NIL;
        MessageBox(_wnd, pChar(ERR07 + _fn), ERRXX, MB_ICONERROR);
        continue;
      end;

      // free surface
      _srf := NIL;
    end;
end;

// ------------------------------------------------------------------

end.
