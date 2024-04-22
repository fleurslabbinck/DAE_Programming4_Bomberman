#include "Command.h"
#include "Components/RenderComponent.h"
#include "Components/GridComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/BomberComponent.h"
#include "Components/HealthComponent.h"
#include "Components/CameraComponent.h"

namespace dae
{
	//---------------------------------
	// MOVE
	//---------------------------------
	glm::vec2 MoveCommand::m_lastDirection{};

	void MoveCommand::Execute()
	{
		GameObject* gameObject{ GetGameObject() };
		GridComponent* gridComponent{ gameObject->GetParent()->GetComponent<GridComponent>()};
		SpriteComponent* spriteComponent{ gameObject->GetComponent<SpriteComponent>() };
		const CollisionComponent* collisionComp{ gameObject->GetComponent<CollisionComponent>() };

		if (spriteComponent->IsDead()) return;

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

		const float distToTarget{ glm::distance(centeredPos, m_targetPos) };

		// check if reached target
		if (distToTarget <= m_targetOffset)
		{
			m_targetPos = gridComponent->GetNextPosition(centeredPos, m_lastDirection);
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

	//---------------------------------
	// BOMBCOMMAND
	//---------------------------------
	void BombCommand::Execute()
	{
		GameObject* gameObject{ GetGameObject() };
		GridComponent* gridComponent{ gameObject->GetParent()->GetComponent<GridComponent>() };
		const CollisionComponent* collisionComp{ gameObject->GetComponent<CollisionComponent>() };

		// get current pos
		glm::vec2 bottomLeft{ gameObject->GetTransform()->GetLocalPosition() };
		glm::vec2 centeredPos{ bottomLeft + collisionComp->GetCenter() };

		gameObject->GetComponent<BomberComponent>()->DropBomb(gameObject->GetParent(), gameObject->GetComponent<HealthComponent>(), gridComponent->GetGridStartPos(centeredPos));
	}
}