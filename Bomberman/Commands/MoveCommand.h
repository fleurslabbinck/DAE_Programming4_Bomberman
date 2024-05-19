#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include "Input/Command.h"

namespace dae
{
	//---------------------------------
	// MOVECOMMAND
	//---------------------------------
	class MoveCommand : public GameObjectCommand
	{
	public:
		MoveCommand(dae::GameObject* gameObject, float speed, glm::vec2 direction);
		void Execute() override;

	private:
		const float m_speed;
		const float m_targetOffset{ 0.5f };
		const glm::vec2 m_direction;
		glm::vec2 m_targetPos{};
	};
}
#endif