
#include "stdafx.h"
#include "InputManager.h"

InputManager::InputManager(void)
{
	// Initialize all states to 0
	ZeroMemory(currentKeyboardState, sizeof(currentKeyboardState));
	ZeroMemory(&currentGamePadState, sizeof(currentGamePadState));

	// Perform initial update
	Update();
}

void InputManager::Update(void)
{
	// Copy current states into old state fields
	memcpy(oldKeyboardState, currentKeyboardState, sizeof(currentKeyboardState));
	memcpy(&oldGamePadState, &currentGamePadState, sizeof(currentGamePadState));

	// Poll game pad for new state
	LONG result = XInputGetState(0, &currentGamePadState);

	// If an error occured
	if (result != ERROR_SUCCESS)
	{
		if (result == ERROR_DEVICE_NOT_CONNECTED)
			this->gamePadIsConnected = false;
	}
	else
	{
		this->gamePadIsConnected = true;
	}
}

void InputManager::ParseMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_KEYDOWN:
		{
			if (wParam < 256)
			{
				this->currentKeyboardState[wParam] = true;
			}
		}
		break;
	case WM_KEYUP:
		{
			if (wParam < 256)
			{
				this->currentKeyboardState[wParam] = false;
			}
		}
		break;
	}
}

bool InputManager::IsButtonDown(DWORD buttonCode) const
{
	return (this->currentGamePadState.Gamepad.wButtons & buttonCode) == buttonCode;
}
bool InputManager::HasButtonPressed(DWORD buttonCode) const
{
	return ((this->currentGamePadState.Gamepad.wButtons & buttonCode) == buttonCode) &&
		((this->oldGamePadState.Gamepad.wButtons & buttonCode) == 0);
}
bool InputManager::HasKeyPressed(UINT keyCode) const
{
	return this->IsKeyDown(keyCode) && !this->WasKeyDown(keyCode);
}

const Vector2 InputManager::ParseStick(SHORT x, SHORT y, SHORT deadZone)
{
	// Get the deadzone filtered left stick X value
	FLOAT filteredX = 0;
	if (x < 0)
	{
		if ((-x) >= deadZone)
			filteredX = ((FLOAT)x) / 32768.0f;
	}
	else
	{
		if (x >= deadZone)
			filteredX = ((FLOAT)x) / 32767.0f;
	}

	// Get the deadzone filtered left stick Y value
	FLOAT filteredY = 0;
	if (y < 0)
	{
		if ((-y) >= deadZone)
			filteredY = ((FLOAT)y) / 32768.0f;
	}
	else
	{
		if (y >= deadZone)
			filteredY = ((FLOAT)y) / 32767.0f;
	}

	return Vector2(filteredX, filteredY);
}

const Vector2 InputManager::GetLeftStick(void) const
{
	return ParseStick(currentGamePadState.Gamepad.sThumbLX, currentGamePadState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
}
const Vector2 InputManager::GetRightStick(void) const
{
	return ParseStick(currentGamePadState.Gamepad.sThumbRX, currentGamePadState.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
}

const Vector2 InputManager::GetTriggers(void) const
{
	Vector2 triggers(
		(float)(currentGamePadState.Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? currentGamePadState.Gamepad.bLeftTrigger : 0),
		(float)(currentGamePadState.Gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? currentGamePadState.Gamepad.bRightTrigger : 0));
	return triggers / 255.0f;
}

bool InputManager::IsKeyDown(UINT keyCode) const
{
	if (keyCode < 256)
		return this->currentKeyboardState[keyCode];
	else
		return false;
}
bool InputManager::WasKeyDown(UINT keyCode) const
{
	if (keyCode < 256)
		return this->oldKeyboardState[keyCode];
	else
		return false;
}

const bool InputManager::IsGamePadConnected(void) const
{
	return this->gamePadIsConnected;
}