#include <SDL.h>
#include <Windows.h>
#include <WinBase.h>
#include <Xinput.h>
#include "InputManager.h"

bool dae::InputManager::ProcessInput()
{
	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
	
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &state);
	}

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return true;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
		}
		// etc...
	}

	return false;
}

void dae::InputManager::BindCommand(dae::GameObject* gameObject)
{
	m_moveLeftController = MoveLeft(gameObject);
	m_moveRightController = MoveRight(gameObject);
	m_moveDownController = MoveDown(gameObject);
	m_moveUpController = MoveUp(gameObject);
}