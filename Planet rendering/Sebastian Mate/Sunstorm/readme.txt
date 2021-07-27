 ----------------------------------------------------------------------------
 SUNSTORM 3D Version 0.32
 Copyright 2003 Sebastian Mate
 ----------------------------------------------------------------------------

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


 Features
 --------
 Sunstorm is a real 3D engine written in QuickBasic. It uses fast algorithms
 to render an amazing virtual world:

 -> Mouse Free-Look
 -> Animated environment
 -> Weather
 -> Fast QuickSorting instead of Z-buffering
 -> Automatic environment reloading with an unlimited environment size
 -> Smoth textures thanks to DirectQB
 -> All 3D-calculations are done within QB
 -> Up to 100 fps on a fast machine (Athlon 1800+ XP)


 Installation
 ------------
 Requirements:
 -> A real DOS environment (DOS, Win9x ... It does NOT work in XP since
    it uses DirectQB. You can try it under XP with the /NoDQB switch, though)
 -> EMS memory manager (HIMEM.SYS and EMM383.EXE)
 -> A mouse driver

 To install the EMS-driver, add the following lines to your C:\CONFIG.SYS:
 DEVICE=C:\WINDOWS\HIMEM.SYS
 DEVICE=C:\WINDOWS\EMM386.EXE

 Change the above paths according to your DOS/Windows installation!

 To install a DOS mouse driver, I can highly recommend "CuteMouse". Download
 it at http://cutemouse.sourceforge.net and install it.

 Now just unzip all files to a folder. That's all. Run SUNSTORM.EXE.

 Additional commandline parameters:

 /NoDQB     Disables DirectQB and uses a grid-display. Use the numeric keypad
            to navigate.
 /NoTex     Disables textures, but still uses DirectQB.


 Recompilation
 -------------
 Requirements:
 -> QuickBasic 4.5
 -> FFIX Library by V1ctor (included)
 -> The latest DirectQB version (included, compiled with QB 4.5 German)

 Copy ALL files to your QB 4.5 directory. To compile it, run COMPILE.BAT.


 The Terrain Generator
 ---------------------
 There is a small program which generates the 1.3D data file. Run it. You
 can also modify the parameters in the BAS file!

 Contact
 -------
 Please feel free to contact me. I really appreciate feedback in any form ...

 sebmate@arcor.de
 http://www.sm-gimi.de
 http://www.lateinuebersetzer.de.vu





