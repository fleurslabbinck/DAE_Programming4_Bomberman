#include "Gamepad.h"

#include <Windows.h>
#include <Xinput.h>

namespace dae
{
	int Gamepad::gamepadCount{};

	class Gamepad::GamepadImpl
	{
	public:
		GamepadImpl() : m_gamepadIdx{ gamepadCount }
		{
			++gamepadCount;

			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));

			DWORD result{ XInputGetState(m_gamepadIdx, &state) };

			if (result == ERROR_SUCCESS)
			{
				m_currentState = state;
				m_currentState.dwPacketNumber = result;
			}
		}

		bool UpdateButtons()
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

		bool IsPressed(GamepadButton button) const
		{
			return m_currentState.Gamepad.wButtons & GetGamepadInput(button);
		}

		bool IsDownThisFrame(GamepadButton button) const
		{
			return m_buttonsPressedThisFrame & GetGamepadInput(button);
		}

		bool IsUpThisFrame(GamepadButton button) const
		{
			return m_buttonsReleasedThisFrame & GetGamepadInput(button);
		}

		unsigned int GetGamepadInput(GamepadButton button) const
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

	private:
		const int m_gamepadIdx;
		XINPUT_STATE m_currentState{};
		WORD m_buttonsPressedThisFrame{};
		WORD m_buttonsReleasedThisFrame{};
	};

	Gamepad::Gamepad()
		: m_Pimpl{ std::make_unique<GamepadImpl>() }
	{

	}

	bool Gamepad::UpdateButtons()
	{
		return m_Pimpl->UpdateButtons();
	}

	bool Gamepad::IsPressed(GamepadButton button) const
	{
		return m_Pimpl->IsPressed(button);
	}

	bool Gamepad::IsDownThisFrame(GamepadButton button) const
	{
		return m_Pimpl->IsDownThisFrame(button);
	}

	bool Gamepad::IsUpThisFrame(GamepadButton button) const
	{
		return m_Pimpl->IsUpThisFrame(button);
	}
}