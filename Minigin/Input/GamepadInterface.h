#ifndef GAMEPADINTERFACE_H
#define GAMEPADINTERFACE_H

namespace dae
{
	enum class GamepadButton {
		DPadLeft,
		DPadRight,
		DPadDown,
		DPadUp
	};

	class GamepadInterface
	{
	public:
		virtual ~GamepadInterface() = default;

		virtual int GetPlayerIdx() const = 0;

		virtual bool UpdateButtons() = 0;
		virtual bool IsPressed(GamepadButton button) const = 0;
		virtual bool IsDownThisFrame(GamepadButton button) const = 0;
		virtual bool IsUpThisFrame(GamepadButton button) const = 0;
	};
}
#endif