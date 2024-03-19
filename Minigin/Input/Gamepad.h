#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "GamepadImpl.h"
#include <memory>

namespace dae
{
	class Gamepad final
	{
	public:
		Gamepad(int playerIdx);
		~Gamepad() = default;
		Gamepad(const Gamepad&) = delete;
		Gamepad& operator=(const Gamepad&) = delete;
		Gamepad(Gamepad&&) noexcept = default;
		Gamepad& operator=(Gamepad&&) noexcept = default;

		int GetPlayerIdx() const;

		bool UpdateButtons();
		bool IsPressed(GamepadButton button) const;
		bool IsDownThisFrame(GamepadButton button) const;
		bool IsUpThisFrame(GamepadButton button) const;

	private:
		std::unique_ptr<GamepadInterface> m_pImpl;
	};
}
#endif