(*

  DESC:

   LMTS - Unit for working with Pulsar Studio LMTS files.
   Copyright © 2002 Lord Trancos / Pulsar Studio.
   <lordtrancos@softhome.net>

  API:

   DirectX 8

*)


unit dx8lmts;

// ---------------------------------------------------------------------------

interface

uses Windows, DirectXGraphics, DX8GFX, DX8TEXT, SysUtils, lmts;

const
  C_DXLMTS_FVF = D3DFVF_XYZ or D3DFVF_TEX2;

type
  TDXLMTS_RenderMode = (rm_Multitexture, rm_TwoPass,
                        rm_OnlyLightmaps, rm_NoLightmaps);

  TDXLMTS = record
    header  : TLMTS_Header;
    texData : pointer;
    texPal  : pointer;
    subsets : pointer;
    tris    : IDirect3DVertexBuffer8;
    ok      : boolean;
  end;

procedure dxlmts_Reset(var _dxlmts: TDXLMTS);
procedure dxlmts_Free(var _dxlmts: TDXLMTS; _freeTextures: boolean);
function  dxlmts_Load(_fn: string; _textTag, _lmTag: longint): TDXLMTS;
procedure dxlmts_Render(_dxlmts: TDXLMTS; _mode: TDXLMTS_RenderMode);

// ---------------------------------------------------------------------------

implementation

// ---------------------------------------------------------------------------

procedure dxlmts_Reset(var _dxlmts: TDXLMTS);

begin
  Fillchar(_dxlmts, SizeOf(_dxlmts), 0);
end;

// ---------------------------------------------------------------------------

procedure dxlmts_Free(var _dxlmts: TDXLMTS; _freeTextures: boolean);

var
  _pal : pWord;
  _cnt : longint;

begin
  // free texture data
  if assigned(_dxlmts.texData)
    then FreeMem(_dxlmts.texData,
                 _dxlmts.header.nTexts * SizeOf(TLMTS_TexData));

  // free subset buffer
  if assigned(_dxlmts.subsets)
    then FreeMem(_dxlmts.subsets,
                 _dxlmts.header.nSubsets * _dxlmts.header.subSize);

  // free textures and texture palette
  if assigned(_dxlmts.texPal) then
  begin
    _pal := _dxlmts.texPal;
    if (_freeTextures) then
      for _cnt := 0 to _dxlmts.header.nTexts-1 do
      begin
        TE_FreeText(_pal^);
        inc(_pal);
      end;
    FreeMem(_dxlmts.texPal, _dxlmts.header.nTexts);
  end;

  // free vertex buffer
  if assigned(_dxlmts.tris) then _dxlmts.tris := NIL;

  // clear
  dxlmts_Reset(_dxlmts);
end;

// ---------------------------------------------------------------------------

function dxlmts_Load(_fn: string; _textTag, _lmTag: longint): TDXLMTS;

// _textTag - tag for textures
// _lmTag - tag for lightmaps

var
  _lmts : TLMTS;
  _siz  : longint;
  _p    : pointer;
  _cur  : PLMTS_TexData;
  _cnt  : longint;
  _ti   : longint;
  _pal  : pWord;
  _path : string;
  _tag  : longint;

begin
  dxlmts_Reset(Result);

  // load lmts file
  _lmts := LMTS_Load(_fn);
  if not _lmts.ok then exit;

  // copy header
  Move(_lmts.header, Result.header, SizeOf(TLMTS_Header));

  // create vertex buffer
  _siz := _lmts.header.nTris * 3 * _lmts.header.vtxSize;
  if failed(D3DDEV8.CreateVertexBuffer(_siz, 0, C_DXLMTS_FVF, D3DPOOL_MANAGED,
                                       Result.tris)) then
  begin
    LMTS_Free(_lmts);
    exit;
  end;

  // lock vertex buffer
  if failed(Result.tris.Lock(0, 0, pByte(_p), 0)) then
  begin
    dxlmts_Free(Result, false);
    LMTS_Free(_lmts);
    exit;
  end;

  // fill and unlock
  Move(_lmts.tris^, _p^, _siz);
  Result.tris.Unlock;

  try
    // create buffer for texture data
    _siz := _lmts.header.nTexts * SizeOf(TLMTS_TexData);
    GetMem(Result.texData, _siz);
    Move(_lmts.texData^, Result.texData^, _siz);

    // create and fill buffer for subsets
    _siz := _lmts.header.nSubsets * _lmts.header.subSize;
    GetMem(Result.subsets, _siz);
    Move(_lmts.subsets^, Result.subsets^, _siz);

    // create buffer for texture palette
    _siz := _lmts.header.nTexts * SizeOf(word);
    GetMem(Result.texPal, _siz);
    Fillchar(Result.texPal^, _siz, $ff);
  except
    // free lmts
    dxlmts_Free(Result, false);
    LMTS_Free(_lmts);
    exit;
  end;

  // load textures
  _pal := Result.texPal;
  _cur := _lmts.texData;
  _path := ExtractFilePath(_fn);
  for _cnt := 0 to Result.header.nTexts-1 do
  begin
    // load texture
    if (_cur^.Flags and 1 = 1) then _tag := _lmTag else _tag := _textTag;
    _ti := TE_LoadText(_path + StrPas(_cur^.FName), _tag);
    if (_ti < 0) then
    begin
      dxlmts_Free(Result, true);
      LMTS_Free(_lmts);
      exit;
    end;
    inc(_cur);
    // set texture id on the texture palette
    _pal^ := _ti;
    inc(_pal);
  end;

  // free lmts
  LMTS_Free(_lmts);

  // all right
  Result.ok := true;
end;

// ---------------------------------------------------------------------------

procedure dxlmts_Render(_dxlmts: TDXLMTS; _mode: TDXLMTS_RenderMode);

// _multiTex - use multitexture

var
  _i : longint;
  _tid : word;
  _pal : pWord;
  _sub : PLMTS_Subset;

begin
  _sub := _dxlmts.subsets;

  // set vertex shader and stream source
  D3DDEV8.SetVertexShader(C_DXLMTS_FVF);
  D3DDEV8.SetStreamSource(0, _dxlmts.tris, _dxlmts.header.vtxSize);

  // set some render states
  D3DDEV8.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  D3DDEV8.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

  // render
  case _mode of

    // ******* MULTITEXTURE *******
    rm_Multitexture:
    begin
      // enable multitexture
      D3DDEV8.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
      D3DDEV8.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      D3DDEV8.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

      // set tex. coors.
      D3DDEV8.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      D3DDEV8.SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);

      for _i := 0 to _dxlmts.header.nSubsets - 1 do
      begin
        // set textures
        _pal := _dxlmts.texPal;
        inc(_pal, _sub^.TextID1);
        if (_pal^ <> C_NO_TEXTURE)
          then D3DDEV8.SetTexture(0, G_TE_Textures[_pal^].Text)
            else D3DDEV8.SetTexture(0, NIL);

        if (_sub^.TextID2 <> C_NO_TEXTURE) then
        begin
          // set texture
          _pal := _dxlmts.texPal;
          inc(_pal, _sub^.TextID2);
          if (_pal^ <> C_NO_TEXTURE)
            then D3DDEV8.SetTexture(1, G_TE_Textures[_pal^].Text)
              else D3DDEV8.SetTexture(1, NIL);
        end else D3DDEV8.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE); // disable multitexture

        // Render subset
        D3DDEV8.DrawPrimitive(D3DPT_TRIANGLELIST, _sub^.Offset * 3,
                              _sub^.Count);

        // re-enable multitexture
        if (_sub^.TextID2 = C_NO_TEXTURE)
          then D3DDEV8.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);

        // next subset
        inc(_sub);
      end;

      // disable multitexture
      D3DDEV8.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    end;

    // ******* other modes *******
    else begin
      for _i := 0 to _dxlmts.header.nSubsets - 1 do
      begin
        // 1st pass. Disable alpha blend
        D3DDEV8.SetRenderState(D3DRS_ALPHABLENDENABLE, 0);

        // Set the texture for this subset.
        if (_mode = rm_OnlyLightmaps) then
        begin
          D3DDEV8.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 1);
          _tid := _sub^.TextID2;
        end else
        begin
          D3DDEV8.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
          _tid := _sub^.TextID1;
        end;

        if _tid <> C_NO_TEXTURE then
        begin
          _pal := _dxlmts.texPal;
          inc(_pal, _tid);
          if (_pal^ <> C_NO_TEXTURE)
            then D3DDEV8.SetTexture(0, G_TE_Textures[_pal^].Text)
              else D3DDEV8.SetTexture(0, NIL);
        end else D3DDEV8.SetTexture(0, NIL);

        // Render subset
        D3DDEV8.DrawPrimitive(D3DPT_TRIANGLELIST, _sub^.Offset * 3,
                              _sub^.Count);

        // 2nd pass.
        if (_sub^.TextID2 <> C_NO_TEXTURE) and (_mode = rm_TwoPass) then
        begin
          // Enable alpha blend
          D3DDEV8.SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
          D3DDEV8.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
          D3DDEV8.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);

          // Set the texture for this subset.
          D3DDEV8.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 1);
          _pal := _dxlmts.texPal;
          inc(_pal, _sub^.TextID2);
          if (_pal^ <> C_NO_TEXTURE)
            then D3DDEV8.SetTexture(0, G_TE_Textures[_pal^].Text)
              else D3DDEV8.SetTexture(0, NIL);

          // Render subset
          D3DDEV8.DrawPrimitive(D3DPT_TRIANGLELIST, _sub^.Offset * 3,
                                _sub^.Count);
        end;

        // next subset
        inc(_sub);
      end;
    end;
    // *****************
  end;
end;

// ---------------------------------------------------------------------------

end.
