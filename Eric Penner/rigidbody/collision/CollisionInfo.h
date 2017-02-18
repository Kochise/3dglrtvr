#pragma once

#include "engine/entity.h"
#include <vector>
using namespace std;

class CCollisionInfo
{
public:
	virtual ~CCollisionInfo(void){}
	CCollisionInfo(void){}

	CEntityPtr pEntity1,pEntity2;
	CVector3 Point;
	CVector3 Normal;   

	float fRestitution;   // 0 = Perfectly inelastic, 1 = Perfectly elastic;
	float fFriction;      // 0 = no friction. You kind of need to tweak this
};

typedef SmartPtr<CCollisionInfo> CCollisionInfoPtr;
typedef vector<CCollisionInfo> CCollisionInfoVector;
