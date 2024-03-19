#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <vector>
#include "Singletons/Singleton.h"
#include "Command.h"
#include "Gamepad.h"

namespace dae
{
	enum class ControlMethod {
		Gamepad,
		Keyboard,
	};

	struct PlayerController
	{
		ControlMethod controlMethod{ ControlMethod::Gamepad };
		std::unique_ptr<Gamepad> gamepad;
		Move move{};

		void Bind(GameObject* gameObject, float speed)
		{
			move = Move(gameObject, speed);
		}

		void Execute()
		{
			if (move.GetSet()) move.Execute();
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

		void AddPlayerController(dae::GameObject* gameObject, float speed, ControlMethod controlMethod);
		bool ProcessInput();

	private:
		int m_playerControllerCount{};
		std::vector<PlayerController> m_playerControllers;

		void UpdateInput();
		//void UpdateGamepadInput();
		//void UpdateKeyboardInput();
		void ExecuteCommands();
	};
}
#endif