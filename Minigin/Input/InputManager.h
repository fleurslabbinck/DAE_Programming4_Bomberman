#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <vector>
#include "Singletons/Singleton.h"
#include "Command.h"
#include "Gamepad.h"
#include <unordered_map>

namespace dae
{
	enum class ControlMethod {
		Gamepad,
		Keyboard,
	};

	enum class Input {
		Left = 0,
		Right = 1,
		Down = 2,
		Up = 4,

		Used = 4,
	};

	struct PlayerController
	{
		ControlMethod controlMethod{ ControlMethod::Gamepad };
		std::unique_ptr<Gamepad> gamepad;

		std::unordered_map<Input, std::unique_ptr<Command>> bindings{};

		void BindCommand(Input input, std::unique_ptr<Command> command) { bindings[input] = std::move(command); };

		void Execute()
		{
			
		}
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager() = default;
		~InputManager() = default;
		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;

		bool ProcessInput();
		PlayerController* AddPlayerController(ControlMethod controlMethod);

	private:
		int m_playerControllerCount{};
		std::vector<std::unique_ptr<PlayerController>> m_playerControllers;

		void ExecuteCommands();
		std::vector<Command*> HandleInput(std::unique_ptr<PlayerController>& playerController);
		bool IsPressed(const std::unique_ptr<PlayerController>& playerController, Input input);
	};
}
#endif