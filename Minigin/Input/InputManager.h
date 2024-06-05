#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL.h>
#include <vector>
#include "../Singleton.h"
#include "Command.h"
#include "Gamepad.h"
#include <unordered_map>

namespace dae
{
	struct PlayerController
	{
		enum class ControlMethod {
			Gamepad,
			Keyboard,
		};

		enum class KeyState {
			Down,
			DownThisFrame,
			UpThisFrame
		};

		ControlMethod controlMethod{ ControlMethod::Gamepad };
		std::unique_ptr<Gamepad> gamepad;

		std::vector<std::tuple<KeyState, int, std::unique_ptr<Command>>> bindings{};

		void BindCommand(KeyState keyState, int input, std::unique_ptr<Command> command)
		{
			bindings.push_back(std::tuple<KeyState, int, std::unique_ptr<Command>>{ keyState, input, std::move(command) });
		}
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;

		bool ProcessInput();
		PlayerController* AddPlayerController(PlayerController::ControlMethod controlMethod);
		void RemovePlayerControllers();
		
	private:
		friend class Singleton<InputManager>;
		InputManager();
		~InputManager() = default;

		const Uint8* m_currentKeyStates;
		Uint8 m_previousKeyStates[SDL_NUM_SCANCODES]{};

		std::vector<std::unique_ptr<PlayerController>> m_playerControllers;

		void UpdateKeyStates();
		void ExecuteCommands();
		std::vector<Command*> HandleInput(const std::unique_ptr<PlayerController>& playerController);
		bool IsDown(const std::unique_ptr<PlayerController>& playerController, int input) const;
		bool IsDownThisFrame(const std::unique_ptr<PlayerController>& playerController, int input);
	};
}
#endif