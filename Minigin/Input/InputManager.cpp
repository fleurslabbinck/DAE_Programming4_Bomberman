#include <SDL.h>
#include "InputManager.h"

namespace dae
{
	PlayerController* InputManager::AddPlayerController(PlayerController::ControlMethod controlMethod)
	{
		std::unique_ptr<Gamepad> gamepad{};

		if (controlMethod == PlayerController::ControlMethod::Gamepad) gamepad = std::make_unique<Gamepad>();
		else gamepad = nullptr;

		std::unique_ptr<PlayerController> newPlayerController{ std::make_unique<PlayerController>(controlMethod, std::move(gamepad)) };
		m_newPlayerControllers.push_back(std::move(newPlayerController));

		return m_newPlayerControllers.back().get();
	}

	void InputManager::SetUpdatePlayerControllersFlag()
	{
		m_updatePlayerControllers = true;
		Gamepad::ResetGamepadCount();
	}

	void InputManager::UpdatePlayerControllers()
	{
		if (!m_updatePlayerControllers) return;
		m_playerControllers.clear();

		for (std::unique_ptr<PlayerController>& playerController : m_newPlayerControllers)
		{
			m_playerControllers.push_back(std::move(playerController));
		}

		m_newPlayerControllers.clear();

		m_updatePlayerControllers = false;
	}

	bool InputManager::ProcessInput()
	{
		ExecuteCommands();
		UpdatePlayerControllers();

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				return true;
			}
		}

		return false;
	}

	void InputManager::ExecuteCommands()
	{
		for (std::unique_ptr<PlayerController>& playerController : m_playerControllers)
		{
			for (Command* command : HandleInput(playerController)) command->Execute();
		}
	}

	std::vector<Command*> InputManager::HandleInput(const std::unique_ptr<PlayerController>& playerController) const
	{
		if (playerController->gamepad) playerController->gamepad->UpdateButtons();

		std::vector<Command*> commands{};

		for (const std::tuple<int, std::unique_ptr<Command>>& binding : playerController->bindings)
		{
			if (IsPressed(playerController, std::get<0>(binding))) commands.push_back(std::get<1>(binding).get());
		}

		return commands;
	}

	bool InputManager::IsPressed(const std::unique_ptr<PlayerController>& playerController, int input) const
	{
		switch (playerController->controlMethod)
		{
		case PlayerController::ControlMethod::Gamepad:
			return playerController->gamepad->IsPressed(static_cast<GamepadButton>(input));
			break;
		case PlayerController::ControlMethod::Keyboard:
		{
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			return currentKeyStates[input];
		}
			break;
		}

		return false;
	}
}