/* GRS.cpp
 * Copyright (c) 1999 By James Beattie 
 * This file contains all the functions for loading the GRS (gross) 
 * graphics file format and drawing them in OpenGL.
 *
 * NOTE: GRS_Load creates a linked list of polylines. 
 * (see pline_t in GRS.h) Be sure to free complex GRS 
 * images before loading others, or the program won't 
 * have enough memory to open them.
 */
 
#include <gl/glut.h>
#include "grs.h"

int GRS_Load(grs_t * grs, char * path)
{
	FILE * filePtr = fopen(path, "r"); 
	
	if(!filePtr) return -1;
	GRS_Free(grs);
	grs->start = NULL;
	
	char line[256], tmp[256]; 
	int hdr = 0; 
	int lcount, scount;

	grs->bounds.l = +1e10;
	grs->bounds.b = +1e10;
	grs->bounds.r = -1e10;
	grs->bounds.t = -1e10;
	grs->linecount= 0;
	grs->polylinecount = 0;
	
	if(strrchr(path, '/')) strncpy(grs->filename, strrchr(path, '/')+1, 199);
	else strncpy(grs->filename, path, 199);
	
	while(fscanf(filePtr, "%[^\n]\n", line) == 1)
	{
		// Have we read passed the header yet?
		if(!hdr && sscanf(line, "*%s", tmp) == 1) 
		{	
			while(fscanf(filePtr, "%[^\n]\n", line) == 1)
			{
				float l, t, r, b;
				if(sscanf(line, "%f%f%f%f", &l, &t, &r, &b) > 0)
				{
					grs->bounds.l = l; 
					grs->bounds.t = t; 
					grs->bounds.r = r; 
					grs->bounds.b = b; 
					break;
				}
			}
			while(fscanf(filePtr, "%[^\n]\n", line) == 1)
			{
				if(sscanf(line, "%d\n", &lcount) == 1)
				{
					grs->line_count = lcount;
					hdr = 1;
					break;
				}
			}
		}
		
		if(hdr)
		{
			int i, j;
			pline_t * prev = NULL;
			for(i = 0; i < lcount; i++)
			{
				pline_t * p = (pline_t *)malloc(sizeof(pline_t));
				if(!p) return -2;
				
				if(grs->start == NULL) 
				{
					grs->start = p;
				}
				
				if(prev != NULL)
				{
					prev->next = p;
				}
				
				if(i == lcount - 1) p->next = NULL; 

				if(fscanf(filePtr, "%d\n", &scount) == 1)
				{
					p->seg_count = scount;	
					p->seg_list = (point_t *)malloc(p->seg_count * sizeof(point_t));
					if(!p->seg_list)
					{
						return -3;
					}
					grs->linecount += scount-1;
					grs->polylinecount++;
					for(j = 0; j < scount; j++)
					{
						float x, y;
						if(fscanf(filePtr, "%f%f\n", &x, &y))
						{
							if(grs->bounds.l > x) grs->bounds.l = x;
							if(grs->bounds.r < x) grs->bounds.r = x;
							if(grs->bounds.b > y) grs->bounds.b = y;
							if(grs->bounds.t < y) grs->bounds.t = y;
							
							p->seg_list[j].x = x; 
							p->seg_list[j].y = y;
								
						}
						else 
						{
							return -4;
						}
					}
					prev = p;
				}
				else
				{
					return -5;
				}
			}
		}
	}
	fclose(filePtr);
	return 1;
}

void GRS_Draw(grs_t * grs)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	// so sloppy.. for now..
	
#define ICON_BAR 32 // icon bar width
#define STAT_BAR 56 // status bar height
	int vport[4];
	glGetIntegerv(GL_VIEWPORT, vport);
	float aspect = (float)vport[2] / (float)vport[3];
	float grs_aspect = (grs->bounds.t - grs->bounds.b)/
	                   (grs->bounds.r - grs->bounds.l);
	float scr_aspect = (float)(vport[3]-STAT_BAR) / (float)(vport[2] - ICON_BAR);
	
	if(grs_aspect > scr_aspect)
	glScalef(2.f/(grs->bounds.t - grs->bounds.b),
	         2.f/(grs->bounds.t - grs->bounds.b), 1.f); else
	glScalef(2.f/(grs->bounds.r - grs->bounds.l)*aspect,
	         2.f/(grs->bounds.r - grs->bounds.l)*aspect, 1.f);
	glTranslatef(-(grs->bounds.l + grs->bounds.r)/2.f,
	             -(grs->bounds.t + grs->bounds.b)/2.f, 0.f);
	int i;
	for(pline_t * p = grs->start; p; p = p->next)
	{
		glBegin(GL_LINE_STRIP);
		for(i = 0; i < p->seg_count; i++)
		{
			glVertex2f(p->seg_list[i].x, p->seg_list[i].y);
		}
		glEnd();
	}
	glPopMatrix();
}

void GRS_Free(grs_t * grs)
{
	for(pline_t *pnext, *p = grs->start; p; p = pnext)
	{
		pnext = p->next;
		free(p); 
	}
	grs->start = NULL;
	grs->line_count = 0; 
}
