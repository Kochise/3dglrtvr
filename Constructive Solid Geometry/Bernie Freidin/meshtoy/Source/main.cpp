// **************************************************************************
// *** Right-Triangle Irregular Network (RTIN) Toy
// *** (c) Bernie Freidin, 2000
// *** Requires OPENGL32, GLU32, GLUT32
// **************************************************************************

// **************************************************************************
// *** Keys:
// ***   1,2,3,4,5: reset mesh
// ***   6,7,8,9:   set color mode
// ***   A:         set texture mode A
// ***   B:         set texture mode B
// ***   ESCAPE:    quit
// **************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gl/glut.h>

// **************************************************************************
static int VIEW_X = 512;
static int VIEW_Y = 512;
static int CONTINUOUS_SPLIT = 1;
static int COLOR_MODE = 2;
#define MAX_DEPTH 12
#define MAX_NODES 65000
#define ALPHA 1.0
#define DEBUGGING 0
// **************************************************************************

#define SQRT3_2 0.866025403784438646763723170752936

#if DEBUGGING
static void ASSERT(int cond)
{
	if(!cond)
	{
		FILE *err = fopen("err.log", "w");
		if(!err) exit(-2); // wtf?
		fprintf(err, "assertion failed!\n");
		fclose(err);
		exit(-1);
	}
}
#else
#define ASSERT(cond)
#endif

struct node_t
{
	// nodes form both a connected mesh and a binary tree
	double x[3], y[3];
	int type, flip, value, depth;
	node_t *child[2];
	node_t *parent;
	node_t *adjacent[3];
};

struct rules_t
{
	// rules define the splitting of node triangles
	int c_type[2];
	int c_flip[2];
	int c_data[12];
	int rot;
};

#define MARKED ((node_t*)-1)
static node_t *MESH   = NULL, *MP = NULL;
static node_t *HILITE = NULL;
static int HLT__K = 0, VALUE = 0;
static int MESH_TREES = 0;
static double COLOR_TABLE[(MAX_DEPTH+1)*3];

#define NUM_NODE_TYPES 5
static rules_t RULES[NUM_NODE_TYPES] = {
	{{0,0}, {0,0}, {1,2,0, 1,2,0, 2,0,1, 2,0,1}, 0}, // 4.8.8
	{{2,3}, {0,0}, {1,2,0, 1,2,0, 0,1,2, 0,1,2}, 0}, // 4.6.12
	{{1,1}, {1,0}, {0,2,1, 2,1,0, 2,0,1, 2,0,1}, 0}, // 3.12.12
	{{1,1}, {0,1}, {1,2,0, 1,2,0, 2,1,0, 1,0,2}, 1}, // 6.6.6
	{{4,4}, {0,0}, {0,1,2, 0,1,2, 0,1,2, 0,1,2}, 1}  // freeform
};

static node_t *FindAdjacentChild(node_t *n, node_t *m);
static void    NodeLink1(node_t *n, int which);
static void    NodeLink2(node_t *n, int which);
static void    NodeCycle(node_t *n, int data[6]);
static void    NodeSplit(node_t *n, int value);
static node_t *NodePick(double x, double y, int *k);
static void    NodeDraw(node_t *n, int wire, int k);
static void    MeshDraw(node_t *n);

static node_t *FindAdjacentChild(node_t *n, node_t *m)
{
	for(int j = 0; j < 2; j++) { if(!n->child[j]) continue;
	for(int i = 0; i < 3; i++) {
		if(n->child[j]->adjacent[i] == m) return n->child[j];
	}}
	return NULL;
}

static void NodeLink1(node_t *n, int which)
{
	node_t *n0 = n->adjacent[which];
	for(int i = 0; i < 3; i++) {
	if(n0->adjacent[i] == n->parent) {
	   n0->adjacent[i] = n;
	   return;
	}}
}

static void NodeLink2(node_t *n, int which)
{
	node_t *n0 = n->adjacent[which];
	for(int i = 0; i < 3; i++) {
	if(n0->adjacent[i] == MARKED) {
	   n0->adjacent[i] = n;
	   return;
	}}
}

static void NodeCycle(node_t *n, int data[6])
{
	node_t tmp;
	memcpy(&tmp, n, sizeof(node_t));
	
	for(int i = 0; i < 3; i++)
	{
		n->x[data[i]] = tmp.x[i];
		n->y[data[i]] = tmp.y[i];
		n->adjacent[data[i+3]] = tmp.adjacent[i];
	}
}

static void NodeSplit(node_t *n, node_t *m, int value)
{
	ASSERT(n->child[0] == NULL);
	ASSERT(n->child[1] == NULL);
	ASSERT(MP-MESH+2 <= MAX_NODES);
	
	// allocate child nodes
	node_t *c1 = MP++;
	node_t *c2 = MP++;
	
	// freely oriented?
	rules_t *rules = &RULES[n->type];
	if(rules->rot && m)
	{
		static int ROT1[] = {1,2,0,1,2,0};
		while(n->adjacent[0] != m) NodeCycle(n, ROT1);
	}
	// set vertices
	double midpt_x = (n->x[0] + n->x[1])/2.0;
	double midpt_y = (n->y[0] + n->y[1])/2.0;
	c1->x[0] = n->x[0], c1->y[0] = n->y[0];
	c1->x[1] = midpt_x, c1->y[1] = midpt_y;
	c1->x[2] = n->x[2], c1->y[2] = n->y[2];
	c2->x[0] = midpt_x, c2->y[0] = midpt_y;
	c2->x[1] = n->x[1], c2->y[1] = n->y[1];
	c2->x[2] = n->x[2], c2->y[2] = n->y[2];
	
	// set parent-child pointers
	c1->parent = n;
	c2->parent = n;
	c1->child[0] = c1->child[1] = NULL;
	c2->child[0] = c2->child[1] = NULL;
	n->child[0] = c1;
	n->child[1] = c2;
	
	// set adjacent pointers
	c1->adjacent[1] = c2;
	c1->adjacent[2] = n->adjacent[2];
	c2->adjacent[1] = n->adjacent[1];
	c2->adjacent[2] = c1;
	if(c1->adjacent[2]) NodeLink1(c1, 2);
	if(c2->adjacent[1]) NodeLink1(c2, 1);
	
	// set node values
	c1->type = rules->c_type[0];
	c2->type = rules->c_type[1];
	c1->flip = rules->c_flip[0]^n->flip;
	c2->flip = rules->c_flip[1]^n->flip;
	c1->value = value;
	c2->value = value;
	int depth = n->depth+1;
	if(depth > MAX_DEPTH) depth = MAX_DEPTH;
	c1->depth = depth;
	c2->depth = depth;
	
	// recursive split propagation
	if(n->adjacent[0])
	{
		node_t *a0, *a1 = n->adjacent[0];
		
		if(a1->child[0] || a1->child[1])
		{
			// mark children for later linking
			c1->adjacent[0] = MARKED;
			c2->adjacent[0] = MARKED;
		}
		else
		{
			// keep splitting until T-vertex is gone
			while(a1)
			{
				NodeSplit(a1, n, value);
				a0 = a1;
				a1 = FindAdjacentChild(a0, n);
			}
			// link children to marked children
			int sw = n->flip ^ a0->flip;
			c1->adjacent[0] = a0->child[sw^1];
			c2->adjacent[0] = a0->child[sw];
			// note - if either of these is false,
			// then our triangulation is corrupt
			if(c1->adjacent[0]) NodeLink2(c1, 0);
			if(c2->adjacent[0]) NodeLink2(c2, 0);
		}
	}
	else
	{
		// edge of mesh
		c1->adjacent[0] = NULL;
		c2->adjacent[0] = NULL;
	}
	// re-orient children
	NodeCycle(c1, rules->c_data + 0);
	NodeCycle(c2, rules->c_data + 6);
	
	// remove adjacent pointers
	n->adjacent[0] = NULL;
	n->adjacent[1] = NULL;
	n->adjacent[2] = NULL;
}

static node_t *NodePick(double x, double y, int *k)
{
	node_t *n = MESH;
	
	// first, determine initial triangle
	for(int i = 0; i <= MESH_TREES; i++, n++)
	{
		if(i == MESH_TREES) return NULL;
		if((n->x[0]-n->x[1])*(n->y[0]-y) <
		   (n->y[0]-n->y[1])*(n->x[0]-x) ) continue;
		if((n->x[1]-n->x[2])*(n->y[1]-y) <
		   (n->y[1]-n->y[2])*(n->x[1]-x) ) continue;
		if((n->x[2]-n->x[0])*(n->y[2]-y) <
		   (n->y[2]-n->y[0])*(n->x[2]-x) ) continue;
		break;
	}
	// traverse binary tree
	while(1)
	{
		if(!n || !n->child[0] || !n->child[1]) break;
		
		double ax = n->x[2] - (n->x[0] + n->x[1])/2.0;
		double ay = n->y[2] - (n->y[0] + n->y[1])/2.0;
		double bx = n->x[2] - x;
		double by = n->y[2] - y;
		n = n->child[(ax*by < ay*bx) ? n->flip : n->flip^1];
	}
	if(n && k)
	{
		if(!RULES[n->type].rot) *k = 2;
		else
		{
			double ax = n->x[2] - (n->x[0] + n->x[1])/2.0;
			double ay = n->y[2] - (n->y[0] + n->y[1])/2.0;
			double bx = n->x[2] - x;
			double by = n->y[2] - y;
			int f1 = (ax*by < ay*bx) ? n->flip^1 : n->flip;
			ax = n->x[0] - (n->x[1] + n->x[2])/2.0;
			ay = n->y[0] - (n->y[1] + n->y[2])/2.0;
			bx = n->x[0] - x;
			by = n->y[0] - y;
			int f2 = (ax*by < ay*bx) ? n->flip^1 : n->flip;
			ax = n->x[1] - (n->x[2] + n->x[0])/2.0;
			ay = n->y[1] - (n->y[2] + n->y[0])/2.0;
			bx = n->x[1] - x;
			by = n->y[1] - y;
			int f3 = (ax*by < ay*bx) ? n->flip^1 : n->flip;
			if(f1 && !f3) *k = 0; else
			if(f2 && !f1) *k = 1; else
			if(f3 && !f2) *k = 2;
		}
	}
	return n;
}

static void NodeDraw(node_t *n, int wire, int k)
{
	if(wire)
	{
		glDisable(GL_TEXTURE_2D);		
		
		glBegin(GL_LINE_LOOP);
		glVertex2d(n->x[0], n->y[0]);
		glVertex2d(n->x[1], n->y[1]);
		glVertex2d(n->x[2], n->y[2]);
		glEnd();
		
		if(k >= 0)
		{
			glBegin(GL_LINES);
			glVertex2d(n->x[k], n->y[k]);
			glVertex2d((n->x[(k+1)%3] + n->x[(k+2)%3])/2.0,
			           (n->y[(k+1)%3] + n->y[(k+2)%3])/2.0);
			glEnd();
		}
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		double *c;
		
		switch(COLOR_MODE)
		{
		case 0: // mono gray
			glColor4d(1.0, 1.0, 1.0, ALPHA);
			break;
		case 1: // turn-based gray
			if(n->value&1) glColor4d(0.5, 0.5, 0.5, ALPHA);
			else           glColor4d(1.0, 1.0, 1.0, ALPHA);
			break;
		case 2: // depth-based color
			c = COLOR_TABLE + n->depth*3;
			glColor4d(c[0], c[1], c[2], ALPHA);
			break;
		case 3: // type-based color
			switch(n->type)
			{
			case 0: glColor4d(1.0, 1.0, 0.0, ALPHA); break;
			case 1: glColor4d(0.0, 1.0, 0.0, ALPHA); break;
			case 2: glColor4d(1.0, 0.0, 0.0, ALPHA); break;
			case 3: glColor4d(0.3, 0.2, 1.0, ALPHA); break;
			case 4: glColor4d(0.5, 0.7, 1.0, ALPHA); break;
			}
		}
		glBegin(GL_TRIANGLES);
		glTexCoord2d(0.0, 0.0);     glVertex2d(n->x[0], n->y[0]);
		glTexCoord2d(1.0, 0.0);     glVertex2d(n->x[1], n->y[1]);
		glTexCoord2d(0.5, SQRT3_2); glVertex2d(n->x[2], n->y[2]);
		glEnd();
	}
}

static void MeshDraw(node_t *n)
{
	if(!n->child[0] || !n->child[1])
	{
		NodeDraw(n, 0, 0);
	}
	else
	{
		// recurse
		MeshDraw(n->child[0]);
		MeshDraw(n->child[1]);
	}
}

static void InitializeMesh(int type)
{
	MP = MESH;
	
	if(type == 0) // two [4.8.8] triangles
	{
		node_t *n1 = MP++; memset(n1, 0, sizeof(node_t));
		node_t *n2 = MP++; memset(n2, 0, sizeof(node_t));
		n1->type = n2->type = 0;
		n1->x[0] = n1->y[1] = 1.0;
		n2->x[1] = n2->y[0] = 1.0;
		n2->x[2] = n2->y[2] = 1.0;
		n1->adjacent[0] = n2;
		n2->adjacent[0] = n1;
		MESH_TREES = 2;
	}
	else if(type == 1) // four [4.6.12] triangles
	{
		node_t *n1 = MP++; memset(n1, 0, sizeof(node_t));
		node_t *n2 = MP++; memset(n2, 0, sizeof(node_t));
		node_t *n3 = MP++; memset(n3, 0, sizeof(node_t));
		node_t *n4 = MP++; memset(n4, 0, sizeof(node_t));
		n1->type = n2->type = n3->type = n4->type = 1;
		n1->x[0] = n2->x[1] = n2->x[2] = SQRT3_2;
		n3->x[0] = n4->x[1] = n4->x[2] = SQRT3_2;
		n1->y[1] = n2->y[0] = n2->y[2] = 0.5;
		n3->y[0] = n3->y[2] = n4->y[1] = 0.5;
		n3->y[1] = n4->y[0] = n4->y[2] = 1.0;
		n1->adjacent[0] = n2;
		n2->adjacent[0] = n1;
		n2->adjacent[2] = n3;
		n3->adjacent[2] = n2;
		n3->adjacent[0] = n4;
		n4->adjacent[0] = n3;
		MESH_TREES = 4;
	}
	else if(type == 2) // four [4.6.12] triangles, two [4.8.8] triangles
	{
		double a = 0.366025403784438646763723170752936;
		
		node_t *n1 = MP++; memset(n1, 0, sizeof(node_t));
		node_t *n2 = MP++; memset(n2, 0, sizeof(node_t));
		node_t *n3 = MP++; memset(n3, 0, sizeof(node_t));
		node_t *n4 = MP++; memset(n4, 0, sizeof(node_t));
		node_t *n5 = MP++; memset(n5, 0, sizeof(node_t));
		node_t *n6 = MP++; memset(n6, 0, sizeof(node_t));
		n1->type = n2->type = n3->type = n4->type = 1;
		n5->type = n6->type = 0;
		n3->x[0] = n4->x[1] = n5->x[1] = n6->x[0] = a;
		n1->y[1] = n4->y[0] = n5->y[2] = a;
		n1->x[0] = n5->x[0] = n6->x[1] = n2->x[1] = 1.0 - a;
		n2->y[0] = n3->y[1] = n6->y[2] = 1.0 - a;
		n2->x[0] = n2->x[2] = n3->x[1] = 1.0;
		n3->x[2] = n6->x[2] = 1.0;
		n3->y[0] = n3->y[2] = n4->y[1] = 1.0;
		n4->y[2] = n5->y[1] = n6->y[0] = 1.0;
		n1->adjacent[0] = n5;
		n2->adjacent[0] = n6;
		n3->adjacent[0] = n6;
		n4->adjacent[0] = n5;
		n5->adjacent[0] = n6;
		n5->adjacent[1] = n4, n5->adjacent[2] = n1;
		n6->adjacent[0] = n5;
		n6->adjacent[1] = n2, n6->adjacent[2] = n3;
		MESH_TREES = 6;
	}
	else if(type == 3) // four [4.8.8] triangles, two [4.6.12] triangles
	{
		double a = 0.366025403784438646763723170752936;
		
		node_t *n1 = MP++; memset(n1, 0, sizeof(node_t));
		node_t *n2 = MP++; memset(n2, 0, sizeof(node_t));
		node_t *n3 = MP++; memset(n3, 0, sizeof(node_t));
		node_t *n4 = MP++; memset(n4, 0, sizeof(node_t));
		node_t *n5 = MP++; memset(n5, 0, sizeof(node_t));
		node_t *n6 = MP++; memset(n6, 0, sizeof(node_t));
		n1->type = n2->type = n3->type = n4->type = 0;
		n5->type = n6->type = 1;
		n3->x[0] = n4->x[1] = n5->x[1] = n6->x[0] = a;
		n1->y[1] = n4->y[0] = n5->y[2] = 1.0 - a;
		n1->x[0] = n5->x[0] = n6->x[1] = n2->x[1] = 1.0 - a;
		n2->y[0] = n3->y[1] = n6->y[2] = a;
		n2->x[0] = n2->x[2] = n3->x[1] = 1.0;
		n3->x[2] = n6->x[2] = 1.0;
		n3->y[0] = n3->y[2] = n4->y[1] = 1.0;
		n4->y[2] = n5->y[1] = n6->y[0] = 1.0;
		n1->adjacent[0] = n5;
		n2->adjacent[0] = n6;
		n3->adjacent[0] = n6;
		n4->adjacent[0] = n5;
		n5->adjacent[0] = n6;
		n5->adjacent[1] = n4, n5->adjacent[2] = n1;
		n6->adjacent[0] = n5;
		n6->adjacent[1] = n2, n6->adjacent[2] = n3;
		MESH_TREES = 6;
	}
	else if(type == 4) // two freeform triangles
	{
		node_t *n1 = MP++; memset(n1, 0, sizeof(node_t));
		node_t *n2 = MP++; memset(n2, 0, sizeof(node_t));
		n1->type = n2->type = 4;
		n1->x[0] = n1->y[1] = 1.0;
		n2->x[1] = n2->y[0] = 1.0;
		n2->x[2] = n2->y[2] = 1.0;
		n1->adjacent[0] = n2;
		n2->adjacent[0] = n1;
		MESH_TREES = 2;
	}
	else MESH_TREES = 0;
	VALUE = 0;
	HILITE = NULL;
}

static void InitializeColorTable(void)
{
	for(int depth = 0; depth <= MAX_DEPTH; depth++)
	{
		double *c = COLOR_TABLE + depth*3;
		double v = (double)depth / (double)MAX_DEPTH;
		c[0] = fabs(v - 0.5)*2.0;
		c[1] = 1.0 - pow(v, 1.3);
		c[2] = pow(v, 0.7);
	}
}

static void InitializeTexture(int size, int style)
{
	long *tex = (long*)malloc(sizeof(long)*size*size);
	if(!tex) exit(-1); // out of memory
	
	for(int y = 0; y < size; y++)
	{
		for(int x = 0; x < size; x++)
		{
			double u = (double)x / (double)(size-1);
			double v = (double)y / (double)(size-1);
			double t1 = v;
			double t2 = (0.0+u)*SQRT3_2 - v*0.5;
			double t3 = (1.0-u)*SQRT3_2 - v*0.5;
			double t  = t1*t2*t3*16.0;
			if(t < 0.0) t = 0.0;
			if(t > 1.0) t = 1.0;
			
			switch(style)
			{
			case 0: t = 0.0 + pow(t, 0.25); break;
			case 1: t = 1.0 - pow(t, 0.41); break;
			}
			if(t < 0.25) t = 0.25;
			tex[x + y*size] = 0x01010101 * (long)(t*255.0);
		}
	}
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8,
	                  size, size, GL_RGBA,
	                  GL_UNSIGNED_BYTE, tex);
	free(tex);
}

static void GLUT__Display(void)
{
	if(ALPHA == 1.0)
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
	for(int i = 0; i < MESH_TREES; i++)
	{
		MeshDraw(MESH + i);
	}
	if(HILITE)
	{
		glColor4d(1.0, 0.0, 0.0, 1.0);
		NodeDraw(HILITE, 1, HLT__K);
	}
	glFlush();
    glutSwapBuffers();
}

static void GLUT__Reshape(int width, int height)
{
	if(width  > height) width  = height;
	if(height < width ) height = width;
	VIEW_X = width;
	VIEW_Y = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

static void GLUT__Keyboard(unsigned char key, int x, int y)
{
	if(key == 27 ) exit(0); // ESC key quits program
	if(key == '1') InitializeMesh(0);
	if(key == '2') InitializeMesh(1);
	if(key == '3') InitializeMesh(2);
	if(key == '4') InitializeMesh(3);
	if(key == '5') InitializeMesh(4);
	if(key == '6') COLOR_MODE = 0;
	if(key == '7') COLOR_MODE = 1;
	if(key == '8') COLOR_MODE = 2;
	if(key == '9') COLOR_MODE = 3;
	if(key == 'a') InitializeTexture(256, 0);
	if(key == 'b') InitializeTexture(256, 1);
}

static void GLUT__Special(int key, int x, int y)
{
}

static void GLUT__Mouse(int button, int state, int x, int y)
{
	y = VIEW_Y - y;
	double u = (double)x / (double)VIEW_X;
	double v = (double)y / (double)VIEW_Y;
	
	if(state == GLUT_DOWN)
	{
		int k;
		node_t *n = NodePick(u, v, &k);
		if(n && n->depth < MAX_DEPTH)
		{
			static int ROT1[] = {1,2,0,1,2,0};
			static int ROT2[] = {2,0,1,2,0,1};
			if(k == 0) NodeCycle(n, ROT2); else
			if(k == 1) NodeCycle(n, ROT1);
			NodeSplit(n, NULL, VALUE++);
		}
		HILITE = n;
	}
}

static void GLUT__Motion(int x, int y)
{
	y = VIEW_Y - y;
	double u = (double)x / (double)VIEW_X;
	double v = (double)y / (double)VIEW_Y;
	
	if(CONTINUOUS_SPLIT)
	{
		int k;
		node_t *n = NodePick(u, v, &k);
		if(n && n->depth < MAX_DEPTH)
		{
			static int ROT1[] = {1,2,0,1,2,0};
			static int ROT2[] = {2,0,1,2,0,1};
			if(k == 0) NodeCycle(n, ROT2); else
			if(k == 1) NodeCycle(n, ROT1);
			NodeSplit(n, NULL, VALUE++);
		}
		HILITE = n;
	}
}

static void GLUT__PassiveMotion(int x, int y)
{
	y = VIEW_Y - y;
	double u = (double)x / (double)VIEW_X;
	double v = (double)y / (double)VIEW_Y;
	HILITE = NodePick(u, v, &HLT__K);
}

static void GLUT__Idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	fprintf(stdout, "MeshToy: RTIN Subdivision Demo\n");
	fprintf(stdout, "(c) Bernie Freidin, 2000\n\n");
	fprintf(stdout, "Keys:\n");
	fprintf(stdout, "  Left-click to subdivide triangles\n");
	fprintf(stdout, "  1-5 to reset mesh (1 is standard RTIN)\n");
	fprintf(stdout, "  6-9 to set color mode\n");
	fprintf(stdout, "  A/B to set invert mode\n");
	fprintf(stdout, "  ESCAPE to quit\n\n");
	
	ASSERT(1); // force compiler to include ASSERT function
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(VIEW_X, VIEW_Y);
	int win_id = glutCreateWindow(argv[0]);
	glutDisplayFunc(GLUT__Display);
	glutReshapeFunc(GLUT__Reshape);
	glutKeyboardFunc(GLUT__Keyboard);
	glutSpecialFunc(GLUT__Special);
	glutMouseFunc(GLUT__Mouse);
	glutMotionFunc(GLUT__Motion);
	glutPassiveMotionFunc(GLUT__PassiveMotion);
	glutIdleFunc(GLUT__Idle);
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(2.0f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glEnable(GL_TEXTURE_2D);
	
	if(ALPHA)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}
	MESH = (node_t*)malloc(sizeof(node_t)*MAX_NODES);
	if(!MESH) exit(-1); // out of memory
	
	InitializeMesh(0);
	InitializeColorTable();
	InitializeTexture(256, 0);
	
	glutMainLoop();
	return 0;
}
