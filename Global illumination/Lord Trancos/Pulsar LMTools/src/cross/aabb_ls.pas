(*

  DESC:

   AABB-line segment overlap test code.

   Original C source code
   (C) 1995-2001 Volition, Inc.
     (http://www.volition-inc.com/)
     (ftp://ftp.gdmag.com/pub/src/aug01.zip)

   Delphi translation
   by Lord Trancos.
     (http://www.dxlab.tk - http://www.geocities.com/dxlab)
     (lordtrancos@softhome.net)

  API:

   NONE

*)

unit aabb_ls;

// ----------------------------------------------------------------------------

interface

uses vTypes;

function overlap_LineSegment_AABB(_min, _max, _p1, _p2: P_Vec;
                                  var _int: T_Vec): boolean;

// ----------------------------------------------------------------------------

implementation

const
  // cohen-sutherland clipping outcodes
  CLIP_RIGHT  = 1 shl 0;
  CLIP_LEFT   = 1 shl 1;
  CLIP_TOP    = 1 shl 2;
  CLIP_BOTTOM = 1 shl 3;
  CLIP_FRONT  = 1 shl 4;
  CLIP_BACK   = 1 shl 5;

// ----------------------------------------------------------------------------

// calculates the cohen-sutherland outcode for a point and a bounding box.
//
// min: min vector of the bounding box
// max: max vector of the bounding box
// pnt: the point to check
//
// returns: the outcode
//

function calc_outcode(_min, _max, _pnt : P_Vec): longint;

begin
  Result := 0;

  if _pnt^[X] > _max^[X] then Result := Result or CLIP_RIGHT else
    if _pnt^[X] < _min^[X] then Result := Result or CLIP_LEFT;

  if _pnt^[Y] > _max^[Y] then Result := Result or CLIP_TOP else
    if _pnt^[Y] < _min^[Y] then Result := Result or CLIP_BOTTOM;

  if _pnt^[Z] > _max^[Z] then Result := Result or CLIP_BACK else
    if _pnt^[Z] < _min^[Z] then Result := Result or CLIP_FRONT;
end;

// ----------------------------------------------------------------------------

// determines if a linesegment intersects a bounding box. this is based on
//  the cohen-sutherland line-clipping algorithm.
//
// min: bounding box min vector
// max: bounding box max vector
// p1: end point of line segment
// p2: other end point
// int: (out) the point in/on the bounding box where the intersection 
//            occured.  note that this point may not be on the surface of the box.
//
// returns: true if the segment and box intersect.
//

function overlap_LineSegment_AABB(_min, _max, _p1, _p2: P_Vec;
                                  var _int: T_Vec): boolean;

var
  _oc1, _oc2 : longint;
  _v1, _v2 : single;

begin
  Result := false; 

  // point 1 inside bounding box
  _oc1 := calc_outcode(_min, _max, _p1);
  if (_oc1 = 0) then
  begin
    _int := _p1^;
    Result := true;
    exit;
  end;

  // point 2 inside bounding box
  _oc2 := calc_outcode(_min, _max, _p2);
  if (_oc2 = 0) then
  begin
    _int := _p2^;
    Result := true;
    exit;
  end;

  // both points on same side of box
  if (_oc1 and _oc2) > 0 then exit;

  // check intersections
  if ((_oc1 and (CLIP_RIGHT or CLIP_LEFT)) > 0) then
  begin
    if ((_oc1 and CLIP_RIGHT) > 0) then _int[X] := _max^[X] else _int[X] := _min^[X];
    _v1 := _p2^[X] - _p1^[X];
    _v2 := _int[X] - _p1^[X];
    _int[Y] := _p1^[Y] + _v2 * (_p2^[Y] - _p1^[Y]) / _v1;
    _int[Z] := _p1^[Z] + _v2 * (_p2^[Z] - _p1^[Z]) / _v1;
    if (_int[Y] <= _max^[Y]) and (_int[Y] >= _min^[Y]) and
       (_int[Z] <= _max^[Z]) and (_int[Z] >= _min^[Z]) then
    begin
      Result := true;
      exit;
    end;
  end;

  if ((_oc1 and (CLIP_TOP or CLIP_BOTTOM)) > 0) then
  begin
    if ((_oc1 and CLIP_TOP) > 0) then _int[Y] := _max^[Y] else _int[Y] := _min^[Y];
    _v1 := _p2^[Y] - _p1^[Y];
    _v2 := _int[Y] - _p1^[Y];
    _int[X] := _p1^[X] + _v2 * (_p2^[X] - _p1^[X]) / _v1;
    _int[Z] := _p1^[Z] + _v2 * (_p2^[Z] - _p1^[Z]) / _v1;
    if (_int[X] <= _max^[X]) and (_int[X] >= _min^[X]) and
       (_int[Z] <= _max^[Z]) and (_int[Z] >= _min^[Z]) then
    begin
      Result := true;
      exit;
    end;
  end;

  if ((_oc1 and (CLIP_FRONT or CLIP_BACK)) > 0) then
  begin
    if ((_oc1 and CLIP_BACK) > 0) then _int[Z] := _max^[Z] else _int[Z] := _min^[Z];
    _v1 := _p2^[Z] - _p1^[Z];
    _v2 := _int[Z] - _p1^[Z];
    _int[X] := _p1^[X] + _v2 * (_p2^[X] - _p1^[X]) / _v1;
    _int[Y] := _p1^[Y] + _v2 * (_p2^[Y] - _p1^[Y]) / _v1;
    if (_int[X] <= _max^[X]) and (_int[X] >= _min^[X]) and
       (_int[Y] <= _max^[Y]) and (_int[Y] >= _min^[Y]) then
    begin
      Result := true;
      exit;
    end;
  end;
end;

// ----------------------------------------------------------------------------

end.