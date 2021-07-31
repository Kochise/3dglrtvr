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

// Lights related stuff

unit lights;

// ---------------------------------------------------------------------------

interface

uses vTypes, Math, MathVect;

type
  PDirectLight = ^TDirectLight;
  POmni_Point  = ^TOmni_Point;
  PSpotLight   = ^TSpotLight;

  TAmbient_Global = record
    Color : longint; // In R8G8B8 format.
  end;

  TDirectLight = record
    // user values
    Direction   : T_Vec;
    Multiplier  : single;
    Color       : longint; // In R8G8B8 format.
    CastShadows : boolean;
    Enabled     : boolean;
    // app values
    Ena4Tri     : boolean;
    SCCorners   : array of cardinal;
    InvDir      : T_Vec;
  end;

  TOmni_Point = record
    // user values
    Position    : T_Vec;
    Multiplier  : single;
    AttStart    : single;  // Attenuation start.
    AttEnd      : single;  // Attenuation end.
    Color       : longint; // In R8G8B8 format.
    CastShadows : boolean;
    Enabled     : boolean;
    // app values
    AttRange    : single;
    Ena4Tri     : boolean;
    SCCorners   : array of cardinal;
  end;

  TSpotLight = record
    Position    : T_Vec;
    Target      : T_Vec;
    Theta       : single;   // Inner cone.
    Phi         : single;   // Outer cone.
    Multiplier  : single;
    AttStart    : single;   // Attenuation start.
    AttEnd      : single;   // Attenuation end.
    Color       : longint;  // In R8G8B8 format.
    CastShadows : boolean;
    Enabled     : boolean;
    // app values
    AttRange    : single;
    ConeRange   : single;
    Ena4Tri     : boolean;
    SCCorners   : array of cardinal;
    Direction   : T_Vec;    
  end;

var
  gAmbient_Global : TAmbient_Global;
  gDirectLight    : TDirectLight;
  gOmni_Point     : TOmni_Point;
  gSpotLight      : TSpotLight;

  gADirectLight   : array of TDirectLight = NIL;
  gAOmni_Point    : array of TOmni_Point = NIL;
  gASpotLight     : array of TSpotLight = NIL;

procedure lit_Add_DirectLight; stdcall;
procedure lit_Add_Omni_Point; stdcall;
procedure lit_Add_SpotLight; stdcall;

function lit_DirectLight(const _normal: P_Vec;
                         const _direct: PDirectLight): longint;
function lit_Omni_Point(const _point, _normal: P_Vec;
                        const _light: POmni_Point): longint;
function lit_SpotLight(const _point, _normal: P_Vec;
                       const _spot: PSpotlight): longint;

// ---------------------------------------------------------------------------

implementation

// ---------------------------------------------------------------------------

procedure InitLightsWithDefaultValues;

begin
  // Ambient color
  gAmbient_Global.Color := $000000;

  // Directional light
  with gDirectLight do
  begin
    Direction[X] := 0;
    Direction[Y] := -1;
    Direction[Z] := 0;
    Multiplier   := 1.0;
    Color        := $FFFFFF;
    CastShadows  := true;
    Enabled      := true;
    // app values
    Ena4Tri      := false;
    SCCorners    := NIL;
  end;

  // Omnidirectional point light
  with gOmni_Point do
  begin
    // user
    Position[X] := 0;
    Position[Y] := 0;
    Position[Z] := 0;
    Multiplier  := 1.0;
    AttStart    := 256;
    AttEnd      := 512;
    Color       := $FFFFFF;
    CastShadows := true;
    Enabled     := true;
    // app
    AttRange    := AttEnd - AttStart;
    Ena4Tri     := false;
    SCCorners   := NIL;
  end;

  // Spot light
  with gSpotLight do
  begin
    // user
    Position[X] := 0;
    Position[Y] := 0;
    Position[Z] := 0;
    Target[X]   :=  0;
    Target[Y]   := -1;
    Target[Z]   :=  0;
    Theta       := 0.5;
    Phi         := 1.0;
    Multiplier  := 1.0;
    AttStart    := 256;
    AttEnd      := 512;
    Color       := $FFFFFF;
    CastShadows := true;
    Enabled     := true;
    // app
    AttRange    := AttEnd - AttStart;
    ConeRange   := Phi - Theta;
    Ena4Tri     := false;
    SCCorners   := NIL;
    Direction   := Target;
  end;
end;

// ---------------------------------------------------------------------------

procedure lit_Add_DirectLight; stdcall;

var
  _idx : longint;

begin
  // add
  SetLength(gADirectLight, Length(gADirectLight)+1);
  _idx := Length(gADirectLight) - 1;
  gADirectLight[_idx] := gDirectLight;
  // normalize direction
  gADirectLight[_idx].Direction := vectNormalize(@gADirectLight[_idx].Direction);
  // get inverse direction
  gADirectLight[_idx].InvDir := vectInv(@gADirectLight[_idx].Direction);
end;

// ---------------------------------------------------------------------------

procedure lit_Add_Omni_Point; stdcall;

var _idx : longint;

begin
  // add
  SetLength(gAOmni_Point, Length(gAOmni_Point)+1);
  _idx := Length(gAOmni_Point) - 1;
  gAOmni_Point[_idx] := gOmni_Point;
  // get att. range
  with gAOmni_Point[_idx] do AttRange := AttEnd - AttStart;
end;

// ---------------------------------------------------------------------------

procedure lit_Add_SpotLight; stdcall;

var
  _idx : longint;
  _v : T_Vec;

begin
  // add
  SetLength(gASpotLight, Length(gASpotLight)+1);
  _idx := Length(gASpotLight) - 1;
  gASpotLight[_idx] := gSpotLight;
  // get direction
  _v := vectSubV(@gASpotLight[_idx].Target, @gASpotLight[_idx].Position);
  gASpotLight[_idx].Direction := vectNormalize(@_v);
  // get att. range
  with gASpotLight[_idx] do AttRange := AttEnd - AttStart;
  // get cone range
  with gASpotLight[_idx] do ConeRange := Phi - Theta;
end;

// ---------------------------------------------------------------------------

function FinalColor(const _byt: byte;
                    const _lightColor: longint): longint;

// Returns the final light color in R8G8B8 format.

var _r, _g, _b : byte;

begin
  // Get RGB Values for light color.
  _r := (_lightColor shr 16) and $FF;
  if _byt < _r then _r := _byt;
  _g := (_lightColor shr 8) and $FF;
  if _byt < _g then _g := _byt;
  _b := _lightColor and $FF;
  if _byt < _b then _b := _byt;

  // Compose color.
  Result := (_r shl 16) + (_g shl 8) + _b;
end;

// -------------------------------------------------------------------

function lit_DirectLight(const _normal: P_Vec;
                         const _direct: PDirectLight): longint;

var
  _dot : single;
  _val : longint;

begin
  Result := 0;

  // since bot vectors are normalized, the dot product is = the angle
  _dot := - vectDot(@_direct.Direction, _normal);

  // _dot should range from 0 to 1, where 1 means an angle of 0 degrees
  if (_dot > 0) then
  begin
    // get integer light value
    _val := round(_dot * _direct.Multiplier * 255);
    if (_val > 255) then _val := 255;

    // get final color
    if (_val > 0) then Result := FinalColor(_val, _direct.Color);
  end;
end;

// ---------------------------------------------------------------------------

function lit_Omni_Point(const _point, _normal: P_Vec;
                        const _light: POmni_Point): longint;

// Returns the light color in R8G8B8 format.

var _p : T_Vec;
    _pl  : single;
    _val : longint;
    _dot : single;

begin
  Result := 0;

  // get ray and ray length
  _p  := vectSubV(_point, @_light^.Position);
  _pl := vectLen(@_p);
  _p  := vectDivS(@_p, _pl); // normalize

  // Full attenuated ?
  if (_light^.AttEnd > 0) and (_pl > _light^.AttEnd) then exit;

  // Get the angle between ray and normal, since both are normalized
  // use dot product.
  _dot := - vectDot(@_p, _normal);
  if (_dot <= 0) then exit; // light does not face polygon

  // Attenuated ?
  if (_pl > _light^.AttStart) and (_light^.AttEnd > 0) then
    begin
      _pl  := _pl - _light^.AttStart;
      _dot := _dot - (_pl * _dot) / _light^.AttRange;
    end;

  // get light value
  _val := round(_dot * _light^.Multiplier * 255);
  if (_val > 255) then _val := 255;

  // get final color
  if (_val > 0) then Result := FinalColor(_val, _light^.Color);
end;

// ---------------------------------------------------------------------------

function lit_SpotLight(const _point, _normal: P_Vec;
                       const _spot: PSpotlight): longint;

// Returns the light color in R8G8B8 format.

var _p, _v : T_Vec;
    _pl, _vl : single;
    _a : single;
    _val : longint;
    _dot : single;

begin
  Result := 0;

  // Get first vector.
  _p  := vectSubV(_point, @_spot^.Position);
  _pl := vectLen(@_p);
  _p  := vectDivS(@_p, _pl); // normalize

  // Full attenuated ?
  if (_spot^.AttEnd > 0) and (_pl > _spot^.AttEnd) then exit;

  // Get light vector.
  _v  := vectSubV(@_spot^.Target, @_spot^.Position);
  _vl := vectLen(@_v);

  // Get the angle between point and light.
  _a := arccos(vectDot(@_p, @_v) / _vl);

  // Outside spotlight cone ?
  if (_a > _spot^.Phi) then exit;

  // Get the angle between ray and normal, since both are normalized
  // use dot product.
  _dot := - vectDot(@_p, _normal);
  if (_dot <= 0) then exit; // light does not face polygon

  // Between theta and phi ?
  if (_a > _spot.Theta)
    then _a := _a - _spot^.Theta
      else _a := 0;

  // Attenuation ?
  if (_pl > _spot^.AttStart) and (_spot^.AttEnd > 0)
    then _pl := _pl - _spot^.AttStart
      else _pl := 0;

  // Update light value.
  if (_a > 0) then _dot := _dot - (_a * _dot) / _spot^.ConeRange;
  if (_pl > 0) then _dot := _dot - (_pl * _dot) / _spot^.AttRange;

  // get light value
  _val := round(_dot * _spot^.Multiplier * 255);
  if (_val > 255) then _val := 255;

  // get final color
  if (_val > 0) then Result := FinalColor(_val, _spot^.Color);
end;

// ---------------------------------------------------------------------------

initialization

InitLightsWithDefaultValues;

// ---------------------------------------------------------------------------

finalization

// ---------------------------------------------------------------------------

end.
