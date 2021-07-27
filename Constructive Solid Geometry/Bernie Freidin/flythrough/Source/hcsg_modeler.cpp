// **************************************************************************
// HCSG Demo
// (c) Bernie Freidin, 1999-2000
// **************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "strtok_r.h"
#include "hcsg_modeler.h"

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

// ****************************************************************************
// === SHAPES =================================================================
// ****************************************************************************

enum CSG_SHAPETYPE
{
	CSG_SHAPETYPE_UNDEFINED = 0,
	CSG_SHAPETYPE_POLYGON
};

struct shape_t
{
	int      type;
	char     name[24];
	int      vertcount;
	vec2    *vert;
	shape_t *next;
};

// ****************************************************************************
// === COORDS =================================================================
// ****************************************************************************

struct coords_t
{
	mat4 transform2; // final transformation
	mat4 transform1; // stack transformation
	mat4 local;      // local transformation
	vec3 minv;       // bounding box lower corner
	vec3 maxv;       // bounding box upper corner
	mat4 axis;       // axis transform
};

// ****************************************************************************
// === RECURSION ==============================================================
// ****************************************************************************

struct recurs_t
{
	int counter;
	int marker;
};

// ****************************************************************************
// === EVENTS =================================================================
// ****************************************************************************

struct event_t
{
	int       type; // uses CSG_OPTYPE
	int       depth;
	coords_t  coords;
	shape_t  *shape;
	event_t  *next;
};

// *********************************************************
// Notes on EVENT structure:
// 
// When a brush is added or subtracted, an EVENT entry is
// created and added into the event list. A pointer to the
// current (or specified) brush is stored alongside a -copy-
// of the current coordinate frame.
// 
// When PUSHCOORDS or POPCOORDS actions are invoked, an
// EVENT entry is created and added into the event list. A
// -copy- of the current coordinate frame is stored in the
// EVENT entry, but the brush field is left as NULL.
// 
// This makes it easy to re-position a single brush within
// the geometry by changing the associated coordinate frame.
// It also makes it easy to re-position a whole group of
// primitives by inserting PUSHCOORDS and POPCOORDS events
// into the list.
// *********************************************************

// ****************************************************************************
// === TAGS ===================================================================
// ****************************************************************************

enum CSG_TAG
{
	CSG_TAG_X = 1,
	CSG_TAG_Y,
	CSG_TAG_Z,
	CSG_TAG_X1,
	CSG_TAG_X2,
	CSG_TAG_Y1,
	CSG_TAG_Y2,
	CSG_TAG_Z1,
	CSG_TAG_Z2
};

struct tag_t
{
	char s1[4];
	char s2[4];
	int id;
};

static tag_t CSG_TAG_LIST[] = {
	{"x",   "",   CSG_TAG_X},
	{"y",   "",   CSG_TAG_Y},
	{"z",   "",   CSG_TAG_Z},
	{"bot", "y1", CSG_TAG_Y1},
	{"top", "y2", CSG_TAG_Y2},
	{"lft", "x1", CSG_TAG_X1},
	{"rgt", "x2", CSG_TAG_X2},
	{"fnt", "z1", CSG_TAG_Z1},
	{"bak", "z2", CSG_TAG_Z2},
	{"",    "",   -1}
};

// ****************************************************************************
// === COMMANDS ===============================================================
// ****************************************************************************

enum CSG_COMMAND_FORMAT
{
	CSG_COMMAND_FORMAT_UNSPECIFIED = 0,
	CSG_COMMAND_FORMAT1, // <COMMAND>
	CSG_COMMAND_FORMAT2, // <COMMAND>=<VALUE>
	CSG_COMMAND_FORMAT3, // <COMMAND>=(<VALUE0> <VALUE1> ... <VALUEn>)
	CSG_COMMAND_FORMAT4, // <COMMAND> {"<NAME>",*}
	CSG_COMMAND_FORMAT5  // <COMMAND> {"<NAME>",*}=(<V0> <V1> ... <Vn>)
};

enum CSG_COMMAND
{
	CSG_COMMAND_NOP = 0,
	CSG_COMMAND_RUNSCRIPT,
	//
	CSG_COMMAND_PUSHCOORDS,
	CSG_COMMAND_POPCOORDS,
	CSG_COMMAND_MATRIX,
	CSG_COMMAND_IDENTITY,
	CSG_COMMAND_BBOX,
	CSG_COMMAND_STEPBBOX,
	CSG_COMMAND_AXIS,
	CSG_COMMAND_ROTATE,
	CSG_COMMAND_ROTATEAXIS,
	CSG_COMMAND_TRANSLATE,
	CSG_COMMAND_SCALE,
	CSG_COMMAND_SHEAR,
	CSG_COMMAND_PREROTATE,
	CSG_COMMAND_PREROTATEAXIS,
	CSG_COMMAND_PRETRANSLATE,
	CSG_COMMAND_PRESCALE,
	CSG_COMMAND_PRESHEAR,
	//
	CSG_COMMAND_SHAPE,
	CSG_COMMAND_SPIRALSTAIR1,
	CSG_COMMAND_REGULARPOLY1,
	CSG_COMMAND_FREESHAPE,
	//
	CSG_COMMAND_DEPTH,
	CSG_COMMAND_CSGADD,
	CSG_COMMAND_CSGSUB,
	//
	CSG_COMMAND_LOOP,
	CSG_COMMAND_ENDLOOP,
	CSG_COMMAND_RETURN
};

static void CSG_ExecNOP(char *paramstr[]);
static void CSG_ExecRUNSCRIPT(char *paramstr[]);
//
static void CSG_ExecPUSHCOORDS(char *paramstr[]);
static void CSG_ExecPOPCOORDS(char *paramstr[]);
static void CSG_ExecMATRIX(char *paramstr[]);
static void CSG_ExecIDENTITY(char *paramstr[]);
static void CSG_ExecBBOX(char *paramstr[]);
static void CSG_ExecSTEPBBOX(char *paramstr[]);
static void CSG_ExecAXIS(char *paramstr[]);
static void CSG_ExecROTATE(char *paramstr[]);
static void CSG_ExecROTATEAXIS(char *paramstr[]);
static void CSG_ExecTRANSLATE(char *paramstr[]);
static void CSG_ExecSCALE(char *paramstr[]);
static void CSG_ExecSHEAR(char *paramstr[]);
static void CSG_ExecPREROTATE(char *paramstr[]);
static void CSG_ExecPREROTATEAXIS(char *paramstr[]);
static void CSG_ExecPRETRANSLATE(char *paramstr[]);
static void CSG_ExecPRESCALE(char *paramstr[]);
static void CSG_ExecPRESHEAR(char *paramstr[]);
//
static void CSG_ExecSHAPE(char *paramstr[]);
static void CSG_ExecSPIRALSTAIR1(char *paramstr[]);
static void CSG_ExecREGULARPOLY1(char *paramstr[]);
static void CSG_ExecFREESHAPE(char *paramstr[]);
//
static void CSG_ExecDEPTH(char *paramstr[]);
static void CSG_ExecCSGADD(char *paramstr[]);
static void CSG_ExecCSGSUB(char *paramstr[]);
//
static void CSG_ExecLOOP(char *paramstr[]);
static void CSG_ExecENDLOOP(char *paramstr[]);

typedef void vf_t(char *paramstr[]);
struct command_t
{
	char  name[24];
	int   format;
	int   paramcount; // -1 indicates variable-length list
	int   index;
	vf_t *func;
};

static command_t CSG_COMMAND_LIST[] = {
	{"nop",           1,  0, CSG_COMMAND_NOP,           CSG_ExecNOP}, // no-op
	{"runscript",     4,  0, CSG_COMMAND_RUNSCRIPT,     CSG_ExecRUNSCRIPT},
	//
	{"pushcoords",    1,  0, CSG_COMMAND_PUSHCOORDS,    CSG_ExecPUSHCOORDS},
	{"popcoords",     1,  0, CSG_COMMAND_POPCOORDS,     CSG_ExecPOPCOORDS},
	{"matrix",        3, 16, CSG_COMMAND_MATRIX,        CSG_ExecMATRIX},
	{"identity",      1,  0, CSG_COMMAND_IDENTITY,      CSG_ExecIDENTITY},
	{"bbox",          3,  6, CSG_COMMAND_BBOX,          CSG_ExecBBOX},
	{"stepbbox",      3, -1, CSG_COMMAND_STEPBBOX,      CSG_ExecSTEPBBOX},
	{"axis",          2,  1, CSG_COMMAND_AXIS,          CSG_ExecAXIS},
	{"rotate",        3, -1, CSG_COMMAND_ROTATE,        CSG_ExecROTATE},
	{"rotateaxis",    3,  4, CSG_COMMAND_ROTATEAXIS,    CSG_ExecROTATEAXIS},
	{"translate",     3,  3, CSG_COMMAND_TRANSLATE,     CSG_ExecTRANSLATE},
	{"scale",         3,  3, CSG_COMMAND_SCALE,         CSG_ExecSCALE},
	{"shear",         3, -1, CSG_COMMAND_SHEAR,         CSG_ExecSHEAR},
	{"prerotate",     3, -1, CSG_COMMAND_PREROTATE,     CSG_ExecPREROTATE},
	{"prerotateaxis", 3,  4, CSG_COMMAND_PREROTATEAXIS, CSG_ExecPREROTATEAXIS},
	{"pretranslate",  3,  3, CSG_COMMAND_PRETRANSLATE,  CSG_ExecPRETRANSLATE},
	{"prescale",      3,  3, CSG_COMMAND_PRESCALE,      CSG_ExecPRESCALE},
	{"preshear",      3, -1, CSG_COMMAND_PRESHEAR,      CSG_ExecPRESHEAR},
	//
	{"shape",         5, -1, CSG_COMMAND_SHAPE,         CSG_ExecSHAPE},
	{"spiralstair1",  5,  4, CSG_COMMAND_SPIRALSTAIR1,  CSG_ExecSPIRALSTAIR1},
	{"regularpoly1",  5,  3, CSG_COMMAND_REGULARPOLY1,  CSG_ExecREGULARPOLY1},
	{"freeshape",     4,  0, CSG_COMMAND_FREESHAPE,     CSG_ExecFREESHAPE},
	//
	{"depth",         2,  1, CSG_COMMAND_DEPTH,         CSG_ExecDEPTH},
	{"csgadd",        4,  0, CSG_COMMAND_CSGADD,        CSG_ExecCSGADD},
	{"csgsub",        4,  0, CSG_COMMAND_CSGSUB,        CSG_ExecCSGSUB},
	//
	{"loop",          2,  1, CSG_COMMAND_LOOP,          CSG_ExecLOOP},
	{"endloop",       1,  0, CSG_COMMAND_ENDLOOP,       CSG_ExecENDLOOP},
	{"return",        1,  0, CSG_COMMAND_RETURN,        CSG_ExecNOP},
	{"",             -1, -1, -1, NULL}
};

// ****************************************************************************
// === GLOBALS/PROTOTYPES =====================================================
// ****************************************************************************

#define CSG_MAX_COORDS_DEPTH   10
#define CSG_MAX_RECURS_DEPTH   10
#define CSG_MAX_COMMAND_PARAMS 4
#define CSG_MAX_ARRAY_SIZE     200 // allows for 100-sided polygons

static coords_t CSG_COORDS_STACK[CSG_MAX_COORDS_DEPTH];
static int      CSG_COORDS_DEPTH;
static recurs_t CSG_RECURS_STACK[CSG_MAX_RECURS_DEPTH];
static int      CSG_RECURS_DEPTH;
static int      CSG_CURRENT_DEPTH;
static shape_t *CSG_CURRENT_SHAPE;
static FILE    *CSG_CURRENT_FILE;
static shape_t *CSG_SHAPE_LIST;
static event_t *CSG_EVENT_LIST;
static hull_t  *CSG_WORLD; // shadow variable
static int      CSG_SHAPE_ID; // unique id
static int      CSG_REVERSE_ORDER = 0; // internal flag

static int      CSG_ParseTag(char *str);
static double   CSG_ParseScalarValue(char *str);
static int      CSG_ParseScalarArray(char *str, double *array);
static char    *CSG_ParseParamString(char **str, char markers[3]);
static shape_t *CSG_FindShape(char name[], int create);
static void     CSG_FreeShape(shape_t *shape);
static event_t *CSG_Event(int type, shape_t *shape);
static void     CSG_Update(coords_t *coords);
static hull_t  *CSG_Extrude(shape_t *shape);

// ****************************************************************************
// === FUNCTIONS ==============================================================
// ****************************************************************************

void CSG_InitializeModeler(hull_t *world)
{
	CSG_PUSHNAME("InitializeModeler");
	
	// *********************************************
	// Allocates a new coordinate stack and sets the
	// stack depth to zero. Clears all shapes and
	// brushes.
	// *********************************************
	
	CSG_COORDS_DEPTH  = 2;
	CSG_RECURS_DEPTH  = 0;
	CSG_CURRENT_DEPTH = 0;
	CSG_CURRENT_SHAPE = NULL;
	CSG_CURRENT_FILE  = NULL;
	CSG_SHAPE_LIST    = NULL; // dynamic pool
	CSG_EVENT_LIST    = NULL; // dynamic pool
	CSG_WORLD         = world;
	CSG_SHAPE_ID      = 0;
	
	// ********************************
	// Initialize two coordinate frames
	// ********************************
	
	for(int i = 0; i < 2; i++)
	{
		coords_t *coords = &CSG_COORDS_STACK[i];
		
		coords->transform1.identity();
		coords->transform2.identity();
		coords->axis.identity();
		coords->local.identity();
		coords->minv = vec3(-1.0, -1.0, -1.0);
		coords->maxv = vec3(+1.0, +1.0, +1.0);
	}
	CSG_POPNAME_;
}

int CSG_GetMemInUse_Modeler(void)
{
	CSG_PUSHNAME("GetMemInUse_Modeler");
	
	// ********************************************************
	// Returns the amount of memory used by the modeler engine
	// ********************************************************
	
	int mem = 0;
	
	if(!CSG_COORDS_STACK)
	{
		CSG_POPNAME(0); // assume modeler is uninitialized
	}
	for(shape_t *s = CSG_SHAPE_LIST; s; s = s->next)
	{
		mem += sizeof(shape_t);
		mem += sizeof(vec2) * s->vertcount;
	}
	for(event_t *e = CSG_EVENT_LIST; e; e = e->next)
	{
		mem += sizeof(event_t);
	}
	CSG_POPNAME(mem);
}

void CSG_RunScript(char *fname)
{
	CSG_PUSHNAME("RunScript");
	
	// ***********************************************
	// Opens a file and parses the contents, executing
	// commands as specified.
	// ***********************************************
	
	char linebuf[4001];
	int brk = 0;
	FILE *fp = fopen(fname, "r");
	
	if(!fp)
	{
		CSG_POPNAME_; // safe
	}
	CSG_CURRENT_FILE = fp;
	
	while(!brk && fgets(linebuf, 4000, fp))
	{
		char *s1 = linebuf;
		
		while(!brk)
		{
			char *cmdstr = strtok_r(s1, " =\t\n\r", &s1);
			int i;
			
			if(!cmdstr) break; // no more commands
			if(cmdstr[0] == '/' &&
			   cmdstr[1] == '/') break; // comment (C++ style)
			if(cmdstr[0] == '#') break; // comment
			if(cmdstr[0] == ';') break; // comment
			
			// ***********************
			// Search command database
			// ***********************
			
			command_t *command;
			
			int len = strlen(cmdstr);
			for(i = 0; i < len; i++)
			{
				cmdstr[i] = tolower(cmdstr[i]);
			}
			for(i = 0;; i++)
			{
				command = &CSG_COMMAND_LIST[i];
				if(!strcmp(cmdstr, command->name)) break;
				CSG_ASSERT(command->index >= 0, "unknown command");
			}
			// ***************
			// Find parameters
			// ***************
			
			char *paramstr[CSG_MAX_COMMAND_PARAMS];
			
			switch(command->format)
			{
				case CSG_COMMAND_FORMAT1: break;
				case CSG_COMMAND_FORMAT2:
					paramstr[0] = strtok_r(s1, " =:\t\n\r", &s1);
					paramstr[1] = NULL;
					break;
				case CSG_COMMAND_FORMAT3:
					paramstr[0] = CSG_ParseParamString(&s1, "[]");
					paramstr[1] = NULL;
					break;
				case CSG_COMMAND_FORMAT4:
					paramstr[0] = CSG_ParseParamString(&s1, "\"\"*");
					paramstr[1] = NULL;
					break;
				case CSG_COMMAND_FORMAT5:
					paramstr[0] = CSG_ParseParamString(&s1, "\"\"*");
					paramstr[1] = CSG_ParseParamString(&s1, "[]");
					break;
				default:
					CSG_ASSERT(0, "unknown command format");
			}
			// ***************
			// Execute command
			// ***************
			
			if(command->index == CSG_COMMAND_RETURN) brk = 1;
			command->func(paramstr);
		}
	}
	fclose(fp);
	CSG_POPNAME_;
}

static int CSG_ParseTag(char *str)
{
	CSG_PUSHNAME("ParseTag");
	
	// ************
	// Finds tag id
	// ************
	
	CSG_ASSERT(str, "parse error");
	
	int i, len = strlen(str);
	
	for(i = 0; i < len; i++)
	{
		str[i] = tolower(str[i]);
	}
	for(i = 0;; i++)
	{
		if(CSG_TAG_LIST[i].id < 0) break;	
		if(!strcmp(CSG_TAG_LIST[i].s1, str) ||
		   !strcmp(CSG_TAG_LIST[i].s2, str) )
		{
			CSG_POPNAME(CSG_TAG_LIST[i].id);
		}
	}
	CSG_POPNAME(0);
}

static double CSG_ParseScalarValue(char *str)
{
	CSG_PUSHNAME("ParseScalarValue");
	
	// ************************************************
	// Parses a scalar value. Currently, we handle only
	// the forms <k>, <k*d>, and <k/d>.
	// ************************************************
	
	CSG_ASSERT(str, "parse error");
	
	char *symbolstr;
	
	if(symbolstr = strchr(str, '*'))
	{
		CSG_POPNAME(atof(str)*atof(symbolstr+1));
	}
	if(symbolstr = strchr(str, '/'))
	{
		CSG_POPNAME(atof(str)/atof(symbolstr+1));
	}
	CSG_POPNAME(atof(str));
}

static int CSG_ParseScalarArray(char *str, double *array)
{
	CSG_PUSHNAME("ParseScalarArray");
	CSG_ASSERT(str, "parse error");
	
	char *s1 = str;
	char *s2 = strtok_r(s1, " ,:(){}\t\n\r", &s1);
	int k = 0;
	
	while(s2)
	{
		CSG_ASSERT(k < CSG_MAX_ARRAY_SIZE, "array too big");
		array[k++] = CSG_ParseScalarValue(s2);
		s2 = strtok_r(s1, " ,:(){}\t\n\r", &s1);
	}
	CSG_POPNAME(k);
}

static char *CSG_ParseParamString(char **str, char markers[3])
{
	CSG_PUSHNAME("ParseParamString");
	
	// *******************************************************
	// Isolates a parameter list or name surrounded by markers
	// markers[0] and markers[1]. If markers[2] is specified,
	// the function returns NULL if marker[2] is found first.
	// *******************************************************
	
	CSG_ASSERT(*str, "parse error");
	CSG_ASSERT(strlen(markers) >= 2, "invalid marker string");
	
	char *s1 = strchr(*str, markers[0]);
	char *s2 = strchr(*str, markers[2]);
	
	if(markers[2] && s2 && (s2 < s1 || !s1))
	{
		// check for garbage
		//FIXME: check??
		//CSG_ASSERT(!strtok(*str, " =\t\n\r"), "parse error");
		*str = s2+1;
		CSG_POPNAME(NULL);
	}
	CSG_ASSERT(s1, "parse error"); // no beginning marker
	s1[0] = '\0'; s1++;
	CSG_ASSERT(!strtok(*str, " =\t\n\r"), "parse error"); // garbage
	s2 = strchr(s1, markers[1]);
	CSG_ASSERT(s2, "parse error"); // no ending marker
	s2[0] = '\0'; s2++;
	*str = s2;
	CSG_POPNAME(s1);
}

static shape_t *CSG_FindShape(char name[], int create)
{
	CSG_PUSHNAME("FindShape");
	
	// **********************************************
	// Finds a shape with the specified name. If none
	// exists, a new shape is created if create=1. If
	// name is NULL and create=1, a uniquely-named
	// shape is created. If name is NULL and create=0
	// the current shape is returned.
	// **********************************************
	
	shape_t *sh = NULL;
	
	if(!name)
	{
		if(!create)
		{
			CSG_POPNAME(CSG_CURRENT_SHAPE);
		}
	}
	else for(sh = CSG_SHAPE_LIST; sh; sh = sh->next)
	{
		if(!strcmp(name, sh->name)) break;
	}
	if(!sh)
	{
		CSG_ASSERT(create, "can't find shape");
		
		// ******************
		// Allocate new shape
		// ******************
		
		sh = (shape_t*)CSG_AllocMem(sizeof(shape_t), 1);
		
		if(name)
		{
			strncpy(sh->name, name, 23);
		}
		else
		{
			sprintf(sh->name, "____________SHAPE%i", CSG_SHAPE_ID++);
		}
		// *******************
		// Link into main list
		// *******************
		
		sh->next       = CSG_SHAPE_LIST;
		CSG_SHAPE_LIST = sh;
	}
	CSG_POPNAME(sh);
}

static event_t *CSG_Event(int type, shape_t *shape)
{
	CSG_PUSHNAME("Event");
	
	// *************************************************
	// Creates a new event with the specified type and
	// brush and copies the current coordinate frame
	// into it. The event is appended to the event list.
	// *************************************************
	
	event_t *event = (event_t*)CSG_AllocMem(sizeof(event_t), 1);
	
	event->type    = type;
	event->shape   = shape;
	event->depth   = CSG_CURRENT_DEPTH;
	event->coords  = CSG_COORDS_STACK[CSG_COORDS_DEPTH-1];
	event->next    = CSG_EVENT_LIST;
	CSG_EVENT_LIST = event;
	
	CSG_POPNAME(event);
}

static void CSG_Copy(shape_t *dst, shape_t *src)
{
	CSG_PUSHNAME("Copy[SHAPE]");
	
	// ***************
	// Copy shape data
	// ***************
	
	shape_t *next = dst->next; // save pointer
	
	if(dst->vert) CSG_FreeMem(dst->vert);
	
	memcpy(dst, src, sizeof(shape_t));
	
	if(src->vert)
	{
		dst->vert = (vec2*)CSG_AllocMem(sizeof(vec2), src->vertcount);
		memcpy(dst->vert, src->vert, sizeof(vec2) * src->vertcount);
	}
	dst->next = next;
	CSG_POPNAME_;
}

static void CSG_FreeShape(shape_t *shape)
{
	CSG_PUSHNAME("FreeShape");
	
	if(CSG_SHAPE_LIST == shape)
	{
		CSG_SHAPE_LIST = shape->next;
	}
	else
	{
		for(shape_t *s = CSG_SHAPE_LIST; s; s = s->next)
		{
			if(s->next == shape)
			{
				s->next = shape->next;
				break;
			}
		}
		CSG_ASSERT(0, "shape missing from list");
	}
	CSG_FreeMem(shape->vert);
	CSG_FreeMem(shape);
	CSG_POPNAME_;
}

static void CSG_Update(coords_t *coords)
{
	CSG_PUSHNAME("Update[COORDS]");
	
	coords_t *coprev = &CSG_COORDS_STACK[CSG_COORDS_DEPTH-2];
	CSG_ASSERT(CSG_COORDS_DEPTH > 1, "coordinate stack underflow");
	CSG_ASSERT(coords == coprev+1, "tried to modify non-current matrix");
	
	mat4 bbox;
	bbox.identity();
	
	for(int i = 0; i < 3; i++)
	{
		bbox[i][i] = (coords->maxv[i] - coords->minv[i]) / 2.0;
		bbox[i][3] = (coords->maxv[i] + coords->minv[i]) / 2.0;
	}
	coords->transform1 = coprev->transform1 * coords->local;
	coords->transform2 = coords->transform1 * bbox * coords->axis;
	CSG_POPNAME_;
}

static hull_t *CSG_Extrude(shape_t *shape)
{
	CSG_PUSHNAME("Extrude");
	
	// ***************************************************
	// Extrude a specified shape to a HCSG hull (using the
	// current coordinate frame).
	// ***************************************************
	
	coords_t *coords = &CSG_COORDS_STACK[CSG_COORDS_DEPTH-1];
	
	vref_t vref[CSG_MAX_VERTS_PER_HULL];
	sref_t sref[CSG_MAX_FACES_PER_HULL];
	
	int n, vc = shape->vertcount;
	
	// ****************
	// Compute vertices
	// ****************
	
	for(n = 0; n < vc; n++)
	{
		vec3 v1 = vec3(shape->vert[n].x, shape->vert[n].y, -1.0);
		vec3 v2 = vec3(shape->vert[n].x, shape->vert[n].y, +1.0);
		v1 = coords->transform2*v1;
		v2 = coords->transform2*v2;
		vref[n]    = CSG_AddToVertPool(v1);
		vref[n+vc] = CSG_AddToVertPool(v2);
	}
	// ****************
	// Compute surfaces
	// ****************
	
	for(n = 0; n < vc; n++)
	{
		sref[n] = CSG_AddToSurfPool(vref, (n<vc-1)?n+1:0, vc+n, n);
	}
	sref[vc+0] = CSG_AddToSurfPool(vref, 2, 1, 0);
	sref[vc+1] = CSG_AddToSurfPool(vref, vc, vc+1, vc+2);
	
	// *************
	// Allocate hull
	// *************
	
	hull_t *hull = CSG_Alloc(NULL);
	poly_t *face = CSG_Alloc(hull, vc, sref[vc]);
	
	for(n = 0; n < vc; n++)
	{
		CSG_AddToPoly(face, vref[n], sref[n>0?n-1:vc-1]);
	}
	CSG_Update(face);
	
	face = CSG_Alloc(hull, vc, sref[vc+1]);
	
	for(n = vc-1; n >= 0; n--)
	{
		CSG_AddToPoly(face, vref[vc+n], sref[n]);
	}
	CSG_Update(face);
	
	for(n = 0; n < vc; n++)
	{
		int n1 = (n < vc-1) ? n+1 : 0;
		int n2 = (n > 0) ? n-1 : vc-1;
		
		face = CSG_Alloc(hull, 4, sref[n]);
		
		CSG_AddToPoly(face, vref[n1],    sref[n1]);
		CSG_AddToPoly(face, vref[n],     sref[vc]);
		CSG_AddToPoly(face, vref[vc+n],  sref[n2]);
		CSG_AddToPoly(face, vref[vc+n1], sref[vc+1]);
		
		CSG_Update(face);
	}
	CSG_Update(hull);
	hull->shape = shape;
	CSG_POPNAME(hull);
}

static void CSG_ExecNOP(char *paramstr[])
{
	CSG_PUSHNAME("ExecNOP");
	CSG_POPNAME_;
}

static void CSG_ExecRUNSCRIPT(char *paramstr[])
{
	CSG_PUSHNAME("ExecRUNSCRIPT");
	
	// ****************
	// Run a new script
	// ****************
	
	FILE *save = CSG_CURRENT_FILE;
	CSG_RunScript(paramstr[0]);
	CSG_CURRENT_FILE = save;
	CSG_POPNAME_;
}

static void CSG_ExecPUSHCOORDS(char *paramstr[])
{
	CSG_PUSHNAME("ExecPUSHCOORDS");
	
	// ******************************************************
	// Create a new coordinate frame (identity) and push this
	// onto the coordinate stack.
	// 
	// Note that the preferred axis is reset to Z by this
	// function (this is slightly counter-intuitive, if the
	// preferred axis is thought of as a global state).
	// ******************************************************
	
	CSG_ASSERT(CSG_COORDS_DEPTH > 1,
	"coordinate stack underflow");
	CSG_ASSERT(CSG_COORDS_DEPTH < CSG_MAX_COORDS_DEPTH,
	"coordinate stack overflow");
	CSG_Event(CSG_PUSH, NULL);
	
	CSG_COORDS_DEPTH++;
	
	coords_t *coprev = &CSG_COORDS_STACK[CSG_COORDS_DEPTH-2];
	coords_t *coords = &CSG_COORDS_STACK[CSG_COORDS_DEPTH-1];
	
	memcpy(coords, coprev, sizeof(coords_t));
	coords->local.identity();
		
	CSG_Update(coords);
	CSG_POPNAME_;
}

static void CSG_ExecPOPCOORDS(char *paramstr[])
{
	CSG_PUSHNAME("ExecPOPCOORDS");
	
	// ****************************************************
	// Pop a coordinate frame from the stack. Note that the
	// stack depth must never fall below 2 (not 1).
	// ****************************************************
	
	CSG_ASSERT(CSG_COORDS_DEPTH > 1, "coordinate stack underflow");
	CSG_Event(CSG_POP, NULL);
	CSG_COORDS_DEPTH--;
	CSG_POPNAME_;
}

static void CSG_ExecMATRIX(char *paramstr[])
{
	CSG_PUSHNAME("ExecMATRIX");
	
	coords_t *coords = &CSG_COORDS_STACK[CSG_COORDS_DEPTH-1];
	double tmp[CSG_MAX_ARRAY_SIZE];	
	int count = CSG_ParseScalarArray(paramstr[0], tmp);
	CSG_ASSERT(count == 16, "incorrect array size");
	memcpy(&coords->local, tmp, sizeof(mat4));
	CSG_Update(coords);
	CSG_POPNAME_;
}

static void CSG_ExecIDENTITY(char *paramstr[])
{
	CSG_PUSHNAME("ExecIDENTITY");
	
	coords_t *coords = &CSG_COORDS_STACK[CSG_COORDS_DEPTH-1];
	coords->local.identity();
	CSG_Update(coords);
	CSG_POPNAME_;
}

static void CSG_ExecBBOX(char *paramstr[])
{
	CSG_PUSHNAME("ExecBBOX");
	
	// *************************************************
	// Change the current coordinate frame so that the
	// coordinates (-1,-1,-1) to (1,1,1) fall within the
	// specified bounding box.
	// *************************************************
	
	coords_t *coords = &CSG_COORDS_STACK[CSG_COORDS_DEPTH-1];
	double tmp[CSG_MAX_ARRAY_SIZE];
	int count = CSG_ParseScalarArray(paramstr[0], tmp);
	CSG_ASSERT(count == 6, "incorrect array size");
	CSG_ASSERT(tmp[0] < tmp[3], "invalid coordinates");
	CSG_ASSERT(tmp[1] < tmp[4], "invalid coordinates");
	CSG_ASSERT(tmp[2] < tmp[5], "invalid coordinates");
	coords->minv = vec3(tmp[0], tmp[1], tmp[2]);
	coords->maxv = vec3(tmp[3], tmp[4], tmp[5]);
	CSG_Update(coords);
	CSG_POPNAME_;
}

static void CSG_ExecSTEPBBOX(char *paramstr[])
{
	CSG_PUSHNAME("ExecSTEPBBOX");
	
	char *s1 = paramstr[0];
	char *s2 = strtok_r(s1, " ,:[]{}\t\n\r", &s1);
	
	coords_t *coords = &CSG_COORDS_STACK[CSG_COORDS_DEPTH-1];
	
	while(s2)
	{
		int    side;
		double delta;
		
		side = CSG_ParseTag(s2);
		s2 = strtok_r(s1, " ,:[]{}\t\n\r", &s1);
		delta = CSG_ParseScalarValue(s2);
		s2 = strtok_r(s1, " ,:[]{}\t\n\r", &s1);
		
		switch(side)
		{
			case CSG_TAG_X1: coords->minv.x += delta; break;
			case CSG_TAG_Y1: coords->minv.y += delta; break;
			case CSG_TAG_Z1: coords->minv.z += delta; break;
			case CSG_TAG_X2: coords->maxv.x += delta; break;
			case CSG_TAG_Y2: coords->maxv.y += delta; break;
			case CSG_TAG_Z2: coords->maxv.z += delta; break;
			default: CSG_ASSERT(0, "invalid tag");
		}
	}
	CSG_Update(coords);
	CSG_POPNAME_;
}

static void CSG_ExecAXIS(char *paramstr[])
{
	CSG_PUSHNAME("ExecAXIS");
	
	coords_t *coords = &CSG_COORDS_STACK[CSG_COORDS_DEPTH-1];
	
	coords->axis.zero();
	coords->axis.w.w = 1.0;
	
	switch(CSG_ParseTag(paramstr[0]))
	{
		case CSG_TAG_X:
			coords->axis.x.z = -1.0;
			coords->axis.y.y = +1.0;
			coords->axis.z.x = +1.0;
			break;
		case CSG_TAG_Y:
			coords->axis.x.x = +1.0;
			coords->axis.y.z = +1.0;
			coords->axis.z.y = -1.0;
			break;
		case CSG_TAG_Z:
			coords->axis.x.x = +1.0;
			coords->axis.y.y = +1.0;
			coords->axis.z.z = +1.0;
			break;
		default:
			CSG_ASSERT(0, "invalid axis; must be X, Y, or Z");
	}
	CSG_Update(coords);
	CSG_POPNAME_;
}

static void CSG_ExecROTATE(char *paramstr[])
{
	CSG_PUSHNAME("ExecROTATE");
	
	coords_t *coords = &CSG_COORDS_STACK[CSG_COORDS_DEPTH-1];
	
	char *s1 = paramstr[0];
	char *s2 = strtok_r(s1, " ,:[]{}\t\n\r", &s1);
	
	mat4 m; m.identity();
	
	while(s2)
	{
		int    axis;
		double theta;
		
		axis = CSG_ParseTag(s2);
		s2 = strtok_r(s1, " ,:[]{}\t\n\r", &s1);
		theta = CSG_ParseScalarValue(s2);
		s2 = strtok_r(s1, " ,:[]{}\t\n\r", &s1);
		
		// **********************************************************
		// NOTE: Successive rotations can introduce floating-point
		// errors, so avoid unecessary rotations when possible (i.e.,
		// use PUSHCOORDS/POPCOORDS instead of rotating backwards).
		// **********************************************************
		
		while(theta >  +180.0) theta -= 360.0;
		while(theta <= -180.0) theta += 360.0;
		if(theta == 0.0) continue;
		
		double c, s;
		
		if(theta == -90.0) c =  0.0, s = -1.0; else
		if(theta == +90.0) c =  0.0, s = +1.0; else
		if(theta == 180.0) c = -1.0, s =  0.0; else
		{
			c = cos(theta*PI/180.0);
			s = sin(theta*PI/180.0);
		}
		switch(axis)
		{
			case CSG_TAG_X: m = m*mat4::rotX(c, s); break;
			case CSG_TAG_Y: m = m*mat4::rotY(c, s); break;
			case CSG_TAG_Z: m = m*mat4::rotZ(c, s); break;
			default: CSG_ASSERT(0, "invalid tag");
		}
	}
	if(CSG_REVERSE_ORDER) coords->local = m * coords->local;
	else                  coords->local = coords->local * m;
	CSG_REVERSE_ORDER = 0;
	CSG_Update(coords);
	CSG_POPNAME_;
}

static void CSG_ExecROTATEAXIS(char *paramstr[])
{
	CSG_PUSHNAME("ExecROTATEAXIS");
	CSG_POPNAME_;
}

static void CSG_ExecTRANSLATE(char *paramstr[])
{
	CSG_PUSHNAME("ExecTRANSLATE");
	
	coords_t *coords = &CSG_COORDS_STACK[CSG_COORDS_DEPTH-1];
	double tmp[CSG_MAX_ARRAY_SIZE];
	int count = CSG_ParseScalarArray(paramstr[0], tmp);
	CSG_ASSERT(count == 3, "incorrect array size");
	
	mat4 m(1.0, 0.0, 0.0, tmp[0],
	       0.0, 1.0, 0.0, tmp[1],
	       0.0, 0.0, 1.0, tmp[2],
	       0.0, 0.0, 0.0, 1.0);
	
	if(CSG_REVERSE_ORDER) coords->local = m * coords->local;
	else                  coords->local = coords->local * m;
	CSG_REVERSE_ORDER = 0;
	CSG_Update(coords);
	CSG_POPNAME_;
}

static void CSG_ExecSCALE(char *paramstr[])
{
	CSG_PUSHNAME("ExecSCALE");
	
	coords_t *coords = &CSG_COORDS_STACK[CSG_COORDS_DEPTH-1];
	double tmp[CSG_MAX_ARRAY_SIZE];
	int count = CSG_ParseScalarArray(paramstr[0], tmp);
	CSG_ASSERT(count == 3, "incorrect array size");
	
	mat4 m(tmp[0], 0.0,    0.0,    0.0,
	       0.0,    tmp[1], 0.0,    0.0,
	       0.0,    0.0,    tmp[2], 0.0,
	       0.0,    0.0,    0.0,    1.0);
	
	if(CSG_REVERSE_ORDER) coords->local = m * coords->local;
	else                  coords->local = coords->local * m;
	CSG_REVERSE_ORDER = 0;
	CSG_Update(coords);
	CSG_POPNAME_;
}

static void CSG_ExecSHEAR(char *paramstr[])
{
	CSG_PUSHNAME("ExecSHEAR");
	CSG_POPNAME_;
}

static void CSG_ExecPREROTATE(char *paramstr[])
{
	CSG_REVERSE_ORDER = 1;
	CSG_ExecROTATE(paramstr);
}

static void CSG_ExecPREROTATEAXIS(char *paramstr[])
{
	CSG_REVERSE_ORDER = 1;
	CSG_ExecROTATEAXIS(paramstr);
}

static void CSG_ExecPRETRANSLATE(char *paramstr[])
{
	CSG_REVERSE_ORDER = 1;
	CSG_ExecTRANSLATE(paramstr);
}

static void CSG_ExecPRESCALE(char *paramstr[])
{
	CSG_REVERSE_ORDER = 1;
	CSG_ExecSCALE(paramstr);
}

static void CSG_ExecPRESHEAR(char *paramstr[])
{
	CSG_REVERSE_ORDER = 1;
	CSG_ExecSHEAR(paramstr);
}

static void CSG_ExecSHAPE(char *paramstr[])
{
	CSG_PUSHNAME("ExecSHAPE");
	
	shape_t *sh = CSG_FindShape(paramstr[0], 1);
	double tmp[CSG_MAX_ARRAY_SIZE];
	int count = CSG_ParseScalarArray(paramstr[1], tmp);
	sh->type = CSG_SHAPETYPE_POLYGON;
	sh->vertcount = count/2;
	CSG_ASSERT(sh->vertcount >= 3, "too few vertices");
	CSG_FreeMem(sh->vert);
	sh->vert = (vec2*)CSG_AllocMem(sizeof(vec2), sh->vertcount);
	memcpy(sh->vert, tmp, sizeof(vec2)*sh->vertcount);
	CSG_CURRENT_SHAPE = sh;
	
	double maxcomp = sh->vert[0].x;
	int i;
	
	for(i = 0; i < sh->vertcount; i++)
	{
		if(maxcomp < sh->vert[i].x) maxcomp = sh->vert[i].x;
		if(maxcomp < sh->vert[i].y) maxcomp = sh->vert[i].y;
	}
	if(maxcomp < 1.0) maxcomp = 1.0; //FIXME: use _min
	
	for(i = 0; i < sh->vertcount; i++)
	{
		sh->vert[i] /= maxcomp;
	}
	CSG_POPNAME_;
}

static void CSG_ExecSPIRALSTAIR1(char *paramstr[])
{
	CSG_PUSHNAME("ExecSPIRALSTAIR1");
	
	// THETA0, THETASTEP, INNERRADIUS, OUTERPOINTS
	// Useful for making spiral staircases
	
	shape_t *sh = CSG_FindShape(paramstr[0], 1);
	double tmp[CSG_MAX_ARRAY_SIZE];
	int count = CSG_ParseScalarArray(paramstr[1], tmp);
	CSG_ASSERT(count == 4, "incorrect array size");
	CSG_ASSERT(tmp[3] == floor(tmp[3]), "integer required");
	CSG_ASSERT(tmp[3] >= 2.0, "too few outer points");
	CSG_ASSERT(tmp[2] >= 0.0 && tmp[2] < 1.0, "invalid inner radius");
	CSG_ASSERT(tmp[1] > 0.0 && tmp[1] < 180.0, "invalid angle");
	sh->vertcount = (int)tmp[3]+1;
	if(tmp[2] > 0.0) sh->vertcount++;
	CSG_ASSERT(sh->vertcount >= 3, "too few vertices");
	sh->vert = (vec2*)CSG_AllocMem(sizeof(vec2), sh->vertcount);
	
	double angle = tmp[0]*PI/180.0;
	double delta = tmp[1]*PI/180.0;
	int n;
	
	for(n = 0; n < (int)tmp[3]; n++)
	{
		sh->vert[n].x =  cos(angle);
		sh->vert[n].y = -sin(angle);
		
		angle += delta/(tmp[3]-1.0);
	}
	sh->vert[n] = sh->vert[n-1]*tmp[2];
	
	if(tmp[2] > 0.0)
	{
		sh->vert[n+1] = sh->vert[0]*tmp[2];
	}
	CSG_CURRENT_SHAPE = sh;
	CSG_POPNAME_;
}

static void CSG_ExecREGULARPOLY1(char *paramstr[])
{
	CSG_PUSHNAME("ExecREGULARPOLY1");
	
	// THETA0, THETASTEP, NPOINTS
	
	shape_t *sh = CSG_FindShape(paramstr[0], 1);
	double tmp[CSG_MAX_ARRAY_SIZE];
	int count = CSG_ParseScalarArray(paramstr[1], tmp);
	CSG_ASSERT(count == 3, "incorrect array size");
	CSG_ASSERT(tmp[2] == floor(tmp[2]), "integer required");
	sh->vertcount = (int)tmp[2];
	CSG_ASSERT(sh->vertcount >= 3, "too few vertices");
	sh->vert = (vec2*)CSG_AllocMem(sizeof(vec2), sh->vertcount);
	
	double angle = tmp[0]*PI/180.0;
	double delta = tmp[1]*PI/180.0;
	
	for(int n = 0; n < sh->vertcount; n++)
	{
		sh->vert[n].x =  cos(angle);
		sh->vert[n].y = -sin(angle);
		
		angle += delta;
	}
	CSG_CURRENT_SHAPE = sh;
	CSG_POPNAME_;
}

static void CSG_ExecFREESHAPE(char *paramstr[])
{
	CSG_PUSHNAME("ExecFREESHAPE");
	
	shape_t *sh = CSG_FindShape(paramstr[0], 0);
	CSG_FreeShape(sh);
	CSG_POPNAME_;
}

static void CSG_ExecDEPTH(char *paramstr[])
{
	CSG_PUSHNAME("ExecDEPTH");
	
	// *************************************
	// Set current depth for HCSG operations
	// *************************************
	
	double val = CSG_ParseScalarValue(paramstr[0]);
	
	CSG_ASSERT(val == floor(val), "depth must be integer");
	CSG_CURRENT_DEPTH = (int)val;
	CSG_POPNAME_;
}

struct _hull_entry_t{ hull_t *hull; int depth; };
static _hull_entry_t _HULL_LIST[1000];
static int           _HULL_COUNT = 0;

int _CSG_AddHull(void)
{
	static int count = 0;
	if(count >= _HULL_COUNT) return 1; // finished
	CSG_BooleanOp(CSG_WORLD,
		_HULL_LIST[count].hull,
		_HULL_LIST[count].depth, CSG_ADD, 0);
	count++;
	return 0;
}

static void CSG_ExecCSGADD(char *paramstr[])
{
	CSG_PUSHNAME("ExecCSGADD");
	
	shape_t *shape = CSG_FindShape(paramstr[0], 0);
	hull_t  *hull  = CSG_Extrude(shape);
	
	CSG_Event(CSG_ADD, shape);
	
	// --RECENT CHANGE--
	// CSG_BooleanOp(CSG_WORLD, hull, CSG_CURRENT_DEPTH, CSG_ADD, 0);
	// CSG_Free(hull); // BooleanOp CSG_ADD clones hull
	// Instead of adding the hull to the world, we append it to a list
	// of hulls to add. This makes it easier to watch the b-rep
	// updating in realtime, as we can do the hull addition in the
	// main event loop.
	_HULL_LIST[_HULL_COUNT].hull = hull;
	_HULL_LIST[_HULL_COUNT].depth = CSG_CURRENT_DEPTH;
	_HULL_COUNT++;
	// --RECENT CHANGE--
	CSG_POPNAME_;
}

static void CSG_ExecCSGSUB(char *paramstr[])
{
	CSG_PUSHNAME("ExecCSGSUB");
	CSG_POPNAME_;
}

static void CSG_ExecLOOP(char *paramstr[])
{
	CSG_PUSHNAME("ExecLOOP");
	
	// ***************************************************
	// WARNING: Loop declarations MUST be at the end of a
	// line, otherwise the marker will be incorrectly set!
	// ***************************************************
	
	double counter = CSG_ParseScalarValue(paramstr[0]);
	
	CSG_ASSERT(CSG_RECURS_DEPTH < CSG_MAX_RECURS_DEPTH, "recursion overflow");
	CSG_ASSERT(counter == floor(counter), "counter must be integer");
	CSG_ASSERT((int)counter > 0, "counter must be positive");
	
	recurs_t *recurs = &CSG_RECURS_STACK[CSG_RECURS_DEPTH++];
	recurs->counter  = (int)counter;
	recurs->marker   = (int)ftell(CSG_CURRENT_FILE);
	CSG_POPNAME_;
}

static void CSG_ExecENDLOOP(char *paramstr[])
{
	CSG_PUSHNAME("ExecENDLOOP");
	
	// ***************************************************
	// WARNING: Endloop declarations MUST be at the end of
	// a line, otherwise the loop will not work properly!
	// ***************************************************
	
	CSG_ASSERT(CSG_RECURS_DEPTH > 0, "recursion underflow");
	
	recurs_t *recurs = &CSG_RECURS_STACK[CSG_RECURS_DEPTH-1];
	
	if(--recurs->counter > 0)
	{
		fseek(CSG_CURRENT_FILE, recurs->marker, SEEK_SET);
	}
	else
	{
		CSG_RECURS_DEPTH--;
	}
	CSG_POPNAME_;
}
