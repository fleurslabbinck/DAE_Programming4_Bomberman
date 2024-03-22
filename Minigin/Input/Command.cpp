#include "Command.h"

//---------------------------------
// MOVE
//---------------------------------
float dae::MoveCommand::CalculateOffset() const
{
	return m_speed * dae::TimeManager::GetInstance().GetDeltaTime();
}

void dae::MoveLeft::Execute()
{
	dae::GameObject* gameObject{ GetGameObject() };
	if (!gameObject) return;

	glm::vec2 offset{};
	offset.x -= CalculateOffset();

	gameObject->GetTransform()->Translate(offset);
}

void dae::MoveRight::Execute()
{
	dae::GameObject* gameObject{ GetGameObject() };
	if (!gameObject) return;

	glm::vec2 offset{};
	offset.x += CalculateOffset();

	gameObject->GetTransform()->Translate(offset);
}

void dae::MoveDown::Execute()
{
	dae::GameObject* gameObject{ GetGameObject() };
	if (!gameObject) return;

	glm::vec2 offset{};
	offset.y += CalculateOffset();

	gameObject->GetTransform()->Translate(offset);
}

void dae::MoveUp::Execute()
{
	dae::GameObject* gameObject{ GetGameObject() };
	if (!gameObject) return;

	glm::vec2 offset{};
	offset.y -= CalculateOffset();

	gameObject->GetTransform()->Translate(offset);
}