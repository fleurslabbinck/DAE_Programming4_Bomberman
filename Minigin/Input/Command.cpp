#include "Command.h"
#include "Components/RenderComponent.h"
#include "Components/GridComponent.h"

namespace dae
{
	//---------------------------------
	// MOVE
	//---------------------------------
	glm::vec2 MoveCommand::m_lastDirection{};

	void MoveCommand::Execute()
	{
		dae::GameObject* gameObject{ GetGameObject() };

		// get current pos
		glm::vec2 currentPos{ gameObject->GetTransform()->GetLocalPosition() };
		const glm::vec2 textureDimensions{ gameObject->GetComponent<RenderComponent>()->GetDimensions() };
		currentPos = { currentPos.x + textureDimensions.x / 2, currentPos.y + textureDimensions.y / 2 };

		glm::vec2 dir{};

		// continue to target position if change of direction
		if (m_direction != m_lastDirection)
		{
			m_targetPos = currentPos;
			m_lastDirection = m_direction;
		}

		// get grid
		dae::GameObject* grid{ gameObject->GetParent() };
		if (!grid) return;
		dae::GridComponent* gridComponent{ grid->GetComponent<GridComponent>() };

		// check if reached target
		if (glm::distance(currentPos, m_targetPos) <= m_targetOffset) m_targetPos = gridComponent->GetNextPosition(currentPos, m_direction);

		// check if target blocked
		if (gridComponent->IsPosBlocked(m_targetPos)) return;

		dir = glm::normalize(m_targetPos - currentPos);

		gameObject->GetTransform()->Translate(dir * m_speed * dae::TimeManager::GetInstance().GetDeltaTime());
	}
}
