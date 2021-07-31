(*

  DESC:

   DX8TEXT - Texture-Engine unit.
   Copyright © 2001-2002 Lord Trancos.
   <lordtrancos@softhome.net>

  API:

   DirectX 8.0

*)

unit DX8TEXT;

// -------------------------------------------------------------------

interface

uses Windows, Classes, DirectXGraphics, DX8GFX, D3DX8;

type
  TTE_Entry = record
                Text  : IDirect3DTexture8;
                Tag   : longint;
                Count : longint; // # uses
              end;

  TTE_Load = function(_fn: pChar; _tag: longint;
                      var _t: IDirect3DTexture8): boolean; stdcall;

var
  G_TE_Initialized : boolean = false;
  G_TE_MaxTextNum  : word = 0; // Max # of Textures
  G_TE_Textures    : array of TTE_Entry = NIL;

const
  C_NO_TEXTURE = $FFFF;

// Main functions.
function  TE_Init(_maxText: word; _texLoad: TTE_Load): boolean;
procedure TE_Close;
function  TE_Reload: boolean;
procedure TE_FreeAllTextures;

// Texture load / free
function  TE_LoadText(_fn: string; _tag: longint): longint;
procedure TE_FreeText(_index: longint);

// Bonus
function  TE_GetFilename(_index: longint): string;
function  TE_Count: longint;

// -------------------------------------------------------------------

implementation

var
  G_TE_Filenames : TStringList = NIL;
  G_TE_TextLoad  : TTE_Load = NIL;

// -------------------------------------------------------------------

function TE_Load_DefaultCB(_fn: pChar; _tag: longint;
                           var _t: IDirect3DTexture8): boolean; stdcall;


begin
  Result := not failed(D3DXCreateTextureFromFileEx(D3DDEV8, _fn, 0, 0, 0, 0,
                       D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE,
                       D3DX_FILTER_TRIANGLE, 0, NIL, NIL, _t)); // (D3DDEV8, _fn, _t));
end;

// -------------------------------------------------------------------

function  TE_Init(_maxText: word; _texLoad: TTE_Load): boolean;

// _texLoad - callback used when loading textures (*)
// (*) NIL to use a default callback

var _i : longint;

begin
  Result := false;

  // Set array length
  G_TE_MaxTextNum := _maxText;
  try
    SetLength(G_TE_Textures, G_TE_MaxTextNum);
  except
    exit;
  end;

  try
    // Clear texture array
    for _i := 0 to G_TE_MaxTextNum - 1
      do FillChar(G_TE_Textures[_i], SizeOf(TTE_Entry), 0);
    // Create texture list.
    G_TE_Filenames := TStringList.Create;
  except
    G_TE_Textures := NIL;
    exit;
  end;

  // set text loading callback
  if assigned(_texLoad)
    then G_TE_TextLoad := _texLoad
      else G_TE_TextLoad := TE_Load_DefaultCB;

  // All right
  G_TE_Initialized := true;
  Result := G_TE_Initialized;
end;

// -------------------------------------------------------------------

procedure TE_FreeAllTextures;

var _i : longint;

begin
  // free all textures
  if assigned(G_TE_Textures) then
  begin
    for _i := 0 to G_TE_Filenames.Count - 1 do
    begin
      G_TE_Textures[_i].Count := 0;
      G_TE_Textures[_i].Tag   := 0;
      if assigned(G_TE_Textures[_i].Text)
        then G_TE_Textures[_i].Text := NIL;
      G_TE_Filenames.Strings[_i] := '';
    end;
    G_TE_Filenames.Clear;
  end;
end;

// -------------------------------------------------------------------

procedure TE_Close;

begin
  // Free textures.
  TE_FreeAllTextures;
  G_TE_Textures := NIL;  
  // Free texture list.
  if assigned(G_TE_Filenames) then G_TE_Filenames.Free;
  // Engine is disabled.
  G_TE_Initialized := false;
end;

// -------------------------------------------------------------------

function TE_LoadText(_fn: string; _tag: longint): longint;

// returns < 0 if any error, or the texture index if all is right.

var _index : longint;

begin
  Result := -1;

  // Texture Engine initialized ?
  if not G_TE_Initialized then exit;

  // Check if the texture it's already loaded.
  _index := G_TE_Filenames.IndexOf(_fn);

  // Already loaded
  if (_index > -1) then
  begin
    inc(G_TE_Textures[_index].Count);
    Result := _index;
    exit;
  end;

  // Find free slot
  _index := G_TE_Filenames.IndexOf('');
  if (_index = -1) then
  begin
    // Can create new slot ?
    if (G_TE_Filenames.Count >= G_TE_MaxTextNum) then exit;
    // Create new slot
    try G_TE_Filenames.Append(''); except exit; end;
    _index := G_TE_Filenames.Count-1;
  end;

  // It's really free ?
  if (G_TE_Textures[_index].Text <> NIL) then exit;

  // Load
  if not G_TE_TextLoad(pChar(_fn), _tag, G_TE_Textures[_index].Text) then exit;

  // All right
  if (G_TE_Textures[_index].Text <> NIL) then
  begin
    G_TE_Textures[_index].Tag := _tag;
    G_TE_Textures[_index].Count := 1;
    G_TE_Filenames.Strings[_index] := _fn;
  end else _index := C_NO_TEXTURE;
  Result := _index;
end;

// -------------------------------------------------------------------

procedure TE_FreeText(_index: longint);

begin
  // All is right ?
  if (not G_TE_Initialized) or (_index < 0) or (_index >= G_TE_MaxTextNum) or
     (G_TE_Textures[_index].Count = 0) then exit;

  // ---
  dec(G_TE_Textures[_index].Count);
  if (G_TE_Textures[_index].Count = 0) then
  begin
    G_TE_Filenames.Strings[_index] := ''; // Remove filename from list
    G_TE_Textures[_index].Text := NIL;    // Free texture
    G_TE_Textures[_index].Tag := 0;
  end;

  // Remove free slots at the end of the list
  while (G_TE_Filenames.Count > 0) and
        (G_TE_Filenames.Strings[G_TE_Filenames.Count-1] = '')
  do G_TE_Filenames.Delete(G_TE_Filenames.Count-1);
end;

// -------------------------------------------------------------------

function TE_Reload: boolean;

// Call this function when devices are lost to reload textures.
// Returns false if any texture was not reloaded.

var _cnt : longint;

begin
  Result := false;
  if (not G_TE_Initialized) then exit;

  Result := true;
  for _cnt := 0 to G_TE_MaxTextNum - 1 do
  begin
    if (G_TE_Textures[_cnt].Count > 0) then
    begin
      if not G_TE_TextLoad(pChar(G_TE_Filenames[_cnt]),
                           G_TE_Textures[_cnt].Tag,
                           G_TE_Textures[_cnt].Text) then
      begin
        G_TE_Textures[_cnt].Count := 0;
        Result := false;
      end;
    end;
  end;
end;

// -------------------------------------------------------------------

function  TE_GetFilename(_index: longint): string;

begin
  Result := '';

  if (not G_TE_Initialized) or
     (_index < 0) or (_index >= G_TE_MaxTextNum) then exit;

  Result := G_TE_Filenames[_index];
end;

// -------------------------------------------------------------------

function  TE_Count: longint;

var _i : longint;

begin
  Result := -1;
  if (not G_TE_Initialized) then exit;

  Result := 0;
  for _i := 0 to G_TE_MaxTextNum - 1 do
  if assigned(G_TE_Textures[_i].Text) then inc(Result);
end;

// -------------------------------------------------------------------

end.
