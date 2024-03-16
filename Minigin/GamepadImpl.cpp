#include "GamepadImpl.h"

GamepadImpl::GamepadImpl(int gamepadIdx, int playerIdx)
	: m_gamepadIdx{ gamepadIdx }, m_playerIdx{ playerIdx }
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	DWORD result{ XInputGetState(m_gamepadIdx, &state) };

	if (result == ERROR_SUCCESS)
	{
		m_currentState = state;
		m_currentState.dwPacketNumber = result;
	}
}

bool GamepadImpl::UpdateButtons()
{
	XINPUT_STATE previousState;
	CopyMemory(&previousState, &m_currentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));
	XInputGetState(m_gamepadIdx, &m_currentState);

	WORD buttonChanges = m_currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
	m_buttonsPressedThisFrame = buttonChanges & m_currentState.Gamepad.wButtons;
	m_buttonsReleasedThisFrame = buttonChanges & (~m_currentState.Gamepad.wButtons);

	return m_buttonsPressedThisFrame || m_buttonsReleasedThisFrame;
}

bool GamepadImpl::IsPressed(GamepadButton button) const
{
	return m_currentState.Gamepad.wButtons & GetGamepadInput(button);
}

bool GamepadImpl::IsDownThisFrame(GamepadButton button) const
{
	return m_buttonsPressedThisFrame & GetGamepadInput(button);
}

bool GamepadImpl::IsUpThisFrame(GamepadButton button) const
{
	return m_buttonsReleasedThisFrame & GetGamepadInput(button);
}

unsigned int GamepadImpl::GetGamepadInput(GamepadButton button) const
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
	}
	
	return gamepadInput;
}