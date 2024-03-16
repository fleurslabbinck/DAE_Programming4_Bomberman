#include "Gamepad.h"
#include "GamepadImpl.h"

Gamepad::Gamepad(int gamepadIdx, int playerIdx)
	: m_pImpl{ std::make_unique<GamepadImpl>(gamepadIdx, playerIdx) }
{}

int Gamepad::GetPlayerIdx() const
{
	return m_pImpl->GetPlayerIdx();
}

bool Gamepad::UpdateButtons()
{
	return m_pImpl->UpdateButtons();
}

bool Gamepad::IsPressed(GamepadButton button) const
{
	return m_pImpl->IsPressed(button);
}

bool Gamepad::IsDownThisFrame(GamepadButton button) const
{
	return m_pImpl->IsDownThisFrame(button);
}

bool Gamepad::IsUpThisFrame(GamepadButton button) const
{
	return m_pImpl->IsUpThisFrame(button);
}