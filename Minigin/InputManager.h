#pragma once
#include "Singleton.h"
#include "Command.h"
#include "Gamepad.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		void InitGamepads(int amt);
		void BindCommand(dae::GameObject* gameObject);
		bool ProcessInput();

	private:
		MoveLeft m_moveLeftController{};
		MoveRight m_moveRightController{};
		MoveDown m_moveDownController{};
		MoveUp m_moveUpController{};

		std::vector<Gamepad> m_gamepads{};
	};
}
