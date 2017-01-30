/*
  $Id: draw.c,v 1.2.2.4 2003/02/02 15:39:30 euske Exp $
  
  XCruiser - a filesystem visualization utility
  draw.c: drawing the universe
  
  Copyright (C) 1999-2002  Yusuke Shinyama <yusuke at cs . nyu . edu>
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include <limits.h>
#include "xcruiser.h"
#include "MainScreeP.h"

#define SIGHT_ANGLE		0.7
#define CLIP_RANGE		10000
#define CLIP_ZPLANE		0.001
#define GALAXY_VERTICES		120
#define WORMHOLE_CURVATURE	0.2
#define EXPAND_RADIUS		0.05
#define LEAVING_RADIUS		1.10
#define ENTERING_RADIUS		0.80

#define WORMHOLE_POINTS_MIN	5
#define WORMHOLE_POINTS_MAX	30
#define WORMHOLE_POINTS_RATE	10.0

#define select_color(parent,s1) (((parent)->info.depth < (s1)->info.depth)? 0 : 1)


/* z-buffer */
typedef struct _zstar {
    vec3	ppos;
    star*	s;
} zstar;
typedef struct _zbuffer {
    int		n;
    zstar*	zs;
} zbuffer;

static	galaxyinfo*	entering;
static	double		entering_galaxy_r;
static	vec3		entering_new_pos;

static	frame*		curframe;
static	view*		curview;
static	galaxyinfo*	avoidance_galaxy;

static	Display*	disp;
static	Drawable	buff;
static	MainScreenPart*	scr;
static	int		width, height, centerx, centery;

static	double		cirx[GALAXY_VERTICES], ciry[GALAXY_VERTICES];

#if DEBUG_DRAW
static star* debugging;
#endif


/*
 *  Auxiliary routines
 */

/*  Initialize trigonometric values 
 */
void init_draw(void)
{
    int i;
    for(i = 0; i < GALAXY_VERTICES; i++) {
	cirx[i] = cos(i * (M_PI*2.0/(double)GALAXY_VERTICES));
	ciry[i] = sin(i * (M_PI*2.0/(double)GALAXY_VERTICES));
    }
}

/*  Draw various size texts (used for showing stars' names)
 */
static void draw_label(int n, int x, int y, int size, char* s)
{
    if (0 < size) {
	int i, len = strlen(s);
	FontGCAList* gca = scr->gca_label[n];
	for(i = 0; i < gca->num_gcs-1 && gca->sizes[i] < size; i++) 
	    ;
	x -= XTextWidth(gca->fss[i], s, len)/2;
	XDrawString(disp, buff, gca->gcs[i], x, y, s, len);
    }
}

/*  Translate 3D to 2D
 */
static void trans_pos(short* x, short* y, vec3* ppos)
{
    double x0, y0;
    x0 = SIGHT_ANGLE * (double)width * ppos->x / ppos->z + (double)centerx;
    y0 =-SIGHT_ANGLE * (double)width * ppos->y / ppos->z + (double)centery;
    if (x0 < -CLIP_RANGE) {
	x0 = -CLIP_RANGE;
	y0 = -CLIP_RANGE * y0 / x0;
    } else if (CLIP_RANGE < x0) {
	x0 = CLIP_RANGE;
	y0 = CLIP_RANGE * y0 / x0;
    }
    if (y0 < -CLIP_RANGE) {
	x0 = -CLIP_RANGE * x0 / y0;
	y0 = -CLIP_RANGE;
    } else if (CLIP_RANGE < y0) {
	x0 = CLIP_RANGE * x0 / y0;
	y0 = CLIP_RANGE;
    }
    *x = x0; *y = y0;
}

/*  Clip a line which penetrates the screen
 */
static int clip_zplane(vec3* p0, vec3* p1)
{
    double z;
    if (CLIP_ZPLANE < p0->z && CLIP_ZPLANE < p1->z)
	return(1);
    if (p0->z <= CLIP_ZPLANE && p1->z <= CLIP_ZPLANE)
	return(0);
    z = (CLIP_ZPLANE - p0->z)/(p1->z - p0->z);
    if (p0->z <= CLIP_ZPLANE) {
	p0->x = (p1->x - p0->x)*z + p0->x;
	p0->y = (p1->y - p0->y)*z + p0->y;
	p0->z = CLIP_ZPLANE;
    } else {
	p1->x = (p1->x - p0->x)*z + p0->x;
	p1->y = (p1->y - p0->y)*z + p0->y;
	p1->z = CLIP_ZPLANE;
    }
    return(1);
}

/*  Rotate: ppos = rotete(p)
 */
static void rotate_coordinates(vec3* ppos, vec3* p)
{
    vec3_apply_matrix(ppos, p,
		      &curview->dright,
		      &curview->dupper,
		      &curview->dforward);
}

static int point_in_sight(vec3* p)
{
    return(0 < p->z && 
	   SIGHT_ANGLE * fabs(p->x) < p->z && 
	   SIGHT_ANGLE * fabs(p->y) < p->z);
}

static void update_focus(star* s, int x, int y, int r)
{
    int cx = scr->cursorx + centerx - x;
    int cy = scr->cursory + centery - y;
    if (cx*cx+cy*cy < r*r)
	scr->focus = s;
}


/*  Insert a star into the z-buffer
 */
static void insert_zstar(zbuffer* buf, vec3* p, star* s1)
{
    vec3 ppos;
    rotate_coordinates(&ppos, p);
    
    /* is it in sight? */
    if (0 < ppos.z || s1->i.type == ST_GALAXY || s1->i.type == ST_WORMHOLE) {
	/* insert the star into z-buffer */
	double z = ppos.z;
	int i, j;
	for(i = 0; i < buf->n && z < buf->zs[i].ppos.z; i++)
	    ;
	for(j = buf->n; i < j; j--)
	    buf->zs[j] = buf->zs[j-1];
	buf->zs[i].ppos = ppos;
	buf->zs[i].s = s1;
	buf->n++;
    }
}

/* Pre-calculation of stars' positions
 */
static void precalc_pos(frame* frm, star* s, double r, vec3* ofst)
{
    s->i.frame = frm;
    s->i.scaled_pos = *ofst;
    s->i.scaled_radius = r;
    if (s->i.type == ST_GALAXY && s->g.expanded && 
	s != (star*)avoidance_galaxy) {
	int i;
	for(i = 0; i < s->g.nstars; i++) {
	    vec3 ofst1;
	    star* s1 = &s->g.children[i];
	    vec3_set(&ofst1,
		     r * s1->i.pos.x + ofst->x,
		     r * s1->i.pos.y + ofst->y,
		     r * s1->i.pos.z + ofst->z);
	    precalc_pos(frm, s1, r*s1->i.radius, &ofst1);
	}
    }
}

/*
 *  Draw a star
 */
static void draw_planet(planetinfo* p, vec3* ppos, int activated);
static void draw_galaxy(galaxyinfo* g, vec3* ppos, int activated);
static void draw_wormhole(wormholeinfo* g, vec3* ppos, int activated);
static void draw_star1(star* s, vec3* ppos, int activated);

/*  Draw a planet
 */
static void draw_planet(planetinfo* p, vec3* ppos, int activated)
{
    if (point_in_sight(ppos)) {
	GC gc;
	GCList* gcl;
	short x, y, rs;
	
	gcl = scr->gcl_planet[activated][p->info.perm];
	gc = gcl->gcs[p->info.color % gcl->num_gcs];
	
	trans_pos(&x, &y, ppos);
	rs = (double)width * p->info.scaled_radius / vec3_length(ppos);
	XFillArc(disp, buff, gc, x-rs, y-rs, rs*2, rs*2, 0, 360*64);
	XDrawArc(disp, buff, scr->gc_background,
		 x-rs, y-rs, rs*2, rs*2, 0, 360*64);
	draw_label(activated, x, y-rs, rs/4, p->info.basename);
	update_focus((star*)p, x, y, rs);
    }
}


/*  Draw a galaxy (recursively)
 */
static void calc_galaxy_vertices(XSegment* seg0, int* nseg0,
				 XSegment* seg1, int* nseg1,
				 galaxyinfo* g, vec3* center, double r)
{
    double cx, cy;
    int i, step, n0 = 0, n1 = 0;
    double side0, side1;
    vec3 vx, vy;
    vec3 p0, p1;
    
    /* pre-calculation for drawing a ring */
    rotate_coordinates(&vx, &g->px);
    vec3_modify(&vx, r);
    rotate_coordinates(&vy, &g->py);
    vec3_modify(&vy, r);
    
    /* step: how many vertices can be skipped? */
    step = vec3_length(center)/r + 1;
    if (10 < step) step = 10;
    
    /* calculate the first vertex of a ring */
    cx = cirx[GALAXY_VERTICES - step];
    cy = ciry[GALAXY_VERTICES - step];
    vec3_set(&p0,
	     vx.x*cx + vy.x*cy + center->x,
	     vx.y*cx + vy.y*cy + center->y,
	     vx.z*cx + vy.z*cy + center->z);
    side0 = center->z - p0.z;
    
    /* calculate the rest vertices of a ring */
    for(i = 0; i < GALAXY_VERTICES; i += step) {
	cx = cirx[i];
	cy = ciry[i];
	vec3_set(&p1,
		 vx.x*cx + vy.x*cy + center->x,
		 vx.y*cx + vy.y*cy + center->y,
		 vx.z*cx + vy.z*cy + center->z);
	side1 = center->z - p1.z;
	if (clip_zplane(&p0, &p1)) {
	    if (side0 < 0 && side1 < 0) {
		trans_pos(&seg0[n0].x1, &seg0[n0].y1, &p0);
		trans_pos(&seg0[n0].x2, &seg0[n0].y2, &p1);
		n0++;
	    } else {
		trans_pos(&seg1[n1].x1, &seg1[n1].y1, &p0);
		trans_pos(&seg1[n1].x2, &seg1[n1].y2, &p1);
		n1++;
	    }
	}
	side0 = side1;
	p0 = p1;
    }
    *nseg0 = n0;
    *nseg1 = n1;
}

static void draw_inside_galaxy(galaxyinfo* g, int activated)
{
    /* draw the inside of galaxy */
    zbuffer buf;
    buf.n  = 0;
    buf.zs = (zstar*)malloc(sizeof(zstar) * g->nstars);
    if (buf.zs) {
	int i;
	for(i = 0; i < g->nstars; i++) {
	    star* s1 = &g->children[i];
	    if (s1 != (star*)avoidance_galaxy)
		insert_zstar(&buf, &s1->i.scaled_pos, s1);
	}
	for(i = 0; i < buf.n; i++)
	    draw_star1(buf.zs[i].s, &buf.zs[i].ppos, activated);
	free((char*) buf.zs);
    }
}

static void draw_galaxy(galaxyinfo* g, vec3* ppos, int activated)
{
    double d, r, pers_r;
    
    /* check whether you are entering this galaxy? */
    d = vec3_length(ppos);
    r = g->info.scaled_radius;
    pers_r = r / d;
    if (g->info.parent == curframe->g && d < r * ENTERING_RADIUS) {
	/* entering:  the entering galaxy
	 * entering_galaxy_r:   the scale of the inside galaxy
	 * entering_new_pos: the position in the inside galaxy coordinates 
	 */
	entering_galaxy_r = r;
	entering = g;
	vec3_set(&entering_new_pos,
		 -g->info.scaled_pos.x / r,
		 -g->info.scaled_pos.y / r,
		 -g->info.scaled_pos.z / r);
    }
#if DEBUG_DRAW
    if (debugging == (star*)g) {
	fprintf(stderr, "distance: %s: r=%.3f, d=%.3f\n", g->info.basename, r, d);
    }
#endif
  
    if (point_in_sight(ppos) || d < r) {
	GC gc;
	int nseg0, nseg1;
	short x, y;
	XSegment seg0[GALAXY_VERTICES], seg1[GALAXY_VERTICES];	
    
	trans_pos(&x, &y, ppos);
	update_focus((star*)g, x, y, pers_r * (double)width);
    
	if (curframe->g == g)
	    activated = 1;
	/* get GC */
	if (g == &Universe.g) {
	    gc = scr->gc_root[activated];
	} else {
	    GCList* gcl = scr->gcl_galaxy[activated][g->info.perm];
	    gc = gcl->gcs[g->info.color % gcl->num_gcs];
	}
	
	/* calc the perspective position */
	calc_galaxy_vertices(seg0, &nseg0, seg1, &nseg1, g, ppos, r);
	
	/* draw the first half */
	XSetLineAttributes(disp, gc,
			   (g == curframe->g)? scr->currentGalaxyLineWidth : 0,
			   0,0,0);
	XDrawSegments(disp, buff, gc, seg0, nseg0);
	
	/* is it expanded? */
	if (g->expanded) {
	    /* check collapse */
	    if (g != &Universe.g && pers_r < EXPAND_RADIUS && !g->marked) {
		g->marked = M_COLLAPSE;
	    } else {
		draw_inside_galaxy(g, activated);
	    }
	} else {
	    /* check expand */
	    if (EXPAND_RADIUS < pers_r && !g->marked) {
		g->marked = M_EXPAND;
	    }
	}
	
	/* draw the second half */
	XSetLineAttributes(disp, gc,
			   (g == curframe->g)? scr->currentGalaxyLineWidth : 0,
			   0,0,0);
	XDrawSegments(disp, buff, gc, seg1, nseg1);
	
	/* draw the label of the galaxy */
	draw_label(activated, x, y, pers_r * 100.0, g->info.basename);
    } else {
	draw_inside_galaxy(g, activated);
    }
}


/*  Draw a wormhole
 */
static void convert_zone(vec3* ppos1, int idx1, vec3* ppos0, int idx0)
{
    vec3 p = *ppos0;
    vec3* orig;
    int idx = idx0;
    while(idx != idx1) {
	orig = &curview->frms[idx].g->info.scaled_pos;
	if (idx < idx1) {
	    idx++;
	    vec3_set(&p,
		     p.x - orig->x,
		     p.y - orig->y,
		     p.z - orig->z);
	    vec3_modify(&p, 1/curview->frms[idx].r);
	} else {
	    vec3_modify(&p, curview->frms[idx].r);
	    vec3_set(&p,
		     p.x + orig->x,
		     p.y + orig->y,
		     p.z + orig->z);
	    idx--;
	}
    }
    *ppos1 = p;
}

static int calc_bezier(wormholeinfo* w, vec3* pts)
{
    int i, npts;
    double dv;
    vec3 p0, p1, a, b, v0, v1;
  
    /* p0: source */
    p0 = w->info.scaled_pos;
    /* p1: destination */
    p1 = w->nearest->i.scaled_pos;
    rotate_coordinates(&p0, &p0);
    rotate_coordinates(&p1, &p1);
    
    /* calc the vertices of a bezier curve if in sight */
    /* dv = w->nearest->i.scaled_radius * WORMHOLE_CURVATURE; */
    dv = w->common->info.scaled_radius * WORMHOLE_CURVATURE;
    rotate_coordinates(&v0, &w->v0);
    rotate_coordinates(&v1, &w->v1);
    /* |v0| = |v1| = 1 */
    if (vec3_length(&p0) < vec3_length(&p1)) {
	/* focus the source */
	double r = w->nearest->i.frame->r0 / w->info.frame->r0;
	vec3_modify(&p1, r);
    } else {
	/* focus the distination */
	double r = w->info.frame->r0 / w->nearest->i.frame->r0;
	vec3_modify(&p0, r);
    }
    vec3_modify(&v0, dv);
    vec3_modify(&v1, dv);
    vec3_set(&a,
	     -2*(p1.x - p0.x) + v0.x + v1.x,
	     -2*(p1.y - p0.y) + v0.y + v1.y,
	     -2*(p1.z - p0.z) + v0.z + v1.z);
    vec3_set(&b,
	     3*(p1.x - p0.x) - 2*v0.x - v1.x,
	     3*(p1.y - p0.y) - 2*v0.y - v1.y,
	     3*(p1.z - p0.z) - 2*v0.z - v1.z);
  
    npts = (WORMHOLE_POINTS_RATE * vec3_distance(&p0, &p1) /
	    vec3_length(&p0)) + WORMHOLE_POINTS_MIN;
    if (WORMHOLE_POINTS_MAX < npts) npts = WORMHOLE_POINTS_MAX;
  
#if DEBUG_DRAW
    if (debugging == (star*)w) {
	fprintf(stderr, "wormhole: %s(common:%s): dv=%.3f(common:%.3f), betw=%.3f, dist=%.3f, npts=%d\n",
		w->info.basename, w->common->info.basename, dv, w->common->info.scaled_radius,
		vec3_distance(&p0, &p1), vec3_length(&p0), npts);
    }
#endif
    
    npts = WORMHOLE_POINTS_MAX;
    for(i = 0; i < npts; i++) {
	double t = (double)i / (double)(npts-1);
	vec3_set(&pts[i],
		 ((a.x*t + b.x)*t + v0.x)*t + p0.x,
		 ((a.y*t + b.y)*t + v0.y)*t + p0.y,
		 ((a.z*t + b.z)*t + v0.z)*t + p0.z);
    }
    return(npts);
}

static void draw_wormhole(wormholeinfo* w, vec3* ppos, int activated)
{
    /* get GC */
    GC gc;
    GCList* gcl;

    gcl = scr->gcl_wormhole[activated];
    gc = gcl->gcs[w->info.color % gcl->num_gcs];
    
    /* draw the entrance and the label */
    if (point_in_sight(ppos) && 0 < w->info.scaled_radius) {
	short x, y, rs;
	trans_pos(&x, &y, ppos);
	rs = (double)width * w->info.scaled_radius / vec3_length(ppos);
	XDrawArc(disp, buff, gc, x-rs, y-rs, rs*2, rs*2, 0, 360*64);
	draw_label(activated, x, y, rs/4, w->info.basename);
	update_focus((star*)w, x, y, rs);
    }
    
    /* draw the link if exists */
    if (w->nearest) {
	int i, nseg, npts;
	vec3 pprev, pts[WORMHOLE_POINTS_MAX];
	XSegment seg[WORMHOLE_POINTS_MAX];
	nseg = 0;
	npts = calc_bezier(w, pts);
	pprev = pts[0];
	for(i = 0; i < npts; i++) {
	    vec3 p = pts[i];
	    if (clip_zplane(&pprev, &p)) {
		trans_pos(&seg[nseg].x1, &seg[nseg].y1, &pprev);
		trans_pos(&seg[nseg].x2, &seg[nseg].y2, &p);
		nseg++;
	    }
	    pprev = p;
	}
	XDrawSegments(disp, buff, gc, seg, nseg);
    }
}


/*  Draw a star
 */
static void draw_star1(star* s, vec3* ppos, int activated)
{
    switch(s->i.type) {
    case ST_GALAXY:
	draw_galaxy(&s->g, ppos, activated);
	break;
    case ST_PLANET:
	draw_planet(&s->p, ppos, activated);
	break;
    case ST_WORMHOLE:
	draw_wormhole(&s->w, ppos, activated);
	break;
    }
#if DEBUG_DRAW
    if (debugging == s) {
	dump_star(s);
    }
#endif
}


/*  Draw the whole universe
 */
void draw_universe(MainScreenWidget me, view* vi)
{
    int i, n = vi->curidx;
  
    disp = XtDisplay((Widget)me);
    scr = &me->mainScreen;
    buff = scr->screen_buff;
    width = me->core.width;
    height = me->core.height;
    centerx = me->mainScreen.centerx;
    centery = me->mainScreen.centery;
  
    me->mainScreen.focus = NULL;
    entering = NULL;
    curview = vi;
    curframe = &vi->frms[n];
    
    for(i = 0; i <= n; i++) {
	vec3 ofst = vi->frms[i].viewp;
	vec3_modify(&ofst, -1.0);
	avoidance_galaxy = (i == n)? NULL : vi->frms[i+1].g;
	precalc_pos(&vi->frms[i], (star*)vi->frms[i].g, 1.0, &ofst);
    }
    for(i = 0; i <= n; i++) {
	vec3 ppos = vi->frms[i].viewp;
	vec3_modify(&ppos, -1.0);
	avoidance_galaxy = (i == n)? NULL : vi->frms[i+1].g;
	rotate_coordinates(&ppos, &ppos);
	draw_star1((star*)vi->frms[i].g, &ppos, 0);
    }
    
    if (entering) {
	scr->velocity *= enter_galaxy(vi, entering, entering_galaxy_r, &entering_new_pos);
    } else if (n && LEAVING_RADIUS < vec3_length(&vi->frms[n].viewp)) {
	scr->velocity *= leave_galaxy(vi);
    }
  
#if DEBUG_DRAW
    if (debugging) {
	debugging = NULL;
    }
#endif
}

void chase_pos(double* dx, double* dy, vec3* p)
{
    vec3 ppos;
    double len;
    rotate_coordinates(&ppos, p);
    len = sqrt(ppos.x*ppos.x + ppos.y*ppos.y);
    *dx = ppos.x / len * 0.01;
    *dy = ppos.y / len * 0.01;
}

void do_route(route* r, double* dx, double* dy, double* dv)
{
    
}

#if DEBUG_DRAW
void debug_star(star* s)
{
    debugging = s;
}

void dump_star(star* s)
{
    fprintf(stderr, "dump_star: %s ", s->i.basename);
    switch(s->i.type) {
    case ST_GALAXY:
	fprintf(stderr, "(galaxy)\n");
	break;
    case ST_PLANET:
	fprintf(stderr, "(planet)\n");
	break;
    case ST_WORMHOLE:
	fprintf(stderr, "(wormhole)\n");
	break;
    }
    fprintf(stderr, "pos:          %s\n", vec3_dbg(&s->i.pos));
    fprintf(stderr, "scaled_pos:   %s\n", vec3_dbg(&s->i.scaled_pos));
    fprintf(stderr, "radius:       %.3f\n", s->i.radius);
    fprintf(stderr, "scaled_radius:%.3f\n", s->i.scaled_radius);
}

void dump_frame(view* v)
{
    int i;
    fprintf(stderr, "dump_frame:\n");
    for(i = 0; i <= v->curidx; i++) {
	fprintf(stderr, " %2d: r=%.3f, r0=%.3f, g=%s, viewp=%s\n",
		i, v->frms[i].r, v->frms[i].r0, v->frms[i].g->info.basename,
		vec3_dbg(&v->frms[i].viewp));
    }
}
#endif
