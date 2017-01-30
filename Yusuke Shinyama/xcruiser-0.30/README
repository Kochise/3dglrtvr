README for XCruiser (version 0.3)
$Id: README,v 1.1.1.1.2.2 2003/01/26 18:29:18 euske Exp $

  XCruiser - a filesystem visualization utility
  Copyright (C) 1999-2003  Yusuke Shinyama <yusuke at cs . nyu . edu>


WHAT IS IT?

XCruiser (formerly known as XCruise) is a filesystem visualization
utility which compares a filesystem to a 3D-formed universe and
allows you to "cruise" within it. It constructs a universe from
directory trees, and you can navigate with a mouse.

Here are simple analogies. Every file in a filesystem is shown as
"a planet" with a solid circle. Similarly directories are
"galaxies" (hollow rings) and symbolic links are "wormholes"
(green curves).  Unlike the real universe, a directory can contain
subdirectories inside, which forms a hierarchical filesystem. The
radius of each star is determined by its mass (i.e. filesize) and
the position is determined by its name. Closer names are placed
closer to each other. Shorter filenames are placed closer to the
center of the galaxy.


OPERATIONS

You have one main window. Planets are shown as solid circles in
warm colors. Galaxies are cyan or white, and wormholes are
green. The program also displays the current velocity and the name
of current galaxy where your ship is in. The current direction is
indicated in revolving green meters around the center of the
screen. You can move the white cross cursor with a
mouse. Unreadable files or directories appear in magenta.

There're two types of flying mode, which appears at the next to
the velocity at the top left of the screen as a letter "P" (Polar)
and "C" (Cartesian).  In polar flying mode, you can change your
direction with a mouse and drive forward with the left button. In
Cartesian flying mode in contrast, your ship moves in parallel
without changing the direction. When you reach close enough to a
galaxy, the ship "enters" the galaxy and slows down.  Outer stars
are displayed in darker colors.

Default controls:

  Left button:   Accelerate forward.
  Middle button: Open the file (incompletely implemented).
  Right button:  Accelerate backward.
  Z:             Reposition the ship.
  X:             Change the flying mode.
  V:             View/hide the information.
  M:             View/hide the direction indicators.
  Return or O:   Open the file.
  F:             Freeze the ship.
  Q:             Quit the program.
  Space:         Accelerate forward.
  Cursor keys:   Move the cursor.
     (You need to turn off auto key repeating when you're using keyboards.)


OPTIONS

  -display dpy		Specifies an X server to connect.
  -background color	Background color.
  -geometry geom	Window geometry.

Other options are configurable as X resources.
See XCruiser.ad for details.


A DOCTOR'S WARNING

Please take care of motion sickness. (I'm not joking!)


HISTORY

I developed XCruiser (XCruise) when I was a junior student in
university.  At that time I was inspired by the idea by a certain
professor that tree-like filesystems are not necessarily
appropriate to us, since we sometime remember objects with spatial
hints. First I developed this on my Macintosh SE/30, and then
ported to X11 with a monochrome terminal. Hope that this program
give some idea to those who're developing user interfaces.


LICENSE

XCruiser comes with ABSOLUTELY NO WARRANTY.
This program is distributed under the GNU General Public License.


AUTHOR

Yusuke Shinyama
Contact: yusuke at cs . nyu . edu
http://www.unixuser.org/~euske/
