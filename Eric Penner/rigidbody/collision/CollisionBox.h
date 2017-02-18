#pragma once

#include "CollisionModel.h"
#include "Engine/Vector.h"
#include "Sphere.h"
#include <vector>
using namespace std;

class CCollisionBox : public CCollisionModel
{
	friend class CCollisionDetector;

public:
	virtual ~CCollisionBox(void){}
	CCollisionBox(CPositionPtr pPosition,float fMass,float fWidth,float fHeight,float fDepth)
	{
		SetPosition(pPosition);
		SetDimensions(fMass,fWidth,fHeight,fDepth);
	}

	void SetDimensions(float fMass,float fWidth,float fHeight,float fDepth);
	void AddHardPoints(CVector3 p1,CVector3 p2,int nNumber);

	CSphere GetBoundingSphere()
	{
		return CSphere(m_pPosition->GetCenter(),m_BoundingSphereRadius);
	}

	float GetVolume()
	{
		return m_WidthOver2 * m_HeightOver2 * m_DepthOver2 * 8;
	}

	bool ContainsPoint(CVector3 Point)
	{
		return (fabs(Point.x) <= m_WidthOver2  &&  
		        fabs(Point.y) <= m_HeightOver2 &&
		        fabs(Point.z) <= m_DepthOver2   );
	}

protected:

	//Indexes for the different corners. Specified as a 3 bit string XYZ (Left-Right Bottom-Top Back-Front)
	static const int LBB = 0; 
	static const int LBF = 1;
	static const int LTB = 2;
	static const int LTF = 3;
	static const int RBB = 4;
	static const int RBF = 5;
	static const int RTB = 6;
	static const int RTF = 7;

	float m_WidthOver2;
	float m_HeightOver2;
	float m_DepthOver2;

	CVector3 m_Corners[8];

	float m_BoundingSphereRadius;

	vector<CVector3> m_HardPoints;
};

typedef SmartPtr<CCollisionBox> CCollisionBoxPtr;
