/*
  XCruiser - a filesystem visualization utility
  screen.c: Implementation of main screen

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

#include <GL/freeglut.h>
#include "xcruiser.h"
#include "screen.h"

#define DEBUG_EVENT 0

color innerGalaxyColors[2] = { cyan, gray };
color innerUnreadableGalaxyColors[1] = { magenta };
color innerPlanetColors[4] = { yellow, orange, violet, red };
color innerUnreadablePlanetColors[1] = { magenta };
color innerWormholeColors[1] = { green };
color innerLabelColor = white;

color outerGalaxyColors[2] = { cyan128, gray96 };
color outerUnreadableGalaxyColors[1] = { magenta192 };
color outerPlanetColors[4] = { yellow192, darkorange, darkviolet, brown };
color outerUnreadablePlanetColors[1] = { magenta192 };
color outerWormholeColors[1] = { green128 };
color outerLabelColor = gray;

void* labelFonts[3] = { GLUT_BITMAP_HELVETICA_10, GLUT_BITMAP_HELVETICA_12, GLUT_BITMAP_HELVETICA_18 };  

static GCList* getGCList(color* colors, int n)
{
    GCList* gcl = (GCList*)malloc(sizeof(GCList));
    gcl->num = n;
    gcl->gcs = (GC*)malloc(sizeof(GC) * gcl->num);
    for (int i = 0; i < gcl->num; i++) gcl->gcs[i] = (GC){ colors[i], NULL };
    return gcl;
}

static FontList* getFontList(color p, void** fonts, int n)
{
    FontList* fgca = (FontList*)malloc(sizeof(FontList));
    fgca->num = n;
    fgca->sizes = (int*)malloc(sizeof(int) * fgca->num);
    fgca->gcs = (GC*)malloc(sizeof(GC) * fgca->num);
    for(int i = 0; i < fgca->num; i++) {
        fgca->gcs[i] = (GC){ p, fonts[i] };
        fgca->sizes[i] = text_height(fgca->gcs[i]);
    }
    return fgca;
}

void set_velocity(double d)
{
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: set_velocity(%f)\n", d);
#endif
    screen.velocity = d;
}

void add_velocity(double d)
{
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: add_velocity(%f)\n", d);
#endif
    screen.velocity += d / (fabs(screen.velocity)+1.0);
}

void brake(double d)
{
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: brake(%f)\n", d);
#endif
    double v = screen.velocity;
    if (0 < v) {
        v -= d * (fabs(screen.velocity)+1.0);
        if (v < 0) v = 0;
    } else {
        v += d * (fabs(screen.velocity)+1.0);
        if (0 < v) v = 0;
    }
    screen.velocity = v;
}

void move_cursor_to(double x, double y)
{
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: move_cursor_to(%f, %f)\n", x, y);
#endif
    screen.cursorx = x * screen.centerx;
    screen.cursory = y * screen.centery;
}

void move_cursor(double x, double y)
{
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: move_cursor(%f, %f)\n", x, y);
#endif
    if (x != 0 && y != 0) {
        screen.cursorx = x - screen.centerx;
        screen.cursory = y - screen.centery;
    } else {
        screen.cursorx += x * screen.centerx;
        screen.cursory += y * screen.centery;
    }
}

void toggle_ui(char* mode)
{
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: toggle_ui(%s)\n", mode);
#endif
    if (!strcasecmp("info", mode)) {
        screen.show_info = !screen.show_info;
    } else if (!strcasecmp("measure", mode)) {
        screen.show_measure = !screen.show_measure;
    } else if (!strcasecmp("label", mode)) {
        screen.show_label = !screen.show_label;
    }
}

/*
route* set_route(wormholeinfo* w)
{
    route* r = (route*) malloc(sizeof(route));
    if (r) {
        r->w = w;
        r->p = 0.0;
    }
    return r;
}
*/

void open_file()
{
#if DEBUG_EVENT
    fprintf(stderr, "MainScreen: open_file()\n");
#endif
    star* focus = screen.focus;
    if (focus && focus != &Universe) {
        char fname[MAX_CHARS], buff[MAX_CHARS];
        snprintf(fname, sizeof(fname), "%s%s", focus->info.parent->pathname, focus->info.basename);
        snprintf(buff, sizeof(buff), fname, fname);
        fprintf(stderr, "exec: \"%s\"\n", buff);
    }
}

void get_info(){ /* not implemented yet */ }
void autopilot(char* place){ /* not implemented yet */ }

void reposition()
{
    screen.state = STATE_REPOSITION;
}

void freeze()
{
    screen.freeze = !screen.freeze;
}

void flyingmode()
{
    screen.flyingmode = (screen.flyingmode == FLY_POLAR) ? FLY_CARTESIAN : FLY_POLAR;
}

#if DEBUG_DRAW
void debug()
{
    fprintf(stderr, "debug -------------------------------\n");
    dump_frame(&screen.view);
    if (screen.focus && screen.focus != &Universe) {
        debug_star(screen.focus);
    } else {
        debug_star((star*)screen.view.frames[screen.view.curidx].g);
    }
}
#endif

/* idle */
#define BOUND 0.8
#define ROT 0.3
void render()
{
    screen.width  = glutGet(GLUT_WINDOW_WIDTH);
    screen.height = glutGet(GLUT_WINDOW_HEIGHT);
    screen.centerx = screen.width / 2;
    screen.centery = screen.height / 2;

    int mx = screen.centerx * BOUND;
    int my = screen.centery * BOUND;
  
    setup_measure();

    if (screen.cursorx < -mx) screen.cursorx = -mx;
    else if (mx < screen.cursorx) screen.cursorx = mx;
    if (screen.cursory < -my) screen.cursory = -my;
    else if (my < screen.cursory) screen.cursory = my;

    if (!screen.freeze) {
        double dx, dy;
        switch(screen.state) {
        case STATE_AUTOPILOT:
            do_route(screen.activeRoute, &dx, &dy, &screen.velocity);
            break;
        case STATE_REPOSITION:
            if (0.99 < reposition_ship(&screen.view))
                screen.state = STATE_NORMAL;
            break;
        case STATE_NORMAL:
            dx = (double)screen.cursorx / (double)screen.centerx;
            dy = (double)screen.cursory / (double)screen.centery;
            double dl = sqrt(dx*dx + dy*dy) * ROT;
            dx = dx * dl * dl;
            dy = -dy * dl * dl;
            if (screen.flyingmode == FLY_POLAR) {
                move_ship_polar(&screen.view, screen.velocity, dx, dy, screen.pitchLimit);
            } else {
                move_ship_cartesian(&screen.view, screen.velocity, dx, dy);
            }
            break;
        }
    }

    View view = screen.view;
    double mat[16] = {
        view.dright.x, view.dupper.x, -view.dforward.x, 0,
        view.dright.y, view.dupper.y, -view.dforward.y, 0,
        view.dright.z, view.dupper.z, -view.dforward.z, 0,
        0,             0,              0,               1 };
    memcpy(screen.mat, mat, sizeof(double)*16);

    glClearColor(screen.gc_background.foreground.x, screen.gc_background.foreground.y, screen.gc_background.foreground.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glEnable(GL_DEPTH_TEST);
    draw_universe(&screen.view);
    glDisable(GL_DEPTH_TEST);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    // flip y-axis to adjust original XCruiser coordinates.
    gluOrtho2D(0, screen.width, screen.height, 0);
    draw_information(&screen.view, screen.state, screen.flyingmode);
    process_marked_stars();

    glPopMatrix();
    
#if DEBUG_EVENT
    fprintf(stderr, "screen: idle: (%d, %d)\n", screen.cursorx, screen.cursory);
#endif
}

void init_screen()
{
#if DEBUG_EVENT
    fprintf(stderr, "screen: initialize\n");
#endif
    screen.state = STATE_NORMAL;
    screen.flyingmode = FLY_POLAR;
    screen.freeze = false;
    screen.velocity = 0.0;
    screen.cursorx = screen.cursory = 0;
    init_view(&screen.view);
    screen.activeRoute = NULL;

    // default settings
    screen.show_measure = true;
    screen.show_label = true;
    screen.show_info = true;
    screen.cursorSize = 20;
    screen.pitchLimit = M_PI*70/180;
    screen.currentGalaxyLineWidth = 2;

    screen.gc_background = (GC){ bg,    NULL };
    screen.gc_measure    = (GC){ green, labelFonts[0] };
    screen.gc_info       = (GC){ green, labelFonts[1] };
    screen.gc_alert      = (GC){ red,   labelFonts[2] };
    screen.gc_cursor     = (GC){ white, NULL };
    
    // inner galaxy
    screen.gc_root[1]       = (GC){ blue, NULL };
    screen.gcl_galaxy[1][1] = getGCList(innerGalaxyColors, 2);
    screen.gcl_galaxy[1][0] = getGCList(innerUnreadableGalaxyColors, 1);
    screen.gcl_planet[1][1] = getGCList(innerPlanetColors, 4);
    screen.gcl_planet[1][0] = getGCList(innerUnreadablePlanetColors, 1);
    screen.gcl_wormhole[1]  = getGCList(innerWormholeColors, 1);
    screen.gca_label[1]     = getFontList(innerLabelColor, labelFonts, 3);

    // outer galaxy
    screen.gc_root[0]       = (GC){ blue, NULL };
    screen.gcl_galaxy[0][1] = getGCList(outerGalaxyColors, 2);
    screen.gcl_galaxy[0][0] = getGCList(outerUnreadableGalaxyColors, 1);
    screen.gcl_planet[0][1] = getGCList(outerPlanetColors, 4);
    screen.gcl_planet[0][0] = getGCList(outerUnreadablePlanetColors, 1);
    screen.gcl_wormhole[0]  = getGCList(outerWormholeColors, 1);
    screen.gca_label[0]     = getFontList(outerLabelColor, labelFonts, 3);
}
