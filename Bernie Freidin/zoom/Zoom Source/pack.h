#ifndef PACK_H
#define PACK_H

struct pack_t
{
	double x, y, z;
	double box[4];
	pack_t *parent[3];
	pack_t *child[3];
	int depth;
	int error;
};

struct packinfo_t
{
	int    obj_created;    // object counter
	int    obj_in_view;    // object counter
	int    obj_total;      // object counter
	int    node_reject;    // object counter
	
	int    dynamic;        // enable dynamic circle gen.
	int    dynamic_shadow; // enable dynamic in shadow
	int    shadow_mode;    // drawing is shadow style
	int    show_vec_3;     // show all 3 vector components
	int    show_depth;     // show depth (lower priority)
	double max_render_z;   // maximum visible curvature
	double lod_constant;   // controls smoothness of circles
	double fade_exponent;  // controls brightness
};

void    PACK_Initialize(packinfo_t *packinfo);
void    PACK_SetView(double view_x, double view_y);
pack_t *PACK_Create(pack_t *a, pack_t *b, pack_t *c, int depth);
void    PACK_DrawCircle(pack_t *pack);
void    PACK_Draw(pack_t *pack, pack_t *up);
void    PACK_Draw(int shadow_mode, zoom_t *zoom);
int     PACK_Search(double x, double y, pack_t *pack);
pack_t *PACK_Search(double x, double y, int show);
void    PACK_ClearTSPMemory(pack_t *pack);
void    PACK_ClearTSPMemory(void);

#endif
