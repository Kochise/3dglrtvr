/*
  XCruiser - a filesystem visualization utility
  zone.c: managing directory trees
  
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

#include <sys/stat.h>
#include <dirent.h>
#include "xcruiser.h"

#define curt(x) pow(x, 1.0/3.0)

/* Constants */
/* MAX_FRAMES: the maximum number of frames */
#define MAX_FRAMES 64
/* MAX_STARS: the maximum number of the stars in a galaxy */
#define MAX_STARS 256
/* MAX_LINKDEPTH: the maximum depth of symbolic links */
#define MAX_LINKDEPTH 64
/* FILE_UNIT_SIZE: for calculating the mass of a file */
#define FILE_UNIT_SIZE 4096

#define ROTATION_RATIO      0.40
#define RADIUS_PROPORTION   0.05
#define DISTANCE_PROPORTION 0.80

/* debug */
#define DEBUG_SCAN 0
#define DEBUG_MATRIX 0
#define DEBUG_ENTER_LEAVE 0

/*
star* find_common(star* s1, star* s2)
{
    while(s1 != s2) {
        if (s1->info.depth < s2->info.depth) {
            s2 = (star*)s2->info.parent;
        } else if (s1->info.depth > s2->info.depth) {
            s1 = (star*)s1->info.parent;
        } else {  // s1->info.depth == s2->info.depth
            s1 = (star*)s1->info.parent;
            s2 = (star*)s2->info.parent;
        }
    }
    return s1;
}
*/

/* Build a galaxy tree */
/* calculate the galaxy direction from a name */
static void calc_dir(vec3* p, char* name)
{
    int rot1 = 0, rot2 = 0, len = strlen(name);
    for(int i = 0; i < len; i++) {
        if (i % 2) rot2 += *name++;
        else rot1 += *name++;
    }

    double r1 = rot1 * 0.01, r2 = rot2 * 0.01;
    p->x = -sin(r1) * cos(r2);
    p->y =  cos(r1) * cos(r2);
    p->z =  sin(r2);
}

/* calculate the star position from a name */
static double calc_pos(star* s1)
{
    int i;
    char* s;
    double r1, r2, r;
    int len = strlen(s1->info.basename);
    for(s = s1->info.basename, r1 = 0, r = 0.1, i = 0;
            i < len; i += 2, s += 2, r *= ROTATION_RATIO) {
        r1 += (*s & 0x3F) * r;
    }
    for(s = s1->info.basename+1, r2 = 0, r = 0.1, i = 1;
            i < len; i += 2, s += 2, r *= ROTATION_RATIO) {
        r2 += (*s & 0x3F) * r;
    }
    
    if (10 < len) len = 10;
    double d = (double)(s1->info.basename[0] % 15 + len*3 - 5);
    s1->info.pos.x = -d * sin(r1) * cos(r2);
    s1->info.pos.y =  d * cos(r1) * cos(r2);
    s1->info.pos.z =  d * sin(r2);
    return d;
}

/* make a planet */
static int make_planet(star* s1, struct stat* buf1)
{
    s1->info.type = ST_PLANET;
    s1->info.perm = (buf1->st_mode & S_IROTH) || ((buf1->st_mode & S_IRUSR) && buf1->st_uid == getuid());
    s1->info.radius = curt((double)(buf1->st_size / FILE_UNIT_SIZE)) + 1.0;
    s1->p.size = buf1->st_size;
    return 0;
}

/* make a galaxy */
static int make_galaxy(char* path, star* s1, struct stat* buf1)
{
    char pathbuf[MAX_CHARS];
    sprintf(pathbuf, "%s/", path);
    if (!(s1->g.pathname = strdup(pathbuf))) return -1;
    s1->info.type = ST_GALAXY;
    s1->info.perm = ((buf1->st_mode & S_IROTH) && (buf1->st_mode & S_IXOTH)) ||
        ((buf1->st_mode & S_IRUSR) && (buf1->st_mode & S_IXUSR) && buf1->st_uid == getuid());
    s1->info.radius = curt((double)(buf1->st_size / FILE_UNIT_SIZE)) + 1.0;
    s1->g.marked = M_NONE;
    s1->g.expanded = false;
    s1->g.nstars = 0;
    s1->g.children = NULL;
    /* s1->info.px and s1->info.py must be orthogonal */
    calc_dir(&s1->g.px, s1->info.basename);
    vec3_orthogonal(&s1->g.py, &s1->g.px);
    return 0;
}

/* make a wormhole */
static int crop_path(char* buf, char** pathp)
{
    char* path = *pathp;
    while(*path == '/') path++;
    if (*path == '\0') return 0;
    int i = 0;
    while(*path && *path != '/') {
        *buf++ = *path++;
        i++;
    }
    *pathp = path;
    *buf++ = '\0';
    return ++i;
}

static int make_wormhole(char* path, star* s1, galaxyinfo* parent, struct stat* buf1)
{
    char destpath[MAX_CHARS];
    char* pathidx[MAX_LINKDEPTH];
    int depth = 0;
    
    /* read a symbolic link */
    { 
        int len;
        if ((len = readlink(path, destpath, sizeof(destpath)-1)) < 0) return -1;
        destpath[len] = '\0';
    }
    /* crop the pathname */
    {
        int len, i = 0;
        char* sp = destpath;
        char* buf = malloc(MAX_CHARS + MAX_LINKDEPTH);
        if (destpath[0] == '/') {
            strcpy(buf, "/");
            pathidx[depth++] = buf;
            sp++;
            i = 2;
        }
        while(depth < MAX_LINKDEPTH-1 && (len = crop_path(buf + i, &sp)) ) {
            pathidx[depth++] = buf + i;
            i += len;
        }
        pathidx[depth] = NULL;
        s1->w.linkto = (char**)malloc(sizeof(char*) * (depth+1));
        for(i = 0; i <= depth; i++)
            s1->w.linkto[i] = pathidx[i];
    }
    /* Here, each pathidx[0]..pathidx[depth-1] is a directory name, pathidx[depth] is NULL */
    /* find the common directory */
    {
        galaxyinfo* glist[MAX_LINKDEPTH];
        galaxyinfo* g = parent;
        int cur, i, n = 0;
        while(n < MAX_LINKDEPTH && g) {
            glist[n++] = g;
            g = g->info.parent;
        }
        n--;
        /* glist[n] == &Universe.g, glist[0] == parent */
        if (pathidx[0][0] == '/') {
            cur = n;
            i = 1;
        } else {
            cur = i = 0;
        }
        for(; i < depth; i++) {
            if (!strcmp(pathidx[i], ".")) {
                ;
            } else if (cur < n && !strcmp(pathidx[i], "..")) {
                cur++;
            } else if (0 < cur &&
                   !strcmp(pathidx[i], glist[cur-1]->info.basename)) {
                cur--;
            } else {
                break;
            }
        }
        s1->w.common = glist[cur];
    }
    s1->w.nearest = NULL;
    calc_dir(&s1->w.v0, s1->info.basename);
    calc_dir(&s1->w.v1, pathidx[depth-1]);
    vec3_mul(&s1->w.v1, -1.0);
#if DEBUG_SCAN
    fprintf(stderr, "make_wormhole: %s -> %s (%s)\n", path, destpath, s1->w.common->info.basename);
#endif
    s1->info.type = ST_WORMHOLE;
    s1->info.perm = 1;
    s1->info.radius = 1.0;
    return 0;
}

/* scan a directory and fill the array of the galaxy g */
static void scan_dir(galaxyinfo* g)
{
    static star starbuff[MAX_STARS];
    DIR* dirp;
    double rmax = 0, dmax = 0;
    int n = 0;
    
    /* is this directory readable? */
    if (g->info.perm && (dirp = opendir(g->pathname))) {
        star* s1 = starbuff;
        struct dirent* ent;
        /* read directory */
        while( (ent = readdir(dirp)) )
        {
            int result;
            char pathbuf[MAX_CHARS];
            struct stat starbuf1;
            if (MAX_STARS <= n) {
                fprintf(stderr, "scan_dir: too many stars: %d in %s\n", n, g->pathname);
                break;
            }
            if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..") || ent->d_name[0] == '.')
                continue;
            /* star's name */
            if (!(s1->info.basename = strdup(ent->d_name)))
                return; /* can't allocate */
            sprintf(pathbuf, "%s%s", g->pathname, s1->info.basename);
            /* get the information */
            lstat(pathbuf, &starbuf1);
            result = -1;
            if ((starbuf1.st_mode & S_IFLNK) == S_IFLNK) {
                /* for wormhole */
                result = make_wormhole(pathbuf, s1, g, &starbuf1);
            } else if ((starbuf1.st_mode & S_IFDIR) == S_IFDIR) {
                /* for galaxy */
                result = make_galaxy(pathbuf, s1, &starbuf1);
            } else if ((starbuf1.st_mode & S_IFREG) == S_IFREG &&
                   (starbuf1.st_mode & S_IFBLK) != S_IFBLK &&
                   (starbuf1.st_mode & S_IFCHR) != S_IFCHR &&
                   g->pathname[1]) {
                /* for planet */
                result = make_planet(s1, &starbuf1);
            }
            if (!result) {
                /* if succeed, add generic information */
                double d;
                s1->info.color = s1->info.basename[0];
                s1->info.depth = g->info.depth + 1;
                s1->info.parent = g;
                d = calc_pos(s1);
                if (rmax < s1->info.radius)
                    rmax = s1->info.radius;
                if (dmax < d)
                    dmax = d;
                n++; s1++;
            }
        }
        closedir(dirp);
    }
  
    if (n) {
        g->children = (star*)malloc(sizeof(star) * n);
        if (g->children) {
            /* coordinate position and size */
            for(int i = 0; i < n; i++) {
                starbuff[i].info.radius *= (RADIUS_PROPORTION / rmax);
                vec3_mul(&starbuff[i].info.pos, DISTANCE_PROPORTION / dmax);
#if DEBUG_SCAN
                fprintf(stderr, "scan_dir: %d: %s, r=%3.3f, %s\n", 
                        i, starbuff[i].i.basename, starbuff[i].i.radius, vec3_dbg(&starbuff[i].i.pos));
#endif
            }
            /* copy the buffer */
            memcpy((char*)g->children, (char*)starbuff, sizeof(star) * n);
            g->nstars = n;
        } else {
            g->nstars = 0;
        }
    }
}

/* update a wormhole */
static int update_wormhole1(wormholeinfo* w)
{
    star* nearest;
    int i;
    char* path1;

    /* find the nearest star */
    if (w->linkto[0][0] == '/') {
        nearest = &Universe;
        i = 1;
    } else {
        nearest = (star*)w->info.parent;
        i = 0;
    }
    while((path1 = w->linkto[i++]) && nearest->info.type == ST_GALAXY && nearest->g.expanded) {
        if (!strcmp(path1, ".")) {
            ;
        } else if (!strcmp(path1, "..") && nearest->info.parent) {
            nearest = (star*)nearest->info.parent;
        } else {
            for(int j = 0; j < nearest->g.nstars; j++) {
                star* s1 = &nearest->g.children[j];
                if (!strcmp(path1, s1->info.basename)) {
                    nearest = s1;
                    break;
                }
            }
        }
    }
    
    /* update the curve when found */
    w->nearest = nearest;
#if DEBUG_SCAN
    fprintf(stderr, "update_wormhole1: %s->%s\n", w->linkto[0], nearest->i.basename);
#endif
    return 0;
}

/* Update wormholes in a galaxy (recursively) */
static void update_wormholes(galaxyinfo* g)
{
#if DEBUG_SCAN
    fprintf(stderr, "update_wormholes: %s\n", g->pathname);
#endif
    if (g->expanded && g->children)
    {
        for(int i = 0; i < g->nstars; i++)
        {
            star* s1 = &g->children[i];
            switch(s1->info.type) {
            case ST_GALAXY:
                update_wormholes(&s1->g);
                break;
            case ST_WORMHOLE:
                update_wormhole1(&s1->w);
                break;
            }
        }
    }
}

/* Expand a galaxy */
static int need_update;
static void expand_galaxy(galaxyinfo* g)
{
    g->expanded = true;
#if DEBUG_SCAN
    fprintf(stderr, "expand: %s\n", g->pathname);
#endif
    draw_alert("Scanning...\n%s\n", g->pathname);
    scan_dir(g);
    need_update = 1;
}

/* Collapse a galaxy */
static void delete_star(star* s1);

static void collapse_galaxy(galaxyinfo* g)
{
    if (g->expanded) {
#if DEBUG_SCAN
        fprintf(stderr, "collapse: %s\n", g->pathname);
#endif    
        g->expanded = false;
        if (g->nstars) {
            for(int i = 0; i < g->nstars; i++)
            delete_star(&g->children[i]);
            g->nstars = 0;
        }
    }
    need_update = 1;
}

static void delete_star(star* s1)
{
    switch(s1->info.type) {
    case ST_PLANET:
        free((char*)s1->info.basename);
        break;
    case ST_WORMHOLE:
        free((char*)s1->w.linkto[0]);
        free((char*)s1->w.linkto);
        free((char*)s1->info.basename);
        break;
    case ST_GALAXY:
        collapse_galaxy(&s1->g);
        free((char*)s1->g.children);
        free((char*)s1->g.pathname);
        free((char*)s1->info.basename);
        break;
    }
}

static void process_marked_star1(star* s1)
{
    if (s1->info.type == ST_GALAXY) {
        switch(s1->g.marked) {
        case M_EXPAND: expand_galaxy(&s1->g); break;
        case M_COLLAPSE: collapse_galaxy(&s1->g); break;
        default:
            for(int i = 0; i < s1->g.nstars; i++)
                process_marked_star1(&s1->g.children[i]);
        }
        s1->g.marked = M_NONE;
    }
}

void process_marked_stars()
{
    need_update = 0;
    process_marked_star1(&Universe);
    if (need_update) update_wormholes(&Universe.g);
}

/* Calculate the direction of the ship

     left-handed:
       y         
       | z       
       |/        
       +--x      
                 
      X = Z * Y     
      Y = X * Z     
      Z = Y * X     
*/

static void move_ship_prim(View* view, double v, vec3* d)
{
    v *= 0.02;
    for(int i = view->curidx; 0 <= i; i--) {
        vec3_add(&view->frames[i].viewp, v * d->x, v * d->y, v * d->z);
        v *= view->frames[i].r;
    }
#if DEBUG_MATRIX
    fprintf(stderr, "move_ship: right %s\n", vec3_dbg(&view->dright));
    fprintf(stderr, "move_ship: upper %s\n", vec3_dbg(&view->dupper));
    fprintf(stderr, "move_ship: forward  %s\n", vec3_dbg(&view->dforward));
    fprintf(stderr, "move_ship: products (%1.3f, %1.3f, %1.3f)\n",
        vec3_dproduct(&view->dright, &view->dupper),
        vec3_dproduct(&view->dupper, &view->dforward),
        vec3_dproduct(&view->dforward, &view->dright));
#endif /* DEBUG_MATRIX */
}

void move_ship_polar(View* view, double v, double dx, double dy, int plimit)
{
    vec3 v0;
  
    vec3_set(&v0,
         dx * view->dright.x + view->dforward.x,
         dx * view->dright.y + view->dforward.y,
         dx * view->dright.z + view->dforward.z);
    vec3_normalize(&v0);

    vec3_set(&view->dforward,
         dy * view->dupper.x + v0.x,
         dy * view->dupper.y + v0.y,
         dy * view->dupper.z + v0.z);
    vec3_normalize(&view->dforward);
    
    if (plimit < M_PI/2) {
        double vlimit = sqrt(view->dforward.x*view->dforward.x + view->dforward.z*view->dforward.z) * tan(plimit);
        if (view->dforward.y < -vlimit) {
            view->dforward.y = -vlimit;
            vec3_normalize(&view->dforward);
        } else if (vlimit < view->dforward.y) {
            view->dforward.y = vlimit;
            vec3_normalize(&view->dforward);
        }
        vec3_set(&v0, 0.0, 1.0, 0.0);
        vec3_cproduct(&view->dright, &view->dforward, &v0);
        vec3_normalize(&view->dright);
        vec3_cproduct(&view->dupper, &view->dright, &view->dforward);
        vec3_normalize(&view->dupper);
    } else {
        /* plimit >= M_PI/2 */
        vec3_add(&view->dright, -dx * v0.x, -dx * v0.y, -dx * v0.z);
        vec3_normalize(&view->dright);
        vec3_add(&view->dupper, -dy * v0.x, -dy * v0.y, -dy * v0.z);
        vec3_normalize(&view->dupper);
    }

    move_ship_prim(view, v, &view->dforward);
}

void move_ship_cartesian(View* view, double v, double dx, double dy)
{
    vec3 v0;
    dx = pow(dx*10.0, 3);
    dy = pow(dy*10.0, 3);
    vec3_set(&v0,
         dx * view->dright.x + dy * view->dupper.x + v * view->dforward.x,
         dx * view->dright.y + dy * view->dupper.y + v * view->dforward.y,
         dx * view->dright.z + dy * view->dupper.z + v * view->dforward.z);
    move_ship_prim(view, 1.0, &v0);
}

static double direct_ship(View* view, double t, vec3* gright, vec3* gupper, vec3* gforward)
{
    vec3_set(&view->dright, 
         view->dright.x * (1-t) + gright->x * t,
         view->dright.y * (1-t) + gright->y * t,
         view->dright.z * (1-t) + gright->z * t);
    vec3_set(&view->dupper, 
         view->dupper.x * (1-t) + gupper->x * t,
         view->dupper.y * (1-t) + gupper->y * t,
         view->dupper.z * (1-t) + gupper->z * t);
    vec3_set(&view->dforward, 
         view->dforward.x * (1-t) + gforward->x * t,
         view->dforward.y * (1-t) + gforward->y * t,
         view->dforward.z * (1-t) + gforward->z * t);

    return fabs(vec3_dproduct(gright, &view->dright) + vec3_dproduct(gupper, &view->dupper) + vec3_dproduct(gforward, &view->dforward));
}

double reposition_ship(View* view)
{
    vec3 gright, gupper, gforward;
    gforward = view->dforward;
    gforward.y = 0;
    vec3_normalize(&gforward);
    vec3_set(&gupper, 0.0, 1.0, 0.0);
    vec3_cproduct(&gright, &gforward, &gupper);
    vec3_normalize(&gright);
    return direct_ship(view, 0.1, &gright, &gupper, &gforward);
}

/* Enter/Leave a galaxy */
double enter_galaxy(View* view, galaxyinfo* g, double galaxy_r, vec3* newpos)
{
    if (view->curidx < MAX_FRAMES-1) {
#if DEBUG_ENTER_LEAVE
        fprintf(stderr, "enter_galaxy: %s: galaxy_r=%.3f newpos=%s\n", g->info.basename, galaxy_r, vec3_dbg(newpos));
#if DEBUG_DRAW
        dump_frame(view);
#endif
#endif
        view->curidx++;
        view->frames[view->curidx].g = g;
        view->frames[view->curidx].r = galaxy_r;
        view->frames[view->curidx].r0 = view->frames[view->curidx-1].r0 * galaxy_r;
        view->frames[view->curidx].viewp = *newpos;
    } else {
        fprintf(stderr, "enter_galaxy: too many depth\n");
    }
    return galaxy_r;
}

double leave_galaxy(View* view)
{
    double r = 1;
    if (0 < view->curidx) {
#if DEBUG_ENTER_LEAVE
        fprintf(stderr, "leave_galaxy: %s\n", view->frames[view->curidx].g->info.basename);
#if DEBUG_DRAW
        dump_frame(view);
#endif
#endif
        r = view->frames[view->curidx].r;
        view->frames[view->curidx].g = NULL;
        view->curidx--;
    } else {
        fprintf(stderr, "leave_galaxy: out of the universe(impossible!)\n");
    }
    return r;
}

/* Initialization */
void init_universe(void)
{
    Universe.g.info.basename = "";
    Universe.g.info.type    = ST_GALAXY;
    Universe.g.info.perm    = 1;
    Universe.g.info.color   = 0;
    vec3_set(&Universe.g.info.pos, 0.0, 0.0, 0.0);
    Universe.g.info.depth   = 0;
    Universe.g.info.radius  = 1.0;
    Universe.g.info.parent  = NULL;
    Universe.g.marked       = M_EXPAND;
    Universe.g.pathname     = "/";
    Universe.g.expanded     = false;
    vec3_set(&Universe.g.px, 1.0, 0.0, 0.0);
    vec3_set(&Universe.g.py, 0.0, 0.0, 1.0);
    process_marked_stars();
}

int init_view(View* view)
{
    view->curidx = 0;
    vec3_set(&view->dright,   1.0, 0.0, 0.0);
    vec3_set(&view->dupper,   0.0, 1.0, 0.0);
    vec3_set(&view->dforward, 0.0, 0.0, 1.0);
    view->frames = (Frame*)malloc(sizeof(Frame) * MAX_FRAMES);
    if (!view->frames) return -1;
    view->frames[0].r = 1.0;
    view->frames[0].r0 = 1.0;
    view->frames[0].g = &Universe.g;
    vec3_set(&view->frames[0].viewp, 0.0, 0.0, -2.0);
    return 0;
}
