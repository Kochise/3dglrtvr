
#include "stdafx.h"
#include "Camera.h"
#include "Matrix4x4.h"

#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera(void)
{
	this->position = Vector3(83.0f, 15.67f, 4.14f);
	this->rotation = Vector3(-0.255f, 1.533f, 0.0f);
}

void Camera::Update(const InputManager& inputManager, float elapsedSeconds)
{
    Vector3 cameraVelocity;
    Vector3 cameraRotation;

	// Get camera offsets from keyboard input
    if (inputManager.IsKeyDown(0x10)) // Shift
    {
        if (inputManager.IsKeyDown(0x57)) // W
            cameraRotation.X += 1;
        if (inputManager.IsKeyDown(0x53)) // S
            cameraRotation.X -= 1;
        if (inputManager.IsKeyDown(0x41)) // A
            cameraRotation.Y += 1;
        if (inputManager.IsKeyDown(0x44)) // D
            cameraRotation.Y -= 1;
    }
    else
    {
        if (inputManager.IsKeyDown(0x57)) // W
            cameraVelocity.Z -= 1;
        if (inputManager.IsKeyDown(0x53)) // S
            cameraVelocity.Z += 1;
        if (inputManager.IsKeyDown(0x41)) // A
            cameraVelocity.X -= 1;
        if (inputManager.IsKeyDown(0x44)) // D
            cameraVelocity.X += 1;
        if (inputManager.IsKeyDown(0x51)) // Q
            cameraVelocity.Y += 1;
        if (inputManager.IsKeyDown(0x45)) // E
            cameraVelocity.Y -= 1;
    }

	// Use game pad stick input if pad is connected
	if (inputManager.IsGamePadConnected())
	{
		Vector2 rightStick = inputManager.GetRightStick();
		Vector2 leftStick = inputManager.GetLeftStick();
		Vector2 triggers = inputManager.GetTriggers();

		cameraRotation.Y += -rightStick.X;
		cameraRotation.X += rightStick.Y;
	
		rotation.X = max(-(((float)M_PI) / 2.0f), min(rotation.X, ((float)M_PI) / 2.0f));

		cameraVelocity.Z += -leftStick.Y;
		cameraVelocity.X += leftStick.X;
		cameraVelocity.Y += (triggers.Y - triggers.X);
	}

	// Update Camera Rotation
	cameraRotation = Vector3::Clamp(cameraRotation, Vector3(-1, -1, -1), Vector3(1, 1, 1));
    cameraRotation *= (elapsedSeconds * 1.5f);
    rotation += cameraRotation;

	// Update Camera Position
	cameraVelocity = Vector3::Clamp(cameraVelocity, Vector3(-1, -1, -1), Vector3(1, 1, 1));
    cameraVelocity *= (elapsedSeconds * 75.0f);
	cameraVelocity = GetRotationMatrix() * cameraVelocity;
    position += cameraVelocity;

}

Vector3 Camera::GetForward(void) const
{
	Matrix4x4 rot = GetRotationMatrix();
	Vector3 forward = rot * Vector3(0, 0, -1);
	return forward;
}

Matrix4x4 Camera::GetRotationMatrix(void) const
{
	return Matrix4x4::CreateRotationX(rotation.X) * Matrix4x4::CreateRotationY(rotation.Y) * Matrix4x4::CreateRotationZ(rotation.Z);
}
Matrix4x4 Camera::GetView(void) const
{
	// Get Forward Vector
	Matrix4x4 rot = GetRotationMatrix();

	Vector3 forward = rot * Vector3(0, 0, -1);
	Vector3 up = rot * Vector3(0, 1, 0);

	return Matrix4x4::CreateLookAt(position, position + forward, up);
}

const Vector3& Camera::GetPosition(void) const
{
	return this->position;
}
const Vector3& Camera::GetRotation(void) const
{
	return this->rotation;
}

void Camera::SetPosition(const Vector3& position)
{
	this->position = position;
}
void Camera::SetRotation(const Vector3& rotation)
{
	this->rotation = rotation;
}