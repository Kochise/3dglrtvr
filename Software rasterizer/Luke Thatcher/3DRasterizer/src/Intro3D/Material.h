
#pragma once

#include "Vector4.h"

class Material
{
public:
	Material(void);
	Material(const Vector4& ambientColor, const Vector4& diffuseColor, const Vector4& specularColor, float specularPower, Texture2D* diffuseTexture);

	const Vector4& GetDiffuseColor(void) const;
	const Vector4& GetSpecularColor(void) const;
	const Vector4& GetAmbientColor(void) const;
	float GetSpecularPower(void) const;
	Texture2D* GetDiffuseTexture(void) const;

	void SetDiffuseColor(const Vector4& diffuse);
	void SetSpecularColor(const Vector4& specular);
	void SetAmbientColor(const Vector4& ambient);
	void SetSpecularPower(float power);
	void SetDiffuseTexture(Texture2D* diffuseTexture);

private:
	Texture2D* diffuseTexture;
	Vector4 specularColor;
	Vector4 diffuseColor;
	Vector4 ambientColor;
	float specularPower;
};

inline Material::Material(void)
{
	this->specularPower = 0.0f;
}
inline Material::Material(const Vector4& ambientColor, const Vector4& diffuseColor, const Vector4& specularColor, float specularPower, Texture2D* diffuseTexture)
{
	this->ambientColor = ambientColor;
	this->diffuseColor = diffuseColor;
	this->specularColor = specularColor;
	this->specularPower = specularPower;
	this->diffuseTexture = diffuseTexture;
}

inline const Vector4& Material::GetDiffuseColor(void) const
{
	return this->diffuseColor;
}
inline const Vector4& Material::GetSpecularColor(void) const
{
	return this->specularColor;
}
inline const Vector4& Material::GetAmbientColor(void) const
{
	return this->ambientColor;
}
inline float Material::GetSpecularPower(void) const
{
	return this->specularPower;
}
inline Texture2D* Material::GetDiffuseTexture(void) const
{
	return this->diffuseTexture;
}

inline void Material::SetDiffuseColor(const Vector4& diffuse)
{
	this->diffuseColor = diffuse;
}
inline void Material::SetSpecularColor(const Vector4& specular)
{
	this->specularColor = specular;
}
inline void Material::SetAmbientColor(const Vector4& ambient)
{
	this->ambientColor = ambient;
}
inline void Material::SetSpecularPower(float power)
{
	this->specularPower = power;
}
inline void Material::SetDiffuseTexture(Texture2D* diffuseTexture)
{
	this->diffuseTexture = diffuseTexture;
}