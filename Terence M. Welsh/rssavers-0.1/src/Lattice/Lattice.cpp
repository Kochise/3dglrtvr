/*
 * Copyright (C) 1999-2005  Terence M. Welsh
 *
 * This file is part of Lattice.
 *
 * Lattice is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 *
 * Lattice is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


// Lattice screensaver


#include <windows.h>
#include <stdio.h>
#include <rsWin32Saver/rsWin32Saver.h>
#include <rsText/rsText.h>
#include <math.h>
#include <time.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <regstr.h>
#include <commctrl.h>
#include <Lattice/texture.h>
#include <Lattice/camera.h>
#include <rsMath/rsMath.h>
#include <resource.h>


// Where in the registry to store user defined variables
#define PI 3.14159265359f
#define PIx2 6.28318530718f
#define D2R 0.0174532925f
#define R2D 57.2957795131f
#define NUMOBJECTS 20
#define LATSIZE 12


// Globals
LPCTSTR registryPath = ("Software\\Really Slick\\Lattice");
HGLRC hglrc;
HDC hdc;
float aspectRatio;
int readyToDraw = 0;
float frameTime = 0.0f;
unsigned int lattice[LATSIZE][LATSIZE][LATSIZE];
unsigned int list_base;
unsigned int texture_id[2];
float bPnt[10][6];  // Border points and direction vectors where camera can cross from cube to cube
float path[7][6];
int transitions[20][6] = {
        1, 2, 12, 4, 14, 8,
        0, 3, 15, 7, 7, 7,
        3, 4, 14, 0, 7, 16,
        2, 1, 15, 7, 7, 7,
        5, 10, 12, 17, 17, 17,
        4, 3, 13, 11, 9, 17,
        12, 4, 10, 17, 17, 17,
        2, 0, 14, 8, 16, 19,
        1, 3, 15, 7, 7, 7,
        4, 10, 12, 17, 17, 17,
        11, 4, 12, 17, 17, 17,
        10, 5, 15, 13, 17, 18,
        13, 10, 4, 17, 17, 17,
        12, 1, 11, 5, 6, 17,
        15, 2, 12, 0, 7, 19,
        14, 3, 1, 7, 7, 7,
        3, 1, 15, 7, 7, 7,
        5, 11, 13, 6, 9, 18,
        10, 4, 12, 17, 17, 17,
        15, 1, 3, 7, 7, 7};
int globalxyz[3];
int lastBorder;
int segments;
camera* theCamera;
// text output
rsText* textwriter;
// Parameters edited in the dialog box
int dLongitude;
int dLatitude;
int dThick;
int dDensity;
int dDepth;
int dFov;
int dPathrand;
int dSpeed;
int dTexture;
BOOL dSmooth;
BOOL dFog;


// Modulo function for picking the correct element of lattice array
int myMod(int x){
	while(x < 0)
		x += LATSIZE;
	return(x % LATSIZE);
}


// start point, start slope, end point, end slope, position (0.0 - 1.0)
// returns point somewhere along a smooth curve between the start point
// and end point
float interpolate(float a, float b, float c, float d, float where){
	float q = 2.0f * (a - c) + b + d;
	float r = 3.0f * (c - a) - 2.0f * b - d;
	return((where * where * where * q) + (where * where * r) + (where * b) + a);
}


void initTextures(){
	glGenTextures(2, texture_id);
	switch(dTexture){
	case 1:
		glBindTexture(GL_TEXTURE_2D, texture_id[0]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TEXSIZE, TEXSIZE, GL_RGB,
			GL_UNSIGNED_BYTE, indtex1);
		glBindTexture(GL_TEXTURE_2D, texture_id[1]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TEXSIZE, TEXSIZE, GL_RGB,
			GL_UNSIGNED_BYTE, indtex2);
		break;
	case 2:
		glBindTexture(GL_TEXTURE_2D, texture_id[0]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TEXSIZE, TEXSIZE, GL_RGB,
			GL_UNSIGNED_BYTE, crystex);
		break;
	case 3:
		glBindTexture(GL_TEXTURE_2D, texture_id[0]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TEXSIZE, TEXSIZE, GL_RGB,
			GL_UNSIGNED_BYTE, chrometex);
		break;
	case 4:
		glBindTexture(GL_TEXTURE_2D, texture_id[0]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TEXSIZE, TEXSIZE, GL_RGB,
			GL_UNSIGNED_BYTE, brasstex);
		break;
	case 5:
		glBindTexture(GL_TEXTURE_2D, texture_id[0]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, TEXSIZE, TEXSIZE, GL_RGBA,
			GL_UNSIGNED_BYTE, shinytex);
		break;
	case 6:
		glBindTexture(GL_TEXTURE_2D, texture_id[0]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA, TEXSIZE, TEXSIZE, GL_ALPHA, 
			GL_UNSIGNED_BYTE, ghostlytex);
		break;
	case 7:
		glBindTexture(GL_TEXTURE_2D, texture_id[0]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA, TEXSIZE, TEXSIZE, GL_ALPHA, 
			GL_UNSIGNED_BYTE, circuittex);
		break;
	case 8:
		glBindTexture(GL_TEXTURE_2D, texture_id[0]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, TEXSIZE, TEXSIZE, GL_RGBA,
			GL_UNSIGNED_BYTE, doughtex);
	}
}


void makeTorus(int smooth, int longitude, int latitude, float centerradius, float thickradius){
	int i, j;
	float r, rr;  // Radius
	float z, zz;  // Depth
	float cosa, sina;  // Longitudinal positions
	float cosn, cosnn, sinn, sinnn;  // Normals for shading
	float ncosa, nsina;  // Longitudinal positions for shading
	float u, v1, v2, ustep, vstep;
	float temp;
	float oldcosa, oldsina, oldncosa, oldnsina, oldcosn, oldcosnn, oldsinn, oldsinnn;

	// Smooth shading?
	if(smooth)
		glShadeModel(GL_SMOOTH);
	else
		glShadeModel(GL_FLAT);

	// Initialize texture stuff
	vstep = 1.0f / float(latitude);
	ustep = float(int((centerradius / thickradius) + 0.5f)) / float(longitude);
	v2 = 0.0f;

	for(i=0; i<latitude; i++){
		temp = PIx2 * float(i) / float(latitude);
		cosn = cosf(temp);
		sinn = sinf(temp);
		temp = PIx2 * float(i+1) / float(latitude);
		cosnn = cosf(temp);
		sinnn = sinf(temp);
		r = centerradius + thickradius * cosn;
		rr = centerradius + thickradius * cosnn;
		z = thickradius * sinn;
		zz = thickradius * sinnn;
		if(!smooth){  // Redefine normals for flat shaded model
			temp = PIx2 * (float(i) + 0.5f) / float(latitude);
			cosn = cosnn = cosf(temp);
			sinn = sinnn = sinf(temp);
		}
		v1 = v2;
		v2 += vstep;
		u = 0.0f;
		glBegin(GL_TRIANGLE_STRIP);
			for(j=0; j<longitude; j++){
				temp = PIx2 * float(j) / float(longitude);
				cosa = cosf(temp);
				sina = sinf(temp);
				if(smooth){
					ncosa = cosa;
					nsina = sina;
				}
				else{  // Redefine longitudinal component of normal for flat shading
					temp = PIx2 * (float(j) - 0.5f) / float(longitude);
					ncosa = cosf(temp);
					nsina = sinf(temp);
				}
				if(j==0){  // Save first values for end of circular tri-strip
					oldcosa = cosa;
					oldsina = sina;
					oldncosa = ncosa;
					oldnsina = nsina;
					oldcosn = cosn;
					oldcosnn = cosnn;
					oldsinn = sinn;
					oldsinnn = sinnn;
				}
				glNormal3f(cosnn * ncosa, cosnn * nsina, sinnn);
				glTexCoord2f(u, v2);
				glVertex3f(cosa * rr, sina * rr, zz);
				glNormal3f(cosn * ncosa, cosn * nsina, sinn);
				glTexCoord2f(u, v1);
				glVertex3f(cosa * r, sina * r, z);
				u += ustep;  // update u texture coordinate
			}
			//  Finish off circular tri-strip with saved first values
			glNormal3f(oldcosnn * oldncosa, oldcosnn * oldnsina, oldsinnn);
			glTexCoord2f(u, v2);
			glVertex3f(oldcosa * rr, oldsina * rr, zz);
			glNormal3f(oldcosn * oldncosa, oldcosn * oldnsina, oldsinn);
			glTexCoord2f(u, v1);
			glVertex3f(oldcosa * r, oldsina * r, z);
		glEnd();
	}
}


void setMaterialAttribs(){
	if(dTexture == 0 || dTexture >= 5)
		glColor3f(rsRandf(1.0f), rsRandf(1.0f), rsRandf(1.0f));
	if(dTexture == 1)
		glBindTexture(GL_TEXTURE_2D, texture_id[rsRandi(2)]);
}


//  Build the lattice display lists
void makeLatticeObjects(){
	int i, d = 0;
	float thick = float(dThick) * 0.001f;

	list_base = glGenLists(NUMOBJECTS);

	for(i=0; i<NUMOBJECTS; i++){
		glNewList(list_base + i, GL_COMPILE);
			if(dTexture >= 2)
				glBindTexture(GL_TEXTURE_2D, texture_id[0]);
			if(d < dDensity){
				glPushMatrix();
					setMaterialAttribs();
					glTranslatef(-0.25f, -0.25f, -0.25f);
					if(rsRandi(2))
						glRotatef(180.0f, 1, 0, 0);
					makeTorus(dSmooth, dLongitude, dLatitude, 0.36f - thick, thick);
				glPopMatrix();
			}
			d = (d + 37) % 100;
			if(d < dDensity){
				glPushMatrix();
					setMaterialAttribs();
					glTranslatef(0.25f, -0.25f, -0.25f);
					if(rsRandi(2))
						glRotatef(90.0f, 1, 0, 0);
					else
						glRotatef(-90.0f, 1, 0, 0);
					makeTorus(dSmooth, dLongitude, dLatitude, 0.36f - thick, thick);
				glPopMatrix();
			}
			d = (d + 37) % 100;
			if(d < dDensity){
				glPushMatrix();
					setMaterialAttribs();
					glTranslatef(0.25f, -0.25f, 0.25f);
					if(rsRandi(2))
						glRotatef(90.0f, 0, 1, 0);
					else
						glRotatef(-90.0f, 0, 1, 0);
					makeTorus(dSmooth, dLongitude, dLatitude, 0.36f - thick, thick);
				glPopMatrix();
			}
			d = (d + 37) % 100;
			if(d < dDensity){
				glPushMatrix();
					setMaterialAttribs();
					glTranslatef(0.25f, 0.25f, 0.25f);
					if(rsRandi(2))
						glRotatef(180.0f, 1, 0, 0);
					makeTorus(dSmooth, dLongitude, dLatitude, 0.36f - thick, thick);
				glPopMatrix();
			}
			d = (d + 37) % 100;
			if(d < dDensity){
				glPushMatrix();
					setMaterialAttribs();
					glTranslatef(-0.25f, 0.25f, 0.25f);
					if(rsRandi(2))
						glRotatef(90.0f, 1, 0, 0);
					else
						glRotatef(-90.0f, 1, 0, 0);
					makeTorus(dSmooth, dLongitude, dLatitude, 0.36f - thick, thick);
				glPopMatrix();
			}
			d = (d + 37) % 100;
			if(d < dDensity){
				glPushMatrix();
					setMaterialAttribs();
					glTranslatef(-0.25f, 0.25f, -0.25f);
					if(rsRandi(2))
						glRotatef(90.0f, 0, 1, 0);
					else
						glRotatef(-90.0f, 0, 1, 0);
					makeTorus(dSmooth, dLongitude, dLatitude, 0.36f - thick, thick);
				glPopMatrix();
			}
		glEndList();
		d = (d + 37) % 100;
	}
}


void reconfigure(){
	int i, j;
	int newBorder, positive;

	// End of old path = start of new path
	for(i=0; i<6; i++)
		path[0][i] = path[segments][i];

	// determine if direction of motion is positive or negative
	// update global position
	if(lastBorder < 6){
		if((path[0][3] + path[0][4] + path[0][5]) > 0.0f){
			positive = 1;
			globalxyz[lastBorder / 2] ++;
		}
		else{
			positive = 0;
			globalxyz[lastBorder / 2] --;
		}
	}
	else{
		if(path[0][3] > 0.0f){
			positive = 1;
			globalxyz[0] ++;
		}
		else{
			positive = 0;
			globalxyz[0] --;
		}
		if(path[0][4] > 0.0f)
			globalxyz[1] ++;
		else
			globalxyz[1] --;
		if(path[0][5] > 0.0f)
			globalxyz[2] ++;
		else
			globalxyz[2] --;
	}

	if(!rsRandi(11 - dPathrand)){  // Change directions
		if(!positive)
			lastBorder += 10;
		newBorder = transitions[lastBorder][rsRandi(6)];
		positive = 0;
		if(newBorder < 10)
			positive = 1;
		else
			newBorder -= 10;
		for(i=0; i<6; i++)  // set the new border point
			path[1][i] = bPnt[newBorder][i];
		if(!positive){  // flip everything if direction is negative
			if(newBorder < 6)
				path[1][newBorder/2] *= -1.0f;
			else
				for(i=0; i<3; i++)
					path[1][i] *= -1.0f;
			for(i=3; i<6; i++)	
				path[1][i] *= -1.0f;
		}
		for(i=0; i<3; i++)  // reposition the new border
			path[1][i] += globalxyz[i];
		lastBorder = newBorder;
		segments = 1;
	}
	else{  // Just keep going straight
		newBorder = lastBorder;
		for(i=0; i<6; i++)
			path[1][i] = bPnt[newBorder][i];
		i = newBorder / 2;
		if(!positive){
			if(newBorder < 6)
				path[1][i] *= -1.0f;
			else{
				path[1][0] *= -1.0f;
				path[1][1] *= -1.0f;
				path[1][2] *= -1.0f;
			}
			path[1][3] *= -1.0f;
			path[1][4] *= -1.0f;
			path[1][5] *= -1.0f;
		}
		for(j=0; j<3; j++){
			path[1][j] += globalxyz[j];
			if((newBorder < 6) && (j != 1))
				path[1][j] += rsRandf(0.15f) - 0.075f;
		}
		if(newBorder >= 6)
			path[1][0] += rsRandf(0.1f) - 0.05f;
		segments = 1;
	}
}


void draw(){
	int i, j, k;
	int indexx, indexy, indexz;
	rsVec xyz, dir, angvel, tempVec;
	static rsVec oldxyz(0.0f, 0.0f, 0.0f);
	static rsVec oldDir(0.0f, 0.0f, -1.0f);
	static rsVec oldAngvel(0.0f, 0.0f, 0.0f);
	float angle, distance;
	float rotMat[16];
	rsQuat newQuat;
	static rsQuat quat;
	static int flymode = 1;
	static float flymodeChange = 20.0f;
	static int seg = 0;  // Section of path
	static float where = 0.0f;  // Position on path
	static float rollVel = 0.0f, rollAcc = 0.0f;
	rsQuat cullQuat;
	float cullMat[16];
	float transMat[16];
	rsVec cull[5];  // storage for transformed culling vectors
	static int drawDepth = dDepth + 2;

	where += float(dSpeed) * 0.05f * frameTime;
	if(where >= 1.0f){
		where -= 1.0f;
		seg++;
	}
	if(seg >= segments){
		seg = 0;
		reconfigure();
	}

	// Calculate position
	xyz[0] = interpolate(path[seg][0], path[seg][3], path[seg+1][0],
		path[seg+1][3], where);
	xyz[1] = interpolate(path[seg][1], path[seg][4], path[seg+1][1],
		path[seg+1][4], where);
	xyz[2] = interpolate(path[seg][2], path[seg][5], path[seg+1][2],
		path[seg+1][5], where);

	// Do rotation stuff
	dir = xyz - oldxyz;  // Direction of motion
	dir.normalize();
	angvel.cross(dir, oldDir);  // Desired axis of rotation
	float dot = oldDir.dot(dir);
	if(dot < -1.0f)
		dot = -1.0f;
	if(dot > 1.0f)
		dot = 1.0f;
	angle = acosf(dot);  // Desired turn angle
	const float maxSpin(0.25f * float(dSpeed) * frameTime);
	if(angle > maxSpin)  // Cap the spin
		angle = maxSpin;
	if(angle < -maxSpin)
		angle = -maxSpin;
	angvel.scale(angle);  // Desired angular velocity
	tempVec = angvel - oldAngvel;  // Change in angular velocity
	distance = tempVec.length();  // Don't let angular velocity change too much
	const float rotationInertia(0.007f * float(dSpeed) * frameTime);
	if(distance > rotationInertia){
		tempVec.scale((rotationInertia) / distance);
		angvel = oldAngvel + tempVec;
	}
	// transition from one fly mode to the other?
	flymodeChange -= frameTime;
	if(flymodeChange <= 1.0f)  // prepare to transition
		angvel.scale(flymodeChange);
	if(flymodeChange <= 0.0f){  // transition from one fly mode to the other?
		flymode = rsRandi(4);
		flymodeChange = rsRandf(float(150 - dSpeed)) + 5.0f;
	}
	// Recompute desired rotation
	tempVec = angvel;
	angle = tempVec.normalize();
	newQuat.make(angle, tempVec[0], tempVec[1], tempVec[2]);  // Use rotation
	if(flymode)  // fly normal (straight)
		quat.preMult(newQuat);
	else  // don't fly normal (go backwards and stuff)
		quat.postMult(newQuat);

	// Roll
	static float rollChange = rsRandf(10.0f) + 2.0f;
	rollChange -= frameTime;
	if(rollChange <= 0.0f){
		rollAcc = rsRandf(0.02f * float(dSpeed)) - (0.01f * float(dSpeed));
		rollChange = rsRandf(10.0f) + 2.0f;
	}
	rollVel += rollAcc * frameTime;
	if(rollVel > (0.04f * float(dSpeed)) && rollAcc > 0.0f)
		rollAcc = 0.0f;
	if(rollVel < (-0.04f * float(dSpeed)) && rollAcc < 0.0f)
		rollAcc = 0.0f;
	newQuat.make(rollVel * frameTime, oldDir[0], oldDir[1], oldDir[2]);
	quat.preMult(newQuat);

	quat.toMat(rotMat);

	// Save old stuff
	oldxyz = xyz;
	oldDir[0] = -rotMat[2];
	oldDir[1] = -rotMat[6];
	oldDir[2] = -rotMat[10];
	oldAngvel = angvel;

	// Apply transformations
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(rotMat);
	glGetFloatv(GL_MODELVIEW, cullMat);
	glTranslatef(-xyz[0], -xyz[1], -xyz[2]);
	glGetFloatv(GL_MODELVIEW, transMat);

	// Just in case display lists contain no colors
	glColor3f(1.0f, 1.0f, 1.0f);

	// Environment mapping for crystal, chrome, brass, shiny, and ghostly
	if(dTexture == 2 || dTexture == 3 || dTexture == 4  || dTexture == 5 || dTexture == 6){
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}

	// Render everything
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for(i=globalxyz[0]-drawDepth; i<=globalxyz[0]+drawDepth; i++){
		for(j=globalxyz[1]-drawDepth; j<=globalxyz[1]+drawDepth; j++){
			for(k=globalxyz[2]-drawDepth; k<=globalxyz[2]+drawDepth; k++){
				tempVec[0] = float(i) - xyz[0];
				tempVec[1] = float(j) - xyz[1];
				tempVec[2] = float(k) - xyz[2];
				float tpos[3];  // transformed position
				tpos[0] = tempVec[0]*rotMat[0] + tempVec[1]*rotMat[4] + tempVec[2]*rotMat[8];
				tpos[1] = tempVec[0]*rotMat[1] + tempVec[1]*rotMat[5] + tempVec[2]*rotMat[9];
				tpos[2] = tempVec[0]*rotMat[2] + tempVec[1]*rotMat[6] + tempVec[2]*rotMat[10];
				if(theCamera->inViewVolume(tpos, 0.9f)){
					indexx = myMod(i);
					indexy = myMod(j);
					indexz = myMod(k);
					// draw it
					glPushMatrix();
						glTranslatef(float(i), float(j), float(k));
						glCallList(lattice[indexx][indexy][indexz]);
					glPopMatrix();
				}
			}
		}
	}

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	// print text
	static float totalTime = 0.0f;
	totalTime += frameTime;
	static std::string str;
	static int frames = 0;
	++frames;
	if(frames == 20){
		str = "FPS = " + to_string(20.0f / totalTime);
		totalTime = 0.0f;
		frames = 0;
	}
	if(kStatistics){
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0f, 50.0f * aspectRatio, 0.0f, 50.0f, -1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(1.0f, 48.0f, 0.0f);

		glColor3f(1.0f, 0.6f, 0.0f);
		textwriter->draw(str);

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}

    wglSwapLayerBuffers(hdc, WGL_SWAP_MAIN_PLANE);
}


void IdleProc(){
	// update time
	static rsTimer timer;
	frameTime = timer.tick();

	if(readyToDraw && !isSuspended && !checkingPassword)
		draw();
}


void initSaver(HWND hwnd){
	RECT rect;
	int i, j, k;
	int left, right, top, bottom;

	srand((unsigned)time(NULL));

	// Window initialization
	hdc = GetDC(hwnd);
	SetBestPixelFormat(hdc);
	hglrc = wglCreateContext(hdc);
	GetClientRect(hwnd, &rect);
	wglMakeCurrent(hdc, hglrc);
	left = rect.left;
	right = rect.right - rect.left;
	bottom = rect.top;
	top = rect.bottom - rect.top;
	glViewport(left, bottom, right, top);

	if(dTexture == 9)  // Choose random texture
		dTexture = rsRandi(9);

	if(dTexture != 2 && dTexture != 6)  // No z-buffering for crystal or ghostly
		glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	aspectRatio = float(rect.right) / float(rect.bottom);
	float mat[16] = {cosf(float(dFov) * 0.5f * D2R) / sinf(float(dFov) * 0.5f * D2R), 0.0f, 0.0f, 0.0f,
		0.0f, 0.0, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f - 0.02f / float(dDepth), -1.0f,
		0.0f, 0.0f, -(0.02f + 0.0002f / float(dDepth)), 0.0f};
	mat[5] = mat[0] * (float(right) / float(top));
	glLoadMatrixf(mat);
	theCamera = new camera;
	theCamera->init(mat, float(dDepth));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(dTexture != 3 && dTexture != 4 && dTexture != 6){  // No lighting for chrome, brass, or ghostly
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		float ambient[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		float diffuse[4] = {1.0f, 1.0f, 1.0f, 0.0f};
		float specular[4] = {1.0f, 1.0f, 1.0f, 0.0f};
		float position[4] = {400.0f, 300.0f, 500.0f, 0.0f};
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
	}
	glEnable(GL_COLOR_MATERIAL);
	if(dTexture == 0 || dTexture == 5 || dTexture >= 7){
		glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);
		glColorMaterial(GL_FRONT, GL_SPECULAR);
	}
	if(dTexture == 2){
		glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);
		glColorMaterial(GL_FRONT, GL_SPECULAR);
	}
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	if(dFog){
		glEnable(GL_FOG);
		float fog_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		glFogfv(GL_FOG_COLOR, fog_color);
		glFogf(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_START, float(dDepth) * 0.3f);
		glFogf(GL_FOG_END, float(dDepth) - 0.1f);
	}

	if(dTexture == 2 || dTexture == 6){  // Use blending for crystal and ghostly
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_BLEND);
	}

	if(dTexture == 7){  // Use blending for circuits
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}

	if(dTexture){
		glEnable(GL_TEXTURE_2D);
		initTextures();
	}

	// Initialize lattice objects and their positions in the lattice array
	makeLatticeObjects();
	for(i=0; i<LATSIZE; i++){
		for(j=0; j<LATSIZE; j++){
			for(k=0; k<LATSIZE; k++){
				lattice[i][j][k] = list_base + rsRandi(NUMOBJECTS);
			}
		}
	}

	// Initialize border points
	// horizontal border points
	for(i=0; i<6; i++){
		for(j=0; j<6; j++){
			bPnt[i][j] = 0.0f;
		}
	}
	bPnt[0][0] = 0.5f;    bPnt[0][1] = -0.25f;  bPnt[0][2] = 0.25f;  bPnt[0][3] = 1.0f; // right
	bPnt[1][0] = 0.5f;    bPnt[1][1] = 0.25f;   bPnt[1][2] = -0.25f; bPnt[1][3] = 1.0f; // right
	bPnt[2][0] = -0.25f;  bPnt[2][1] = 0.5f;    bPnt[2][2] = 0.25f;  bPnt[2][4] = 1.0f; // top
	bPnt[3][0] = 0.25f;   bPnt[3][1] = 0.5f;    bPnt[3][2] = -0.25f; bPnt[3][4] = 1.0f; // top
	bPnt[4][0] = -0.25f;  bPnt[4][1] = -0.25f;  bPnt[4][2] = 0.5f;   bPnt[4][5] = 1.0f; // front
	bPnt[5][0] = 0.25f;   bPnt[5][1] = 0.25f;   bPnt[5][2] = 0.5f;   bPnt[5][5] = 1.0f; // front
	// diagonal border points
	bPnt[6][0] = 0.5f;    bPnt[6][1] = -0.5f;   bPnt[6][2] = -0.5f;  bPnt[6][3] = 1.0f; bPnt[6][4] = -1.0f; bPnt[6][5] = -1.0f;
	bPnt[7][0] = 0.5f;    bPnt[7][1] = 0.5f;    bPnt[7][2] = -0.5f;  bPnt[7][3] = 1.0f; bPnt[7][4] = 1.0f;  bPnt[7][5] = -1.0f;
	bPnt[8][0] = 0.5f;    bPnt[8][1] = -0.5f;   bPnt[8][2] = 0.5f;   bPnt[8][3] = 1.0f; bPnt[8][4] = -1.0f; bPnt[8][5] = 1.0f;
	bPnt[9][0] = 0.5f;    bPnt[9][1] = 0.5f;    bPnt[9][2] = 0.5f;   bPnt[9][3] = 1.0f; bPnt[9][4] = 1.0f;  bPnt[9][5] = 1.0f;

	globalxyz[0] = 0;
	globalxyz[1] = 0;
	globalxyz[2] = 0;

	// Set up first path section
	path[0][0] = 0.0f;
	path[0][1] = 0.0f;
	path[0][2] = 0.0f;
	path[0][3] = 0.0f;
	path[0][4] = 0.0f;
	path[0][5] = 0.0f;
	j = rsRandi(12);
	k = j % 6;
	for(i=0; i<6; i++)
		path[1][i] = bPnt[k][i];
	if(j > 5){  // If we want to head in a negative direction
		i = k / 2;  // then we need to flip along the appropriate axis
		path[1][i] *= -1.0f;
		path[1][i+3] *= -1.0f;
	}
	lastBorder = k;
	segments = 1;

	// Initialize text
	textwriter = new rsText;
}


void cleanUp(HWND hwnd){
	// Kill device context
	ReleaseDC(hwnd, hdc);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
}


void setDefaults(int which){
	switch(which){
	case DEFAULTS1:  // Regular
		dLongitude = 16;
		dLatitude = 8;
		dThick = 50;
		dDensity = 50;
		dDepth = 5;
		dFov = 90;
		dPathrand = 7;
		dSpeed = 10;
		dTexture = 0;
		dSmooth = FALSE;
		dFog = TRUE;
		break;
	case DEFAULTS2:  // Chainmail
		dLongitude = 24;
		dLatitude = 12;
		dThick = 50;
		dDensity = 80;
		dDepth = 4;
		dFov = 90;
		dPathrand = 7;
		dSpeed = 10;
		dTexture = 3;
		dSmooth = TRUE;
		dFog = TRUE;
		break;
	case DEFAULTS3:  // Brass Mesh
		dLongitude = 4;
		dLatitude = 4;
		dThick = 40;
		dDensity = 50;
		dDepth = 5;
		dFov = 90;
		dPathrand = 7;
		dSpeed = 10;
		dTexture = 4;
		dSmooth = FALSE;
		dFog = TRUE;
		break;
	case DEFAULTS4:  // Computer
		dLongitude = 4;
		dLatitude = 6;
		dThick = 70;
		dDensity = 90;
		dDepth = 4;
		dFov = 90;
		dPathrand = 7;
		dSpeed = 10;
		dTexture = 7;
		dSmooth = FALSE;
		dFog = TRUE;
		break;
	case DEFAULTS5:  // Slick
		dLongitude = 24;
		dLatitude = 12;
		dThick = 100;
		dDensity = 30;
		dDepth = 5;
		dFov = 90;
		dPathrand = 7;
		dSpeed = 10;
		dTexture = 5;
		dSmooth = TRUE;
		dFog = TRUE;
		break;
	case DEFAULTS6:  // Tasty
		dLongitude = 24;
		dLatitude = 12;
		dThick = 100;
		dDensity = 25;
		dDepth = 5;
		dFov = 90;
		dPathrand = 7;
		dSpeed = 10;
		dTexture = 8;
		dSmooth = TRUE;
		dFog = TRUE;
	}
}


// Initialize all user-defined stuff
void readRegistry(){
	LONG result;
	HKEY skey;
	DWORD valtype, valsize, val;

	setDefaults(DEFAULTS1);

	result = RegOpenKeyEx(HKEY_CURRENT_USER, registryPath, 0, KEY_READ, &skey);
	if(result != ERROR_SUCCESS)
		return;

	valsize=sizeof(val);

	result = RegQueryValueEx(skey, "Longitude", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dLongitude = val;
	result = RegQueryValueEx(skey, "Latitude", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dLatitude = val;
	result = RegQueryValueEx(skey, "Thick", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dThick = val;
	result = RegQueryValueEx(skey, "Density", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dDensity = val;
	result = RegQueryValueEx(skey, "Depth", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dDepth = val;
	result = RegQueryValueEx(skey, "Fov", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dFov = val;
	result = RegQueryValueEx(skey, "Pathrand", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dPathrand = val;
	result = RegQueryValueEx(skey, "Speed", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dSpeed = val;
	result = RegQueryValueEx(skey, "Texture", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dTexture = val;
	result = RegQueryValueEx(skey, "Smooth", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dSmooth = val;
	result = RegQueryValueEx(skey, "Fog", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dFog = val;
	result = RegQueryValueEx(skey, "FrameRateLimit", 0, &valtype, (LPBYTE)&val, &valsize);
	if(result == ERROR_SUCCESS)
		dFrameRateLimit = val;

	RegCloseKey(skey);
}


// Save all user-defined stuff
void writeRegistry(){
    LONG result;
	HKEY skey;
	DWORD val, disp;

	result = RegCreateKeyEx(HKEY_CURRENT_USER, registryPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &skey, &disp);
	if(result != ERROR_SUCCESS)
		return;

	val = dLongitude;
	RegSetValueEx(skey, "Longitude", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dLatitude;
	RegSetValueEx(skey, "Latitude", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dThick;
	RegSetValueEx(skey, "Thick", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dDensity;
	RegSetValueEx(skey, "Density", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dDepth;
	RegSetValueEx(skey, "Depth", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dFov;
	RegSetValueEx(skey, "Fov", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dPathrand;
	RegSetValueEx(skey, "Pathrand", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dSpeed;
	RegSetValueEx(skey, "Speed", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dTexture;
	RegSetValueEx(skey, "Texture", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dSmooth;
	RegSetValueEx(skey, "Smooth", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dFog;
	RegSetValueEx(skey, "Fog", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));
	val = dFrameRateLimit;
	RegSetValueEx(skey, "FrameRateLimit", 0, REG_DWORD, (CONST BYTE*)&val, sizeof(val));

	RegCloseKey(skey);
}


BOOL aboutProc(HWND hdlg, UINT msg, WPARAM wpm, LPARAM lpm){
	switch(msg){
	case WM_CTLCOLORSTATIC:
		if(HWND(lpm) == GetDlgItem(hdlg, WEBPAGE)){
			SetTextColor(HDC(wpm), RGB(0,0,255));
			SetBkColor(HDC(wpm), COLORREF(GetSysColor(COLOR_3DFACE)));
			return(int(GetSysColorBrush(COLOR_3DFACE)));
		}
		break;
    case WM_COMMAND:
		switch(LOWORD(wpm)){
		case IDOK:
		case IDCANCEL:
			EndDialog(hdlg, LOWORD(wpm));
			break;
		case WEBPAGE:
			ShellExecute(NULL, "open", "http://www.reallyslick.com", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	return FALSE;
}


void initControls(HWND hdlg){
	char cval[16];

	SendDlgItemMessage(hdlg, LONGITUDE, UDM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(100), DWORD(4))));
	SendDlgItemMessage(hdlg, LONGITUDE, UDM_SETPOS, 0, LPARAM(dLongitude));

	SendDlgItemMessage(hdlg, LATITUDE, UDM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(100), DWORD(2))));
	SendDlgItemMessage(hdlg, LATITUDE, UDM_SETPOS, 0, LPARAM(dLatitude));

	SendDlgItemMessage(hdlg, THICK, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(100))));
	SendDlgItemMessage(hdlg, THICK, TBM_SETPOS, 1, LPARAM(dThick));
	SendDlgItemMessage(hdlg, THICK, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, THICK, TBM_SETPAGESIZE, 0, LPARAM(10));
	sprintf(cval, "%d", dThick);
	SendDlgItemMessage(hdlg, THICKTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, DENSITY, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(100))));
	SendDlgItemMessage(hdlg, DENSITY, TBM_SETPOS, 1, LPARAM(dDensity));
	SendDlgItemMessage(hdlg, DENSITY, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, DENSITY, TBM_SETPAGESIZE, 0, LPARAM(10));
	sprintf(cval, "%d %%", dDensity);
	SendDlgItemMessage(hdlg, DENSITYTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, DEPTH, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(LATSIZE-2))));
	SendDlgItemMessage(hdlg, DEPTH, TBM_SETPOS, 1, LPARAM(dDepth));
	SendDlgItemMessage(hdlg, DEPTH, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, DEPTH, TBM_SETPAGESIZE, 0, LPARAM(2));
	sprintf(cval, "%d", dDepth);
	SendDlgItemMessage(hdlg, DEPTHTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, FOV, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(10), DWORD(150))));
	SendDlgItemMessage(hdlg, FOV, TBM_SETPOS, 1, LPARAM(dFov));
	SendDlgItemMessage(hdlg, FOV, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, FOV, TBM_SETPAGESIZE, 0, LPARAM(10));
	sprintf(cval, "%d degrees", dFov);
	SendDlgItemMessage(hdlg, FOVTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, PATHRAND, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(10))));
	SendDlgItemMessage(hdlg, PATHRAND, TBM_SETPOS, 1, LPARAM(dPathrand));
	SendDlgItemMessage(hdlg, PATHRAND, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, PATHRAND, TBM_SETPAGESIZE, 0, LPARAM(2));
	sprintf(cval, "%d", dPathrand);
	SendDlgItemMessage(hdlg, PATHRANDTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, SPEED, TBM_SETRANGE, 0, LPARAM(MAKELONG(DWORD(1), DWORD(100))));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETPOS, 1, LPARAM(dSpeed));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETLINESIZE, 0, LPARAM(1));
	SendDlgItemMessage(hdlg, SPEED, TBM_SETPAGESIZE, 0, LPARAM(10));
	sprintf(cval, "%d", dSpeed);
	SendDlgItemMessage(hdlg, SPEEDTEXT, WM_SETTEXT, 0, LPARAM(cval));

	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(9), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(8), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(7), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(6), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(5), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(4), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(3), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(2), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(1), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_DELETESTRING, WPARAM(0), 0);
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("none"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("Industrial"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("Crystal"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("Chrome"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("Brass"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("Shiny"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("Ghostly"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("Circuits"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("Doughnuts"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_ADDSTRING, 0, LPARAM("random"));
	SendDlgItemMessage(hdlg, TEXTURE, CB_SETCURSEL, WPARAM(dTexture), 0);

	CheckDlgButton(hdlg, SMOOTH, dSmooth);

	CheckDlgButton(hdlg, FOG, dFog);

	initFrameRateLimitSlider(hdlg, FRAMERATELIMIT, FRAMERATELIMITTEXT);
}


// Prototype in rsWin32Saver.h
BOOL ScreenSaverConfigureDialog(HWND hdlg, UINT msg,
										 WPARAM wpm, LPARAM lpm){
	int ival;
	char cval[16];

	switch(msg){
    case WM_INITDIALOG:
        InitCommonControls();
        readRegistry();
        initControls(hdlg);
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wpm)){
        case IDOK:
            dLongitude = SendDlgItemMessage(hdlg, LONGITUDE, UDM_GETPOS, 0, 0);
			dLatitude = SendDlgItemMessage(hdlg, LATITUDE, UDM_GETPOS, 0, 0);
			dThick = SendDlgItemMessage(hdlg, THICK, TBM_GETPOS, 0, 0);
			dDensity = SendDlgItemMessage(hdlg, DENSITY, TBM_GETPOS, 0, 0);
			dDepth = SendDlgItemMessage(hdlg, DEPTH, TBM_GETPOS, 0, 0);
			dFov = SendDlgItemMessage(hdlg, FOV, TBM_GETPOS, 0, 0);
			dPathrand = SendDlgItemMessage(hdlg, PATHRAND, TBM_GETPOS, 0, 0);
			dSpeed = SendDlgItemMessage(hdlg, SPEED, TBM_GETPOS, 0, 0);
			dTexture = SendDlgItemMessage(hdlg, TEXTURE, CB_GETCURSEL, 0, 0);
			dSmooth = (IsDlgButtonChecked(hdlg, SMOOTH) == BST_CHECKED);
			dFog = (IsDlgButtonChecked(hdlg, FOG) == BST_CHECKED);
			dFrameRateLimit = SendDlgItemMessage(hdlg, FRAMERATELIMIT, TBM_GETPOS, 0, 0);
			writeRegistry();
            // Fall through
        case IDCANCEL:
            EndDialog(hdlg, LOWORD(wpm));
            break;
		case DEFAULTS1:
			setDefaults(DEFAULTS1);
			initControls(hdlg);
			break;
		case DEFAULTS2:
			setDefaults(DEFAULTS2);
			initControls(hdlg);
			break;
		case DEFAULTS3:
			setDefaults(DEFAULTS3);
			initControls(hdlg);
			break;
		case DEFAULTS4:
			setDefaults(DEFAULTS4);
			initControls(hdlg);
			break;
		case DEFAULTS5:
			setDefaults(DEFAULTS5);
			initControls(hdlg);
			break;
		case DEFAULTS6:
			setDefaults(DEFAULTS6);
			initControls(hdlg);
			break;
		case ABOUT:
			DialogBox(mainInstance, MAKEINTRESOURCE(DLG_ABOUT), hdlg, DLGPROC(aboutProc));
		}
		return TRUE;
	case WM_HSCROLL:
		if(HWND(lpm) == GetDlgItem(hdlg, THICK)){
			ival = SendDlgItemMessage(hdlg, THICK, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, THICKTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, DENSITY)){
			ival = SendDlgItemMessage(hdlg, DENSITY, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d %%", ival);
			SendDlgItemMessage(hdlg, DENSITYTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, DEPTH)){
			ival = SendDlgItemMessage(hdlg, DEPTH, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, DEPTHTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, FOV)){
			ival = SendDlgItemMessage(hdlg, FOV, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d degrees", ival);
			SendDlgItemMessage(hdlg, FOVTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, PATHRAND)){
			ival = SendDlgItemMessage(hdlg, PATHRAND, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, PATHRANDTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, SPEED)){
			ival = SendDlgItemMessage(hdlg, SPEED, TBM_GETPOS, 0, 0);
			sprintf(cval, "%d", ival);
			SendDlgItemMessage(hdlg, SPEEDTEXT, WM_SETTEXT, 0, LPARAM(cval));
		}
		if(HWND(lpm) == GetDlgItem(hdlg, FRAMERATELIMIT))
			updateFrameRateLimitSlider(hdlg, FRAMERATELIMIT, FRAMERATELIMITTEXT);
		return TRUE;
	}
    return FALSE;
}


LONG ScreenSaverProc(HWND hwnd, UINT msg, WPARAM wpm, LPARAM lpm){
	switch(msg){
	case WM_CREATE:
		readRegistry();
		initSaver(hwnd);
		readyToDraw = 1;
		break;
	case WM_DESTROY:
		readyToDraw = 0;
		cleanUp(hwnd);
		break;
	}
	return DefScreenSaverProc(hwnd, msg, wpm, lpm);
}