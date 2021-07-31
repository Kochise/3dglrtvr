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

unit global;

// ---------------------------------------------------------------------------

interface

uses vTypes, ARGB, LMTS, octree;

const
  APPTITLE = 'LMGen v0.2b - Copyright (c) 2002-2003 Pulsar Studio & Lord Trancos'#10;
  USAGE = '  USAGE: LMGen.exe base_name [quality settings]'#10#10+
          '  Where base_name is the filename without extension of the files'#10+
          '  used as input. Quality settings can be the following (type they together):'#10+#10+
          '    A - Enable alpha channel.'#10+
          '    F - Disable lightmap filtering.'#10+
          '    H - Enable hight quality shadows (oversample).'#10+
//          '    L - Disable log file (only logs warning messages).'#10+
          '    S - Disable shadows.'#10#10+
          '  Example: LMGen.exe stage1 saf'#10#10+
          '  See LMGen.txt for details.'#10;

  TXT01 = '* Script language initialized.';
  TXT02 = '* Script file successfully processed.';
  TXT03 = '* 3D Data successfully loaded (%d tris in %d subsets).';
  TXT04 = #13'Creating lightmaps... %d%%   ';
  TXT05 = #13'* Lightmaps created successfully.';
  TXT06 = 'Done! ;)'#10;
  TXT07 = #10'- Warning! triangle #%d too large.';
  TXT08 = '* 2D Data successfully loaded (%d textures).';
  TXT09 = 'Creating octree... (plz wait)';
  TXT10 = #13'* Octree successfully created.';
  TXT11 = #10'- Warning! cannot create a lightmap for triangle #%d.'#10+
             '  Maybe triangle shape it''s a line or a point.';
  TXT12 = '- Warning! Lumel size (lightmap.lumel_size) not specified in the script file.'#10+
          '  A default value of 8 will be used. However, each map needs a specific'#10+
          '  value based on map''s dimensions. You should provide a lumel size, please'#10+
          '  read lmgen.txt for details about how to calculate an appropriate lumel'#10+
          '  size value for your map.'#10+
          '  Using a non appropriate value may cause the generation of too many'#10+
          '  or too few lightmaps.';
  TXT13 = 'Calculating some triangle data... (plz wait)';
  TXT14 = #13'* Triangle data calculated.                 ';

  ERR01 = 'Error initializing script language.';
  ERR02 = 'Error running script file.';
  ERR03 = 'File [%s] not found!';
  ERR04 = 'Error processing script on line %d.'#10'Description: %s';
  ERR05 = 'Error loading file [%s].';
  ERR06 = 'ERROR: Not enough memory.';
  ERR07 = 'Error writing file [%s].';
  ERR08 = #13'Error while creating Octree! ';
  ERR09 = 'ERROR: Unkown quality setting: ';
  ERR10 = 'ERROR: cannot open/create log file.';

  C_EXT_SCRIPT = '.scr';    // script file name extension
  C_EXT_3DFILE = '.lmts';   // 3d file
  C_EXT_LMFILE = '.lmdb';   // lightmap database
  C_EXT_LMLOG  = '.lm.log'; // log file

  C_PARAMMSG00 = '>> Alpha channel ENABLED.';
  C_PARAMMSG01 = '>> Lightmap filtering DISABLED.';
  C_PARAMMSG02 = '>> Hight quality shadows ENABLED.';
  C_PARAMMSG03 = '>> Shadows DISABLED.';
  C_PARAMMSG04 = '>> Log file DISABLED.';

  // default quality settings
  C_DEFAULT_CAST_SHADOWS = true;
  C_DEFAULT_USE_ALPHA    = false;
  C_DEFAULT_SOFT_SHADOWS = false;
  C_DEFAULT_FILTER_LMAPS = true;
  C_DEFAULT_LOG_WARNINGS = true;

  // default lightmap settings
  C_DEFAULT_LMAP_LUMEL_SIZE = 8;
  C_DEFAULT_LMAP_MAX_SIZE = 256;

  // default collision detection settings
  C_DEFAULT_COLL_DIST = 1.0;

  // default octree settings
  C_DEFAULT_OCT_MINNODETRI  = 25;
  C_DEFAULT_OCT_MINNODESIZE = 0;
  C_DEFAULT_OCT_MAXLEVELS   = 6;

type
  // misc triangle data
  TTriData = record
    a, b, c : PLMTS_Vertex; // triangle
    e0, e1 : T_Vec; // edges
    d : single; // used for plane's equation
    n : T_Vec; // normal
    uv00, uvE1, uvE2 : T_Vec; // planar mapping
    dU, dV : single; // lightmap size in world units
    subsetID, textID : word; // subset and texture IDs
    lastRayID : cardinal; // last ray that checked this triangle (shadow cast stuff)
    valid : boolean; // triangle it's valid for lightmapping
                     // (it's shape isn't a point or a line)
  end;

var
  // quality settings
  g_Cast_Shadows      : boolean = C_DEFAULT_CAST_SHADOWS; // s
  g_Use_Alpha_Channel : boolean = C_DEFAULT_USE_ALPHA;    // a
  g_Soft_Shadows      : boolean = C_DEFAULT_SOFT_SHADOWS; // h
  g_Filter_LightMaps  : boolean = C_DEFAULT_FILTER_LMAPS; // f
  g_Log_Warnings      : boolean = C_DEFAULT_LOG_WARNINGS; // l

  // lightmap settings
  g_LMap_Lumel_Size : single  = -1; // default value will be used later
  g_LMap_Max_Size   : longint = C_DEFAULT_LMAP_MAX_SIZE;

  // collision detection settings
  g_Collision_Dist  : single = -1; // default value will be calculated

  // octree settings
  g_Oct_MinNodeTri  : longint = C_DEFAULT_OCT_MINNODETRI;
  g_Oct_MinNodeSize : single  = C_DEFAULT_OCT_MINNODESIZE;
  g_Oct_MaxLevels   : longint = C_DEFAULT_OCT_MAXLEVELS;

  // -----
  g_FileNameBase : string = '';
  g_DestFN : string = '';
  g_LMTS : TLMTS;
  g_LMDB : file;
  g_LMDB_FN : string;
  g_LastLM : record
    lm      : TARGB;
    uv00    : T_Vec;
    uvEdge1 : T_Vec;
    uvEdge2 : T_Vec;
  end;

  g_Octree : TOctree;
  g_MaxRayLength : single;
  g_MeshTextures : array of TARGB = NIL;
  g_DisabledSubsets : array of boolean = NIL;
  g_TempARGB  : TARGB;
  g_TempInt32 : longint;

  g_TriData : array of TTriData = NIL; // misc triangle data
  g_RayID : cardinal = 0; // used in shadow casting to avoid checking
                          // ray/triangle collision more than one time.

  // log file
  g_LogFile_IsOpen : boolean = false;
  g_LogFile : Text;

procedure GetTri(const _triIdx: longint;
                  var _a, _b, _c: P_Vec);
procedure lm_Disable_Subset; stdcall;

// ---------------------------------------------------------------------------

implementation

// ---------------------------------------------------------------------------

procedure GetTri(const _triIdx: longint;
                  var _a, _b, _c: P_Vec);

var
  _cur : pointer;

begin
  _cur := g_LMTS.tris;
  inc(longint(_cur), g_LMTS.header.vtxSize * _triIdx * 3);
  _a := _cur;
  inc(longint(_cur), g_LMTS.header.vtxSize);
  _b := _cur;
  inc(longint(_cur), g_LMTS.header.vtxSize);
  _c := _cur;
end;

// ---------------------------------------------------------------------------

procedure lm_Disable_Subset; stdcall;

begin
  // ingnore invalid subsets
  if (g_TempInt32 < 0) or (g_TempInt32 >= g_LMTS.header.nSubsets) then exit;

  // disable this subset
  g_DisabledSubsets[g_TempInt32] := true;
end;

// ---------------------------------------------------------------------------

end.
