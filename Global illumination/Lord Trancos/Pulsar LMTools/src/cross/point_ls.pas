(*

  DESC:

   Closest Point On Line Segment code.

   Original C source code
   by Ben Humphrey (DigiBen).
     (http://www.gametutorials.com)
     (DigiBen@GameTutorials.com)
   from his "Closest Point on a Line" tutorial.
     (http://www.gametutorials.com/download/OpenGL/ClosestPointOnPlane_OGL.zip)

   Delphi translation
   by Lord Trancos.
     (http://www.dxlab.tk - http://www.geocities.com/dxlab)
     (lordtrancos@softhome.net)

  API:

   NONE

*)

unit point_ls;

// ----------------------------------------------------------------------------

interface

uses vTypes, MathVect;

function closest_point_on_line(_lA, _lB, _p: P_Vec): T_Vec;

// ----------------------------------------------------------------------------

implementation

// ----------------------------------------------------------------------------

function closest_point_on_line(_lA, _lB, _p: P_Vec): T_Vec;

var
  _v1, _v2, _v3 : T_Vec;
  _d, _t : single;

begin
  // Create the vector from end point _lA to our point _p.
  _v1 := vectSubV(_p, _lA);
  
  // Create a direction vector from end point _lA to end point _lB
  _v2 := vectSubV(_lB, _lA);

  // Get the line segment magnitude
  _d := vectLen(@_v2);

  // normalize the direction vector
  _v2 := vectNormalize(@_v2);

  // Using the dot product, we project the vector _v1 onto the vector _v2.
  // This essentially gives us the distance from our projected vector from _lA.
  _t := vectDot(@_v2, @_v1);

  // If our projected distance from _lA, "_t", is less than or equal to 0, it must
  // be closest to the end point _vA.  We want to return this end point.
  if (_t <= 0) then
  begin
    Result := _lA^;
    exit;
  end;

  // If our projected distance from _lA, "_t", is greater than or equal to the magnitude
  // or distance of the line segment, it must be closest to the end point _lB.
  if (_t >= _d) then
  begin
    Result := _lB^;
    exit;
  end;

  // Here we create a vector that is of length _t and in the direction of _v2
  _v3 := vectMulS(@_v2, _t);

  // To find the closest point on the line segment, we just add _v3 to the original
  // end point _lA.
  Result := vectAddV(_lA, @_v3);
end;

// ----------------------------------------------------------------------------

end.
