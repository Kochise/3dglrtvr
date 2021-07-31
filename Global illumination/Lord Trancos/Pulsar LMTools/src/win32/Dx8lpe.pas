{
  DX8LPE - Large-Pics Engine unit.
  Copyright © 2001-2002 Lord Trancos.
  <lordtrancos@softhome.net>
}

unit DX8LPE;

// -------------------------------------------------------------------

interface

uses Windows, Classes, DirectXGraphics, D3DX8, DX8GFX, DX8GUI3D;

type
  TLPE_Entry = record
                 Text   : array of IDirect3DTexture8;
                 Tag    : longint;
                 Count  : longint; // # uses
                 Width  : longint; // Total img Width.
                 Height : longint; // Total img Height.
                 xCRes  : longint; // Cell X Y
                 yCRes  : longint; // Resolution.
                 xCells : longint; // # X Cells.
                 yCells : longint; // # Y Cells.
               end;

  TLPE_Load = function(_fn: pChar; _tag: longint;
                       var _s: IDirect3DSurface8): boolean; stdcall;

  // DO NOT CREATE MIPMAPS!!
  TLPE_Create = function(const _textWidth, _textHeight: longint;
                         const _format: cardinal;
                         var _t: IDirect3DTexture8): boolean; stdcall;

var
  G_LPE_Initialized : boolean = false;
  G_LPE_MaxSurfNum  : word = 0; // Max # of Surfaces
  G_LPE_LargePics   : array of TLPE_Entry = NIL;

// Main
function  LPE_Init(_maxPics: word;
                   _surfLoad: TLPE_Load;
                   _textCreate: TLPE_Create): boolean;
procedure LPE_Close;
function  LPE_Reload: boolean;

// load - free
function  LPE_LoadPic(_fn: string; _tag: longint): longint;
procedure LPE_FreePic(_index: longint);

// draw
procedure LPE_Draw(_coor: TGUI3DCoor; _index: longint;
                   _colors: PGUI3DColors);

// bonus func
function LPE_GetPicInfo(_filename: pChar;
                        var _nfo: TD3DXImageInfo): boolean;
function LPE_LoadSurfaceFromFile(_filename: pChar;
                                 var _width, _height: longint;
                                 _format: TD3DFormat): IDirect3DSurface8;

// -------------------------------------------------------------------

implementation

var
  G_LPE_Filenames  : TStringList = NIL;
  G_LPE_SurfLoad   : TLPE_Load   = NIL;
  G_LPE_TextCreate : TLPE_Create = NIL;

// -------------------------------------------------------------------

function LPE_GetPicInfo(_filename: pChar;
                        var _nfo: TD3DXImageInfo): boolean;

// Loads a picture, retrieves some information about it, and
// frees the picture.

var _surf : IDirect3DSurface8;
    _r : TRect;

begin
  Result := false;

  // Create a 16 bit dummy surface.
  _surf := NIL;
  if (failed(D3DDEV8.CreateImageSurface(4, 4, D3DFMT_R5G6B5,
             _surf))) or (_surf = NIL) then exit;

  // Load file into dummy surface.
  _r := Rect(0, 0, 3, 3);
  Result := not (failed(D3DXLoadSurfaceFromFileA(_surf, NIL, @_r, _filename,
                                          NIL, D3DX_FILTER_NONE, 0, @_nfo)));

  // Free surface,
  _surf := NIL;
end;

// -------------------------------------------------------------------

function LPE_LoadSurfaceFromFile(_filename: pChar;
                                 var _width, _height: longint;
                                 _format: TD3DFormat): IDirect3DSurface8;

// Loads a surface from a file (bmp, jpeg, png...)
// _format must be 0 to use the default image format.

var _imginfo : TD3DXImageInfo;

begin
  Result := NIL;

  // Get pic nfo
  if not LPE_GetPicInfo(_filename, _imginfo) then exit;
  _width  := _imginfo.Width;
  _height := _imginfo.Height;

  // Create the surface.
  if _format = 0 then _format := _imginfo.Format;
  with _imginfo do
    if (failed(D3DDEV8.CreateImageSurface(Width, Height, _format,
        Result))) or (Result = NIL) then exit;

  // Load file into the surface.
  if (failed(D3DXLoadSurfaceFromFileA(Result, NIL, NIL, _filename,
                          NIL, D3DX_FILTER_NONE, 0, NIL)))
    then Result := NIL;
end;

// -------------------------------------------------------------------

function TLPE_Load_DefaultCB(_fn: pChar; _tag: longint;
                             var _s: IDirect3DSurface8): boolean; stdcall;

var _width, _height: longint;

begin
  _s := LPE_LoadSurfaceFromFile(_fn, _width, _height, D3DFMT_A1R5G5B5);
  Result := assigned(_s);
end;

// -------------------------------------------------------------------

function TLPE_Create_DefaultCB(const _textWidth, _textHeight: longint;
                               const _format: cardinal;
                               var _t: IDirect3DTexture8): boolean; stdcall;

begin
  Result := not failed (D3DXCreateTexture(D3DDEV8,
                        _textWidth, _textHeight, 1, 0,
                        _format, D3DPOOL_MANAGED, _t));
end;

// -------------------------------------------------------------------

procedure LPE_TrueFree(_index: longint);

var _cnt : longint;

begin
  with G_LPE_LargePics[_index] do
  begin
    // Free textures
    if assigned(Text) and (length(Text) > 0) then
      for _cnt := 0 to length(Text) - 1 do
        if assigned(Text[_cnt]) then Text[_cnt] := NIL;
    Text   := NIL;
    Tag    := 0;
    Width  := 0;
    Height := 0;
    xCRes  := 0;
    yCRes  := 0;
    xCells := 0;
    yCells := 0;
  end;
end;

// -------------------------------------------------------------------

function LPE_Init(_maxPics: word;
                  _surfLoad: TLPE_Load;
                  _textCreate: TLPE_Create): boolean;

// _surfLoad - callback used when loading surfaces (*)
// _textCreate - callback used when creating textures (*)
// * NIL to use a default callback

var _i : longint;

begin
  Result := false;

  // Set array length
  G_LPE_MaxSurfNum := _maxPics;
  try
    SetLength(G_LPE_LargePics, G_LPE_MaxSurfNum);
  except
    exit;
  end;

  try
    // Clear array
    for _i := 0 to G_LPE_MaxSurfNum - 1
      do FillChar(G_LPE_LargePics[_i], SizeOf(TLPE_Entry), 0);
    // Create filename list.
    G_LPE_Filenames := TStringList.Create;
  except
    G_LPE_LargePics := NIL;
    exit;
  end;

  // set callbacks
  if assigned(_surfLoad)
    then G_LPE_SurfLoad := _surfLoad
      else G_LPE_SurfLoad := TLPE_Load_DefaultCB;
  if assigned(_textCreate)
    then G_LPE_TextCreate := _textCreate
      else G_LPE_TextCreate := TLPE_Create_DefaultCB;

  // All right
  G_LPE_Initialized := true;
  Result := G_LPE_Initialized;
end;

// -------------------------------------------------------------------

procedure LPE_Close;

var _cnt : longint;

begin
  // Free pics
  if assigned(G_LPE_LargePics) then
    begin
      for _cnt := 0 to G_LPE_MaxSurfNum - 1
        do LPE_TrueFree(_cnt);
      G_LPE_LargePics := NIL;
    end;

  // Free texture list.
  if assigned(G_LPE_Filenames) then G_LPE_Filenames.Free;

  // Engine is disabled.
  G_LPE_Initialized := false;
end;

// -------------------------------------------------------------------

procedure LPE_SurfToTextBitBlt(_surf: IDirect3DSurface8;
                               _surfWidth,
                               _srcOffsetX, _srcOffsetY,
                               _dstOffsetX, _dstOffsetY,
                               _dstWidth, _dstHeight : longint;
                               _text: IDirect3DTexture8);

// Desc : Copies data from surfate to texture.
//        Surface and texture must have the same format.
//        Warning : this function does not perform cliping.
//                  (Be careful using this function...)

var _sr, _tr : TD3DLocked_Rect;
    _pixelSize : longint; // Pixel size in bytes.
    _scanlSize : longint; // Destination scan line size in bytes.
    _srcCur : pByte;
    _dstCur : pByte;
    _cntY : longint;

begin
  // Copy data
  if (not failed(_surf.LockRect(_sr, NIL, 0))) and
     (not failed(_text.LockRect(0, _tr, NIL, 0))) then
    begin
      _pixelSize := _sr.Pitch div _surfWidth;
      _scanlSize := _pixelSize * _dstWidth;

      // Set src cursor.
      _srcCur := _sr.pBits;
      inc(_srcCur, (_sr.Pitch * _srcOffsetY) + (_pixelSize * _srcOffsetX));

      // Set dst cursor.
      _dstCur := _tr.pBits;
      inc(_dstCur, (_tr.Pitch * _dstOffsetY) + (_pixelSize * _dstOffsetX));

      // Copy Data.
      for _cntY := 0 to _dstHeight - 1 do
        begin
          Move(_srcCur^, _dstCur^, _scanlSize);
          // Next line.
          inc(_srcCur, _sr.Pitch);
          inc(_dstCur, _tr.Pitch);
        end;
    end;

  // Unlock.
  _text.UnlockRect(0);
  _surf.UnlockRect;
end;

// -------------------------------------------------------------------

function LPE_TrueLoad(_index: longint; _fn: pChar;
                      _tag: longint): boolean;

var _surf : IDirect3DSurface8;
    _desc : TD3DSurface_Desc;
    _cnt : longint;
    _cntX, _cntY : longint;
    _dw, _dh : longint;

begin
  Result := false;

  // Load surface
  _surf := NIL;
  if not G_LPE_SurfLoad(_fn, _tag, _surf) then exit;

  // Get surface desc
  if not failed(_surf.GetDesc(_desc)) then
  with G_LPE_LargePics[_index] do
  begin
    // Get resolution.
    Width  := _desc.Width;
    Height := _desc.Height;

    // Check how many textures are requiered for current device.
    xCRes  := D3DDEVCAPS8.MaxTextureWidth;
    xCells := 0;
    _cnt := Width;
    repeat
      inc(xCells);
      dec(_cnt, xCRes);
    until (_cnt <= 0);

    yCRes  := D3DDEVCAPS8.MaxTextureHeight;
    yCells := 0;
    _cnt := Height;
    repeat
      inc(yCells);
      dec(_cnt, yCRes);
    until (_cnt <= 0);

    // Create those textures
    try
      SetLength(Text, xCells * yCells);
      for _cnt := 0 to Length(Text)-1 do Text[_cnt] := NIL;
    except
      _surf := NIL;
      exit;
    end;

    for _cnt := 0 to Length(Text)-1 do
      if not G_LPE_TextCreate(D3DDEVCAPS8.MaxTextureWidth,
                              D3DDEVCAPS8.MaxTextureHeight,
                              _desc.Format, Text[_cnt]) then
      begin
        for _cntX := 0 to Length(Text)-1 do Text[_cntX] := NIL;
        Text := NIL;
        _surf := NIL;
        exit;
      end;

    // Draw textures
    _cnt := 0;
    for _cntY := 0 to yCells - 1 do
      for _cntX := 0 to xCells - 1 do
      begin
        if ((_cntX+1) * xCRes) > Width
          then _dw := Width - (_cntX * xCRes)
            else _dw := xCRes;

        if ((_cntY+1) * yCRes) > Height
          then _dh := Height - (_cntY * yCRes)
            else _dh := yCRes;

        LPE_SurfToTextBitBlt(_surf, Width, _cntX * xCRes, _cntY * yCRes,
                             0, 0, _dw, _dh, Text[_cnt]);

        // next cell
        inc(_cnt);
      end;

    // All right
    Result := true;
  end;

  // Free surface
  _surf := NIL;
end;

// -------------------------------------------------------------------

function LPE_LoadPic(_fn: string; _tag: longint): longint;

// returns < 0 if any error, or the pic index if all is right.

var _index : longint;

begin
  Result := -1;

  // Engine initialized ?
  if not G_LPE_Initialized then exit;

  // Check if the pic it's already loaded.
  _index := G_LPE_Filenames.IndexOf(_fn);

  // Already loaded
  if (_index > -1) then
  begin
    inc(G_LPE_LargePics[_index].Count);
    Result := _index;
    exit;
  end;

  // Find free slot
  _index := G_LPE_Filenames.IndexOf('');
  if (_index = -1) then
  begin
    // Can create new slot ?
    if (G_LPE_Filenames.Count >= G_LPE_MaxSurfNum) then exit;
    // Create new slot
    try G_LPE_Filenames.Append(''); except exit; end;
    _index := G_LPE_Filenames.Count-1;
  end;

  // It's really free ?
  if (G_LPE_LargePics[_index].Text <> NIL) then exit;

  // Load
  if LPE_TrueLoad(_index, pChar(_fn), _tag) then
  with G_LPE_LargePics[_index] do
  begin
    Tag := _tag;
    Count := 1;
    G_LPE_Filenames.Strings[_index] := _fn;
    Result := _index;
  end;
end;

// -------------------------------------------------------------------

procedure LPE_Draw(_coor: TGUI3DCoor; _index: longint;
                   _colors: PGUI3DColors);

// Remember to call GUI3D_SetVertexShader.
//
// Also if you use texture filtering you should set:
//   D3DDEV8.SetTextureStageState(0, D3DTSS_ADDRESSU,  D3DTADDRESS_CLAMP);
//   D3DDEV8.SetTextureStageState(0, D3DTSS_ADDRESSV,  D3DTADDRESS_CLAMP);
// to avoid artifacts.
//

var _cntX, _cntY : longint;
    _trans : TD3DXVector2;
    _cnt : longint;
    _x2, _y2 : single;
    _sX, _sY : single;

begin
//  if (not G_LPE_Initialized) or (_index < 0) or (_index >= G_LPE_MaxSurfNum) or
//     (G_LPE_LargePics[_index].Count = 0) then exit;

  _sX := (_coor.w / G_LPE_LargePics[_index].Width);
  _sY := (_coor.h / G_LPE_LargePics[_index].Height);

  _cnt := 0;
  with G_LPE_LargePics[_index] do
  begin
    if (length(Text) > 0) then
      for _cntY := 0 to yCells-1 do
        for _cntX := 0 to xCells-1 do
        begin
          _trans.x := _coor.x + (_cntX * xCRes * _sX);
          _trans.y := _coor.y + (_cntY * yCRes * _sY);

          // Draw only if it's on screen!! (clipping)
          _x2 := _trans.x + (xCRes * _sX);
          _y2 := _trans.x + (yCRes * _sY);
          if (_x2 >= 0) and (_y2 >= 0) and
             (_trans.x < D3DDEVPPAR8.BackBufferWidth) and
             (_trans.y < D3DDEVPPAR8.BackBufferHeight) then
          begin
            D3DDEV8.SetTexture(0, Text[_cnt]);
            GUI3D_DrawRect(GUI3D_Coor(_trans.x, _trans.y, xCRes * _sX, yCRes * _sY), NIL);
          end;
          // next cell
          inc(_cnt);
        end;
  end;
end;

// -------------------------------------------------------------------

procedure LPE_FreePic(_index: longint);

begin
  // All is right ?
  if (not G_LPE_Initialized) or (_index < 0) or (_index >= G_LPE_MaxSurfNum) or
     (G_LPE_LargePics[_index].Count = 0) then exit;

  // ---
  dec(G_LPE_LargePics[_index].Count);
  if (G_LPE_LargePics[_index].Count = 0) then
  begin
    G_LPE_Filenames.Strings[_index] := '';  // Remove filename from list

    // Free textures
    LPE_TrueFree(_index);
  end;

  // Remove free slots at the end of the list
  while (G_LPE_Filenames.Count > 0) and
        (G_LPE_Filenames.Strings[G_LPE_Filenames.Count-1] = '')
  do G_LPE_Filenames.Delete(G_LPE_Filenames.Count-1);
end;

// -------------------------------------------------------------------

function LPE_Reload: boolean;

// Call this function when devices are lost to reload pics.
// Returns false if any pic was not reloaded.

var _cnt : longint;

begin
  Result := false;
  if (not G_LPE_Initialized) then exit;

  Result := true;
  for _cnt := 0 to G_LPE_MaxSurfNum - 1 do
  begin
    if (G_LPE_LargePics[_cnt].Count > 0) then
    begin
      if not LPE_TrueLoad(_cnt, pChar(G_LPE_Filenames[_cnt]),
                          G_LPE_LargePics[_cnt].Tag) then
      begin
        G_LPE_LargePics[_cnt].Count := 0;
        Result := false;
      end;
    end;
  end;
end;

// -------------------------------------------------------------------

end.
