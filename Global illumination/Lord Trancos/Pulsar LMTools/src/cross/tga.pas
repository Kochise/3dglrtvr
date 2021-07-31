(*

  DESC:

   TGA - Unit for working with Truevision TARGA files.
   Copyright © 2002-2003 Lord Trancos / Pulsar Studio.
   <lordtrancos@softhome.net>

  API:

   NONE

*)

unit tga;

// ---------------------------------------------------------------------------

interface

uses filez, bitmap;

const
  C_TGA_TYPE_COLOR_MAPPED         = 1;
  C_TGA_TYPE_TRUE_COLOR           = 2;  // <-- only this type is supported!
  C_TGA_TYPE_BLACK_AND_WHITE      = 3;
  C_TGA_TYPE_RLE_COLOR_MAPPED     = 9;
  C_TGA_TYPE_RLE_TRUE_COLOR       = 10;
  C_TGA_TYPE_RLE_BLACK_AND_WHITE  = 11;

  C_TGA_ORDER_MASK         = $30;
  C_TGA_ORDER_BOTTOM_LEFT  = $00;
  C_TGA_ORDER_BOTTOM_RIGHT = $10;
  C_TGA_ORDER_TOP_LEFT     = $20;
  C_TGA_ORDER_TOP_RIGHT    = $30;
  C_TGA_ORDER_RIGHT        = $10;
  C_TGA_ORDER_TOP          = $20;

  C_TGA_MAX_IMAGEID_LENGTH = 255;

type
  TTGA_ColorMapSpecification = packed record
    FirstEntryIndex   : word;
    ColorMapLength    : word;
    ColorMapEntrySize : byte;
  end;

  TTGA_ImageSpecification = packed record
    XOrigin         : word;
    YOrigin         : word;
    ImageWidth      : word;
    ImageHeight     : word;
    PixelDepth      : byte;
    ImageDescriptor : byte;
  end;

  TTGA_Header = packed record
    IDLength     : byte;
    ColorMapType : byte;
    ImageType    : byte;
    ColorMapSpec : TTGA_ColorMapSpecification;
    ImageSpec    : TTGA_ImageSpecification;
  end;

  TTGA = record
    header  : TTGA_Header;
    imageID : array[0..C_TGA_MAX_IMAGEID_LENGTH-1] of char;
    data    : pointer;
    ok      : boolean;
  end;

function Get32ARGBTGAHeader(_width, _height: word): TTGA_Header;

procedure TGA_Reset(var _tga : TTGA);
procedure TGA_Free(var _tga : TTGA);
function  TGA_Create(_width, _height: word): TTGA;
function  TGA_LoadFromFile(var _f: file): TTGA;
function  TGA_Load(_fn : string): TTGA;
function  TGA_SaveToFile(var _f: file; _tga: TTGA): boolean;
function  TGA_Save(_fn: string; _tga: TTGA): boolean;
function  TGA_Flip(_tga: TTGA): boolean;
function  TGA_Mirror(_tga: TTGA): boolean;
procedure TGA_ChangeOrder(var _tga: TTGA; _order: byte);

// ---------------------------------------------------------------------------

implementation

// ---------------------------------------------------------------------------

function TGA_Create(_width, _height: word): TTGA;

var
  _siz : longint;

begin
  TGA_Reset(Result);

  // get memory
  try
    _siz := 4 * _width * _height;
    if (_siz <> 0) then GetMem(Result.data, _siz);
  except
    exit;
  end;

  // fill header
  Result.header := Get32ARGBTGAHeader(_width, _height);

  // all right
  Result.ok := true;
end;

// ---------------------------------------------------------------------------

function Get32ARGBTGAHeader(_width, _height: word): TTGA_Header;

begin
  FillChar(Result, SizeOf(Result), 0);
  with Result do
  begin
    ImageType := C_TGA_TYPE_TRUE_COLOR;
    ImageSpec.ImageWidth  := _width;
    ImageSpec.ImageHeight := _height;
    ImageSpec.PixelDepth  := 32;
    ImageSpec.ImageDescriptor := C_TGA_ORDER_TOP_LEFT + $F;
  end;
end;

// ---------------------------------------------------------------------------

procedure TGA_Reset(var _tga : TTGA);

begin
  FillChar(_tga, SizeOf(_tga), 0);
end;

// ---------------------------------------------------------------------------

procedure TGA_Free(var _tga : TTGA);

var
  _siz : longint;

begin
  try
    with _tga.header.ImageSpec do _siz := 4 * ImageWidth * ImageHeight;
    if (_siz <> 0) then FreeMem(_tga.data, _siz);
  except
  end;
  TGA_Reset(_tga);
end;

// ---------------------------------------------------------------------------

function TGA_LoadFromFile(var _f: file): TTGA;

// only 32 bit tgas are supported

var _siz : longint;

begin
  TGA_Reset(Result);

  try
    // read header
    BlockRead(_f, Result.header, SizeOf(TTGA_Header));
    // verify
    if (Result.header.ColorMapType <> 0) or
       (Result.header.ImageType <> C_TGA_TYPE_TRUE_COLOR) or
       (Result.header.ImageSpec.PixelDepth <> 32) then exit;
    // read image id
    if (Result.header.IDLength > 0)
      then BlockRead(_f, Result.imageID, Result.header.IDLength);
    // read data
    with Result.header.ImageSpec do _siz := 4 * ImageWidth * ImageHeight;
    if (_siz <> 0) then
    begin
      GetMem(Result.data, _siz);
      BlockRead(_f, Result.data^, _siz);
    end;
  except
    TGA_Free(Result);
    exit;
  end;

  // all right
  Result.ok := true;
end;

// ---------------------------------------------------------------------------

function TGA_Load(_fn : string): TTGA;

// only 32 bit tgas are supported

var _f : file;

begin
  TGA_Reset(Result);

  // open file
  if not filezOpenBin(_fn, obfRead, _f) then exit;

  // load data
  Result := TGA_LoadFromFile(_f);

  // close file
  CloseFile(_f);
end;

// ---------------------------------------------------------------------------

function TGA_SaveToFile(var _f: file; _tga: TTGA): boolean;

// allows 16 and 32 bit targa files

var
  _siz : longint;

begin
  Result := false;

  // check argb
  if not _tga.ok then exit;

  try
    // write header
    BlockWrite(_f, _tga.header, SizeOf(TTGA_Header));
    // write bitmap
    with _tga.header.ImageSpec do
    case PixelDepth of
      16 : _siz := 2 * ImageWidth * ImageHeight;
      32 : _siz := 4 * ImageWidth * ImageHeight;
      else exit;
    end;
    if (_siz <> 0) then BlockWrite(_f, _tga.data^, _siz);
  except
    exit;
  end;

  // all right
  Result := true;
end;

// ---------------------------------------------------------------------------

function TGA_Save(_fn: string; _tga: TTGA): boolean;

// allows 16 and 32 bit targa files

var _f : file;

begin
  Result := false;

  // check argb
  if not _tga.ok then exit;

  // open file
  if not filezOpenBin(_fn, obfWrite, _f) then exit;

  // save data
  Result := TGA_SaveToFile(_f, _tga);

  // close file
  CloseFile(_f);
end;

// ---------------------------------------------------------------------------

function TGA_Flip(_tga: TTGA): boolean;

begin
  Result := false;

  // it's ok
  if not _tga.ok then exit;

  // flip image
  with _tga.header.ImageSpec
  do Result := bitmap_flip(ImageHeight, ImageWidth * 4, _tga.data);
end;

// ---------------------------------------------------------------------------

function TGA_Mirror(_tga: TTGA): boolean;

begin
  Result := false;

  // it's ok
  if not _tga.ok then exit;

  // mirror image
  with _tga.header.ImageSpec
  do Result := bitmap_mirror(ImageWidth, ImageHeight, 32, _tga.data);
end;

// ---------------------------------------------------------------------------

procedure TGA_ChangeOrder(var _tga: TTGA; _order: byte);

// _order - desired order (use C_TGA_ORDER_*).

var
  _or : byte;
  _sb, _sl, _db, _dl : byte;

begin
  // find changes
  _or := (_tga.header.ImageSpec.ImageDescriptor and C_TGA_ORDER_MASK);
  _sb := (_or and C_TGA_ORDER_TOP);
  _sl := (_or and C_TGA_ORDER_RIGHT);
  _db := (_order and C_TGA_ORDER_TOP);
  _dl := (_order and C_TGA_ORDER_RIGHT);

  // change
  if (_sb <> _db) then TGA_Flip(_tga);
  if (_sl <> _dl) then TGA_Mirror(_tga);
  with _tga.header.ImageSpec
  do ImageDescriptor := (ImageDescriptor and $F) + _order;
end;

// ---------------------------------------------------------------------------

end.
