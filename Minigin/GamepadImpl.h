#include "GamepadInterface.h"

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
	//XINPUT_STATE m_currentState{};
	unsigned short m_previousButtons{};
	unsigned short m_buttonsPressedThisFrame{};
	unsigned short m_buttonsReleasedThisFrame{};

	unsigned int GetGamepadInput(GamepadButton button) const;
};
