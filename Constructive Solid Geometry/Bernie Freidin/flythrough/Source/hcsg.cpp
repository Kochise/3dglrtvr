// **************************************************************************
// HCSG Demo
// (c) Bernie Freidin, 1999-2000
// **************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "hcsg.h"
#ifdef __MAC__
#include <Windows.h>
#endif

static vert_t *CSG_VERT_POOL  = NULL;
static surf_t *CSG_SURF_POOL  = NULL;
static ccof_t *CSG_CCOF_POOL  = NULL;
static int     CSG_VERT_COUNT = 0;
static int     CSG_SURF_COUNT = 0;
static int     CSG_CCOF_COUNT = 0;
static ccof_t *CSG_CCOF[CSG_MAX_DEPTH+1];
static int     CSG_POLYGONALIZATION_DEPTH = 1;
static hull_t *CSG_WORLD = NULL;

static int     CSG_CODE_OUT;
static int     CSG_CODE_IN;

static void   *CSG_CURRENT_SHAPE;

// Debugging stuff. NOTE: We can't put the function name stack
// on the heap using our standard CSG_AllocMem command, since
// this command itself uses the function name stack!

#define CSG_MAX_FUNC_DEPTH 100
static FILE   *CSG_LOGFILE = NULL;
static int     CSG_ERRORS = 0;
static int     CSG_FUNC_DEPTH = 0;
static char    CSG_FUNC_NAME_STACK[CSG_MAX_FUNC_DEPTH][61] = {""};

static int     CSG_MEMORY_USED  = 0;
static int     CSG_MEMORY_CHECK = 0;

#define CSG_RESOLVE_KEEP_NEW 1
#define CSG_RESOLVE_KEEP_OLD 2

// *******************************************************************
// FIXME: For now, we are forced to use KEEP_OLD for RESOLVE_ADD_3D,
// because otherwise CSG_BooleanSub(HULL) will be invoked, which
// requires a different polygonalization function than CSG_BooleanAdd,
// which we have not yet written..
// *******************************************************************

static int CSG_RESOLVE_ADD_2D = CSG_RESOLVE_KEEP_NEW;
static int CSG_RESOLVE_ADD_3D = CSG_RESOLVE_KEEP_OLD;

// ==============================================================
// HCSG: Hierarchial Constructive Solid Geometry Data Structures:
// 
// Vertices and surface planes are stored in global pools.
// There are two basic building blocks that describe geometry,
// the POLYGON and the HULL. Polygons contain lists of vertex
// references and a surface reference, as well as a list of
// surface references which indicate the cutting planes forming
// the polygon. Hulls also contain a list of vertex references,
// and a linked list of polygons (NEXT).
// 
// In addition, both polygons and hulls have sub-lists (DOWN).
// An entity in a sub-list is always contained entirely within
// the parent entity.
// ==============================================================

void CSG_Initialize(void)
{
	CSG_PUSHNAME("Initialize");
	
	// **********************
	// Initialize HCSG engine
	// **********************
	
	CSG_VERT_POOL = (vert_t*)CSG_AllocMem(sizeof(vert_t), CSG_MAX_VERTS);
	CSG_SURF_POOL = (surf_t*)CSG_AllocMem(sizeof(surf_t), CSG_MAX_SURFS);
	CSG_CCOF_POOL = (ccof_t*)CSG_AllocMem(sizeof(ccof_t), CSG_MAX_CCOFS);
	CSG_VERT_COUNT = 0;
	CSG_SURF_COUNT = 0;
	CSG_CCOF_COUNT = 0;
	CSG_MEMORY_CHECK = 1;
	CSG_POPNAME_;
}

hull_t *CSG_World(void)
{
	CSG_PUSHNAME("World");
	if(CSG_WORLD) CSG_Free(CSG_WORLD);
	CSG_WORLD = CSG_SolidCube(-1000.0, -1000.0, -1000.0,
	                          +1000.0, +1000.0, +1000.0);
	CSG_WORLD->depth = 0;
	CSG_POPNAME(CSG_WORLD);
}

vert_t *CSG_GetVertPool(void)
{
	return CSG_VERT_POOL;
}

surf_t *CSG_GetSurfPool(void)
{
	return CSG_SURF_POOL;
}

int CSG_GetMemInUse_Modeler(void); // prototype
int CSG_GetMemInUse(void *obj)
{
	CSG_PUSHNAME("GetMemInUse");
	
	// **************************************************
	// Determines the amount of memory used by an object.
	// This works for POLY and HULL objects. If NULL is
	// specified, then the function returns the amount of
	// global memory used by the HCSG engine including
	// all pools, etc. If -1 is specified, the function
	// returns the amount of global memory used by the
	// pools (VERT, SURF, CCOF) only.
	// **************************************************
	
	int mem;
	
	if(!obj)
	{
		mem = CSG_MEMORY_USED;
	}
	else if(obj == (void*)-1)
	{
		mem  = CSG_GetMemInUse_Modeler();
		mem += sizeof(vert_t) * CSG_MAX_VERTS;
		mem += sizeof(surf_t) * CSG_MAX_SURFS;
		mem += sizeof(ccof_t) * CSG_MAX_CCOFS;
	}
	else
	{
		mem = 0;
		
		if(*(int*)obj == CSG_TYPE_POLY)
		{
			poly_t *poly = (poly_t*)obj;
			
			for(poly_t *p = poly->down; p; p = p->next)
			{
				mem += CSG_GetMemInUse(p);
			}
			mem += sizeof(vref_t) * poly->vertcount;
			mem += sizeof(sref_t) * poly->vertcount;
			mem += sizeof(poly_t);
		}
		else if(*(int*)obj == CSG_TYPE_HULL)
		{
			hull_t *hull = (hull_t*)obj;
			
			for(poly_t *p = hull->face; p; p = p->next)
			{
				mem += CSG_GetMemInUse(p);
			}
			for(hull_t *h = hull->down; h; h = h->next)
			{
				mem += CSG_GetMemInUse(h);
			}
			mem += sizeof(vref_t) * hull->vertcount;
			mem += sizeof(hull_t);
		}
	}
	CSG_POPNAME(mem);
}

void CSG_CheckMem(void)
{
	CSG_PUSHNAME("CheckMem");
	
	// ***********************
	// Checks for memory leaks
	// ***********************
	
	if(!CSG_MEMORY_CHECK)
	{
		CSG_POPNAME_;
	}
	int mem1 = CSG_GetMemInUse(NULL);      // as reported by Alloc/Free
	int mem2 = CSG_GetMemInUse((void*)-1); // pools
	int mem3 = CSG_GetMemInUse(CSG_WORLD); // world
	
	char msg[100];
	sprintf(msg, "memory leak found: %i", mem1 - mem2 - mem3);
	
	CSG_ASSERT(mem1 == mem2 + mem3, msg);
	CSG_POPNAME_;
}

void CSG_Messg(char *msg)
{
	// *************
	// Log some text
	// *************
	
return; // no logging
	if(!CSG_LOGFILE)
	{
		CSG_LOGFILE = fopen("CSG_log.txt", "w");
	}
	fprintf(CSG_LOGFILE, "-> %s\n", msg);
	fflush(CSG_LOGFILE);
}

void CSG_Error(char *msg)
{
	// ********************
	// Log an error message
	// ********************
	
	if(!CSG_LOGFILE)
	{
		CSG_LOGFILE = fopen("CSG_log.txt", "w");
	}
	if(CSG_FUNC_DEPTH > 0)
	{
		char *fn = CSG_FUNC_NAME_STACK[CSG_FUNC_DEPTH-1];
		fprintf(CSG_LOGFILE, "-> ERROR: %s: %s\n", fn, msg);
	}
	else
	{
		fprintf(CSG_LOGFILE, "-> ERROR: <?>: %s\n", msg);
	}
	fflush(CSG_LOGFILE);
#if 1
	int div0 = 0;
	int x = 1 / div0; // cause DIVIDE-BY-ZERO and halt
#endif
	CSG_ERRORS++;
}

void CSG_PushFunctionName(char *name)
{
	// ****************************************************
	// Useful for debugging - every function should declare
	// its name so that an error message may report it.
	// ****************************************************
	
	CSG_ASSERT(CSG_FUNC_DEPTH < CSG_MAX_FUNC_DEPTH,
	"function stack overflow");
	
	strncpy(CSG_FUNC_NAME_STACK[CSG_FUNC_DEPTH++], name, 60);
}

void CSG_PopFunctionName(void)
{
	CSG_ASSERT(CSG_FUNC_DEPTH > 0, "function stack underflow");
	CSG_FUNC_DEPTH--;
}

void CSG_Print(poly_t *poly)
{
	CSG_PUSHNAME("Print[POLY]");
	
	char s1[200] = "v= ";
	char s2[200];
	
	for(int n = 0; n < poly->vertcount; n++)
	{
		vert_t v = CSG_VERT_POOL[poly->vref[n]];
		
		sprintf(s2, "(%.2f,%.2f,%.2f)", v.x, v.y, v.z);
		strcat(s1, s2);
	}
	CSG_Messg("POLY:");
	CSG_Messg(s1);
	CSG_Messg("END POLY");
	CSG_POPNAME_;
}

void CSG_Print(hull_t *hull)
{
	CSG_PUSHNAME("Print[HULL]");
	CSG_Messg("HULL:");
	
	for(poly_t *p = hull->face; p; p = p->next)
	{
		CSG_Print(p);
	}
	CSG_Messg("END HULL");
	CSG_POPNAME_;
}

vref_t CSG_AddToVertPool(vert_t &vert)
{
	CSG_PUSHNAME("AddToVertPool");
	
	// **********************************************
	// Add a vertex to the vertex pool. If the vertex
	// already exists, return its index.
	// **********************************************
	
	int i;
	
	for(i = 0; i < CSG_VERT_COUNT; i++)
	{
		vec3 err = vert - CSG_VERT_POOL[i];
		
		if(err*err <= CSG_MAXERR*CSG_MAXERR)
		{
			CSG_POPNAME((vref_t)i);
		}
	}
	CSG_ASSERT(CSG_VERT_COUNT < CSG_MAX_VERTS, "too many vertices");
	CSG_VERT_POOL[i] = vert;
	CSG_VERT_COUNT++;
	CSG_POPNAME((vref_t)i);
}

sref_t CSG_AddToSurfPool(surf_t &surf)
{
	CSG_PUSHNAME("AddToSurfPool");
	
	// *************************************************
	// Add a surface to the surface pool. If the surface
	// already exists, return its index.
	// *************************************************
	
	int i;
	
	for(i = 0; i < CSG_SURF_COUNT; i++)
	{
		if(fabs(CSG_SURF_POOL[i].dist - surf.dist) <= CSG_MAXERR)
		{
			if(fabs(CSG_SURF_POOL[i].norm*surf.norm - 1.0) <= CSG_MAXERR2)
			{
				CSG_POPNAME((sref_t)i);
			}
		}
		if(fabs(CSG_SURF_POOL[i].dist + surf.dist) <= CSG_MAXERR)
		{
			if(fabs(CSG_SURF_POOL[i].norm*surf.norm + 1.0) <= CSG_MAXERR2)
			{
				CSG_POPNAME((sref_t)i | CSG_SIDE_MASK);
			}
		}
	}
	CSG_ASSERT(CSG_SURF_COUNT < CSG_MAX_SURFS, "too many surfaces");
	CSG_SURF_COUNT++;
	CSG_SURF_POOL[i] = surf;
	
	// **************************************************
	// Compute surface mask. This field controls active
	// bounding-box components during POLY-POLY and POLY-
	// HULL bounding box checks.
	// **************************************************
	
	double nx = fabs(surf.norm.x);
	double ny = fabs(surf.norm.y);
	double nz = fabs(surf.norm.z);
	int mask;
	
	if(nx >= ny && nx >= nz) mask = 2+4; else
	if(ny >= nx && ny >= nz) mask = 1+4; else
	mask = 1+2;
	
	CSG_SURF_POOL[i].mask = mask;
	CSG_SURF_POOL[i].data = rand();
	CSG_POPNAME((sref_t)i);
}

sref_t CSG_AddToSurfPool(vref_t vref[], int i1, int i2, int i3)
{
	CSG_PUSHNAME("AddToSurfPool");
	
	// ***********************************************
	// Add a surface to the surface pool, created from
	// three vertices. If the surface already exists,
	// return its index.
	// ***********************************************
	
	surf_t surf;
	
	vert_t v1 = CSG_VERT_POOL[vref[i1]];
	vert_t v2 = CSG_VERT_POOL[vref[i2]];
	vert_t v3 = CSG_VERT_POOL[vref[i3]];
	
	vec3 norm = (v2-v1) ^ (v3-v1);
	double q = norm.len();
	
	CSG_ASSERT(q != 0.0, "zero normal");
	
	surf.norm.x = norm.x / q;
	surf.norm.y = norm.y / q;
	surf.norm.z = norm.z / q;
	surf.dist = -(norm*v1) / q;
	
	CSG_POPNAME(CSG_AddToSurfPool(surf));
}

void CSG_AddToPoly(poly_t *poly, vref_t vert_id, sref_t surf_id)
{
	CSG_PUSHNAME("AddToPoly");
	
	// ************************************************
	// Adds a vertex and surface reference to a polygon
	// ************************************************
	
	poly->vref[poly->vertcount] = vert_id;
	poly->sref[poly->vertcount] = surf_id;
	
#if CSG_DEBUG
	
	// *****************************
	// This helps catch errors early
	// *****************************
	
	int n2 = poly->vertcount - 1;
	
	double tt1 = CSG_Distance(vert_id, poly->surf_id);
	double tt2 = CSG_Distance(vert_id, surf_id);
	double tt3 = (n2 > -1) ? CSG_Distance(poly->vref[n2], surf_id):0.0;
	
	CSG_ASSERT(tt1 == 0.0, "vertex not on surface");
	CSG_ASSERT(tt2 == 0.0, "vertex not on surface");
	CSG_ASSERT(tt3 == 0.0, "vertex not on surface");
#endif
	poly->vertcount++;
	CSG_POPNAME_;
}

void CSG_Copy(poly_t *dst, poly_t *src)
{
	CSG_PUSHNAME("Copy[POLY]");
	
	// **********************************************
	// Deallocate existing dst->VREF and dst->SREF if
	// needed, and copy src->VREF and src->SREF into
	// dst. Also copy VERTCOUNT. Leave other data
	// unchanged.
	// **********************************************
	
	if(dst->vref) CSG_FreeMem(dst->vref);
	if(dst->sref) CSG_FreeMem(dst->sref);
	
	dst->vref = (vref_t*)CSG_AllocMem(sizeof(vref_t), src->vertcount);
	dst->sref = (sref_t*)CSG_AllocMem(sizeof(sref_t), src->vertcount);
	memcpy(dst->vref, src->vref, sizeof(vref_t) * src->vertcount);
	memcpy(dst->sref, src->sref, sizeof(sref_t) * src->vertcount);
	dst->vertcount = src->vertcount;
	CSG_POPNAME_;
}

void CSG_Copy(hull_t *dst, hull_t *src)
{
	CSG_PUSHNAME("Copy[HULL]");
	CSG_ASSERT(0, "not implemented!");
	CSG_POPNAME_;
}

poly_t *CSG_Clone(void *parent, poly_t *src)
{
	CSG_PUSHNAME("Clone[POLY]");
	
	// **********************************************
	// Clone polygon. Sub-polygons are not preserved.
	// **********************************************
	
	poly_t *dst = CSG_Alloc(parent, src->vertcount, src->surf_id);
	
	memcpy(dst->vref, src->vref, sizeof(vref_t) * src->vertcount);
	memcpy(dst->sref, src->sref, sizeof(sref_t) * src->vertcount);
	
	dst->vertcount = src->vertcount;
	dst->bbox      = src->bbox;
	dst->shape     = src->shape;
	
	CSG_POPNAME(dst);
}

hull_t *CSG_Clone(void *parent, hull_t *src)
{
	CSG_PUSHNAME("Clone[HULL]");
	
	// *******************************************************
	// Clone hull. Sub-hulls are not preserved, but faces are.
	// *******************************************************
	
	hull_t *dst = CSG_Alloc(parent);
	
	dst->vref = (vref_t*)CSG_AllocMem(sizeof(vref_t), src->vertcount);
	memcpy(dst->vref, src->vref, sizeof(vref_t) * src->vertcount);
	
	dst->vertcount = src->vertcount;
	dst->bbox      = src->bbox;
	dst->shape     = src->shape;
	
	for(poly_t *p = src->face; p; p = p->next)
	{
		CSG_Clone(dst, p);
	}
	CSG_POPNAME(dst);
}

void *CSG_AllocMem(int size, int nelems)
{
	CSG_PUSHNAME("AllocMem");
	
	// **************************
	// Allocate a block of memory
	// **************************
	
#if CSG_DEBUG
	CSG_MEMORY_USED += size * nelems;
#endif
#ifdef __MAC__
	void *tmp = NewPtr(size * nelems);
	CSG_ASSERT(tmp, "out of memory");
	memset(tmp, 0, size * nelems);
#else
	void *tmp = calloc(size, nelems);
	CSG_ASSERT(tmp, "out of memory");
#endif
	CSG_POPNAME(tmp);
}

poly_t *CSG_Alloc(void *parent, int vertcount, sref_t surf_id)
{
	CSG_PUSHNAME("Alloc[POLY]");
	
	// ***************************************
	// Allocate new polygon and link to parent
	// ***************************************
	
	poly_t *poly = (poly_t*)CSG_AllocMem(sizeof(poly_t), 1);
	poly->flags   = CSG_TYPE_POLY;
	poly->surf_id = surf_id;
	CSG_Link(parent, poly);
	
	if(vertcount > 0)
	{
		// *********************************************
		// Allocate space for vertices if vertcount > 0,
		// otherwise leave as NULL. Note that vertcount
		// of polygon is left at 0 regardless.
		// *********************************************
		
		poly->vref = (vref_t*)CSG_AllocMem(sizeof(vref_t), vertcount);
		poly->sref = (sref_t*)CSG_AllocMem(sizeof(sref_t), vertcount);
	}
	poly->q = (double)rand();
	CSG_POPNAME(poly);
}

hull_t *CSG_Alloc(void *parent)
{
	CSG_PUSHNAME("Alloc[HULL]");
	
	// ************************************
	// Allocate new hull and link to parent
	// ************************************
	
	hull_t *hull = (hull_t*)CSG_AllocMem(sizeof(hull_t), 1);
	hull->flags = CSG_TYPE_HULL;
	CSG_Link(parent, hull);
	CSG_POPNAME(hull);
}

void CSG_FreeMem(void *dst)
{
	CSG_PUSHNAME("FreeMem");
	
	// *****************
	// Free memory block
	// *****************
	
	if(!dst)
	{
		CSG_POPNAME_;
	}
#ifdef __MAC__
#if CSG_DEBUG
	CSG_MEMORY_USED -= GetPtrSize((char*)dst);
#endif
	DisposePtr((char*)dst);
#else
#if CSG_DEBUG
	CSG_MEMORY_USED -= ((long*)dst)[-4]; // implementation-specific!
#endif
	free(dst);
#endif
	CSG_POPNAME_;
}

void CSG_Free(poly_t *poly)
{
	CSG_PUSHNAME("Free[POLY]");
	
	// ***************************************************
	// Recursively free poly and all associated structures
	// ***************************************************
	
	CSG_Unlink(poly);
	
	for(poly_t *pnext, *p = poly->down; p; p = pnext)
	{
		pnext = p->next;
		CSG_Free(p);
	}
	CSG_FreeMem(poly->vref);
	CSG_FreeMem(poly->sref);
	CSG_FreeMem(poly);
	CSG_POPNAME_;
}

void CSG_Free(hull_t *hull)
{
	CSG_PUSHNAME("Free[HULL]");
	
	// ***************************************************
	// Recursively free hull and all associated structures
	// ***************************************************
	
	CSG_Unlink(hull);
	
	for(poly_t *pnext, *p = hull->face; p; p = pnext)
	{
		pnext = p->next;
		CSG_Free(p);
	}
	for(hull_t *hnext, *h = hull->down; h; h = hnext)
	{
		hnext = h->next;
		CSG_Free(h);
	}
	CSG_FreeMem(hull->vref);
	CSG_FreeMem(hull);
	CSG_POPNAME_;
}

void CSG_Unlink(poly_t *poly)
{
	CSG_PUSHNAME("Unlink[POLY]");
	
	// **************************
	// Unlink polygon from parent
	// **************************
	
	if(poly->parent)
	{
		if(*(int*)poly->parent == CSG_TYPE_POLY)
		{
			poly_t *parent = (poly_t*)poly->parent;
			
			if(parent->down == poly)
			{
				parent->down = poly->next;
			}
			else
			{
				poly_t *p = parent->down;
				
				while(p && p->next && p->next != poly) p = p->next;
				
				CSG_ASSERT(p && p->next, "no reference from parent");
				
				p->next = poly->next;
			}
		}
		else // is hull
		{
			hull_t *parent = (hull_t*)poly->parent;
			
			if(parent->face == poly)
			{
				parent->face = poly->next;
			}
			else
			{
				poly_t *p = parent->face;
				
				while(p && p->next && p->next != poly) p = p->next;
				
				CSG_ASSERT(p && p->next, "no reference from parent");
				
				p->next = poly->next;
			}
		}
		poly->parent = NULL;
		poly->next   = NULL;
	}
	poly->depth = 0;
	CSG_POPNAME_;
}

void CSG_Unlink(hull_t *hull)
{
	CSG_PUSHNAME("Unlink[HULL]");
	
	// ***********************
	// Unlink hull from parent
	// ***********************
	
	if(hull->parent)
	{
		hull_t *parent = (hull_t*)hull->parent;
		
		if(parent->down == hull)
		{
			parent->down = hull->next;
		}
		else
		{
			hull_t *h = parent->down;
			
			while(h && h->next && h->next != hull) h = h->next;
			
			CSG_ASSERT(h && h->next, "no reference from parent");
			
			h->next = hull->next;
		}
		hull->parent = NULL;
		hull->next   = NULL;
	}
	hull->depth = 0;
	CSG_POPNAME_;
}

void CSG_Link(void *parent, poly_t *poly)
{
	CSG_PUSHNAME("Link[POLY]");
	
	// **********************
	// Link polygon to parent
	// **********************
	
	poly->parent = parent;
	
	if(!parent)
	{
		poly->depth = 0; // this is a dangerous assumption..?
		CSG_POPNAME_;
	}
	switch(*(int*)parent)
	{
		case CSG_TYPE_POLY:
		{
			poly_t *dst = (poly_t*)parent;
			
			poly->depth = dst->depth + 1;
			poly->next  = dst->down;
			dst->down   = poly;
			break;
		}
		case CSG_TYPE_HULL:
		{
			hull_t *dst = (hull_t*)parent;
			
			poly->depth = 0;
			poly->next  = dst->face;
			dst->face   = poly;
			break;
		}
	}
	CSG_POPNAME_;
}

void CSG_Link(void *parent, hull_t *hull)
{
	CSG_PUSHNAME("Link[HULL]");
	
	// *******************
	// Link hull to parent
	// *******************
	
	hull->parent = parent;
	
	if(!parent)
	{
		hull->depth = -1;
		CSG_POPNAME_;
	}
	switch(*(int*)parent)
	{
		case CSG_TYPE_HULL:
		{
			hull_t *dst = (hull_t*)parent;
			
			hull->depth = dst->depth + 1;
			hull->next  = dst->down;
			dst->down   = hull;
			break;
		}
	}
	CSG_POPNAME_;
}

int CSG_Count(poly_t *poly)
{
	CSG_PUSHNAME("Count[POLY]");
	
	// **********************
	// Count polygons in list
	// **********************
	
	int count = 0;
	
	while(poly)
	{
		count++;
		poly = poly->next;
	}
	CSG_POPNAME(count);
}

int CSG_Count(hull_t *hull)
{
	CSG_PUSHNAME("Count[HULL]");
	
	// *******************
	// Count hulls in list
	// *******************
	
	int count = 0;
	
	while(hull)
	{
		count++;
		hull = hull->next;
	}
	CSG_POPNAME(count);
}

int CSG_Count(ccof_t *ccof)
{
	CSG_PUSHNAME("Count[CCOF");
	
	// ********************
	// Count CCOF's in list
	// ********************
	
	int count = 0;
	
	while(ccof)
	{
		count++;
		ccof = ccof->next;
	}
	CSG_POPNAME(count);
}

void CSG_SetPolygonalizationDepth(int depth)
{
	CSG_PUSHNAME("SetPolygonalizationDepth");
	
	// *****************************************************
	// Sets the depth level (in the HCSG hierarchy) at which
	// polygonalization begins. Essentially, this causes all
	// hulls and faces at lesser depths to be ignored during
	// the polygonalization process.
	// *****************************************************
	
	CSG_ASSERT(depth >= 0 && depth <= CSG_MAX_DEPTH, "out of range");
	CSG_POLYGONALIZATION_DEPTH = depth;
	CSG_POPNAME_;
}

void CSG_GenCCOF(poly_t *face)
{
	CSG_PUSHNAME("GenCCOF");
	
	// **********************************************************
	// Generate list of Coplanar-Coincident Opposite Faces (CCOF)
	// **********************************************************
	
	hull_t *hull = (hull_t*)face->parent;
	int ascend = 1;
	
	// ******************
	// First, reset lists
	// ******************
	
	for(int i = 0; i <= CSG_MAX_DEPTH; i++)
	{
		CSG_CCOF[i] = NULL;
	}
	CSG_CCOF_COUNT = 0;
	
	// ***************************************************
	// Ascend hierarchy until face is strictly inside hull
	// ***************************************************
	
	while(ascend)
	{
		ascend = 0;
		
		for(poly_t *p = hull->face; p; p = p->next)
		{
			if(p->surf_id == face->surf_id)
			{
				hull = (hull_t*)hull->parent;
				
				if(!hull)
				{
					CSG_POPNAME_; // CCOF = {}
				}
				ascend = 1;
				break;
			}
		}
	}
	// *************************************************
	// Descend hierarchy recursively and enumerate faces
	// *************************************************
	
	CSG_GenCCOF(face, hull);
	CSG_POPNAME_;
}

void CSG_GenCCOF(poly_t *face, hull_t *hull)
{
	CSG_PUSHNAME("GenCCOF");
	
	// *********************************
	// Internal recursive stuff for CCOF
	// *********************************
	
	sref_t surf_id_opp = face->surf_id ^ CSG_SIDE_MASK;
	
	for(hull_t *h = hull->down; h; h = h->next)
	{
		for(poly_t *p = h->face; p; p = p->next)
		{
			if(p->surf_id != surf_id_opp) continue;
			if(CSG_Intersect(p, face))    continue;
			
			if(h->depth >= CSG_POLYGONALIZATION_DEPTH)
			{
				CSG_ASSERT(CSG_CCOF_COUNT < CSG_MAX_CCOFS, "too many CCOFs");
				
				ccof_t *cc = &CSG_CCOF_POOL[CSG_CCOF_COUNT++];
				
				cc->face = p;
				cc->next = CSG_CCOF[h->depth];
				CSG_CCOF[h->depth] = cc;
			}
			CSG_GenCCOF(face, h);
			break;
		}
	}
	CSG_POPNAME_;
}

void CSG_Update(bbox_t *bbox, vref_t vref[], int vertcount)
{
	CSG_PUSHNAME("Update[BBOX]");
	
	// *******************
	// Update bounding box
	// *******************
	
	CSG_ASSERT(vertcount > 0, "zero vertcount");
	
	bbox->minv = CSG_VERT_POOL[vref[0]];
	bbox->maxv = CSG_VERT_POOL[vref[0]];
	
	for(int i = 1; i < vertcount; i++)
	{
		vert_t v = CSG_VERT_POOL[vref[i]];
		
		if(bbox->minv.x > v.x) bbox->minv.x = v.x;
		if(bbox->minv.y > v.y) bbox->minv.y = v.y;
		if(bbox->minv.z > v.z) bbox->minv.z = v.z;
		if(bbox->maxv.x < v.x) bbox->maxv.x = v.x;
		if(bbox->maxv.y < v.y) bbox->maxv.y = v.y;
		if(bbox->maxv.z < v.z) bbox->maxv.z = v.z;
	}
	CSG_POPNAME_;
}

void CSG_Update(poly_t *poly)
{
	CSG_PUSHNAME("Update[POLY]");
	
	// ***************************
	// Update polygon fields: BBOX
	// ***************************
	
	CSG_Update(&poly->bbox, poly->vref, poly->vertcount);
	CSG_CHECK(poly);
	CSG_POPNAME_;
}

void CSG_Update(hull_t *hull)
{
	CSG_PUSHNAME("Update[HULL]");
	
	// *****************************************
	// Update hull fields: BBOX, VREF, VERTCOUNT
	// *****************************************
	
	poly_t *p;
	
	int vertcount = 0;
	int edgecount = 0;
	int facecount = 0;
	
	for(p = hull->face; p; p = p->next)
	{
		edgecount += p->vertcount;
		facecount++;
	}
	edgecount /= 2;
	vertcount  = edgecount - facecount + 2; // Euler formula: V=E-F+2
	
	CSG_FreeMem(hull->vref);
	hull->vref = (vref_t*)CSG_AllocMem(sizeof(vref_t), vertcount);
	hull->vertcount = 0;
	
	for(p = hull->face; p; p = p->next)
	{
		for(int i, n = 0; n < p->vertcount; n++)
		{
			for(i = 0; i < hull->vertcount; i++)
			{
				if(p->vref[n] == hull->vref[i]) break;
			}
			if(i < hull->vertcount) continue;
			
			CSG_ASSERT(hull->vertcount != vertcount, "connectivity error");
			
			hull->vref[hull->vertcount++] = p->vref[n];
		}
	}
	CSG_Update(&hull->bbox, hull->vref, hull->vertcount);
	CSG_CHECK(hull);
	CSG_POPNAME_;
}

void CSG_Update(hull_t *hull, sref_t surf_id)
{
	CSG_PUSHNAME("Update[HULL,SURF_ID]");
	
	// ***********************************************
	// Given a hull which was formed by CSG_Split,
	// compute the missing face which lies in surf_id.
	// ***********************************************
	
	// ********************
	// Enumerate free edges
	// ********************
	
	vref_t edge[CSG_MAX_VERTS_PER_POLY*2];
	sref_t sref[CSG_MAX_VERTS_PER_POLY];
	
	int edgecount = 0;
	
	for(poly_t *p = hull->face; p; p = p->next)
	{
		for(int n = 0; n < p->vertcount; n++)
		{
			int n2 = (n > 0) ? n-1 : p->vertcount-1;
			
			vref_t vert_id1 = p->vref[n2];
			vref_t vert_id2 = p->vref[n];
			
			int edgerefs = 1;
			
			for(poly_t *q = hull->face; q; q = q->next)
			{
				if(q == p) continue;
				
				for(int m = 0; m < q->vertcount; m++)
				{
					int m2 = (m > 0) ? m-1 : q->vertcount-1;
					
					vref_t r1 = q->vref[m2];
					vref_t r2 = q->vref[m];
					
					if(r1 == vert_id2 && r2 == vert_id1)
					{
						edgerefs++;
						break;
					}
				}
#if !CSG_DEBUG
				if(m < q->vertcount) break; // slight optimization
#endif
			}
			if(edgerefs == 1)
			{
				// *********************
				// Add edge to free list
				// *********************
				
				edge[edgecount*2 + 0] = vert_id1; // left vertex
				edge[edgecount*2 + 1] = vert_id2; // right vertex
				sref[edgecount++]     = p->surf_id;
#if CSG_DEBUG
				double tt1 = CSG_Distance(vert_id1, surf_id);
				double tt2 = CSG_Distance(vert_id2, surf_id);
				
				CSG_ASSERT(tt1 == 0.0, "wrong surface");
				CSG_ASSERT(tt2 == 0.0, "wrong surface");
			}
			else CSG_ASSERT(edgerefs == 2, "edgerefs > 2");
#else
			}
#endif
		}
	}
	// ***************************************
	// Attach free edges together to form face
	// ***************************************
	
	poly_t *poly = CSG_Alloc(hull, edgecount, surf_id);
	
	for(int i = 0, j = 0; i < edgecount; i++)
	{
		vref_t vert_id1 = edge[j*2];
		
		CSG_AddToPoly(poly, vert_id1, sref[j]);
		
		while(edge[1 + j*2] != vert_id1)
		{
			// NOTE: possible crash site
			if(++j >= edgecount) j = 0;
		}
	}
	CSG_Update(poly);
	CSG_Update(hull);
	CSG_POPNAME_;
}

int CSG_In(vref_t vref[], int vertcount, sref_t surf_id)
{
	CSG_PUSHNAME("In/Intersect");
	
	// ************************************************
	// Intersection utility function. Returns 1 iff all
	// vertices (from POLY or HULL) are on "inside" of
	// surface plane. WARNING: This function does NOT
	// modify CSG_CODE_IN or CSG_CODE_OUT.
	// ************************************************
	
	for(int n = 0; n < vertcount; n++)
	{
		if(CSG_Distance(vref[n], surf_id) < 0.0)
		{
			CSG_POPNAME(0);
		}
	}
	CSG_POPNAME(1);
}

int CSG_Intersect(bbox_t *bbox1, bbox_t *bbox2, int mask)
{
	CSG_PUSHNAME("Intersect[BBOX,BBOX]");
	
	// ***************************************
	// Intersection between two bounding boxes
	// ***************************************
	
	// **************************************************
	// General note regarding intersection functions:
	// 
	// Intersection functions return 1 iff object1 is
	// entirely outside or touching object2, 0 otherwise.
	// Additionally, the static variable CSG_CODE_IN is
	// set to 1 iff object1 is entirely inside or
	// touching object2, 0 otherwise. The static variable
	// CSG_CODE_OUT mirrors the returned value (normally,
	// it is not queried). If object1 is contained in the
	// boundary of object2 (i.e., a ploygon coplanar with
	// one face of a polyhedron) it is treated as being
	// outside. The exception to this rule is when a
	// polygon or polyhedron is tested against a single
	// plane - in this case, the polygon or polyhedron
	// being entirely coplanar results in an error.
	// **************************************************
	
	CSG_CODE_OUT = 1;
	CSG_CODE_IN  = 0;
	
	if(mask & 1)
	{
		if(bbox1->maxv.x <= bbox2->minv.x + CSG_MAXERR ||
		   bbox1->minv.x >= bbox2->maxv.x - CSG_MAXERR )
		{
			CSG_POPNAME(1);
		}
	}
	if(mask & 2)
	{
		if(bbox1->maxv.y <= bbox2->minv.y + CSG_MAXERR ||
		   bbox1->minv.y >= bbox2->maxv.y - CSG_MAXERR )
		{
			CSG_POPNAME(1);
		}
	}
	if(mask & 4)
	{
		if(bbox1->maxv.z <= bbox2->minv.z + CSG_MAXERR ||
		   bbox1->minv.z >= bbox2->maxv.z - CSG_MAXERR )
		{
			CSG_POPNAME(1);
		}
	}
	CSG_CODE_OUT = 0;
	
	if(bbox1->minv.x >= bbox2->minv.x - CSG_MAXERR &&
	   bbox1->maxv.x <= bbox2->maxv.x + CSG_MAXERR &&
	   bbox1->minv.y >= bbox2->minv.y - CSG_MAXERR &&
	   bbox1->maxv.y <= bbox2->maxv.y + CSG_MAXERR &&
	   bbox1->minv.z >= bbox2->minv.z - CSG_MAXERR &&
	   bbox1->maxv.z <= bbox2->maxv.z + CSG_MAXERR )
	{
		CSG_CODE_IN = 1;
	}
	CSG_POPNAME(0);
}

int CSG_Intersect(poly_t *poly1, sref_t surf_id)
{
	CSG_PUSHNAME("Intersect[POLY,SURF_ID]");
	
	// ****************************************
	// Intersection between polygon and surface
	// ****************************************
	
	CSG_CODE_OUT = 1;
	CSG_CODE_IN  = 1;
	
	for(int n = 0; n < poly1->vertcount; n++)
	{
		double t = CSG_Distance(poly1->vref[n], surf_id);
		
		if(t > 0.0) CSG_CODE_OUT = 0; else
		if(t < 0.0) CSG_CODE_IN  = 0;
	}
	CSG_ASSERT(!CSG_CODE_OUT || !CSG_CODE_IN, "invalid code");
	CSG_POPNAME(CSG_CODE_OUT);
}

int CSG_Intersect(hull_t *hull1, sref_t surf_id)
{
	CSG_PUSHNAME("Intersect[HULL,SURF_ID]");
	
	// *************************************
	// Intersection between hull and surface
	// *************************************
	
	CSG_CODE_OUT = 1;
	CSG_CODE_IN  = 1;
	
	for(int n = 0; n < hull1->vertcount; n++)
	{
		double t = CSG_Distance(hull1->vref[n], surf_id);
		
		if(t > 0.0) CSG_CODE_OUT = 0; else
		if(t < 0.0) CSG_CODE_IN  = 0;
	}
	CSG_ASSERT(!CSG_CODE_OUT || !CSG_CODE_IN, "invalid code");
	CSG_POPNAME(CSG_CODE_OUT);
}

int CSG_Intersect(poly_t *poly1, poly_t *poly2)
{
	CSG_PUSHNAME("Intersect[POLY,POLY]");
	
	// *********************************
	// Intersection between two polygons
	// *********************************
	
	int n;
	
	sref_t surf_id1 = poly1->surf_id & CSG_SURF_MASK;
	sref_t surf_id2 = poly2->surf_id & CSG_SURF_MASK;
	
	CSG_ASSERT(surf_id1 == surf_id2, "not coplanar");
	
	int mask = CSG_SURF_POOL[surf_id1].mask;
	
	if(CSG_Intersect(&poly1->bbox, &poly2->bbox, mask))
	{
		CSG_POPNAME(1);
	}
	// ***********
	// Inside test
	// ***********
	
	if(CSG_CODE_IN)
	{
		for(n = 0; n < poly2->vertcount; n++)
		{
			sref_t surf_id = poly2->sref[n];
			
			int in = CSG_In(poly1->vref, poly1->vertcount, surf_id);
			
			if(!in) break;
		}
		if(n == poly2->vertcount)
		{
			CSG_POPNAME(0);
		}
		CSG_CODE_IN = 0;
	}
	// ************
	// Outside test
	// ************
	
	for(n = 0; n < poly1->vertcount; n++)
	{
		sref_t surf_id = poly1->sref[n] ^ CSG_SIDE_MASK;
		
		if(CSG_In(poly2->vref, poly2->vertcount, surf_id))
		{
			CSG_POPNAME(CSG_CODE_OUT = 1);
		}
	}
	for(n = 0; n < poly2->vertcount; n++)
	{
		sref_t surf_id = poly2->sref[n] ^ CSG_SIDE_MASK;
		
		if(CSG_In(poly1->vref, poly1->vertcount, surf_id))
		{
			CSG_POPNAME(CSG_CODE_OUT = 1);
		}
	}
	CSG_POPNAME(0);
}

int CSG_Intersect(hull_t *hull1, hull_t *hull2)
{
	CSG_PUSHNAME("Intersect[HULL,HULL]");
	
	// *************************************************
	// Intersection between two hulls. WARNING: The
	// algorithm used here can sometimes fail, returning
	// a false result of 0 when in fact CSG_CODE_OUT
	// should have been set to 1. This happens when the
	// two hulls may be seperated by a plane, but not by
	// any of the planes referenced by the hull faces.
	// In this case, the program must be prepared to
	// recover gracefully.
	// 
	// Consider, for example, the two trianglular prisms
	// defined by the vertices:
	// Prism A: (-1,-1,-1)(-1,+1,-1)(0,0,-1)
	//          (-1,-1,+1)(-1,+1,+1)(0,0,+1)
	// Prism B: (+1,+1,+1)(+1,+1,-1)(0,+1,0)
	//          (+1,-1,+1)(+1,-1,-1)(0,-1,0)
	// Of course in this case the bounding boxes would
	// indicate rejection, but if the geometry were
	// rotated slightly then the error would occur.
	// *************************************************
	
	poly_t *p;
	
	if(CSG_Intersect(&hull1->bbox, &hull2->bbox, 7))
	{
		CSG_POPNAME(1);
	}
	// ***********
	// Inside test
	// ***********
	
	if(CSG_CODE_IN)
	{
		for(p = hull2->face; p; p = p->next)
		{
			sref_t surf_id = p->surf_id;
			
			if(!CSG_In(hull1->vref, hull1->vertcount, surf_id))
			{
				break;
			}
		}
		if(!p)
		{
			CSG_POPNAME(0);
		}
		CSG_CODE_IN = 0;
	}
	// ************
	// Outside test
	// ************
	
	for(p = hull1->face; p; p = p->next)
	{
		sref_t surf_id = p->surf_id ^ CSG_SIDE_MASK;
		
		if(CSG_In(hull2->vref, hull2->vertcount, surf_id))
		{
			CSG_POPNAME(CSG_CODE_OUT = 1);
		}
	}
	for(p = hull2->face; p; p = p->next)
	{
		sref_t surf_id = p->surf_id ^ CSG_SIDE_MASK;
		
		if(CSG_In(hull1->vref, hull1->vertcount, surf_id))
		{
			CSG_POPNAME(CSG_CODE_OUT = 1);
		}
	}
	CSG_POPNAME(0);
}

int CSG_Intersect(poly_t *poly1, hull_t *hull2)
{
	CSG_PUSHNAME("Intersect[POLY,HULL]");
	
	// ****************************************************
	// This function uses the slow brute-force intersection
	// testing algorithm of actually clipping one object to
	// another and determining if anything is left. It is
	// important that this function never fail.
	// ****************************************************
	
	sref_t surf_id1 = poly1->surf_id & CSG_SURF_MASK;
	
	int mask = CSG_SURF_POOL[surf_id1].mask;
	
	if(CSG_Intersect(&poly1->bbox, &hull2->bbox, mask))
	{
		CSG_POPNAME(1);
	}
	if(CSG_Intersect(hull2, poly1->surf_id))
	{
		CSG_POPNAME(1);
	}
	if(CSG_CODE_IN)
	{
		CSG_CODE_OUT = 1;
		CSG_CODE_IN  = 0;
		CSG_POPNAME(1);
	}
	poly_t *tmp = CSG_Clone(NULL, poly1);
	
	CSG_CODE_OUT = 1;
	
	for(poly_t *p = hull2->face; p; p = p->next)
	{
		sref_t surf_id2 = p->surf_id & CSG_SURF_MASK;
		
		if(surf_id1 == surf_id2)
		{
			CSG_Free(tmp);
			CSG_POPNAME(1);
		}
		if(CSG_Clip(tmp, p->surf_id))
		{
			// "tmp" is already free
			CSG_POPNAME(1);
		}
	}
	CSG_Free(tmp);
	CSG_POPNAME(CSG_CODE_OUT = 0);
}

int CSG_Intersect(poly_t *poly1, int edge1, poly_t *poly2)
{
	CSG_PUSHNAME("Intersect[EDGE,POLY]");
	
	// ******************************************
	// Intersection between an edge and a polygon
	// ******************************************
	
	int edge2 = (edge1 > 0) ? edge1-1 : poly1->vertcount-1;
	
	vref_t vert_id1 = poly1->vref[edge1];
	vref_t vert_id2 = poly1->vref[edge2];
	sref_t surf_id1 = poly1->sref[edge1];
	sref_t surf_id2;
	
	if(CSG_Intersect(poly2, poly1->sref[edge1]))
	{
		CSG_POPNAME(1);
	}
	CSG_CODE_IN  = 0;
	CSG_CODE_OUT = 1;
	
	for(int n = 0; n < poly2->vertcount; n++)
	{
		surf_id2 = poly2->sref[n] & CSG_SURF_MASK;
		
		if(surf_id1 == surf_id2)
		{
			CSG_POPNAME(1);
		}
		double t1 = CSG_Distance(vert_id1, poly2->sref[n]);
		double t2 = CSG_Distance(vert_id2, poly2->sref[n]);
		
		if(t1 <= 0.0 && t2 <= 0.0)
		{
			CSG_POPNAME(1);
		}
	}
	CSG_POPNAME(CSG_CODE_OUT = 0);
}

double CSG_Distance(vert_t &vert, sref_t surf_id)
{
	CSG_PUSHNAME("Distance");
	
	// ****************************************************
	// Distance from vertex to surface plane. If vert is in
	// the "inside" halfspace, a positive value is returned.
	// If vert is in the "outside" halfspace, a negative
	// value is returned. If the distance is less than or
	// equal to CSG_MAXERR, zero is returned.
	// ****************************************************
	
	surf_t *surf = &CSG_SURF_POOL[surf_id & CSG_SURF_MASK];
	double dist = vert*surf->norm + surf->dist;
	
	if(fabs(dist) <= CSG_MAXERR)
	{
		CSG_POPNAME(0.0);
	}
	if(surf_id & CSG_SIDE_MASK)
	{
		CSG_POPNAME(-dist);
	}
	CSG_POPNAME(dist);
}

double CSG_Distance(vref_t vert_id, sref_t surf_id)
{
	return CSG_Distance(CSG_VERT_POOL[vert_id], surf_id);
}

int CSG_Clip(poly_t *poly, sref_t surf_id)
{
	CSG_PUSHNAME("Clip[POLY,SURF_ID]");
	
	poly_t *pnew = CSG_Split(poly->parent, poly, surf_id);
	if(pnew) CSG_Free(pnew);
	CSG_POPNAME(poly->flags?0:1);
}

int CSG_Clip(hull_t *hull, sref_t surf_id)
{
	CSG_PUSHNAME("Clip[HULL,SURF_ID]");
	
	hull_t *hnew = CSG_Split(hull->parent, hull, surf_id);
	if(hnew) CSG_Free(hnew);
	CSG_POPNAME(hull->flags?0:1);
}

int CSG_Clip(poly_t *poly, poly_t *clip)
{
	CSG_PUSHNAME("Clip[POLY,POLY]");
	
	// ***************************************************
	// Assuming polygon is not entirely inside or entirely
	// outside of clipping volume polygon, clip polygon to
	// each intersecting edge. Returns 1 iff polygon is
	// clipped to nothing.
	// ***************************************************
	
	for(int n = 0; n < clip->vertcount; n++)
	{
		if(CSG_Intersect(clip, n, poly)) continue;
		if(CSG_Clip(poly, clip->sref[n]))
		{
			CSG_Messg("Clip[POLY]: WARNING: Recovering from bad intersect...");
			CSG_POPNAME(1);
		}
	}
	CSG_POPNAME(0);
}

int CSG_Clip(hull_t *hull, hull_t *clip)
{
	CSG_PUSHNAME("Clip[HULL,HULL]");
	
	// ************************************************
	// Assuming hull is not entirely inside or entirely
	// outside of clipping volume hull, clip hull to
	// each intersecting face. Returns 1 iff hull is
	// clipped to nothing.
	// ************************************************
	
	for(poly_t *p = clip->face; p; p = p->next)
	{
		if(CSG_Intersect(p, hull)) continue;
		if(CSG_Clip(hull, p->surf_id))
		{
			CSG_Messg("Clip[HULL]: WARNING: Recovering from bad intersect...");
			CSG_POPNAME(1);
		}
	}
	CSG_POPNAME(0);
}

poly_t *CSG_Split(void *parent, poly_t *poly, sref_t surf_id)
{
	CSG_PUSHNAME("Split[POLY]");
	
	// ********************************************
	// Recursively split polygon and all associated
	// structures. Outside fragment is returned.
	// ********************************************
	
	CSG_ASSERT((surf_id       & CSG_SURF_MASK) !=
	           (poly->surf_id & CSG_SURF_MASK), "coincident polys");
	
	if(CSG_Intersect(poly, surf_id))
	{
		if(poly->parent != parent)
		{
			CSG_Unlink(poly);
			CSG_Link(parent, poly);
		}
		CSG_POPNAME(poly);
	}
	if(CSG_CODE_IN)
	{
		CSG_POPNAME(NULL);
	}
	poly_t *pnew = CSG_Alloc(parent, 0, poly->surf_id);
	
	pnew->shape = poly->shape;
	
	for(poly_t *pnext, *p = poly->down; p; p = pnext)
	{
		pnext = p->next;
		CSG_Split(pnew, p, surf_id);
	}
	vert_t v1, v2 = CSG_VERT_POOL[poly->vref[poly->vertcount - 1]];
	double t1, t2 = CSG_Distance(v2, surf_id);
	
	// ***********************
	// Allocate for worst-case
	// ***********************
	
	poly_t *split1 = CSG_Alloc(NULL, poly->vertcount+1, poly->surf_id);
	poly_t *split2 = CSG_Alloc(NULL, poly->vertcount+1, poly->surf_id);
	
	// Inner Fragment: split1
	// Outer Fragment: split2
	
	for(int n = 0; n < poly->vertcount; n++)
	{
		v1 = v2, v2 = CSG_VERT_POOL[poly->vref[n]];
		t1 = t2, t2 = CSG_Distance(v2, surf_id);
		
		if(t2 > 0.0)
		{
			if(t1 < 0.0)
			{
				// *****************************************
				// NOTE: To avoid inaccurate clipping, we
				// must avoid calling the vec3 "/" operator!
				// *****************************************
				
				vert_t clip = v1*t2 - v2*t1;
				vref_t vert_id;
				
				clip.x /= t2 - t1;
				clip.y /= t2 - t1;
				clip.z /= t2 - t1;
				
				vert_id = CSG_AddToVertPool(clip);
#if CSG_DEBUG
				double tt1 = CSG_Distance(vert_id, surf_id);
				double tt2 = CSG_Distance(vert_id, poly->surf_id);
				double tt3 = CSG_Distance(vert_id, poly->sref[n]);
				
				CSG_ASSERT(tt1==0.0&&tt2==0.0&&tt3==0.0, "clipping error");
#endif
				CSG_AddToPoly(split1, vert_id, surf_id);
				CSG_AddToPoly(split2, vert_id, poly->sref[n]);
			}
			CSG_AddToPoly(split1, poly->vref[n], poly->sref[n]);
		}
		else if(t2 < 0.0)
		{
			if(t1 > 0.0)
			{
				// *****************************************
				// NOTE: To avoid inaccurate clipping, we
				// must avoid calling the vec3 "/" operator!
				// *****************************************
				
				vert_t clip = v1*t2 - v2*t1;
				vref_t vert_id;
				
				clip.x /= t2 - t1;
				clip.y /= t2 - t1;
				clip.z /= t2 - t1;
				
				vert_id = CSG_AddToVertPool(clip);	
#if CSG_DEBUG
				double tt1 = CSG_Distance(vert_id, surf_id);
				double tt2 = CSG_Distance(vert_id, poly->surf_id);
				double tt3 = CSG_Distance(vert_id, poly->sref[n]);
				
				CSG_ASSERT(tt1==0.0&&tt2==0.0&&tt3==0.0, "clipping error");
#endif
				CSG_AddToPoly(split1, vert_id, poly->sref[n]);
				CSG_AddToPoly(split2, vert_id, surf_id ^ CSG_SIDE_MASK);
			}
			CSG_AddToPoly(split2, poly->vref[n], poly->sref[n]);
		}
		else
		{
			if(t1 < 0.0)
			{
				CSG_AddToPoly(split1, poly->vref[n], surf_id);
				CSG_AddToPoly(split2, poly->vref[n], poly->sref[n]);
			}
			else if(t1 > 0.0)
			{
				CSG_AddToPoly(split1, poly->vref[n], poly->sref[n]);
				CSG_AddToPoly(split2, poly->vref[n], surf_id ^ CSG_SIDE_MASK);
			}
			else CSG_ASSERT(0, "coincident edges");
		}
	}
	CSG_Copy(poly, split1);
	CSG_Copy(pnew, split2);
	CSG_Free(split1);
	CSG_Free(split2);
	CSG_Update(poly);
	CSG_Update(pnew);
#if CSG_DEBUG
	CSG_Intersect(poly, surf_id); CSG_ASSERT(CSG_CODE_IN,  "failed");
	CSG_Intersect(pnew, surf_id); CSG_ASSERT(CSG_CODE_OUT, "failed");
#endif
	CSG_POPNAME(pnew);
}

hull_t *CSG_Split(void *parent, hull_t *hull, sref_t surf_id)
{
	CSG_PUSHNAME("Split[HULL]");
	
	// *****************************************
	// Recursively split hull and all associated
	// structures. Outside fragment is returned.
	// *****************************************
	
	if(CSG_Intersect(hull, surf_id))
	{
		if(hull->parent != parent)
		{
			CSG_Unlink(hull);
			CSG_Link(parent, hull);
		}
		CSG_POPNAME(hull);
	}
	if(CSG_CODE_IN)
	{
		CSG_POPNAME(NULL);
	}
	hull_t *hnew = CSG_Alloc(parent);
	hnew->shape = hull->shape;
	
	for(poly_t *pnext, *p = hull->face; p; p = pnext)
	{
		pnext = p->next;
		CSG_Split(hnew, p, surf_id);
	}
	for(hull_t *hnext, *h = hull->down; h; h = hnext)
	{
		hnext = h->next;
		CSG_Split(hnew, h, surf_id);
	}
	CSG_Update(hull, surf_id);
	CSG_Update(hnew, surf_id ^ CSG_SIDE_MASK);
	CSG_POPNAME(hnew);
}

void CSG_BooleanOp(hull_t *dst, hull_t *src, int target_depth, int op, int clip)
{
	CSG_PUSHNAME("BooleanOp");
	CSG_ASSERT(target_depth > dst->depth, "incompatible depths");
	CSG_ASSERT(op == CSG_ADD || op == CSG_SUB, "unknown op");
	
	if(target_depth - dst->depth == 1)
	{
		if(op == CSG_ADD)
		{
			hull_t *clone = CSG_Clone(NULL, src);
			CSG_BooleanAdd(dst, clone, clip);
		}
		else if(op == CSG_SUB) CSG_BooleanSub(dst, src);
	}
	else
	{
		for(hull_t *h = dst->down; h; h = h->next)
		{
			if(CSG_Intersect(src, h)) continue;
			
			CSG_BooleanOp(h, src, target_depth, op, CSG_CODE_IN ? 0 : 1);
		}
	}
	CSG_POPNAME_;
}

void CSG_BooleanAdd(poly_t *dst, poly_t *src, int clip)
{
	CSG_PUSHNAME("BooleanAdd[POLY]");
	
	// ***********************************************
	// Two-dimensional CSG boolean addition. Note that
	// CSG_RESOLVE_ADD_2D controls whether existing or
	// new polygons are split.
	// ***********************************************
	
	if(clip)
	{
		if(CSG_Clip(src, dst))
		{
			CSG_POPNAME_;
		}
	}
	if(CSG_RESOLVE_ADD_2D == CSG_RESOLVE_KEEP_NEW)
	{
		CSG_BooleanSub(dst, src);
	}
	else if(CSG_RESOLVE_ADD_2D == CSG_RESOLVE_KEEP_OLD)
	{
		for(poly_t *p = dst->down; p; p = p->next)
		{
			if(CSG_Intersect(src, p)) continue;
			if(CSG_CODE_IN == 0)
			{
				int split = 0;
				
				for(int n = 0; n < p->vertcount; n++)
				{
					if(CSG_Intersect(p, n, src)) continue;
					poly_t *pnew = CSG_Split(NULL, src, p->sref[n]);
					CSG_BooleanAdd(dst, pnew, 0);
					split = 1;
				}
				CSG_ASSERT(split == 1, "intersection failure");
				if(!split) continue; // for symmetry
			}
			CSG_Free(src);
			CSG_POPNAME_;
		}
	}
	src->shape = CSG_CURRENT_SHAPE;
	CSG_Link(dst, src);
	CSG_POPNAME_;
}

void CSG_BooleanAdd(hull_t *dst, hull_t *src, int clip)
{
	CSG_PUSHNAME("BooleanAdd[HULL]");
	
	// ************************************************
	// Adds volume represented by "src" hull to volume
	// represented by children of "dst" hull, splitting
	// and re-polygonalizing as necessary. Note that
	// CSG_RESOLVE_ADD_3D controls whether existing or
	// new volumes are split.
	// ************************************************
	
	if(clip)
	{
		if(CSG_Clip(src, dst))
		{
			CSG_POPNAME_;
		}
	}
	if(CSG_RESOLVE_ADD_3D == CSG_RESOLVE_KEEP_NEW)
	{
		// *******************************************
		// FIXME: Inefficient! Polygonalization should
		// occur only after inclusion of new hull.
		// *******************************************
		
		CSG_BooleanSub(dst, src);
	}
	else if(CSG_RESOLVE_ADD_3D == CSG_RESOLVE_KEEP_OLD)
	{
		for(hull_t *h = dst->down; h; h = h->next)
		{
			if(CSG_Intersect(src, h)) continue;
			if(CSG_CODE_IN == 0)
			{
				int split = 0;
				
				for(poly_t *p = h->face; p; p = p->next)
				{
					if(CSG_Intersect(p, src)) continue;
					hull_t *hnew = CSG_Split(NULL, src, p->surf_id);
					CSG_BooleanAdd(dst, hnew, 0);
					split = 1;
				}
				if(!split) continue;
			}
			CSG_Free(src);
			CSG_POPNAME_;
		}
	}
	src->shape = CSG_CURRENT_SHAPE;
	CSG_Link(dst, src);
	CSG_Polygonalize(src);
	CSG_POPNAME_;
}

void CSG_BooleanSub(poly_t *dst, poly_t *src)
{
	CSG_PUSHNAME("BooleanSub[POLY]");
	
	// ****************************************************
	// Subtracts polygon represented by "src" poly from all
	// polygons represented by children of "dst" poly,
	// splitting as necessary.
	// ****************************************************
	
	for(poly_t *pnext, *p = dst->down; p; p = pnext)
	{
		pnext = p->next;
		
		if(CSG_Intersect(p, src)) continue;
		if(CSG_CODE_IN == 0)
		{
			int split = 0;
			
			for(int n = 0; n < src->vertcount; n++)
			{
				if(CSG_Intersect(src, n, p)) continue;
				CSG_Split(dst, p, src->sref[n]);
				split = 1;
			}
			CSG_ASSERT(split == 1, "intersection failure");
			if(!split) continue; // for symmetry
		}
		CSG_Free(p);
	}
	CSG_POPNAME_;
}

void CSG_BooleanSub(hull_t *dst, hull_t *src)
{
	CSG_PUSHNAME("BooleanSub[HULL]");
	
	// ***************************************************
	// Subtracts volume represented by "src" hull from all
	// volumes represented by children of "dst" hull,
	// splitting them and re-polygonalizing as necessary.
	// ***************************************************
	
	for(hull_t *hnext, *h = dst->down; h; h = hnext)
	{	
		hnext = h->next;
		
		if(CSG_Intersect(h, src)) continue;	
		if(CSG_CODE_IN == 0)
		{
			int split = 0;
			
			for(poly_t *p = src->face; p; p = p->next)
			{
				if(CSG_Intersect(p, h)) continue;
				CSG_Split(dst, h, p->surf_id);
				split = 1;
			}
			if(!split) continue;
		}
		CSG_Free(h);
	}
	// FIXME: We need to write the proper polygonalizer..
	CSG_ASSERT(0, "polygonalization for boolean subtraction??");
	CSG_Polygonalize(src);
	CSG_POPNAME_;
}

void CSG_PTrim(poly_t *face)
{
	CSG_PUSHNAME("PTrim");
	
	// *****************************************************
	// Polygon trimming: used internally in CSG_Polygonalize
	// *****************************************************
	
	if(!face->down)
	{
		CSG_POPNAME_; // nothing left
	}
	hull_t *hull = (hull_t*)face->parent;
	
	if(hull->depth >= CSG_POLYGONALIZATION_DEPTH+1)
	{
		poly_t *t = NULL;
		
		for(poly_t *p = ((hull_t*)hull->parent)->face; p; p = p->next)
		{
			if(p->surf_id == face->surf_id)
			{
				t = CSG_Clone(NULL, p);
				break;
			}
		}
		if(t)
		{
			CSG_Clone(t, t);
			
			for(ccof_t *cc = CSG_CCOF[hull->depth-1]; cc; cc = cc->next)
			{
				if(CSG_Intersect(t, cc->face)) continue;
				if(CSG_CODE_IN)
				{
					CSG_Free(t);
					t = NULL;
					break;
				}
				CSG_BooleanSub(t, cc->face);
			}
			if(t)
			{
				for(poly_t *p = t->down; p; p = p->next)
				{
					CSG_BooleanSub(face, p);
				}
				CSG_Free(t);
			}
		}
	}
	for(ccof_t *cc = CSG_CCOF[hull->depth]; cc; cc = cc->next)
	{
		if(!face->down)
		{
			CSG_POPNAME_; // nothing left
		}
		if(CSG_Intersect(face, cc->face)) continue;
		CSG_BooleanSub(face, cc->face);
	}
	CSG_POPNAME_;
}

void CSG_PFlip(poly_t *poly)
{
	CSG_PUSHNAME("PFlip");
	
	// ************************************
	// Reverse the orientation of a polygon
	// ************************************
	
	for(int n = 0; n < poly->vertcount/2; n++)
	{
		int vn =     poly->vertcount - n - 1;
		int sn = n ? poly->vertcount - n : 0;
		
		vref_t vtmp = poly->vref[n];
		sref_t stmp = poly->sref[n];
		
		poly->vref[n] = poly->vref[vn], poly->vref[vn] = vtmp;
		poly->sref[n] = poly->sref[sn], poly->sref[sn] = stmp;
	}
	poly->surf_id ^= CSG_SIDE_MASK;
	CSG_POPNAME_;
}

void CSG_Polygonalize(hull_t *hull)
{
	CSG_PUSHNAME("Polygonalize");
	
	// ******************************************************
	// This function is used after each CSG boolean operation
	// to update the current polygon mesh.
	// ******************************************************
	
	for(poly_t *p = hull->face; p; p = p->next)
	{
		int i;
		
		CSG_GenCCOF(p);
		
		if(hull->depth >= CSG_POLYGONALIZATION_DEPTH+1)
		{
			hull_t *parent = (hull_t*)hull->parent;
			
			for(poly_t *q = parent->face; q; q = q->next)
			{
				if(q->surf_id != p->surf_id) continue;
				
				CSG_BooleanSub(q, p);
			}
		}
		if(hull->depth >= CSG_POLYGONALIZATION_DEPTH)
		{
			CSG_Clone(p, p);
			CSG_PTrim(p);
		}
		for(i = hull->depth; i <= CSG_MAX_DEPTH; i += 2)
		{
			for(ccof_t *cc = CSG_CCOF[i]; cc; cc = cc->next)
			{
				CSG_BooleanSub(cc->face, p);
			}
		}
		for(i = hull->depth+1; i <= CSG_MAX_DEPTH; i += 2)
		{
			for(ccof_t *cc = CSG_CCOF[i]; cc; cc = cc->next)
			{
				poly_t *a = CSG_Clone(NULL, p);
				
				CSG_PFlip(a);
				CSG_BooleanAdd(cc->face, a, 1);
				
				hull_t *parent = (hull_t*)cc->face->parent;
				
				for(hull_t *h = parent->down; h; h = h->next)
				{
					for(poly_t *q = h->face; q; q = q->next)
					{
						if(q->surf_id != cc->face->surf_id) continue;
						
						CSG_BooleanSub(cc->face, q);
						break;
					}
				}
			}
		}
	}
	CSG_POPNAME_;
}

void CSG_Check(poly_t *poly)
{
	CSG_PUSHNAME("Check[POLY]");
	
	if(poly->depth > 0)
	{
		poly_t *parent = (poly_t*)poly->parent;
		
		if(CSG_Intersect(poly, parent)) CSG_Error("Check[POLY]: child not contained");
		if(CSG_CODE_IN == 0)            CSG_Error("Check[POLY]: child not contained");
	}
	if(poly->down)
	{
		for(poly_t *p1 = poly->down; p1; p1 = p1->next)
		for(poly_t *p2 = p1->next;   p2; p2 = p2->next)
		{
			if(!CSG_Intersect(p1, p2)) CSG_Error("Check[POLY]: children not disjoint");
		}
	}
	if(poly->next && !poly->parent)
	{
		CSG_Error("Check[POLY]: poly has siblings, but no parent");
	}
	if(poly->vertcount < 3)
	{
		CSG_Error("Check[POLY]: too few vertices");
	}
	if(poly->vertcount > CSG_MAX_VERTS_PER_POLY)
	{
		CSG_Error("Check[POLY]: too many vertices");
	}
	sref_t surf_id = poly->surf_id & CSG_SURF_MASK;
	
	if(surf_id >= (vref_t)CSG_SURF_COUNT)
	{
		CSG_Error("Check[POLY]: incorrect surface reference");
	}
	for(int n = 0; n < poly->vertcount; n++)
	{
		int n2 = (n < poly->vertcount-1) ? n+1 : 0;
		
		vref_t vert_id1 = poly->vref[n];
		sref_t surf_id1 = poly->sref[n] & CSG_SURF_MASK;
		
		if(vert_id1 >= (vref_t)CSG_VERT_COUNT)
		{
			CSG_Error("Check[POLY]: incorrect vertex reference");
		}
		if(surf_id1 >= (vref_t)CSG_SURF_COUNT)
		{
			CSG_Error("Check[POLY]: incorrect surface reference");
		}
		if(surf_id1 == surf_id)
		{
			CSG_Error("Check[POLY]: duplicate surface reference");
		}
		double tt1 = CSG_Distance(vert_id1, poly->surf_id);
		double tt2 = CSG_Distance(vert_id1, poly->sref[n2]);
		double tt3 = CSG_Distance(vert_id1, poly->sref[n]);
		
		if(tt1 != 0.0 || tt2 != 0.0 || tt3 != 0.0)
		{
			CSG_Error("Check[POLY]: vertex not on surface");
		}
		for(int i = 0; i < poly->vertcount; i++)
		{
			if(i == n) continue;
			
			vref_t vert_id2 = poly->vref[i];
			sref_t surf_id2 = poly->sref[i] & CSG_SURF_MASK;
			
			if(vert_id2 == vert_id1)
			{
				CSG_Error("Check[POLY]: duplicate vertex reference");
			}
			if(surf_id2 == surf_id1)
			{
				CSG_Error("Check[POLY]: duplicate surface reference");
			}
			if(i == n2) continue;
			
			tt1 = CSG_Distance(vert_id1, poly->sref[i]);
			
			if(tt1 < 0.0)
			{
				CSG_Error("Check[POLY]: polygon not convex");
			}
			else if(tt1 == 0.0)
			{
				CSG_Messg("Check[POLY]: WARNING: polygon not strictly convex");
			}
		}
	}
	CSG_POPNAME_;
}

void CSG_Check(hull_t *hull)
{
	CSG_PUSHNAME("Check[HULL]");
	
	if(hull->depth > 0)
	{
		hull_t *parent = (hull_t*)hull->parent;
		
		if(CSG_Intersect(hull, parent)) CSG_Error("Check[HULL]: child not contained");
		if(CSG_CODE_IN == 0)            CSG_Error("Check[HULL]: child not contained");
	}
	if(hull->down)
	{
		for(hull_t *h1 = hull->down; h1; h1 = h1->next)
		for(hull_t *h2 = h1->next;   h2; h2 = h2->next)
		{
			if(!CSG_Intersect(h1, h2)) CSG_Error("Check[HULL]: children not disjoint");
		}
	}
	if(hull->next && !hull->parent)
	{
		CSG_Error("Check[HULL]: hull has siblings, but no parent");
	}
	if(hull->vertcount < 4)
	{
		CSG_Error("Check[HULL]: too few vertices");
	}
	if(hull->vertcount > CSG_MAX_VERTS_PER_HULL)
	{
		CSG_Error("Check[HULL]: too many vertices");
	}
	int facecount = CSG_Count(hull->face);
	
	if(facecount < 4)
	{
		CSG_Error("Check[HULL]: too few faces");
	}
	if(facecount > CSG_MAX_FACES_PER_HULL)
	{
		CSG_Error("Check[HULL]: too many faces");
	}
	for(poly_t *p = hull->face; p; p = p->next)
	{
		sref_t surf_id1 = p->surf_id & CSG_SURF_MASK;
		
		for(poly_t *q = hull->face; q; q = q->next)
		{
			if(q == p) continue;
			
			sref_t surf_id2 = q->surf_id & CSG_SURF_MASK;
			
			if(surf_id1 == surf_id2)
			{
				CSG_Error("Check[HULL]: duplicate surface reference");
			}
		}
		for(int n = 0; n < hull->vertcount; n++)
		{
			vref_t vert_id1 = hull->vref[n];
			
			double tt1 = CSG_Distance(vert_id1, p->surf_id);
			
			if(tt1 < 0.0)
			{
				CSG_Error("Check[HULL]: polygon not convex");
			}
			else if(tt1 == 0.0)
			{
				int i;
				
				for(i = 0; i < p->vertcount; i++)
				{
					if(p->vref[i] == vert_id1) break;
				}
				if(i == p->vertcount)
				{
					// ***********************************************
					// Vertex "vert_id1" does not exist in polygon "p"
					// ***********************************************
					
					CSG_Messg("Check[HULL]: WARNING: polygon not strictly convex");
				}
			}
		}
	}
	CSG_POPNAME_;
}

hull_t *CSG_SolidCube(double x1, double y1, double z1, double x2, double y2, double z2)
{
	CSG_PUSHNAME("SolidCube");
	
	// *****************************************************
	// This creates a solid cube from the coordinates given.
	// The vertex indices are as follows:
	// 
	//   6---7
	// 2-+-3 |
	// | | | |
	// | 4-+-5
	// 0---1
	// *****************************************************
	
	static vref_t vtab[] = {0,1,3,2, 4,6,7,5, 0,2,6,4, 1,5,7,3, 0,4,5,1, 2,3,7,6};
	static sref_t stab[] = {2,4,3,5, 4,2,5,3, 4,0,5,1, 0,4,1,5, 0,2,1,3, 2,0,3,1};
	
	hull_t *hull = CSG_Alloc(NULL);
	
	sref_t surf_id[6];
	vref_t vert_id[8];
	
	int i;
	
	for(i = 0; i < 8; i++)
	{
		vert_t vert;
		
		vert.x = (i & 1) ? x2 : x1;
		vert.y = (i & 2) ? y2 : y1;
		vert.z = (i & 4) ? z2 : z1;
		
		vert_id[i] = CSG_AddToVertPool(vert);
	}
	for(i = 0; i < 6; i++)
	{
		int i1 = vtab[i*4+0];
		int i2 = vtab[i*4+1];
		int i3 = vtab[i*4+2];
		
		surf_id[i] = CSG_AddToSurfPool(vert_id, i1, i2, i3);
	}
	for(i = 0; i < 6; i++)
	{
		poly_t *poly = CSG_Alloc(hull, 4, surf_id[i]);
		
		for(int n = 0; n < 4; n++)
		{
			vref_t vref = vert_id[vtab[n + i*4]];
			sref_t sref = surf_id[stab[n + i*4]];
			
			CSG_AddToPoly(poly, vref, sref);
		}
		CSG_Update(poly);
	}
	CSG_Update(hull);
	CSG_POPNAME(hull);
}
