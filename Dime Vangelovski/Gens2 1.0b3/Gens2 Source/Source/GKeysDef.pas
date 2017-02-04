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

{Unit for Keys management -
                                                                   - Made by Dime Vangelovski
                                                                                  29.08.2005}
Unit GKeysDef;

Interface

Var
    key_q,key_w,key_e,key_r,key_t,key_y,key_u,key_i,key_o,key_p,key_a,key_s,
    key_d,key_f,key_g,key_h,key_j,key_k,key_l,key_z,key_x,key_c,key_v,key_b,
    key_n,key_m,key_pomalo,key_pogolemo,key_eden,key_dva,key_tri,key_cetiri,
    key_pet,key_ses,key_sedum,key_osum,key_devet,key_nula,key_minus,key_plus,
    key_zmija,key_zin,key_zout:SmallInt;

Function KeyState(vknum: SmallInt): Boolean;
Procedure VK_Codes;

Implementation

Uses
     Windows;

Function KeyState(vknum: SmallInt): Boolean;
Var
   Ktest: word;
begin
  Ktest := (GetAsyncKeyState(vknum) and $8000) or
          ((GetAsyncKeyState(vknum) shl 15) and $8000);
  if (Ktest > 0) then
    Result := TRUE
  else
    Result := FALSE;
end;

Procedure VK_Codes;
begin
  key_q := VkKeyScan('q');
  key_w := VkKeyScan('w');
  key_e := VkKeyScan('e');
  key_r := VkKeyScan('r');
  key_t := VkKeyScan('t');
  key_y := VkKeyScan('y');
  key_u := VkKeyScan('u');
  key_i := VkKeyScan('i');
  key_o := VkKeyScan('o');
  key_p := VkKeyScan('p');
  key_a := VkKeyScan('a');
  key_s := VkKeyScan('s');
  key_d := VkKeyScan('d');
  key_f := VkKeyScan('f');
  key_g := VkKeyScan('g');
  key_h := VkKeyScan('h');
  key_j := VkKeyScan('j');
  key_k := VkKeyScan('k');
  key_l := VkKeyScan('l');
  key_z := VkKeyScan('z');
  key_x := VkKeyScan('x');
  key_c := VkKeyScan('c');
  key_v := VkKeyScan('v');
  key_b := VkKeyScan('b');
  key_n := VkKeyScan('n');
  key_m := VkKeyScan('m');
  key_pomalo := VkKeyScan(',');
  key_pogolemo := VkKeyScan('.');
  key_eden := VkKeyScan('1');
  key_dva := VkKeyScan('2');
  key_tri := VkKeyScan('3');
  key_cetiri := VkKeyScan('4');
  key_pet := VkKeyScan('5');
  key_ses := VkKeyScan('6');
  key_sedum := VkKeyScan('7');
  key_osum := VkKeyScan('8');
  key_devet := VkKeyScan('9');
  key_nula := VkKeyScan('0');
  key_minus := VkKeyScan('-');
  key_plus := VkKeyScan('+');
  key_zmija := VkKeyScan('`');
  key_zin := VkKeyScan('[');
  key_zout := VkKeyScan(']');
end;

END.