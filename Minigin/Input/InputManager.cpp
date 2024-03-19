#include <SDL.h>
#include "InputManager.h"

void dae::InputManager::AddPlayerController(dae::GameObject* gameObject, float speed, ControlMethod controlMethod)
{
	std::unique_ptr<Gamepad> gamepad{};

	if (controlMethod == ControlMethod::Gamepad) gamepad = std::make_unique<Gamepad>(m_playerControllerCount);
	else gamepad = nullptr;

	m_playerControllers.push_back(PlayerController((controlMethod), std::move(gamepad)));

	m_playerControllers[m_playerControllerCount].Bind(gameObject, speed);
	++m_playerControllerCount;
}

bool dae::InputManager::ProcessInput()
{
	UpdateInput();
	ExecuteCommands();

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return true;
		}
	}

	return false;
}

void dae::InputManager::UpdateInput()
{
	for (PlayerController& playerController : m_playerControllers)
	{
		glm::vec2 direction{};

		switch (playerController.controlMethod)
		{
		case ControlMethod::Gamepad:

			if (playerController.gamepad->UpdateButtons())
			{
				if (!playerController.gamepad->IsPressed(GamepadButton::DPadLeft) && !playerController.gamepad->IsPressed(GamepadButton::DPadRight) &&
					!playerController.gamepad->IsPressed(GamepadButton::DPadDown) && !playerController.gamepad->IsPressed(GamepadButton::DPadUp))
				{
					playerController.move.Reset();
					break;
				}

				if (playerController.gamepad->IsPressed(GamepadButton::DPadLeft)) direction.x -= 1;
				if (playerController.gamepad->IsPressed(GamepadButton::DPadRight)) direction.x += 1;
				if (playerController.gamepad->IsPressed(GamepadButton::DPadDown)) direction.y += 1;
				if (playerController.gamepad->IsPressed(GamepadButton::DPadUp)) direction.y -= 1;

				playerController.move.SetDirection(direction);
			}

			break;
		case ControlMethod::Keyboard:
		{
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

			if (!currentKeyStates[SDL_SCANCODE_A] && !currentKeyStates[SDL_SCANCODE_D] && !currentKeyStates[SDL_SCANCODE_S] && !currentKeyStates[SDL_SCANCODE_W])
			{
				playerController.move.Reset();
				break;
			}

			if (currentKeyStates[SDL_SCANCODE_A]) direction.x -= 1;
			if (currentKeyStates[SDL_SCANCODE_D]) direction.x += 1;
			if (currentKeyStates[SDL_SCANCODE_S]) direction.y += 1;
			if (currentKeyStates[SDL_SCANCODE_W]) direction.y -= 1;

			playerController.move.SetDirection(direction);

			break;
		}
		}
	}
}

//void dae::InputManager::UpdateGamepadInput()
//{
//	for (Gamepad& gamepad : m_gamepads)
//	{
//		if (gamepad.UpdateButtons())
//		{
//			glm::vec2 direction{};
//
//			if (!gamepad.IsPressed(GamepadButton::DPadLeft) && !gamepad.IsPressed(GamepadButton::DPadRight) && !gamepad.IsPressed(GamepadButton::DPadDown) && !gamepad.IsPressed(GamepadButton::DPadUp))
//			{
//				m_controllers[gamepad.GetPlayerIdx()].move.Reset();
//				break;
//			}
//
//			if (gamepad.IsPressed(GamepadButton::DPadLeft)) direction.x -= 1;
//			if (gamepad.IsPressed(GamepadButton::DPadRight)) direction.x += 1;
//			if (gamepad.IsPressed(GamepadButton::DPadDown)) direction.y += 1;
//			if (gamepad.IsPressed(GamepadButton::DPadUp)) direction.y -= 1;
//
//			m_controllers[gamepad.GetPlayerIdx()].move.SetDirection(direction);
//		}
//	}
//}

//void dae::InputManager::UpdateKeyboardInput()
//{
//	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
//
//	size_t idx{};
//
//	if (m_gamepads.empty()) idx = 0;
//	else idx = m_gamepads.size();
//
//	for (idx; idx < m_controllers.size(); ++idx)
//	{
//		glm::vec2 direction{};
//
//		if (!currentKeyStates[SDL_SCANCODE_A] && !currentKeyStates[SDL_SCANCODE_D] && !currentKeyStates[SDL_SCANCODE_S] && !currentKeyStates[SDL_SCANCODE_W])
//		{
//			m_controllers[idx].move.Reset();
//			break;
//		}
//
//		if (currentKeyStates[SDL_SCANCODE_A]) direction.x -= 1;
//		if (currentKeyStates[SDL_SCANCODE_D]) direction.x += 1;
//		if (currentKeyStates[SDL_SCANCODE_S]) direction.y += 1;
//		if (currentKeyStates[SDL_SCANCODE_W]) direction.y -= 1;
//
//		m_controllers[idx].move.SetDirection(direction);
//	}
//}

void dae::InputManager::ExecuteCommands()
{
	for (PlayerController& playerController : m_playerControllers) playerController.Execute();
}