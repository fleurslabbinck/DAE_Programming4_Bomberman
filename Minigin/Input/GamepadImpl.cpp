#ifndef GAMEPADIMPL_CPP
#define GAMEPADIMPL_CPP

#include "GamepadImpl.h"
#include <Windows.h>
#include <Xinput.h>

int dae::GamepadImpl::gamepadCount{};

dae::GamepadImpl::GamepadImpl()
	: m_gamepadIdx{ gamepadCount }
{
	++gamepadCount;

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	DWORD result{ XInputGetState(m_gamepadIdx, &state) };

	if (result == ERROR_SUCCESS)
	{
		m_previousButtons = state.Gamepad.wButtons;
	}
}

bool dae::GamepadImpl::UpdateButtons()
{
	XINPUT_STATE currentState;
	ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	XInputGetState(m_gamepadIdx, &currentState);

	WORD buttonChanges = m_previousButtons ^ currentState.Gamepad.wButtons;

	m_buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
	m_buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);

	m_previousButtons = currentState.Gamepad.wButtons;

	return m_buttonsPressedThisFrame || m_buttonsReleasedThisFrame;
}

bool dae::GamepadImpl::IsPressed(GamepadButton button) const
{
	return m_previousButtons & GetGamepadInput(button);
}

bool dae::GamepadImpl::IsDownThisFrame(GamepadButton button) const
{
	return m_buttonsPressedThisFrame & GetGamepadInput(button);
}

bool dae::GamepadImpl::IsUpThisFrame(GamepadButton button) const
{
	return m_buttonsReleasedThisFrame & GetGamepadInput(button);
}

unsigned int dae::GamepadImpl::GetGamepadInput(GamepadButton button) const
{
	unsigned int gamepadInput{};

	switch (button)
	{
	case GamepadButton::DPadLeft:
		gamepadInput = XINPUT_GAMEPAD_DPAD_LEFT;
		break;
	case GamepadButton::DPadRight:
		gamepadInput = XINPUT_GAMEPAD_DPAD_RIGHT;
		break;
	case GamepadButton::DPadDown:
		gamepadInput = XINPUT_GAMEPAD_DPAD_DOWN;
		break;
	case GamepadButton::DPadUp:
		gamepadInput = XINPUT_GAMEPAD_DPAD_UP;
		break;
	case GamepadButton::A:
		gamepadInput = XINPUT_GAMEPAD_A;
		break;
	case GamepadButton::B:
		gamepadInput = XINPUT_GAMEPAD_B;
		break;
	case GamepadButton::X:
		gamepadInput = XINPUT_GAMEPAD_X;
		break;
	case GamepadButton::Y:
		gamepadInput = XINPUT_GAMEPAD_Y;
		break;
	}
	
	return gamepadInput;
}
#endif