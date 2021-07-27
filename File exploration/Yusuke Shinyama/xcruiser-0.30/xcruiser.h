/*
  $Id: xcruiser.h,v 1.1.2.3 2003/01/27 01:36:10 euske Exp $

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
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#include "MainScreen.h"

#define DEBUG_DRAW 0


/*  Constants
 */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define deg2rad(x) ((x)*M_PI/180.0)
#define rad2deg(x) ((x)*180/M_PI)

/* MAX_CHARS: the maximum number of pathnames */
#define MAX_CHARS 1024

/*  Type definitions
 */
enum {
    ST_PLANET,
    ST_GALAXY,
    ST_WORMHOLE
};
enum {
    M_NONE = 0,
    M_EXPAND,
    M_COLLAPSE,
    M_DELETE
};
enum _state {
    STATE_NORMAL = 0,
    STATE_AUTOPILOT,
    STATE_REPOSITION,
};
enum _flyingmode {
    FLY_POLAR,
    FLY_CARTESIAN,
};

/* vector representing coordinates (used to be struct Point3D) */
typedef struct _vec3 {
    double x;
    double y;
    double z;
} vec3;

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

/* planet part */
typedef struct _planetinfo {
    starinfo info;
    unsigned long size;
} planetinfo;

/* galaxy part */
typedef struct _galaxyinfo {
    starinfo info;
    char* pathname;
    int marked;
    int expanded;
    vec3 px, py;
    int nstars;
    union _star* children;
} galaxyinfo;

/* wormhole part */
typedef struct _wormholeinfo {
    starinfo info;
    char** linkto;
    struct _galaxyinfo* common;
    union _star* nearest;
    vec3 v0, v1;
} wormholeinfo;

/* star */
typedef union _star {
    starinfo i;
    planetinfo p;
    galaxyinfo g;
    wormholeinfo w;
} star;

/* view:
 * View is represented as a sequence of frames.
 */
typedef struct _frame {
    double r;			/* current scale */
    double r0;			/* product of all scales */
    galaxyinfo* g;
    vec3 viewp;			/* position */
} frame;
typedef struct _view {
    int curidx;
    vec3 dright, dupper, dforward;
    frame* frms;
} view;

/* route */
typedef struct _route {
    wormholeinfo* w;
    double p;
} route;


/*  Variable declarations
 */
extern star Universe;


/*  Prototype Declarations
 */

/* in draw.c */
extern void init_draw(void);
extern void draw_universe(MainScreenWidget scr, view* vi);
extern void do_route(route* r, double* dx, double* dy, double* dv);
extern void debug_star(star* s);
extern void dump_star(star* s);
extern void dump_frame(view* s);

/* in info.c */
extern void setup_measure(MainScreenWidget scr);
extern void draw_alert(MainScreenWidget scr, char* format, ...);
extern void draw_information(MainScreenWidget scr, view* vi, int state, int flyingmode);

/* vec3.c */
extern double vec3_length(vec3* p);
extern double vec3_distance(vec3* p1, vec3* p2);
extern double vec3_dproduct(vec3* p1, vec3* p2);
extern void vec3_cproduct(vec3* p, vec3* p1, vec3* p2);
extern void vec3_set(vec3* p, double x, double y, double z);
extern void vec3_add(vec3* p, double x, double y, double z);
extern void vec3_modify(vec3* p, double n);
extern void vec3_formalize(vec3* p);
extern void vec3_apply_matrix(vec3* p, vec3* v, vec3* x, vec3* y, vec3* z);
extern char* vec3_dbg(vec3* p);

/* in zone.c */
extern void init_universe(void);
extern int init_curview(view* cur);
extern void process_marked_stars(MainScreenWidget scr);
extern double enter_galaxy(view* cur, galaxyinfo* g, double r, vec3* p);
extern double leave_galaxy(view* cur);
extern void move_ship_polar(view* cur, double v, double dx, double dy, int plimit);
extern void move_ship_cartesian(view* cur, double v, double dx, double dy);
extern double reposition_ship(view* cur);

#endif /* _XCRUISE_H */
