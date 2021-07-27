                  RollerCoaster2000 Screen Saver by Plusplus

                              plusplus@free.fr

                    http://plusplus.free.fr/rollercoaster


Introduction
------------
RollerCoaster2000 is a rollercoaster simulator which generates the track from
a description of it stored in a text file. So it should be easy to create new
tracks (as long as you keep a pencil and a paper near your computer to draw
control points and tangents :-). For the track file format see the 
"track_format.txt" file.

Requirements
------------
* It's surely better with a 3D hardware accelerator with proper OpenGL driver.
* This screen saver has only been tested with GeForce2MX and Voodoo3 under
  Windows98SE. It should also work with other versions of Windows.

Installation
------------
To install, simply copy the "RollerCoaster2000.scr" file to the Windows folder
(usually C:\WINDOWS).

Uninstallation
--------------
Although I can't imagine that you would even think about doing that, to
uninstall, do the following:
* The saver may have created some keys and values in the registry. The ones
  you can delete (using the regedit utility) are:
  - the key HKEY_CURRENT_USER\Software\Plusplus\RollerCoaster2000ScreenSaver,
    and all its values,
  - after that, you can also remove the key
    HKEY_CURRENT_USER\Software\Plusplus, if it doesn't contain other subkeys.
* Then delete the "RollerCoaster2000.scr" file from the Windows folder.

Use
---
If you have installed the screen saver in the windows directory, you can
configure and test it from the Screen Saver section of the display settings
dialog box. Otherwise, you can test, or configure the screen saver by
right-clicking its icon, wherever it lies.

The settings you can modify are screen resolution, number of bits per pixel
and the name of the file containing rollercoaster description (See the
"track_format.txt" file for more information about this).

Disclaimer
----------
This screen saver is a free software. You can redistribute the zip archive as
long as you do not modify it.

Use only at your own risk! Although this is highly unlikely to happen, I can't
be responsible for any damage caused by this software.

Thanks
------
Jeff Molofee: Great OpenGL tutorials at http://nehe.gamedev.net

Jason Allen: This screen saver use a glBMP library which is derived from the
  C++ version that you can find at Jason's site http://delphigl.cfxweb.net

Jacob Navia: author of the free lcc-win32. You can download it from
  http://www.cs.virginia.edu/~lcc-win32
