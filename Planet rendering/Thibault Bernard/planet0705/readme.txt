
Hardware & software requirements
--------------------------------

You need a DX9 graphic card (GeForceFX, Radeon9500) to run "planet engine".
Driver version recommended:
  - Detonator 5x.xx for NVIDIA users
  - Catalyst 3.8 or better for ATI users


Keys
----

ESC: 			quit program
g:			grap/release mouse
up arrow: 		move forward
down arrow: 	move backward
left arrow: 	move to the left
right arrow: 	move to the right

o:			toggle on-screen display
w or z:		toggle wireframe
t:			switch mipmapping (no mipmapping / nearest / linear)
f:			freeze/unfreeze tesselation routine
s:			save current position (file: "saved.cfg")
r:			toggle front-to-back rendering

F1-F6:		set speed (from 1 km/s to 10,000 km/s)
F7:			auto-speed

Numpad 7 & 9:	rotate planet


Mouse
-----
left click:		grab/release mouse
right click + move:	rotate around z axis
move mouse:		rotate point of view


Configuration file "planet engine.cfg"
--------------------------------------

POSITION
./positions/saved.cfg
# path of the position file to load.

VIDEOMODE
1 0
# select videomode
# 1st parameter: 0 = 640x480, 1=800x600, 2=1024x768, 3=1280x1024, 4=1600x1200
# 2nd paramter:  0 = no fullscreen, other values = fullscreen

FORCEARB
# If this command is present, planet engine will use the standard ARB shader
# instead of the NVIDIA proprietary shader.
# This command has no effect on Radeon.


Contact
-------
web: drtypo.free.fr
mail: dr.typo@free.fr


Credits
-------

Main code: Thibault "DrTypo" Bernard
Fractal noise: Ken Perlin
Quaternion routines: Nick Bobic
Merge sort routine from "Exposure C++" by Leon Schram
Normal map generation: NVIDIA

Radeon beta-test: Joker

Textures: (c) www.space-graphics.com

planet engine uses Simple DirectMedia Layer
http://www.libsdl.org/index.php


Disclaimer
----------

This program may destroy your hard-drive, give you dreadful diseases
or start a global nuclear war.
Use at your own risks.

This program doesn't like crappy video cards.

This program is freeware. If you want the sourcecode, please send me
an e-mail.
