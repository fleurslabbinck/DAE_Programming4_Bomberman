#pragma once
#include <vector>
#include "Singleton.h"
#include "Command.h"
#include "Gamepad.h"

namespace dae
{
	enum class InputMethod {
		Gamepad,
		Keyboard,
	};

	struct Controller
	{
		MoveLeft moveLeft{};
		MoveRight moveRight{};
		MoveDown moveDown{};
		MoveUp moveUp{};

		InputMethod inputMethod;

		void Bind(GameObject* gameObject)
		{
			moveLeft = MoveLeft(gameObject);
			moveRight = MoveRight(gameObject);
			moveDown = MoveDown(gameObject);
			moveUp = MoveUp(gameObject);
		}

		void Update()
		{

		}

		void Execute()
		{
			if (moveLeft.IsEnabled()) moveLeft.Execute();
			if (moveRight.IsEnabled()) moveRight.Execute();
			if (moveDown.IsEnabled()) moveDown.Execute();
			if (moveUp.IsEnabled()) moveUp.Execute();
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
