(*

  DESC:

   Closest Point On Triangle Edges.

   by Lord Trancos.
     (http://www.dxlab.tk)
     (lordtrancos@softhome.net)

  API:

   NONE

*)

unit point_te;

// ----------------------------------------------------------------------------

interface

uses vTypes, MathVect, point_ls;

function closest_point_on_triEdges(_vA, _vB, _vC, _p: P_Vec): T_Vec;

// ----------------------------------------------------------------------------

implementation

// ----------------------------------------------------------------------------

function closest_point_on_triEdges(_vA, _vB, _vC, _p: P_Vec): T_Vec;

var
  _ab, _bc, _ca, _t : T_Vec;
  _lab, _lbc, _lca, _lt : single;

begin
  _ab := closest_point_on_line(_vA, _vB, _p);
  _bc := closest_point_on_line(_vB, _vC, _p);
  _ca := closest_point_on_line(_vC, _vA, _p);

  _t := vectSubV(_p, @_ab);
  _lab := vectLen(@_t);

  _t := vectSubV(_p, @_bc);
  _lbc := vectLen(@_t);

  _t := vectSubV(_p, @_ca);
  _lca := vectLen(@_t);

  _lt := _lab;
  Result := _ab;

  if _lbc < _lt then
  begin
    _lt := _lbc;
    Result := _bc;
  end;

  if _lca < _lt then Result := _ca;     
end;

// ----------------------------------------------------------------------------

end.
