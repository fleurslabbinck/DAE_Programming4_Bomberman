#include "MoveCommand.h"

#include "../Minigin/Objects/Components/RenderComponent.h"
#include "../Minigin/Objects/CollisionManager.h"
#include "../Components/GridComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BomberComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/CameraComponent.h"

namespace dae
{
	//---------------------------------
	// MOVE
	//---------------------------------
	MoveCommand::MoveCommand(dae::GameObject* gameObject, float speed, glm::vec2 direction)
		: GameObjectCommand(gameObject), m_speed { speed }, m_direction{ direction }
	{
		
	}

	void MoveCommand::Execute()
	{
		GameObject* gameObject{ GetGameObject() };
		GridComponent* gridComponent{ gameObject->GetParent()->GetComponent<GridComponent>() };
		SpriteComponent* spriteComponent{ gameObject->GetComponent<SpriteComponent>() };
		const ColliderComponent* colliderComp{ gameObject->GetComponent<ColliderComponent>() };

		if (spriteComponent->IsDead()) return;

		glm::vec2 dir{};

		// get current pos
		glm::vec2 centeredPos{ colliderComp->GetLocalCenter() };

		// check entity collision
		//if (collisionComp->HandleCollision(bottomLeft, gridComponent->GetEntitiesClose(centeredPos))) return;

		glm::vec2 lastDirection{ spriteComponent->GetLastDirection() };

		// reset target position if change of direction
		if (m_direction != lastDirection)
		{
			m_targetPos = centeredPos;
			lastDirection = m_direction;
			spriteComponent->SetDirection(m_direction);
		}

		const float distToTarget{ glm::distance(centeredPos, m_targetPos) };

		// check if reached target
		if (distToTarget <= m_targetOffset)
		{
			m_targetPos = gridComponent->GetNextPosition(centeredPos, lastDirection);
		}

		// set animation frame
		spriteComponent->AnimateMovement();

		// check for block
		if (!CollisionManager::GetInstance().CanMove(colliderComp, m_targetPos)) return;

		dir = glm::normalize(m_targetPos - centeredPos);
		const glm::vec2 offset{ dir * m_speed * TimeManager::GetInstance().GetDeltaTime() };

		// move object
		gameObject->GetTransform()->Translate(offset);

		// update camera when moving
		if (offset.x * offset.x > FLT_EPSILON) if (CameraComponent* cameraComponent = gameObject->GetComponent<CameraComponent>()) cameraComponent->FollowPlayer();
	}
}