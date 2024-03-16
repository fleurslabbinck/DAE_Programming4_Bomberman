#pragma once
#include "Singleton.h"
#include "Command.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void BindCommand(dae::GameObject* gameObject);

	private:
		MoveLeft m_moveLeftController{};
		MoveRight m_moveRightController{};
		MoveDown m_moveDownController{};
		MoveUp m_moveUpController{};
	};

}
