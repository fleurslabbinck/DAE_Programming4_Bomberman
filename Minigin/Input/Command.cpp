#include "Command.h"

//---------------------------------
// MOVE
//---------------------------------
void dae::Move::Execute()
{
	dae::GameObject* gameObject{ GetGameObject() };
	if (!gameObject) return;

	m_direction = glm::normalize(m_direction);

	if (m_direction != glm::vec2{}) gameObject->GetTransform()->Translate(m_direction * m_speed * dae::TimeManager::GetInstance().GetDeltaTime());
}

void dae::Move::SetDirection(const glm::vec2& dir)
{
	m_direction = dir;
	m_set = true;
}