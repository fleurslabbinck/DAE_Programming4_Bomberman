#include "GamepadInterface.h"
#include <Windows.h>
#include <Xinput.h>

class GamepadImpl : public GamepadInterface
{
public:
	GamepadImpl(int gamepadIdx = 0, int playerIdx = 0);
	virtual ~GamepadImpl() = default;

	virtual int GetPlayerIdx() const { return m_playerIdx; }

	virtual bool UpdateButtons() override;
	virtual bool IsPressed(GamepadButton button) const override;
	virtual bool IsDownThisFrame(GamepadButton button) const override;
	virtual bool IsUpThisFrame(GamepadButton button) const override;

private:
	const int m_gamepadIdx;
	const int m_playerIdx;
	XINPUT_STATE m_currentState{};
	WORD m_buttonsPressedThisFrame{};
	WORD m_buttonsReleasedThisFrame{};

	unsigned int GetGamepadInput(GamepadButton button) const;
};
