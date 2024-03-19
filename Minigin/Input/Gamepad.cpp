#include "Gamepad.h"

dae::Gamepad::Gamepad(int playerIdx)
	: m_pImpl{ std::make_unique<GamepadImpl>(playerIdx) }
{}

int dae::Gamepad::GetPlayerIdx() const
{
	return m_pImpl->GetPlayerIdx();
}

bool dae::Gamepad::UpdateButtons()
{
	return m_pImpl->UpdateButtons();
}

bool dae::Gamepad::IsPressed(GamepadButton button) const
{
	return m_pImpl->IsPressed(button);
}

bool dae::Gamepad::IsDownThisFrame(GamepadButton button) const
{
	return m_pImpl->IsDownThisFrame(button);
}

bool dae::Gamepad::IsUpThisFrame(GamepadButton button) const
{
	return m_pImpl->IsUpThisFrame(button);
}