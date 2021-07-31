(*

  DESC:

   Closest Point On Triangle.

   by Lord Trancos.
     (http://www.dxlab.tk)
     (lordtrancos@softhome.net)

  API:

   NONE

*)

unit point_t;

// ----------------------------------------------------------------------------

interface

uses vTypes, MathVect, MathTri, point_te;

function closest_point_on_triangle(_vA, _vB, _vC, _p: P_Vec): T_Vec;

// ----------------------------------------------------------------------------

implementation

// ----------------------------------------------------------------------------

function closest_point_on_triangle(_vA, _vB, _vC, _p: P_Vec): T_Vec;

var
  _orig, _normal : T_Vec;
  _ed1, _ed2 : T_Vec;
  _d : single;

begin
  // get edges
  triTwoEdges(_vA, _vB, _vC, _ed1, _ed2);  

  // get plane
  _orig   := _vA^;
  _normal := triNormal(@_ed1, @_ed2);

  // is the point inside the triangle?
  if triPointInside(_p, _vA, _vB, _vC, @_normal, 0) then
  begin
    // get d
    _d := vectDot(@_normal, @_orig);

    // project point
    Result := planeProjectPoint(@_orig, @_normal, _p, _d);
  end else Result := closest_point_on_triEdges(_vA, _vB, _vC, _p);
end;

// ----------------------------------------------------------------------------

end.
