{
  DX8GUI3D - DirectX 8 3D GUI unit.
  Copyright © 2001-2002 Lord Trancos.
  <lordtrancos@softhome.net>
}

unit DX8GUI3D;

// --------------------------------------------------------------------

interface

uses Windows, DirectXGraphics, DX8GFX;

const
  GUI3D_D3DFVF = D3DFVF_XYZRHW or D3DFVF_DIFFUSE or D3DFVF_TEX1;

type
  TGUI3DCoor = record
                 x, y, w, h : single; // x, y, width and height
                 x2, y2 : single;
               end;

  PGUI3DColors = ^TGUI3DColors;
  TGUI3DColors = array[0..3] of dWord;
  TGUI3DUV     = record
                   U, V: single;
                 end;
  PGUI3DUVs    = ^TGUI3DUVs;
  TGUI3DUVs    = array[0..3] of TGUI3DUV;
  TUVMapMode   = (uvNormal);

  PGUI3DCellSys = ^TGUI3DCellSys;
  TGUI3DCellSys = record
                    Width  : word; // texture width
                    Height : word; // texture height
                    CWidth   : word; // source cell width
                    CHeight  : word; // source cell height
                    UInc, VInc  : single;
                    CPerLine : word; // # cells per line
                  end;

// Coor
function  GUI3D_Coor(const _x, _y, _w, _h: single): TGUI3DCoor;
function  GUI3D_CoorEx(const _x, _y, _w, _h: single): TGUI3DCoor;
function  GUI3D_CoorExB(const _x, _y, _w, _h: single;
                        const _srcW, _srcH: single): TGUI3DCoor;

// Color
function  GUI3D_OneColor(const _argb: dWord): TGUI3DColors;
function  GUI3D_FourColors(const _c1, _c2, _c3, _c4: dWord): TGUI3DColors;
procedure GUI3D_SetColors(const _col: TGUI3DColors);

// UVs
function  GUI3D_GetUVs(const _u1, _v1, _u2, _v2,
                             _u3, _v3, _u4, _v4: single): TGUI3DUVs;

// Main
procedure GUI3D_SetVertexShader;
function  GUI3D_DrawRect(const _coor: TGUI3DCoor;
                         const _textCoor: PGUI3DUVs): boolean;

// Cell systems
function GUI3D_CellSys(const _w, _h, _cW, _cH: word): TGUI3DCellSys;
function GUI3D_DrawCell(const _coor: TGUI3DCoor;
                        const _cellIndex: word;
                        const _cellSys: PGUI3DCellSys;
                        const _mirror: boolean): boolean;

// Type functions
function GUI3D_Write(const _coor: TGUI3DCoor;
                     const _s: string;
                     const _fntInfo: TGUI3DCellSys): boolean;

// --------------------------------------------------------------------

implementation

type
  TGUI3DVertex = record
                   X, Y, Z, RHW : single;
                   DColor : dWord;
                   U, V : single;
                 end;

var
  GUI3D_RectVA : array[0..3] of TGUI3DVertex;

// --------------------------------------------------------------------

procedure GUI3D_Init;

var _cnt : longint;

begin
  // Set default values for Z and RHW
  for _cnt := 0 to 3 do
  begin
    GUI3D_RectVA[_cnt].Z := 0.5;
    GUI3D_RectVA[_cnt].RHW := 1;
  end;
end;

// --------------------------------------------------------------------

function  GUI3D_Coor(const _x, _y, _w, _h: single): TGUI3DCoor;

begin
  with Result do
  begin
    x := _x;
    y := _y;
    w := _w;
    h := _h;
    x2 := x + w{ - 1};
    y2 := y + h{ - 1};
  end;
end;

// --------------------------------------------------------------------

function  GUI3D_CoorEx(const _x, _y, _w, _h: single): TGUI3DCoor;

// works with %

begin
  with Result do
  begin
    x := (_x * D3DDEVPPAR8.BackBufferWidth)  / 100;
    y := (_y * D3DDEVPPAR8.BackBufferHeight) / 100;
    w := (_w * D3DDEVPPAR8.BackBufferWidth)  / 100;
    h := (_h * D3DDEVPPAR8.BackBufferHeight) / 100;
    x2 := x + w{ - 1};
    y2 := y + h{ - 1};
  end;
end;

// --------------------------------------------------------------------

function  GUI3D_CoorExB(const _x, _y, _w, _h: single;
                        const _srcW, _srcH: single): TGUI3DCoor;

// works with %

begin
  with Result do
  begin
    x := (_x * D3DDEVPPAR8.BackBufferWidth)  / 100;
    y := (_y * D3DDEVPPAR8.BackBufferHeight) / 100;
    if _w <> 0 then w := (_w * D3DDEVPPAR8.BackBufferWidth)  / 100;
    if _h <> 0 then h := (_h * D3DDEVPPAR8.BackBufferHeight) / 100;
    if _w = 0 then w := ((h / _srcH) * _srcW);
    if _h = 0 then h := ((w / _srcW) * _srcH);
    x2 := x + w{ - 1};
    y2 := y + h{ - 1};
  end;
end;

// --------------------------------------------------------------------

procedure GUI3D_SetColors(const _col: TGUI3DColors);

var _cnt : longint;

begin
  for _cnt := 0 to 3 do GUI3D_RectVA[_cnt].dColor := _col[_cnt];
end;

// --------------------------------------------------------------------

function GUI3D_OneColor(const _argb: dWord): TGUI3DColors;

var _cnt : longint;

begin
  for _cnt := 0 to 3 do Result[_cnt] := _argb;
end;

// --------------------------------------------------------------------

function GUI3D_FourColors(const _c1, _c2, _c3, _c4: dWord): TGUI3DColors;

begin
  Result[0] := _c3;
  Result[1] := _c1;
  Result[2] := _c4;
  Result[3] := _c2;
end;

// --------------------------------------------------------------------

function GUI3D_GetUVs(const _u1, _v1, _u2, _v2,
                            _u3, _v3, _u4, _v4: single): TGUI3DUVs;

begin
  Result[0].U := _u3;
  Result[0].V := _v3;
  Result[1].U := _u1;
  Result[1].V := _v1;
  Result[2].U := _u4;
  Result[2].V := _v4;
  Result[3].U := _u2;
  Result[3].V := _v2;
end;

// --------------------------------------------------------------------

procedure GUI3D_SetVertexShader;

begin
  // Set the vertex shader.
  D3DDEV8.SetVertexShader(GUI3D_D3DFVF);
end;

// --------------------------------------------------------------------

function  GUI3D_DrawRect(const _coor: TGUI3DCoor;
                         const _textCoor: PGUI3DUVs): boolean;

var _cnt : longint;

begin
  // Set rect
  GUI3D_RectVA[0].X := _coor.x;
  GUI3D_RectVA[0].Y := _coor.y2;
  GUI3D_RectVA[1].X := _coor.x;
  GUI3D_RectVA[1].Y := _coor.y;
  GUI3D_RectVA[2].X := _coor.x2;
  GUI3D_RectVA[2].Y := _coor.y2;
  GUI3D_RectVA[3].X := _coor.x2;
  GUI3D_RectVA[3].Y := _coor.y;

  // Set UVs
  if (_textCoor = NIL) then
  begin
    GUI3D_RectVA[0].U := 0;  GUI3D_RectVA[0].V := 1;
    GUI3D_RectVA[1].U := 0;  GUI3D_RectVA[1].V := 0;
    GUI3D_RectVA[2].U := 1;  GUI3D_RectVA[2].V := 1;
    GUI3D_RectVA[3].U := 1;  GUI3D_RectVA[3].V := 0;
  end else for _cnt := 0 to 3 do
           begin
             GUI3D_RectVA[_cnt].U := _textCoor^[_cnt].U;
             GUI3D_RectVA[_cnt].V := _textCoor^[_cnt].V;
           end;

  // Draw
  Result := not failed(D3DDEV8.DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2,
                                 @GUI3D_RectVA, SizeOf(TGUI3DVertex)));
end;

// --------------------------------------------------------------------

function GUI3D_CellSys(const _w, _h, _cW, _cH: word): TGUI3DCellSys;

begin
  with Result do
  begin
    Width  := _w;
    Height := _h;
    CWidth  := _cW;
    CHeight := _cH;
    UInc := (_cW / _w);
    VInc := (_cH / _h);
    CPerLine := _w div _cW;
  end;
end;

// --------------------------------------------------------------------

function GUI3D_DrawCell(const _coor: TGUI3DCoor;
                        const _cellIndex: word;
                        const _cellSys: PGUI3DCellSys;
                        const _mirror: boolean): boolean;

var _u1, _v1, _u2, _v2 : single;

begin
  // Build rect
  GUI3D_RectVA[0].X := _coor.x;
  GUI3D_RectVA[1].X := _coor.x;
  GUI3D_RectVA[2].X := _coor.x2;
  GUI3D_RectVA[3].X := _coor.x2;
  GUI3D_RectVA[0].Y := _coor.y2;
  GUI3D_RectVA[1].Y := _coor.y;
  GUI3D_RectVA[2].Y := _coor.y2;
  GUI3D_RectVA[3].Y := _coor.y;

  // Set UVs
  _u1 := (_cellIndex mod _cellSys.CPerLine) * _cellSys.UInc;
  _v1 := (_cellIndex div _cellSys.CPerLine) * _cellSys.VInc;
  _u2 := _u1 + _cellSys.UInc;
  _v2 := _v1 + _cellSys.VInc;

  GUI3D_RectVA[0].V := _v2;
  GUI3D_RectVA[1].V := _v1;
  GUI3D_RectVA[2].V := _v2;
  GUI3D_RectVA[3].V := _v1;
  if not _mirror then
  begin
    GUI3D_RectVA[0].U := _u1;
    GUI3D_RectVA[1].U := _u1;
    GUI3D_RectVA[2].U := _u2;
    GUI3D_RectVA[3].U := _u2;
  end else
  begin
    GUI3D_RectVA[0].U := _u2;
    GUI3D_RectVA[1].U := _u2;
    GUI3D_RectVA[2].U := _u1;
    GUI3D_RectVA[3].U := _u1;
  end;

  // Draw
  Result := not failed(D3DDEV8.DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2,
                                 @GUI3D_RectVA, SizeOf(TGUI3DVertex)));
end;

// --------------------------------------------------------------------

function GUI3D_Write(const _coor: TGUI3DCoor;
                     const _s: string;
                     const _fntInfo: TGUI3DCellSys): boolean;

// (only ascii from 32 are supported)
// coor.w -> character width
// coor.h -> character height

var _x, _y, _x2 : single;
    _chrCoor : TGUI3DCoor;
    _cnt, _len : longint;
    _chrIdx : longint;

begin
  Result := false;

  _x := _coor.x;
  _y := _coor.y;
  _x2 := _coor.x;

  // Write text
  _len := length(_s);
  if (_len > 0) then
    for _cnt := 1 to _len do
    begin
      _chrIdx := byte(_s[_cnt]);

      // line break
      if (_chrIdx = 10) then
      begin
        _x := _x2;
        _y := _y + _coor.h;
        Continue;
      end;

      // is a valid character ?
      if (_chrIdx < 32) then _chrIdx := 0 else _chrIdx := _chrIdx - 32;

      // draw char
      _chrCoor := GUI3D_Coor(_x, _y, _coor.w, _coor.h);
      if not GUI3D_DrawCell(_chrCoor, _chrIdx, @_fntInfo, false) then exit;

      // next char
      _x := _x + _coor.w;
    end;

  // All right
  Result := true;
end;

// --------------------------------------------------------------------

begin
  GUI3D_Init;
end.
