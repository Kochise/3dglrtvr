#include ".\glutshapes.h"

#include "Matrix.h"
#include "Engine.h"

#include "Random.h"

CMaterial CMaterial::RandomMaterial()
{
	CVector3 Rand(CRandom::RandomFloat(0,1),CRandom::RandomFloat(0,1),CRandom::RandomFloat(0,1));
	Rand = CVector3::Normalize(Rand);

	CMaterial Mat = {Rand.x,Rand.y,Rand.z,1,1,1,50};

	return Mat;
}


void CMaterialRenderable::Render() 
{
	GLfloat MaterialDiffuse[]   = {m_Material.dr,m_Material.dg,m_Material.db,1.0};
	GLfloat MaterialSpecular[]  = {m_Material.sr,m_Material.sg,m_Material.sb,1.0};
	GLfloat MaterialShininess[] = {m_Material.scoef};
	glMaterialfv(GL_FRONT, GL_DIFFUSE  , MaterialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR , MaterialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, MaterialShininess);

	m_pRenderable->Render();
}


void CSpringRenderable::Render()
{
	CVector3 P1 = m_GroundPosition;
	CVector3 P2 = CMatrix::TransformCoordinate(m_AttachmentPosition,m_pEntity->GetPosition()->GetMatrix());

	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glVertex3f(P1.GetX(),P1.GetY(),P1.GetZ());
		glVertex3f(P2.GetX(),P2.GetY(),P2.GetZ());
	glEnd();
	glEnable(GL_LIGHTING);
}