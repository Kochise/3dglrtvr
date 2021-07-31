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

uses a3ds, lmts;

const
  APPTITLE = 'A3DS2LMTS v0.1a - Copyright (c) 2003 Pulsar Studio & Lord Trancos'#10;

  USAGE = '  USAGE: A3DS2LMTS.exe base_name [-scr]'#10#10+
          '  Where base_name is the filename without extension of the file'#10+
          '  used as input. And [-scr] is an optional parameter that enables'#10+
          '  light info output to a .SCR file. Example:'#10#10+
          '    A3DS2LMTS map -scr'#10#10+
          '  Warning! Existing files will be overwritten.'#10+
          '  See A3DS2LMTS.txt for details.'#10;

  C_PARAM = '-scr';

  C_EXT_3DS  = '.3ds';
  C_EXT_LMTS = '.1.lmts';
  C_EXT_SCR  = '.scr';

  C_3DSMAT_MAXNAMELEN = 128; // 3ds material max name len

  TXT01 = 'Loading 3DS file...';
  TXT02 = #13'* 3DS file loaded successfully.';
  TXT03 = '* %d materials found.';
  TXT04 = 'Warning! material face list found for unknown material named [%s].';
  TXT05 = '* %d vertices and %d faces found.';
  TXT06 = 'Saving LMTS file...';
  TXT07 = #13'* LMTS file saved successfully.';
  TXT08 = 'Done! ;)'#10;
  TXT09 = '* SCR file saved successfully; %d lights found.';
  TXT10 = 'Warning! Texture coordinates for vertex #%d are invalid!';    

  ERR01 = 'Error loading file [%s].';
  ERR02 = 'Invalid 3DS file or file format not suported.';
  ERR03 = 'Error 3DS file version not found or not supported.';
  ERR04 = 'Not enough memory!';
  ERR05 = 'Error; cannot retrieve name for material #%d.';
  ERR06 = 'Error; cannot find enough vertex or faces.';
  ERR07 = 'Vertices and texture coordinates does not match on mesh %s.';
  ERR08 = 'Error writing file [%s].';
  ERR09 = 'Cannot create file [%s].';

  SCR01 = '; Script created automaticaly by a3ds2lmts';
  SCR17 = '; ----------------------------------------';
  SCR02 = '; light name: ';
  SCR03 = 'OMNI_POINT.';
  SCR04 = 'SPOTLIGHT.';
  SCR05 = 'ADD';
  SCR06 = 'POSITION = %f, %f, %f';
  SCR07 = 'MULTIPLIER = %f';
  SCR08 = 'ATTENUATION = %f, %f';
  SCR09 = 'COLOR = $';
  SCR10 = 'CAST_SHADOWS = true';
  SCR11 = 'ENABLED = true';
  SCR12 = 'TARGET = %f, %f, %f';
  SCR13 = 'THETA = %f';
  SCR14 = 'PHI = %f';
  SCR15 = 'AMBIENT_GLOBAL.COLOR = $';
  SCR16 = '; Ambient light ';

type
  TMaterial = record
    MatName : array[0..C_3DSMAT_MAXNAMELEN] of char;
    TextFN  : array[0..127] of char;
    TextID  : longint;
  end;

  TVertex = record
    x, y, z : single;
    u, v : single;
  end;

  TFace = record
    a, b, c : word;
    matID : word;
  end;

  TLight = record
    x, y, z : single; // pos
    mult : single;    // multiplier
    att_start, att_end : single; // attenuation
    color : cardinal;
    cast_shadows : boolean;
    enabled : boolean;
    isSpot : boolean;
    tx, ty, tz : single; // target
    theta : single; // hotspot
    phi : single; // falloff
  end;

var
  g_3DS : TA3DS_FileInMemory;
  g_3DEdit : PA3DS_Chunk = NIL;

  g_MatCount  : longint = 0;                 // # materials
  g_VtxCount  : longint = 0;                 // # vertices
  g_FaceCount : longint = 0;                 // # faces
  g_TextCount : longint = 0;                 // # of textures
  g_LitCount  : longint = 0;                 // # of lights
  g_MatList   : array of TMaterial = NIL;    // materials
  g_VtxList   : array of TVertex = NIL;      // vertices
  g_FaceList  : array of TFace = NIL;        // faces

  g_LMTS : TLMTS;

  g_SCR_Output : boolean = false; // create .SCR file
  g_SCR : text;

  g_FaceOffset : longint = 0;

// ---------------------------------------------------------------------------

implementation

// ---------------------------------------------------------------------------

end.
