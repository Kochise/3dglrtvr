/*
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
#include "screen.h"

#define WORMHOLE_CURVATURE 0.2
#define EXPAND_RADIUS      0.05
#define LEAVING_RADIUS     1.10
#define ENTERING_RADIUS    0.80

#define WORMHOLE_POINTS_MIN  20
#define WORMHOLE_POINTS_MAX  30
#define WORMHOLE_POINTS_RATE 10.0

static galaxyinfo* entering;
static double      entering_galaxy_r;
static vec3        entering_new_pos;

static Frame*      curframe;
static galaxyinfo* avoidance_galaxy;

#if DEBUG_DRAW
static star* debugging;
#endif

/*
 *  Auxiliary routines
 */

static void draw_string(GC gc, float x, float y, float z, char* s)
{
    glColor3f(gc.foreground.x,gc.foreground.y,gc.foreground.z);
    glRasterPos3f(x, y, z);
    glutBitmapString(gc.font, (const unsigned char*)s);
}

static void draw_arc(GC gc, float x, float y, float rs, bool fill) 
{ 
    glColor3f(gc.foreground.x,gc.foreground.y,gc.foreground.z);
    if (fill) glBegin(GL_POLYGON); 
    else glBegin(GL_LINE_LOOP);
    for (float d = 0; d < M_PI*2; d+=M_PI/10)
        glVertex2f(rs * cosf(d) + x, rs * sinf(d) + y);
    glEnd(); 
}

void draw_line(GC gc, float x1, float y1, float x2, float y2)
{
    glColor3f(gc.foreground.x,gc.foreground.y,gc.foreground.z);
    glBegin(GL_LINE_STRIP); 
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd(); 
}

static void draw_segments3d(GC gc, segment* seg, int nseg)
{
    glColor3f(gc.foreground.x,gc.foreground.y,gc.foreground.z);
    glBegin(GL_LINES); 
    for (int i=0; i < nseg; i++) {
        glVertex3f(seg[i].x1, seg[i].y1, seg[i].z1);
        glVertex3f(seg[i].x2, seg[i].y2, seg[i].z2);
    }
    glEnd(); 
}

void draw_segments(GC gc, segment* seg, int nseg)
{
    glColor3f(gc.foreground.x,gc.foreground.y,gc.foreground.z);
    glBegin(GL_LINES); 
    for (int i=0; i < nseg; i++) {
        glVertex2f(seg[i].x1, seg[i].y1);
        glVertex2f(seg[i].x2, seg[i].y2);
    }
    glEnd(); 
}

float text_width(GC gc, char* str)
{
    return glutBitmapLength(gc.font, (const unsigned char*)str);
}

float text_height(GC gc)
{
    return glutBitmapHeight(gc.font);
}

/* Draw various size texts (used for showing stars' names) */
static void draw_label(int activated, float x, float y, float z, int size, char* s)
{
    if (0 < size) {
        FontList* gca = screen.gca_label[activated];
        int i;
        for(i = 0; i < gca->num-1 && gca->sizes[i] < size; i++) ;

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        // flip y-axis to adjust original XCruiser coordinates.
        gluOrtho2D(0, screen.width, screen.height, 0);
        x -= text_width(gca->gcs[i], s)/2;
        draw_string(gca->gcs[i], x, y, z, s);
        glPopMatrix();
    }
}

/* Translate 3D to 2D */
static void trans_pos(float* x, float* y, float*z, vec3* pos)
{
    double model[16],proj[16];
    int viewport[4];
    double sx,sy,sz;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    gluProject(pos->x,pos->y,pos->z,model,proj,viewport,&sx,&sy,&sz);

    *x = sx; *y = screen.height-sy; *z = sz;
}

/* Rotate: ppos = rotete(p) */
static void rotate_coordinates(vec3* ppos, vec3* p)
{
    vec3_apply_matrix(ppos, p,
            &screen.view.dright,
            &screen.view.dupper,
            &screen.view.dforward);
}

static void update_focus(star* s, int x, int y, int r)
{
    int cx = screen.cursorx + screen.centerx - x;
    int cy = screen.cursory + screen.centery - y;
    if (cx*cx+cy*cy < r*r) screen.focus = s;
}

/* Pre-calculation of stars' positions */
static void precalc_pos(Frame* frame, star* s, double r, vec3* ofst)
{
    s->info.frame = frame;
    s->info.scaled_pos = *ofst;
    s->info.scaled_radius = r;
    if (s->info.type == ST_GALAXY && s->g.expanded && s != (star*)avoidance_galaxy) {
        for(int i = 0; i < s->g.nstars; i++) {
            vec3 ofst1;
            star* s1 = &s->g.children[i];
            vec3_set(&ofst1,
                 r * s1->info.pos.x + ofst->x,
                 r * s1->info.pos.y + ofst->y,
                 r * s1->info.pos.z + ofst->z);
            precalc_pos(frame, s1, r*s1->info.radius, &ofst1);
        }
    }
}

/* Draw a star */
static void draw_star(star* s, vec3* ppos, int activated);

/* Draw a planet */
static void draw_planet(planetinfo* p, vec3* ppos, int activated)
{
    float x,y,z;
    float rs = (double)screen.width * p->info.scaled_radius / vec3_length(ppos);

    glPushMatrix();
    glMultMatrixd(screen.mat);
    glTranslated(p->info.scaled_pos.x, p->info.scaled_pos.y, p->info.scaled_pos.z);

    trans_pos(&x, &y, &z, &p->info.scaled_pos);
    update_focus((star*)p, x, y, rs);

    GCList* gcl = screen.gcl_planet[activated][p->info.perm];
    GC gc = gcl->gcs[p->info.color % gcl->num];
    glColor3f(gc.foreground.x,gc.foreground.y,gc.foreground.z);
    GLUquadric* q = gluNewQuadric();
    gluQuadricDrawStyle(q, GLU_LINE);
    gluSphere(q, p->info.scaled_radius, 10, 10);
    gluDeleteQuadric(q);
    glPopMatrix();

    draw_label(activated, x, y - rs, z, rs/4, p->info.basename);
}

static void draw_inside_galaxy(galaxyinfo* g, int activated)
{
    /* draw the inside of galaxy */
    for(int i = 0; i < g->nstars; i++){
        star* s = &g->children[i];
        vec3 ppos;
        rotate_coordinates(&ppos, &s->info.scaled_pos);
        /* is it in sight? */
        if (0 < ppos.z || s->info.type == ST_GALAXY || s->info.type == ST_WORMHOLE) {
            if (s != (star*)avoidance_galaxy)
                draw_star(s, &ppos, activated);
        }
    }
}

static void euler_angles(vec3* angles, const vec3* x, const vec3* y, const vec3* z)
{
    // calculation of Euler angles
    double alpha = acos(-z->y/sqrt(1-z->z*z->z)) /M_PI*180.0;
    double beta  = acos(z->z) /M_PI*180.0;
    double gamma = acos(y->z/sqrt(1-z->z*z->z)) /M_PI*180.0;
    *angles = (vec3){ alpha, beta, gamma };
}

static void draw_galaxy(galaxyinfo* g, vec3* ppos, int activated)
{
    /* check whether you are entering this galaxy? */
    double d = vec3_length(ppos);
    double r = g->info.scaled_radius;
    double pers_r = r / d;
    if (g->info.parent == curframe->g && d < r * ENTERING_RADIUS) {
        /* 
         * entering: the entering galaxy
         * entering_galaxy_r: the scale of the inside galaxy
         * entering_new_pos: the position in the inside galaxy coordinates 
         */
        entering_galaxy_r = r;
        entering = g;
        vec3_set(&entering_new_pos, -g->info.scaled_pos.x / r, -g->info.scaled_pos.y / r, -g->info.scaled_pos.z / r);
    }
#if DEBUG_DRAW
    if (debugging == (star*)g) {
        fprintf(stderr, "distance: %s: r=%.3f, d=%.3f\n", g->info.basename, r, d);
    }
#endif
  
    if (curframe->g == g) activated = 1;
    /* get GC */
    GC gc;
    if (g == &Universe.g) {
        gc = screen.gc_root[activated];
    }else{
        GCList* gcl = screen.gcl_galaxy[activated][g->info.perm];
        gc = gcl->gcs[g->info.color % gcl->num];
    }
    
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

    vec3 vz;
    vec3_cproduct(&vz, &g->px, &g->py);
    vec3 angles;
    euler_angles(&angles, &g->px, &g->py, &vz);

    glPushMatrix();
    glMultMatrixd(screen.mat);
    glTranslated(g->info.scaled_pos.x, g->info.scaled_pos.y, g->info.scaled_pos.z);

    float x,y,z;
    trans_pos(&x, &y, &z, &g->info.scaled_pos);
    update_focus((star*)g, x, y, pers_r * (double)screen.width);

    glRotated(angles.x, 0,0,1);
    glRotated(angles.y, 1,0,0);
    //glRotatef(angles.z, 0,0,1);
    
    /* draw the whole ring */
    glColor3f(gc.foreground.x,gc.foreground.y,gc.foreground.z);
    glLineWidth((g == curframe->g) ? screen.currentGalaxyLineWidth : 1);
    GLUquadric* q = gluNewQuadric();
    gluQuadricDrawStyle(q, GLU_LINE);
    gluDisk(q, 0, r, 20, 4);
    gluDeleteQuadric(q);
    glLineWidth(1);
    glPopMatrix();

    /* draw the label of the galaxy */
    draw_label(activated, x, y, z, pers_r * 100.0, g->info.basename);

    if (r <= d) {
        draw_inside_galaxy(g, activated);
    }
}

/* Draw a wormhole */
/*
static void convert_zone(vec3* ppos1, int idx1, vec3* ppos0, int idx0)
{
    vec3 p = *ppos0;
    vec3* orig;
    int idx = idx0;
    while(idx != idx1) {
        orig = &view.frames[idx].g->info.scaled_pos;
        if (idx < idx1) {
            idx++;
            vec3_set(&p,
                 p.x - orig->x,
                 p.y - orig->y,
                 p.z - orig->z);
            vec3_mul(&p, 1/view.frames[idx].r);
        } else {
            vec3_mul(&p, view.frames[idx].r);
            vec3_set(&p,
                 p.x + orig->x,
                 p.y + orig->y,
                 p.z + orig->z);
            idx--;
        }
    }
    *ppos1 = p;
}
*/

static int calc_bezier(wormholeinfo* w, vec3* pts)
{
    /* p0: source */
    vec3 p0 = w->info.scaled_pos;
    /* p1: destination */
    vec3 p1 = w->nearest->info.scaled_pos;
    
    /* calc the vertices of a bezier curve if in sight */
    /* dv = w->nearest->i.scaled_radius * WORMHOLE_CURVATURE; */
    vec3 v0, v1;
    double dv = w->common->info.scaled_radius * WORMHOLE_CURVATURE;
    v0 = w->v0;
    v1 = w->v1;
    /* |v0| = |v1| = 1 */
    if (vec3_length(&p0) < vec3_length(&p1)) {
        /* focus the source */
        double r = w->nearest->info.frame->r0 / w->info.frame->r0;
        vec3_mul(&p1, r);
    } else {
        /* focus the distination */
        double r = w->info.frame->r0 / w->nearest->info.frame->r0;
        vec3_mul(&p0, r);
    }
    vec3_mul(&v0, dv);
    vec3_mul(&v1, dv);

    vec3 a = { -2*(p1.x - p0.x) + v0.x + v1.x,
               -2*(p1.y - p0.y) + v0.y + v1.y,
               -2*(p1.z - p0.z) + v0.z + v1.z };
    vec3 b = { 3*(p1.x - p0.x) - 2*v0.x - v1.x,
               3*(p1.y - p0.y) - 2*v0.y - v1.y,
               3*(p1.z - p0.z) - 2*v0.z - v1.z };
  
    int npts = (WORMHOLE_POINTS_RATE * vec3_distance(&p0, &p1) / vec3_length(&p0)) + WORMHOLE_POINTS_MIN;
    if (WORMHOLE_POINTS_MAX < npts) npts = WORMHOLE_POINTS_MAX;
  
#if DEBUG_DRAW
    if (debugging == (star*)w) {
        fprintf(stderr, "wormhole: %s(common:%s): dv=%.3f(common:%.3f), betw=%.3f, dist=%.3f, npts=%d\n",
            w->info.basename, w->common->info.basename, dv, w->common->info.scaled_radius,
            vec3_distance(&p0, &p1), vec3_length(&p0), npts);
    }
#endif
    
    npts = WORMHOLE_POINTS_MAX;
    for(int i = 0; i < npts; i++) {
        double t = (double)i / (double)(npts-1);
        vec3_set(&pts[i],
             ((a.x*t + b.x)*t + v0.x)*t + p0.x,
             ((a.y*t + b.y)*t + v0.y)*t + p0.y,
             ((a.z*t + b.z)*t + v0.z)*t + p0.z);
    }
    return npts;
}

static void draw_wormhole(wormholeinfo* w, vec3* ppos, int activated)
{
    GCList* gcl = screen.gcl_wormhole[activated];
    GC gc = gcl->gcs[w->info.color % gcl->num];
    
    /* draw the entrance and the label */
    if (0 < w->info.scaled_radius) {
        float rs = (double)screen.width * w->info.scaled_radius / vec3_length(ppos);

        glPushMatrix();
        glMultMatrixd(screen.mat);
        glTranslated(w->info.scaled_pos.x, w->info.scaled_pos.y, w->info.scaled_pos.z);

        float x,y,z;
        trans_pos(&x, &y, &z, &w->info.scaled_pos);
        update_focus((star*)w, x, y, rs);

        draw_arc(gc, 0, 0, w->info.scaled_radius, false);
        glPopMatrix();

        draw_label(activated, x, y, z, rs/4, w->info.basename);
    }
    
    /* draw the link if exists */
    if (w->nearest) {
        vec3 pprev, pts[WORMHOLE_POINTS_MAX];
        segment seg[WORMHOLE_POINTS_MAX];
        int nseg = 0;
        int npts = calc_bezier(w, pts);
        pprev = pts[0];
        for(int i = 0; i < npts; i++) {
            vec3 p = pts[i];
            seg[nseg].x1 = pprev.x; seg[nseg].y1 = pprev.y; seg[nseg].z1 = pprev.z;
            seg[nseg].x2 = p.x; seg[nseg].y2 = p.y; seg[nseg].z2 = p.z;
            nseg++;
            pprev = p;
        }
        glPushMatrix();
        glMultMatrixd(screen.mat);
        draw_segments3d(gc, seg, nseg);
        glPopMatrix();
    }
}

/* Draw a star */
static void draw_star(star* s, vec3* ppos, int activated)
{
    switch(s->info.type) {
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
    if (debugging == s) dump_star(s);
#endif
}

/* Draw the whole universe */
void draw_universe(View* view)
{
    int n = view->curidx;

    screen.focus = NULL;
    entering = NULL;
    curframe = &view->frames[n];

    for(int i = 0; i <= n; i++) {
        vec3 ofst = view->frames[i].viewp;
        vec3_mul(&ofst, -1.0);
        avoidance_galaxy = (i == n) ? NULL : view->frames[i+1].g;
        precalc_pos(&view->frames[i], (star*)view->frames[i].g, 1.0, &ofst);
    }
    for(int i = 0; i <= n; i++) {
        vec3 ppos = view->frames[i].viewp;
        vec3_mul(&ppos, -1.0);
        avoidance_galaxy = (i == n) ? NULL : view->frames[i+1].g;
        rotate_coordinates(&ppos, &ppos);
        draw_star((star*)view->frames[i].g, &ppos, 0);
    }
    
    if (entering)
        screen.velocity *= enter_galaxy(view, entering, entering_galaxy_r, &entering_new_pos);
    else if (n && LEAVING_RADIUS < vec3_length(&view->frames[n].viewp))
        screen.velocity *= leave_galaxy(view);
  
#if DEBUG_DRAW
    if (debugging) debugging = NULL;
#endif
}

/*
static void chase_pos(double* dx, double* dy, vec3* p)
{
    vec3 ppos;
    double len;
    rotate_coordinates(&ppos, p);
    len = sqrt(ppos.x*ppos.x + ppos.y*ppos.y);
    *dx = ppos.x / len * 0.01;
    *dy = ppos.y / len * 0.01;
}
*/

void do_route(route* r, double* dx, double* dy, double* dv)
{
    // for auto-pilot mode...
}

#if DEBUG_DRAW
void debug_star(star* s)
{
    debugging = s;
}

static void dump_star(star* s)
{
    fprintf(stderr, "dump_star: %s ", s->info.basename);
    switch(s->info.type) {
    case ST_GALAXY: fprintf(stderr, "(galaxy)\n"); break;
    case ST_PLANET: fprintf(stderr, "(planet)\n"); break;
    case ST_WORMHOLE: fprintf(stderr, "(wormhole)\n"); break;
    }
    fprintf(stderr, "pos:          %s\n", vec3_dbg(&s->info.pos));
    fprintf(stderr, "scaled_pos:   %s\n", vec3_dbg(&s->info.scaled_pos));
    fprintf(stderr, "radius:       %.3f\n", s->info.radius);
    fprintf(stderr, "scaled_radius:%.3f\n", s->info.scaled_radius);
}

static void dump_frame(View* v)
{
    fprintf(stderr, "dump_frame:\n");
    for(int i = 0; i <= v->curidx; i++) {
        fprintf(stderr, " %2d: r=%.3f, r0=%.3f, g=%s, viewp=%s\n",
            i, v->frames[i].r, v->frames[i].r0, v->frames[i].g->info.basename, vec3_dbg(&v->frames[i].viewp));
    }
}
#endif
