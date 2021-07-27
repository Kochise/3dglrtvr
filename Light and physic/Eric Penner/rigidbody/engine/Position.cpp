#include "position.h"


CPosition::CPosition(void)
	:m_Center(0,0,0)
{
	m_Orientation = CMatrix::Identity();
	m_OrientationInverse = CMatrix::Identity();
}


CPosition::~CPosition(void)
{
}

CMatrix CPosition::GetMatrix()
{
	return m_Orientation * CMatrix::Translation(m_Center);
}

CMatrix CPosition::GetMatrixInverse()
{
	return CMatrix::Translation(-m_Center) * CMatrix::Transpose(m_Orientation);
}