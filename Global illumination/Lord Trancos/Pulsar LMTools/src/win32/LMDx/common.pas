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

unit common;

// ------------------------------------------------------------------

interface

uses Windows, SysUtils, DirectXGraphics, DX8XMESH, timing,
     dx8lmts, xcomdlg;

const
  APPTITLE = 'LMDx v1.1b';

  // supported FVFs for exporting
  C_VALID_FVF1 = D3DFVF_XYZ or D3DFVF_TEX1;
  C_VALID_FVF2 = D3DFVF_XYZ or D3DFVF_NORMAL or D3DFVF_TEX1;

  // display settings
  C_DISPLAY_FULLSCREEN = false;
  C_DISPLAY_WIDTH   = 800;
  C_DISPLAY_HEIGHT  = 600;
  C_DISPLAY_BPP     = 16;
  C_DISPLAY_BBCOUNT = 1;    // backbuffer count

  C_MAX_TEXTURES = 2048;
  C_DEF_SPEED = 50; // Default speed
  C_SPEED_SF  = 0.001;
  C_MAXYROT =  Pi/3; // Max camera
  C_MINYROT = -Pi/3; // rotation.

  C_EXPORT_EXT3D = '.1.LMTS';
  C_EXPORT_EXT2D = '.TGA';

  C_FLT_FILTER = 'All supported files.'#0'*.x;*.lmts'#0+
                 'Lightmapped Triangle Soup files (.LMTS)'#0'*.lmts'#0+
                 'Direct X Model files (.X)'#0'*.x'#0#0;

  ABOUT = APPTITLE+#10+
          'Copyright (C) 2002, 2003 Lord Trancos. All rights reserved.'#10#10+
          'LMDx is part of Pulsar LMTools.'#10#10+
          'Pulsar LMTools is free software; you can redistribute it and/or'#10+
          'modify it under the terms of the GNU General Public License'#10+
          'as published by the Free Software Foundation; either'#10+
          'version 2 of the License, or any later version.'#10#10+
          'Pulsar LMTools is distributed in the hope that it will be useful,'#10+
          'but WITHOUT ANY WARRANTY; without even the implied warranty of'#10+
          'MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.'#10+
          'See the GNU General Public License for more details (license.txt).'#10#10+
          'You should have received a copy of the GNU General Public License'#10+
          'along with this program; if not, write to the Free Software'#10+
          'Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.'#10;

  NFO00 = 'Current Mesh have %d Faces, %d Vertices and %d Subsets.';
  NFO01 = ' Speed: %d  (Use "+" to increase or "-" to decrease speed.)';
  NFO02 = 'Loading...';
  NFO03 = 'Writing...';
  NFO04 = 'Export finished.';
  NFO05 = 'FOV: %.3f';
  NFO06 = 'Near: %.3f, Far: %f';
  NFO07 = 'Fog is OFF.';
  NFO08 = 'Fog Density: %.5f';

  WRNXX = 'Warning';
  WRN01 = 'Error loading mesh or associated textures.';
  WRN02 = 'Could not load texture file [%s].'#10'Press [Cancel] to abort loading.';

  ERRXX = 'Error';
  ERR00 = 'Could not initialize DirectX Graphics.'#10+
          'Try with another device type.'#10#10+
          '(You must have installed DirectX 8 or better '#10+
          'and a 16, 24 or 32 bpp desktop to run this program.)';
  ERR01 = 'Error initializing texture engine.';
  ERR02 = 'File not found!';
  ERR03 = 'Error initializing DirectInput.'#10+
          'You must have installed DirectX 8 or better.';
  ERR04 = 'Error initializing mouse.';
  ERR05 = 'FVF not supported for exporting.';
  ERR06 = 'Error converting to non indexed mesh.';
  ERR07 = 'Error while writing file: ';
  ERR08 = 'Error while loading file: ';

  C_CAM_FOV_MIN = PI/6;
  C_CAM_FOV_MAX = PI/1.5;
  C_CAM_FOV_SCALE = 0.001;

  C_CAM_FAR_VMAX = 250;
  C_CAM_FAR_VMIN = 2;
  C_CAM_FAR_VSCALE = 0.05;
  C_CAM_NEAR_DIV = 2000;

  C_BACKGROUND_DEFAULT_COLOR = $FF000080;

  C_FOG_MIN_DENSITY = 0.000001;
  C_FOG_MAX_DENSITY = 0.1;
  C_FOG_DEFAULT_DENSITY = C_FOG_MIN_DENSITY;
  C_FOG_SCALE = 0.00001;

var
  g_DXReady      : boolean = false;
  g_WindowReady  : boolean = false;

  g_Mesh0 : TMeshSource;
  g_Mesh1 : TDXLMTS;

  g_RenderMode   : TDXLMTS_RenderMode;
  g_DeviceType   : longint = 0;
  g_OpenDlgPath  : string = '';
  g_MeshFileName : string = '';

  g_BackgrndColor : cardinal = C_BACKGROUND_DEFAULT_COLOR; // background color
  g_CustomColors  : TCustomColors; // for color dialog box

  // Camera.
  g_CEnabled : boolean = false;
  g_CSpeed   : longint = C_DEF_SPEED;
  g_XRot : single;
  g_YRot : single;
  g_CTarget   : TD3DVector;
  g_CPosition : TD3DVector;

  // fps related
  g_FPS_FRMTIM_L : longint = 0;
  g_FPS_TIME_FLAG  : int64 = 0;
  g_FPS_FRAMETIME  : int64 = 0;
  g_FPS_FRAMETIMES : int64 = 0;
  g_FPS_FRAMECOUNT : dWord = 0;
  g_FPS_LASTFPSVAL : single = 0;

  // camera
  g_CAM_Fov  : single = PI/2;
  g_CAM_FarV : single = 100;
  g_CAM_Far  : single;
  g_CAM_Near : single;

  // fog
  g_FOG_Density : single = C_FOG_DEFAULT_DENSITY;

function SwapRedAndBlue(_c: cardinal): cardinal;

// ------------------------------------------------------------------

implementation

// ------------------------------------------------------------------

function SwapRedAndBlue(_c: cardinal): cardinal;

begin
  Result := ($FF000000) or (_c and $FF00) or ((_c and $FF) shl 16) or
            ((_c shr 16) and $FF);
end;

// ------------------------------------------------------------------

end.
