#include <GL/glew.h>
#include <GL/glut.h>

#include "VolumeRenderer.h"


VolumeRenderer::VolumeRenderer(Volume *vol) : volume(vol)
{
	vol->getDimensions(w, h, d);
	vol->getScale(sx, sy, sz);

	rx = ry = 0;

	unsigned char *tmpVol = new unsigned char[4*w*h*d];

	for (int i=0; i<w*h*d; i++)
	{
		double r, g, b, a;

		if (vol->getCTF() != 0)
		{
			tmpVol[4*i+3] = vol->getVoxel(i);
			vol->getCTF()->GetColor(tmpVol[4*i+3], r, g, b, a);

			tmpVol[4*i+0] = 255 * r;
			tmpVol[4*i+1] = 255 * g;
			tmpVol[4*i+2] = 255 * b;
		}
		else
		{
			tmpVol[4*i+0] = 255;
			tmpVol[4*i+1] = 255;
			tmpVol[4*i+2] = 255;
			tmpVol[4*i+2] = vol->getVoxel(i);
		}

		if (vol->getOTF() != 0)
		{
			vol->getOTF()->GetColor(tmpVol[4*i+3], r, g, b, a);
			tmpVol[4*i+3] = 255 * a;
		}
	}

	glGenTextures(1, &texnum);
	glBindTexture(GL_TEXTURE_3D, texnum);

	// clamp to border so we get a black border around the volume and no parts of the volume repeated
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// enable the border
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, w, h, d, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmpVol);

	// configure texture coordinate generation
	// for view-aligned slices
	float plane_x[] = {1.0f, 0.0f, 0.0f, 0.0f};
	float plane_y[] = {0.0f, 1.0f, 0.0f, 0.0f};

	glTexGenfv(GL_S, GL_OBJECT_PLANE, plane_x);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, plane_y);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
}

VolumeRenderer::~VolumeRenderer()
{

}

void VolumeRenderer::render(int nslices)
{
	if (nslices <= 0)
		nslices = (int)((w+h+d)/3.0);

	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, texnum);

	for (int i=1; i<=nslices; i++)
	{
		double z = (i*1.0/(nslices+1)-0.5);

		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();

		glTranslatef(0.5, 0.5, 0.5);
		glScalef(sx*1.5, sy*1.5, sz*1.5);
		glRotatef(rx, 1, 0, 0);
		glRotatef(ry, 0, 1, 0);
		glTranslatef(0, 0, z);

		glMatrixMode(GL_MODELVIEW);

		glBegin(GL_QUADS);

			//glColor4f(1, 1, 1, 1.0/nslices);

		   	glVertex3f(-0.5, -0.5, z);
			glVertex3f(-0.5, 0.5, z);
			glVertex3f(0.5, 0.5, z);
			glVertex3f(0.5, -0.5, z);
	
		glEnd();
	}

	glDisable(GL_TEXTURE_3D);
}

void VolumeRenderer::rotate(double x, double y)
{
	rx += x;
	ry += y;
}


