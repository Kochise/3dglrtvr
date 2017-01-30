/*
  $Id: info.c,v 1.1.2.2 2003/01/26 18:29:18 euske Exp $

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
 *	 Draw informations on a window
 */
#include <stdarg.h>
#include "xcruiser.h"
#include "MainScreeP.h"

#define ALERT_BUFFER_LENGTH 1024

static	Display*	disp;
static	Drawable	buff;
static	MainScreenPart*	scr;
static	int		info1x, info1y, info1w;
static	int		info2x, info2y, info2w;
static	int		alert = 0;


/*  Show a message atop of the window
 */
void draw_alert(MainScreenWidget me, char* format, ...)
{
    Display* d = XtDisplay((Widget)me);
    MainScreenPart* scr = &me->mainScreen;
    if (!alert) {
	static char strbuff[ALERT_BUFFER_LENGTH];
	va_list args;
	int y, len = 0, line = 0;
	char c, *s, *s0;
	if (!me) return;
	va_start(args, format);
	/* should use vsnprintf */
	(void) vsprintf(strbuff, format, args);
	va_end(args);
	s0 = strbuff;
	for(s = s0; (c = *s++); )
	    if (c == '\n') line++;
	y = scr->centery - line * 20;
	for(s = s0; (c = *s++); ) {
	    if (c == '\n') {
		XDrawString(d, XtWindow(me), scr->gc_alert,
			    scr->centerx - XTextWidth(scr->alertFont, s0, len)/2,
			    y, s0, len);
		y += 40;
		len = 0;
		s0 = s;
	    } else {
		len++;
	    }
	}
	XFlush(disp);
	alert = 1;
    }
}


/*  Draw a cursor
 */
static void draw_cursor(void)
{
    XSegment seg[4];
    
    int c = scr->cursorSize;
    int cx = scr->cursorx + scr->centerx;
    int cy = scr->cursory + scr->centery;
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
    XDrawSegments(disp, scr->screen_buff, scr->gc_cursor, seg, 4);
}

/*  Draw informations
 */
static void draw_string(int x, int y, char* s) {
    XDrawString(disp, buff, scr->gc_info, x, y, s, strlen(s));
}
static void draw_info1(frame* frm, int state, int flyingmode)
{
    static char s[MAX_CHARS + 100];
    star* focus = scr->focus;
    draw_string(info1x, info1y,    "Velocity:");
    draw_string(info1x, info1y+30, "Current Galaxy:");
    if (flyingmode == FLY_POLAR) {
	/* FLY_POLAR */
	sprintf(s, "%3.1f P", scr->velocity);
    } else {
	/* FLY_CARTESIAN */
	sprintf(s, "%3.1f C", scr->velocity);
    }
    draw_string(info1x+info1w, info1y, s);
    draw_string(info1x+info1w, info1y+30, frm->g->pathname);
    if (focus && focus != &Universe) {
	switch(focus->i.type) {
	case ST_PLANET:
	    sprintf(s, "> %s [%lu]", 
		    focus->i.basename, focus->p.size);
	    break;
	case ST_GALAXY:
	    if (focus->g.expanded)
		sprintf(s, "> %s/ (%d files)",
			focus->i.basename, focus->g.nstars);
	    else
		sprintf(s, "> %s/", focus->i.basename);
	    break;
	case ST_WORMHOLE:
	    if (focus->w.nearest)
		sprintf(s, "> %s@ -> %s",
			focus->i.basename, focus->w.nearest->i.basename);
	    else
		sprintf(s, "> %s@", focus->i.basename);
	    break;
	}
	draw_string(info1x, info1y+60, s);
    }
    /*
      draw_string(info2x, info2y+30, "Button2 : Open");
    */
    draw_string(info2x, info2y+30, "Button1 : Forward");
    draw_string(info2x, info2y+60, "Button3 : Backward");

    /* put details in title rather than over data
       We generate the new title, and only set when
       the details change from last time
    */
    /*
      sprintf(buff, "XCruiser - Velocity: %s  Current Galaxy: %s",
      scr->velocity, frm->g.pathname);
      XStoreName(disp, mywin, buff);
    */
}

/*  Draw measures
 */
#define MeasureSmallUnit 3
#define MeasureSmallGrad 4
#define MeasureLargeUnit 15
#define MeasureLargeGrad 8
#define MeasureSmallUnitStep 16

#define floor(x,y) (((int)((x)/(y)))*(y))
#define ceil(x,y) floor((x)+(y)-1,y)

void setup_measure(MainScreenWidget me) {
    int padding, measure_length, measure_width;
    scr = &me->mainScreen;
    padding = me->core.width / 20;
    measure_length = me->core.width / 3;
    measure_width = me->core.width / 10;
    
    scr->measure_clip_h.width = measure_length;
    scr->measure_clip_h.height = measure_width;
    scr->measure_clip_h.x = scr->centerx - measure_length/2;
    scr->measure_clip_h.y = padding + scr->centery + measure_length/2;

    scr->measure_clip_v.width = measure_width;
    scr->measure_clip_v.height = measure_length;
    scr->measure_clip_v.x = padding + scr->centerx + measure_length/2;
    scr->measure_clip_v.y = scr->centery - measure_length/2;
}

static void draw_string_centered(int x, int y, char* s) {
    int len = strlen(s);
    x -= XTextWidth(scr->measureFont, s, len)/2;
    y += scr->measureFont->ascent;
    XDrawString(disp, buff, scr->gc_measure, x, y, s, len);
}
static void draw_measure(int rot1, int rot2)
{
    int i, x, y, deg_start, deg_end;
    char s[20];
    GC gc = scr->gc_measure;
    XFontStruct* fn = scr->measureFont;
    int range = scr->measure_clip_h.width * MeasureSmallUnit / MeasureSmallUnitStep / 2;
    rot1 += 360;
    rot2 += 360;
    
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
    XSetClipRectangles(disp, gc, 0, 0, &scr->measure_clip_h, 1, YXBanded);
    deg_start = ceil(rot1-range, MeasureSmallUnit);
    deg_end = floor(rot1+range, MeasureSmallUnit);
    y = scr->measure_clip_h.y;
    x = scr->centerx - scr->measure_clip_h.width/2 + 
	(deg_start-(rot1-range)) * MeasureSmallUnitStep / MeasureSmallUnit;
    for(i = deg_start; i <= deg_end; i += MeasureSmallUnit) {
	if (i % MeasureLargeUnit == 0) {
	    sprintf(s, "%d", i%360);
	    draw_string_centered(x, y+MeasureLargeGrad+2, s);
	    XDrawLine(disp, buff, gc, x, y, x, y+MeasureLargeGrad);
	} else {
	    XDrawLine(disp, buff, gc, x, y, x, y+MeasureSmallGrad);
	}
	x += MeasureSmallUnitStep;
    }

    XSetClipRectangles(disp, gc, 0, 0, &scr->measure_clip_v, 1, YXBanded);
    deg_start = ceil(rot2-range, MeasureSmallUnit);
    deg_end = floor(rot2+range, MeasureSmallUnit);
    x = scr->measure_clip_v.x;
    y = scr->centery + scr->measure_clip_v.height/2 -
	(deg_start-(rot2-range)) * MeasureSmallUnitStep / MeasureSmallUnit;
    for(i = deg_start; i <= deg_end; i += MeasureSmallUnit) {
	if (i % MeasureLargeUnit == 0) {
	    sprintf(s, "%d", i%360);
	    XDrawString(disp, buff, gc,	x+MeasureLargeGrad+2, y+fn->ascent/2, s, strlen(s));
	    XDrawLine(disp, buff, gc, x, y, x+MeasureLargeGrad, y);
	} else {
	    XDrawLine(disp, buff, gc, x, y, x+MeasureSmallGrad, y);
	}
	y -= MeasureSmallUnitStep;
    }
}

static int angle(double x, double y)
{
    if (fabs(x) < fabs(y)) {
	int r = rad2deg(atan(x/y));
	return (0 < y)? 90-r : 270-r;
    } else {
	int r = rad2deg(atan(y/x));
	return (0 < x)? r : 180+r;
    }
}
void draw_information(MainScreenWidget me, view* vi, int state, int flyingmode)
{
    int w1, h1;
    disp = XtDisplay((Widget)me);
    scr = &me->mainScreen;
    buff = scr->screen_buff;

    w1 = scr->infoFont->max_bounds.width;
    h1 = scr->infoFont->max_bounds.ascent + scr->infoFont->max_bounds.descent;
    info1x = 32;
    info1y = 32;
    info1w = w1*16;
    info2x = me->core.width - w1*20;
    info2y = me->core.height - h1*4;
    info2w = w1*16;

    draw_cursor();
    if (me->mainScreen.show_measure) {
	draw_measure(angle(vi->dforward.z, vi->dforward.x),
		     angle(vi->dforward.z, vi->dforward.y));
    }
    if (me->mainScreen.show_info) {
	draw_info1(&vi->frms[vi->curidx], state, flyingmode);
    }
    alert = 0;
}
