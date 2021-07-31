----------------------------------------------------------------------
Pulsar LMTools v1.1
Copyright (C) 2002, 2003 Lord Trancos. All rights reserved.
----------------------------------------------------------------------

Pulsar LMTools is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either
version 2 of the License, or any later version.

Pulsar LMTools is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details (license.txt).

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

You should also be able to get a copy of the license here:
  
  http://www.gnu.org/licenses/gpl.txt

----------------------------------------------------------------------
Pulsar LMTools Readme file                                  2003/02/12
----------------------------------------------------------------------

+ What is Pulsar LMTools?
  --------------------------------------------------------------------

  Pulsar LMTools is a set of tools for creating and packing lightmaps.

+ Essential Components of Pulsar LMTools
  --------------------------------------------------------------------

  LMGen   - console application for creating lightmaps.

  LMPack  - console application for packing the lightmaps generated
            by LMGen.

+ Other Components
  --------------------------------------------------------------------

  A3DS2LMTS - console application that converts Autodesk 3D Studio
              files (.3DS) to Pulsar Studio LMTS file format (the 3D
              file format used by LMGen and LMPack). Also, this tool
              is able to generate a script (.SCR) file with info about
              the lights found inside the 3DS file. (this script file
              is also needed by LMGen)

  LMDX - DirectX8 (windows Only) windowed application used as a .LMTS
         viewer and a Direct3D .X to .LMTS converter. Also, can export
         the textures to Truevision TARGA (.TGA) file format.

  LMTools.bat - Batch file that makes easier to run LMGen and LMPack.

  SCRGen.ms - 3DSMax/GMAX Script that can help you to create
              .SCR files for LMGen (you can use A3DS2LMTS instead)

+ What it's needed in order to use LMTools?
  --------------------------------------------------------------------

  To use LMTools you need;

    - Your 3D MAP in .LMTS file format.

    - A script file (.SCR) that tells LMGen where the lights are
      placed and other needed stuff.

    - The textures of your 3D MAP in Truevision TARGA (.TGA)
      file format. (this is only needed in some configurations)

  How can I convert my 3D Map to .LMTS file format ?

    - You can use A3DS2LMTS to convert a .3DS file to .LMTS.
    - You can use LMDx to convert a Direct3D .X file to .LMTS.
    - You can create your own converter (the .LMTS file format
      is described in src/_doc/ff_lmts.txt.)

  How can I write a script (.SCR) file ?

    - A3DS2LMTS can create a script file with light info from a 3DS
      file.
    - SCRGen.ms can write a script file with light info from your
      current GMAX scene.
    
    Althrough A3DS2LMTS or SCRGen can write a .SCR file with light
    information, you should read Apendix A from LMGen.txt.

----------------------------------------------------------------------
DXLab - Visit us at: http://www.dxlab.tk or http://dxlab.host.sk
        <lordtrancos@softhome.net>