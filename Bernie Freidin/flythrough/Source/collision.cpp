// **************************************************************************
// HCSG Demo
// (c) Bernie Freidin, 1999-2000
// **************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "collision.h"

/*
General CDR Algorithm
---------------------
As input, we have a current POSITION and an IMPULSE vector.
The goal of CDR is to determine the new POSITION resulting
from collisions with static geometry. For now, we will
ignore things like gravity and friction, and assume the
colliding object is a sphere.

First, we determine the set of polygons that are within
the collision boundary. The collision boundary is, roughly,
the bounding sphere between POSITION and POSITION+IMPULSE,
expanded by RADIUS. This enumeration traverses the HCSG
hierarchy.

Next, we enter the COLLISION LOOP and repeatedly update
the sweep parameters until the impulse is zero.

Finally, we pass the new position back to the camera
handling function, so that the OpenGL MODELVIEW matrix
may be updated and the scene rendered.

OPTIMIZATION
------------
There are three main avenues for optimization:

1. Reducing the cost per primitive collision. This involves
   algebraic simplifications and code flow improvements.
   Particularly useful would be faster cylinder collisions
   and point-in-polygon tests.
2. Reducing the number of primitive collisions. This
   involves improving the enumeration of relevant polygons
   within the collision bounding sphere, eliminating
   "hidden" collision primitives (e.g. sphere collisions
   on interior vertices), and eliminating duplicate
   collisions (e.g. cylinder collisions on shared edges).
3. Defining certain hull nodes to be "collision-free",
   such that the camera never enters them and their polygons
   are only used for rendering. This is both an optimization
   and an overall navigation improvement, since it could be
   used to eliminate awkward collisions with small geometric
   details (e.g. small cracks in the floor, etc.)
*/

#ifndef CSG_MAX_VERTS_PER_POLY
#define CSG_MAX_VERTS_PER_POLY 40
#endif
#define CDR_MAX_HITS 40
#define CDR_TOLERANCE 1e-8

struct hit_t
{
	double t;
	int k;
	vec3 n;
	vec3 e[CSG_MAX_VERTS_PER_POLY];
};

struct sweep_t
{
	double radius;
	double t;
	vec3 impulse;
	vec3 slide;
	vec3 position;
	vec3 residual;
	int hitcount;
	int pplcount;
	hit_t hit[CDR_MAX_HITS];
	hit_t ppl[CDR_MAX_HITS];
};

static vert_t *CDR_VERT_POOL;
static surf_t *CDR_SURF_POOL;
static hull_t *CDR_WORLD;

#define CDR_MAX_OBJ 1000
static hit_t CDR_OBJ_LIST[CDR_MAX_OBJ];
static int   CDR_OBJ_COUNT;

static int    CDR_GetMaxAxis(vec3 &n);
static double CDR_SnapTo(double z, double z0);
static double CDR_SnapTo(double z, double z0, double z1);
static int    CDR_PointTestQ(vec3 &p, vec3 e[], vec3 &n, int k);
static void   CDR_SweepHit(sweep_t *sweep, vec3 e[], vec3 &n, int k, double t);
static void   CDR_SweepPpl(sweep_t *sweep, vec3 e[], vec3 &n, int k, double t);
static void   CDR_SweepCollision(sweep_t *sweep, vec3 e[], vec3 &n, int k);
static void   CDR_SweepUpdate(sweep_t *sweep);

static void CDR_Search(bbox_t *bbox, hull_t *hull);
static vec3 CDR_GetPolyNormal(poly_t *poly);

// ****************************************************************************
// ****************************************************************************

void CDR_Initialize(hull_t *world)
{
	CSG_PUSHNAME("CDR_Initialize");
	
	// **************************************************
	// Sets static lists so that geometric structures may
	// be referenced by collision-detection functions.
	// This must be called after 
	// **************************************************
	
	CDR_VERT_POOL = CSG_GetVertPool();
	CDR_SURF_POOL = CSG_GetSurfPool();
	CDR_WORLD     = world;
	CSG_POPNAME_;
}

void CDR_Collide(vec3 &position, vec3 &impulse, double radius)
{
	// ************************************
	// Compute effective collision boundary
	// ************************************
	
	bbox_t bbox;
	
	vec3 midp = position + impulse*0.5;
	double cr = sqrt(impulse*impulse)*0.5 + radius*1.1;
	bbox.minv = midp - vec3(cr, cr, cr);
	bbox.maxv = midp + vec3(cr, cr, cr);
	
	// **********************************
	// Enumerate polygons within boundary
	// **********************************
	
	CDR_OBJ_COUNT = 0;
	CDR_Search(&bbox, CDR_WORLD);
	
	if(CDR_OBJ_COUNT < 1)
	{
		position += impulse;
		return;
	}
	// **************
	// Collision loop
	// **************
	
	sweep_t sweep;
	sweep.residual = impulse;
	sweep.radius   = radius;
	
	while(1)
	{
		sweep.position = position;
		sweep.impulse  = sweep.residual;
		sweep.slide    = sweep.residual;
		sweep.residual = vec3(0.0, 0.0, 0.0);
		
		for(int i = 0; i < 25; i++) //FIXME: max collisions per sweep = 25
		{
			sweep.t = 1.0; // no hit
			sweep.hitcount = 0;
			sweep.pplcount = 0;
			
			for(int j = 0; j < CDR_OBJ_COUNT; j++)
			{
				hit_t *obj = &CDR_OBJ_LIST[j];
				
				CDR_SweepCollision(&sweep, obj->e, obj->n, obj->k);
			}
			CDR_SweepUpdate(&sweep);
			
			position = sweep.position;
			
			if(sweep.impulse*sweep.impulse < CDR_TOLERANCE)
			{
				position = sweep.position;
				return;
			}
		}
		//shouldn't get here...
		position = sweep.position;
		return;
	}
}

// ****************************************************************************
// ****************************************************************************

static int CDR_GetMaxAxis(vec3 &n)
{
	int axis = 0;
	if(fabs(n[1]) > fabs(n[axis])) axis = 1;
	if(fabs(n[2]) > fabs(n[axis])) axis = 2;
	return axis;
}

static double CDR_SnapTo(double z, double z0)
{
	if(fabs(z - z0) < CDR_TOLERANCE) z = z0;
	return z;
}

static double CDR_SnapTo(double z, double z0, double z1)
{
	if(fabs(z - z0) < CDR_TOLERANCE) z = z0;
	if(fabs(z - z1) < CDR_TOLERANCE) z = z1;
	return z;
}

static int CDR_PointTestQ(vec3 &p, vec3 e[], vec3 &n, int k)
{
	if(k == 2)
	{
		vec3 w = e[1] - e[0];
		int axis = CDR_GetMaxAxis(w);
		double t = p[axis];
		if(t < e[0][axis] && t < e[1][axis]) return 0;
		if(t > e[0][axis] && t > e[1][axis]) return 0;
	}
	else if(k > 2)
	{
		int a[3], axis = CDR_GetMaxAxis(n);
		
		switch(axis)
		{
			case 0: a[0] = 2, a[1] = 1, a[2] = 0; break;
			case 1: a[0] = 0, a[1] = 2, a[2] = 1; break;
			case 2: a[0] = 1, a[1] = 0, a[2] = 2; break;
		}
		vec2 v3 = vec2(p[a[0]], p[a[1]]);
		double sig = 0.0;
		
		for(int i = 0;;)
		{
			vec2 v1, v2;
			v1.x = e[i][a[0]];
			v1.y = e[i][a[1]];
			if(++i == k) i = 0;
			v2.x = e[i][a[0]];
			v2.y = e[i][a[1]];
			double t = (v1-v3)^(v2-v3);
			if(t*sig < -CDR_TOLERANCE) return 0;
			sig = t;
			if(i == 0) break;
		}
	}
	return 1;
}

static void CDR_SweepHit(sweep_t *sweep, vec3 e[], vec3 &n, int k, double t)
{
	if(t < sweep->t)
	{
		sweep->hitcount = 0;
		sweep->t = t;
	}
	CSG_ASSERT(sweep->hitcount < CDR_MAX_HITS, "too many hits");
	hit_t *hit = &sweep->hit[sweep->hitcount++];
	
	hit->t = t;
	hit->k = k;
	hit->n = n;
	memcpy(hit->e, e, sizeof(vec3)*k);
}

static void CDR_SweepPpl(sweep_t *sweep, vec3 e[], vec3 &n, int k, double t)
{
	CSG_ASSERT(sweep->pplcount < CDR_MAX_HITS, "too many ppl hits");
	hit_t *ppl = &sweep->ppl[sweep->pplcount++];
	
	ppl->t = t;
	ppl->k = k;
	ppl->n = n;
	memcpy(ppl->e, e, sizeof(vec3)*k);
}

static void CDR_SweepCollision(sweep_t *sweep, vec3 e[], vec3 &n, int k)
{
	double r = sweep->radius;
	double numer, denom;
	vec3 q = sweep->position - e[0];
	vec3 v = sweep->slide;
	
	if(k == 1) // sphere
	{
		double vv = v*v, qv = q*v, qq = q*q - r*r;
		double fa = CDR_SnapTo(vv, 0.0);
		double fb = CDR_SnapTo(qv, 0.0);
		double fc = CDR_SnapTo(qq, 0.0);
		double fh = CDR_SnapTo(fb*fb - fa*fc, 0.0);
		if(fh < 0.0) return;
		numer = CDR_SnapTo(-fb - sqrt(fh), 0.0);
		denom = CDR_SnapTo(+fa, 0.0);
	}
	else if(k == 2) // cylinder
	{
		vec3 w = e[1] - e[0];
		
		double vv = v*v, qv = q*v, qq = q*q - r*r;
		double ww = w*w, qw = q*w, vw = v*w;
		double fa = CDR_SnapTo(vv*ww - vw*vw, 0.0); // (v^w)*(v^w)
		double fb = CDR_SnapTo(qv*ww - qw*vw, 0.0); // (q^w)*(v^w)
		double fc = CDR_SnapTo(qq*ww - qw*qw, 0.0); // (q^w)*(q^w) - (w*w)*(r*r)
		double fh = CDR_SnapTo(fb*fb - fa*fc, 0.0);
		if(fh < 0.0) return;
		numer = CDR_SnapTo(-fb - sqrt(fh), 0.0);
		denom = CDR_SnapTo(+fa, 0.0);
	}
	else if(k > 2) // polygon
	{
		numer = CDR_SnapTo(+(q*n) - r, 0.0);
		denom = CDR_SnapTo(-(v*n), 0.0);
	}
	if(denom <= 0.0)
	{
		if(numer == 0.0 && denom == 0.0)
		{
			if(k == 1) return; // error!
			if(k == 2)
			{
				vec3 w = e[1] - e[0];
				n = w^(q^w);
				n /= sqrt(n*n);
			}
			CDR_SweepPpl(sweep, e, n, k, -1.0);
		}
		return;
	}
	double t = CDR_SnapTo(numer/denom, 0.0, sweep->t);
	
	if(t < 0.0 || t > sweep->t) return;
	if(k < 3)
	{
		n = q + v*t;
		
		if(k == 2)
		{
			vec3 w = e[1] - e[0];
			n = w^(n^w);
		}
		n /= sqrt(n*n);
	}
	vec3 p = sweep->position + v*t - n*r;
	if(!CDR_PointTestQ(p, e, n, k)) return;
	
	double ref = CDR_SnapTo(v*n, 0,0);
	
	if(k >= 3)
	{
	//	printf("hit polygon");
	}
	if(ref == 0.0) CDR_SweepPpl(sweep, e, n, k, t);
	else           CDR_SweepHit(sweep, e, n, k, t);
}

static void CDR_SweepUpdate(sweep_t *sweep)
{
	vec3 p = sweep->position + sweep->slide*sweep->t;
	vec3 m = sweep->impulse * (1.0 - sweep->t);
	
	sweep->position = p;
	sweep->impulse  = m;
	
	if(sweep->hitcount < 1) return;
	
	for(int i = 0; i < sweep->pplcount; i++)
	{
		hit_t *ppl = &sweep->ppl[i];
		
		if(ppl->t == -1.0)
		{
			if(!CDR_PointTestQ(p, ppl->e, ppl->n, ppl->k)) continue;
		}
		else if(ppl->t != sweep->t) continue;
		
		hit_t *hit = &sweep->hit[sweep->hitcount++];
		memcpy(hit, ppl, sizeof(hit_t));
	}
	for(int a = 0; a < sweep->hitcount; a++)
	{
		vec3 na = sweep->hit[a].n;
		vec3 va = m - na*(m*na);
// horrible hack: makes collisions sort-of work  =(
sweep->slide = va;
sweep->position += na*0.05;
return;
	
		int blocked = 0;
		
		for(int b = 0; b < sweep->hitcount; b++)
		{
			if(b == a) continue;
			
			vec3 nb = sweep->hit[b].n;
			vec3 vb = m - nb*(m*nb);
			
			if(nb*va < 0.0)
			{
				if(na*vb < 0.0)
				{
					vec3 nab = na^nb; nab = (m^nab)^nab;
					vec3 vab = m - nab*(m*nab);
					
					for(int c = 0; c < sweep->hitcount; c++)
					{
						if(c == a || c == b) continue;
						
						vec3 nc = sweep->hit[c].n;
						
						if(nc*vab < 0.0)
						{
							sweep->impulse *= 0.0;
							sweep->slide   *= 0.0;
							return;
						}
					}
					sweep->slide = vab;
					return;
				}
				blocked = 1;
			}
		}
		if(!blocked)
		{
			sweep->slide = va;
			return;
		}
	}
	sweep->impulse *= 0.0;
	sweep->slide   *= 0.0;
}

static void CDR_Search(bbox_t *bbox, hull_t *hull)
{
	CSG_PUSHNAME("Search");
	
	// *******************************************************
	// Recursively search solid polygons for intersection with
	// collision effect sphere.
	// *******************************************************
	
	if(CSG_Intersect(bbox, &hull->bbox, 7))
	{
		CSG_POPNAME_;
	}
	for(poly_t *f = hull->face; f; f = f->next)
	{
		if(CSG_Intersect(bbox, &f->bbox, 7)) continue;
		
		for(poly_t *p = f->down; p; p = p->next)
		{
			//FIXME: optimize: don't re-test single-polygon faces
			if(CSG_Intersect(bbox, &p->bbox, 7)) continue;
			
			CSG_ASSERT(CDR_OBJ_COUNT+(p->vertcount*2)+1 <= CDR_MAX_OBJ, "too many polygons");
			
			hit_t *obj1 = &CDR_OBJ_LIST[CDR_OBJ_COUNT++];
			obj1->k = p->vertcount;
			obj1->n = CDR_GetPolyNormal(p);
			
			for(int i = 0; i < p->vertcount; i++)
			{
				int j = (i>0)?(i-1):(p->vertcount-1);
				
				obj1->e[i] = CDR_VERT_POOL[p->vref[i]];
				
				hit_t *obj2 = &CDR_OBJ_LIST[CDR_OBJ_COUNT++];
				obj2->k = 1;
				obj2->e[0] = CDR_VERT_POOL[p->vref[i]];
				
				hit_t *obj3 = &CDR_OBJ_LIST[CDR_OBJ_COUNT++];
				obj3->k = 2;
				obj3->e[0] = CDR_VERT_POOL[p->vref[i]];
				obj3->e[1] = CDR_VERT_POOL[p->vref[j]];
			}
		}
	}
	for(hull_t *h = hull->down; h; h = h->next)
	{
		CDR_Search(bbox, h);
	}
	CSG_POPNAME_;
}

static vec3 CDR_GetPolyNormal(poly_t *poly)
{
	vec3 n = CDR_SURF_POOL[poly->surf_id & CSG_SURF_MASK].norm;
	poly_t *p2 = (poly_t*)poly->parent;
	hull_t *h2 = (hull_t*)p2->parent;
	if(!(h2->depth & 1)) n *= -1.0;
	if(poly->surf_id & CSG_SIDE_MASK) n *= -1.0;
	return n;
}
