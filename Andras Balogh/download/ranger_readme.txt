Ranger Mk. II. experimental terrain rendering engine
-----------------------------------------------------------------------
1. Overview:

This technology demo renders vast landscapes with an incredible amount
of detail in real-time. The accurate per pixel shading of the terrain
ensures that you can see every little feature of the surface, even at
extreme distances!

-----------------------------------------------------------------------
2. Installation:

You can find the engine and related material at:
http://web.interware.hu/bandi/ranger.html

After downloading and unzipping the package, you will find two batch
files in the top directory. To start the default demo, simply run
"ranger.bat"! It will load the Puget Sound dataset and a flythrough
animation. However, if you would like to see the engine with
different settings, then try running "noise_land.bat". This will
create a fractal terrain with different lighting conditions. There
is no animation for this dataset however, you have to explore it on
your own!

Important! As of now, this demo will run at full quality only on
GeForce3 or better nVidia GPUs. ATi Technologies and other vendors are
not supported at the moment. This means that on those cards the
renderer will default to wireframe mode. The reason for this is that I
have to use some advanced functions that are only available through
vendor specific extensions. Since my computer is equipped with a
GeForce4, I have developed the rendering path for nVidia GPUs. There
are also standard (ie. not vendor specific) methods for doing the
required calculations (using the fragment program or the GLSL) but
that functionality is only supported by the newest crop of graphics
accelerators (GeForce FX, Radeon9500 and above).

Even in compatibility mode, the demo assumes decent OpenGL support.
Since this is a research project, there are no fallback mechanisms. If
the required functionality (like the VBO) is not supported by the
driver, the demo will quit immediately.

Although the demo might run with some older drivers, for optimal
performance it is strongly recommended to run this program with the
latest video dirvers installed! The reason is that the engine utilizes
some recent technologies, which might not be fully optimized in older
drivers (if supported at all). I have tested this demo with the WHQL
approved Detonator 45.23 drivers. My development/test machine is an
AthlonXP 1800+ with 512MB RAM running Windows XP. Hardware graphics
acceleration is provided by a GeForce4 Ti4200. Performance depends on
a lot of things. With the default settings, running the demo at
1152x864 gives between 40 and 70+ FPS, depending on scene complexity.
One notable fact is that with these settings the demo is always
fillrate limited, meaning that the videocard is the bottleneck, not
the CPU!

Please don't forget that this is experimental software and although
I've done my best to make this demo as robust as possible, I can't
guarantee that it will work as expected. I had very limited
possibility for testing (mainly my own computer). Of course, if you
find that something does not work, you should let me know and I'll see
if I can do something about it!

-----------------------------------------------------------------------
3. Usage instructions:

Move the mouse to look around. Pressing the right mouse button moves
forward, while the middle button moves backward. Clicking the left
button toggles between flying/walking.

Keyboard commands:

w/s - move forward/backward
a/d - strafe left/right
r/f - move up/down
q/e - increase/decrease speed
t/g - adjust view camera FOV (Field Of Vision)
y/h - adjust refinement camera FOV
1 - first-person camera
2 - overhead camera
3 - map view
4 - chasecam
z - cycle through multiple renderers
x - toggle walk/fly
c - toggle frustum rendering
~ - pull down the console
[/] - adjust projected error threshold
;/' - adjust amount of visible detail geometry
F7/F8 - adjust simulation frequency
F9/F10 - adjust time multiplier
F11 - toggle vertical sync on/off
TAB - toggle overlay display
CTRL - save screenshot
SPACE - select between demo playback or manual fly
PageUp/PageDown - adjust playback speed

-----------------------------------------------------------------------
4. The console:

You can pull down the console at any time by pressing the tilde (~)
key. You can scroll the console with the PageUp/PageDown keys or can
quickly jump to the beginning or to the end of the log with the
Home/End keys. You can adjust font size with the Insert/Delete keys.
After the console is down, you can type anything into its command
line. When you press enter, the command is sent to a Lua script
interpreter and the result is displayed on the console. The engine
exports some commands, which you can call from within the console.

Here are a couple examples:

Console.SelectFont("Times New Roman");
Window.SetSize(400, 300); -- resizes the window to 400x300
Window.SetSize(0, 0); -- creates a fullscreen window
Display.SetMode(800, 600); -- switches monitor resolution
OpenGL.ListExtensions(); -- list OpenGL extensions
Sound.LoadMP3("mymusic.mp3", 60); -- loads 60 sec music
Sound.Play(); -- playback music
Exec("myscript.lua") -- tries to execute the script
Quit(); -- quit the demo

There are more commands, I'm not going to list all of them here. To
save typing, you can use the tab key to autocomplete to the next valid
command! If you have not typed anything, tab cycles through all
available commands. Important: the commands are all case sensitive!!
So typing "quit();" instead of "Quit();" won't work!


-----------------------------------------------------------------------
5. Troubleshooting:

This demo has not been thoroughly tested and is likely to contain
numerous bugs. If something goes wrong, just let me know, and I'll see
if I can fix it.

Known Issues:

- The VBO implementation in the Detonator driver version 44.03 is
buggy, causing an access violation whenever the desktop resolution is
changed from within the demo. This problem can be solved by installing
the latest drivers.

- If you can see strange stripes over the terrain, chances are your
driver's image quality is set to "High Performance". This setting
results in negligible speedup at the price of very bad image quality.
So it is recommended to use the default "Quality" setting instead.

- When changing monitor resolution from within the engine, the video
mode with the highest refresh rate is selected automatically. On
machines with an old monitor and a new video card it might be possible
that a refresh rate is reported as usable by the operating system,
when in fact the monitor does not support it. The workaround is to set
the windows desktop to the desired resolution and refresh rate, then
start the demo at desktop resolution (this way it won't try to change
frequency).

-----------------------------------------------------------------------
6. Contact:

Any feedback is welcome!! Drop me a mail at bnd@mailbox.hu!

Hope you like this little demo,


Andras Balogh
