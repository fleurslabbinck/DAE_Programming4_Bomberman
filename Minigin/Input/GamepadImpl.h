#ifndef GAMEPADIMPL_H
#define GAMEPADIMPL_H

#include "GamepadInterface.h"

namespace dae
{
	class GamepadImpl : public GamepadInterface
	{
	public:
		GamepadImpl(int playerIdx);
		virtual ~GamepadImpl() = default;

		virtual int GetPlayerIdx() const { return m_playerIdx; }

		virtual bool UpdateButtons() override;
		virtual bool IsPressed(GamepadButton button) const override;
		virtual bool IsDownThisFrame(GamepadButton button) const override;
		virtual bool IsUpThisFrame(GamepadButton button) const override;

	private:
		static int gamepadCount;
		const int m_gamepadIdx;
		const int m_playerIdx;
		unsigned short m_previousButtons{};
		unsigned short m_buttonsPressedThisFrame{};
		unsigned short m_buttonsReleasedThisFrame{};

		unsigned int GetGamepadInput(GamepadButton button) const;
	};
}
#endif