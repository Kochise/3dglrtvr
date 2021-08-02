/*
  XCruiser - a filesystem visualization utility
  xcruiser.h: common header file
    
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

#ifndef _XCRUISE_H
#define _XCRUISE_H

#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <GL/freeglut.h>

#define DEBUG_DRAW 0

/* MAX_CHARS: the maximum number of pathnames */
#define MAX_CHARS 1024

/* Type definitions */
enum { ST_PLANET, ST_GALAXY, ST_WORMHOLE, };
enum { M_NONE = 0, M_EXPAND, M_COLLAPSE, M_DELETE, };
enum _state { STATE_NORMAL = 0, STATE_AUTOPILOT, STATE_REPOSITION, };
enum _flyingmode { FLY_POLAR, FLY_CARTESIAN, };

/* vector representing coordinates (used to be struct Point3D) */
typedef struct _vec3 {
    double x,y,z;
} vec3;

/* graphics context emulation */

// colors
#define white      {1,1,1}
#define red        {1,0,0}
#define green      {0,1,0}
#define green128   {0,128./255,0}
#define blue       {0,0,1}
#define cyan       {0,1,1}
#define cyan128    {0,128./255,128./255}
#define magenta    {1,0,1}
#define magenta192 {192./255,0,192./255}
#define yellow     {1,1,0}
#define yellow192  {192./255,192./255,0}
#define orange     {1,165./255,0}
#define darkorange {1,140./255,0}
#define violet     {238./255,130./255,238./255}
#define darkviolet {148./255,0,211./255}
#define brown      {165./255,42./255,42./255}
#define gray       {190./255,190./255,190./255}
#define gray96     {96./255,96./255,96./255}
#define bg         {0,0,32./255}

typedef vec3 color;

typedef struct _GC {
    color foreground;
    void* font;
} GC;

typedef struct _GCList {
    int num;
    GC* gcs;
    int* sizes;
} GCList;

typedef GCList FontList;

/* generic information of a star */
typedef struct _starinfo {
    char* basename;
    int type;
    int perm;
    int color;
    int depth;
    vec3 pos;
    double radius;
    struct _galaxyinfo* parent;
    /* temporary item */
    vec3 scaled_pos;
    double scaled_radius;
    struct _frame* frame;
} starinfo;

/* planet */
typedef struct _planetinfo {
    starinfo info;
    unsigned long size;
} planetinfo;

/* galaxy */
typedef struct _galaxyinfo {
    starinfo info;
    char* pathname;
    int marked;
    bool expanded;
    vec3 px, py;
    int nstars;
    union _star* children;
} galaxyinfo;

/* wormhole */
typedef struct _wormholeinfo {
    starinfo info;
    char** linkto;
    struct _galaxyinfo* common;
    union _star* nearest;
    vec3 v0, v1;
} wormholeinfo;

/* star */
typedef union _star {
    starinfo info;
    planetinfo p;
    galaxyinfo g;
    wormholeinfo w;
} star;

/* view:
 * View is represented as a sequence of frames.
 */
typedef struct _frame {
    double r;      /* current scale */
    double r0;     /* product of all scales */
    galaxyinfo* g;
    vec3 viewp;    /* position */
} Frame;

typedef struct _view {
    int curidx;
    vec3 dright, dupper, dforward;
    Frame* frames;
} View;

/* route */
typedef struct _route {
    wormholeinfo* w;
    double p;
} route;

/* Xt data structures */
typedef struct _segment {
    float x1, y1, z1, x2, y2, z2; 
} segment;

typedef struct _rectangle {
    float x, y;
    float width, height;
} rectangle;

/* Variable declarations */
extern star Universe;

/* Prototype Declarations */

/* in draw.c */
extern void draw_line(GC gc, float x1, float y1, float x2, float y2);
extern void draw_segments(GC gc, segment* seg, int nseg);
extern float text_width(GC gc, char* str);
extern float text_height(GC gc);
extern void draw_universe(View* vi);
extern void do_route(route* r, double* dx, double* dy, double* dv);
#if DEBUG_DRAW
extern void debug_star(star* s);
extern void dump_frame(View* s);
#endif

/* in info.c */
extern void draw_alert(char* format, ...);
extern void setup_measure();
extern void draw_information(View* vi, int state, int flyingmode);

/* vec3.c */
extern double vec3_length(vec3* p);
extern double vec3_distance(vec3* p1, vec3* p2);
extern void vec3_cproduct(vec3* p, vec3* p1, vec3* p2);
extern double vec3_dproduct(vec3* p1, vec3* p2);
extern void vec3_apply_matrix(vec3* p, vec3* v, vec3* x, vec3* y, vec3* z);
extern void vec3_set(vec3* p, double x, double y, double z);
extern void vec3_add(vec3* p, double x, double y, double z);
extern void vec3_mul(vec3* p, double n);
extern void vec3_normalize(vec3* p);
extern void vec3_orthogonal(vec3* p, vec3* q);
extern char* vec3_dbg(vec3* p);

/* in zone.c */
extern void init_universe(void);
extern int init_view(View* view);
extern void process_marked_stars();
extern double enter_galaxy(View* view, galaxyinfo* g, double r, vec3* p);
extern double leave_galaxy(View* view);
extern void move_ship_polar(View* view, double v, double dx, double dy, int plimit);
extern void move_ship_cartesian(View* view, double v, double dx, double dy);
extern double reposition_ship(View* view);

/* in screen.c */
extern void init_screen();
extern void render();
extern void set_velocity(double d);
extern void add_velocity(double d);
extern void brake(double d);
extern void move_cursor_to(double x, double y);
extern void move_cursor(double x, double y);
extern void toggle_ui(char* mode);
//extern route* set_route(wormholeinfo* w);
extern void open_file();
extern void get_info();
extern void autopilot(char* place);
extern void reposition();
extern void freeze();
extern void flyingmode();
#if DEBUG_DRAW
extern void debug();
#endif

#endif /* _XCRUISE_H */
