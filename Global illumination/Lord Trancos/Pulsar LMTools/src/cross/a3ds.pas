(*

  DESC:

   A3DS - Unit for working with Autodesk 3D Studio (.3DS) files.
   Copyright © 2003 Lord Trancos / Pulsar Studio.
   <lordtrancos@softhome.net>

  API:

   NONE

*)

unit a3ds;

// ---------------------------------------------------------------------------

interface

uses filez;

Const
  C_3DS_VERSION = 3; // 3DS version supported

  // chunks
  C_3DSCHK_MAIN    = $4D4D; // first chunk
  C_3DSCHK_VER     = $0002; // version
  C_3DSCHK_3DEDIT  = $3D3D; // 3d editor
  C_3DSCHK_OBJECT  = $4000; // object
  C_3DSCHK_MESH    = $4100; // mesh
  C_3DSCHK_VTXLST  = $4110; // vertex list
  C_3DSCHK_FACELST = $4120; // face list
  C_3DSCHK_FACDESC = $4130; // faces description
  C_3DSCHK_TXTCOOR = $4140; // texture coordinates
  C_3DSCHK_MAT     = $AFFF; // material
  C_3DSCHK_MATNAME = $A000; // material name
  C_3DSCHK_MAT01   = $A200; // Texture map 1
  C_3DSCHK_MATFN   = $A300; // map filename
  C_3DSCHK_LIGHT   = $4600; // light
  C_3DSCHK_SPOT    = $4610; // spot light
  C_3DSCHK_LITOFF  = $4620; // light off
  C_3DSCHK_LITATT  = $4625; // Attenuation on
  C_3DSCHK_LITATTS = $4659; // attenuation start
  C_3DSCHK_LITATTE = $465A; // attenuation end
  C_3DSCHK_LITMUL  = $465B; // light multiplier
  C_3DSCHK_AMBIENT = $2100; // ambient light
  C_3DSCHK_RGB_F   = $0010; // Rgb (float)
  C_3DSCHK_RGB_I   = $0011; // Rgb (int)
  C_3DSCHK_RGB_IG  = $0012; // Rgb (int)   - gamma corrected
  C_3DSCHK_RGB_FG  = $0013; // Rgb (float) - gamma corrected

Type
  TA3DS_EnumChunksCB = procedure(_chunk: pointer; var _stop: boolean); stdcall;
  TA3DS_EnumObjectsCB = procedure(_chunk, _subChunks: pointer;
                                  _objName: pChar; var _stop: boolean); stdcall;

  // vector
  PA3DS_Vector = ^TA3DS_Vector;
  TA3DS_Vector = packed record
    x, z, y : single;
  end;

  // face
  PA3DS_Face = ^TA3DS_Face;
  TA3DS_Face = packed record
    c, b, a, flags : word;
  end;

  // texture coordinates
  PA3DS_TextCoor = ^TA3DS_TextCoor;
  TA3DS_TextCoor = packed record
    u, v : single;
  end;

  // Rgb (float)
  PA3DS_RGB_F = ^TA3DS_RGB_F;
  TA3DS_RGB_F = packed record
    r, g, b : single;
  end;

  // Rgb (int)
  PA3DS_RGB_I = ^TA3DS_RGB_I;
  TA3DS_RGB_I = packed record
    r, g, b : byte;
  end;

  // chunk header
  PA3DS_Chunk = ^TA3DS_Chunk;
  TA3DS_Chunk = packed record
    ID    : word;
    Size  : cardinal;
  end;

  // chunk : version
  PA3DS_Chunk_Ver = ^TA3DS_Chunk_Ver;
  TA3DS_Chunk_Ver = packed record
    Chk : TA3DS_Chunk;
    Version : cardinal;
  end;

  // chunk : text (a chunk that ONLY contains a string)
  PA3DS_Chunk_Text = ^TA3DS_Chunk_Text;
  TA3DS_Chunk_Text = packed record
    Chk : TA3DS_Chunk;
    First : Char; // first character of the string
  end;

  // chunk : vtx list (vertex list)
  PA3DS_Chunk_VtxList = ^TA3DS_Chunk_VtxList;
  TA3DS_Chunk_VtxList = packed record
    Chk : TA3DS_Chunk;
    Count : word; // # vertices
    First : TA3DS_Vector; // firts vertex
  end;

  // chunk : texture coordinates list
  PA3DS_Chunk_TextCoorList = ^TA3DS_Chunk_TextCoorList;
  TA3DS_Chunk_TextCoorList = packed record
    Chk : TA3DS_Chunk;
    Count : word; // # texture coordinates
    First : TA3DS_TextCoor; // first texture coordinates
  end;

  // chunk : face list
  PA3DS_Chunk_FaceList = ^TA3DS_Chunk_FaceList;
  TA3DS_Chunk_FaceList = packed record
    Chk : TA3DS_Chunk;
    Count : word; // # faces
    First : TA3DS_Face; // first face
  end;

  // chunk : light
  PA3DS_Chunk_Light = ^TA3DS_Chunk_Light;
  TA3DS_Chunk_Light = packed record
    Chk : TA3DS_Chunk;
    Pos : TA3DS_Vector;
    FirstSubChunk : TA3DS_Chunk; // first sub chunk
  end;

  // chunk : spot light
  PA3DS_Chunk_Spot = ^TA3DS_Chunk_Spot;
  TA3DS_Chunk_Spot = packed record
    Chk : TA3DS_Chunk;
    Target : TA3DS_Vector;
    HotSpot : single;
    FallOff : single;
    FirstSubChunk : TA3DS_Chunk; // first sub chunk
  end;

  // chunk : RGB color (float)
  PA3DS_Chunk_RGB_F = ^TA3DS_Chunk_RGB_F;
  TA3DS_Chunk_RGB_F = packed record
    Chk : TA3DS_Chunk;
    RGB : TA3DS_RGB_F;
  end;

  // chunk : RGB color (int)
  PA3DS_Chunk_RGB_I = ^TA3DS_Chunk_RGB_I;
  TA3DS_Chunk_RGB_I = packed record
    Chk : TA3DS_Chunk;
    RGB : TA3DS_RGB_I;
  end;

  // chunk : one float (chunk with 1 float as data)
  PA3DS_Chunk_OneFloat = ^TA3DS_Chunk_OneFloat;
  TA3DS_Chunk_OneFloat = packed record
    Chk : TA3DS_Chunk;
    f : single;
  end;

  // 3ds file in memory
  TA3DS_FileInMemory = record
    Data : pointer;
    Size : cardinal;
    ok   : boolean;
  end;

function A3DS_RGB_F_To_RGB_I(_rgb_f: PA3DS_RGB_F): cardinal;
procedure A3DS_SkipString(var _p: pointer);

function A3DS_FindChunk(_parent, _start: PA3DS_Chunk;
                        _chkID: word): pointer;
function A3DS_EnumChunks(_parent, _start: PA3DS_Chunk; _chkID: word;
                         _cb: TA3DS_EnumChunksCB): cardinal;
function A3DS_EnumObjects(_a3ds: TA3DS_FileInMemory;
                          _cb: TA3DS_EnumObjectsCB): cardinal;

procedure A3DS_Reset(var _a3ds: TA3DS_FileInMemory);
procedure A3DS_Free(var _a3ds: TA3DS_FileInMemory);
function A3DS_LoadFromFile(var _f: file): TA3DS_FileInMemory;
function A3DS_Load(_fn : string): TA3DS_FileInMemory;

// ---------------------------------------------------------------------------

implementation

// ---------------------------------------------------------------------------

function A3DS_RGB_F_To_RGB_I(_rgb_f: PA3DS_RGB_F): cardinal;

var _r, _g, _b : cardinal;

begin
  _r := round(_rgb_f^.r * 255);
  if (_r > 255) then _r := 255;
  _g := round(_rgb_f^.g * 255);
  if (_g > 255) then _g := 255;
  _b := round(_rgb_f^.b * 255);
  if (_b > 255) then _b := 255;
  Result := _r shl 16 + _g shl 8 + _b;
end;

// ---------------------------------------------------------------------------

procedure A3DS_SkipString(var _p: pointer);

begin
  while (byte(_p^) <> 0) do inc(longint(_p));
  inc(longint(_p));
end;

// ---------------------------------------------------------------------------

function A3DS_FindChunk(_parent, _start: PA3DS_Chunk;
                        _chkID: word): pointer;

// _parent cannot be nil.
// if the parent chunk only contains subchunks use NIL for _start, otherwise
// use a pointer to the first subchunk.

var
  _pos : cardinal;
  _cur : PA3DS_Chunk;
  _siz : cardinal;

begin
  Result := NIL;

  _siz := _parent^.Size;

  if _start = NIL then
  begin
    _pos := SizeOf(TA3DS_Chunk);
    _cur := _parent;
    inc(_cur);
  end else
  begin
    _pos := longint(_start) - longint(_parent);
    _cur := _start;
  end;

  while (_pos < _siz) do
  begin
    if (_cur^.ID <> _chkID) then
    begin
      inc(_pos, _cur^.size);
      inc(longint(pointer(_cur)), _cur^.size);
    end else
    begin
      Result := _cur;
      exit;
    end;
  end;
end;

// ---------------------------------------------------------------------------

function A3DS_EnumChunks(_parent, _start: PA3DS_Chunk; _chkID: word;
                         _cb: TA3DS_EnumChunksCB): cardinal;

// _parent cannot be nil
// _start has the same use that in A3DS_FindChunk
// if _chkID is 0 all chunks will be enumerated
// _cb can be nil

// returns # chunks enumerated

var
  _pos : cardinal;
  _cur : PA3DS_Chunk;
  _siz : cardinal;
  _stop : boolean;

begin
  Result := 0;

  _siz := _parent^.Size;
  _stop := false;

  if _start = NIL then
  begin
    _pos := SizeOf(TA3DS_Chunk);
    _cur := _parent;
    inc(_cur);
  end else
  begin
    _pos := longint(_start) - longint(_parent);
    _cur := _start;
  end;

  while (_pos < _siz) do
  begin
    if (_chkID = 0) or (_cur^.ID = _chkID) then
    begin
      inc(Result);
      if assigned(_cb) then
      begin
        _cb(_cur, _stop);
        if (_stop) then exit;
      end;
    end;
    // next
    inc(_pos, _cur^.size);
    inc(longint(pointer(_cur)), _cur^.size);
  end;
end;

// ---------------------------------------------------------------------------

function A3DS_EnumObjects(_a3ds: TA3DS_FileInMemory;
                          _cb: TA3DS_EnumObjectsCB): cardinal;

// returns # objects enumerated
// _cb can be nil

var
  _pos : cardinal;
  _cur : PA3DS_Chunk;
  _cr2 : ^byte;
  _name : pchar;
  _siz : cardinal;
  _stop : boolean;
  _p : PA3DS_Chunk;

begin
  Result := 0;

  // find 3d editor chunk
  _p := A3DS_FindChunk(_a3ds.Data, NIL, C_3DSCHK_3DEDIT);
  if _p = NIL then exit;

  // enum objects
  _stop := false;
  _pos := SizeOf(TA3DS_Chunk);
  _cur := _p;
  inc(_cur);
  _siz := _p^.Size;

  while (_pos < _siz) do
  begin
    if (_cur^.ID = C_3DSCHK_OBJECT) then
    begin
      // "read" object name and get the position of the sub-chunks (cr2)
      _cr2 := pointer(_cur);
      inc(_cr2, SizeOf(TA3DS_Chunk));
      _name := pointer(_cr2);
      while (_cr2^ <> 0) do
      begin
        inc(_cr2);
        inc(_pos);
      end;
      inc(_cr2);
      inc(_pos);

      inc(Result);
      if assigned(_cb) then
      begin
        _cb(_cur, _cr2, _name, _stop);
        if (_stop) then exit;
      end;
    end;
    // next
    inc(_pos, _cur^.size);
    inc(longint(pointer(_cur)), _cur^.size);
  end;
end;

// ---------------------------------------------------------------------------

procedure A3DS_Reset(var _a3ds: TA3DS_FileInMemory);

begin
  Fillchar(_a3ds, SizeOf(_a3ds), 0);
end;

// ---------------------------------------------------------------------------

procedure A3DS_Free(var _a3ds: TA3DS_FileInMemory);

begin
  try
    if assigned(_a3ds.Data) then FreeMem(_a3ds.Data, _a3ds.Size);
  except
  end;

  A3DS_Reset(_a3ds);
end;

// ---------------------------------------------------------------------------

function A3DS_LoadFromFile(var _f: file): TA3DS_FileInMemory;

var
  _chk : TA3DS_Chunk;

begin
  A3DS_Reset(Result);

  // get and check file size
  Result.Size := filesize(_f);
  if Result.Size < SizeOf(TA3DS_Chunk) then exit;

  try
    // read header
    BlockRead(_f, _chk, SizeOf(_chk));
    // validate file
    if (_chk.ID <> C_3DSCHK_MAIN) or (_chk.Size <> Result.Size) then exit;
    // read file into memory
    Seek(_f, 0);
    GetMem(Result.Data, Result.Size);
    BlockRead(_f, Result.Data^, Result.Size);
    // all right
    Result.ok := true;
  except
    A3DS_Free(Result)
  end;
end;

// ---------------------------------------------------------------------------

function A3DS_Load(_fn : string): TA3DS_FileInMemory;

var
  _f : file;

begin
  A3DS_Reset(Result);

  // open file
  if not filezOpenBin(_fn, obfRead, _f) then exit;

  // load data
  Result := A3DS_LoadFromFile(_f);

  // close file
  CloseFile(_f);
end;

// ---------------------------------------------------------------------------

end.
