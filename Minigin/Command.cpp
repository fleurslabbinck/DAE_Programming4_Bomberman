#include "Command.h"

//---------------------------------
// MOVE
//---------------------------------
void Move::Execute()
{
	dae::GameObject* gameObject{ GetGameObject() };
	if (!gameObject) return;

	gameObject->Move(m_direction);
}

void Move::SetDirection(const glm::vec3& dir)
{
	m_direction = dir;
	m_set = true;
}