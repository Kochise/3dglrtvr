                        RollerCoaster2000 by Plusplus

                              plusplus@free.fr

                    http://plusplus.free.fr/rollercoaster


Introduction
------------
RollerCoaster2000 is a rollercoaster simulator which generates the track from a
description of it stored in a text file. So it should be easy to create new
tracks (as long as you keep a pencil and a paper near your computer to draw
control points and tangents :-). For the track file format see track_format.txt

Requirements
------------
* GLUT 3.7 (for information see:
  http://www.opengl.org/developers/documentation/glut.html)
* It's also surely better with a 3D hardware accelerator with proper OpenGL
  driver.

Installation
------------
Just unzip it where you want.

Use
---
You can run RollerCoaster2000 by changing to its directory, and typing ./roller
on your console.

You can also specify some options:
-h            print this help
-f            select fullscreen mode
-s            select stereoscopic mode (for use with a stereo display device)
-c {1|5|6}    select C.A.V.E. mode(5 or 6 screens, or 1 for normal mode)
-t <filename> select a file containing a track description (default rc2k.trk)
-x <width>    select window width (default 800)
-y <height>   select window height (default 600)
-i            disables intro(skip straight to ride)

When running fullscreen, both under Linux and Windows, GLUT keeps the same
screen resolution as the desktop resolution. So specifying width and height has
no effect.

When the animation plays:
left/right arrow keys modify field of view angle
up/down arrow keys modify the focal length (useful only with stereo mode)
+/- keys modify distance between eyes (useful only with stereo mode)

When using CAVE display mode the given width and height are for a single
viewport. Since CAVE mode can display up to 6 viewports, the size of the
complete window will be 3*width by 2*height, where width and height are the x
and y resolutions specified by the -x and -y command-line options. You should
use a square resolution so that the border of adjacent viewports match. The
CAVE display mode has been tested by Rodrigo Damazio on a single-pipe Onyx
machine running IRIX, having the screens mapped with IRIX's ircombine. The
caveroller shell script should help launching RollerCoaster2000 with CAVE
display mode.

Disclaimer
----------
This software is free. You can redistribute the zip archive as long as you do
not modify it.

Use only at your own risk! Although this is highly unlikely to happen, I can't
be responsible for any damage caused by this software.

Thanks
------
Rodrigo Damazio <rdamazio@lsi.usp.br>: Implementation and test of the C.A.V.E.
display mode.

Jeff Molofee: Great OpenGL tutorials at http://nehe.gamedev.net

Jason Allen: The glBMP library included in this project is derived from the C++
  version that you can find at Jason's site http://delphigl.cfxweb.net

Paul Bourke: Explanation of how to create stereo images can be found on his
  site at http://astronomy.swin.edu.au/pbourke/stereographics/

Armin Hopp: His company http://www.digital-image.de provides with virtual
  reality systems. Helped by testing stereoscopic version.
