#pragma once

#include "../Pointers.h"
#include "vector.h"

class CRenderable
{
public:
	CRenderable(void){}
	virtual ~CRenderable(void){}

	virtual void Render(){}
};


//Build some wrapper renderable classes to change the renderstate
/*
class CSpecularEnable : public CRenderable
{
public:

	CSpecularEnable(CRenderablePtr pRenderable){m_pRenderable = pRenderable;}
	virtual ~CSpecularEnable(){}

	virtual void render();

protected:

	CRenderablePtr m_pRenderable;
};


//Build some wrapper renderable classes to change the renderstate
class CTransparentEnable : public CRenderable
{
public:

	CTransparentEnable(CRenderablePtr pRenderable){m_pRenderable = pRenderable;}
	virtual ~CTransparentEnable(){}

	virtual void render();

protected:

	CRenderablePtr m_pRenderable;
};

*/