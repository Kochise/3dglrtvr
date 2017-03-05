
#pragma once

#include <Xinput.h>
#include "Vector2.h"

class InputManager
{
public:
	InputManager(void);

	const Vector2 GetLeftStick(void) const;
	const Vector2 GetRightStick(void) const;
	const Vector2 GetTriggers(void) const;
	const bool IsGamePadConnected(void) const;

	void ParseMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void Update(void);
	bool IsKeyDown(UINT keyCode) const;
	bool WasKeyDown(UINT keyCode) const;
	bool IsButtonDown(DWORD buttonCode) const;
	bool HasButtonPressed(DWORD buttonCode) const;
	bool HasKeyPressed(UINT keyCode) const;

private:
	bool currentKeyboardState[256];
	bool oldKeyboardState[256];

	bool gamePadIsConnected;

	XINPUT_STATE currentGamePadState;
	XINPUT_STATE oldGamePadState;

	static const Vector2 ParseStick(SHORT x, SHORT y, SHORT deadZone);
};