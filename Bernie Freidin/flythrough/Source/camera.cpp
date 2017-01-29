// **************************************************************************
// HCSG Demo
// (c) Bernie Freidin, 1999-2000
// **************************************************************************

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "camera.h"
#include "collision.h"
#ifndef WIN32
#define __MAC__ //FIXME: FOR NOW: enable __MAC__ if not WIN32
#endif
#ifdef __MAC__
#include <Events.h>
#endif

#ifndef __MAC__
#define _A_ 'A' // see keyboards mapping
#define _Z_ 'Z' // see keyboards mapping
#else
enum
{
	VK_NUMPAD9 = 0x5C,
	VK_NUMPAD8 = 0x5B,
	VK_NUMPAD7 = 0x59,
	VK_NUMPAD6 = 0x58,
	VK_NUMPAD5 = 0x57,
	VK_NUMPAD4 = 0x56,
	VK_NUMPAD3 = 0x55,
	VK_NUMPAD2 = 0x54,
	VK_NUMPAD1 = 0x53,
	VK_NUMPAD0 = 0x52,	
	_Z_        = 0x06,
	_A_        = 0x00,
	_C_        = 0x05,
	VK_SHIFT   = 0x38,
	VK_DOWN    = 0x7D,
	VK_UP      = 0x7E
};

static unsigned char MAC__keymap[16];

static int GetAsyncKeyState(int key)
{
	return((MAC__keymap[k>>3]>>(k&7))&1) ? 0x8000 : 0; 
}
#endif

#define _min(a,b) ((a)<(b)?(a):(b))
#define _max(a,b) ((a)>(b)?(a):(b))
#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

struct clamp_t
{
	double spring_center;
	double spring_linear;
	double spring_nonlinear;
	double minv;
	double maxv;
	int    minmax_clamp;
};

struct camera_t
{
	double orientation[16]; // world->local
	double position[3];     // worldspace
	double velocity[3];     // local
	double acceleration[3]; // local
	double ypr[3];
	double ypr_velocity[3];
	double ypr_accel[3];
	clamp_t clamp_position;
	clamp_t clamp_velocity;
	clamp_t clamp_ypr[3];
	clamp_t clamp_ypr_velocity[3];
	double instantaneous_accel[7]; // fwd,bck,hrz,vrt,yaw,pch,rol
};
static camera_t CAMERA;

static double Dot3(double v1[3], double v2[3])
{
	return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

static double Dot3_4(double v1[3], double v2[])
{
	return v1[0]*v2[0] + v1[1]*v2[4] + v1[2]*v2[8];
}

static void Clamp(double &value, clamp_t *clamp)
{
	// ***************************
	// Apply clamp to scalar value
	// ***************************
	
	if(clamp->minmax_clamp)
	{
		value = _max(value, clamp->minv);
		value = _min(value, clamp->maxv);
	}
	value -= clamp->spring_center;
	value *= 1.f - clamp->spring_nonlinear;
	if(value > 0.0) value = _max(value - clamp->spring_linear, 0.0);
	if(value < 0.0) value = _min(value + clamp->spring_linear, 0.0);
	value += clamp->spring_center;
}

static void Clamp3(double v[3], clamp_t *clamp)
{
	// ************************
	// Apply clamp to 3D vector
	// ************************
	
	double value = sqrt(Dot3(v, v));
	double vsave = value;
	
	if(fabs(value) < 1e-12) return;
	
	Clamp(value, clamp);
	vsave = value/vsave;
	v[0] *= vsave;
	v[1] *= vsave;
	v[2] *= vsave;
}

static void ClampTo180(double &v)
{
	if(v < -180.0) v += 360.0; else
	if(v > +180.0) v -= 360.0;
}

static void LoadScalar(double *v)
{
	// *****************************
	// Load scalar value from strtok
	// *****************************
	
	char *paramstr = strtok(NULL, " =:\t");
	if(!paramstr) return;
	*v = atof(paramstr);
}

static void LoadClampParams(clamp_t *clamp)
{
	// *********************************
	// Load clamp parameters from strtok
	// *********************************
	
	char *paramstr = strtok(NULL, " =:\t"); if(!paramstr) return;
	clamp->spring_center = atof(paramstr);
	paramstr = strtok(NULL, " =:\t"); if(!paramstr) return;
	clamp->spring_linear = atof(paramstr);
	paramstr = strtok(NULL, " =:\t"); if(!paramstr) return;
	clamp->spring_nonlinear = atof(paramstr);
	paramstr = strtok(NULL, " =:\t"); if(!paramstr) return;
	clamp->minv = atof(paramstr);
	paramstr = strtok(NULL, " =:\t"); if(!paramstr) return;
	clamp->maxv = atof(paramstr);
	paramstr = strtok(NULL, " =:\t"); if(!paramstr) return;
	clamp->minmax_clamp = atoi(paramstr);
}

static void LoadVector3D(double v[3])
{
	// ****************************
	// Load a 3D vector from strtok
	// ****************************
	
	for(int i = 0; i < 3; i++)
	{
		char *paramstr = strtok(NULL, " =:\t");
		if(!paramstr) return;
		v[i] = atof(paramstr);
	}
}

void InitializeCamera(char *path)
{
	// **************************************
	// Load default camera settings from file
	// **************************************
	
	FILE *prefs = fopen(path, "r");
	char str[200];
	
	if(!prefs)
	{
		char wait[200];
		fprintf(stdout, "Can't open \"%s\"\n", path);
		fprintf(stdout, "[Press RETURN to quit]\n");
		fgets(wait, 199, stdin);
		exit(-1);
	}
	memset(&CAMERA, 0, sizeof(camera_t));
	
	while(fgets(str, 199, prefs))
	{
		if(str[0] == '#') continue;
		if(str[0] == '/' && str[1] == '/') continue;
		
		char *vname = strtok(str, " =:\t");
		
		if(!vname) continue;
		if(!strcmp(vname, "SET_POSITION"))
			LoadVector3D(CAMERA.position); else
		if(!strcmp(vname, "CLAMP_POSITION"))
			LoadClampParams(&CAMERA.clamp_position); else
		if(!strcmp(vname, "CLAMP_VELOCITY"))
			LoadClampParams(&CAMERA.clamp_velocity); else
		if(!strcmp(vname, "CLAMP_YAW"))
			LoadClampParams(&CAMERA.clamp_ypr[0]); else
		if(!strcmp(vname, "CLAMP_PITCH"))
			LoadClampParams(&CAMERA.clamp_ypr[1]); else
		if(!strcmp(vname, "CLAMP_ROLL"))
			LoadClampParams(&CAMERA.clamp_ypr[2]); else
		if(!strcmp(vname, "CLAMP_YAW_VELOCITY"))
			LoadClampParams(&CAMERA.clamp_ypr_velocity[0]); else
		if(!strcmp(vname, "CLAMP_PITCH_VELOCITY"))
			LoadClampParams(&CAMERA.clamp_ypr_velocity[1]); else
		if(!strcmp(vname, "CLAMP_ROLL_VELOCITY"))
			LoadClampParams(&CAMERA.clamp_ypr_velocity[2]); else
		if(!strcmp(vname, "SET_FORWARD_ACCELERATION"))
			LoadScalar(&CAMERA.instantaneous_accel[0]); else
		if(!strcmp(vname, "SET_BACKWARD_ACCELERATION"))
			LoadScalar(&CAMERA.instantaneous_accel[1]); else
		if(!strcmp(vname, "SET_HORIZONTAL_ACCELERATION"))
			LoadScalar(&CAMERA.instantaneous_accel[2]); else
		if(!strcmp(vname, "SET_VERTICAL_ACCELERATION"))
			LoadScalar(&CAMERA.instantaneous_accel[3]); else
		if(!strcmp(vname, "SET_YAW_ACCELERATION"))
			LoadScalar(&CAMERA.instantaneous_accel[4]); else
		if(!strcmp(vname, "SET_PITCH_ACCELERATION"))
			LoadScalar(&CAMERA.instantaneous_accel[5]); else
		if(!strcmp(vname, "SET_ROLL_ACCELERATION"))
			LoadScalar(&CAMERA.instantaneous_accel[6]);
	}
	fclose(prefs);
}

void UpdateCamera(int frames, int collisions)
{
	// **********************************************
	// UpdateCamera() performs the following actions:
	// 
	// Set accelerations based on keyboard state
	// Compute orientation matrix from YPR
	// Compute world velocity
	// Update position based on world velocity
	// Update YPR based on rotational velocity
	// Update local velocity
	// Update rotational velocity
	// Update OpenGL modelview matrix
	// **********************************************
	
	// Set accelerations based on keyboard state
	
#ifdef __MAC__
	KeyMap km;
	GetKeys(km);
	memcpy(MAC__keymap, km, 16);
#endif
	double yaw = 0.0;
	double pch = 0.0;
	double rol = 0.0;
	double fwd = 0.0;
	double sid = 0.0;
	double upd = 0.0;
	if(GetAsyncKeyState(VK_NUMPAD4) & 0x8000) yaw += CAMERA.instantaneous_accel[4];
	if(GetAsyncKeyState(VK_NUMPAD6) & 0x8000) yaw -= CAMERA.instantaneous_accel[4];
	if(GetAsyncKeyState(_A_)        & 0x8000) pch += CAMERA.instantaneous_accel[5];
	if(GetAsyncKeyState(_Z_)        & 0x8000) pch -= CAMERA.instantaneous_accel[5];
	if(GetAsyncKeyState(VK_NUMPAD7) & 0x8000) rol += CAMERA.instantaneous_accel[6];
	if(GetAsyncKeyState(VK_NUMPAD9) & 0x8000) rol -= CAMERA.instantaneous_accel[6];
	if(GetAsyncKeyState(VK_NUMPAD5) & 0x8000) fwd -= CAMERA.instantaneous_accel[0];
	if(GetAsyncKeyState(VK_NUMPAD2) & 0x8000) fwd += CAMERA.instantaneous_accel[1];
	if(GetAsyncKeyState(VK_NUMPAD1) & 0x8000) sid -= CAMERA.instantaneous_accel[2];
	if(GetAsyncKeyState(VK_NUMPAD3) & 0x8000) sid += CAMERA.instantaneous_accel[2];
	if(GetAsyncKeyState(VK_UP)      & 0x8000) upd += CAMERA.instantaneous_accel[3];
	if(GetAsyncKeyState(VK_DOWN)    & 0x8000) upd -= CAMERA.instantaneous_accel[3];
	CAMERA.acceleration[0] = sid;
	CAMERA.acceleration[1] = upd;
	CAMERA.acceleration[2] = fwd;
	CAMERA.ypr_accel[0] = yaw;
	CAMERA.ypr_accel[1] = pch;
	CAMERA.ypr_accel[2] = rol;
	
	for(int j, i = 0; i < frames; i++)
	{		
		// Update rotational velocity
		
		for(j = 0; j < 3; j++)
		{
			CAMERA.ypr_velocity[j] += CAMERA.ypr_accel[j];
			ClampTo180(CAMERA.ypr_velocity[j]);
			Clamp(CAMERA.ypr_velocity[j], &CAMERA.clamp_ypr_velocity[j]);
		}
		// Update YPR based on rotational velocity
		
		for(j = 0; j < 3; j++)
		{
			CAMERA.ypr[j] += CAMERA.ypr_velocity[j];
			ClampTo180(CAMERA.ypr[j]);
			Clamp(CAMERA.ypr[j], &CAMERA.clamp_ypr[j]);
		}
		// Update local velocity
		
		CAMERA.velocity[0] += CAMERA.acceleration[0];
		CAMERA.velocity[1] += CAMERA.acceleration[1];
		CAMERA.velocity[2] += CAMERA.acceleration[2];
		Clamp3(CAMERA.velocity, &CAMERA.clamp_velocity);
		
		// Compute orientation matrix from YPR
		
		double cy = cos(CAMERA.ypr[0]*PI/180.0);
		double sy = sin(CAMERA.ypr[0]*PI/180.0);
		double cp = cos(CAMERA.ypr[1]*PI/180.0);
		double sp = sin(CAMERA.ypr[1]*PI/180.0);
		double cr = cos(CAMERA.ypr[2]*PI/180.0);
		double sr = sin(CAMERA.ypr[2]*PI/180.0);
		
		CAMERA.orientation[0] = sp*sr*sy + cr*cy;
		CAMERA.orientation[4] = cp*sr;
		CAMERA.orientation[8] = cy*sp*sr - cr*sy;
		CAMERA.orientation[1] = cr*sp*sy - cy*sr;
		CAMERA.orientation[5] = cp*cr;
		CAMERA.orientation[9] = cr*cy*sp + sr*sy;
		CAMERA.orientation[2] = cp*sy;
		CAMERA.orientation[6] = -sp;
		CAMERA.orientation[10]= cp*cy;
		
		// Compute impulse vector
		
		vec3 impulse;
		impulse.x = Dot3(CAMERA.velocity, CAMERA.orientation+0);
		impulse.y = Dot3(CAMERA.velocity, CAMERA.orientation+4);
		impulse.z = Dot3(CAMERA.velocity, CAMERA.orientation+8);
		vec3 position;
		position.x = CAMERA.position[0];
		position.y = CAMERA.position[1];
		position.z = CAMERA.position[2];
		
		// Collision-response physics
		//FIXME: use vec3 for camera vectors so we don't have to convert
		
		if(collisions)
		{
			CDR_Collide(position, impulse, 0.4);
		}
		else
		{
			position += impulse;
		}
		CAMERA.position[0] = position.x;
		CAMERA.position[1] = position.y;
		CAMERA.position[2] = position.z;
		Clamp3(CAMERA.position, &CAMERA.clamp_position);		
	}
	double px = Dot3_4(CAMERA.position, CAMERA.orientation+0);
	double py = Dot3_4(CAMERA.position, CAMERA.orientation+1);
	double pz = Dot3_4(CAMERA.position, CAMERA.orientation+2);
	
	CAMERA.orientation[12] = -px;
	CAMERA.orientation[13] = -py;
	CAMERA.orientation[14] = -pz;
	CAMERA.orientation[3]  = 0.0;
	CAMERA.orientation[7]  = 0.0;
	CAMERA.orientation[11] = 0.0;
	CAMERA.orientation[15] = 1.0;
	
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	double aspect = (double)vp[2] / (double)vp[3];
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspect, 0.01, 100.0);
	//glScalef(1.f, 1.f, -1.f); // compensate for -z axis
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(CAMERA.orientation);
}
