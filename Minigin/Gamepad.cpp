#include "Gamepad.h"

Gamepad::Gamepad(int idx)
	: m_idx{ idx }
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	
	DWORD result{ XInputGetState(m_idx, &state) };

	if (result == ERROR_SUCCESS)
	{
		m_currentState = state;
		m_currentState.dwPacketNumber = result;
	}
}

bool Gamepad::HandleButtons()
{
	XINPUT_STATE previousState;
	CopyMemory(&previousState, &m_currentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));
	XInputGetState(m_idx, &m_currentState);

	WORD buttonChanges = m_currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
	m_buttonsPressedThisFrame = buttonChanges & m_currentState.Gamepad.wButtons;
	m_buttonsReleasedThisFrame = buttonChanges & (~m_currentState.Gamepad.wButtons);

	return m_buttonsPressedThisFrame || m_buttonsReleasedThisFrame;
}

bool Gamepad::IsDownThisFrame(unsigned int button) const
{
	return m_buttonsPressedThisFrame & button;
}

bool Gamepad::IsUpThisFrame(unsigned int button) const
{
	return m_buttonsReleasedThisFrame & button;
}

bool Gamepad::IsPressed(unsigned int button) const
{
	return m_currentState.Gamepad.wButtons & button;
}