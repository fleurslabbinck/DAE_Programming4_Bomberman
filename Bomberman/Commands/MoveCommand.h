#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include "../minigin/Input/Command.h"

namespace dae
{
	//---------------------------------
	// MOVECOMMAND
	//---------------------------------
	class MoveCommand : public GameObjectCommand
	{
	public:
		MoveCommand(dae::GameObject* gameObject, float speed, glm::vec2 direction) : GameObjectCommand(gameObject), m_speed{ speed }, m_direction{ direction } {}
		void Execute() override;

	private:
		const float m_speed;
		const glm::vec2 m_direction;
		static glm::vec2 m_lastDirection;
		glm::vec2 m_targetPos{};
		const float m_targetOffset{ 0.5f };
	};
}
#endif