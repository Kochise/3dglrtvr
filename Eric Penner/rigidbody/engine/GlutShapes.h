#pragma once

#include "Renderable.h"


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


struct CMaterial
{
	float dr,dg,db,sr,sg,sb,scoef;

	static CMaterial RandomMaterial();
};

class CMaterialRenderable : public CRenderable
{
public:
	CMaterialRenderable(CMaterial Material,CRenderablePtr pRenderable )
	{
		m_Material    = Material;
		m_pRenderable = pRenderable;
	}
	virtual ~CMaterialRenderable(void){}

	void Render();

	CMaterial      m_Material;
	CRenderablePtr m_pRenderable;
};



class CGlutCube : public CRenderable
{
public:
	CGlutCube(float fSize){m_fSize = fSize;}
	virtual ~CGlutCube(void){}

	void Render() {glutSolidCube(m_fSize);}

	float m_fSize;
};

class CGlutSphere : public CRenderable
{
public:
	CGlutSphere(float fRadius){m_fRadius = fRadius;}
	virtual ~CGlutSphere(void){}

	void Render() {glutSolidSphere(m_fRadius,20,20);}

	float m_fRadius;
};


class CSpringRenderable : public CRenderable
{
public:
	virtual ~CSpringRenderable(void){}
	CSpringRenderable(CVector3 GroundPosition,CVector3 AttachmentPosition, CEntityPtr pEntity)
	{
		m_GroundPosition     = GroundPosition;
		m_AttachmentPosition = AttachmentPosition;
		m_pEntity            = pEntity;
	}

	void Render();

	CVector3   m_GroundPosition;
	CVector3   m_AttachmentPosition; 
	CEntityPtr m_pEntity;
};





