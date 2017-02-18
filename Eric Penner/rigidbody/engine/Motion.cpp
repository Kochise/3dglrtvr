#include "motion.h"

CMotion::CMotion(void)
	:m_Velocity(0,0,0)
	,m_AngularVelocity(0,0,0)
	,m_RotationAxis(0,0,0)
	,m_RotationRate(0)
{
}

CMotion::~CMotion(void)
{
}
