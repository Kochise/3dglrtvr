/*
  XCruiser - a filesystem visualization utility
  vec3.c: vector manipulation

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

#include "xcruiser.h"

double vec3_length(vec3* p)
{
    return sqrt(p->x*p->x + p->y*p->y + p->z*p->z);
}

double vec3_distance(vec3* p1, vec3* p2)
{
    double dx = p1->x - p2->x;
    double dy = p1->y - p2->y;
    double dz = p1->z - p2->z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

// cross (outer) product
void vec3_cproduct(vec3* p, vec3* p1, vec3* p2)
{
    p->x = p1->y*p2->z - p2->y*p1->z;
    p->y = p2->x*p1->z - p1->x*p2->z;
    p->z = p1->x*p2->y - p2->x*p1->y;
}

// direct (inner) product
double vec3_dproduct(vec3* p1, vec3* p2)
{
    return (p1->x*p2->x + p1->y*p2->y + p1->z*p2->z);
}

void vec3_apply_matrix(vec3* p, vec3* v, vec3* x, vec3* y, vec3* z)
{
    vec3_set(p, 
        vec3_dproduct(x,v),
        vec3_dproduct(y,v),
        vec3_dproduct(z,v)
        );
}

void vec3_set(vec3* p, double x, double y, double z)
{
    p->x = x;
    p->y = y;
    p->z = z;
}

void vec3_add(vec3* p, double x, double y, double z)
{
    p->x += x;
    p->y += y;
    p->z += z;
}

void vec3_mul(vec3* p, double n)
{
    p->x *= n;
    p->y *= n;
    p->z *= n;
}

void vec3_normalize(vec3* p)
{
    double r = vec3_length(p);
    if (r == 0.0) {
        fprintf(stderr, "vec3_normalize: warning: zero vector\n");
        vec3_set(p, 0.0, 0.0, 1.0);
    } else {
        vec3_set(p, p->x / r, p->y / r, p->z / r);
    }
}

void vec3_orthogonal(vec3* p, vec3* q)
{
    p->x = q->z - q->y;
    p->y = q->x - q->z;
    p->z = q->y - q->x;
    vec3_normalize(p);
}

char* vec3_dbg(vec3* p)
{
    static char buff[256];
    sprintf(buff, "[%3.3f, %3.3f, %3.3f]", p->x, p->y, p->z);
    return buff;
}
