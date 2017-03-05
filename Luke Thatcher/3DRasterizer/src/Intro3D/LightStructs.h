
#pragma once

#include <math.h>
#include "Vector3.h"
#include "Vector4.h"

struct Attenuation
{
public:
	Attenuation(void)
	{
		this->constAtten = 0.0f;
		this->linearAtten = 0.0f;
		this->quadAtten = 0.0f;
	}
	Attenuation(float constAtten, float linearAtten, float quadAtten)
	{
		this->constAtten = constAtten;
		this->linearAtten = linearAtten;
		this->quadAtten = quadAtten;
	}

	inline float GetConstAtten(void) const
	{
		return this->constAtten;
	}
	inline float GetLinearAtten(void) const
	{
		return this->linearAtten;
	}
	inline float GetQuadAtten(void) const
	{
		return this->quadAtten;
	}

	inline void SetConstAtten(float val)
	{
		this->constAtten = val;
	}
	inline void SetLinearAtten(float val)
	{
		this->linearAtten = val;
	}
	inline void SetQuadAtten(float val)
	{
		this->quadAtten = val;
	}

	inline float CalculateAttenuation(float distance) const
	{
		return 1.0f / (constAtten + (distance * linearAtten) + (distance * distance * quadAtten));
	}

protected:
	float constAtten;
	float linearAtten;
	float quadAtten;
};
__declspec(align(16)) struct Direction
{
public:
	Direction(void)
	{
	}
	Direction(const Vector3& direction)
	{
		this->direction = direction;
	}
	inline const Vector3& GetDirection(void) const
	{
		return this->direction;
	}
	inline void SetDirection(const Vector3& rotation)
	{
		this->direction = direction;
	}

protected:
	Vector3 direction;
};
__declspec(align(16)) struct Position
{
public:
	Position(void)
	{
	}
	Position(const Vector3& position)
	{
		this->position = position;
	}
	inline const Vector3& GetPosition(void) const
	{
		return this->position;
	}
	inline void SetPosition(const Vector3& position)
	{
		this->position = position;
	}

protected:
	Vector3 position;
};
__declspec(align(16)) struct LightBase
{
public:
	LightBase(void)
	{
	}
	LightBase(const Vector4& diffuseI, const Vector4& specularI)
	{
		this->diffuseI = diffuseI;
		this->specularI = specularI;
	}

	inline const Vector4& GetDiffuse(void) const
	{
		return diffuseI;
	}
	inline const Vector4& GetSpecular(void) const
	{
		return specularI;
	}

	inline void SetDiffuse(const Vector4& diffuseI)
	{
		this->diffuseI = diffuseI;
	}
	inline void SetSpecular(const Vector4& specularI)
	{
		this->specularI = specularI;
	}

protected:
	Vector4 diffuseI;
	Vector4 specularI;
};

__declspec(align(16)) struct DirectionalLight : public LightBase, public Direction
{
public:
	DirectionalLight(void)
	{
	}
	DirectionalLight(const Vector3& direction, const Vector4& diffuseI, const Vector4& specularI)
		: LightBase(diffuseI, specularI), Direction(direction)
	{
	}
};
__declspec(align(16)) struct PointLight : public LightBase, public Position, public Attenuation
{
public:
	PointLight(void)
	{
	}
	PointLight(const Vector3& position, const Vector4& diffuseI, const Vector4& specularI, float constAtten, float linearAtten, float quadAtten)
		: LightBase(diffuseI, specularI), Position(position), Attenuation(constAtten, linearAtten, quadAtten)
	{
	}
};
__declspec(align(16)) struct SpotLight : public LightBase, public Direction, public Position, public Attenuation
{
public:
	SpotLight(void)
	{
		this->SetPhi(0);
		this->SetTheta(0);
		this->SetFalloff(0);
	}
	SpotLight(const Vector4& diffuseI, const Vector4& specularI, const Vector3& position, const Vector3& direction, float theta, float phi, float falloff, float constAtten, float linearAtten, float quadAtten)
		: LightBase(diffuseI, specularI), Direction(direction), Position(position), Attenuation(constAtten, linearAtten, quadAtten)
	{
		this->SetPhi(phi);
		this->SetTheta(theta);
		this->SetFalloff(falloff);
	}

	inline float GetTheta(void) const
	{
		return theta;
	}
	inline float GetPhi(void) const
	{
		return phi;
	}
	inline float GetFalloff(void) const
	{
		return falloff;
	}

	inline float GetCosPhiOver2(void) const
	{
		return this->cosPhiOver2;
	}
	inline float GetCosThetaOver2(void) const
	{
		return this->cosThetaOver2;
	}

	inline void SetTheta(float theta)
	{
		this->theta = theta;
		this->cosThetaOver2 = cos(theta * 0.5f);
	}
	inline void SetPhi(float phi)
	{
		this->phi = phi;
		this->cosPhiOver2 = cos(phi * 0.5f);
	}
	inline void SetFalloff(float falloff)
	{
		this->falloff = falloff;
	}

	inline bool CalculateSpotLightFactor(float rho, float& spotLightFactor) const
	{
		if (rho > GetCosThetaOver2())
		{
			spotLightFactor = 1.0f;
			return true;
		}
		else if (rho <= GetCosPhiOver2())
		{
			return false;
		}
		else
		{
			spotLightFactor = pow((rho - GetCosPhiOver2()) / (GetCosThetaOver2() - GetCosPhiOver2()), GetFalloff());
			spotLightFactor = max(spotLightFactor, 0.0f);
			spotLightFactor = min(spotLightFactor, 1.0f);
			return true;
		}
	}

private:
	float theta;
	float phi;
	float falloff; 
	float cosThetaOver2;
	float cosPhiOver2;
};