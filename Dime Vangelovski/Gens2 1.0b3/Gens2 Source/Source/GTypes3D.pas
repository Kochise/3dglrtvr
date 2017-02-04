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

{Unit for declaring the 3D structures and types used in the engine -

                                                                   - Made by Dime Vangelovski
                                                                                  26.04.2005}
Unit GTypes3D;

Interface

Const
      // Main engine constants
      Pol               = 100000;   // Maximum polygons per world
      MaxVertex         = 100000;   // Maximum vertices per object
      Max_Rot_Objects   = 100;      // Maximum rotation objects
      Rot_Pol_per_Obj   = 1000;     // Maximum polygons per object
      Max_Sh_Cast_Pols  = 10000;    // Maximum shadow casting polygons per world
      maxzones          = 5000;     // Maximum number of space partitioning sphere zones
      maxlights         = 100;      // Maximum number of lights per world
      maxtex            = 300;      // Maximum textures allowed - 128 MB RAM Required !!!
      MipBias           = 0.5;      // MipMap quality biasing, to choose smaller or larger mipmaps
                                    // * larger bias = smaller mipmaps *
      Max_Shadow_Toler  = 300;      // Maximum raytracing shadow tolerance (shadow error)
                                    // larger tolerance value trades shadow accuracy for speed
      FarZClip          = 30000;    // Far Z-Clip Plane Distance
      Poly_vertex_count = 4;        // Polygon Vertex Number - DO NOT CHANGE THIS VALUE !!!

Type
     vertex = record
                 x:single;
                 y:single;
                 z:single;
              end;

     lice = record
              t1:vertex;
              t2:vertex;
              t3:vertex;
              t4:vertex;
            end;

     AlphaPols = array [1..Pol] of Word;

Var
////////////////////////////////////////////////////////////////////////////////
// Polygon objects variables

    MainPolyWorld,poligon,rotpoly,mainpoly:array[1..pol] of lice;
    Rot2World:array[1..Max_Rot_Objects,1..Rot_Pol_per_Obj] of integer;
    RotObjs2Pol:array[1..Max_Rot_Objects] of integer;
    RotObject2Center:array[1..Max_Rot_Objects] of integer;
    RotCenter:array[1..Max_Rot_Objects] of vertex;
    RotObjAngles,RotObjAnglesAdd:array[1..Max_Rot_Objects] of vertex;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Light objects variables

    LightVertex,LightVertexTmp,mainlightv:array[1..maxlights] of vertex;
    DynLightVec,DynamicLightVelocity,PlayerPos,PlayerViewAngle:vertex;
    NewDynFactor,NewPos,LightMoveSpeed:vertex;
    VectorMagnitude:single;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Space Partitioning Sphere objects variables

    Sphere_Zone_Center,Sphere_Zone_Center_Tmp,Main_Zone_Center:array[1..maxzones] of vertex;
    Sphere_Zone_Radius:array[1..maxzones] of single;
    Vo_koi_zoni_se_naogjam:array[1..maxzones] of Integer;
    In_Zona_Count:Integer;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Vector variables for Moving and the World Axis

    WorldAxisX,WorldAxisY,WorldAxisZ:Vertex;
    WorldAxisXTmp,WorldAxisYTmp,WorldAxisZTmp:Vertex;
    PlayerVectorMove,PlayerVectorStrafe,LightVectorMove:Vertex;
    PlayerMove,PlayerStrafe:Vertex;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Camera object variables

    CameraEye:Vertex;
    CameraDirectionAngle:Vertex;
////////////////////////////////////////////////////////////////////////////////

Implementation

END.