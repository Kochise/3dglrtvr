(*

  DESC:

   vTypes
   Copyright © 2002 Lord Trancos / Pulsar Studio.
   <lordtrancos@softhome.net>

  API:

   NONE

*)

unit vTypes;

// ----------------------------------------------------------------------------

interface

const
  X = 0;
  Y = 1;
  Z = 2;

type
  // vector
  P_Vec = ^T_Vec;
  T_Vec = array[0..2] of single;

  // triangle
  P_Tri = ^T_Tri;
  T_Tri = array[0..2] of T_Vec;

  // matrix
  P_Matrix = ^T_Matrix;
  T_Matrix = record
               _11, _12, _13, _14,
               _21, _22, _23, _24,
               _31, _32, _33, _34,
               _41, _42, _43, _44 : single; 
             end;

function vtp_Vec(_x, _y, _z: single): T_Vec;

// ----------------------------------------------------------------------------

implementation

// ----------------------------------------------------------------------------

function vtp_Vec(_x, _y, _z: single): T_Vec;

begin
  Result[0] := _x;
  Result[1] := _y;
  Result[2] := _z;
end;

// ----------------------------------------------------------------------------

end.
