/*
  XCruiser - a filesystem visualization utility
  info.c: displaying information
    
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

/*
 *  info.c
 *     Draw informations on a window
 */
#include <stdarg.h>
#include "xcruiser.h"
#include "screen.h"

#define ALERT_BUFFER_LENGTH 1024

static int info1x, info1y, info1w;
static int info2x, info2y, info2w;
static bool alert = false;

static void draw_string(GC gc, int x, int y, char* s)
{
    glColor3f(gc.foreground.x,gc.foreground.y,gc.foreground.z);
    glRasterPos2i(x, y);
    glutBitmapString(gc.font, (const unsigned char*)s);
}

/* Show a message atop of the window */
void draw_alert(char* format, ...)
{
    if (!alert) {
        static char strbuff[ALERT_BUFFER_LENGTH];
        va_list args;

        va_start(args, format);
        /* should use vsnprintf */
        vsprintf(strbuff, format, args);
        va_end(args);
        draw_string(screen.gc_alert, screen.centerx - text_width(screen.gc_alert,strbuff)/2, screen.centery - text_height(screen.gc_alert)/2, strbuff);
        alert = true;
    }
}

/* Draw a cursor */
static void draw_cursor(void)
{
    segment seg[4];
    
    int c = screen.cursorSize;
    int cx = screen.cursorx + screen.centerx;
    int cy = screen.cursory + screen.centery;
    seg[0].y1 = seg[1].y1 = seg[0].y2 = seg[1].y2 = cy;
    seg[2].x1 = seg[3].x1 = seg[2].x2 = seg[3].x2 = cx;
    seg[0].x1 = cx - c;
    seg[0].x2 = cx - c*2;
    seg[1].x1 = cx + c;
    seg[1].x2 = cx + c*2;
    seg[2].y1 = cy - c;
    seg[2].y2 = cy - c*2;
    seg[3].y1 = cy + c;
    seg[3].y2 = cy + c*2;
    draw_segments(screen.gc_cursor, seg, 4);
}

/* Draw informations */
static void draw_info1(Frame* frame, int state, int flyingmode)
{
    static char s[MAX_CHARS + 100];
    star* focus = screen.focus;
    draw_string(screen.gc_info, info1x, info1y,    "Velocity:");
    draw_string(screen.gc_info, info1x, info1y+15, "Position:");
    draw_string(screen.gc_info, info1x, info1y+30, "Current Galaxy:");
    /* FLY_POLAR or FLY_CARTESIAN */
    sprintf(s, "%3.1f %c", screen.velocity, flyingmode == FLY_POLAR ? 'P' : 'C');
    draw_string(screen.gc_info, info1x+info1w, info1y, s);
    draw_string(screen.gc_info, info1x+info1w, info1y+15, vec3_dbg(&frame->viewp));
    draw_string(screen.gc_info, info1x+info1w, info1y+30, frame->g->pathname);
    if (focus && focus != &Universe) {
        switch(focus->info.type) {
        case ST_PLANET:
            sprintf(s, "> %s [%lu]", focus->info.basename, focus->p.size);
            break;
        case ST_GALAXY:
            if (focus->g.expanded)
                sprintf(s, "> %s/ (%d files)", focus->info.basename, focus->g.nstars);
            else
                sprintf(s, "> %s/", focus->info.basename);
            break;
        case ST_WORMHOLE:
            if (focus->w.nearest)
                sprintf(s, "> %s@ -> %s", focus->info.basename, focus->w.nearest->info.basename);
            else
                sprintf(s, "> %s@", focus->info.basename);
            break;
        }
        draw_string(screen.gc_info, info1x, info1y+60, s);
    }
    draw_string(screen.gc_info, info2x, info2y+15, "Button1 : Forward");
    draw_string(screen.gc_info, info2x, info2y+30, "Button2 : Open");
    draw_string(screen.gc_info, info2x, info2y+45, "Button3 : Backward");

    /* put details in title rather than over data
       We generate the new title, and only set when the details change from last time
    */
    /*
      sprintf(buff, "XCruiser - Velocity: %s  Current Galaxy: %s", screen.velocity, frame->g.pathname);
      XStoreName(disp, mywin, buff);
    */
}

/* Draw measures */
#define MeasureSmallUnit 3
#define MeasureSmallGrad 4
#define MeasureLargeUnit 15
#define MeasureLargeGrad 8
#define MeasureSmallUnitStep 16

#define floor(x,y) (((int)((x)/(y)))*(y))
#define ceil(x,y) floor((x)+(y)-1,y)

void setup_measure()
{
    int padding, measure_length;
    padding = screen.height / 20;
    measure_length = screen.height * 0.65;
    
    screen.measure_clip_h.width = measure_length;
    screen.measure_clip_h.y = padding + screen.centery + measure_length/2;

    screen.measure_clip_v.height = measure_length;
    screen.measure_clip_v.x = padding + screen.centerx + measure_length/2;
}

static void draw_measure(double rot1, double rot2)
{
    GC gc = screen.gc_measure;
    int x, y, deg_start, deg_end;
    char s[20];
    int range = screen.measure_clip_h.width * MeasureSmallUnit / MeasureSmallUnitStep / 2;
    rot1 = rot1*180/M_PI + 360;
    rot2 = rot2*180/M_PI + 360;
    
    /*
      smallunitstep = 8pix
      smallunit = 6deg
      largeunit = 30deg (largeunit should be multiple of smallunit!)

                rot=148deg
                     V
      [ . . | . . . . | . . . . | . .] (width=120pix,range=width*smallunit/smallunitstep=90deg)
           120       150       180
        ^                           ^
      deg_start                  deg_end

      deg_start=ceiling(148-(range/2)=103deg)=108deg
      deg_end=floor(142+(range/2)=182deg.)=180deg.
      x_start=(108deg-103deg)*smallunitstep/smallunit=6.7pix
    */
    deg_start = ceil(rot1-range, MeasureSmallUnit);
    deg_end = floor(rot1+range, MeasureSmallUnit);
    y = screen.measure_clip_h.y;
    x = screen.centerx - screen.measure_clip_h.width/2 + 
        (deg_start-(rot1-range)) * MeasureSmallUnitStep / MeasureSmallUnit;
    for(int i = deg_start; i <= deg_end; i += MeasureSmallUnit) {
        if (i % MeasureLargeUnit == 0) {
            sprintf(s, "%d", i%360);
            draw_string(gc, x-text_width(gc,s)/2, y+MeasureLargeUnit+text_height(gc)*.75, s);
            draw_line(gc, x, y, x, y+MeasureLargeGrad);
        } else {
            draw_line(gc, x, y, x, y+MeasureSmallGrad);
        }
        x += MeasureSmallUnitStep;
    }

    deg_start = ceil(rot2-range, MeasureSmallUnit);
    deg_end = floor(rot2+range, MeasureSmallUnit);
    x = screen.measure_clip_v.x;
    y = screen.centery + screen.measure_clip_v.height/2 -
        (deg_start-(rot2-range)) * MeasureSmallUnitStep / MeasureSmallUnit;
    for(int i = deg_start; i <= deg_end; i += MeasureSmallUnit) {
        if (i % MeasureLargeUnit == 0) {
            sprintf(s, "%d", i%360);
            draw_string(gc, x+MeasureLargeGrad+2, y+text_height(gc)*.25, s);
            draw_line(gc, x, y, x+MeasureLargeGrad, y);
        } else {
            draw_line(gc, x, y, x+MeasureSmallGrad, y);
        }
        y -= MeasureSmallUnitStep;
    }
}

static double angle(double x, double y)
{
    if (fabs(x) < fabs(y)) {
        double r = atan(x/y);
        return (0 < y) ? M_PI/2-r : M_PI/2*3-r;
    } else {
        double r = atan(y/x);
        return (0 < x) ? r : M_PI+r;
    }
}

void draw_information(View* vi, int state, int flyingmode)
{
    int w1 = text_width(screen.gc_info, "w");
    int h1 = text_height(screen.gc_info);
    info1x = info1y = 32;
    info1w = w1*16;
    info2x = screen.width - w1*20;
    info2y = screen.height - h1*4;
    info2w = w1*16;

    draw_cursor();
    if (screen.show_measure) {
        draw_measure(angle(vi->dforward.z, vi->dforward.x), angle(vi->dforward.z, vi->dforward.y));
    }
    if (screen.show_info) {
        draw_info1(&vi->frames[vi->curidx], state, flyingmode);
    }
    alert = false;
}
