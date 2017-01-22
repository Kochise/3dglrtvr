An Incremental Implicit Surface Tesselation Algorithm
=====================================================
Sebastien Loisel, loisel@math.mcgill.ca

Overview
========

The pathetic Marching Cubes algorithm isn't cutting it. We want to do something
better to visualize our metaballs. This short paper presents an algorithm idea
which will hopefully yield a better tesselation with fewer polygons and most 
importantly, smaller memory requirements. Hopefully this can be made fast,
I'll tell you if it is when I try it.

The idea is to seed space with a number of vertices (the number of vertices
you want in the end tesselation) and then try to evolve these vertices
(hill-climb) to a situation where sit on the boundary of the implicit surface.
In the evolution process we can even introduce some sort of repulsion force
which will make certain that points are distributed sensibly.

Introduction
============

We are given a set of metaballs (I won't bother to discuss those, if you're
curious, see elsewhere). One important point is that they should deform
"sensibly" and that their "rest shape" (the shape they take when they're
all alone) be known and tesselated easily.

Then what we do is we generate a tesselation for each metaball assuming that
it's in its "rest shape". If it is, we're done. If it's not then we'll try
to move these vertices around so that they fit the actual shape of the balls.

Once we've reached a satisfactory state we just render the triangles as they
are and grin.

Initial tesselation
===================

We're given some metaballs. A metaball is defined by a function f from R^3 to R
with the inside of the metaball in rest shape given by f(x,y,z)>1. We usually
think of f as the density distribution of the metaball.

Given a list of metaballs, f1, f2, f3, ..., fn, we put g=f1+f2+f3+...+fn and
rendering the set of metaballs is essentially rendering the implicit volume
defined by g>1.

Here we'll assume that f is nice, that is, the more f is differentiable and the
tamer the derivatives are, the better it is.

Now we seed the algorithm by generating a tesselation for f1, f2, f3, ..., fn
assuming they are in rest shape. In most case, the rest shape for the
metaballs will be a sphere so what we're doing here is generating a tesselation
for a sphere. If we want we can even choose different tesselation
levels/precision for each ball, according to specific needs. For instance,
an algorithm could decide to generate a high detail tesselation for a ball
or even just a part of it which will be very obvious on the screen while
generating a pathetic level tesselation for a ball that's thought to be not
even on the screen.

Tesselating a sphere isn't covered here, find this somewhere else.

Hill-Climbing the Tesselation
=============================

Now we need to jiggle the vertices around and make them gravitate towards the
actual boundary of the volume g>1. Now we note that the gradient of g, which
is the vector valued function (dg/dx,dg/dy,dg/dz) points in the direction of
greatest ascent for the function g.

So for each vertex vk we have, we compute g(vk) and the gradient Dg(vk). If
g(vk)>1 we need to decrease it so we replace vk by vk-w*Dg(vk) where vk is
called the "step size" and is a very small value (say, 0.01). If on the
other hand g(vk)<1 then we replace vk by vk+w*Dg(vk).

The clever reader will find other ways to find the boundary. For instance,
the problem can be re-writen as finding the root of g(x)-1, to which we
can apply a root-finding algorithm like Newton-Raphson in 3d (actually, Newton
is flaky in its convergence so that may not be such a good idea), fixed point
iteration, whatever. Make sure to pick a relatively stable algorithm. Another
way is to look at this as a spring thing and do the usual trick when we're
trying to find the rest position of a system of springs (a tweaked Euler).

Do this for all the vertices and you have new vertex positions. Repeat a few
times and then draw and grin.

Now you can add some tweaks and gizmos. You can add some sort of force that
pushes vertices away from one another thus attempting to make the distribution
of the vertices as uniform as possible. As a further tweak, the repulsion force
can be scaled according to the magnitude of the gradient - a small gradient
will make the repulsion stronger while a large gradient will make the repulsion
tiny. This will make the tesselation adapt and have many vertices where the
surface has edge-like features.

Conclusion
==========

I don't know. Have to try it first.
