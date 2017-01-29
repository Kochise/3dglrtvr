#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gl/glut.h>
#include "zoom.h"
#include "pack.h"

#define _min3(a,b,c) ((a)<(b)?((a)<(c)?(a):(c)):((b)<(c)?(b):(c)))
#define _max3(a,b,c) ((a)>(b)?((a)>(c)?(a):(c)):((b)>(c)?(b):(c)))
#define PI2 6.28318530717958647692528676655901 // Pi times 2

static packinfo_t *INFO;

static zoom_t *Z;
static int     SHADOW_MODE;
static double  VIEW_X;
static double  VIEW_Y;
static double  VIEW_ASPECT;

static pack_t *PACK[2] = {NULL, NULL};
static pack_t A = {0.0, +0.0, -1.0};
static pack_t B = {0.0, +1.0, +2.0};
static pack_t C = {0.0, -1.0, +2.0};
static pack_t *SEARCH_RESULT;

void PACK_Initialize(packinfo_t *packinfo)
{
	INFO = packinfo;
	INFO->obj_total = 3;
	
	for(int i = 0; i < 3; i++)
	{
		A.child[i] = NULL, A.depth = -1;
		B.child[i] = NULL, B.depth = 0;
		C.child[i] = NULL, C.depth = 0;
	}
}

void PACK_SetView(double view_x, double view_y)
{
	VIEW_X = view_x;
	VIEW_Y = view_y;
	VIEW_ASPECT = view_x / view_y;
}

pack_t *PACK_Create(pack_t *a, pack_t *b, pack_t *c, int depth)
{
	if(c->z > INFO->max_render_z*Z->z) return NULL;
	if(c->z < 0.0) return NULL; //FIXME: really necessary?
	
	double ax = a->x, ay = a->y, az = a->z;
	double bx = b->x, by = b->y, bz = b->z;
	double cx = c->x, cy = c->y, cz = c->z;
 	
	// ****************************************************
	// Compute next circle (this is a 7th-order equation!!)
	// ****************************************************
	
	double dz = az + bz + cz + 2.0*sqrt(az*bz + bz*cz + cz*az);
	double vx = ax*bz - az*bx;
	double vy = ay*bz - az*by;
	double mx = az*(ax*(1.0 - bx*bx + by*by) - 2.0*ay*(bx*by)) +
	            bz*(bx*(1.0 - ax*ax + ay*ay) - 2.0*by*(ax*ay)) ;
	double my = az*(ay*(1.0 + bx*bx - by*by) - 2.0*ax*(bx*by)) +
	            bz*(by*(1.0 + ax*ax - ay*ay) - 2.0*bx*(ax*ay)) ;
	double ex = mx*dz + 2.0*vx*(az-bz);
	double ey = my*dz + 2.0*vy*(az-bz);
	double fx = az*az*az*bx*(1.0 - bx*bx - by*by) +
	            bz*bz*bz*ax*(1.0 - ax*ax - ay*ay) ;
	double fy = az*az*az*by*(1.0 - bx*bx - by*by) +
	            bz*bz*bz*ay*(1.0 - ax*ax - ay*ay) ;
	double nx = az*bz*ex - dz*fx;
	double ny = az*bz*ey - dz*fy;
	double s  = 4.0*az*bz*sqrt(az*bz + bz*dz + dz*az);
	double nz = 2.0*az*bz*(az+bz)*(az+bz);
	double dx = (nx - vy*s)/nz;
	double dy = (ny + vx*s)/nz;
	
	pack_t *d = (pack_t*)malloc(sizeof(pack_t));
	
	d->x = dx;
	d->y = dy;
	d->z = dz;
	d->parent[0] = a;
	d->parent[1] = b;
	d->parent[2] = c;
	d->child[0] = PACK_Create(a, b, d, depth+1);
	d->child[1] = PACK_Create(b, c, d, depth+1);
	d->child[2] = PACK_Create(c, a, d, depth+1);
	d->depth = depth;
	d->error = (fabs(dx-floor(dx)) > 0.1 ||
	            fabs(dy-floor(dy)) > 0.1 ||
				fabs(dz-floor(dz)) > 0.1 ) ? 1 : 0;
	
	INFO->obj_created++;
	INFO->obj_total++;
	
	// *************************
	// Compute bounding triangle
	// *************************
	
	double abx = (ax+bx)/(az+bz);
	double aby = (ay+by)/(az+bz);
	double bcx = (bx+cx)/(bz+cz);
	double bcy = (by+cy)/(bz+cz);
	double cax = (cx+ax)/(cz+az);
	double cay = (cy+ay)/(cz+az);
	
	// ********************
	// Compute bounding box
	// ********************
	
	d->box[0] = _min3(abx, bcx, cax);
	d->box[1] = _min3(aby, bcy, cay);
	d->box[2] = _max3(abx, bcx, cax);
	d->box[3] = _max3(aby, bcy, cay);
	return d;
}

static void DrawText(char *s)
{
	while(*s != '\0')
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)(*s++));
	}
}

static void DrawCircleVector1(pack_t *pack, double r)
{
	char s[10];

	if(INFO->show_depth) sprintf(s, "%i", (int)pack->depth);
	else                 sprintf(s, "%i", (int)pack->z);
	
	double len = (double)strlen(s);
	
	if(r*VIEW_Y*Z->z > 12.0*len)
	{
		double x = r*pack->x;
		double y = r*pack->y;
		
		glRasterPos2d(x -  8.0/(VIEW_Y*Z->z)*len,
					  y - 13.0/(VIEW_Y*Z->z));
		
		DrawText(s);
	}
}

static void DrawCircleVector3(pack_t *pack, double r)
{
	char s1[10];
	char s2[10];
	char s3[10];
	sprintf(s1, "%i", (int)pack->x);
	sprintf(s2, "%i", (int)pack->y);
	sprintf(s3, "%i", (int)pack->z);
	double len1 = (double)strlen(s1);
	double len2 = (double)strlen(s2);
	double len3 = (double)strlen(s3);
	double len = _max3(len1, len2, len3);
	
	if(len1 + len2 + len3 > 4.0)
	{
		if(r*VIEW_Y*Z->z > 15.0*len)
		{
			double x = r*pack->x;
			double y = r*pack->y;
			
			double x_loc = x -  8.0/(VIEW_Y*Z->z)*len;
			double y_loc = y - 39.0/(VIEW_Y*Z->z);
			double dy    =     26.0/(VIEW_Y*Z->z);
			
			glRasterPos2d(x_loc + 16.0*(len-len3)/(VIEW_Y*Z->z), y_loc);
			DrawText(s3); y_loc += dy;
			glRasterPos2d(x_loc + 16.0*(len-len2)/(VIEW_Y*Z->z), y_loc); 
			DrawText(s2); y_loc += dy;
			glRasterPos2d(x_loc + 16.0*(len-len1)/(VIEW_Y*Z->z), y_loc);
			DrawText(s1);
		}
	}
	else
	{
		char s4[40];
		sprintf(s4, "%s,%s,%s", s1, s2, s3);
		
		len = (double)strlen(s4);
		
		if(r*VIEW_Y*Z->z > 12.0*len)
		{	
			double x = r*pack->x;
			double y = r*pack->y;
			
			glRasterPos2d(x -  8.0/(VIEW_Y*Z->z)*len,
						  y - 13.0/(VIEW_Y*Z->z));
			
			DrawText(s4);
		}
	}
}

void PACK_DrawCircle(pack_t *pack)
{
	double r = 1.0/fabs(pack->z);
	double x = r*pack->x;
	double y = r*pack->y;
	
	// **************
	// Trivial reject
	// **************
	
	if((y - r + Z->y)*Z->z > +1.0) return;
	if((y + r + Z->y)*Z->z < -1.0) return;
	if((x - r + Z->x)*Z->z > +VIEW_ASPECT) return;
	if((x + r + Z->x)*Z->z < -VIEW_ASPECT) return;
	
	double q = 1.0 - pow(r*INFO->max_render_z*Z->z, INFO->fade_exponent);
	
	if(!SHADOW_MODE && pack->z > 0.0)
	{
		if(pack->error) glColor3d(0.7, 0.3, 0.3);
		else            glColor3d(0.0, 1.0, 1.0);
		
		if(INFO->show_vec_3)
		{
			DrawCircleVector3(pack, r);
		}
		else
		{
			DrawCircleVector1(pack, r);
		}
	}
	if(pack->error)
	{
		if(SHADOW_MODE) glColor3d(0.3, 0.0, 0.0);
		else            glColor3d(1.0, 0.0, 0.0);
	}
	else
	{
		//NOTE: in a dark room: use q*0.1, q*0.3
		if(SHADOW_MODE) glColor3d(q*0.0, q*0.3, q*0.6);
		else            glColor3d(q*0.2, q*0.2, q*1.0);
	}
	// *******************************************************
	// Compute dTHETA based on LOD heuristic: radial deviation
	// *******************************************************
	
	double err    = fabs(pack->z)/(INFO->lod_constant*Z->z);
	double dtheta = acos(err>1.0?-1.0 : 1.0 - 4.0*err + 2.0*err*err);
	
	// ***************************************
	// Draw circle (this should be tight code)
	// ***************************************
	
	double xx = r, x2;
	double yy = 0.0;
	double c  = cos(dtheta);
	double s  = sin(dtheta);
	
	glBegin(GL_LINE_STRIP);
	
	for(double theta = 0.0; theta <= PI2; theta += dtheta)
	{
		glVertex2d(x + xx, y + yy);
		
		x2 = xx*c - yy*s;
		yy = xx*s + yy*c;
		xx = x2;
	}
	glVertex2d(x + r, y);
	glEnd();
	
	if(pack == SEARCH_RESULT)
	{
		// **********************************
		// Draw lines to children and parents
		// **********************************
		
		glBegin(GL_LINES);
		
		for(int i = 0; i < 3; i++)
		{
			if(pack->parent[i])
			{
				double pr = 1.0/pack->parent[i]->z;
				double px = pr*pack->parent[i]->x;
				double py = pr*pack->parent[i]->y;
				
				glColor3d(1.0, 1.0, 1.0); glVertex2d(x, y);
				glColor3d(0.0, 0.0, 0.0); glVertex2d(px, py);
			}
			if(pack->child[i])
			{
				double cr = 1.0/pack->child[i]->z;
				double cx = cr*pack->child[i]->x;
				double cy = cr*pack->child[i]->y;
				
				glColor3d(0.0, 1.0, 0.0); glVertex2d(x, y);
				glColor3d(0.0, 0.0, 0.0); glVertex2d(cx, cy);
			}
		}
		glEnd();
	}
	INFO->obj_in_view++;
}

void PACK_Draw(pack_t *pack, pack_t *up)
{
	if(pack)
	{
		if(pack->depth > 1)
		{
			// *************************************
			// Trivial reject if circle is too small
			// *************************************
			
			if(pack->z > INFO->max_render_z*Z->z)
			{
				//don't count as node rejection
				return;
			}
			// ************************************
			// Trivial reject if box is out of view
			// ************************************
			
			double lft = (pack->box[0] + Z->x)*Z->z;
			double top = (pack->box[1] + Z->y)*Z->z;
			double rgt = (pack->box[2] + Z->x)*Z->z;
			double bot = (pack->box[3] + Z->y)*Z->z;
			
			if(lft > +VIEW_ASPECT || top > +1.0 ||
			   rgt < -VIEW_ASPECT || bot < -1.0 )
			{
				INFO->node_reject++;
				return;
			}
		}
		// ***********************
		// Draw circle and recurse
		// ***********************
		
		PACK_DrawCircle(pack);
		PACK_Draw(pack->child[0], pack);
		PACK_Draw(pack->child[1], pack);
		PACK_Draw(pack->child[2], pack);
	}
	else if(up)
	{
		if(!INFO->dynamic) return;
		if(!INFO->dynamic_shadow && INFO->shadow_mode) return;
		
		// ***********************************
		// Dynamic generation of more TSP data
		// ***********************************
		
		for(int i = 0; i < 3; i++)
		{
			if(up->child[i]) continue; // already done it
			
			pack_t *a = up->parent[i];
			pack_t *b = up->parent[i<2?i+1:0];
			
			up->child[i] = PACK_Create(up, a, b, up->depth+1);
		}
	}
}

void PACK_Draw(int shadow_mode, zoom_t *zoom)
{
	SHADOW_MODE = shadow_mode, Z = zoom;
	
	// **********************
	// Root of circle packing
	// **********************
	
	INFO->obj_created = 0;
	INFO->obj_in_view = 0;
	INFO->node_reject = 0;
	
	// ********************************
	// Initialize packing, if necessary
	// ********************************
	
	if(!PACK[0]) PACK[0] = PACK_Create(&A, &B, &C, 1);
	if(!PACK[1]) PACK[1] = PACK_Create(&A, &C, &B, 1);
	
	// **************
	// Render packing
	// **************
	
	PACK_DrawCircle(&A);
	PACK_DrawCircle(&B);
	PACK_DrawCircle(&C);
	PACK_Draw(PACK[0], NULL);
	PACK_Draw(PACK[1], NULL);
}

int PACK_Search(double x, double y, pack_t *pack)
{
	if(!pack) return 0;
	
	if(pack->depth > 0)
	{
		double lft = (pack->box[0] + Z->x)*Z->z;
		double top = (pack->box[1] + Z->y)*Z->z;
		double rgt = (pack->box[2] + Z->x)*Z->z;
		double bot = (pack->box[3] + Z->y)*Z->z;
		
		if(lft > x || top > y || rgt < x || bot < y )
		{
			return 0;
		}
		double r0 = Z->z/pack->z;
		double x0 = pack->x*r0 + Z->x*Z->z;
		double y0 = pack->y*r0 + Z->y*Z->z;
		
		if((x-x0)*(x-x0) + (y-y0)*(y-y0) <= r0*r0)
		{
			SEARCH_RESULT = pack;
			return 1;
		}
	}
	if(PACK_Search(x, y, pack->child[0])) return 1;
	if(PACK_Search(x, y, pack->child[1])) return 1;
	if(PACK_Search(x, y, pack->child[2])) return 1;
	return 0;
}

pack_t *PACK_Search(double x, double y, int show)
{
	SEARCH_RESULT = NULL;
	
	if(show)
	{
		double xd = +(2.0*(double)x/VIEW_X - 1.0)*VIEW_ASPECT;
		double yd = -(2.0*(double)y/VIEW_Y - 1.0);
		
		if(PACK_Search(xd, yd, PACK[0])) return SEARCH_RESULT;
		if(PACK_Search(xd, yd, PACK[1])) return SEARCH_RESULT;
	}
	return NULL;
}

void PACK_ClearTSPMemory(pack_t *pack)
{
	if(!pack) return;
	
	for(int i = 0; i < 3; i++)
	{
		PACK_ClearTSPMemory(pack->child[i]);
	}
	free(pack);
	INFO->obj_total--;
}

void PACK_ClearTSPMemory(void)
{
	PACK_ClearTSPMemory(PACK[0]);
	PACK_ClearTSPMemory(PACK[1]);
	PACK[0] = NULL;
	PACK[1] = NULL;
	
	if(INFO->obj_total != 0)
	{
		printf("what???");
	}
}
