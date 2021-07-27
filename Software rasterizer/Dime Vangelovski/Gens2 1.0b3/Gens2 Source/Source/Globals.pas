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

{Unit for declaring the global variables used in the engine -
                                                                   - Made by Dime Vangelovski
                                                                                  29.08.2005}
Unit Globals;

Interface

Uses
     GTypes3D;

Const
      Engine_Version = 'GENS II 3-D Engine  ver 1.0  Beta 3 (Build 200)';
      Engine_Copyright = '© 2004-2005  by  Dime Vangelovski';

      // Reserved for needs of Loading 3D Studio .ASE file
      WorldFile = 'Maps\World.ase';
      Light_Corona_texture = 'Textures\LightCorona.bmp';

      // Maximum allowed screen resolution
      Max_XRes = 1280;
      Max_YRes = 960;

      // Monitor Screen Refresh Rates in Hz  !!! Should be altered with caution !!!
      Screen_Refresh_320  = 60; //75;
      Screen_Refresh_512  = 60; //75;
      Screen_Refresh_640  = 60; //85;
      Screen_Refresh_1024 = 60; //85;
      Screen_Refresh_1280 = 60; //60;

      // Constants for tweaking the affine texture distortion by adjusting the FOV parameters
      MinNearZClip : Single = 35;
      MaxNearZClip : Single = 63;
      MinFOV       : Dword  = 10;
      MaxFOV       : Dword  = 130;
      DefaultFOV   : Dword  = 89;

Type
     Ttexfile = array[1..Pol] of string;
     BMPtex = array[0..255,0..255] of Dword;

     sort = array[1..pol] of Integer;

Var
////////////////////////////////////////////////////////////////////////////////
// Texture mapping variables

    TexXres,TexYres,pol2tex:array[1..pol] of Integer;
    TexLightX,TexLightY:Integer;
    Texture_Filename:Ttexfile;
    shotheader:array[0..53] of byte;
    doublesided:array[1..pol] of boolean;
    MipMapOffset,MaterialCount,Source,Dest:Dword;
    MipMapFactor,MipMapResF:Byte;
    texture,texlight:BMPtex;
    alphap:array[1..pol] of byte;

    // Main Large Texture (detail)
    texbuf:array[1..maxtex+1,0..255,0..255] of Dword;  // 256x256

    // MipMap Textures
    miptex1:array[1..maxtex+1,0..127,0..127] of Dword; // 128x128
    miptex2:array[1..maxtex+1,0..63,0..63] of Dword;   // 64x64
    miptex3:array[1..maxtex+1,0..31,0..31] of Dword;   // 32x32
    miptex4:array[1..maxtex+1,0..15,0..15] of Dword;   // 16x16
    miptex5:array[1..maxtex+1,0..7,0..7] of Dword;     // 8x8
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables for poligon scanning and rendering

    poly,ztmp:array[0..Max_YRes,1..2] of Integer;
    tex:array[0..Max_YRes,1..4] of Integer;
    c1R,c1G,c1B,c2R,c2G,c2B,c3R,c3G,c3B,c4R,c4G,c4B:Integer;
    colR,colG,colB:array[0..Max_YRes,1..2] of Integer;
    zbuf:array[0..Max_XRes * Max_YRes] of smallint;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Trigger variables for some engine functions

    vsync,invertm,fsaa,scrhelp,animate,lensfv,showtris,lightsoff,mipmap,
    autoscrshot,lighttype,noclip,prpixel,CastShadows,LowResShadowEnable,
    m_filter,ShadowTmp,OldRendering,HeadLight,ScrShot,Engine_Initialized,
    displaypolys: Boolean;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Resolution and viewing variables

    Res_Xend, Res_Yend, Res_Xend_p1, Res_Xend_m1, Res_Yend_p1, Res_Yend_m1,
    Res_PixNum, Res_Mask, Xcenter, Ycenter, FOV: Dword;
    Fovall,FOVtan,FocusX,FocusY,NearZClip:single;
    Vid_Mode,Vid_YMove_Factor,xres,yres:Integer;
    Res_Shl1, Res_Shl2: Byte;
    VirtBuffer: Pointer;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables used in lighting

    GeneralLightIntensity,GeneralLightR,GeneralLightG,GeneralLightB,CurrentZoneLightCount:Integer;
    LightCo,Light_Scatering_R,Light_Scatering_G,Light_Scatering_B:Dword;
    LightIntensity,LightR,LightG,LightB:array[1..maxlights] of Dword;
    lightlens:string;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// New variables used for raytrace shadow optimizing

    NormalLookup:array[1..pol] of vertex;
    SrBoja_R,SrBoja_G,SrBoja_B:array[1..maxtex+1] of Byte;
    LightShadow:array[1..maxlights] of Byte;
    RecvShadowPoly,CastShadowPoly:array[1..pol] of boolean;
    PolyNoMain:array[1..Max_Sh_Cast_Pols] of Word;
    PolyNo:array[1..maxlights,1..Max_Sh_Cast_Pols] of Word;
    ShadowOccured,Is_Dynamic,Light_Disabled:array[1..maxlights] of boolean;
    MovingLightCoordinate,LightMovingFactor:array[1..maxlights] of vertex;
    PotentialShadowPoly,RayPolys,ShadowMap_LightCull,RayTrace_LightCull:array[1..maxlights] of Word;
    ShadowStart,ShadowEnd,CastCheck,CastCount,RayTrace_Lights,ShadowMap_Lights,
    ShadowTolerance,LightTolerance,RaysCasted,AmbientLightR,AmbientLightG,AmbientLightB:Integer;
    DisableLightSystem,DisableShadows:boolean;
    Surface_Reflectivity_Coeficient_R,
    Surface_Reflectivity_Coeficient_G,
    Surface_Reflectivity_Coeficient_B,lightbounce:single;
    ShadowType,RayTraceSkipFactor:Byte;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Some common and general global variables

    pa,pagehelpGens,endpageGens,scrshotno:Dword;

    cx,sx,cy,sy,cz,sz,agolx,agoly,agolz:single;

    i,ii,j,jj,tmp,ScreenPolyCount:Integer;
    polmax,litmax,ZoneCount,RotObjectsMax:integer;

    sMouseSensitivity,sKeybSensitivity:single;
    Mspeed,Kspeed,PlayerMoveSpeed,DynLightSpeed:single;
    LoadingText_Counter: Integer;
    FPS,CurrentFPS:Dword;
////////////////////////////////////////////////////////////////////////////////

Implementation

END.