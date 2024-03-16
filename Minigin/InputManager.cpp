#include "InputManager.h"

void dae::InputManager::InitGamepads(int amt)
{
	for (int i{}; i < amt; ++i)
	{
		const Gamepad gamepad{ Gamepad(i) };
		m_gamepads.push_back(gamepad);
	}
}

void dae::InputManager::BindCommand(dae::GameObject* gameObject)
{
	m_moveLeftController = MoveLeft(gameObject);
	m_moveRightController = MoveRight(gameObject);
	m_moveDownController = MoveDown(gameObject);
	m_moveUpController = MoveUp(gameObject);
}

bool dae::InputManager::ProcessInput()
{
	for (Gamepad& gamepad : m_gamepads)
	{
		if (gamepad.HandleButtons())
		{
			if (gamepad.IsPressed(XINPUT_GAMEPAD_DPAD_LEFT)) m_moveLeftController.Execute();
			else if (gamepad.IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT)) m_moveRightController.Execute();

			if (gamepad.IsPressed(XINPUT_GAMEPAD_DPAD_DOWN)) m_moveDownController.Execute();
			else if (gamepad.IsPressed(XINPUT_GAMEPAD_DPAD_UP)) m_moveUpController.Execute();
		}
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
