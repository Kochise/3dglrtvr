/* **********************************************************
 * Copyright 2005 Dimitar Vangelovski
 *
 * This file is part of the Gens2 3D Engine.
 * 
 * The Gens2 3D Engine is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 * 
 * The Gens2 3D Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with the Gens2 3D Engine; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 * **********************************************************/

{Unit for Some 3DNow! Optimized functions -
                                                                      - Made by Dime Vangelovski
                                                                                     01.09.2005}
Unit G3DNowFn;

Interface

Function Mag_Vect_3DNow (a: Pointer):single;
Function Dot_3DNow (a, b: Pointer): Single;

Implementation

/*
Function Vector_Magnitude (vec: vertex):single;
begin
  Result := Safe_Sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
end;
*/

Function Mag_Vect_3DNow (a: Pointer):single;
var r:single;
begin
    asm
        femms
        mov         eax, [a]
        movq        mm0, [eax]
        movd        mm1, [eax+8]
        pfmul       mm0, mm0
        pfmul       mm1, mm1
        pfacc       mm0, mm0
        pfadd       mm0, mm1
        pfrsqrt     mm1, mm0
        movq        mm2, mm1
        pfmul       mm1, mm1
        pfrsqit1    mm1, mm0
        pfrcpit2    mm1, mm2
        pfmul       mm0, mm1
        movd        [r], mm0
        femms
    end;
    Result := r;
end;

/*
Function Dot_Product (a, b:vertex): Single;
begin
  Result := a.x * b.x + a.y * b.y + a.z * b.z;
end;
*/

Function Dot_3DNow (a, b: Pointer): Single;
asm
   femms
   mov     EAX, [a]
   mov     EDX, [b]
   movq    mm0, [EAX]
   movq    mm3, [EDX]
   pfmul   mm0, mm3
   movd    mm2, [EAX + 8]
   movd    mm1, [EDX + 8]
   pfacc   mm0, mm0
   pfmul   mm1, mm2
   pfadd   mm0, mm1
   movd    [Result], mm0
   femms
end;

END.