
                    Earth Screen Saver for Windows
                                                
                      Version 3.1 Release Notes
                           by John Walker
                       http://www.fourmilab.ch/
                                    
INTRODUCTION
============

The Earth screen saver displays an image of the Earth as currently
illuminated by the Sun, from a variety of viewpoints.  You can view
the Earth from the Sun (day side), the night side, from the Moon, or
from an arbitrary altitude above any point on the globe specified by
latitude and longitude.  Day and night regions of the globe are shown
based on the current date and time.  The image of the Earth shifts
location on the screen every 10 minutes to avoid burning in the
phosphor in one location.

The Earth Screen Saver is in the public domain.  You can do anything
you like with it.  The images are generated based on imagery prepared
by the NASA Goddard Space Flight Center from the MODIS instrument onboard
the Terra satellite.  The image of the Earth by night is based on
nine months of observation by the Defense Meterological Satellite
Program.  For further information and additional images, visit the
NASA Earth Observatory site:
    http://earthobservatory.nasa.gov/
    
If you obtained this file from a site other than www.fourmilab.ch, you
may want to check:
    http://www.fourmilab.ch/earthscr/
to see if there has been an update subsequent to this release.

INSTALLATION
============

The Earth Screen Saver works only on 32-bit Windows systems such as
Windows 95 and Windows NT; it cannot be used on Windows 3.x.  To
install the Earth Screen Saver, download the archive EARTHSCR.ZIP and
extract the files from it with PKUNZIP.  You will obtain:

    EARTH.SCR       - Screen saver module
    README.TXT      - This document
                
Copy EARTH.SCR to your Windows directory (usually C:\WINDOWS), then
from the Desktop option of the Control Panel, choose "Earth" as your
screen saver and use "Settings.." to select the view you prefer.

CONFIGURATION
=============

The "Settings..." button in the Control Panel/Display/Screen Saver
panel displays a dialogue which allows you to select the view of the
Earth and the size of the image.  The image size defaults to "Auto",
which fills 2/3 of your screen, but you can enter any size from 64
pixels to the entire height of your monitor.

You can view the Earth from the following vantage points:

    Sun (day side)
        This is the default.  The illuminated hemisphere of the Earth
        is shown, rotating as the day progresses.

    Moon
        The Earth is shown as it currently appears from the Moon.  The
        phase of the Earth is inverse to that of the Moon: at new Moon
        the Earth is full and vice versa.

    Night side
        The night hemisphere is shown.  This can be handy for
        shortwave listeners interested in bands on which propagation
        is best at night.

    Above location
        The Earth is viewed from the given latitude, longitude, and
        altitude.  The default altitude is that of geosynchronous
        Earth satellites.  The finite resolution of the map image
        limits the detail you can see by entering low altitudes.
        Enter the latitude and longitude of the location you want to
        look down upon, in degrees, minutes, and seconds in the boxes,
        and don't forget to click the buttons to specify whether the
        latitude is North or South and the longitude East or West.
        Don't worry about getting the longitude and latitude
        absolutely precise--a couple of minutes of error don't make
        much difference in the appearance of the earth rendered at
        this scale.  If you enter your own latitude and longitude,
        you'll see the Earth centred upon your location and be able
        to watch the progression of day and night.


HOME PLANET
===========

The Earth Screen Saver was developed based on Home Planet, a
comprehensive Earth and sky simulator for Windows which displays the
Earth, tracks satellites, asteroids, and comets, includes an
extensible multimedia object catalogue, a simulated telescope for
viewing the sky at any magnification or location, a database of more
than a quarter million stars, and a complete hypertext help file and
introduction to astronomy linked to the components of the program.
Displays include the illuminated portion of the Earth, the Sky, the
Telescope, the Earth as viewed from a satellite, the Moon, or the Sun,
an orrery, panels displaying current information about the Moon and
planets, and more.  Real-time astronomical information can be exported
to other applications via DDE.  There's even a cuckoo clock (you can
turn it off).  Home Planet is in the public domain.

For more information about Home Planet, visit the World-Wide Web page:

    http://www.fourmilab.ch/homeplanet/
    
UPDATE LOG
==========

Version 2.0 (19 Apr 1996): Initial release.  The first release is
numbered 2.0 to keep it in sync with the related Sky screen saver.

Version 2.0a (28 Apr 1996): Workaround for inane and undocumented
misbehaviour in the Microsoft screen saver library.  See the development
log in the source release for the ugly details.  This fixes the problem
where the image would cease to be updated but the caption would continue
to move around on the screen, cluttering it up.

Version 3.0 (12 Aug 2002): Integrated "Scream Saver" library for easier
testing.  Fixed anachronisms in code dating from 16-bit days.  Changed
year format to display full four digit year number.  Replaced topographic
map with NASA cloudless Earth image.  Fixed screen saver logic to be
restartable for debugging under Scream Saver (this isn't necessary when
run as an actual system screen saver).

Version 3.1 (21 Jul 2006): Ported to Visual Studio .NET and linked with
its libraries in order to support dual-monitor configurations on Windows
XP and later.  Settings are now stored in the registry instead of the
CONTROL.INI file, which allows settings to be saved for non-administrator
users.

