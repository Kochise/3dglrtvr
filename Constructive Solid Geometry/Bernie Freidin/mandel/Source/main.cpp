// -------------------------------
// MANDEL: Mandelbrot Set Explorer
// (c) Bernie Freidin, 1999
// -------------------------------

/*
Keys:
	C: clear screen
	D: redraw
	J: Julia mode
	M: Mandelbrot mode
	H: hybrid mode
	B: apply iteration bias
	BACKSPACE: revert to previous zoom state
	ESCAPE: quit
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <gl/glut.h>

#define VERSION (0xDEADC0DE + 0x000)

// Performance Measurements
// 1:18:42 - blend
// 1:14:34 - no blend
// 1:04:19 - blend/tight
// 0:59:82 - no blend/tight
// 0:22:64 - blend/release
// 0:20:16 - blend/tight/release
// 19.12 secs - blend/release/no redraw
// 0:20:41 - blend/release/esc@1e-15
// 0:19:63 - blend/release/esc@1e-12
// ~40 sec - blend/release/esc@1e-15 (PII-350)

#define _min(a,b) ((a)<(b)?(a):(b))
#define _max(a,b) ((a)>(b)?(a):(b))

#define ICON_BAR       0
#define STAT_BAR       100
#define MIN_WINDOW_X   128
#define MIN_WINDOW_Y   96
#define MAX_WINDOW_X   1280
#define MAX_WINDOW_Y   1024
#define MAX_SLICE_RES  16
#define MAX_ITERATIONS 32768
#define MAX_ZOOMS      250
#define MAX_C_ENTRIES  16 // color entries per preset
#define MAX_C_PRESETS  21 // max total preset count
#define C_PRESET_PATH "ColorPresets.txt"

typedef long pix_t; // 32bpp

#define MAX_JULIA_DEFAULT_PRESETS 10 // including SCvC

static double JULIA_DEFAULT_COORDS[MAX_JULIA_DEFAULT_PRESETS*2] = {
	-0.750, 0.00,    // parabolic fixed, period 2
	-0.125, 0.64925, // parabolic fixed, period 3
	-1.000, 0.00,    // superattractive, period 2
	-0.130, 0.76,    // superattractive, period 3
	+0.280, 0.53,    // superattractive, period 4
	-0.830, 0.16,    // swirly, period 2
	-.3905407802, -.5867879073, // Siegel Disc
	-0.1011, 0.9563, // Misiurewicz Point
	+0.000, 1.00,    // dendrite
	-9.0, -9.0
};

static char JULIA_DEFAULT_NAMES[MAX_JULIA_DEFAULT_PRESETS][64] = {
	"parabolic fixed point (period 2)",
	"parabolic fixed point (period 3)",
	"superattractive (period 2)",
	"superattractive (period 3)",
	"superattractive (period 4)",
	"swirly (period 2)",
	"Siegel Disc",
	"Misiurewicz Point",
	"dendrite",
	"Set Coordinates via Console..."
};

struct zoom_t
{
	double x, y, z;
	int iteration_bias;
};

struct box_t
{
	int x1, y1, x2, y2;
};

struct cdat_t
{
	char   name[48];
	int    count;
	double exponent;
	double colors[4*MAX_C_ENTRIES];
	pix_t  table[MAX_ITERATIONS+1];
};

static double STAT_BAR_COLOR[]   = {0.2, 0.2, 0.4};
static double STAT_TEXT_COLOR[]  = {0.5, 1.0, 0.5};
static double STAT_TEXT_COLOR2[] = {0.0, 1.0, 1.0};
static double BOX_NO_HLT_COLOR[] = {0.5, 0.5, 0.0};
static double BOX_HILITE_COLOR[] = {1.0, 1.0, 0.0};
static double SLICE_HLT_COLOR[]  = {1.0, 1.0, 1.0};

static int    WINDOW_X;   // total width  of window, in pixels
static int    WINDOW_Y;   // total height of window, in pixels
static int    VIEW_X;     // width  of viewing region, in pixels
static int    VIEW_Y;     // height of viewing region, in pixels
static box_t  BOX[2];     // zoom box (0=current, 1=old)
static int    BOX_STATE;  // internal tracking state
static int    BOX_HILITE; // internal hilite state
static int    BOX_UPDATE; // zoom box needs update?
static pix_t *SLICE_BUF;       // working buffer for image
static int    SLICE_POS;       // current vertical slice position
static int    SLICE_RES;       // current block size
static int    SLICE_X;         // VIEW_X/SLICE_RES
static int    SLICE_Y;         // VIEW_Y/SLICE_RES
static int    SLICE_UPDATE;    // slice needs update?
static int    SLICE_TMP_COUNT; // internal counter
static int    SLICE_HLT_POS;   // hilite position
static int    BLOCK_SIZE;      // block resolution (16,8,4,2,or 1)
static int    SSAMP_SIZE;      // supersampling grid size
static double FOCUS;           // supersampling focus
static int    ITERATION_COUNT[4];    // iteration counters
static int    ITERATIONS_PER_PIXEL;  // max iterations per pixel
static int    ITERATIONS_PER_UPDATE; // max iterations per update
static int    RENDERING_ENABLED;     // i.e. !"paused"
static int    REALTIME_STAT_UPDATE;  // update status after slice?
static int   *LOG2TABLE;             // little lookup thingy
static zoom_t ZOOM_LIST[MAX_ZOOMS];  // zoom states
static int    ZOOM_COUNT;            // number of zooms
static int    APP_RESHAPE_EVENT;     // internal flag
static double MOUSE_X;               // mouse x-coord in worldspace
static double MOUSE_Y;               // mouse y-coord in worldspace
static int    MOUSE_UPDATE;          // mouse moved - update coords
static int    MOUSE_ITERATIONS;      // iterations under mouse point
static int    JULIA_MODE;            // render Julia set?
static double JULIA_X;               // Julia x coefficient
static double JULIA_Y;               // Julia y coefficient
static int    JULIA_ZOOM_INDEX;      // index into zoom list where
                                     // Julia-mode was invoked..
static int    M_ID[20];
static char   C_PRESET_NAMES[MAX_C_PRESETS][48];
static int    C_PRESET_COUNT;
static int    HYSTERIA_MODE;    // (c) James Beattie, 1999
static double HYSTERIA_FACTOR;  // (c) James Beattie, 1999

static int    ESCAPE1_COLOR_DEF; // method used to color pixels
static int    ESCAPE2_COLOR_DEF; // method used to color pixels
static cdat_t ESCAPE1_COLOR_DAT; // color gradient lookups
static cdat_t ESCAPE2_COLOR_DAT; // color gradient lookups
static double ESCAPE2_THRESHOLD; // threshold for alternate escape*
static int    ESCAPE2_EQUATION;  // equation for alternate escape*
static int    ESCAPE2_MIN_ITER;  // minimum iterations for alt-esc
static int    ESCAPE2_MAX_ITER;  // maximum iterations for alt-esc
static double ESCAPE1_BLEND_FACTOR; // advanced stuff
static double ESCAPE1_XBAND_FACTOR; // advanced stuff
static double ESCAPE2_SHADE_FACTOR; // advanced stuff

// NOTE: Alternate escapes are activated when some function of
// x, y, dx, and/or dy in the Iteration() function comes within
// some threshold of zero. When this happens, the function exits,
// coloring the pixel based on the alternate color gradient.
// Visually, this looks like zillions of objects (spheres, wires,
// rings, blobs, etc.) are clustering around the boundary and
// interior of the Mandelbrot set.

static void SaveSliceBuffer(FILE *fp)
{
	// *****************************************************
	// Saves the slice buffer to a file. Note that the
	// pixels are stored in a multiresolution format so that
	// images that have not fully rendered do not take up a
	// huge amount of memory.
	// *****************************************************
	
	int y, ymark = (SLICE_HLT_POS + SLICE_RES) & ~(SLICE_RES*2-1);
	
	for(y = 0; y < ymark; y += SLICE_RES)
	{
		pix_t *buf = SLICE_BUF + (SLICE_RES+(y+1)*2)*(VIEW_X/2) - SLICE_X;
		
		fwrite(buf, sizeof(pix_t), SLICE_X, fp);
	}
	if(SLICE_RES == MAX_SLICE_RES) return;
	
	for(; y < VIEW_Y; y += SLICE_RES*2)
	{
		pix_t *buf = SLICE_BUF + (SLICE_RES+y+1)*VIEW_X - SLICE_X/2;
		
		fwrite(buf, sizeof(pix_t), SLICE_X/2, fp);
	}
}

static void LoadSliceBuffer(FILE *fp)
{
	// *****************************************************
	// Loads in the slice buffer from a file. Note that the
	// pixels are stored in a multiresolution format so that
	// images that have not fully rendered do not take up a
	// huge amount of memory.
	// *****************************************************
	
	int y, ymark = (SLICE_HLT_POS + SLICE_RES) & ~(SLICE_RES*2-1);
	
	memset(SLICE_BUF, 0, sizeof(pix_t)*VIEW_X*VIEW_Y);
	
	for(y = 0; y < ymark; y += SLICE_RES)
	{
		pix_t *buf = SLICE_BUF + (SLICE_RES+(y+1)*2)*(VIEW_X/2) - SLICE_X;
		
		fread(buf, sizeof(pix_t), SLICE_X, fp);
	}
	if(SLICE_RES == MAX_SLICE_RES) return;
	
	for(; y < VIEW_Y; y += SLICE_RES*2)
	{
		pix_t *buf = SLICE_BUF + (SLICE_RES+y+1)*VIEW_X - SLICE_X/2;
		
		fread(buf, sizeof(pix_t), SLICE_X/2, fp);
	}
}

// *******************
// Function prototypes
// *******************

static void  Initialize(void);
static void  Clear(void);
static void  MenuCallback_0(int id);
static void  MenuCallback_1(int id);
static void  MenuCallback_2(int id);
static void  MenuCallback_3(int id);
static void  MenuCallback_4(int id);
static void  MenuCallback_5(int id);
static void  MenuCallback_6(int id);
static void  MenuCallback_7(int id);
static void  MenuCallback_8(int id);
static void  MenuCallback_9(int id);
static void  MenuCallback_10(int id);
static void  MenuCallback_11(int id);
static void  MenuCallback_12(int id);
static void  MenuCallback_13(int id);
static void  MenuCallback_14(int id); // Julia presets
static void  UpdateMenu_1(void);
static void  UpdateMenu_2(void);
static void  UpdateMenu_3(void);
static void  UpdateMenu_4(void);
static void  UpdateMenu_5(void);
static void  UpdateMenu_6(void);
static void  UpdateMenu_7(void);
static void  UpdateMenu_8(void);
static void  UpdateMenu_9(void);
static void  UpdateMenu_10(void);
static void  UpdateMenu_11(void);
static void  UpdateMenu_13(void);
static int   QFileExists(char *path);
static int   QDisableRendering(void);
static int   QPointInBox(box_t *box, int x, int y);
static void  SaveImageAsTGA(char *path);
static void  SaveImageAsBMP(char *path);
static void  LoadColorPresetList(char *path);
static void  LoadColorPreset(cdat_t *cdat, char *path, char *name);
static void  RecomputeColorGradient(cdat_t *cdat);
static void  SetPixelCoords(int lft, int bot, int rgt, int top, int clear);
static void  Pix2WorldCoords(double &x, double &y);
static void  DrawText(int x, int y, char *str);
static void  IncrementCount(int dcount);
static void  AppendCountStr(char *str);
static void  DrawStatus(int field);
static pix_t Blend(pix_t *table, int index, double blend);
static pix_t Iterate(double cx, double cy, int &iter);
static pix_t RenderPixel(int i, int j);
static void  RedrawXLine(int x, int y, int width);
static void  RedrawYLine(int x, int y, int height);
static void  Redraw(void);
static void  EraseBox(box_t *box);
static void  DrawBox(box_t *box);
static int   DrawSlice(void);
static void  DrawSliceHilite(void);
static void  Zoom(box_t *box);

static void Initialize(void)
{
	int i, j;
	
	SLICE_BUF = (pix_t*)malloc(sizeof(pix_t)*MAX_WINDOW_X*MAX_WINDOW_Y);
	LOG2TABLE = (int*)calloc(sizeof(int), MAX_SLICE_RES+1);
	
	for(i = 1, j = 0; i <= MAX_SLICE_RES; i *= 2, j++)
	{
		LOG2TABLE[i] = j;
	}
	BLOCK_SIZE = 1;
	SSAMP_SIZE = 4;
	FOCUS      = 1.1;
	
	ITERATIONS_PER_PIXEL  = 256; // must NOT exceed MAX_ITERATIONS
	ITERATIONS_PER_UPDATE = 20000;
	RENDERING_ENABLED     = 1;
	REALTIME_STAT_UPDATE  = 1;
	ZOOM_LIST[0].x        = 0.5; // default zoom
	ZOOM_LIST[0].y        = 0.0;
	ZOOM_LIST[0].z        = 0.75;
	ZOOM_LIST[0].iteration_bias = 0;
	ZOOM_COUNT            = 1;
	APP_RESHAPE_EVENT     = 0;
	ESCAPE1_COLOR_DEF     = 3;
	ESCAPE2_COLOR_DEF     = 0;
	ESCAPE2_THRESHOLD     = 0.03125;
	ESCAPE2_EQUATION      = 0;
	ESCAPE1_COLOR_DAT.exponent = 3.0;
	ESCAPE2_COLOR_DAT.exponent = 17.0;
	ESCAPE2_MIN_ITER      = 1; // never set below 1
	ESCAPE2_MAX_ITER      = MAX_ITERATIONS;
	ESCAPE1_BLEND_FACTOR  = 2.75;
	ESCAPE1_XBAND_FACTOR  = 10.0;
	ESCAPE2_SHADE_FACTOR  = 100.0;
	ITERATION_COUNT[0]    = 0;
	ITERATION_COUNT[1]    = 0;
	ITERATION_COUNT[2]    = 0;
	ITERATION_COUNT[3]    = MAX_ITERATIONS;
	JULIA_MODE            = 0;
	JULIA_X               = -1.0;
	JULIA_Y               = +0.0;
	HYSTERIA_MODE         = 0;
	HYSTERIA_FACTOR       = 64.0;
	
	M_ID[14] = glutCreateMenu(MenuCallback_14); // Julia presets
	
	for(i = 0; i < MAX_JULIA_DEFAULT_PRESETS; i++)
	{
		glutAddMenuEntry(JULIA_DEFAULT_NAMES[i], i+1);
	}
	M_ID[1] = glutCreateMenu(MenuCallback_1); // mode
	glutAddMenuEntry("Mandelbrot Set", 1);
	glutAddSubMenu("Julia Set", M_ID[14]);
	glutAddMenuEntry("Julia/Mandelbrot Hybrid Fractal", 3);
	
	M_ID[2] = glutCreateMenu(MenuCallback_2); // resolution
	glutAddMenuEntry("16x16 blocks", 1);
	glutAddMenuEntry("8x8 blocks", 2);
	glutAddMenuEntry("4x4 blocks", 3);
	glutAddMenuEntry("2x2 blocks", 4);
	glutAddMenuEntry("1x1 blocks", 5);
	
	M_ID[3] = glutCreateMenu(MenuCallback_3); // supersampling
	glutAddMenuEntry("1x1 (off)", 1);
	glutAddMenuEntry("2x2", 2);
	glutAddMenuEntry("4x4", 3);
	glutAddMenuEntry("8x8", 4);
	glutAddMenuEntry("12x12", 4);
	glutAddMenuEntry("Set via console...(1-16)", 99);
	
	M_ID[4] = glutCreateMenu(MenuCallback_4); // focus
	glutAddMenuEntry("0.2 (grainy)", 1);
	glutAddMenuEntry("0.5", 2);
	glutAddMenuEntry("1.0 (perfect)", 3);
	glutAddMenuEntry("1.1 (nice)", 4);
	glutAddMenuEntry("1.5", 5);
	glutAddMenuEntry("2.0", 6);
	glutAddMenuEntry("3.0 (fuzzy)", 7);
	glutAddMenuEntry("Set via console...(0-10)", 99);
	
	M_ID[5] = glutCreateMenu(MenuCallback_5); // iterations
	glutAddMenuEntry("32", 1);
	glutAddMenuEntry("64", 2);
	glutAddMenuEntry("128", 3);
	glutAddMenuEntry("256", 4);
	glutAddMenuEntry("512", 5);
	glutAddMenuEntry("1024", 6);
	glutAddMenuEntry("2048", 7);
	glutAddMenuEntry("Set via console...(4-32768)", 99);
	
	M_ID[6] = glutCreateMenu(MenuCallback_6); // layer-1 color method
	glutAddMenuEntry("None", 1);
	glutAddMenuEntry("Constant Color", 2);
	glutAddMenuEntry("Non-Interpolated Gradient", 3);
	glutAddMenuEntry("Interpolated Gradient", 4); // default
	glutAddMenuEntry("Banded", 5);
	
	M_ID[7] = glutCreateMenu(MenuCallback_7); // layer-1 color gradient
	
	for(i = 0; i < MAX_C_PRESETS; i++)
	{
		glutAddMenuEntry("...", i+1);
	}
	M_ID[8] = glutCreateMenu(MenuCallback_8); // layer-2 color method
	glutAddMenuEntry("None", 1);
	glutAddMenuEntry("Constant Color", 2);
	glutAddMenuEntry("Smooth Shaded", 3);
	glutAddMenuEntry("Reverse Ordered", 4);
	glutAddMenuEntry("Reverse Color Min", 5);
	
	M_ID[9] = glutCreateMenu(MenuCallback_9); // layer-2 color gradient
	
	for(i = 0; i < MAX_C_PRESETS; i++)
	{
		glutAddMenuEntry("...", i+1);
	}
	M_ID[10] = glutCreateMenu(MenuCallback_10); // layer-2 escape equation
	glutAddMenuEntry("x^2 + y^2 \"bubbles\"", 1);
	glutAddMenuEntry("x^2 - y^2 \"spikes1\"", 2);
	glutAddMenuEntry("dx^2 - dy^2 \"spikes2\"", 3);
	glutAddMenuEntry("x dx + y dy \"rings\"", 4);
	glutAddMenuEntry("x dx - y dy \"boomerangs\"", 5);
	glutAddMenuEntry("x dy - y dx \"wires1\"", 6);
	glutAddMenuEntry("x y - dx dy \"wires2\"", 7);
	glutAddMenuEntry("(x^4 + y^4)/(x^2 - y^2) \"clovers\"", 8);
	glutAddMenuEntry("x cos(dy) + y sin(dx) \"spikes3\"", 9);
	glutAddMenuEntry("|x|dx + |y|dy \"twists\"", 10);
	
	M_ID[11] = glutCreateMenu(MenuCallback_11); // advanced options
	glutAddMenuEntry("Set Layer-1 Color Exponent... 3.000", 1);
	glutAddMenuEntry("Set Layer-1 Color Interpolation Factor... 2.750", 2);
	glutAddMenuEntry("Set Layer-1 Color Banding Scale Factor... 10.000", 3);
	glutAddMenuEntry("Set Layer-1 Iteration Bias... <automatic>", 4);
	glutAddMenuEntry("Set Layer-2 Color Exponent... 17.000", 5);
	glutAddMenuEntry("Set Layer-2 Color Shade Scale Factor... 100.000", 6);
	glutAddMenuEntry("Set Layer-2 Escape Threshold... 0.0312500", 7);
	glutAddMenuEntry("Set Layer-2 Min Iterations... 1", 8);
	glutAddMenuEntry("Set Layer-2 Max Iterations... 32768", 9);
	glutAddMenuEntry("Set Hysteria Factor... 64.00000", 10);
	glutAddMenuEntry("Reset All Advanced Options", 11);
	
	M_ID[12] = glutCreateMenu(MenuCallback_12); // realtime stat
	glutAddMenuEntry("On", 1);
	glutAddMenuEntry("Off", 2);
	
	M_ID[13] = glutCreateMenu(MenuCallback_13); // Hysteria by James Beattie
	glutAddMenuEntry("On", 1);
	glutAddMenuEntry("Off", 2);
	
	M_ID[0] = glutCreateMenu(MenuCallback_0); // main
	glutAddSubMenu("Mode: Mandelbrot Set", M_ID[1]);
	glutAddSubMenu("Resolution: 1x1", M_ID[2]);
	glutAddSubMenu("Supersampling: 4x4", M_ID[3]);
	glutAddSubMenu("Focus: 1.1", M_ID[4]);
	glutAddSubMenu("Iterations: 256", M_ID[5]);
	glutAddSubMenu("Layer-1 Color Method: Interpolated", M_ID[6]);
	glutAddSubMenu("...", M_ID[7]);
	glutAddSubMenu("Layer-2 Color Method: None", M_ID[8]);
	glutAddSubMenu("...", M_ID[9]);
	glutAddSubMenu("Layer-2 Escape Equation: x^2 + y^2", M_ID[10]);
	glutAddSubMenu("Advanced Options [set via console]", M_ID[11]);
	glutAddSubMenu("Realtime Status Update: On", M_ID[12]);
	glutAddSubMenu("Hysteria by James Beattie: Off", M_ID[13]);
	glutAddMenuEntry("Save Image as *.TGA File...", 1);
	glutAddMenuEntry("Save Image as *.BMP File...", 2);
	glutAddMenuEntry("Save Rendering State...", 3);
	glutAddMenuEntry("Load Rendering State...", 4);
	glutAddMenuEntry("Set Min/Max Coords via Console...", 5);
	glutAddMenuEntry("Reload Color Presets", 6);
	glutAddMenuEntry("Exit", 7);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	// **************************************************
	// Color presets must be loaded after menu is created
	// **************************************************
	
	LoadColorPresetList(C_PRESET_PATH);
	LoadColorPreset(&ESCAPE1_COLOR_DAT, C_PRESET_PATH, C_PRESET_NAMES[0]);
	LoadColorPreset(&ESCAPE2_COLOR_DAT, C_PRESET_PATH, C_PRESET_NAMES[3]);
	
	char str[200];
	glutSetMenu(M_ID[0]);
	sprintf(str, "Layer-1 Color Gradient: %s", C_PRESET_NAMES[0]);
	glutChangeToSubMenu(7, str, M_ID[7]);
	sprintf(str, "Layer-2 Color Gradient: %s", C_PRESET_NAMES[3]);
	glutChangeToSubMenu(9, str, M_ID[9]);
	
	Clear();
}

static void Clear(void)
{
	memset(BOX, 0, sizeof(box_t)*2);
	memset(SLICE_BUF, 0, sizeof(pix_t)*VIEW_X*VIEW_Y);
	
	BOX_STATE          = 0;
	BOX_HILITE         = 0;
	BOX_UPDATE         = 0;
	SLICE_POS          = 0;
	SLICE_RES          = MAX_SLICE_RES;
	SLICE_X            = VIEW_X/SLICE_RES;
	SLICE_Y            = VIEW_Y/SLICE_RES;
	SLICE_UPDATE       = 0;
	SLICE_TMP_COUNT    = 0;
	SLICE_HLT_POS      = 0;
	ITERATION_COUNT[0] = 0;
	ITERATION_COUNT[1] = 0;
	ITERATION_COUNT[2] = 0;
	ITERATION_COUNT[3] = MAX_ITERATIONS;
	
	// NOTE: SLICE_HLT_POS serves a dual purpose; to position the hilited
	// scanline above the last rendered slice, and to determine how to
	// unpack pixel data from SLICE_BUF when re-drawing the pixels under
	// the old zoom box.
	
	RENDERING_ENABLED = 1;
	
	DrawStatus(0);
}

static int QFileExists(char *path)
{
	// ***********************************
	// Tests whether a file already exists
	// ***********************************
	
	FILE *test = fopen(path, "rb");
	
	if(test)
	{
		fclose(test);
		return 1;
	}
	return 0;
}

static int QPointInBox(box_t *box, int x, int y)
{
	// ***************************************
	// Determines if mouse click is inside box
	// ***************************************
	
	if(x < _min(box->x1, box->x2)) return 0;
	if(y < _min(box->y1, box->y2)) return 0;
	if(x > _max(box->x1, box->x2)) return 0;
	if(y > _max(box->y1, box->y2)) return 0;
	return 1;
}

static int QDisableRendering(void)
{
	// *********************************************
	// Determine whether rendering should be stopped
	// *********************************************
	
	if(SLICE_POS*SLICE_RES < VIEW_Y) return 0;
	if(SLICE_RES == BLOCK_SIZE)      return 1;
	
	return 0;
}

static void SaveImageAsTGA(char *path)
{
	// ************************
	// Writes image to TGA file
	// ************************
	
	FILE *tga = fopen(path, "wb");
	short data[] = {0,2,0,0,0,0,-1,-1,24};
	
	if(!tga)
	{
		fprintf(stdout, "Can't open file \"%s\"\n", path);
		return;
	}
	// ****************
	// Write TGA header
	// ****************
	
	data[6] = VIEW_X;
	data[7] = VIEW_Y;
	fwrite(data, sizeof(short)*9, 1, tga);
	
	// **************************
	// Allocate buffer for pixels
	// **************************
	
	unsigned char *pix = (unsigned char*)malloc(VIEW_X*VIEW_Y*3);
	if(!pix) exit(-1);
	
	// ************************
	// Redraw view, grab pixels
	// ************************
	
	Redraw(); // gets rid of zoom box, but not slice hilite
	
	glReadPixels(ICON_BAR, STAT_BAR,
	             VIEW_X, VIEW_Y, GL_RGB, GL_UNSIGNED_BYTE, pix);
	
	// ************************
	// Convert BGR->RGB for TGA
	// ************************
	
	for(int i = 0; i < VIEW_X*VIEW_Y; i++)
	{
		pix_t ptmp = pix[i*3+2];
		pix[i*3+2] = pix[i*3];
		pix[i*3]   = ptmp;
	}
	// *************************************
	// Write pixels, close file, free buffer
	// *************************************
	
	fwrite(pix, 1, VIEW_X*VIEW_Y*3, tga);
	fclose(tga);
	free(pix);
}

static void SaveImageAsBMP(char *path)
{
	// ************************
	// Writes image to BMP file
	// ************************
	
	FILE *bmp = fopen(path, "wb");
	char tag[] = "BM";
	long data;
	
	if(!bmp)
	{
		fprintf(stdout, "Can't open file \"%s\"\n", path);
		return;
	}
	// ****************
	// Write BMP header
	// ****************
	
	fwrite(tag, sizeof(char), 2, bmp);
	data = 0x36 + VIEW_X*VIEW_Y*3;
	fwrite(&data, sizeof(long), 1, bmp);
	data = 0;
	fwrite(&data, sizeof(long), 1, bmp);
	data = 0x36;
	fwrite(&data, sizeof(long), 1, bmp);
	data = 0x28;
	fwrite(&data, sizeof(long), 1, bmp);
	data = VIEW_X;
	fwrite(&data, sizeof(long), 1, bmp);
	data = VIEW_Y;
	fwrite(&data, sizeof(long), 1, bmp);
	short sdata = 1;
	fwrite(&sdata, sizeof(short), 1, bmp);
	sdata = 24;
	fwrite(&sdata, sizeof(short), 1, bmp);
	data = 0;
	fwrite(&data, sizeof(long), 1, bmp);
	fwrite(&data, sizeof(long), 1, bmp);
	data = 0xB12;
	fwrite(&data, sizeof(long), 1, bmp);
	fwrite(&data, sizeof(long), 1, bmp);
	data = 0;
	fwrite(&data, sizeof(long), 1, bmp);
	fwrite(&data, sizeof(long), 1, bmp);
	
	// **************************
	// Allocate buffer for pixels
	// **************************
	
	unsigned char *pix = (unsigned char*)malloc(VIEW_X*VIEW_Y*3);
	if(!pix) exit(-1);
	
	// ************************
	// Redraw view, grab pixels
	// ************************
	
	Redraw(); // gets rid of zoom box, but not slice hilite
	
	glReadPixels(ICON_BAR, STAT_BAR,
	             VIEW_X, VIEW_Y, GL_RGB, GL_UNSIGNED_BYTE, pix);
	
	// ************************
	// Convert BGR->RGB for BMP
	// ************************
	
	for(int i = 0; i < VIEW_X*VIEW_Y; i++)
	{
		pix_t ptmp = pix[i*3+2];
		pix[i*3+2] = pix[i*3];
		pix[i*3]   = ptmp;
	}
	// *************************************
	// Write pixels, close file, free buffer
	// *************************************
	
	fwrite(pix, 1, VIEW_X*VIEW_Y*3, bmp);
	
	fclose(bmp);
	free(pix);
}

static void MenuCallback_0(int id)
{
	char path[200];
	
	if(id < 5)
	{
		fprintf(stdout, "Enter filename: ");
		fgets(path, 199, stdin);
		{
			if(path[0] == '\n')
			{
				fprintf(stdout, "Canceled.\n");
				return;
			}
			char *ret = strchr(path, '\n');
			if(ret) *ret = '\0'; // remove newline
		}
	}
	if(id == 1)
	{
		// *****************
		// Save image as TGA
		// *****************
		
		if(!strstr(path, ".tga") && !strstr(path, ".TGA"))
		{
			strcat(path, ".tga");
		}
		if(QFileExists(path))
		{
			char yes_no[200];
			fprintf(stdout, "File already exists. Overwrite? (y/n): ");
			fgets(yes_no, 199, stdin);
			
			if(yes_no[0] != 'y' && yes_no[0] != 'Y')
			{
				fprintf(stdout, "Canceled.\n");
				return;
			}
		}
		SaveImageAsTGA(path);
		
		fprintf(stdout, "Saved.\n");
	}
	if(id == 2)
	{
		// *****************
		// Save image as BMP
		// *****************
		
		if(!strstr(path, ".bmp") && !strstr(path, ".BMP"))
		{
			strcat(path, ".bmp");
		}
		if(QFileExists(path))
		{
			char yes_no[200];
			fprintf(stdout, "File already exists. Overwrite? (y/n): ");
			fgets(yes_no, 199, stdin);
			
			if(yes_no[0] != 'y' && yes_no[0] != 'Y')
			{
				fprintf(stdout, "Canceled.\n");
				return;
			}
		}
		SaveImageAsBMP(path);
		
		fprintf(stdout, "Saved.\n");
	}
	else if(id == 3)
	{
		// ********************
		// Save rendering state
		// ********************
		
		if(QFileExists(path))
		{
			char yes_no[200];
			fprintf(stdout, "File already exists. Overwrite? (y/n): ");
			fgets(yes_no, 199, stdin);
			
			if(yes_no[0] != 'y' && yes_no[0] != 'Y')
			{
				fprintf(stdout, "Canceled.\n");
				return;
			}
		}
		FILE *rstate = fopen(path, "wb");
		
		if(!rstate)
		{
			fprintf(stdout, "Can't open file\"%s\"\n", path);
			return;
		}
		// ************************************************
		// 30+ global variables is a BITCH to keep track of
		// ************************************************
		
		int version = VERSION;
		fwrite(&version, sizeof(int), 1, rstate);
		fwrite(&VIEW_X, sizeof(int), 1, rstate);
		fwrite(&VIEW_Y, sizeof(int), 1, rstate);
		fwrite(&SLICE_POS, sizeof(int), 1, rstate);
		fwrite(&SLICE_HLT_POS, sizeof(int), 1, rstate);
		fwrite(&SLICE_RES, sizeof(int), 1, rstate);
		fwrite(&BLOCK_SIZE, sizeof(int), 1, rstate);
		fwrite(&SSAMP_SIZE, sizeof(int), 1, rstate);
		SaveSliceBuffer(rstate);
		fwrite(&ITERATION_COUNT[1], sizeof(int), 1, rstate);
		fwrite(&ITERATION_COUNT[2], sizeof(int), 1, rstate);
		fwrite(&ITERATION_COUNT[3], sizeof(int), 1, rstate);
		fwrite(&JULIA_MODE, sizeof(int), 1, rstate);
		fwrite(&JULIA_X, sizeof(double), 1, rstate);
		fwrite(&JULIA_Y, sizeof(double), 1, rstate);
		fwrite(&JULIA_ZOOM_INDEX, sizeof(int), 1, rstate);
		fwrite(&ESCAPE1_COLOR_DEF, sizeof(int), 1, rstate);
		fwrite(&ESCAPE2_COLOR_DEF, sizeof(int), 1, rstate);
		int adj = sizeof(pix_t)*(MAX_ITERATIONS+1);
		fwrite(&ESCAPE1_COLOR_DAT, sizeof(cdat_t)-adj, 1, rstate);
		fwrite(&ESCAPE2_COLOR_DAT, sizeof(cdat_t)-adj, 1, rstate);
		fwrite(&ESCAPE2_THRESHOLD, sizeof(double), 1, rstate);
		fwrite(&ESCAPE2_EQUATION, sizeof(int), 1, rstate);
		fwrite(&ESCAPE2_MIN_ITER, sizeof(int), 1, rstate);
		fwrite(&ESCAPE2_MAX_ITER, sizeof(int), 1, rstate);
		fwrite(&ESCAPE1_BLEND_FACTOR, sizeof(double), 1, rstate);
		fwrite(&ESCAPE1_XBAND_FACTOR, sizeof(double), 1, rstate);
		fwrite(&ESCAPE2_SHADE_FACTOR, sizeof(double), 1, rstate);
		fwrite(&ITERATIONS_PER_PIXEL, sizeof(int), 1, rstate);
		fwrite(&HYSTERIA_MODE, sizeof(int), 1, rstate);
		fwrite(&HYSTERIA_FACTOR, sizeof(double), 1, rstate);
		fwrite(&FOCUS, sizeof(double), 1, rstate);
		//fwrite(&REALTIME_STAT_UPDATE, sizeof(int), 1, rstate);
		fwrite(&ZOOM_COUNT, sizeof(int), 1, rstate);
		fwrite(ZOOM_LIST, sizeof(zoom_t), ZOOM_COUNT, rstate);
		fclose(rstate);
		
		fprintf(stdout, "Saved.\n");
	}
	else if(id == 4)
	{
		// ********************
		// Load rendering state
		// ********************
		
		FILE *rstate = fopen(path, "rb");
		
		if(!rstate)
		{
			fprintf(stdout, "Can't open file\"%s\"\n", path);
			return;
		}
		int version;
		fread(&version, sizeof(int), 1, rstate);
		
		if(version != VERSION)
		{
			fprintf(stdout, "Incompatible versions!\n");
			fclose(rstate);
			return;
		}
		fread(&VIEW_X, sizeof(int), 1, rstate);
		fread(&VIEW_Y, sizeof(int), 1, rstate);
		fread(&SLICE_POS, sizeof(int), 1, rstate);
		fread(&SLICE_HLT_POS, sizeof(int), 1, rstate);
		fread(&SLICE_RES, sizeof(int), 1, rstate);
		fread(&BLOCK_SIZE, sizeof(int), 1, rstate);
		fread(&SSAMP_SIZE, sizeof(int), 1, rstate);
		SLICE_X = VIEW_X/SLICE_RES;
		SLICE_Y = VIEW_Y/SLICE_RES;
		LoadSliceBuffer(rstate);
		fread(&ITERATION_COUNT[1], sizeof(int), 1, rstate);
		fread(&ITERATION_COUNT[2], sizeof(int), 1, rstate);
		fread(&ITERATION_COUNT[3], sizeof(int), 1, rstate);
		fread(&JULIA_MODE, sizeof(int), 1, rstate); // update MENU0:1
		fread(&JULIA_X, sizeof(double), 1, rstate);
		fread(&JULIA_Y, sizeof(double), 1, rstate);
		fread(&JULIA_ZOOM_INDEX, sizeof(int), 1, rstate);
		fread(&ESCAPE1_COLOR_DEF, sizeof(int), 1, rstate); // update MENU0:6
		fread(&ESCAPE2_COLOR_DEF, sizeof(int), 1, rstate); // update MENU0:8
		int adj = sizeof(pix_t)*(MAX_ITERATIONS+1);
		fread(&ESCAPE1_COLOR_DAT, sizeof(cdat_t)-adj, 1, rstate); // 0:7
		fread(&ESCAPE2_COLOR_DAT, sizeof(cdat_t)-adj, 1, rstate); // 0:9
		RecomputeColorGradient(&ESCAPE1_COLOR_DAT);
		RecomputeColorGradient(&ESCAPE2_COLOR_DAT);
		fread(&ESCAPE2_THRESHOLD, sizeof(double), 1, rstate); // 11:
		fread(&ESCAPE2_EQUATION, sizeof(int), 1, rstate); // 0:10
		fread(&ESCAPE2_MIN_ITER, sizeof(int), 1, rstate); // 11:
		fread(&ESCAPE2_MAX_ITER, sizeof(int), 1, rstate); // 11:
		fread(&ESCAPE1_BLEND_FACTOR, sizeof(double), 1, rstate); // 11:
		fread(&ESCAPE1_XBAND_FACTOR, sizeof(double), 1, rstate); // 11:
		fread(&ESCAPE2_SHADE_FACTOR, sizeof(double), 1, rstate); // 11:
		fread(&ITERATIONS_PER_PIXEL, sizeof(int), 1, rstate); // 
		fread(&HYSTERIA_MODE, sizeof(int), 1, rstate); // 
		fread(&HYSTERIA_FACTOR, sizeof(double), 1, rstate); // 11:
		fread(&FOCUS, sizeof(double), 1, rstate); // 
		//fread(&REALTIME_STAT_UPDATE, sizeof(int), 1, rstate);
		fread(&ZOOM_COUNT, sizeof(int), 1, rstate);
		fread(ZOOM_LIST, sizeof(zoom_t), ZOOM_COUNT, rstate);
		fclose(rstate);
		
		// ************
		// Update menus
		// ************
		
		UpdateMenu_1();
		UpdateMenu_2();
		UpdateMenu_3();
		UpdateMenu_4();
		UpdateMenu_5();
		UpdateMenu_6();
		UpdateMenu_7();
		UpdateMenu_8();
		UpdateMenu_9();
		UpdateMenu_10();
		UpdateMenu_11();
		UpdateMenu_13();
		
		// ***************************
		// Reconstruct other variables
		// ***************************
		
		int old_win_x = WINDOW_X;
		int old_win_y = WINDOW_Y;
		
		WINDOW_X = VIEW_X + ICON_BAR;
		WINDOW_Y = VIEW_Y + STAT_BAR;
		SLICE_UPDATE = 0;
		SLICE_TMP_COUNT = 0;
		
		if(WINDOW_X != old_win_x || WINDOW_Y != old_win_y)
		{
			// *********************************************
			// Window will be redrawn inside GLUT__Reshape()
			// *********************************************
			
			APP_RESHAPE_EVENT = 1;
			glutReshapeWindow(WINDOW_X, WINDOW_Y);
		}
		else
		{
			// ***************************************
			// Window doesn't need reshaping - so just
			// redraw it here..
			// ***************************************
			
			Redraw();
		}
	}
	else if(id == 5)
	{
		// ***********************************
		// Set min/max coordinates via console
		// ***********************************
		
		char str[200];
		double q[4], bounds[4];
		
		for(int i = 0; i < 4; i++)
		{
			switch(i)
			{
				case 0: fprintf(stdout, "Enter X Min: "); break;
				case 1: fprintf(stdout, "Enter Y Min: "); break;
				case 2: fprintf(stdout, "Enter X Max: "); break;
				case 3: fprintf(stdout, "Enter Y Max: "); break;
			}
			fgets(str, 199, stdin);
			if(str[0] == '\n')
			{
				fprintf(stdout, "Canceled.\n");
				return;
			}
			q[i] = atof(str);
		}
		bounds[0] = _max(_min(q[0], q[2]), -3.0);
		bounds[1] = _max(_min(q[1], q[3]), -3.0);
		bounds[2] = _min(_max(q[0], q[2]), +3.0);
		bounds[3] = _min(_max(q[1], q[3]), +3.0);
		
		if(bounds[0] == bounds[2] || bounds[1] == bounds[3])
		{
			fprintf(stdout, "Invalid bounds!\n");
			return;
		}
		ZOOM_LIST[ZOOM_COUNT].x =     -(bounds[0] + bounds[2])/2.0;
		ZOOM_LIST[ZOOM_COUNT].y =     -(bounds[1] + bounds[3])/2.0;
		ZOOM_LIST[ZOOM_COUNT].z = -2.0/(bounds[1] - bounds[3]);
		ZOOM_COUNT++;
		Clear();
	}
	else if(id == 6)
	{
		LoadColorPresetList(C_PRESET_PATH);
		LoadColorPreset(&ESCAPE1_COLOR_DAT, C_PRESET_PATH,
			ESCAPE1_COLOR_DAT.name);
		LoadColorPreset(&ESCAPE2_COLOR_DAT, C_PRESET_PATH,
			ESCAPE2_COLOR_DAT.name);
	}
	else if(id == 7) exit(0);
}

static void MenuCallback_14(int id) // Julia presets
{
	char str[200];
	double jx, jy;
	
	if(id-1 < MAX_JULIA_DEFAULT_PRESETS-1)
	{
		jx = JULIA_DEFAULT_COORDS[(id-1)*2+0];
		jy = JULIA_DEFAULT_COORDS[(id-1)*2+1];
	}
	else
	{
		fprintf(stdout, "Enter Julia X: ");
		fgets(str, 199, stdin);
		
		if(str[0] == '\n')
		{
			fprintf(stdout, "Canceled.\n");
			return;
		}
		jx = atof(str);
		fprintf(stdout, "Enter Julia Y: ");
		fgets(str, 199, stdin);
		
		if(str[0] == '\n')
		{
			fprintf(stdout, "Canceled.\n");
			return;
		}
		jy = atof(str);
	}
	if(jx != JULIA_X || jy != JULIA_Y || JULIA_MODE != 1)
	{
		JULIA_X = jx;
		JULIA_Y = jy;
		
		if(JULIA_MODE)
		{
			// **********
			// Reset zoom
			// **********
			
			ZOOM_COUNT = JULIA_ZOOM_INDEX;
		}
		JULIA_ZOOM_INDEX = ZOOM_COUNT;
		JULIA_MODE = 1;
		ZOOM_LIST[ZOOM_COUNT].x = 0.00;
		ZOOM_LIST[ZOOM_COUNT].y = 0.00;
		ZOOM_LIST[ZOOM_COUNT].z = 0.75;
		ZOOM_LIST[ZOOM_COUNT].iteration_bias = 0;
		ZOOM_COUNT++;
		Clear();
	}
	glutSetMenu(M_ID[0]);
	glutChangeToSubMenu(1, "Mode: Julia Set", M_ID[1]);
}

static void UpdateMenu_1(void) // mode
{
	glutSetMenu(M_ID[0]);
	
	switch(JULIA_MODE)
	{
		case 0:
			glutChangeToSubMenu(1, "Mode: Mandelbrot Set", M_ID[1]);
			break;
		case 1:
			glutChangeToSubMenu(1, "Mode: Julia Set", M_ID[1]);
			break;
		case 2:
			glutChangeToSubMenu(1,
			"Mode: Julia/Mandelbrot Hybrid Fractal", M_ID[1]);
			break;
	}
}

static void MenuCallback_1(int id) // mode
{
	if(id == 1 && JULIA_MODE != 0)
	{
		JULIA_MODE = 0;
		ZOOM_COUNT = JULIA_ZOOM_INDEX;
		UpdateMenu_1();
		Clear();
	}
	else if(id == 3 && JULIA_MODE != 2)
	{
		if(JULIA_MODE)
		{
			// **********
			// Reset zoom
			// **********
			
			ZOOM_COUNT = JULIA_ZOOM_INDEX;
		}
		JULIA_ZOOM_INDEX = ZOOM_COUNT;
		JULIA_MODE = 2;
		ZOOM_LIST[ZOOM_COUNT].x = 0.00;
		ZOOM_LIST[ZOOM_COUNT].y = 0.00;
		ZOOM_LIST[ZOOM_COUNT].z = 0.75;
		ZOOM_LIST[ZOOM_COUNT].iteration_bias = 0;
		ZOOM_COUNT++;
		UpdateMenu_1();
		Clear();
	}
}

static void UpdateMenu_2(void) // resolution
{
	char str[200];
	
	sprintf(str, "Resolution: %ix%i", BLOCK_SIZE, BLOCK_SIZE);
	glutSetMenu(M_ID[0]);
	glutChangeToSubMenu(2, str, M_ID[2]);
}

static void MenuCallback_2(int id) // resolution
{
	int vtab[] = {16, 8, 4, 2, 1};
	
	if(BLOCK_SIZE != vtab[id-1])
	{
		BLOCK_SIZE = vtab[id-1];
		UpdateMenu_2();
		Clear();
	}
}

static void UpdateMenu_3(void) // supersampling
{
	char str[200];
	
	if(SSAMP_SIZE == 1) strcpy(str, "Supersampling: Off");
	else sprintf(str, "Supersampling: %ix%i", SSAMP_SIZE, SSAMP_SIZE);
	glutSetMenu(M_ID[0]);
	glutChangeToSubMenu(3, str, M_ID[3]);
}

static void MenuCallback_3(int id) // supersampling
{
	int vtab[] = {1, 2, 4, 8, 12};
	int result;
	
	if(id == 99)
	{
		char s[200];
		fprintf(stdout, "Enter supersampling size (1-16): ");
		fgets(s, 199, stdin);
		if(strlen(s) < 1) return; // nothing typed
		result = atoi(s);
		if(result < 1)  result = 1;
		if(result > 16) result = 16;
	}
	else result = vtab[id-1];
	
	if(SSAMP_SIZE != result)
	{
		SSAMP_SIZE = result;
		UpdateMenu_3();
		Clear();
	}
}

static void UpdateMenu_4(void) // focus
{
	char str[200];
	
	sprintf(str, "Focus: %.2f", FOCUS);
	glutSetMenu(M_ID[0]);
	glutChangeToSubMenu(4, str, M_ID[4]);
}

static void MenuCallback_4(int id) // focus
{
	double vtab[] = {0.2, 0.5, 1.0, 1.1, 1.5, 2.0, 3.0};
	double result;
	
	if(id == 99)
	{
		char s[200];
		fprintf(stdout, "Enter focus (0-10): ");
		fgets(s, 199, stdin);
		if(strlen(s) < 1) return; // nothing typed
		result = atof(s);
		if(result <  0.0) result =  0.0;
		if(result > 10.0) result = 10.0;
	}
	else result = vtab[id-1];
	
	if(FOCUS != result)
	{
		FOCUS = result;
		UpdateMenu_4();
		Clear();
	}
}

static void UpdateMenu_5(void) // iterations
{
	char str[200];
	
	sprintf(str, "Iterations: %i", ITERATIONS_PER_PIXEL);
	glutSetMenu(M_ID[0]);
	glutChangeToSubMenu(5, str, M_ID[5]);
}

static void MenuCallback_5(int id) // iterations
{
	int vtab[] = {32, 64, 128, 256, 512, 1024, 2048};
	int result;
	
	if(id == 99)
	{
		char s[200];
		fprintf(stdout, "Enter max iterations per pixel (4-32768): ");
		fgets(s, 199, stdin);
		if(strlen(s) < 1) return; // nothing typed
		result = atoi(s);
		if(result < 4) result = 4;
		if(result > 32768) result = 32768;
	}
	else result = vtab[id-1];
	
	if(ITERATIONS_PER_PIXEL != result)
	{
		ITERATIONS_PER_PIXEL = result;
		UpdateMenu_5();
		Clear();
	}
}

static void UpdateMenu_6(void) // layer-1 color method
{
	char str[200];
	
	switch(ESCAPE1_COLOR_DEF)
	{
		case 0: strcpy(str, "Layer-1 Color Method: None"); break;
		case 1: strcpy(str, "Layer-1 Color Method: Constant Color"); break;
		case 2: strcpy(str, "Layer-1 Color Method: Non-Interpolated"); break;
		case 3: strcpy(str, "Layer-1 Color Method: Interpolated"); break;
		case 4: strcpy(str, "Layer-1 Color Method: Banded"); break;
	}
	glutSetMenu(M_ID[0]);
	glutChangeToSubMenu(6, str, M_ID[6]);
}

static void MenuCallback_6(int id) // layer-1 color method
{
	if(ESCAPE1_COLOR_DEF != id-1)
	{
		ESCAPE1_COLOR_DEF = id-1;
		UpdateMenu_6();
		Clear();
	}
}

static void UpdateMenu_7(void) // layer-1 color gradient
{
	char str[200];
	
	glutSetMenu(M_ID[0]);
	sprintf(str, "Layer-1 Color Gradient: %s", ESCAPE1_COLOR_DAT.name);
	glutChangeToSubMenu(7, str, M_ID[7]);
}

static void MenuCallback_7(int id) // layer-1 color gradient
{
	char *name = C_PRESET_NAMES[id-1];
	if(id > C_PRESET_COUNT) return;
	if(!strcmp(ESCAPE1_COLOR_DAT.name, name)) return;
	
	LoadColorPreset(&ESCAPE1_COLOR_DAT, C_PRESET_PATH, name);
	UpdateMenu_7();
	Clear();
}

static void UpdateMenu_8(void) // layer-2 color method
{
	char str[200];
	
	switch(ESCAPE2_COLOR_DEF)
	{
		case 0: strcpy(str, "Layer-2 Color Method: None"); break;
		case 1: strcpy(str, "Layer-2 Color Method: Constant Color"); break;
		case 2: strcpy(str, "Layer-2 Color Method: Smooth Shaded"); break;
		case 3: strcpy(str, "Layer-2 Color Method: Reverse Ordered"); break;
		case 4: strcpy(str, "Layer-2 Color Method: Reverse Color Min"); break;
	}
	glutSetMenu(M_ID[0]);
	glutChangeToSubMenu(8, str, M_ID[8]);
}

static void MenuCallback_8(int id) // layer-2 color method
{
	if(ESCAPE2_COLOR_DEF != id-1)
	{
		ESCAPE2_COLOR_DEF = id-1;
		UpdateMenu_8();
		Clear();
	}
}

static void UpdateMenu_9(void) // layer-1 color gradient
{
	char str[200];
	
	glutSetMenu(M_ID[0]);
	sprintf(str, "Layer-2 Color Gradient: %s", ESCAPE2_COLOR_DAT.name);
	glutChangeToSubMenu(9, str, M_ID[9]);
}

static void MenuCallback_9(int id) // layer-2 color gradient
{
	char *name = C_PRESET_NAMES[id-1];
	if(id > C_PRESET_COUNT) return;
	if(!strcmp(ESCAPE2_COLOR_DAT.name, name)) return;
	
	LoadColorPreset(&ESCAPE2_COLOR_DAT, C_PRESET_PATH, name);
	UpdateMenu_9();
	Clear();
}

static void UpdateMenu_10(void) // layer-2 escape equation
{
	char str[200];
	
	switch(ESCAPE2_EQUATION)
	{
		case 0: strcpy(str, "Layer-2 Escape Equation: x^2 + y^2");   break;
		case 1: strcpy(str, "Layer-2 Escape Equation: x^2 - y^2");   break;
		case 2: strcpy(str, "Layer-2 Escape Equation: dx^2 - dy^2"); break;
		case 3: strcpy(str, "Layer-2 Escape Equation: x dx + y dy"); break;
		case 4: strcpy(str, "Layer-2 Escape Equation: x dx - y dy"); break;
		case 5: strcpy(str, "Layer-2 Escape Equation: x dy - y dx"); break;
		case 6: strcpy(str, "Layer-2 Escape Equation: x y - dx dy"); break;
		case 7: strcpy(str,
		        "Layer-2 Escape Equation: (x^4 + y^4)/(x^2 - y^2)"); break;
		case 8: strcpy(str,
		        "Layer-2 Escape Equation: x cos(dy) + y sin(dx)"); break;
		case 9: strcpy(str,
		        "Layer-2 Escape Equation: |x|dx + |y|dy"); break;
	}
	glutSetMenu(M_ID[0]);
	glutChangeToSubMenu(10, str, M_ID[10]);
}

static void MenuCallback_10(int id) // layer-2 escape equation
{	
	if(ESCAPE2_EQUATION != id-1)
	{
		ESCAPE2_EQUATION = id-1;
		UpdateMenu_10();
		if(ESCAPE2_COLOR_DEF) Clear();
	}
}

static void UpdateMenu_11(void) // advanced options
{
	char str[200];
	
	glutSetMenu(M_ID[11]);
	
	sprintf(str, "Set Layer-1 Color Exponent... %.3f",
	ESCAPE1_COLOR_DAT.exponent);
	glutChangeToMenuEntry(1, str, 1);
	sprintf(str,
	"Set Layer-1 Color Interpolation Factor... %.3f",
	ESCAPE1_BLEND_FACTOR);
	glutChangeToMenuEntry(2, str, 2);
	sprintf(str,
	"Set Layer-1 Color Banding Scale Factor... %.3f",
	ESCAPE1_XBAND_FACTOR);
	glutChangeToMenuEntry(3, str, 3);
	sprintf(str, "Set Layer-2 Color Exponent... %.3f",
	ESCAPE2_COLOR_DAT.exponent);
	glutChangeToMenuEntry(5, str, 5);
	sprintf(str,
	"Set Layer-2 Color Shade Scale Factor... %.3f",
	ESCAPE2_SHADE_FACTOR);
	glutChangeToMenuEntry(6, str, 6);
	sprintf(str, "Set Layer-2 Escape Threshold... %.7f",
	ESCAPE2_THRESHOLD);
	glutChangeToMenuEntry(7, str, 7);
	sprintf(str, "Set Layer-2 Min Iterations... %i",
	ESCAPE2_MIN_ITER);
	glutChangeToMenuEntry(8, str, 8);
	sprintf(str, "Set Layer-2 Max Iterations... %i",
	ESCAPE2_MAX_ITER);
	glutChangeToMenuEntry(9, str, 9);
	sprintf(str, "Set Hysteria Factor... %.5f",
	HYSTERIA_FACTOR);
	glutChangeToMenuEntry(10, str, 10);
}

static void MenuCallback_11(int id) // advanced options
{
	char str[200];
	
	glutSetMenu(M_ID[11]);
	
	if(id <= 10)
	{
		fprintf(stdout, "Enter value: ");
		fgets(str, 199, stdin);
		
		if(str[0] == '\n')
		{
			fprintf(stdout, "Canceled.\n");
			return;
		}
		if(id == 1)
		{
			ESCAPE1_COLOR_DAT.exponent = atof(str);
			RecomputeColorGradient(&ESCAPE1_COLOR_DAT);
			sprintf(str, "Set Layer-1 Color Exponent... %.3f", atof(str));
			glutChangeToMenuEntry(1, str, 1);
		}
		else if(id == 2)
		{
			ESCAPE1_BLEND_FACTOR = atof(str);
			sprintf(str,
			"Set Layer-1 Color Interpolation Factor... %.3f", atof(str));
			glutChangeToMenuEntry(2, str, 2);
		}
		else if(id == 3)
		{
			ESCAPE1_XBAND_FACTOR = atof(str);
			sprintf(str,
			"Set Layer-1 Color Banding Scale Factor... %.3f", atof(str));
			glutChangeToMenuEntry(3, str, 3);
		}
		else if(id == 4)
		{
			if(str[0] == '?')
			{
				fprintf(stdout, "Iteration Bias= %i\n",
				ZOOM_LIST[ZOOM_COUNT-1].iteration_bias);
			}
			else
			{
				//WARNING: very dangerous!
				ZOOM_LIST[ZOOM_COUNT-1].iteration_bias = atoi(str);
			}
		}
		else if(id == 5)
		{
			ESCAPE2_COLOR_DAT.exponent = atof(str);
			RecomputeColorGradient(&ESCAPE2_COLOR_DAT);
			sprintf(str, "Set Layer-2 Color Exponent... %.3f", atof(str));
			glutChangeToMenuEntry(5, str, 5);
		}
		else if(id == 6)
		{
			ESCAPE2_SHADE_FACTOR = atof(str);
			sprintf(str,
			"Set Layer-2 Color Shade Scale Factor... %.3f", atof(str));
			glutChangeToMenuEntry(6, str, 6);
		}
		else if(id == 7)
		{
			ESCAPE2_THRESHOLD = atof(str);
			sprintf(str, "Set Layer-2 Escape Threshold... %.7f", atof(str));
			glutChangeToMenuEntry(7, str, 7);
		}
		else if(id == 8)
		{
			ESCAPE2_MIN_ITER = atoi(str);
			sprintf(str, "Set Layer-2 Min Iterations... %i", atoi(str));
			glutChangeToMenuEntry(8, str, 8);
		}
		else if(id == 9)
		{
			ESCAPE2_MAX_ITER = atoi(str);
			sprintf(str, "Set Layer-2 Max Iterations... %i", atoi(str));
			glutChangeToMenuEntry(9, str, 9);
		}
		else if(id == 10)
		{
			HYSTERIA_FACTOR = atof(str);
			sprintf(str, "Set Hysteria Factor... %.5f", atof(str));
			glutChangeToMenuEntry(10, str, 10);
		}
	}
	else if(id == 11)
	{
		ESCAPE1_COLOR_DAT.exponent = 3.0;
		ESCAPE1_BLEND_FACTOR = 2.75;
		ESCAPE1_XBAND_FACTOR = 10.0;
		ESCAPE2_COLOR_DAT.exponent = 17.0;
		ESCAPE2_SHADE_FACTOR = 100.0;
		ESCAPE2_THRESHOLD = 0.03125;
		ESCAPE2_MIN_ITER = 1;
		ESCAPE2_MAX_ITER = MAX_ITERATIONS;
		HYSTERIA_FACTOR = 64.0;
		RecomputeColorGradient(&ESCAPE1_COLOR_DAT);
		RecomputeColorGradient(&ESCAPE2_COLOR_DAT);
		
		glutChangeToMenuEntry(1,
		"Set Layer-1 Color Exponent... 3.000", 1);
		glutChangeToMenuEntry(2,
		"Set Layer-1 Color Interpolation Factor... 2.750", 2);
		glutChangeToMenuEntry(3,
		"Set Layer-1 Color Banding Scale Factor... 10.000", 3);
		glutChangeToMenuEntry(4,
		"Set Layer-1 Iteration Bias... <automatic>", 4);
		glutChangeToMenuEntry(5,
		"Set Layer-2 Color Exponent... 17.000", 5);
		glutChangeToMenuEntry(6,
		"Set Layer-2 Color Shade Scale Factor... 100.000", 6);
		glutChangeToMenuEntry(7,
		"Set Layer-2 Escape Threshold... 0.0312500", 7);
		glutChangeToMenuEntry(8,
		"Set Layer-2 Min Iterations... 1", 8);
		glutChangeToMenuEntry(9,
		"Set Layer-2 Max Iterations... 32768", 9);
		glutChangeToMenuEntry(10,
		"Set Hysteria Factor... 64.00000", 10);
	}
	Clear();
}

static void MenuCallback_12(int id) // realtime stat
{
	char str[200];
	
	if(id == 1)
	{
		sprintf(str, "Realtime Status Update: On");
		REALTIME_STAT_UPDATE = 1;
	}
	else
	{
		sprintf(str, "Realtime Status Update: Off");
		REALTIME_STAT_UPDATE = 0;
	}
	glutSetMenu(M_ID[0]);
	glutChangeToSubMenu(12, str, M_ID[12]);
}

static void UpdateMenu_13(void) // Hysteria by James Beattie
{
	char str[200];
	
	if(HYSTERIA_MODE)
	{
		sprintf(str, "Hysteria by James Beattie: On");
	}
	else
	{
		sprintf(str, "Hysteria by James Beattie: Off");
	}
	glutSetMenu(M_ID[0]);
	glutChangeToSubMenu(13, str, M_ID[13]);
}

static void MenuCallback_13(int id) // Hysteria by James Beattie
{
	if(id == 1 && HYSTERIA_MODE == 0)
	{
		HYSTERIA_MODE = 1;
		UpdateMenu_13();
		Clear();
	}
	else if(id == 2 && HYSTERIA_MODE == 1)
	{
		HYSTERIA_MODE = 0;
		UpdateMenu_13();
		Clear();
	}
}

static void RecomputeColorGradient(cdat_t *cdat)
{
	for(int k = 1, i = 0; i <= MAX_ITERATIONS; i++)
	{
		double t = pow(1.0 - 1.0/(double)(i+1), cdat->exponent);
		while(t >= cdat->colors[k*4]) k++;
		
		t = (cdat->colors[k*4] - t) /
		    (cdat->colors[k*4] - cdat->colors[(k-1)*4]);
		
		double r1 = cdat->colors[(k-1)*4+1]*t;
		double g1 = cdat->colors[(k-1)*4+2]*t;
		double b1 = cdat->colors[(k-1)*4+3]*t;
		double r2 = cdat->colors[k*4+1]*(1.0-t);
		double g2 = cdat->colors[k*4+2]*(1.0-t);
		double b2 = cdat->colors[k*4+3]*(1.0-t);
		
		pix_t r = (pix_t)((r1+r2)*255.0);
		pix_t g = (pix_t)((g1+g2)*255.0);
		pix_t b = (pix_t)((b1+b2)*255.0);
		
		cdat->table[i] = r|(g<<8)|(b<<16);
	}
}

static void LoadColorPreset(cdat_t *cdat, char *path, char *name)
{
	FILE *cprefs = fopen(path, "r");
	char str[200];
	
	if(!cprefs)
	{
		fprintf(stdout, "Can't open file \"%s\"\n", path);
		return;
	}
	while(fgets(str, 199, cprefs))
	{
		if(str[0] == '#') continue;
		if(str[0] == '/' && str[1] == '/') continue;
		if(str[0] == '\"' && !strncmp(str+1, name, strlen(name)))
		{
			cdat->count = atoi(str + strlen(name) + 3);
			
			for(int k = 0; k < cdat->count; k++)
			{
				fgets(str, 199, cprefs);
				sscanf(str, "%lf %lf %lf %lf\n",
					&cdat->colors[k*4+0],
					&cdat->colors[k*4+1],
					&cdat->colors[k*4+2],
					&cdat->colors[k*4+3]);
			}
			RecomputeColorGradient(cdat);
			strncpy(cdat->name, name, 47);
			break;
		}
	}
	fclose(cprefs);
}

static void LoadColorPresetList(char *path)
{
	FILE *cprefs = fopen(path, "rb");
	char str[200];
	int i;
	
	if(!cprefs)
	{
		fprintf(stdout, "Can't open file \"%s\"\n", path);
		return;
	}
	C_PRESET_COUNT = 0;
	
	while(fgets(str, 199, cprefs))
	{
		if(str[0] == '#') continue;
		if(str[0] == '/' && str[1] == '/') continue;
		if(str[0] == '\"')
		{
			char *quot = strchr(str+1, '\"');
			if(!quot) continue;
			int len = _min(quot-str-1, 47);
			memmove(str, str+1, len);
			str[len] = '\0';
			
			strcpy(C_PRESET_NAMES[C_PRESET_COUNT++], str);

			if(C_PRESET_COUNT == MAX_C_PRESETS)
			{
				break;
			}
		}
	}
	fclose(cprefs);
	
	glutSetMenu(M_ID[7]);
	
	for(i = 0; i < MAX_C_PRESETS; i++)
	{
		if(i < C_PRESET_COUNT)
		{
			glutChangeToMenuEntry(i+1, C_PRESET_NAMES[i], i+1);
		}
		else
		{
			glutChangeToMenuEntry(i+1, "<...>", i+1);
		}
	}
	glutSetMenu(M_ID[9]);
	
	for(i = 0; i < MAX_C_PRESETS; i++)
	{
		if(i < C_PRESET_COUNT)
		{
			glutChangeToMenuEntry(i+1, C_PRESET_NAMES[i], i+1);
		}
		else
		{
			glutChangeToMenuEntry(i+1, "<...>", i+1);
		}
	}
}

static void SetPixelCoords(int lft, int bot, int rgt, int top, int clear)
{
	glViewport(lft, bot, rgt-lft, top-bot);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f, (float)(rgt-lft), 0.f, (float)(top-bot), 1.f, -1.f);
	
	if(clear)
	{
		glRecti(0, 0, rgt-lft, top-bot);
	}
}

static void Pix2WorldCoords(double &x, double &y)
{
	// **********************************************
	// Convert pixel-coordinates to world-coordinates
	// **********************************************
	
	double vx = (double)(VIEW_X/2);
	double vy = (double)(VIEW_Y/2);
	zoom_t *z = &ZOOM_LIST[ZOOM_COUNT-1];
	
	x = (x-vx)/(vy*z->z) - z->x;
	y = (y-vy)/(vy*z->z) - z->y;
}

static void DrawText(int x, int y, char *str)
{
	glRasterPos2i(x, y);
	
	while(*str != '\0')
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *(str++));
	}
}

static void IncrementCount(int dcount)
{
	// **********************************************
	// Increment iteration counters. There are
	// three 32-bit signed values:
	//   [0]: local counter for managing GUI updates
	//   [1]: low-order 9 digits
	//   [2]: high-order 9 digits (total = 18 digits)
	//   [3]: local min-iter counter for current view,
	//        used for ZOOM->iteration_bias (updated
	//        within Iterate)
	// **********************************************
	
	ITERATION_COUNT[0] += dcount; // never overflows!
	ITERATION_COUNT[1] += dcount;
	
	if(ITERATION_COUNT[1] > 999999999)
	{
		ITERATION_COUNT[1] -= 1000000000;
		ITERATION_COUNT[2] ++;
	}
}

static void AppendCountStr(char *str)
{
	char tmp[31];
	
	if(ITERATION_COUNT[2])
	{
		int mask = 100000000; //10^8
		
		sprintf(tmp, "%i", ITERATION_COUNT[2]);
		strcat(str, tmp);
		
		while(mask > 1)
		{
			if(mask > ITERATION_COUNT[1])
			{
				strcat(str, "0");
			}
			mask /= 10;
		}
		sprintf(tmp, "%i", ITERATION_COUNT[1]);
		strcat(str, tmp);
	}
	else
	{
		sprintf(tmp, "%i", ITERATION_COUNT[1]);
		strcat(str, tmp);
	}
}

static void SPrintFloat(char *str, char *prefix, double t)
{
	char tmp[75];
	
	if(t < 0.0)
	{
		sprintf(tmp, "%1.16lf", t);
	}
	else
	{
		sprintf(tmp, " %1.16lf", t);
	}
	strcpy(str, prefix);
	strcat(str, tmp);
}

static void DrawStatus(int field)
{
	// ****************************************
	// Update icon/status bar. Use field=0 to
	// update everything, otherwise use field>0
	// to specify a particular field.
	// ****************************************
	
	zoom_t *zoom = &ZOOM_LIST[ZOOM_COUNT-1];
	char buf[200];
	
	glColor3dv(STAT_BAR_COLOR);
	
	SetPixelCoords(0, 0, ICON_BAR, WINDOW_Y, field == 0);
	SetPixelCoords(ICON_BAR, 0, WINDOW_X, STAT_BAR, field == 0);
	
	if(field == 0)
	{
		double asp = (double)VIEW_X / (double)VIEW_Y;
		
		glColor3dv(STAT_TEXT_COLOR);
		SPrintFloat(buf, "Min X= ", -asp/zoom->z - zoom->x);
		DrawText(10, 32+13*4, buf);
		SPrintFloat(buf, "Min Y= ", -1.0/zoom->z - zoom->y);
		DrawText(10, 32+13*3, buf);
		SPrintFloat(buf, "Max X= ", +asp/zoom->z - zoom->x);
		DrawText(10, 32+13*2, buf);
		SPrintFloat(buf, "Max Y= ", +1.0/zoom->z - zoom->y);
		DrawText(10, 32+13*1, buf);
		sprintf(buf, "Scale=  %.3lf", zoom->z);
		DrawText(10, 32+13*0, buf);
		
		SPrintFloat(buf, "Julia X= ", JULIA_X);
		DrawText(250, 32+13*2, buf);
		SPrintFloat(buf, "Julia Y= ", JULIA_Y);
		DrawText(250, 32+13*1, buf);
	}
	if(field == 0 || field == 1)
	{
		if(field == 1)
		{
			glColor3dv(STAT_BAR_COLOR);
			glRecti(250, 32+13*0-2, VIEW_X, 32+13*1-1);
		}
		glColor3dv(STAT_TEXT_COLOR);
		sprintf(buf, "Iteration Count= ");
		AppendCountStr(buf);
		DrawText(250, 32+13*0, buf);
	}
	if(field == 0 || field == 2)
	{
		int layer, iter = MOUSE_ITERATIONS & 0x7FFFFFFF;
		if(MOUSE_ITERATIONS == -1) layer = iter = -1;
		else if(MOUSE_ITERATIONS & 0x80000000) layer = 2; 
		else layer = 1;
		
		if(field == 2)
		{
			glColor3dv(STAT_BAR_COLOR);
			glRecti(250, 32+13*3-2, VIEW_X, 32+13*5-1);
		}
		glColor3dv(STAT_TEXT_COLOR);
		SPrintFloat(buf, "Mouse X= ", MOUSE_X);
		DrawText(250, 32+13*4, buf);
		SPrintFloat(buf, "Mouse Y= ", MOUSE_Y);
		DrawText(250, 32+13*3, buf);
		sprintf(buf, "ITER= %i", iter);
		DrawText(506, 32+13*4, buf);
		sprintf(buf, "LAYR= %i", layer);
		DrawText(506, 32+13*3, buf);
	}
	if(field == 0 || field == 3)
	{
		if(field == 3)
		{
			glColor3dv(STAT_BAR_COLOR);
			glRecti(100, 8-2, VIEW_X, 8+13-1);
		}
		if(QDisableRendering())
		{
			strcpy(buf, "Rendering: DONE");
		}
		else
		{
			if(!RENDERING_ENABLED)
			{
				strcpy(buf, "Rendering: PAUSED");
			}
			else if(SSAMP_SIZE > 1 && SLICE_RES == BLOCK_SIZE)
			{
				sprintf(buf,
				    "Rendering: %ix%i blocks, %ix%i supersampling...",
					SLICE_RES, SLICE_RES, SSAMP_SIZE, SSAMP_SIZE);
			}
			else
			{
				sprintf(buf, "Rendering: %ix%i blocks...",
					SLICE_RES, SLICE_RES);
			}
		}
		glColor3dv(STAT_TEXT_COLOR2);
		DrawText(100, 8, buf);
	}
	glColor3d(0.0, 0.0, 0.0);
	SetPixelCoords(ICON_BAR, STAT_BAR, WINDOW_X, WINDOW_Y, field == 0);
}

static pix_t ColorMin(pix_t p1, pix_t p2)
{
	pix_t r = _min(p1&255, p2&255); p1>>=8; p2>>=8;
	pix_t g = _min(p1&255, p2&255); p1>>=8; p2>>=8;
	pix_t b = _min(p1&255, p2&255);
	
	return r|(g<<8)|(b<<16);
}

static pix_t ColorMax(pix_t p1, pix_t p2)
{
	if(p2 == 0xFFFFFFFF) return p1;
	
	pix_t r = _max(p1&255, p2&255); p1>>=8; p2>>=8;
	pix_t g = _max(p1&255, p2&255); p1>>=8; p2>>=8;
	pix_t b = _max(p1&255, p2&255);
	
	return r|(g<<8)|(b<<16);
}

static pix_t ColorAdd(pix_t p1, pix_t p2)
{
	pix_t r = _min((p1&255)+(p2&255), 255); p1>>=8; p2>>=8;
	pix_t g = _min((p1&255)+(p2&255), 255); p1>>=8; p2>>=8;
	pix_t b = _min((p1&255)+(p2&255), 255);
	
	return r|(g<<8)|(b<<16);
}

static pix_t ColorSub(pix_t p1, pix_t p2)
{
	pix_t r = (pix_t)_max((int)((p1&255)-(p2&255)), 0); p1>>=8; p2>>=8;
	pix_t g = (pix_t)_max((int)((p1&255)-(p2&255)), 0); p1>>=8; p2>>=8;
	pix_t b = (pix_t)_max((int)((p1&255)-(p2&255)), 0);
	
	return r|(g<<8)|(b<<16);
}

static pix_t ColorMul(pix_t p1, pix_t p2)
{
	pix_t r = ((p1&255)*(p2&255))>>8; p1>>=8; p2>>=8;
	pix_t g = ((p1&255)*(p2&255))>>8; p1>>=8; p2>>=8;
	pix_t b = ((p1&255)*(p2&255))>>8;
	
	return r|(g<<8)|(b<<16);
}

static pix_t ColorAvg(pix_t p1, pix_t p2)
{
	p1 &= 0xFEFEFEFE;
	p2 &= 0xFEFEFEFE;
	return (p1+p2)>>1;
}

static pix_t Blend(pix_t *table, int index, double blend)
{
	// ***************************************
	// Performs bilinear interpolation between
	// color table entries.
	// ***************************************
	
	if(index == MAX_ITERATIONS) return table[index];
	
	pix_t rgb1 = table[index+0];
	pix_t rgb2 = table[index+1];
	double r1 = (double)(rgb1&255); rgb1>>=8;
	double g1 = (double)(rgb1&255); rgb1>>=8;
	double b1 = (double)(rgb1&255);
	double r2 = (double)(rgb2&255); rgb2>>=8;
	double g2 = (double)(rgb2&255); rgb2>>=8;
	double b2 = (double)(rgb2&255);
	
	pix_t r = (pix_t)(r1 + (r2-r1)*blend);
	pix_t g = (pix_t)(g1 + (g2-g1)*blend);
	pix_t b = (pix_t)(b1 + (b2-b1)*blend);
	if(r < 0) r = 0; else if(r > 255) r = 255;
	if(g < 0) g = 0; else if(g > 255) g = 255;
	if(b < 0) b = 0; else if(b > 255) b = 255;
	
	return r|(g<<8)|(b<<16);
}

static pix_t Iterate(double cx, double cy, int &iter)
{
	double x = cx, y = cy, dx = 0.0, dy = 0.0, s;
	pix_t auxc = 0xFFFFFFFF;
	int i, auxi = 0;
	
	if(JULIA_MODE == 1)
	{
		cx = JULIA_X;
		cy = JULIA_Y;
	}
	else if(JULIA_MODE == 2)
	{
		x = JULIA_X; // Julia/Mandelbrot hybrid fractal
		y = JULIA_Y;
	}
	for(iter = 0; iter <= ITERATIONS_PER_PIXEL; iter++)
	{
		s = x*x + y*y;
		
		if(s > 4.0 && ESCAPE1_COLOR_DEF) // escape 1
		{
			pix_t *table = ESCAPE1_COLOR_DAT.table;
			IncrementCount(iter);
			ITERATION_COUNT[3] = _min(ITERATION_COUNT[3], iter);
			
			if(ESCAPE2_COLOR_DEF >= 3 && auxc != 0xFFFFFFFF)
			{
				iter = auxi;
				return auxc;
			}
			i  = _max(0, iter-ZOOM_LIST[ZOOM_COUNT-1].iteration_bias);
			s -= 4.0;
			
			switch(ESCAPE1_COLOR_DEF)
			{
				case 1: return table[0];
				case 2: return table[i];
				case 3:
					s = 1.0 - s/ESCAPE1_BLEND_FACTOR;
					return Blend(table, i, _max(0.0, s));
				case 4:
					s *= ESCAPE1_XBAND_FACTOR;
					return table[_min((int)s, MAX_ITERATIONS)];
			}
		}
		if(iter >= ESCAPE2_MIN_ITER &&
		   iter <= ESCAPE2_MAX_ITER &&
		   ESCAPE2_COLOR_DEF)
		{
			double h;
			
			switch(ESCAPE2_EQUATION)
			{
				case 0: h = s; break; // bubbles;
				case 1: h = fabs(x*x - y*y); break; // spikes1
				case 2: h = fabs(dx*dx - dy*dy); break; // spikes2
				case 3: h = fabs(dx*x + dy*y); break; // rings
				case 4: h = fabs(dx*x - dy*y); break; // boomerangs
				case 5: h = fabs(dx*y - dy*x); break; // wires1
				case 6: h = fabs(x*y - dy*dx); break; // wires2
				case 7: h = fabs((x*x*x*x+y*y*y*y)/(x*x-y*y)); break;//clovers
				case 8: h = fabs(cos(dx)*y + sin(dy)*x); break; // spikes3
				case 9: h = fabs(fabs(x)*dx + fabs(y)*dy); break; // twists
			}
			if(h < ESCAPE2_THRESHOLD)
			{
				pix_t *table = ESCAPE2_COLOR_DAT.table;
				
				if(ESCAPE2_COLOR_DEF <= 2)
				{
					IncrementCount(iter);
				}
				h  = ESCAPE2_THRESHOLD - h;
				h *= ESCAPE2_SHADE_FACTOR;
				h /= ESCAPE2_THRESHOLD;
				
				switch(ESCAPE2_COLOR_DEF)
				{
					case 1:
						iter |= 0x80000000;
						return table[0];
					case 2:
						iter |= 0x80000000;
						return table[_min((int)h, MAX_ITERATIONS)];
					case 3:
						auxi = iter | 0x80000000;
						auxc = table[_min((int)h, MAX_ITERATIONS)];
						break;
					case 4:
						auxi = iter | 0x80000000;
						auxc = ColorMin(auxc, table[_min((int)h, MAX_ITERATIONS)]);
						break;
				}
			}
		}
		dx  = x;
		dy  = y;
		x   = cx + x*x - y*y;
		y   = cy + 2.0*dx*y;
		dx -= x;
		dy -= y;
		
		if(dx*dx + dy*dy < 1e-14 && ESCAPE2_COLOR_DEF <= 2)
		{
			iter = -1;
			break; // optimization for initial rendering
		}
	}
	IncrementCount(MAX_ITERATIONS);
	
	if(auxc != 0xFFFFFFFF     &&
	   ESCAPE1_COLOR_DEF == 0 &&
	   ESCAPE2_COLOR_DEF >= 3 )
	{
		// ***************************************
		// If we don't make this distinction, then
		// we'll get a pure black screen.
		// ***************************************
		
		iter = auxi;
		return auxc;
	}
	if(HYSTERIA_MODE)
	{
		// *************************************
		// Hysteria mode (c) James Beattie, 1999
		// *************************************		
		
		s *= HYSTERIA_FACTOR;
		
		pix_t hg = (pix_t)(255.0*_max(cos(s + 0.000000000000000), 0.0));
		pix_t hb = (pix_t)(255.0*_max(cos(s + 3.141592653589793), 0.0));
		
		iter = -1;
		return (hg<<8)|(hb<<16);
	}
	iter = -1;
	return 0;
}

static pix_t RenderPixel(int i, int j)
{
	// *********************************************
	// Compute pixel color for screen position (i,j)
	// *********************************************
	
	int iter;
	double cx = (double)(i*SLICE_RES);
	double cy = (double)(j*SLICE_RES);
	Pix2WorldCoords(cx, cy);
	
	if(SLICE_RES > BLOCK_SIZE)
	{
		return Iterate(cx, cy, iter);
	}
	else
	{
		double r_sum = 0.5; // offset for round-to-nearest quantization
		double g_sum = 0.5;
		double b_sum = 0.5;
		
		double d  = 2.0/(ZOOM_LIST[ZOOM_COUNT-1].z*(double)VIEW_Y);
		double dd = FOCUS*d/(double)SSAMP_SIZE;
		double o  = dd*(1.0-(double)SSAMP_SIZE)*0.5;
		double s  = 1.0/(double)(SSAMP_SIZE*SSAMP_SIZE);
		double x1 = cx + o;
		
		cy += o; // offset
		
		for(int k = 0, v = 0; v < SSAMP_SIZE; v++)
		{
			cx = x1; // initial worldspace x
			
			for(int u = 0; u < SSAMP_SIZE; u++, k++)
			{
				pix_t rgb = Iterate(cx, cy, iter);
				
				r_sum += s*(double)(rgb&255); rgb>>=8;
				g_sum += s*(double)(rgb&255); rgb>>=8;
				b_sum += s*(double)(rgb&255);
				
				cx += dd;
			}
			cy += dd;
		}
		pix_t r = (pix_t)r_sum;
		pix_t g = (pix_t)g_sum;
		pix_t b = (pix_t)b_sum;
		
		// ***************************
		// Clamp values to valid range
		// ***************************
		
		if(r < 0) r = 0; else if(r > 255) r = 255;
		if(g < 0) g = 0; else if(g > 255) g = 255;
		if(b < 0) b = 0; else if(b > 255) b = 255;
		
		return r|(g<<8)|(b<<16);
	}
	return 0;
}

static void RedrawXLine(int x, int y, int width)
{
	pix_t tmpbuf[MAX_WINDOW_X], *buf;
	int res, shr;
	
	// ***********************
	// Clip to window boundary
	// ***********************
	
	if(x > VIEW_X-1 || x < 1-width) return;
	if(y > VIEW_Y-1 || y < 0)       return;
	
	if(x < 0) width += x, x = 0;
	if(width > VIEW_X-x) width = VIEW_X-x;
	
	// **********************************************
	// Determine slice resolution based on y-position
	// **********************************************
	
	if(y < SLICE_HLT_POS) res = SLICE_RES;
	else res = SLICE_RES*2;
	
	// *****************************
	// Read pixels from slice buffer
	// *****************************
	
	buf = SLICE_BUF + (((y&~(res-1))+1)*2 + res)*(VIEW_X/2) - VIEW_X/res;
	shr = LOG2TABLE[res];
	
	for(int i = 0; i < width; i++)
	{
		tmpbuf[i] = buf[(x+i)>>shr];
	}
	// *************
	// Render pixels
	// *************
	
	glRasterPos2i(x, y);
	glDrawPixels(width, 1, GL_RGBA, GL_UNSIGNED_BYTE, tmpbuf);
}

static void RedrawYLine(int x, int y, int height)
{
	pix_t tmpbuf[MAX_WINDOW_Y], *buf;
	int res;
	
	// ***********************
	// Clip to window boundary
	// ***********************
	
	if(y > VIEW_Y-1 || y < 1-height) return;
	if(x > VIEW_X-1 || x < 0)        return;
	
	if(y < 0) height += y, y = 0;
	if(height > VIEW_Y - y) height = VIEW_Y - y;
	
	// *****************************
	// Read pixels from slice buffer
	// *****************************
	
	res = SLICE_RES;
	buf = SLICE_BUF + (res + 2)*(VIEW_X/2) + x/res - VIEW_X/res;
	
	for(int i = 0; i < height; i++)
	{
		if(y+i >= SLICE_HLT_POS && res == SLICE_RES)
		{
			res = SLICE_RES*2;
			buf = SLICE_BUF + (res + 2)*(VIEW_X/2) + x/res - VIEW_X/res;
		}
		tmpbuf[i] = buf[((y+i)&~(res-1))*VIEW_X];
	}
	// *************
	// Render pixels
	// *************
	
	glRasterPos2i(x, y);
	glDrawPixels(1, height, GL_RGBA, GL_UNSIGNED_BYTE, tmpbuf);
}

static void Redraw(void)
{
	// ************************************************
	// Redraw entire window, including status and view.
	// NOTE: Zoom box is also reset by this function.
	// ************************************************
	
	BOX_STATE  = 0;
	BOX_HILITE = 0;
	BOX_UPDATE = 0;
	
	memset(BOX, 0, sizeof(box_t)*2);
	
	DrawStatus(0);
	
	for(int y = 0; y < VIEW_X; y++)
	{
		RedrawXLine(0, y, VIEW_X);
	}
	DrawSliceHilite();
	
	glFlush();
	glutSwapBuffers();
}

static void EraseBox(box_t *box)
{
	// *****************************************
	// Erase old zoom box by copying pixels from
	// slice buffer info frame buffer.
	// *****************************************
	
	if(box->x1 == box->x2 && box->y1 == box->y2)
	{
		return;
	}
	int x1 = _min(box->x1, box->x2);
	int y1 = _min(box->y1, box->y2);
	int x2 = _max(box->x1, box->x2);
	int y2 = _max(box->y1, box->y2);
	
	RedrawXLine(x1,   y1,   x2 - x1);
	RedrawXLine(x1,   y2-1, x2 - x1);
	RedrawYLine(x1,   y1+1, y2 - y1-2);
	RedrawYLine(x2-1, y1+1, y2 - y1-2);
}

static void DrawBox(box_t *box)
{
	// *************
	// Draw zoom box
	// *************
	
	if(box->x1 == box->x2 && box->y1 == box->y2)
	{
		return;
	}
	int x1 = _min(box->x1, box->x2);
	int y1 = _min(box->y1, box->y2);
	int x2 = _max(box->x1, box->x2);
	int y2 = _max(box->y1, box->y2);
	
	if(BOX_HILITE) glColor3dv(BOX_HILITE_COLOR);
	else           glColor3dv(BOX_NO_HLT_COLOR);
	
	glEnable(GL_BLEND);
	glRecti(x1,   y1,   x2,   y1+1);
	glRecti(x1,   y2-1, x2,   y2);
	glRecti(x1,   y1+1, x1+1, y2-1);
	glRecti(x2-1, y1+1, x2,   y2-1);
	glDisable(GL_BLEND);
}

static int DrawSlice(void)
{
	// ***********************************************
	// Function returns 1 iff slice was actually drawn
	// ***********************************************
	
	static pix_t tmpbuf[MAX_WINDOW_X];
	int x, y = SLICE_POS;
	int no_skip = (y & 1) ||
	              (SLICE_RES == BLOCK_SIZE && SSAMP_SIZE > 1) ||
	              (SLICE_RES == MAX_SLICE_RES);
	
	// **********************************************************
	// Render pixels into SLICE_TMP_BUF; if ITERATIONS_PER_UPDATE
	// limit is reached, then draw pixels into framebuffer, else
	// return and wait for next pass.
	// **********************************************************
	
	ITERATION_COUNT[0] = 0;
	
	if(no_skip)
	{
		for(x = SLICE_TMP_COUNT; x < SLICE_X; x++)
		{
			if(ITERATION_COUNT[0] > ITERATIONS_PER_UPDATE) break;
			tmpbuf[x] = RenderPixel(x, y);
		}
	}
	else
	{
		for(x = SLICE_TMP_COUNT; x < SLICE_X; x+=2)
		{
			if(ITERATION_COUNT[0] > ITERATIONS_PER_UPDATE) break;
			tmpbuf[x>>1] = RenderPixel(x+1, y);
		}
	}
	if(x == SLICE_X)
	{
		SLICE_TMP_COUNT = 0;
	}
	else
	{
		SLICE_TMP_COUNT = x;
		return 0; // not done yet
	}
	// **********************************************************
	// ITERATIONS_PER_UPDATE limit has been reached; continue and
	// draw pixels into framebuffer.
	// **********************************************************
	
	glPixelZoom((float)SLICE_RES, (float)SLICE_RES);
	
	// ************************************
	// First, copy pixels into slice buffer
	// ************************************
	
	pix_t *buf = SLICE_BUF + (SLICE_RES +
	(y*SLICE_RES+1)*2)*(VIEW_X/2) - SLICE_X;
	
	if(no_skip)
	{
		memcpy(buf, tmpbuf, sizeof(pix_t)*SLICE_X);
	}
	else
	{
		pix_t *buf2 = buf + (SLICE_X + SLICE_RES*VIEW_X)/2;
		
		for(x = 0; x < SLICE_X; x+=2)
		{
			buf[x]   = buf2  [x>>1];
			buf[x+1] = tmpbuf[x>>1];
		}
	}
	// **********
	// Draw slice
	// **********
	
	glRasterPos2i(0, y*SLICE_RES);
	glDrawPixels(SLICE_X, 1, GL_RGBA, GL_UNSIGNED_BYTE, buf);
	glPixelZoom(1.f, 1.f);
	
	SLICE_HLT_POS = (y+1)*SLICE_RES;
	
	return 1;
}

static void DrawSliceHilite(void)
{
	// *********************
	// Draw hilited scanline
	// *********************
	
	glColor3dv(SLICE_HLT_COLOR);
	glRecti(0, SLICE_HLT_POS, VIEW_X, SLICE_HLT_POS+1);
}

static void Zoom(box_t *box)
{
	zoom_t Z = ZOOM_LIST[ZOOM_COUNT-1];
	
	// *********************************
	// Convert to normalized coordinates
	// *********************************
	
	double x1 = +(2.0*(double)box->x1/(double)VIEW_X - 1.0);
	double y1 = +(2.0*(double)box->y1/(double)VIEW_Y - 1.0);
	double x2 = +(2.0*(double)box->x2/(double)VIEW_X - 1.0);
	double y2 = +(2.0*(double)box->y2/(double)VIEW_Y - 1.0);
	x1 *= (double)VIEW_X/(double)VIEW_Y;
	x2 *= (double)VIEW_X/(double)VIEW_Y;
	if(x1 > x2) {double x3 = x1; x1 = x2; x2 = x3;}
	if(y1 > y2) {double y3 = y1; y1 = y2; y2 = y3;}
	
	// *********************
	// Create zoom transform
	// *********************
	
	double z2;
	
	if((double)VIEW_X*(y2-y1) < (double)VIEW_Y*(x2-x1))
		z2 = 2.0*((double)VIEW_X/(double)VIEW_Y)/(x2-x1); else
		z2 = 2.0/(y2-y1);
	
	// ***************************
	// Concatenate zoom transforms
	// ***************************
	
	ZOOM_LIST[ZOOM_COUNT].x = -0.5*(x1+x2)/Z.z + Z.x;
	ZOOM_LIST[ZOOM_COUNT].y = -0.5*(y1+y2)/Z.z + Z.y;
	ZOOM_LIST[ZOOM_COUNT].z = z2*Z.z;
	ZOOM_LIST[ZOOM_COUNT].iteration_bias = Z.iteration_bias;
	ZOOM_COUNT++;
	Clear();
}

static void GLUT__Display(void)
{
	// ********************************************
	// First, see if slice -really- needs rendering
	// ********************************************
	
	if(SLICE_UPDATE) SLICE_UPDATE = DrawSlice();
	
	// ********************************
	// Update zoom box and slice hilite
	// ********************************
	
	if(BOX_UPDATE || SLICE_UPDATE)
	{
		// ************************************
		// These objects are rendered together,
		// since they may be overlapping..
		// ************************************
		
		EraseBox(&BOX[1]);
		DrawBox(&BOX[0]);
		BOX[1] = BOX[0];
		DrawSliceHilite();
	}
	if(REALTIME_STAT_UPDATE)
	{
		if(SLICE_UPDATE) DrawStatus(1); // update iteration count
		if(MOUSE_UPDATE) DrawStatus(2); // update mouse position
	}
	// *****************************
	// Prepare to process next slice
	// *****************************
	
	if(SLICE_UPDATE)
	{
		if(++SLICE_POS*SLICE_RES >= VIEW_Y)
		{
			if(SLICE_RES == BLOCK_SIZE)
			{
				RENDERING_ENABLED = 0;
			}
			else
			{
				SLICE_POS  = SLICE_HLT_POS = 0;
				SLICE_RES /= 2;
				
				SLICE_X = VIEW_X/SLICE_RES;
				SLICE_Y = VIEW_Y/SLICE_RES;
				
				if(SLICE_RES == 1 && SSAMP_SIZE > 1)
				{
					// ************************************
					// Reset iteration count on final pass,
					// since we aren't recycling old pixels
					// ************************************
					
					ITERATION_COUNT[0] = 0;
					ITERATION_COUNT[1] = 0;
					ITERATION_COUNT[2] = 0;
				}
			}
			DrawStatus(1); // update iteration count
			DrawStatus(3); // update rendering state
		}
	}
	if(SLICE_UPDATE || BOX_UPDATE || MOUSE_UPDATE)
	{
		glFlush();
		glutSwapBuffers();
	}
	BOX_UPDATE   = 0;
	SLICE_UPDATE = 0;
	MOUSE_UPDATE = 0;
}

static void GLUT__Reshape(int width, int height)
{
	// **************************************************
	// Adjust window size to be multiple of MAX_SLICE_RES
	// **************************************************
	
	VIEW_X = width  - ICON_BAR;
	VIEW_Y = height - STAT_BAR;
	if(VIEW_X > MAX_WINDOW_X) VIEW_X = MAX_WINDOW_X;
	if(VIEW_Y > MAX_WINDOW_Y) VIEW_Y = MAX_WINDOW_Y;
	if(VIEW_X < MIN_WINDOW_X) VIEW_X = MIN_WINDOW_X;
	if(VIEW_Y < MIN_WINDOW_Y) VIEW_Y = MIN_WINDOW_Y;
	VIEW_X &= ~(MAX_SLICE_RES-1);
	VIEW_Y &= ~(MAX_SLICE_RES-1);
	WINDOW_X = VIEW_X + ICON_BAR;
	WINDOW_Y = VIEW_Y + STAT_BAR;
	
	if(width != WINDOW_X || height != WINDOW_Y)
	{
		glutReshapeWindow(WINDOW_X, WINDOW_Y);
	}
	// ************************
	// Set up pixel-coordinates
	// ************************
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f, (float)VIEW_X, 0.f, (float)VIEW_Y, 1.f, -1.f);
	glViewport(ICON_BAR, STAT_BAR, VIEW_X, VIEW_Y);
	
	if(APP_RESHAPE_EVENT)
	{
		Redraw(); // reshape occured via load-rendering-state
	}
	else
	{
		Clear(); // reshape occured via GLUT interface
	}
	APP_RESHAPE_EVENT = 0;
}

static void GLUT__Keyboard(unsigned char key, int x, int y)
{
	if(key == 27)  exit(0); // ESC key quits program
	if(key == 'c') Clear();
	if(key == 'd') Redraw();
	if(key == 'p')
	{
		RENDERING_ENABLED = 0;
		DrawStatus(3);  // update rendering state
		BOX_UPDATE = 1; // this forces a status update
	}
	if(key == 'r')
	{
		RENDERING_ENABLED = 1;
		DrawStatus(3);  // update rendering state
		BOX_UPDATE = 1; // this forces a status update
	}
	if(key == 8 && ZOOM_COUNT > 1) // BACKSPACE key restores prev. zoom state
	{
		ZOOM_COUNT--;
		
		if(JULIA_MODE && ZOOM_COUNT <= JULIA_ZOOM_INDEX)
		{
			JULIA_MODE = 0;
			glutSetMenu(M_ID[0]);
			glutChangeToSubMenu(1,
			"Mode: Mandelbrot Set", M_ID[1]);
		}
		Clear();
	}
	if(key == 'b')
	{
		ZOOM_LIST[ZOOM_COUNT-1].iteration_bias = ITERATION_COUNT[3];
		
		if(ESCAPE1_COLOR_DEF == 2 || ESCAPE1_COLOR_DEF == 3)
		{
			Clear();
		}
	}
	if((key == 'j' || key == 'h') && !JULIA_MODE)
	{
		y = VIEW_Y - y, x = x - ICON_BAR;
		
		// ****************************************
		// Compute world-coordinates of mouse point
		// ****************************************
		
		JULIA_X = (double)x;
		JULIA_Y = (double)y;
		Pix2WorldCoords(JULIA_X, JULIA_Y);
		
		if(key == 'j')
		{
			glutSetMenu(M_ID[0]);
			glutChangeToSubMenu(1, "Mode: Julia Set", M_ID[1]);
			JULIA_MODE = 1;
		}
		else
		{
			glutSetMenu(M_ID[0]);
			glutChangeToSubMenu(1,
			"Mode: Julia/Mandelbrot Hybrid Fractal", M_ID[1]);
			JULIA_MODE = 2;
		}
		JULIA_ZOOM_INDEX = ZOOM_COUNT;
		ZOOM_LIST[ZOOM_COUNT].x = 0.00;
		ZOOM_LIST[ZOOM_COUNT].y = 0.00;
		ZOOM_LIST[ZOOM_COUNT].z = 0.75; // default
		ZOOM_LIST[ZOOM_COUNT].iteration_bias = 0;
		ZOOM_COUNT++;
		Clear();
	}
	if(key == 'm' && JULIA_MODE)
	{
		JULIA_MODE = 0;
		ZOOM_COUNT = JULIA_ZOOM_INDEX;
		glutSetMenu(M_ID[0]);
		glutChangeToSubMenu(1,
		"Mode: Mandelbrot Set", M_ID[1]);
		Clear();
	}
}

static void GLUT__Mouse(int button, int state, int x, int y)
{
	int isdown = (state == GLUT_DOWN) ? 1 : 0;
	
	if(button != GLUT_LEFT_BUTTON) return; // not interesting
	if(isdown == (BOX_STATE & 1))  return; // not interesting
	
	y = VIEW_Y - y, x = x - ICON_BAR;
	
	switch(BOX_STATE)
	{
		case 0: // begin drag
		{
			BOX_HILITE = 0;
			BOX_STATE++;
			break;
		}
		case 1: // end drag
		{
			if(BOX[0].x1 != BOX[0].x2 || BOX[0].y1 != BOX[0].y2)
			{
				BOX_STATE++;
			}
			else
			{
				BOX_STATE--;
			}
			break;
		}
		case 2: // click in box
		{
			if(QPointInBox(&BOX[0], x, y))
			{
				BOX_HILITE = 1;
				BOX_STATE++;
			}
			else
			{
				BOX_HILITE = 0;
				BOX_STATE--;
			}
			break;
		}
		case 3: // release
		{
			if(QPointInBox(&BOX[0], x, y))
			{
				BOX_HILITE = 0;
				BOX_STATE  = 0;
				Zoom(&BOX[0]);
			}
			else
			{
				BOX_HILITE = 0;
				BOX_STATE  = 2;
			}
		}
	}
	if(BOX_STATE == 1)
	{
		if(x < 0) x = 0; else if(x > VIEW_X) x = VIEW_X;
		if(y < 0) y = 0; else if(y > VIEW_Y) y = VIEW_Y;
		
		BOX[0].x1 = BOX[0].x2 = x;
		BOX[0].y1 = BOX[0].y2 = y;
	}
	BOX_UPDATE = 1;
}

static void GLUT__Motion(int x, int y)
{
	y = VIEW_Y - y, x = x - ICON_BAR;
	
	// ************************
	// Handle zoom box dragging
	// ************************
	
	switch(BOX_STATE)
	{
		case 1:
		{
			if(x < 0) x = 0; else if(x > VIEW_X) x = VIEW_X;
			if(y < 0) y = 0; else if(y > VIEW_Y) y = VIEW_Y;
			
			BOX[0].x2 = x;
			BOX[0].y2 = y;
			if(BOX[0].x2 == BOX[0].x1) BOX[0].x2++;
			if(BOX[0].y2 == BOX[0].y1) BOX[0].y2++;
			BOX_UPDATE = 1;
			break;
		}
		case 3:
		{
			int hilite = QPointInBox(&BOX[0], x, y);
			
			if(BOX_HILITE != hilite)
			{
				BOX_HILITE = hilite;
				BOX_UPDATE = 1;
			}
		}
	}
	// ************************************************
	// Convert mouse-coords to world-coords and display
	// ************************************************
	
	if(REALTIME_STAT_UPDATE)
	{
		double mx = (double)x;
		double my = (double)y;
		Pix2WorldCoords(mx, my);
		MOUSE_X = mx;
		MOUSE_Y = my;
		MOUSE_UPDATE = 1;
		(void)Iterate(mx, my, MOUSE_ITERATIONS);
	}
}

static void GLUT__PassiveMotion(int x, int y)
{
	if(!REALTIME_STAT_UPDATE) return;
	
	// ************************************************
	// Convert mouse-coords to world-coords and display
	// ************************************************
	
	double mx = (double)(x - ICON_BAR);
	double my = (double)(VIEW_Y - y);
	Pix2WorldCoords(mx, my);
	MOUSE_X = mx;
	MOUSE_Y = my;
	MOUSE_UPDATE = 1;
	(void)Iterate(mx, my, MOUSE_ITERATIONS);
}

static void GLUT__Idle(void)
{
	if(RENDERING_ENABLED)
	{
		if(QDisableRendering())
		{
			RENDERING_ENABLED = 0; // oops!
		}
		else
		{
			SLICE_UPDATE = 1;
		}
	}
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	VIEW_X = 640, WINDOW_X = VIEW_X + ICON_BAR;
	VIEW_Y = 480, WINDOW_Y = VIEW_Y + STAT_BAR;
	
	fprintf(stdout, "Mandel: Fractal Explorer\n");
	fprintf(stdout, "(c) Bernie Freidin, 1999-2000\n\n");
	fprintf(stdout, "Please refer to 'Mandel Paper.doc' for instructions on use\n");
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_X, WINDOW_Y);
	int win_id = glutCreateWindow(argv[0]);
	glutDisplayFunc(GLUT__Display);
	glutReshapeFunc(GLUT__Reshape);
	glutKeyboardFunc(GLUT__Keyboard);
	glutMouseFunc(GLUT__Mouse);
	glutMotionFunc(GLUT__Motion);
	glutPassiveMotionFunc(GLUT__PassiveMotion);
	glutIdleFunc(GLUT__Idle);
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glBlendFunc(GL_ONE, GL_ONE); // pure additive
	
	Initialize();
	glutMainLoop();
	return 0;
}
