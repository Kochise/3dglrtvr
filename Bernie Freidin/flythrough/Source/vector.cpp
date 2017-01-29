// **************************************************************************
// HCSG Demo
// (c) Bernie Freidin, 1999-2000
// **************************************************************************

#include "vector.h"

// Inter-Dimensional Vector Assignment

// Hyperbolic Midpoint

vec2 vec2::hmidpoint(vec2& p1, vec2& p2)
{
	// EXAMPLE: vec2 m = vec2::hmidpoint(p1, p2);
	
	double q1 = sqrt(1.0 - p1*p1);
	double q2 = sqrt(1.0 - p2*p2);
	
	return (p1*q2 + p2*q1) / (q1 + q2);
}

vec3 vec3::hmidpoint(vec3& p1, vec3& p2)
{
	// EXAMPLE: vec3 m = vec3::hmidpoint(p1, p2);
	
	double q1 = sqrt(1.0 - p1*p1);
	double q2 = sqrt(1.0 - p2*p2);
	
	return (p1*q2 + p2*q1) / (q1 + q2);
}

// Hyperbolic Translation (2D)

mat3 mat3::htranslate(vec2& p)
{
	// EXAMPLE: mat3 m = mat3::translate(p);
	
	double c, s, t = 1.0 / (1.0 + sqrt(1.0 - p*p));
	
	c = 1.0 - p.y*p.y*t;
	s = 1.0 - p.x*p.x*t;
	t =       p.x*p.y*t;
 	
	return mat3(c, t, +p.x, t, s, +p.y, +p.x, +p.y, 1.0);
}

mat3 mat3::hntranslate(vec2& p)
{
	// EXAMPLE: mat3 m = mat3::translate(p);
	
	double c, s, t = 1.0 / (1.0 + sqrt(1.0 - p*p));
	
	c = 1.0 - p.y*p.y*t;
	s = 1.0 - p.x*p.x*t;
	t =       p.x*p.y*t;
 	
	return mat3(c, t, -p.x, t, s, -p.y, -p.x, -p.y, 1.0);
}

// Hyperbolic Rotation (2D)

mat3 mat3::hrotate(vec2& p, double theta)
{
	// EXAMPLE: mat3 m = mat3::hrotate(p, theta);
	
	double c = cos(theta);
	double s = sin(theta);
	double t = 1.0 / (1.0 + sqrt(1.0 - p*p));
	
	mat3 r(c, -s, 0.f, s, c, 0.0, 0.0, 0.0, 1.0);
	
	c = 1.0 - p.y*p.y*t;
	s = 1.0 - p.x*p.x*t;
	t =       p.x*p.y*t;
 	
	mat3 t1(c, t, -p.x, t, s, -p.y, -p.x, -p.y, 1.0);
	mat3 t2(c, t, +p.x, t, s, +p.y, +p.x, +p.y, 1.0);
	
	return t2*r*t1;
}
