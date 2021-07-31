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

uses lmts, argb;

const
  APPTITLE = 'LMPack v0.2b - Copyright (c) 2002-2003 Pulsar Studio & Lord Trancos'#10;
  USAGE = '  USAGE: LMPack.exe base_name [quality settings]'#10#10+
          '  Where base_name is the filename without extension of the files'#10+
          '  used as input. Quality settings can be the following (type they together):'#10+#10+
          '    B - Output 24 bpp Windows BMP files.'#10+
          '    L - Output 16 bpp TGA files.'#10+
          '    R - Disable lightmap rotation.'#10#10+
          '  Default output files are 32 bpp Truevision TGA files.'#10#10+
          '  Example: LMPack.exe stage1 lr'#10#10+
          '  See LMPack.txt for details.'#10;

  TXT01 = #13'* Source lightmaps successfully loaded.';
  TXT02 = #13'Loading lightmaps... %d%%   ';
  TXT03 = '* 3D Data successfully loaded (%d tris in %d subsets).';
  TXT04 = '* Lightmaps optimized: %d/%d (%.2f%%).';
  TXT05 = '* Lightmaps shared: %d/%d (%.2f%%).';
  TXT06 = '* Destination lightmaps created: %d lightmap(s).';
  TXT07 = #13'Saving lightmaps... %d%%   ';
  TXT08 = #13'* Destination lightmaps successfully saved.';
  TXT09 = 'Done! ;)'#10;

  ERR04 = 'Error writing file [%s].';
  ERR05 = 'Error loading file [%s].';
  ERR06 = 'ERROR: Not enough memory.';
  ERR07 = 'Error creating subset list (3D file corrupted?).';
  ERR08 = 'Error while sorting vertex buffer.';
  ERR09 = 'ERROR: file [%s] is not valid.';
  ERR10 = 'ERROR: Unkown quality setting: ';

  C_EXT_3DFILE = '.lmts'; // 3d file
  C_EXT_LMFILE = '.lmdb'; // source lightmaps

  // output file format
  C_OUTPUT_32BPP_TGA = 0;
  C_OUTPUT_16BPP_TGA = 1;
  C_OUTPUT_24BPP_BMP = 2;

  C_PARAMMSG02 = '>> Output set to 24 bpp Windows BMP files.';
  C_PARAMMSG00 = '>> Output set to 16 bpp TGA files.';
  C_PARAMMSG01 = '>> Disabled lightmap rotation.';

  C_DEFAULT_LMROTATION = true;

type
  PLMPackNode = ^TLMPackNode;
  TLMPackNode = record
    lmidx : longint;
    child : array[0..1] of PLMPackNode;
    next  : PLMPackNode;
    x, y, w, h : longint;
  end;

type
  TLMTriData = record
    subID  : longint;
    treeID : longint;
  end;

var
  g_FileNameBase : string = '';
  g_LMTS         : TLMTS;
  g_SrcLightmaps : array of TARGB = NIL;
  g_DstLightmaps : array of TARGB = NIL;
  g_SharedWith   : array of longint = NIL;
  g_TriSubsetID  : array of longint = NIL;
  g_WasRotated   : array of boolean = NIL;
  g_LMPackTrees  : array of record
                     node : TLMPackNode;
                     prev : PLMPackNode;
                   end;

  g_LMap_Max_Size : longint = 0;

  g_Output_Format : byte = C_OUTPUT_32BPP_TGA;

  g_Allow_LMRotation : boolean = C_DEFAULT_LMROTATION;

  g_New_nTexts   : cardinal;
  g_New_nSubsets : cardinal;
  g_New_TexData  : pointer;
  g_New_Subsets  : pointer;
  g_New_Tris     : pointer;

// ---------------------------------------------------------------------------

implementation

// ---------------------------------------------------------------------------

end.
