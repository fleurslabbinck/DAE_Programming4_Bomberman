#pragma once
#include <SDL.h>
#include <Windows.h>
#include <WinBase.h>
#include <Xinput.h>
#include <vector>

class Gamepad final
{
public:
	Gamepad(int idx);
	~Gamepad() = default;

	bool HandleButtons();
	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;

private:
	const int m_idx;
	XINPUT_STATE m_currentState{};
	WORD m_buttonsPressedThisFrame{};
	WORD m_buttonsReleasedThisFrame{};
};