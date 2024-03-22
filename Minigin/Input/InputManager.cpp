#include <SDL.h>
#include "InputManager.h"

dae::PlayerController* dae::InputManager::AddPlayerController(ControlMethod controlMethod)
{
	std::unique_ptr<Gamepad> gamepad{};

	if (controlMethod == ControlMethod::Gamepad) gamepad = std::make_unique<Gamepad>(m_playerControllerCount);
	else gamepad = nullptr;

	std::unique_ptr<PlayerController> newPlayerController{ std::make_unique<PlayerController>(controlMethod, std::move(gamepad)) };

	m_playerControllers.push_back(std::move(newPlayerController));

	++m_playerControllerCount;

	return m_playerControllers[m_playerControllerCount - 1].get();
}

bool dae::InputManager::ProcessInput()
{
	ExecuteCommands();

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return true;
		}
	}

	return false;
}

std::vector<dae::Command*> dae::InputManager::HandleInput(std::unique_ptr<PlayerController>& playerController)
{
	std::vector<Command*> commands{};

	for (int i{}; i <= static_cast<int>(Input::Used); ++i)
	{
		Input input{ static_cast<Input>(i) };

		if (IsPressed(playerController, input))
		{
			auto it = playerController->bindings.find(input);
			if (it != playerController->bindings.end()) commands.push_back(it->second.get());
		}
	}

	return commands;
}

bool dae::InputManager::IsPressed(const std::unique_ptr<PlayerController>& playerController, Input input)
{
	switch (playerController->controlMethod)
	{
	case ControlMethod::Gamepad:

		switch (input)
		{
		case dae::Input::Left:
			return playerController->gamepad->IsPressed(GamepadButton::DPadLeft);
			break;
		case dae::Input::Right:
			return playerController->gamepad->IsPressed(GamepadButton::DPadRight);
			break;
		case dae::Input::Down:
			return playerController->gamepad->IsPressed(GamepadButton::DPadDown);
			break;
		case dae::Input::Up:
			return playerController->gamepad->IsPressed(GamepadButton::DPadUp);
			break;
		}

		break;
	case ControlMethod::Keyboard:
	{
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		switch (input)
		{
		case dae::Input::Left:
			return currentKeyStates[SDL_SCANCODE_A];
			break;
		case dae::Input::Right:
			return currentKeyStates[SDL_SCANCODE_D];
			break;
		case dae::Input::Down:
			return currentKeyStates[SDL_SCANCODE_S];
			break;
		case dae::Input::Up:
			return currentKeyStates[SDL_SCANCODE_W];
			break;
		}
	}
	}

	return false;
}

void dae::InputManager::ExecuteCommands()
{
	for (std::unique_ptr<PlayerController>& playerController : m_playerControllers)
	{
		for (Command* command : HandleInput(playerController)) command->Execute();
	}
}