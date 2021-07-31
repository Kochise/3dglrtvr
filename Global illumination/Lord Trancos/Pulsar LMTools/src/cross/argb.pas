(*

  DESC:

   ARGB - Unit for working with Pulsar Studio ARGB files.
   Copyright © 2002-2003 Lord Trancos / Pulsar Studio.
   <lordtrancos@softhome.net>

  API:

   NONE

*)

unit argb;

// ---------------------------------------------------------------------------

interface

uses filez, bitmap;

const
  C_ARGB_ID   = $42475241;
  C_ARGB_VER  = 0;

type
  TARGB_Header = packed record
    ID         : cardinal;
    Ver        : cardinal;
    width      : word;
    height     : word;
  end;

  TARGB = record
    header : TARGB_Header;
    data   : pointer;
    ok     : boolean;
  end;

procedure ARGB_Reset(var _argb : TARGB);
procedure ARGB_Free(var _argb : TARGB);
function  ARGB_Create(_width, _height: word): TARGB;
function  ARGB_LoadFromFile(var _f: file): TARGB;
function  ARGB_Load(_fn : string): TARGB;
function  ARGB_SaveToFile(var _f: file; _argb: TARGB): boolean;
function  ARGB_Save(_fn : string; _argb: TARGB): boolean;
function  ARGB_Draw(_src, _dst : TARGB;
                    _dstX, _dstY : cardinal): boolean;
function  ARGB_Flip(_argb: TARGB): boolean;
function  ARGB_Mirror(_argb: TARGB): boolean;

// ---------------------------------------------------------------------------

implementation

// ---------------------------------------------------------------------------

procedure ARGB_Reset(var _argb : TARGB);

begin
  FillChar(_argb, SizeOf(_argb), 0);
end;

// ---------------------------------------------------------------------------

procedure ARGB_Free(var _argb : TARGB);

var
  _siz : longint;

begin
  try
    _siz := 4 * _argb.header.width * _argb.header.height;
    if (_siz <> 0) then FreeMem(_argb.data, _siz);
  except
  end;
  ARGB_Reset(_argb);
end;

// ---------------------------------------------------------------------------

function ARGB_Create(_width, _height: word): TARGB;

var
  _siz : longint;

begin
  ARGB_Reset(Result);

  try
    _siz := 4 * _width * _height;
    if (_siz <> 0) then GetMem(Result.data, _siz);
  except
    exit;
  end;

  // fill header
  with Result.header do
  begin
    ID     := C_ARGB_ID;
    Ver    := C_ARGB_VER;
    width  := _width;
    height := _height;
  end;

  // all right
  Result.ok := true;
end;

// ---------------------------------------------------------------------------

function ARGB_LoadFromFile(var _f: file): TARGB;

var _siz : longint;

begin
  ARGB_Reset(Result);

  try
    // read header
    BlockRead(_f, Result.header, SizeOf(TARGB_Header));
    // verify
    if (Result.header.ID <> C_ARGB_ID) or
       (Result.header.Ver <> C_ARGB_VER) then exit;
    // read bitmap
    _siz := 4 * Result.header.width * Result.header.height;
    if (_siz <> 0) then
    begin
      GetMem(Result.data, _siz);
      BlockRead(_f, Result.data^, _siz);
    end;
  except
    ARGB_Free(Result);
    exit;
  end;

  // all right
  Result.ok := true;
end;

// ---------------------------------------------------------------------------

function ARGB_Load(_fn : string): TARGB;

var _f : file;

begin
  ARGB_Reset(Result);

  // open file
  if not filezOpenBin(_fn, obfRead, _f) then exit;

  // load data
  Result := ARGB_LoadFromFile(_f);

  // close file
  CloseFile(_f);
end;

// ---------------------------------------------------------------------------

function ARGB_SaveToFile(var _f: file; _argb: TARGB): boolean;

var
  _siz : longint;

begin
  Result := false;

  // check argb
  if not _argb.ok then exit;

  try
    // write header
    BlockWrite(_f, _argb.header, SizeOf(TARGB_Header));
    // write bitmap
    _siz := 4 * _argb.header.width * _argb.header.height;
    if (_siz <> 0) then BlockWrite(_f, _argb.data^, _siz);
  except
    exit;
  end;

  // all right
  Result := true;
end;

// ---------------------------------------------------------------------------

function ARGB_Save(_fn: string; _argb: TARGB): boolean;

var _f : file;

begin
  Result := false;

  // check argb
  if not _argb.ok then exit;

  // open file
  if not filezOpenBin(_fn, obfWrite, _f) then exit;

  // save data
  Result := ARGB_SaveToFile(_f, _argb);

  // close file
  CloseFile(_f);
end;

// ---------------------------------------------------------------------------

function ARGB_Draw(_src, _dst : TARGB;
                   _dstX, _dstY : cardinal): boolean;

var
  _cnt : longint;
  _sc, _dc : pointer;

begin
  Result := false;

  // check
  if (_dstX + _src.header.width  > _dst.header.width) or
     (_dstY + _src.header.height > _dst.header.height) then exit;

  // draw
  _sc := _src.data;
  _dc := _dst.data;
  inc(longint(_dc), ((_dstY * _dst.header.width) + _dstX) * 4);
  for _cnt := 0 to _src.header.height-1 do
  begin
    // draw line
    Move(_sc^, _dc^, _src.header.width * 4);
    // next line
    inc(longint(_sc), _src.header.width * 4);
    inc(longint(_dc), _dst.header.width * 4);    
  end;
end;

// ---------------------------------------------------------------------------

function  ARGB_Flip(_argb: TARGB): boolean;

begin
  Result := false;

  // it's ok
  if not _argb.ok then exit;

  // flip image
  Result := bitmap_flip(_argb.header.Height, _argb.header.Width * 4,
                        _argb.data);
end;

// ---------------------------------------------------------------------------

function ARGB_Mirror(_argb: TARGB): boolean;

begin
  Result := false;

  // it's ok
  if not _argb.ok then exit;

  // mirror image
  Result := bitmap_mirror(_argb.header.Height, _argb.header.Width,
                          32, _argb.data);
end;

// ---------------------------------------------------------------------------

end.
