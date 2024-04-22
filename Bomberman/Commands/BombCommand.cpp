#include "BombCommand.h"

#include "../Components/GridComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/BomberComponent.h"
#include "../Components/HealthComponent.h"

namespace dae
{
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