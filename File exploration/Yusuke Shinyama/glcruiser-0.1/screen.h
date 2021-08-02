/*
  XCruiser - a filesystem visualization utility
  screen.h: the header file for main screen structure

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

#ifndef _SCREEN_H
#define _SCREEN_H

#include "xcruiser.h"

/* main screen structure */
struct _screen
{
    int width, height;

    bool show_info, show_measure, show_label;

    int currentGalaxyLineWidth;
    int cursorSize;
    int pitchLimit;

    /* graphics context emulation */
    GC gc_background, gc_info, gc_alert, gc_measure, gc_cursor;
    GC gc_root[2];

    GCList* gcl_galaxy[2][2]; // [activated][permitted]
    GCList* gcl_planet[2][2]; // [activated][permitted]
    GCList* gcl_wormhole[2];  // [activated]

    FontList* gca_label[2];// [activated]
    
    int centerx, centery;
    rectangle measure_clip_h, measure_clip_v;
    
    int state;
    int flyingmode;
    bool freeze;
    double velocity;
    int cursorx, cursory;
    View view;
    star* focus;
 
    route* activeRoute;

    double mat[16];
} screen;

#endif /* _SCREEN_H */
