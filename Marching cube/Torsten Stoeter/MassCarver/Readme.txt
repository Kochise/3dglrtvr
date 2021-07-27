Mass Carver
A Volume Renderer based on 3D Texture Slicing
Version 1.0

By Torsten Stoeter, Jan 20, 2009
torsten dot stoeter at st dot ovgu dot de


INTRO
Mass Carver is my attempt at a basic volume renderer utilizing hardware-
accelerated 3D textures on graphics cards to display volume datasets at inter-
active frame-rates. Raw 8-bit volume datasets can be processed and an
accompanying metafile provides necessary additional information to built up the
3D texture. Among a description and resolution of the dataset, the metafile also
contains an opacity and color transfer function. In the rendering cycle view
aligned slices crossing the 3D texture are blended together creating a direct
volume image or a maximum intensity projection, corresponding to the render mode
selected.

BUILD
To build the program from source code under Linux you'll need the GLUT and GLEW
libraries installed. Simply run make in the source code subdirectory. For
convenience a built executable is provided for Windows systems.

USAGE
Run the program on a dataset's metafile to load and display this volume dataset.
Here's an example to open the Baby Head dataset:

$ ./carve baby.vol

CONTROLS
Keyboard
	esc		exit
	r		change render mode
	+		increase amount of slices
	-		decrease amount of slices
Mouse
	left button	rotate volume

METAFILE FORMAT
This is how a custom volume metafile is composed. Note that you cannot add
comments to the file, like shown below, cause this will lead to mis-
interpretation.
--- example.vol ---
Example			# description text
example.raw		# 8 bit raw file name
256 256 44		# x y z resolution
1 1 0.5			# x y z scaling
5			# number of values for opacity tranfser function
50	0		# value in ascending order and corresponding opacity
80	0.1		# next values...
90	0
160	0
170	1
5			# number of values for opacity tranfser function
50	0 0 0		# value in ascending order and corresponding r g b triple
80	0 0.8 0		# next values...
90	0 0 0
160	0 0 0
170	1 1 1
--- end of file ---

DISCLAIMER
Source code and binaries are provided free of charge and without any warranty.
Use it at your own risk. You may use my code in your own projects, informing
me about where and how you use it.

