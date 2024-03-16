#include <SDL.h>
#include "InputManager.h"

int dae::InputManager::controllerCount{};

void dae::InputManager::Init(int nrControllers, int nrGamepads)
{
	for (int i{}; i < nrControllers; ++i)
	{
		m_controllers.push_back(Controller());
	}

	for (int i{}; i < nrGamepads; ++i)
	{
		//const Gamepad gamepad{ Gamepad(i) };
		m_gamepads.push_back(i);
	}
}

void dae::InputManager::BindCommand(dae::GameObject* gameObject)
{
	m_controllers[controllerCount].Bind(gameObject);
	++controllerCount;
}

bool dae::InputManager::ProcessInput()
{
	UpdateGamepadInput();
	UpdateKeyboardInput();
	ExecuteCommands();

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

void dae::InputManager::UpdateGamepadInput()
{
	for (Gamepad& gamepad : m_gamepads)
	{
		if (gamepad.UpdateButtons())
		{
			glm::vec3 direction{};

			if (!gamepad.IsPressed(GamepadButton::DPadLeft) && !gamepad.IsPressed(GamepadButton::DPadRight) && !gamepad.IsPressed(GamepadButton::DPadDown) && !gamepad.IsPressed(GamepadButton::DPadUp))
			{
				m_controllers[gamepad.GetPlayerIdx()].move.Reset();
				break;
			}

			if (gamepad.IsPressed(GamepadButton::DPadLeft)) direction.x -= 1;
			if (gamepad.IsPressed(GamepadButton::DPadRight)) direction.x += 1;
			if (gamepad.IsPressed(GamepadButton::DPadDown)) direction.y += 1;
			if (gamepad.IsPressed(GamepadButton::DPadUp)) direction.y -= 1;

			m_controllers[gamepad.GetPlayerIdx()].move.SetDirection(direction);
		}
	}
}

void dae::InputManager::UpdateKeyboardInput()
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	size_t idx{};

	if (m_gamepads.empty()) idx = 0;
	else idx = m_gamepads.size();

	for (idx; idx < m_controllers.size(); ++idx)
	{
		glm::vec3 direction{};

		if (!currentKeyStates[SDL_SCANCODE_A] && !currentKeyStates[SDL_SCANCODE_D] && !currentKeyStates[SDL_SCANCODE_S] && !currentKeyStates[SDL_SCANCODE_W])
		{
			m_controllers[idx].move.Reset();
			break;
		}

		if (currentKeyStates[SDL_SCANCODE_A]) direction.x -= 1;
		if (currentKeyStates[SDL_SCANCODE_D]) direction.x += 1;
		if (currentKeyStates[SDL_SCANCODE_S]) direction.y += 1;
		if (currentKeyStates[SDL_SCANCODE_W]) direction.y -= 1;

		m_controllers[idx].move.SetDirection(direction);
	}
}

void dae::InputManager::ExecuteCommands()
{
	for (Controller& controller : m_controllers) controller.Execute();
}