#include "Command.h"
#include "Components/RenderComponent.h"
#include "Components/GridComponent.h"

//---------------------------------
// MOVE
//---------------------------------
void dae::MoveCommand::MoveObject(glm::vec2& direction)
{
	dae::GameObject* gameObject{ GetGameObject() };
	if (!gameObject) return;

	glm::vec2 dir{};

	dae::GameObject* grid{ gameObject->GetParent() };
	if (grid)
	{
		glm::vec2 currentPos{ gameObject->GetTransform()->GetLocalPosition() };
		const glm::vec2 textureDimensions{ gameObject->GetComponent<RenderComponent>()->GetDimensions() };
		currentPos = { currentPos.x + textureDimensions.x / 2, currentPos.y + textureDimensions.y / 2 };

		dae::GridComponent* gridComponent{ grid->GetComponent<GridComponent>() };
		const glm::vec2 targetPos{ gridComponent->GetNextPosition(currentPos, direction) };

		if (targetPos == currentPos) return;

		dir = targetPos - currentPos;
	}
	else dir = direction;

	gameObject->GetTransform()->Translate(dir * m_speed * dae::TimeManager::GetInstance().GetDeltaTime());
}

void dae::MoveLeft::Execute()
{
	glm::vec2 direction{ -1, 0 };
	MoveObject(direction);
}

void dae::MoveRight::Execute()
{
	glm::vec2 direction{ 1, 0 };
	MoveObject(direction);
}

void dae::MoveDown::Execute()
{
	glm::vec2 direction{ 0, 1 };
	MoveObject(direction);
}

void dae::MoveUp::Execute()
{
	glm::vec2 direction{ 0, -1 };
	MoveObject(direction);
}