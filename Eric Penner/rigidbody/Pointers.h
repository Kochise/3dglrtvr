#pragma once

#include "smartptr.h"
#include <vector>
using namespace std;


//Engine

class CEntity;
typedef SmartPtr<CEntity> CEntityPtr;
typedef vector<CEntityPtr> CEntityPtrVector;

class CAnimatable;
typedef SmartPtr<CAnimatable> CAnimatablePtr;

class CPosition;
typedef SmartPtr<CPosition> CPositionPtr;

// Collisions

class CCollisionModel;
typedef SmartPtr<CCollisionModel> CCollisionModelPtr;

class CCollisionSphere;
typedef SmartPtr<CCollisionSphere> CCollisionSpherePtr;

class CCollisionHeightMap;
typedef SmartPtr<CCollisionHeightMap> CCollisionHeightMapPtr;

class CRenderable;
typedef SmartPtr<CRenderable> CRenderablePtr;

class CSpecularEnable;
typedef SmartPtr<CSpecularEnable> CSpecularEnablePtr;


class CMotion;
typedef SmartPtr<CMotion> CMotionPtr;

class IForce;
typedef SmartPtr<IForce> IForcePtr;
typedef vector<IForcePtr> IForcePtrVector;

class CGravitationalForce;
typedef SmartPtr<CGravitationalForce> CGravitationalForcePtr;

class CThrustForce;
typedef SmartPtr<CThrustForce> CThrustForcePtr;

class CSuspensionForce;
typedef SmartPtr<CSuspensionForce> CSuspensionForcePtr;

class CDragForce;
typedef SmartPtr<CDragForce> CDragForcePtr;

class CDragForce;
typedef SmartPtr<CDragForce> CDragForcePtr;

class Mesh;
typedef SmartPtr<Mesh> MeshPtr;

class CWeapon;
typedef SmartPtr<CWeapon>  CWeaponPtr;

class CCollisionInfo;
typedef SmartPtr<CCollisionInfo> CCollisionInfoPtr;
typedef vector<CCollisionInfo> CCollisionInfoVector;

class CHeightMap;
typedef SmartPtr<CHeightMap> CHeightMapPtr;
