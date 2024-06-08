#include "InputManager.h"

namespace dae
{
	InputManager::InputManager()
		: m_currentKeyStates{ SDL_GetKeyboardState(NULL) }
	{
		std::memset(m_previousKeyStates, 0, sizeof(m_previousKeyStates));
	}

	PlayerController* InputManager::AddPlayerController(PlayerController::ControlMethod controlMethod)
	{
		std::unique_ptr<Gamepad> gamepad{};

		if (controlMethod == PlayerController::ControlMethod::Gamepad) gamepad = std::make_unique<Gamepad>();
		else gamepad = nullptr;

		std::unique_ptr<PlayerController> newPlayerController{ std::make_unique<PlayerController>(controlMethod, std::move(gamepad)) };
		m_playerControllers.push_back(std::move(newPlayerController));

		return m_playerControllers.back().get();
	}

	void InputManager::RemovePlayerControllers()
	{
		m_playerControllers.clear();
		Gamepad::ResetGamepadCount();
	}

	bool InputManager::ProcessInput()
	{
		UpdateKeyStates();

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
			{
				return true;
			}
			else UpdatePlayerInput(e);
		}

		ExecuteCommands();

		return false;
	}

	void InputManager::UpdateKeyStates()
	{
		std::memcpy(m_previousKeyStates, m_currentKeyStates, sizeof(m_previousKeyStates));
		m_currentKeyStates = SDL_GetKeyboardState(NULL);
	}

	void InputManager::ExecuteCommands()
	{
		for (std::unique_ptr<PlayerController>& playerController : m_playerControllers)
		{
			for (Command* command : HandleInput(playerController)) command->Execute();
		}
	}

	std::vector<Command*> InputManager::HandleInput(const std::unique_ptr<PlayerController>& playerController)
	{
		if (playerController->gamepad) playerController->gamepad->UpdateButtons();

		std::vector<Command*> commands{};

		for (const std::tuple<PlayerController::KeyState, int, std::unique_ptr<Command>>& binding : playerController->bindings)
		{
			if ((std::get<0>(binding) == PlayerController::KeyState::Down && IsDown(playerController, std::get<1>(binding))) ||
				(std::get<0>(binding) == PlayerController::KeyState::DownThisFrame && IsDownThisFrame(playerController, std::get<1>(binding))) ||
				(std::get<0>(binding) == PlayerController::KeyState::UpThisFrame && IsUpThisFrame(playerController, std::get<1>(binding))))
				commands.push_back(std::get<2>(binding).get());
		}

		return commands;
	}

	bool InputManager::IsDown(const std::unique_ptr<PlayerController>& playerController, int input) const
	{
		switch (playerController->controlMethod)
		{
		case PlayerController::ControlMethod::Gamepad:
			return playerController->gamepad->IsDown(static_cast<GamepadButton>(input));
			break;
		case PlayerController::ControlMethod::Keyboard:
			return m_currentKeyStates[input];
			break;
		}

		return false;
	}

	bool InputManager::IsDownThisFrame(const std::unique_ptr<PlayerController>& playerController, int input)
	{
		switch (playerController->controlMethod)
		{
		case PlayerController::ControlMethod::Gamepad:
			return playerController->gamepad->IsDownThisFrame(static_cast<GamepadButton>(input));
			break;
		case PlayerController::ControlMethod::Keyboard:
			return m_currentKeyStates[input] && !m_previousKeyStates[input];
		break;
		}

		return false;
	}

	bool InputManager::IsUpThisFrame(const std::unique_ptr<PlayerController>& playerController, int input)
	{
		switch (playerController->controlMethod)
		{
		case PlayerController::ControlMethod::Gamepad:
			return playerController->gamepad->IsUpThisFrame(static_cast<GamepadButton>(input));
			break;
		case PlayerController::ControlMethod::Keyboard:
			return !m_currentKeyStates[input] && m_previousKeyStates[input];
			break;
		}

		return false;
	}

	void InputManager::UpdatePlayerInput(const SDL_Event& e)
	{
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_BACKSPACE && m_playerInput.length() > 0) m_playerInput.pop_back();
		}
		else if (e.type == SDL_TEXTINPUT)
			if (m_playerInput.size() < 20) m_playerInput += e.text.text;
	}
}