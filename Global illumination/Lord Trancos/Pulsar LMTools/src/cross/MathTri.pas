(*

  DESC:

   Triangle Maths unit
   by Lord Trancos.
     (http://www.dxlab.tk)
     (lordtrancos@softhome.net)

  API:

   NONE

*)

unit MathTri;

// --------------------------------------------------------------------

interface

uses vTypes, MathVect;

type TPlaneSide = (psFront, psBack, psOnPlane);

procedure triTwoEdges(const _v1, _v2, _v3 : P_Vec;
                       var _edge1, _edge2 : T_Vec);
function  triNormal(const _edge1, _edge2 : P_Vec): T_Vec;
procedure triGetPlane(const _v1, _v2, _v3 : P_Vec;
                       var _orig, _normal : T_Vec);
function  triPointInside(const _point, _v1, _v2, _v3, _normal : P_Vec;
                         const _epsilon: single): boolean;
function  triIntersectRay(const _orig, _dir, // ray
                                _v1, _edge1, _edge2 : P_Vec; // triangle
                            var _dist, _u, _v: single): boolean;
function  triIntersectRayEx(const _orig, _dir, // ray
                                  _v1, _edge1, _edge2 : P_Vec; // triangle
                              var _dist, _u, _v: single): boolean;

function  planeGetD(const _orig, _normal: P_Vec): single;
function  planeClassifyPoint(const _orig, _normal, _point: P_Vec;
                             const _d: single): TPlaneSide;
function  planeProjectPoint(const _orig, _normal, _point: P_Vec;
                            const _d: single): T_Vec;
function  planeDistancePoint(const _normal, _point: P_Vec;
                             const _d: single): single;

(*

function  polyNormalA(const _edge1, _edge2 : TD3DVector): TD3DVector;
function  polyNormalB(const _v1, _v2, _v3 : TD3DVector): TD3DVector;
function  polyCenterOfMass(const _v1, _v2, _v3 : TD3DVector): TD3DVector;
function  polyIntersectRayA(const _orig, _dir, _v1,
                                  _edge1, _edge2 : TD3DVector;
                            var _dist, _u, _v: single): boolean;
function  polyIntersectRayB(const _orig, _dir,
                                  _v1, _v2, _v3 : TD3DVector;
                            var _dist, _u, _v: single): boolean;

function  planeDirFaces(const _dir, _normal: TD3DVector): boolean;
function  planePointFaces(const _p, _v1,
                                _normal: TD3DVector): boolean;

*)

// --------------------------------------------------------------------

implementation

// --------------------------------------------------------------------

procedure triTwoEdges(const _v1, _v2, _v3 : P_Vec;
                       var _edge1, _edge2 : T_Vec);

begin
  _edge1 := vectSubV(_v2, _v1);
  _edge2 := vectSubV(_v3, _v1);
end;

// --------------------------------------------------------------------

function triNormal(const _edge1, _edge2 : P_Vec): T_Vec;

begin
  Result := vectCross(_edge1, _edge2);
  Result := vectDivS(@Result, vectLen(_edge1) * vectLen(_edge2));
  Result := vectNormalize(@Result);
end;

// --------------------------------------------------------------------

procedure triGetPlane(const _v1, _v2, _v3 : P_Vec;
                       var _orig, _normal : T_Vec);

var
  _e1, _e2 : T_Vec;

begin
  _orig := _v1^;
  triTwoEdges(_v1, _v2, _v3, _e1, _e2);
  _normal := triNormal(@_e1, @_e2);
end;

// --------------------------------------------------------------------

function triPointInside(const _point, _v1, _v2, _v3, _normal : P_Vec;
                        const _epsilon: single): boolean;

  // -----------------------------------

  function halfPlane(const _p0, _p1 : P_Vec): single;

  var _edge, _n : T_Vec;

  begin
    _edge  := vectSubV(_p1, _p0);
    _n     := vectCross(@_edge, _normal);
    Result := vectDot(_point, @_n) - vectDot(_p0, @_n);
  end;

  // -----------------------------------

var _hp1, _hp2, _hp3 : single;

begin
  _hp1 := halfPlane(_v1, _v2);
  _hp2 := halfPlane(_v2, _v3);
  _hp3 := halfPlane(_v3, _v1);
  Result := (_hp1 < _epsilon) and (_hp2 < _epsilon) and (_hp3 < _epsilon);
end;

// --------------------------------------------------------------------

function triIntersectRay(const _orig, _dir, // ray
                               _v1, _edge1, _edge2 : P_Vec; // triangle
                           var _dist, _u, _v: single): boolean;

// Returns the interpolated texture coordinates (_u, _v) and the
// distance (_dist) if the ray intersects the triangle.

var _pvec, _tvec, _qvec : T_Vec;
    _det, _invDet : single;

begin
  Result := false;

  _pvec := vectCross(_dir, _edge2);

  // Begin calculating determinant - also used to calculate U parameter
  _det := vectDot(_edge1, @_pvec);

  // If determinant is near zero, ray lies in plane of triangle
  if (_det < 0.0001) then exit;

  // Calculate distance from vert1 to ray origin
  _tvec := vectSubV(_orig, _v1);

  // Calculate U parameter and test bounds
  _u := vectDot(@_tvec, @_pvec);
  if (_u < 0) or (_u > _det) then exit;

  // Prepare to test V parameter
  _qvec := vectCross(@_tvec, _edge1);

  // Calculate V parameter and test bounds
  _v := vectDot(_dir, @_qvec);
  if (_v < 0) or (_u + _v > _det) then exit;

  // Calculate distance, scale parameters, ray intersects triangle
  _invDet := 1.0 / _det;
  _dist := vectDot(_edge2, @_qvec) * _invDet;
  _u := _u * _invDet;
  _v := _v * _invDet;

  // Intersects
  Result := true;
end;

// --------------------------------------------------------------------

function triIntersectRayEx(const _orig, _dir, // ray
                                 _v1, _edge1, _edge2 : P_Vec; // triangle
                             var _dist, _u, _v: single): boolean;

// the same that triIntersectRay, but also intersects with the backface
// of the triangle.

var _pvec, _tvec, _qvec : T_Vec;
    _det, _invDet : single;

begin
  Result := false;

  _pvec := vectCross(_dir, _edge2);

  // Begin calculating determinant - also used to calculate U parameter
  _det := vectDot(_edge1, @_pvec);

  // If determinant is near zero, ray lies in plane of triangle
  if (_det > 0) then
  begin
    // Calculate distance from vert1 to ray origin
    _tvec := vectSubV(_orig, _v1);
  end else
  begin
    // Calculate distance from vert1 to ray origin
    _tvec := vectSubV(_v1, _orig);
    _det := -_det;
  end;
  if (_det < 0.0001) then exit;

  // Calculate U parameter and test bounds
  _u := vectDot(@_tvec, @_pvec);
  if (_u <= 0) or (_u >= _det) then exit;

  // Prepare to test V parameter
  _qvec := vectCross(@_tvec, _edge1);

  // Calculate V parameter and test bounds
  _v := vectDot(_dir, @_qvec);
  if (_v <= 0) or (_u + _v >= _det) then exit;

  // Calculate distance, scale parameters, ray intersects triangle
  _invDet := 1.0 / _det;
  _dist := vectDot(_edge2, @_qvec) * _invDet;
  _u := _u * _invDet;
  _v := _v * _invDet;

  // Intersects
  Result := true;
end;

// --------------------------------------------------------------------

function planeGetD(const _orig, _normal: P_Vec): single;

begin
  Result := vectDot(_normal, _orig);
end;

// --------------------------------------------------------------------

function planeClassifyPoint(const _orig, _normal, _point: P_Vec;
                            const _d: single): TPlaneSide;

// _orig, _normal = plane
// _d -> vectDot(_normal, _orig);

var
  _dist : single;

begin
  _dist := vectDot(_normal, _point);

  Result := psOnPlane;
  if (_dist < _d) then
  begin
    Result := psBack;
    exit;
  end;
  if (_dist > _d) then Result := psFront;
end;

// --------------------------------------------------------------------

function planeProjectPoint(const _orig, _normal, _point: P_Vec;
                           const _d: single): T_Vec;

var
  _dist : single;
  _n : T_Vec;

begin
  _dist := vectDot(_normal, _point);

  if (_dist = _d) then
  begin
    Result := _point^;
    exit;
  end;

  _n := vectMulS(_normal, _d - _dist);

  if (_dist < _d) then
  begin
    Result := vectAddV(_point, @_n);
    exit
  end;
  if (_dist > _d) then Result := vectSubV(_point, @_n);
end;

// --------------------------------------------------------------------

function  planeDistancePoint(const _normal, _point: P_Vec;
                             const _d: single): single;

begin
  Result := vectDot(_normal, _point) + _d;
end;

// --------------------------------------------------------------------

(*
function polyNormalB(const _v1, _v2, _v3 : TD3DVector): TD3DVector;

var _edge1, _edge2 : TD3DVector;

begin
  polyTwoEdges(_v1, _v2, _v3, _edge1, _edge2);
  Result := polyNormalA(_edge1, _edge2);
end;

// --------------------------------------------------------------------

function polyCenterOfMass(const _v1, _v2, _v3 : TD3DVector): TD3DVector;

begin
  with Result do
  begin
    x := (_v1.x + _v2.x + _v3.x) / 3;
    y := (_v1.y + _v2.y + _v3.y) / 3;
    z := (_v1.z + _v2.z + _v3.z) / 3;
  end;
end;

// --------------------------------------------------------------------

function polyIntersectRayA(const _orig, _dir, _v1,
                                 _edge1, _edge2 : TD3DVector;
                           var _dist, _u, _v: single): boolean;

// Returns the interpolated texture coordinates (_u, _v) and the
// distance (_dist) if the ray intersects the triangle.

var _pvec, _tvec, _qvec : TD3DVector;
    _det, _invDet : single;

begin
  Result := false;

  _pvec := vectCross(_dir, _edge2);

  // Begin calculating determinant - also used to calculate U parameter
  _det := vectDot(_edge1, _pvec);

  // If determinant is near zero, ray lies in plane of triangle
  if (_det < 0.0001) then exit;

  // Calculate distance from vert1 to ray origin
  _tvec := vectSubV(_orig, _v1);

  // Calculate U parameter and test bounds
  _u := vectDot(_tvec, _pvec);
  if (_u < 0.0) or (_u > _det) then exit;

  // Prepare to test V parameter
  _qvec := vectCross(_tvec, _edge1);

  // Calculate V parameter and test bounds
  _v := vectDot(_dir, _qvec);
  if (_v < 0.0) or (_u + _v > _det) then exit;

  // Calculate distance, scale parameters, ray intersects triangle
  _invDet := 1.0 / _det;
  _dist := vectDot(_edge2, _qvec) * _invDet;
  _u := _u * _invDet;
  _v := _v * _invDet;

  // Intersects
  Result := true;
end;

// --------------------------------------------------------------------

function  polyIntersectRayB(const _orig, _dir,
                                  _v1, _v2, _v3 : TD3DVector;
                            var _dist, _u, _v: single): boolean;

var _edge1, _edge2 : TD3DVector;

begin
  polyTwoEdges(_v1, _v2, _v3, _edge1, _edge2);
  Result := polyIntersectRayA(_orig, _dir, _v1, _edge1, _edge2,
                              _dist, _u, _v);
end;

// --------------------------------------------------------------------

function planeDirFaces(const _dir, _normal: TD3DVector): boolean;

// Returns true if a direction "goes to" a plane.

begin
  Result := (vectDot(_dir, _normal) < -0.0001);
end;

// --------------------------------------------------------------------

function planePointFaces(const _p, _v1,
                               _normal: TD3DVector): boolean;

// Returns true if a point faces the plane.
// _v1 is plane's first vertex.
// _normal is plane's normal.

begin
  Result := planeDirFaces(vectSubV(_v1, _p), _normal);
end;
*)
// --------------------------------------------------------------------

end.
