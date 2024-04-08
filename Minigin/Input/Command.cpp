#include "Command.h"
#include "SceneManager.h"
#include "Components/RenderComponent.h"
#include "Components/GridComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/CameraComponent.h"

namespace dae
{
	//---------------------------------
	// MOVE
	//---------------------------------
	glm::vec2 MoveCommand::m_lastDirection{};

	void MoveCommand::Execute()
	{
		if (SceneManager::GetInstance().GetGameState() != SceneManager::GameState::GAME) return;

		GameObject* gameObject{ GetGameObject() };
		GridComponent* gridComponent{ gameObject->GetParent()->GetComponent<GridComponent>()};
		SpriteComponent* spriteComponent{ gameObject->GetComponent<SpriteComponent>() };
		const CollisionComponent* collisionComp{ gameObject->GetComponent<CollisionComponent>() };
		if (!collisionComp) return;

		glm::vec2 dir{};

		// get current pos
		glm::vec2 bottomLeft{ gameObject->GetTransform()->GetLocalPosition() };
		glm::vec2 centeredPos{ bottomLeft + collisionComp->GetCenter() };

		// check entity collision
		if (collisionComp->HandleCollision(bottomLeft, gridComponent->GetEntitiesClose(centeredPos))) return;

		// reset target position if change of direction
		if (m_direction != m_lastDirection)
		{
			m_targetPos = centeredPos;
			m_lastDirection = m_direction;
			spriteComponent->SetDirection(m_direction);
		}

		// check if reached target
		if (glm::distance(centeredPos, m_targetPos) <= m_targetOffset)
		{
			m_targetPos = gridComponent->GetNextPosition(centeredPos, m_direction);
		}

		// set animation frame
		spriteComponent->AnimateMovement();

		// check for block
		if (!collisionComp->CanMove(gridComponent->GetEntitiesInCell(m_targetPos))) return;

		dir = glm::normalize(m_targetPos - centeredPos);
		const glm::vec2 offset{ dir * m_speed * TimeManager::GetInstance().GetDeltaTime() };

		// move object
		gameObject->GetTransform()->Translate(offset);

		// update camera when moving
		if (offset.x * offset.x > FLT_EPSILON) if (CameraComponent* cameraComponent = gameObject->GetComponent<CameraComponent>()) cameraComponent->FollowPlayer();
	}
}