#pragma once

#include "../smartptr.h"
#include "Vector.h"
#include "Matrix.h"

class CPosition
{
public:
	CPosition(void);
	CPosition(CVector3 Center,CMatrix Orientation)
	{
		SetCenter(Center);
		SetOrientation(Orientation);
	}

	virtual ~CPosition(void);

	CVector3& GetCenter()                        {return m_Center;}
	void	  SetCenter(CVector3 Center)         {m_Center = Center;}
	CMatrix&  GetOrientation()                   {return m_Orientation;}
	CMatrix&  GetOrientationInverse()            {return m_OrientationInverse;}
	void      SetOrientation(CMatrix Orientation)
	{
		m_Orientation = Orientation;
		m_OrientationInverse = CMatrix::Transpose(Orientation);
	}

	CMatrix  GetMatrix();
	CMatrix  GetMatrixInverse();

protected:

	CVector3 m_Center;
	CMatrix  m_Orientation;
	CMatrix  m_OrientationInverse;

};

typedef SmartPtr<CPosition> CPositionPtr;