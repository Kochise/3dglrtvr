3Dmaze

I have now worked with OpenGL for some time. I have often browsed the net for demo’s and ideas. Here I have found a lot of maze programs, but no one in 3 dimensions. Why not? OpenGL is good for 3D… so I set out to make such a program.

Graphically I have been inspired by many things, like Tron or the Matrix, but I believe the program have got its own style by now. I like the glasslike effect in transparent mode. Lovely.

I have realised that it is extremely difficult to move through a 3 dimensional maze. Even if it is only 5x5x5 and it becomes nearly impossible at 9x9x9. The maximum size is 20000 "cubes", i.e. 141x141x1 or similar, but you will get lost in there forever, and it will kill your computer in terms of performance.

A special cute version is to display a transparent maze in one level. This is an ordinary maze; you can study before you enter it.

Due to the difficulty of moving through a 3 dimensional maze, I have build a help function. Pressing “h” will cause the program to display “arrows” in the maze, pointing to the exit. (It cost a lot of score to use).

The score is for those who want that. It is a timer, starting when you enter the maze, and a timer displaying the time you have used the help function. The score is a sum of the two, where help time cost 30 times normal time. To move through the maze with a low score is what you play for…

In the data folder you can find the 9 xml definition files for the mazes. Modify them as you please to build mazes to your liking. Here you also find the setup.xml file, that determine the graphic screen set up (Size, full screen etc…)

Playing keys:

Movement:
a or up: Accelerate forward.
z or down: Decelerate.
q or left: Roll left (See also auto plane).
w or right: Roll right (See also auto plane).
space: Stop all movement.
Mouse up/down/left/right: Turn and tilt.

Misc:
Esc: Quit program.
1-9: Load mazes.
0: Randomise current maze.
h: Show help arrows in maze (cost score).

Flags:
s: Toggle solid/transparent walls.
p: Toggle auto plane. (Level to horizon).
l: Toggle Lines on walls (edges, and cross).
t: Toggle textures.
d: Toggle debug mode (show FPS and maze seed).
g: Toggle ground and sky display.

I have put the source code together with the program so you can use it if you wish. Some places is well commented, others are not commented at all. Fell free to be inspired. If you use it in your own programs, credit me or at least let me know you liked it.

Project compiles with Dev-C++ 4.9.8.0

I myself have found many inspiring pieces around the net, especially NeHe’s tutorials. I believe we all have started by studying pieces of code and examples, and I would like to return the favour with this code.

e-mail: bjarne.fich@dragonslayer.dk
http: www.dragonslayer.dk

