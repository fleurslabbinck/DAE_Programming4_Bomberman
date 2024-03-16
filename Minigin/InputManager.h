#pragma once
#include <vector>
#include "Singleton.h"
#include "Command.h"
#include "Gamepad.h"

namespace dae
{
	struct Controller
	{
		Move move{};

		void Bind(GameObject* gameObject)
		{
			move = Move(gameObject);
		}

		void Execute()
		{
			if (move.GetSet()) move.Execute();
		}
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		void Init(int nrControllers, int nrGamepads);
		void BindCommand(dae::GameObject* gameObject);
		bool ProcessInput();

	private:
		static int controllerCount;

		std::vector<Controller> m_controllers;
		std::vector<Gamepad> m_gamepads{};

		void UpdateGamepadInput();
		void UpdateKeyboardInput();
		void ExecuteCommands();
	};
}
