(*

  DESC:

   AABB-triangle overlap test code.

   Original C source code (released as public domain code)
   by Tomas Akenine-Möller.
     (http://www.ce.chalmers.se/staff/tomasm/)

   Delphi translation (and also as public domain code)
   by Lord Trancos.
     (http://www.dxlab.tk - http://www.geocities.com/dxlab)
     (lordtrancos@softhome.net)

  API:

   NONE

*)

unit aabb_tri;

// ----------------------------------------------------------------------------

interface

uses vTypes;

// main functions
function overlap_Tri_AABB(_boxcenter, _boxhalfsize: P_Vec; _tri: P_Tri): boolean;

// ----------------------------------------------------------------------------

implementation

// --------------------------------------------------------------------

function vectSubV(const _v1, _v2: P_Vec): T_Vec;

begin
  Result[X] := _v1^[X] - _v2^[X];
  Result[Y] := _v1^[Y] - _v2^[Y];
  Result[Z] := _v1^[Z] - _v2^[Z];
end;

// --------------------------------------------------------------------

function vectDot(const _v1, _v2: P_Vec): single;

begin
  Result := _v1^[X] * _v2^[X] + _v1^[Y] * _v2^[Y] + _v1^[Z] * _v2^[Z];
end;

// --------------------------------------------------------------------

function vectCross(const _v1, _v2: P_Vec): T_Vec;

begin
  Result[X] := (_v1^[Y] * _v2^[Z]) - (_v1^[Z] * _v2^[Y]);
  Result[Y] := (_v1^[Z] * _v2^[X]) - (_v1^[X] * _v2^[Z]);
  Result[Z] := (_v1^[X] * _v2^[Y]) - (_v1^[Y] * _v2^[X]);
end;

// ----------------------------------------------------------------------------

procedure findMinMax(const _x0, _x1, _x2: single;
                       var _min, _max: single);

begin
  _min := _x0;
  _max := _x0;
  if (_x1 < _min) then _min := _x1;
  if (_x1 > _max) then _max := _x1;
  if (_x2 < _min) then _min := _x2;
  if (_x2 > _max) then _max := _x2;
end;

// ----------------------------------------------------------------------------

function planeBoxOverlap(_normal: P_Vec; _d: single; _maxbox: P_Vec): boolean;

var _q: longint;
    _vmin, _vmax: T_Vec;

begin
  for _q := X to Z do
  begin
    if (_normal^[_q] > 0) then
    begin
      _vmin[_q] := -_maxbox^[_q];
      _vmax[_q] :=  _maxbox^[_q];
    end else
    begin
      _vmin[_q] :=  _maxbox^[_q];
      _vmax[_q] := -_maxbox^[_q];
    end;
  end;

  if (vectDot(_normal, @_vmin) + _d >  0) then begin Result := false; exit; end;
  if (vectDot(_normal, @_vmax) + _d >= 0) then begin Result := true;  exit; end;

  Result := false;
end;

// ----------------------------------------------------------------------------

function overlap_Tri_AABB(_boxcenter, _boxhalfsize: P_Vec; _tri: P_Tri): boolean;

var _v0, _v1, _v2 : T_Vec;
    _e0, _e1, _e2 : T_Vec;
    _normal : T_Vec;
    _d : single;
    _min, _max, _fex, _fey, _fez : single;
    _p0, _p1, _p2, _rad : single;

    // ------------------------------------------------------------

    function axistest_x01(const _a, _b, _fa, _fb: single): boolean;

    begin
      Result := false;

      _p0 := _a * _v0[Y] - _b * _v0[Z];
      _p2 := _a * _v2[Y] - _b * _v2[Z];
      if _p0 < _p2 then begin _min := _p0; _max := _p2; end
       else begin _min := _p2; _max := _p0; end;
      _rad := _fa * _boxhalfsize^[Y] + _fb * _boxhalfsize^[Z];
      if (_min > _rad) or (_max < -_rad) then exit;

      Result := true;
    end;

    // ------------------------------------------------------------

    function axistest_x2(const _a, _b, _fa, _fb: single): boolean;

    begin
      Result := false;

      _p0 := _a * _v0[Y] - _b * _v0[Z];
      _p1 := _a * _v1[Y] - _b * _v1[Z];
      if _p0 < _p1 then begin _min := _p0; _max := _p1; end
       else begin _min := _p1; _max := _p0; end;
      _rad := _fa * _boxhalfsize^[Y] + _fb * _boxhalfsize^[Z];
      if (_min > _rad) or (_max < -_rad) then exit;

      Result := true;
    end;

    // ------------------------------------------------------------

    function axistest_y02(const _a, _b, _fa, _fb: single): boolean;

    begin
      Result := false;

      _p0 := -_a * _v0[X] + _b * _v0[Z];
      _p2 := -_a * _v2[X] + _b * _v2[Z];
      if _p0 < _p2 then begin _min := _p0; _max := _p2; end
       else begin _min := _p2; _max := _p0; end;
      _rad := _fa * _boxhalfsize^[X] + _fb * _boxhalfsize^[Z];
      if (_min > _rad) or (_max < -_rad) then exit;

      Result := true;
    end;

    // ------------------------------------------------------------

    function axistest_y1(const _a, _b, _fa, _fb: single): boolean;

    begin
      Result := false;

      _p0 := -_a * _v0[X] + _b * _v0[Z];
      _p1 := -_a * _v1[X] + _b * _v1[Z];
      if _p0 < _p1 then begin _min := _p0; _max := _p1; end
       else begin _min := _p1; _max := _p0; end;
      _rad := _fa * _boxhalfsize^[X] + _fb * _boxhalfsize^[Z];
      if (_min > _rad) or (_max < -_rad) then exit;

      Result := true;
    end;

    // ------------------------------------------------------------

    function axistest_z12(const _a, _b, _fa, _fb: single): boolean;

    begin
      Result := false;

      _p1 := _a * _v1[X] - _b * _v1[Y];
      _p2 := _a * _v2[X] - _b * _v2[Y];
      if _p2 < _p1 then begin _min := _p2; _max := _p1; end
       else begin _min := _p1; _max := _p2; end;
      _rad := _fa * _boxhalfsize^[X] + _fb * _boxhalfsize^[Y];
      if (_min > _rad) or (_max < -_rad) then exit;

      Result := true;
    end;

    // ------------------------------------------------------------

    function axistest_z0(const _a, _b, _fa, _fb: single): boolean;

    begin
      Result := false;

      _p0 := _a * _v0[X] - _b * _v0[Y];
      _p1 := _a * _v1[X] - _b * _v1[Y];
      if _p0 < _p1 then begin _min := _p0; _max := _p1; end
       else begin _min := _p1; _max := _p0; end;
      _rad := _fa * _boxhalfsize^[X] + _fb * _boxhalfsize^[Y];
      if (_min > _rad) or (_max < -_rad) then exit;

      Result := true;
    end;

    // ------------------------------------------------------------

begin
  Result := false;

  // move everything so that the boxcenter is in (0,0,0)
  _v0 := vectSubV(@_tri^[0], _boxcenter);
  _v1 := vectSubV(@_tri^[1], _boxcenter);
  _v2 := vectSubV(@_tri^[2], _boxcenter);

  // compute triangle edges
  _e0 := vectSubV(@_v1, @_v0); // edge 0
  _e1 := vectSubV(@_v2, @_v1); // edge 1
  _e2 := vectSubV(@_v0, @_v2); // edge 2

  // test the 9 tests first (this was faster)
  _fex := abs(_e0[X]);
  _fey := abs(_e0[Y]);
  _fez := abs(_e0[Z]);
  if not axistest_x01(_e0[Z], _e0[Y], _fez, _fey) then exit;
  if not axistest_y02(_e0[Z], _e0[X], _fez, _fex) then exit;
  if not axistest_z12(_e0[Y], _e0[X], _fey, _fex) then exit;

  _fex := abs(_e1[X]);
  _fey := abs(_e1[Y]);
  _fez := abs(_e1[Z]);
  if not axistest_x01(_e1[Z], _e1[Y], _fez, _fey) then exit;
  if not axistest_y02(_e1[Z], _e1[X], _fez, _fex) then exit;
  if not  axistest_z0(_e1[Y], _e1[X], _fey, _fex) then exit;

  _fex := abs(_e2[X]);
  _fey := abs(_e2[Y]);
  _fez := abs(_e2[Z]);
  if not  axistest_x2(_e2[Z], _e2[Y], _fez, _fey) then exit;
  if not  axistest_y1(_e2[Z], _e2[X], _fez, _fex) then exit;
  if not axistest_z12(_e2[Y], _e2[X], _fey, _fex) then exit;

  // test overlap in the {x,y,z}-directions
  // find min, max of the triangle each direction, and test for overlap in
  // that direction -- this is equivalent to testing a minimal AABB around
  // the triangle against the AABB

  // test in X-direction
  findMinMax(_v0[X], _v1[X], _v2[X], _min, _max);
  if (_min > _boxhalfsize^[X]) or (_max < -_boxhalfsize^[X]) then exit;

  // test in Y-direction
  findMinMax(_v0[Y], _v1[Y], _v2[Y], _min, _max);
  if (_min > _boxhalfsize^[Y]) or (_max < -_boxhalfsize^[Y]) then exit;

  // test in Z-direction
  findMinMax(_v0[Z], _v1[Z], _v2[Z], _min, _max);
  if (_min > _boxhalfsize^[Z]) or (_max < -_boxhalfsize^[Z]) then exit;

  // test if the box intersects the plane of the triangle
  // compute plane equation of triangle: normal*x+d=0
  _normal := vectCross(@_e0, @_e1);
  _d := - vectDot(@_normal, @_v0); // plane eq: normal.x+d=0
  if not planeBoxOverlap(@_normal, _d, _boxhalfsize) then exit;

  Result := true; // box and triangle overlaps
end;

// ----------------------------------------------------------------------------

end.
