#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <memory>

namespace dae
{
	enum class GamepadButton {
		DPadLeft,
		DPadRight,
		DPadDown,
		DPadUp,
		A,
		B,
		X,
		Y,
	};

	class Gamepad
	{
	public:
		Gamepad();
		virtual ~Gamepad() = default;

		virtual bool UpdateButtons();
		virtual bool IsPressed(GamepadButton button) const;
		virtual bool IsDownThisFrame(GamepadButton button) const;
		virtual bool IsUpThisFrame(GamepadButton button) const;

	private:
		class GamepadImpl;
		std::unique_ptr<GamepadImpl> m_Pimpl;

		static int gamepadCount;
	};
}
#endif