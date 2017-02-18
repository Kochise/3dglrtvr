#pragma once
#include <d3dx9math.h>

//Wrapper class for D3DXMATRIX. Can be passed to all directX methods like normal.

class CVector3 : public D3DXVECTOR3
{
public:
	virtual ~CVector3(void){}
	CVector3(void){this->x = 0; this->y = 0; this->z = 0;}
	CVector3(float fX,float fY,float fZ=0.0f){x = fX; y = fY; z = fZ;}
	CVector3(D3DXVECTOR3 v){x = v.x; y = v.y; z = v.z;}

	float GetX() {return x;}
	float GetY() {return y;}
	float GetZ() {return z;}

	void SetX(float nX) {x = nX;}
	void SetY(float nY) {y = nY;}
	void SetZ(float nZ) {z = nZ;}
	void Set(float nX, float nY, float nZ) {x = nX;y = nY;z = nZ;}

	static CVector3 Normalize(const CVector3& vector)
	{
		float fLength = D3DXVec3Length(&vector);
		if(fLength > 0)
			return vector * (1/fLength);
		else
			return vector;
	}

	static float Length(const CVector3& vector)
	{
		return D3DXVec3Length(&vector);
	}

	static float LengthSq(const CVector3& vector)
	{
		return D3DXVec3LengthSq(&vector);
	}

	static CVector3 Cross(const CVector3& v1,const CVector3& v2)
	{
		CVector3 ret;
		D3DXVec3Cross(&ret,&v1,&v2);
		return ret;
	}

	static float Dot(const CVector3& v1,const CVector3& v2)
	{
		return D3DXVec3Dot(&v1,&v2);
	}

	void Normalize()
	{
		float fLength = Length();
		if(fLength > 0)
			(*this) = (*this) * (1/fLength);
		else
			(*this) = (*this);
	}

	float Length()
	{
		return D3DXVec3Length(this);
	}

	float LengthSq()
	{
		return D3DXVec3LengthSq(this);
	}



};
