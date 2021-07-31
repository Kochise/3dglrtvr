(*

  DESC:

   LMTS - Unit for working with Pulsar Studio LMTS files.
   Copyright © 2002-2003 Lord Trancos / Pulsar Studio.
   <lordtrancos@softhome.net>

  API:

   NONE

*)

unit lmts;

// ---------------------------------------------------------------------------

interface

uses filez;

const
  C_LMTS_ID   = $53544D4C;
  C_LMTS_VER  = 4;
  C_LMTS_SUBS = $53425553;
  C_LMTS_TEXT = $54584554;
  C_LMTS_TRIS = $53495254;

  C_LMTS_TEXFNLEN = 255; // max texture filename length

type
  PLMTS_Header = ^TLMTS_Header;
  TLMTS_Header = packed record
    ID         : cardinal;
    Ver        : cardinal;
    headerSize : cardinal;
    nTexts     : word; // # of textures
    nSubsets   : word;
    nTris      : cardinal;
    subSize    : word;
    vtxSize    : word;
  end;

  PLMTS_TexData = ^TLMTS_TexData;
  TLMTS_TexData = packed record
    FName : array[0..C_LMTS_TEXFNLEN] of char;
    Flags : word;
  end;

  PLMTS_Subset = ^TLMTS_Subset;
  TLMTS_Subset = packed record
    Offset  : longint;
    Count   : longint;
    TextID1 : word;
    TextID2 : word;
  end;

  PLMTS_Vertex = ^TLMTS_Vertex;
  TLMTS_Vertex = packed record
    x, y, z : single;
    u1, v1, u2, v2 : single;
  end;

  PLMTS = ^TLMTS;
  TLMTS = record
    header  : TLMTS_Header;
    usrData : pointer;
    usrSize : cardinal;
    texData : pointer;
    subsets : pointer;
    tris    : pointer;
    ok      : boolean;
  end;

procedure LMTS_Reset(var _lmts : TLMTS);
procedure LMTS_Free(var _lmts : TLMTS);
function  LMTS_Load(_fn : string): TLMTS;
function  LMTS_Save(_fn : string; _lmts: TLMTS): boolean;
function  LMTS_GetTriSubsetIdx(_lmts: PLMTS; _triIdx: longint): longint;
procedure LMTS_GetTriTextureID(_lmts: PLMTS; _subsetIdx: longint;
                               var _t1, _t2: word);

// ---------------------------------------------------------------------------

implementation

// ---------------------------------------------------------------------------

procedure LMTS_Reset(var _lmts : TLMTS);

begin
  FillChar(_lmts, SizeOf(_lmts), 0);
end;

// ---------------------------------------------------------------------------

procedure LMTS_Free(var _lmts : TLMTS);

begin
  try
    if (_lmts.usrData <> NIL)
    then FreeMem(_lmts.usrData, _lmts.header.headerSize - SizeOf(TLMTS_Header));

    if (_lmts.texData <> NIL)
    then FreeMem(_lmts.texData, _lmts.header.nTexts * SizeOf(TLMTS_TexData));

    if (_lmts.subsets <> NIL)
    then FreeMem(_lmts.subsets, _lmts.header.nSubsets * _lmts.header.subSize);

    if (_lmts.tris <> NIL)
    then FreeMem(_lmts.tris, _lmts.header.nTris * 3 * _lmts.header.vtxSize);
  except
  end;
  LMTS_Reset(_lmts);
end;

// ---------------------------------------------------------------------------

function LMTS_Load(_fn : string): TLMTS;

var _f : file;

  // ----------------------------------------

  procedure LoadData(var _lmts: TLMTS);

  var _4cc : cardinal;
      _siz : longint;

  begin
    try
      // read header
      BlockRead(_f, _lmts.header, SizeOf(TLMTS_Header));
      // verify
      if (_lmts.header.ID <> C_LMTS_ID) or
         (_lmts.header.Ver <> C_LMTS_VER) or
         (_lmts.header.headerSize < SizeOf(TLMTS_Header)) or
         (_lmts.header.subSize < SizeOf(TLMTS_Subset)) or
         (_lmts.header.vtxSize < SizeOf(TLMTS_Vertex)) then exit;
      // read "user" data
      _lmts.usrSize := _lmts.header.headerSize - SizeOf(TLMTS_Header);
      if (_lmts.usrSize > 0) then
      begin
        GetMem(_lmts.usrData, _lmts.usrSize);
        BlockRead(_f, _lmts.usrData^, _lmts.usrSize);
      end;
      // read texture FileNames data
      BlockRead(_f, _4cc, SizeOf(_4cc));
      if (_4cc <> C_LMTS_TEXT) then exit;
      _siz := _lmts.header.nTexts * SizeOf(TLMTS_TexData);
      GetMem(_lmts.texData, _siz);
      BlockRead(_f, _lmts.texData^, _siz);
      // read subset data
      BlockRead(_f, _4cc, SizeOf(_4cc));
      if (_4cc <> C_LMTS_SUBS) then exit;
      _siz := _lmts.header.nSubsets * _lmts.header.subSize;
      GetMem(_lmts.subsets, _siz);
      BlockRead(_f, _lmts.subsets^, _siz);
      // read vertex data
      BlockRead(_f, _4cc, SizeOf(_4cc));
      if (_4cc <> C_LMTS_TRIS) then exit;
      _siz := _lmts.header.nTris * 3 * _lmts.header.vtxSize;
      GetMem(_lmts.tris, _siz);
      BlockRead(_f, _lmts.tris^, _siz);
    except
      exit;
    end;

    // all right
    _lmts.ok := true;
  end;

  // ----------------------------------------

begin
  LMTS_Reset(Result);

  // open file
  if not filezOpenBin(_fn, obfRead, _f) then exit;

  // load data
  LoadData(Result);
  if not Result.ok then LMTS_Free(Result);

  // close file
  CloseFile(_f);
end;

// ---------------------------------------------------------------------------

function  LMTS_Save(_fn : string; _lmts: TLMTS): boolean;

var _f : file;

  // ----------------------------------------

  function SaveData: boolean;

  var _4cc : cardinal;
      _siz : longint;

  begin
    Result := false;

    try
      // write header
      BlockWrite(_f, _lmts.header, SizeOf(TLMTS_Header));
      // write "user" data
      if (_lmts.usrSize > 0)
        then BlockWrite(_f, _lmts.usrData^, _lmts.usrSize);
      // write texture file names
      _4cc := C_LMTS_TEXT;
      BlockWrite(_f, _4cc, SizeOf(_4cc));
      _siz := _lmts.header.nTexts * SizeOf(TLMTS_TexData);
      BlockWrite(_f, _lmts.texData^, _siz);
      // write subset data
      _4cc := C_LMTS_SUBS;
      BlockWrite(_f, _4cc, SizeOf(_4cc));
      _siz := _lmts.header.nSubsets * _lmts.header.subSize;
      BlockWrite(_f, _lmts.subsets^, _siz);
      // write vertex data
      _4cc := C_LMTS_TRIS;
      BlockWrite(_f, _4cc, SizeOf(_4cc));
      _siz := _lmts.header.nTris * 3 * _lmts.header.vtxSize;
      BlockWrite(_f, _lmts.tris^, _siz);
    except
      exit;
    end;

    // all right
    Result := true;
  end;

  // ----------------------------------------

begin
  Result := false;

  // check lmts
  if not _lmts.ok then exit;

  // open file
  if not filezOpenBin(_fn, obfWrite, _f) then exit;

  // save data
  Result := SaveData;

  // close file
  CloseFile(_f);
end;

// ---------------------------------------------------------------------------

function  LMTS_GetTriSubsetIdx(_lmts: PLMTS; _triIdx: longint): longint;

var
  _cnt : longint;
  _idx : longint;
  _sp  : pointer;
  _sub : PLMTS_Subset;

begin
  Result := -1;

  if (not _lmts^.ok) or (cardinal(_triIdx) >= _lmts^.header.nTris) then exit;

  _idx := 0;
  _sp := _lmts^.subsets;
  for _cnt := 0 to _lmts^.header.nSubsets - 1 do
  begin
    _sub := _sp;
    if (_triIdx >= _idx) and (_triIdx <= _idx + _sub^.count - 1) then
    begin
      // subset found
      Result := _cnt;
      break;
    end;
    // next subset
    inc(_idx, _sub^.count);
    inc(longint(_sp), _lmts^.header.subSize);
  end;
end;

// ---------------------------------------------------------------------------

procedure LMTS_GetTriTextureID(_lmts: PLMTS; _subsetIdx: longint;
                               var _t1, _t2: word);

// returns $FFFF if subset index is invalid or if there isn't a texture.
var
  _cur : PLMTS_Subset;

begin
  _t1 := $FFFF;
  _t2 := $FFFF;

  if (_subsetIdx < 0) or (_subsetIdx >= _lmts^.header.nSubsets) then exit;

  _cur := _lmts^.subsets;
  inc(longint(pointer(_cur)), _lmts^.header.subSize * _subsetIdx);

  _t1 := _cur^.TextID1;
  _t2 := _cur^.TextID2;
end;

// ---------------------------------------------------------------------------

end.
