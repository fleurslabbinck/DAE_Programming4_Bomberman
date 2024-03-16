#pragma once
#include <SDL.h>
#include <Windows.h>
#include <WinBase.h>
#include <Xinput.h>
#include <vector>

class Gamepad final
{
public:
	Gamepad(int gamepadIdx = 0, int playerIdx = 0);
	~Gamepad() = default;

	int GetPlayerIdx() const { return m_playerIdx; }

	bool UpdateButtons();
	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;

private:
	const int m_gamepadIdx;
	const int m_playerIdx;
	XINPUT_STATE m_currentState{};
	WORD m_buttonsPressedThisFrame{};
	WORD m_buttonsReleasedThisFrame{};
};