#pragma once
#include <d3dx9math.h>
#include "Vector.h"

//Wrapper class for D3DXMATRIX. Can be passed to all directX methods like normal.



class CMatrix : public D3DXMATRIX
{
public:
	CMatrix(){}
	CMatrix(D3DXMATRIX m)
	{
		_11 = m._11;
		_12 = m._12;
		_13 = m._13;
		_14 = m._14;
		_21 = m._21;
		_22 = m._22;
		_23 = m._23;
		_24 = m._24;
		_31 = m._31;
		_32 = m._32;
		_33 = m._33;
		_34 = m._34;
		_41 = m._41;
		_42 = m._42;
		_43 = m._43;
		_44 = m._44;
	}
	/*
	CMatrix(D3DXMATRIX m, float zero)
	{
		_11 = m._11;
		_12 = m._12;
		_13 = m._13;
		_14 = m._14;
		_21 = m._21;
		_22 = m._22;
		_23 = m._23;
		_24 = m._24;
		_31 = m._31;
		_32 = m._32;
		_33 = m._33;
		_34 = m._34;
		_41 = zero;
		_42 = zero;
		_43 = zero;
		_44 = m._44;
	}
	*/

	virtual ~CMatrix(){}




	
	static CVector3 TransformCoordinate(const CVector3& vector,const CMatrix& matrix)
	{
		CVector3 ret;
		D3DXVec3TransformCoord(&ret,&vector,&matrix);
		return ret;
	}

	static CVector3 TransformNormal(const CVector3& vector,const CMatrix& matrix)
	{
		CVector3 ret;
		D3DXVec3TransformNormal(&ret,&vector,&matrix);
		return ret;
	}

	static void TransformNormalArray(CVector3* pOutput,CVector3* pInput,CMatrix& m,int nCount)
	{
		D3DXVec3TransformNormalArray(pOutput,sizeof(CVector3),pInput,sizeof(CVector3),&m,nCount);
	}

	static void TransformCoordinateArray(CVector3* pOutput,CVector3* pInput,CMatrix& m,int nCount)
	{
		D3DXVec3TransformCoordArray(pOutput,sizeof(CVector3),pInput,sizeof(CVector3),&m,nCount);
	}




	static CMatrix Identity()          { CMatrix ret; D3DXMatrixIdentity(&ret);     return ret;}
	static CMatrix Zero()              { CMatrix ret; D3DXMatrixIdentity(&ret); ret._11=0; ret._22=0; ret._33=0; ret._44=0; return ret;}

	static CMatrix Transpose(CMatrix m){ CMatrix ret; D3DXMatrixTranspose(&ret,&m); return ret;}
	static CMatrix Inverse(CMatrix m)  { CMatrix ret; D3DXMatrixInverse(&ret,0,&m); return ret;}
	static CMatrix Scaling(CVector3 v)            {CMatrix ret; D3DXMatrixScaling(&ret,v.GetX(),v.GetY(),v.GetZ()); return ret;} 
	static CMatrix Translation(CVector3 v)     {CMatrix ret; D3DXMatrixTranslation(&ret,v.GetX(),v.GetY(),v.GetZ()); return ret;}
	static CMatrix RotationAxis(CVector3 Axis, float fAngle) {CMatrix ret; D3DXMatrixRotationAxis(&ret,&Axis,fAngle); return ret;}
	static CMatrix RotationX(float fAngle)                   {CMatrix ret; D3DXMatrixRotationX(&ret,fAngle); return ret;}
	static CMatrix RotationY(float fAngle)                   {CMatrix ret; D3DXMatrixRotationY(&ret,fAngle); return ret;}
	static CMatrix RotationZ(float fAngle)                   {CMatrix ret; D3DXMatrixRotationZ(&ret,fAngle); return ret;}
	static CMatrix LookAtRH(CVector3 Eye,CVector3 At,CVector3 Up) {CMatrix ret;D3DXMatrixLookAtRH(&ret,&Eye,&At,&Up); return ret;}
	static CMatrix LookAtLH(CVector3 Eye,CVector3 At,CVector3 Up) {CMatrix ret;D3DXMatrixLookAtLH(&ret,&Eye,&At,&Up); return ret;}
	static CMatrix PerspectiveRH(float l,float r,float b,float t,float zn,float zf) {CMatrix ret; D3DXMatrixPerspectiveOffCenterRH(&ret,l,r,b,t,zn,zf); return ret;}
	static CMatrix PerspectiveLH(float l,float r,float b,float t,float zn,float zf) {CMatrix ret; D3DXMatrixPerspectiveOffCenterLH(&ret,l,r,b,t,zn,zf); return ret;}
};