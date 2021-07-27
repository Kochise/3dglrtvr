// **************************************************************************
// HCSG Demo
// (c) Bernie Freidin, 1999-2000
// **************************************************************************

#ifndef HCSG_H
#define HCSG_H

#include "vector.h"

#ifndef WIN32 //FIXME: FOR NOW: enable __MAC__ if not WIN32
#define __MAC__
#endif

// **************************************************************
// Disable debugging by setting CSG_DEBUG = 0. This will speed up
// the program, but will also fail to detect any errors until it
// is (probably) too late.
// **************************************************************

// **************************************************************
// Gradually, we would like to change all references to CSG_Error
// to CSG_ASSERT, as this is more elegant and standard.
// **************************************************************

#define CSG_DEBUG 0

#if !CSG_DEBUG
#define CSG_PUSHNAME(name)
#define CSG_POPNAME(ret) return ret
#define CSG_POPNAME_ return
#define CSG_ASSERT(cond, msg)
#define CSG_CHECK(obj)
#else
#define CSG_PUSHNAME(name) CSG_PushFunctionName(name)
#define CSG_POPNAME(ret) CSG_PopFunctionName(); return ret
#define CSG_POPNAME_ CSG_PopFunctionName(); return
#define CSG_ASSERT(cond, msg) if(!(cond)) CSG_Error(msg)
#define CSG_CHECK(obj) CSG_Check(obj)
#endif

// *********************
// Miscellaneous defines
// *********************

#define CSG_TYPE_POLY 0x00000001
#define CSG_TYPE_HULL 0x00000002
#define CSG_SURF_MASK 0x7FFFFFFF
#define CSG_SIDE_MASK 0x80000000

enum CSG_OPTYPE
{
	CSG_ADD = 1,
	CSG_SUB,
	CSG_PUSH, // used in modeler
	CSG_POP   // used in modeler
};
#define CSG_MAX_VERTS          60000
#define CSG_MAX_SURFS          10000
#define CSG_MAX_CCOFS          1000
#define CSG_MAX_VERTS_PER_HULL 100
#define CSG_MAX_VERTS_PER_POLY 100
#define CSG_MAX_FACES_PER_HULL 100
#define CSG_MAX_DEPTH          8 // maximum depth of HCSG hierarchy

#define CSG_MAXERR  0.00000001 // sensitive! default= 0.00001
#define CSG_MAXERR2 0.00000010 // sensitive! default= 0.0001

// *************************
// Geometric data structures
// *************************

typedef vec3          vert_t;
typedef unsigned long vref_t;
typedef unsigned long sref_t;

struct surf_t; // prototype
struct bbox_t; // prototype
struct ccof_t; // prototype
struct poly_t; // prototype
struct hull_t; // prototype

struct surf_t
{
	vec3   norm;
	double dist;
	int    mask;
	int    data; //FIXME: remove?
};

struct bbox_t
{
	vec3 minv;
	vec3 maxv;
};

struct ccof_t
{
	poly_t *face;
	ccof_t *next;
};

struct poly_t
{
	int     flags;
	int     depth;
	void   *parent;
	poly_t *next;
	poly_t *down;
	bbox_t  bbox;
	int     vertcount;
	vref_t *vref;
	sref_t *sref;
	sref_t  surf_id;
	void   *shape; // non-geometric data
	double  q;
};

struct hull_t
{
	int     flags;
	int     depth;
	void   *parent;
	hull_t *next;
	hull_t *down;
	poly_t *face;
	bbox_t  bbox;
	int     vertcount;
	vref_t *vref;
	void   *shape; // non-geometric data
};

// *******************
// Function prototypes
// *******************

void    CSG_Initialize(void);
hull_t *CSG_World(void);
vert_t *CSG_GetVertPool(void);
surf_t *CSG_GetSurfPool(void);
int     CSG_GetMemInUse(void *obj);
void    CSG_CheckMem(void);
void    CSG_Messg(char *msg);
void    CSG_Error(char *msg);
void    CSG_PushFunctionName(char *name);
void    CSG_PopFunctionName(void);
void    CSG_Print(poly_t *poly);
void    CSG_Print(hull_t *hull);

vref_t  CSG_AddToVertPool(vert_t &vert);
sref_t  CSG_AddToSurfPool(surf_t &surf);
sref_t  CSG_AddToSurfPool(vref_t vref[], int i1, int i2, int i3);
void    CSG_AddToPoly(poly_t *poly, vref_t vert_id, sref_t surf_id);

void    CSG_Copy(poly_t *dst, poly_t *src);
void    CSG_Copy(hull_t *dst, hull_t *src);
poly_t *CSG_Clone(void *parent, poly_t *src);
hull_t *CSG_Clone(void *parent, hull_t *src);
void   *CSG_AllocMem(int size, int nelems);
poly_t *CSG_Alloc(void *parent, int vertcount, sref_t surf_id);
hull_t *CSG_Alloc(void *parent);
void    CSG_FreeMem(void *dst);
void    CSG_Free(poly_t *poly);
void    CSG_Free(hull_t *hull);

void    CSG_Unlink(poly_t *poly);
void    CSG_Unlink(hull_t *hull);
void    CSG_Link(void *parent, poly_t *poly);
void    CSG_Link(void *parent, hull_t *hull);
int     CSG_Count(poly_t *poly);
int     CSG_Count(hull_t *hull);
int     CSG_Count(ccof_t *ccof);

void    CSG_SetPolygonalizationDepth(int depth);

void    CSG_GenCCOF(poly_t *face);
void    CSG_GenCCOF(poly_t *face, hull_t *hull);

void    CSG_Update(bbox_t *bbox, vref_t vref[], int vertcount);
void    CSG_Update(poly_t *poly);
void    CSG_Update(hull_t *hull);
void    CSG_Update(hull_t *hull, sref_t surf_id);

int     CSG_In(vref_t vref[], int vertcount, sref_t surf_id);
int     CSG_Intersect(bbox_t *bbox1, bbox_t *bbox2, int mask);
int     CSG_Intersect(poly_t *poly1, sref_t surf_id);
int     CSG_Intersect(hull_t *hull1, sref_t surf_id);
int     CSG_Intersect(poly_t *poly1, poly_t *poly2);
int     CSG_Intersect(hull_t *hull1, hull_t *hull2);
int     CSG_Intersect(poly_t *poly1, hull_t *hull2);
int     CSG_Intersect(poly_t *poly1, int edge1, poly_t *poly2);

double  CSG_Distance(vert_t &vert,   sref_t surf_id);
double  CSG_Distance(vref_t vert_id, sref_t surf_id);

int     CSG_Clip(poly_t *poly, sref_t surf_id);
int     CSG_Clip(hull_t *hull, sref_t surf_id);
int     CSG_Clip(poly_t *poly, poly_t *clip);
int     CSG_Clip(hull_t *hull, hull_t *clip);
poly_t *CSG_Split(void *parent, poly_t *poly, sref_t surf_id);
hull_t *CSG_Split(void *parent, hull_t *hull, sref_t surf_id);

void    CSG_BooleanOp(hull_t *dst, hull_t *src, int target_depth, int op, int clip);
void    CSG_BooleanAdd(poly_t *dst, poly_t *src, int clip);
void    CSG_BooleanAdd(hull_t *dst, hull_t *src, int clip);
void    CSG_BooleanSub(poly_t *dst, poly_t *src);
void    CSG_BooleanSub(hull_t *dst, hull_t *src);

void    CSG_PTrim(poly_t *face);
void    CSG_PFlip(poly_t *poly);
void    CSG_Polygonalize(hull_t *hull);

void    CSG_Check(poly_t *poly);
void    CSG_Check(hull_t *hull);

hull_t *CSG_SolidCube(double x1, double y1, double z1, double x2, double y2, double z2);

#endif
