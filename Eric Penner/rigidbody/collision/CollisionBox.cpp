#include "collisionbox.h"

void CCollisionBox::SetDimensions(float fMass,float fWidth,float fHeight,float fDepth)
{
	m_WidthOver2  = fWidth/2;   //x
	m_HeightOver2 = fHeight/2;  //y
	m_DepthOver2  = fDepth/2;   //z

	float fWidthSq  = fWidth  * fWidth;
	float fHeightSq = fHeight * fHeight;
	float fDepthSq  = fDepth  * fDepth;

	m_BoundingSphereRadius = CVector3::Length(CVector3(m_WidthOver2,m_HeightOver2,m_DepthOver2));

	if(fMass != INFINITE_MASS)
	{
		SetMass(fMass);

		CMatrix tensor = CMatrix::Identity();
		tensor._11 = (float)(1.0/12.0 * m_fMass * (fHeightSq + fDepthSq));    //x - Width
		tensor._22 = (float)(1.0/12.0 * m_fMass * (fWidthSq  + fDepthSq));    //y - Height
		tensor._33 = (float)(1.0/12.0 * m_fMass * (fWidthSq  + fHeightSq));   //z - Depth
		SetInertiaTensor(tensor);
	}

	m_Corners[LBB] = CVector3(-m_WidthOver2,-m_HeightOver2,-m_DepthOver2);
	m_Corners[LBF] = CVector3(-m_WidthOver2,-m_HeightOver2, m_DepthOver2);
	m_Corners[LTB] = CVector3(-m_WidthOver2, m_HeightOver2,-m_DepthOver2);
	m_Corners[LTF] = CVector3(-m_WidthOver2, m_HeightOver2, m_DepthOver2);
	m_Corners[RBB] = CVector3( m_WidthOver2,-m_HeightOver2,-m_DepthOver2);
	m_Corners[RBF] = CVector3( m_WidthOver2,-m_HeightOver2, m_DepthOver2);
	m_Corners[RTB] = CVector3( m_WidthOver2, m_HeightOver2,-m_DepthOver2);
	m_Corners[RTF] = CVector3( m_WidthOver2, m_HeightOver2, m_DepthOver2);

	AddHardPoints(m_Corners[LBB],m_Corners[RBB],5);
	AddHardPoints(m_Corners[LBF],m_Corners[RBF],5);
	AddHardPoints(m_Corners[LTB],m_Corners[RTB],5);
	AddHardPoints(m_Corners[LTF],m_Corners[RTF],5);

	AddHardPoints(m_Corners[LBB],m_Corners[LTB],5);
	AddHardPoints(m_Corners[LBF],m_Corners[LTF],5);
	AddHardPoints(m_Corners[RBB],m_Corners[RTB],5);
	AddHardPoints(m_Corners[RBF],m_Corners[RTF],5);	

	AddHardPoints(m_Corners[LBB],m_Corners[LBF],5);
	AddHardPoints(m_Corners[LTB],m_Corners[LTF],5);
	AddHardPoints(m_Corners[RBB],m_Corners[RBF],5);
	AddHardPoints(m_Corners[RTB],m_Corners[RTF],5);
}


void CCollisionBox::AddHardPoints(CVector3 p1,CVector3 p2,int nNumber)
{
	for(int i = 0; i <= nNumber; i++)
	{
		float fLerp = i/(float)nNumber;
		m_HardPoints.push_back(fLerp*p1 + (1-fLerp)*p2);	
	}
}