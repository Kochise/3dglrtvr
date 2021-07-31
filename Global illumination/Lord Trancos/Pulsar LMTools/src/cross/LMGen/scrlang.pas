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

// script language definition for lmgen

unit scrlang;

// ---------------------------------------------------------------------------

interface

uses sse, vTypes, lights, global;

var
  g_ScriptLang : TScrLanguage;

function  scrLangInit: boolean;
procedure scrLangClose;

// ---------------------------------------------------------------------------

implementation

// ---------------------------------------------------------------------------

function scrLangInit: boolean;

var
  _obj : PScrObject;
  _pro : PScrProperty;

begin
  Result := false;

  // create language
  g_ScriptLang := scr_NewLanguage(';');

  // add objects, properties|methos, and value types

  _obj := scr_AddObject(g_ScriptLang, 'LIGHTMAP');
  if _obj = NIL then exit;

    _pro := scr_AddProperty(_obj, 'MAX_SIZE', NIL);
    if _pro = NIL then exit;

    if scr_AddValue(_pro, scrVT_UINT32, @g_LMap_Max_Size) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'LUMEL_SIZE', NIL);
    if _pro = NIL then exit;

    if scr_AddValue(_pro, scrVT_Single, @g_LMap_Lumel_Size) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'DISABLE_SUBSET', lm_Disable_Subset);
    if _pro = NIL then exit;

    if scr_AddValue(_pro, scrVT_UINT32, @g_TempInt32) = NIL then exit;

  _obj := scr_AddObject(g_ScriptLang, 'COLLISION');
  if _obj = NIL then exit;

    _pro := scr_AddProperty(_obj, 'SAFE_DISTANCE', NIL);
    if _pro = NIL then exit;

    if scr_AddValue(_pro, scrVT_Single, @g_Collision_Dist) = NIL then exit;

  _obj := scr_AddObject(g_ScriptLang, 'OCTREE');
  if _obj = NIL then exit;

    _pro := scr_AddProperty(_obj, 'MIN_TRIS', NIL);
    if _pro = NIL then exit;

    if scr_AddValue(_pro, scrVT_UINT32, @g_Oct_MinNodeTri) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'MIN_SIZE', NIL);
    if _pro = NIL then exit;

    if scr_AddValue(_pro, scrVT_Single, @g_Oct_MinNodeSize) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'MAX_LEVELS', NIL);
    if _pro = NIL then exit;

    if scr_AddValue(_pro, scrVT_UINT32, @g_Oct_MaxLevels) = NIL then exit;

  _obj := scr_AddObject(g_ScriptLang, 'AMBIENT_GLOBAL');
  if _obj = NIL then exit;

    _pro := scr_AddProperty(_obj, 'COLOR', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_UINT32, @gAmbient_Global.Color) = NIL then exit;

  _obj := scr_AddObject(g_ScriptLang, 'DIRECTIONAL');
  if _obj = NIL then exit;

    _pro := scr_AddProperty(_obj, 'DIRECTION', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gDirectLight.Direction[X]) = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gDirectLight.Direction[Y]) = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gDirectLight.Direction[Z]) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'MULTIPLIER', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gDirectLight.Multiplier) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'COLOR', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_UINT32, @gDirectLight.Color) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'CAST_SHADOWS', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Bool, @gDirectLight.CastShadows) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'ENABLED', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Bool, @gDirectLight.Enabled) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'ADD', lit_Add_DirectLight);
    if _pro = NIL then exit;

  _obj := scr_AddObject(g_ScriptLang, 'OMNI_POINT');
  if _obj = NIL then exit;

    _pro := scr_AddProperty(_obj, 'POSITION', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gOmni_Point.Position[X]) = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gOmni_Point.Position[Y]) = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gOmni_Point.Position[Z]) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'MULTIPLIER', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gOmni_Point.Multiplier) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'ATTENUATION', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gOmni_Point.AttStart) = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gOmni_Point.AttEnd) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'COLOR', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_UINT32, @gOmni_Point.Color) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'CAST_SHADOWS', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Bool, @gOmni_Point.CastShadows) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'ENABLED', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Bool, @gOmni_Point.Enabled) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'ADD', lit_Add_Omni_Point);
    if _pro = NIL then exit;

  _obj := scr_AddObject(g_ScriptLang, 'SPOTLIGHT');
  if _obj = NIL then exit;

    _pro := scr_AddProperty(_obj, 'POSITION', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gSpotLight.Position[X]) = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gSpotLight.Position[Y]) = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gSpotLight.Position[Z]) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'TARGET', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gSpotLight.Target[X]) = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gSpotLight.Target[Y]) = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gSpotLight.Target[Z]) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'THETA', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gSpotLight.Theta) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'PHI', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gSpotLight.Phi) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'MULTIPLIER', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gOmni_Point.Multiplier) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'ATTENUATION', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gSpotLight.AttStart) = NIL then exit;
    if scr_AddValue(_pro, scrVT_Single, @gSpotLight.AttEnd) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'COLOR', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_UINT32, @gSpotLight.Color) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'CAST_SHADOWS', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Bool, @gSpotLight.CastShadows) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'ENABLED', NIL);
    if _pro = NIL then exit;
    if scr_AddValue(_pro, scrVT_Bool, @gSpotLight.Enabled) = NIL then exit;

    _pro := scr_AddProperty(_obj, 'ADD', lit_Add_SpotLight);
    if _pro = NIL then exit;

  // all right
  Result := true;
end;

// ---------------------------------------------------------------------------

procedure scrLangClose;

begin
  scr_FreeLanguage(g_ScriptLang);
end;

// ---------------------------------------------------------------------------

end.
