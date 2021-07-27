
#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"
#include "InputManager.h"


__declspec(align(16)) class Camera
{
public:
	DEFINE_NEW_DELETE_ALIGNED

	Camera(void);

	void Update(const InputManager& inputManager, float elapsedSeconds);
	
	const Vector3& GetPosition(void) const;
	const Vector3& GetRotation(void) const;

	void SetPosition(const Vector3& position);
	void SetRotation(const Vector3& rotation);

	Matrix4x4 GetView(void) const;
	Vector3 GetForward(void) const;
	Matrix4x4 GetRotationMatrix(void) const;

private:
	Vector3 position;
	Vector3 rotation;
};