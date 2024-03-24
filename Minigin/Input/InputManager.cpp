#include <SDL.h>
#include "InputManager.h"

dae::PlayerController* dae::InputManager::AddPlayerController(ControlMethod controlMethod)
{
	std::unique_ptr<Gamepad> gamepad{};

	if (controlMethod == ControlMethod::Gamepad) gamepad = std::make_unique<Gamepad>();
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

void dae::InputManager::ExecuteCommands()
{
	for (std::unique_ptr<PlayerController>& playerController : m_playerControllers)
	{
		for (Command* command : HandleInput(playerController)) command->Execute();
	}
}

std::vector<dae::Command*> dae::InputManager::HandleInput(const std::unique_ptr<PlayerController>& playerController) const
{
	if (playerController->gamepad) playerController->gamepad->UpdateButtons();

	std::vector<Command*> commands{};

	for (const std::tuple<int, std::unique_ptr<Command>>& binding : playerController->bindings)
	{
		if (IsPressed(playerController, std::get<0>(binding))) commands.push_back(std::get<1>(binding).get());
	}

	return commands;
}

bool dae::InputManager::IsPressed(const std::unique_ptr<PlayerController>& playerController, int input) const
{
	switch (playerController->controlMethod)
	{
	case ControlMethod::Gamepad:
		return playerController->gamepad->IsPressed(static_cast<GamepadButton>(input));
		break;
	case ControlMethod::Keyboard:
	{
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		return currentKeyStates[input];
	}
		break;
	}

	return false;
}