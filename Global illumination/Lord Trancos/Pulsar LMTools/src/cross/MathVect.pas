(*

  DESC:

   Vector Maths unit
   by Lord Trancos.
     (http://www.dxlab.tk - http://www.geocities.com/dxlab)
     (lordtrancos@softhome.net)

  API:

   NONE

*)

unit MathVect;

// --------------------------------------------------------------------

interface

uses vTypes;

type TXYZ = (vectX, vectY, vectZ);

const HALF_PI = PI/2;
      PI_X2   = PI*2;

function  vect(const _x, _y, _z: single): T_Vec;
function  vectSame(const _v1, _v2: P_Vec): boolean;
function  vectAddV(const _v1, _v2: P_Vec): T_Vec;
function  vectSubV(const _v1, _v2: P_Vec): T_Vec;
function  vectMulV(const _v1, _v2: P_Vec): T_Vec;
function  vectDivV(const _v1, _v2: P_Vec): T_Vec;
function  vectAddS(const _v: P_Vec; const _s: single): T_Vec;
function  vectSubS(const _v: P_Vec; const _s: single): T_Vec;
function  vectMulS(const _v: P_Vec; const _s: single): T_Vec;
function  vectDivS(const _v: P_Vec; const _s: single): T_Vec;
function  vectInv(const _v: P_Vec): T_Vec;
function  vectDot(const _v1, _v2: P_Vec): single;
function  vectLen(const _v: P_Vec): single;
function  vectNormalize(const _v: P_Vec): T_Vec;
procedure vectSetLen(var _v: T_Vec; const _len: single);
function  vectCross(const _v1, _v2: P_Vec): T_Vec; // = wedge
function  vectLargestComponent(const _v: P_Vec): TXYZ;

// --------------------------------------------------------------------

implementation

// --------------------------------------------------------------------

function vect(const _x, _y, _z: single): T_Vec;

begin
  Result[X] := _x;
  Result[Y] := _y;
  Result[Z] := _z;
end;

// --------------------------------------------------------------------

function vectSame(const _v1, _v2: P_Vec): boolean;

begin
  Result := (_v1^[X] = _v2^[X]) and
            (_v1^[Y] = _v2^[Y]) and
            (_v1^[Z] = _v2^[Z]);
end;

// --------------------------------------------------------------------

function vectAddV(const _v1, _v2: P_Vec): T_Vec;

begin
  Result[X] := _v1^[X] + _v2^[X];
  Result[Y] := _v1^[Y] + _v2^[Y];
  Result[Z] := _v1^[Z] + _v2^[Z];
end;

// --------------------------------------------------------------------

function vectSubV(const _v1, _v2: P_Vec): T_Vec;

begin
  Result[X] := _v1^[X] - _v2^[X];
  Result[Y] := _v1^[Y] - _v2^[Y];
  Result[Z] := _v1^[Z] - _v2^[Z];
end;

// --------------------------------------------------------------------

function vectMulV(const _v1, _v2: P_Vec): T_Vec;

begin
  Result[X] := _v1^[X] * _v2^[X];
  Result[Y] := _v1^[Y] * _v2^[Y];
  Result[Z] := _v1^[Z] * _v2^[Z];
end;

// --------------------------------------------------------------------

function vectDivV(const _v1, _v2: P_Vec): T_Vec;

begin
  Result[X] := _v1^[X] / _v2^[X];
  Result[Y] := _v1^[Y] / _v2^[Y];
  Result[Z] := _v1^[Z] / _v2^[Z];
end;

// --------------------------------------------------------------------

function vectAddS(const _v: P_Vec; const _s: single): T_Vec;

begin
  Result[X] := _v^[X] + _s;
  Result[Y] := _v^[Y] + _s;
  Result[Z] := _v^[Z] + _s;
end;

// --------------------------------------------------------------------

function vectSubS(const _v: P_Vec; const _s: single): T_Vec;

begin
  Result[X] := _v^[X] - _s;
  Result[Y] := _v^[Y] - _s;
  Result[Z] := _v^[Z] - _s;
end;

// --------------------------------------------------------------------

function vectMulS(const _v: P_Vec; const _s: single): T_Vec;

begin
  Result[X] := _v^[X] * _s;
  Result[Y] := _v^[Y] * _s;
  Result[Z] := _v^[Z] * _s;
end;

// --------------------------------------------------------------------

function vectDivS(const _v: P_Vec; const _s: single): T_Vec;

begin
  Result[X] := _v^[X] / _s;
  Result[Y] := _v^[Y] / _s;
  Result[Z] := _v^[Z] / _s;
end;

// --------------------------------------------------------------------

function vectInv(const _v: P_Vec): T_Vec;

begin
  Result[X] := - _v^[X];
  Result[Y] := - _v^[Y];
  Result[Z] := - _v^[Z];
end;

// --------------------------------------------------------------------

function vectDot(const _v1, _v2: P_Vec): single;

begin
  Result := _v1^[X] * _v2^[X] + _v1^[Y] * _v2^[Y] + _v1^[Z] * _v2^[Z];
end;

// --------------------------------------------------------------------

function vectLen(const _v: P_Vec): single;

begin
  Result := sqrt(vectDot(_v, _v));
end;

// --------------------------------------------------------------------

function vectNormalize(const _v: P_Vec): T_Vec;

var _l : single;

begin
  _l := vectLen(_v);
  Result := vectDivS(_v, _l);
end;

// --------------------------------------------------------------------

procedure vectSetLen(var _v: T_Vec; const _len: single);

begin
  _v := vectMulS(@_v, _len / vectLen(@_v));
end;

// --------------------------------------------------------------------

function vectCross(const _v1, _v2: P_Vec): T_Vec;

begin
  Result[X] := (_v1^[Y] * _v2^[Z]) - (_v1^[Z] * _v2^[Y]);
  Result[Y] := (_v1^[Z] * _v2^[X]) - (_v1^[X] * _v2^[Z]);
  Result[Z] := (_v1^[X] * _v2^[Y]) - (_v1^[Y] * _v2^[X]);  
end;

// --------------------------------------------------------------------

function  vectLargestComponent(const _v: P_Vec): TXYZ;

var _abs : single;

begin
  _abs := Abs(_v^[X]);
  Result := vectX;
  if _abs < Abs(_v^[Y]) then begin _abs := Abs(_v^[Y]); Result := vectY; end;
  if _abs < Abs(_v^[Z]) then Result := vectZ;
end;

// --------------------------------------------------------------------

end.
