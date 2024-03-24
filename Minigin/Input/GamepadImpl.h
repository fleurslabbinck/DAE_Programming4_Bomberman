#ifndef GAMEPADIMPL_H
#define GAMEPADIMPL_H

#include "GamepadInterface.h"

namespace dae
{
	class GamepadImpl : public GamepadInterface
	{
	public:
		GamepadImpl();
		virtual ~GamepadImpl() = default;

		virtual bool UpdateButtons() override;
		virtual bool IsPressed(GamepadButton button) const override;
		virtual bool IsDownThisFrame(GamepadButton button) const override;
		virtual bool IsUpThisFrame(GamepadButton button) const override;

	private:
		static int gamepadCount;
		const int m_gamepadIdx;
		unsigned short m_previousButtons{};
		unsigned short m_buttonsPressedThisFrame{};
		unsigned short m_buttonsReleasedThisFrame{};

		unsigned int GetGamepadInput(GamepadButton button) const;
	};
}
#endif