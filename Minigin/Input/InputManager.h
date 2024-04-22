#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <vector>
#include "Singleton.h"
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

		ControlMethod controlMethod{ ControlMethod::Gamepad };
		std::unique_ptr<Gamepad> gamepad;

		std::vector<std::tuple<int, std::unique_ptr<Command>>> bindings{};

		void BindCommand(int input, std::unique_ptr<Command> command)
		{
			bindings.push_back(std::tuple<int, std::unique_ptr<Command>>{ input, std::move(command) });
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
		InputManager() = default;
		~InputManager() = default;

		int m_playerControllerCount{};
		std::vector<std::unique_ptr<PlayerController>> m_playerControllers;

		void ExecuteCommands();
		std::vector<Command*> HandleInput(const std::unique_ptr<PlayerController>& playerController) const;
		bool IsPressed(const std::unique_ptr<PlayerController>& playerController, int input) const;
	};
}
#endif