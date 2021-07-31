(*

  DESC:

   WBMP - Unit for working with Windows Bitmap files.
   Copyright © 2003 Lord Trancos / Pulsar Studio.
   <lordtrancos@softhome.net>

  API:

   NONE

*)

unit wbmp;

// ---------------------------------------------------------------------------

interface

uses filez; 

const
  C_WBMP_ID  = $4D42;

type
  TWBMP_FileHeader = packed record
    ID : word;                // C_BMP_ID
    Size : cardinal;          // file size
    Reserved : cardinal;      // 0
    OffsetBits : cardinal;    // Offset from beginning of file
                              // to the beginning of the bitmap data
  end;

  TWBMP_InfoHeader = packed record
    Size : cardinal;            // size of TBMP_InfoHeader
    Width : cardinal;           // img. width
    Height : cardinal;          // img. height
    Planes : word;              // planes
    BitCount : word;            // bits per pixel
    Compression : cardinal;     // compression 
    SizeImage : cardinal;       // bitmap data in bytes
    XPelsPerMeter : cardinal;   // Horizontal resolution in pixel per meter
    YPelsPerMeter : cardinal;   // Vertical resolution in pixel per meter
    ColorsUsed : cardinal;      // number of colors used
    ColorsImportant : cardinal; // number of imp. colors (0 = all)
  end;

  TWBMP = record
    fileH : TWBMP_FileHeader;
    infoH : TWBMP_InfoHeader;
    data : pointer;
    ok : boolean;
  end;

procedure WBMP_Reset(var _bmp: TWBMP);
procedure WBMP_Free(var _bmp: TWBMP);
function  WBMP_Create24bpp(_width, _height: longint): TWBMP;
function  WBMP_SaveToFile(var _f: file; _wbmp: TWBMP): boolean;
function  WBMP_Save(_fn: string; _wbmp: TWBMP): boolean;

// ---------------------------------------------------------------------------

implementation

// ---------------------------------------------------------------------------

procedure WBMP_Reset(var _bmp: TWBMP);

begin
  Fillchar(_bmp, SizeOf(_bmp), 0);
end;

// ---------------------------------------------------------------------------

procedure WBMP_Free(var _bmp: TWBMP);

begin
  try
    if assigned(_bmp.data) then FreeMem(_bmp.data, _bmp.infoH.SizeImage);
  except
  end;  
  WBMP_Reset(_bmp);
end;

// ---------------------------------------------------------------------------

function WBMP_Create24bpp(_width, _height: longint): TWBMP;

var _bpl : longint;

begin
  // reset
  WBMP_Reset(Result);  

  // get bytes per line
  _bpl := _width * 3;
  while _bpl mod 4 <> 0 do inc(_bpl);

  // fill info header
  With Result.infoH do
  begin
    Size   := SizeOf(TWBMP_InfoHeader);
    Width  := _width;
    Height := _height;
    Planes := 1;
    BitCount := 24;
    Compression := 0;
    SizeImage   := _height * _bpl;
    XPelsPerMeter := 0;
    YPelsPerMeter := 0;
    ColorsUsed      := 0;
    ColorsImportant := 0;
  end;  

  // fill file header
  With Result.fileH do
  begin
    OffsetBits := SizeOf(TWBMP_FileHeader) + SizeOf(TWBMP_InfoHeader);
    Reserved := 0;
    Size := OffsetBits + Result.infoH.SizeImage;
    ID   := C_WBMP_ID;
  end;

  // create buffer for the image
  try
    GetMem(Result.data, Result.infoH.SizeImage);
  except
    WBMP_Reset(Result);      
    exit;
  end;

  // all right
  Result.ok := true;
end;

// ---------------------------------------------------------------------------

function WBMP_SaveToFile(var _f: file; _wbmp: TWBMP): boolean;

// allows 24 bpp files

begin
  Result := false;

  // check 
  if not _wbmp.ok then exit;

  // write
  try
    // write file header
    BlockWrite(_f, _wbmp.fileH, SizeOf(TWBMP_FileHeader));
    // write info header
    BlockWrite(_f, _wbmp.infoH, SizeOf(TWBMP_InfoHeader));
    // write bitmap
    BlockWrite(_f, _wbmp.data^, _wbmp.infoH.SizeImage);
  except
    exit;
  end;

  // all right
  Result := true;
end;

// ---------------------------------------------------------------------------

function WBMP_Save(_fn: string; _wbmp: TWBMP): boolean;

// allows 24 bpp files

var _f : file;

begin
  Result := false;

  // check
  if not _wbmp.ok then exit;

  // open file
  if not filezOpenBin(_fn, obfWrite, _f) then exit;

  // save data
  Result := WBMP_SaveToFile(_f, _wbmp);

  // close file
  CloseFile(_f);
end;

// ---------------------------------------------------------------------------

end.
