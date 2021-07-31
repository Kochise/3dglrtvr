// ---------------------------------------------------------------------------
// Pulsar LMTools v1.1
// Copyright (C) 2002, 2003 Lord Trancos. All rights reserved.
// ---------------------------------------------------------------------------
// 
// This file is part of Pulsar LMTools
// 
// Pulsar LMTools is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or any later version.
// 
// Pulsar LMTools is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details (license.txt).
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// 
// You should also be able to get a copy of the license here:
//   
//   http://www.gnu.org/licenses/gpl.txt
// 
// ---------------------------------------------------------------------------
//
// Developed by Lord Trancos <lordtrancos@softhome.net> 
//
// ---------------------------------------------------------------------------
//
// DXLab - Visit us at: http://www.dxlab.tk or http://dxlab.host.sk
//
// ---------------------------------------------------------------------------

// shadow casting related stuff

unit shadowc;

// ---------------------------------------------------------------------------

interface

uses Global, vTypes, MathVect, MathTri, aabb_ls, Octree, lmts, rgbcolor;

function ShadowCast(_triIdx: word; _light, _p: P_Vec): cardinal;

// ---------------------------------------------------------------------------

implementation

// ---------------------------------------------------------------------------

procedure PolyUVtoTextureUV(const _u, _v, _u0, _v0,
                                  _u1, _v1, _u2, _v2: single;
                            var _tu, _tv: single);

var
  _ui1, _vi1 : single;
  _ui2, _vi2 : single;

begin
  _ui1 := _u1 - _u0;
  _vi1 := _v1 - _v0;
  _ui2 := _u2 - _u0;
  _vi2 := _v2 - _v0;

  _tu := _u0 + ((_ui1 * _u) + (_ui2 * _v));
  _tv := _v0 + ((_vi1 * _u) + (_vi2 * _v));

  if _tu >= 0 then _tu := frac(_tu) else _tu := 1 + frac(_tu);
  if _tv >= 0 then _tv := frac(_tv) else _tv := 1 + frac(_tv);
end;

// ---------------------------------------------------------------------------

function GetTextureColorAt(_texId: longint;
                           _u, _v: single): cardinal;

var
  _x, _y : longint;
  _cur : pointer;

begin
  with g_MeshTextures[_texId] do
  begin
    _x := round((header.width-1)  * _u);
    _y := round((header.height-1) * _v);

    _cur := data;
    inc(longint(_cur), ((_y * header.width) + _x) * 4);
    Result := cardinal(_cur^);
  end;
end;

// ---------------------------------------------------------------------------

procedure AdjustByAlpha(var _c: cardinal; _alpha: byte);

var
  _r, _g, _b : byte;

begin
  _alpha := not _alpha;

  _r := (_c shr 16) and $FF;
  _g := (_c shr 8) and $FF;
  _b := _c and $FF;

  if _alpha < 128 then
  begin
    _r := (_alpha * _r) div 127;
    _g := (_alpha * _g) div 127;
    _b := (_alpha * _b) div 127;
  end else
  begin
    _r := _r + ((_alpha - 128) * (255 - _r)) div 127;
    _g := _g + ((_alpha - 128) * (255 - _g)) div 127;
    _b := _b + ((_alpha - 128) * (255 - _b)) div 127;
  end;

  _c := (_r shl 16) + (_g shl 8) + _b;
end;

// ---------------------------------------------------------------------------

procedure CheckNode(_triIdx: word;
                    _n: POctNode;
                    const _light, _dest,     // line segment (light->point)
                          _dir: P_Vec;       // light direction
                    const _distance: single; // distance from light to point
                    var _result: cardinal);

var
  _int : T_Vec;
  _cnt : longint;
  _cur : POctItem;
  _dist, _u, _v, _tu, _tv : single;
  _alpha : byte;
  _colT : cardinal;

begin
  // check light ray against node aabb
  if overlap_LineSegment_AABB(@_n^.aabb.min, @_n^.aabb.max, _light, _dest, _int) then
  begin
    // it's splited ?
    if (_n^.splited) then
    begin
      for _cnt := 0 to 7 do
        if assigned(_n^.child[_cnt]) then
        begin
          CheckNode(_triIdx, _n^.child[_cnt], _light, _dest, _dir, _distance,
                    _result);
          if _result = 0 then exit;
        end;
    end else
    begin
      // if not splited check triangles on this node
      _cur := _n^.itemData;
      for _cnt := 0 to _n^.itemCnt - 1 do
      begin
        if (_cur^.triIdx <> _triIdx) and        // isn't current triangle
           (g_TriData[_cur^.triIdx].valid) and  // is a valid triangle
           (g_TriData[_cur^.triIdx].lastRayID <> g_RayID) then // not checked yet
        with g_TriData[_cur^.triIdx] do
        begin
          // mark as checked by this ray
          g_TriData[_cur^.triIdx].lastRayID := g_RayID;

          // check intersection
          if (triIntersectRayEx(_light, _dir, P_Vec(a), @e0, @e1,
                                 _dist, _u, _v)) then
          begin
            if (_dist > 0) and (_dist < _distance - g_Collision_Dist) then
            begin
              if g_Use_Alpha_Channel then
              begin
                // any texture ?
                if (textID <> $FFFF) then
                begin
                  // get texture UV coordinates
                  PolyUVtoTextureUV(_u, _v, a^.u1, a^.v1, b^.u1, b^.v1,
                                    c^.u1, c^.v1, _tu, _tv);

                  // get pixel color value at that UV coordinates
                  _colT := GetTextureColorAt(textID, _tu, _tv);

                  // get alpha value
                  _alpha := (_colT shr 24) and $FF;

                  // adjust by alpha value
                  AdjustByAlpha(_colT, _alpha);

                  // filter light
                  _result := FilterRGB8Color(_result, _colT);
                end else _result := 0;

                // if current color value is black, exit from here
              end else _result := 0;
              if _result = 0 then exit;
            end;
          end;
        end;

        // next tri
        inc(_cur);
      end;
    end;
  end;
end;

// ---------------------------------------------------------------------------

function ShadowCast(_triIdx: word; _light, _p: P_Vec): cardinal;

var
  _dir : T_Vec;
  _dst : single;

begin
  // build ray
  _dir := vectSubV(_p, _light);
  _dst := vectLen(@_dir);        // get distance from light to point
  _dir := vectNormalize(@_dir);  // normalize direction

  // shadow cast
  Result := $FFFFFF;
  inc(g_RayID); // increase Ray ID
  CheckNode(_triIdx, @g_Octree.root, _light, _p, @_dir, _dst, Result);
end;

// ---------------------------------------------------------------------------

end.
