#include "vector.h"

// Hyperbolic Midpoint

vec2 vec2::hmidpoint(vec2& p1, vec2& p2)
{
	// EXAMPLE: vec2 m = vec2::hmidpoint(p1, p2);
	
	float q1 = (float)sqrt(1.f - p1*p1);
	float q2 = (float)sqrt(1.f - p2*p2);
	
	return (p1*q2 + p2*q1) / (q1 + q2);
}

vec3 vec3::hmidpoint(vec3& p1, vec3& p2)
{
	// EXAMPLE: vec3 m = vec3::hmidpoint(p1, p2);
	
	float q1 = (float)sqrt(1.f - p1*p1);
	float q2 = (float)sqrt(1.f - p2*p2);
	
	return (p1*q2 + p2*q1) / (q1 + q2);
}

// Hyperbolic Translation (2D)

mat3 mat3::htranslate(vec2& p)
{
	// EXAMPLE: mat3 m = mat3::translate(p);
	
	float c, s, t = 1.f / (1.f + (float)sqrt(1.f - p*p));
	
	c = 1.f - p.y*p.y*t;
	s = 1.f - p.x*p.x*t;
	t =       p.x*p.y*t;
 	
	return mat3(c, t, +p.x, t, s, +p.y, +p.x, +p.y, 1.f);
}

mat3 mat3::hntranslate(vec2& p)
{
	// EXAMPLE: mat3 m = mat3::translate(p);
	
	float c, s, t = 1.f / (1.f + (float)sqrt(1.f - p*p));
	
	c = 1.f - p.y*p.y*t;
	s = 1.f - p.x*p.x*t;
	t =       p.x*p.y*t;
 	
	return mat3(c, t, -p.x, t, s, -p.y, -p.x, -p.y, 1.f);
}

// Hyperbolic Rotation (2D)

mat3 mat3::hrotate(vec2& p, float theta)
{
	// EXAMPLE: mat3 m = mat3::hrotate(p, theta);
	
	float c = (float)cos(theta);
	float s = (float)sin(theta);
	float t = 1.f / (1.f + (float)sqrt(1.f - p*p));
	
	mat3 r(c, -s, 0.f, s, c, 0.f, 0.f, 0.f, 1.f);
	
	c = 1.f - p.y*p.y*t;
	s = 1.f - p.x*p.x*t;
	t =       p.x*p.y*t;
 	
	mat3 t1(c, t, -p.x, t, s, -p.y, -p.x, -p.y, 1.f);
	mat3 t2(c, t, +p.x, t, s, +p.y, +p.x, +p.y, 1.f);
	
	return t2*r*t1;
}
