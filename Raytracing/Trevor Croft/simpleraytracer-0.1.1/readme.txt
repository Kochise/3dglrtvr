
________________________________________________________________

HOW TO COMPILE
________________________________________________________________

Compile and link all the .cpp files in the src dir using your favorite tools.
Makefiles are provided in the src dir for windows and any unix flavor.
	Windows: nmake -f makefile.win
	*nix: make
Microsoft Visual C++ 6.0 Workspace and Project files are provided in the msvc dir.
________________________________________________________________



________________________________________________________________

EXAMPLE USAGES
________________________________________________________________

First you should get a PPM file viewer, try irfanview: http://www.irfanview.com/



output to default.ppm, 
place the eye at (0,0,0),  
no dynamically inserted light sources:

simplert



output to my.ppm, 
place the eye at (0,0,0),
no dynamically inserted light sources:

simplert my.ppm



output to my.ppm,
place the eye at (1,2,5),
no dynamically inserted light sources:

simplert my.ppm 1 2 5



output to my.ppm, 
place the eye at (0,0,0),
light source at (1,1,1.5) with intensity 0.5:

simplert my.ppm 0 0 0 1 1 1.5 0.5



output to my.ppm, 
place the eye at (0,0,-5), 
light source at (1,1,1.5) with intensity 0.5,
light source at (2.5,2,2) with intensity 0.3:

simplert my.ppm 0 0 -5 1 1 1.5 0.5 2.5 2 2 0.3



You cannot set arguments that come later on in the list without setting the earlier ones.

________________________________________________________________



________________________________________________________________

COORDINATE SYSTEM
________________________________________________________________

The coordinate system for lights and shapes is as follows:
+x to the right
+y is up
+z is into the monitor

The eye always looks straight forward.

Example Position of eye:
	(1,3,-10) - x=1, y=3, z=-10
	Eye moves to the right 1 unit, up 3 units and backwards, away from the monitor 10 units.
	
________________________________________________________________



________________________________________________________________

CUSTOMIZATIONS
________________________________________________________________

Modifying settings:
See the RayTracer constructor in RayTracer.cpp.


Modifying the model:

You must modify main.cpp

the model is in between the two comments:
// begin setup model

// end setup model

You make light sources as follows:
light = new Light(Point(10,2,0)); // put light at 10,2,0
light->Intensity(0.7); // set the intensity of the light to a value from 0-1
p_ray_tracer->AddLight(light); // add the light to the model

You make spheres as follows:
sphere = new Sphere(Point(0,3,20),10); 
// put center of sphere at 0,3,20 it has a radius of 10
sphere->ShapeColor(Color(0,0.5,1)); 
// set the color of the sphere Red=0 Green=0.5 Blue=1  the values must be between 0-1
sphere->DiffuseReflectivity(0.4); 
// set how reflective the sphere is, between 0-1
p_ray_tracer->AddShape(sphere); // add the sphere to the model

You make triangles as follows:
triangle = new Triangle(Point(-20,3,20),  Point(20,3,20), Point(0,-3,2));
// make a new triangle with the points
triangle->ShapeColor(Color(0,0,0));
triangle->DiffuseReflectivity(0.8);
p_ray_tracer->AddShape(triangle);

The model could be generated dynamically through some form of a config file. 
However generating the model dynamically does not demonstrate ray tracing techniques
so it would clutter this project.
This project values simplicity over functionality.

________________________________________________________________




________________________________________________________________

FEATURES
________________________________________________________________

Triangles
Spheres
Shadows
Single Light Source Not at Eye
Code Readability
Specular
Reflection
Multiple Light Sources
Anti-aliasing
Dynamic Light position and intensity and dynamic Eye position
Ambient Color and Intensity as well as Ambient Reflectivity of objects
Non-black backgrounds
Light Intensity


black surfaces make the best mirrors.
________________________________________________________________



________________________________________________________________

BUGS
________________________________________________________________

Black dots from internal reflection on triangles, I haven't seen this show up with spheres.
You can fix this by increasing the anti-aliasing, but the fix doesn't make a perfect image.
________________________________________________________________



________________________________________________________________

FEEDBACK
________________________________________________________________

All forms of feedback is welcome: comments, criticism, praise, and suggestions.

Trevor Croft
trevorcroft@hotmail.com
________________________________________________________________

