{
  RGBColor - Color Conversion and Addition Functions
  Copyright © 2001 Lord Trancos.
  <lordtrancos@softhome.net>
}

unit RGBColor;

// --------------------------------------------------------------------

interface

function RGB8toRGB4(const _color: longint): word;
function RGB8toRGB5(const _color: longint): word;
function SumRGB4Colors(const _c1, _c2: word): word;
function SumRGB5Colors(const _c1, _c2: word): word;
function SumRGB8Colors(const _c1, _c2: longint): longint;
function PercentageRGB8Color(const _c1 : longint;
                             const _perc : byte): longint;
//function Perc128RGB8Color(const _c1 : longint;
//                          const _perc : byte): longint;
function FilterRGB8Color(const _c, _filter: cardinal): cardinal;

// --------------------------------------------------------------------

implementation

// --------------------------------------------------------------------

function RGB8toRGB4(const _color: longint): word;

begin
  Result := (((_color shr 20) and $F) shl 8) +
            (((_color shr 12) and $F) shl 4) +
            ((_color shr 4) and $F);
end;

// -------------------------------------------------------------------

function RGB8toRGB5(const _color: longint): word;

begin
  Result := ((_color shr 19) and $1F) shl 10 +
            ((_color shr 11) and $1F) shl 5 +
            ((_color shr 3) and $1F);
end;

// -------------------------------------------------------------------

function SumRGB4Colors(const _c1, _c2: word): word;

var _r, _g, _b: word;

begin
  _r := ((_c1 shr 8) and $F) + ((_c2 shr 8) and $F);
  if _r > $F then _r := $F;
  _g := ((_c1 shr 4) and $F) + ((_c2 shr 4) and $F);
  if _g > $F then _g := $F;
  _b := (_c1 and $F) + (_c2 and $F);
  if _b > $F then _b := $F;
  Result := (_r shl 8) + (_g shl 4) + _b;
end;

// -------------------------------------------------------------------

function SumRGB5Colors(const _c1, _c2: word): word;

var _r, _g, _b: word;

begin
  _r := ((_c1 shr 10) and $1F) + ((_c2 shr 10) and $1F);
  if _r > $1F then _r := $1F;
  _g := ((_c1 shr 5) and $1F) + ((_c2 shr 5) and $1F);
  if _g > $1F then _g := $1F;
  _b := (_c1 and $1F) + (_c2 and $1F);
  if _b > $1F then _b := $1F;
  Result := (_r shl 10) + (_g shl 5) + _b;
end;

// -------------------------------------------------------------------

function SumRGB8Colors(const _c1, _c2: longint): longint;

var _r, _g, _b: longint;

begin
  _r := ((_c1 shr 16) and $FF) + ((_c2 shr 16) and $FF);
  if _r > $FF then _r := $FF;
  _g := ((_c1 shr 8) and $FF) + ((_c2 shr 8) and $FF);
  if _g > $FF then _g := $FF;
  _b := (_c1 and $FF) + (_c2 and $FF);
  if _b > $FF then _b := $FF;
  Result := (_r shl 16) + (_g shl 8) + _b;
end;

// -------------------------------------------------------------------

function PercentageRGB8Color(const _c1 : longint;
                             const _perc : byte): longint;

var _r, _g, _b : longint;

begin
  // Split color.
  _r := (_c1 shr 16) and $FF;
  _g := (_c1 shr 8) and $FF;
  _b := (_c1) and $FF;

  // Calculate new values.
  _r := round(_r * _perc / 100);
  _g := round(_g * _perc / 100);
  _b := round(_b * _perc / 100);

  // Recompose color
  Result := (_r shl 16) + (_g shl 8) + _b;
end;

// -------------------------------------------------------------------
(*
function Perc128RGB8Color(const _c1 : longint;
                          const _perc : byte): longint;

var _r, _g, _b : longint;

begin
  // Split color.
  _r := (_c1 shr 16) and $FF;
  _g := (_c1 shr 8) and $FF;
  _b := (_c1) and $FF;

  // Calculate new values.
  _r := round(_r * _perc / 255);
  _g := round(_g * _perc / 255);
  _b := round(_b * _perc / 255);

  // Recompose color
  Result := (_r shl 16) + (_g shl 8) + _b;
end;
*)
// -------------------------------------------------------------------

function FilterRGB8Color(const _c, _filter: cardinal): cardinal;

var
  _r, _g, _b, _v : byte;

begin
  _r := ((_c shr 16) and $FF);
  _v := ((_filter shr 16) and $FF);
  if _r > _v then _r := _v;

  _g := ((_c shr 8) and $FF);
  _v := ((_filter shr 8) and $FF);
  if _g > _v then _g := _v;

  _b := (_c and $FF);
  _v := (_filter and $FF);
  if _b > _v then _b := _v;

  Result := (_r shl 16) + (_g shl 8) + _b;
end;

// -------------------------------------------------------------------

end.
